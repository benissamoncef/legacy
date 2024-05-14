/*E*/
/*  Fichier : $Id: xzac17sp.prc,v 1.12 2020/10/13 17:18:55 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2020/10/13 17:18:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif du parametre en entree
*                         HoroLancFin par HoroFin
* C.T.	28/09/94	: Modif NomGare en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
* C.T.	10/11/94	: Modif bugs applix (V 1.5)
* C.T.	09/12/94	: Modif clause existance action (V 1.6)
*                         non teste
* C.T.	14/03/95	: Modif longueur champ affichette applix (V 1.7)
* C.T.	05/12/95	: Autoriser les echangeurs comme peage (V 1.8)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.9)
* JMG	02/03/98	: ajout sit_origine (dem/1605) 1.10
* PNI	02/02/15	: Creation de l'action au sein d'une transaction  1.11
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
* Inserer ou modifier l'actio d'afficher dans une gare de peage
* 
* Sequence d'appel
* SP	XZAC17_Ecrire_Affichage_Gare
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomGare_in
* XDY_Horodate		va_HoroAffDebut_in
* XDY_Horodate		va_HoroAffFin_in
* char(250)		va_Affichette_in
* XY_Horodate		va_HoroLancement_in
* XY_Horodate		va_HoroFin_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroSucces_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evnement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio et le site sont non values en entree 
* alors l'actio est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement et de la gare de peage
* Inserer dans les tables ACTION, AFFICHAGE_EN_GARE
* Appeler XZAH12_Animer_Objet_Mobile
----------------------------------------------------*/

use PRC
go

create procedure XZAC;17
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_NumeroAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_NomGare_in		T_NOM		= null,
	@va_HoroAffDebut_in	datetime	= null,
	@va_HoroAffFin_in	datetime	= null,
	@va_Affichette_in	char(250)	= null,
	@va_HoroLancement_in	datetime	= null,
	@va_HoroFin_in		datetime	= null,
	@va_Operateur_in	T_OPERATEUR	= null,
	@va_HoroSucces_in	datetime	= null,
	@va_NumeroAction_out	int		= null	output,
	@va_NomSiteLocal_in	char(2)		= null
as
	declare @vl_Autoroute T_AUTOROUTE, @vl_Sens T_SENS, @vl_PR T_PR, @vl_Status int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_HoroAffDebut_in = XDC_DATE_NULLE select @va_HoroAffDebut_in = null
	if @va_HoroAffFin_in = XDC_DATE_NULLE select @va_HoroAffFin_in = null
	if @va_HoroLancement_in = XDC_DATE_NULLE select @va_HoroLancement_in = null
	if @va_HoroFin_in = XDC_DATE_NULLE select @va_HoroFin_in = null
	if @va_HoroSucces_in = XDC_DATE_NULLE select @va_HoroSucces_in = null
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Operateur_in = null or
	   @va_HoroLancement_in = null
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)

		return XDC_NOK

	/*A verifier l'existence de la gare */
	if @va_NomGare_in != null
	begin
		if not exists (select * from CFG..RES_PEA where nom = @va_NomGare_in) and
		   not exists (select * from CFG..RES_ECH where nom = @va_NomGare_in)
			return XDC_NOK
	end
	else	select @va_NomGare_in = XDC_CHAINE_VIDE

	/*A recherche de l'autoroute, sens et PR : derniere horodate de validation */
	select 
		@vl_Autoroute = autoroute,
		@vl_Sens = sens,
		@vl_PR = PR
	from EXP..FMC_HIS
	where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and 
		horodate_validation in ( select max(horodate_validation) from EXP..FMC_HIS
					where numero = @va_NumEvt_in and cle = @va_CleEvt_in )

	if @@rowcount != 1
		return XDC_NOK

	if @va_NumeroAction_in = null
	begin
		/*A recherche le site */
		/* MOVIS */
		if @va_NomSiteLocal_in = null
			select @va_Site_in = numero 
        		from CFG..RES_DIS
        		where serveur = @@servername
		else
			select @va_Site_in = numero 
        		from CFG..RES_DIS
			where code = @va_NomSiteLocal_in

        	if @va_Site_in is null
                	return XDC_NOK

begin tran
		/*A recherche du numero d'actio suivant */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 
		from EXP..ACT_GEN
		where sit = @va_Site_in

		/*A inserer une actio de type PMV sur fourgon */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		values( @va_NumeroAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_AFF_GARE, @va_HoroLancement_in,
			@va_HoroSucces_in, null, XDC_NON, null,
			@va_HoroFin_in, @va_Operateur_in, @va_Site_in, null, @va_Site_in)
commit tran
		/*A inserer le complement de l'actio */
		if exists (	select * from EXP..ACT_GEN
				where numero = @va_NumeroAction_out and sit = @va_Site_in)
			insert EXP..ACT_GAR (	actio, nom_gare, horodate_debut, affichette, 
						horodate_fin, sit)
			values (@va_NumeroAction_out, @va_NomGare_in, @va_HoroAffDebut_in, 
				@va_Affichette_in, @va_HoroAffFin_in, @va_Site_in)
		else	return XDC_NOK
	end
	else
	begin
		/*A tester si le site est correct */
		/* MOVIS */
		if @va_NomSiteLocal_in = null
		begin
			if not exists ( select numero from CFG..RES_DIS
                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
			return XDC_NOK
		end
		else
		begin
			if not exists ( select numero from CFG..RES_DIS
                                where code = @va_NomSiteLocal_in and type <> XDC_TYPE_PCNIVEAU3 )
			return XDC_NOK
		end

		/*A tester si l'actio existe */
		if not exists (select * from EXP..ACT_GEN, EXP..ACT_GAR
				where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
					EXP..ACT_GEN.type = XDC_ACT_AFF_GARE and
					EXP..ACT_GEN.sit = @va_Site_in and
					EXP..ACT_GEN.evenement = @va_NumEvt_in and
					EXP..ACT_GEN.cle = @va_CleEvt_in and
					EXP..ACT_GAR.actio = @va_NumeroAction_in and 
					EXP..ACT_GAR.sit = @va_Site_in)
			return XDC_NOK

		/*A modification d'une actio */
		update EXP..ACT_GEN 
			set heure_fin =  @va_HoroFin_in, 
			heure_succes =  @va_HoroSucces_in,
			heure_lancement = @va_HoroLancement_in,
			operateur = @va_Operateur_in
		where numero = @va_NumeroAction_in and sit = @va_Site_in

		/*A modification du complement d'une actio */
		update EXP..ACT_GAR set 
			nom_gare = @va_NomGare_in, 
			horodate_debut = @va_HoroAffDebut_in,
			horodate_fin = @va_HoroAffFin_in,
			affichette = @va_Affichette_in
		where actio = @va_NumeroAction_in and sit = @va_Site_in

		/*A numero d'actio */
		select @va_NumeroAction_out = @va_NumeroAction_in
	end


	return XDC_OK
go
