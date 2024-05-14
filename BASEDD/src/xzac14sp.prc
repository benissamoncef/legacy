/*E*/
/*  Fichier : $Id: xzac14sp.prc,v 1.11 2020/10/13 17:18:41 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2020/10/13 17:18:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif insertion enregistrement 
*                         et parametres entree
* C.T.	20/09/94	: Modif taille message (non reteste)
* C.T.	28/09/94	: Modif message en " " si null
* B.G.  16/10/94        : Suppression des appels xzah
* C.T.	10/11/94	: Modif bugs applix (V 1.5)
* C.T.	09/12/94	: Modif clause existance action (V 1.6)
*                         non teste
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.7)
* JMG	03/02/98	: ajout sit_origine (dem/1605) 1.8
* JMG	21/09/99	: ajout autoroute
* PNI	02/02/15	: Creation de l'action au sein d'une transaction  1.10
* LCL   25/03/20   : MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
Sequence d'appel
* SP	XZAC14_Ecrire_PMV_Mobile
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XY_Horodate		va_HoroDebut_in
* XY_Horodate		va_HoroFin_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* char(48)		va_Message_in
* XDY_Operateur		va_Operateur_in
* XDy_Autoroute		va_Autoroute_in
*
* Arguments en sortie
* XDY_Entier		va_NumAction_out
* 
* Service rendu
* Ajouter ou modifier une actio sur un PMV mobile
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement ou actio non trouvee, ou insertion impossible
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* En insertion, fournir NumeroEvt seul, en modification, fournir egalement NumeroAction
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer ou modifier dans les tables ACTION et PMV_REMORQUE
* Appeler XZAH12_Animer_Objet_Mobile
-------------------------------------------------------*/

use PRC
go

create procedure XZAC;14
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_NumAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_HoroDebut_in	datetime	= null,
	@va_HoroFin_in		datetime	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Message_in		char(48)	= null,
	@va_Operateur_in	T_OPERATEUR	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_NumAction_out	int		= null	output,
	@va_NomSiteLocal_in	char(2)		= null
as
	declare @vl_Autoroute T_AUTOROUTE, @vl_Distance int, @vl_PREvt T_PR, @vl_Status int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_HoroDebut_in = XDC_DATE_NULLE select @va_HoroDebut_in = null
	if @va_HoroFin_in = XDC_DATE_NULLE select @va_HoroFin_in = null
	if @va_PR_in = XDC_PR_INCONNU select @va_PR_in = null
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_Autoroute_in = 0 select @va_Autoroute_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Operateur_in = null or
	   @va_Site_in = null or @va_PR_in = null or @va_Sens_in = null or
	   @va_HoroDebut_in = null or @va_Autoroute_in = null
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK

	/*A positionner a " " si chaine null */
	if @va_Message_in = null
		select @va_Message_in = XDC_CHAINE_VIDE

	/*A recherche du PR et de l'autoroute de l'evt : derniere horodate de validation */
	select 
		@vl_Autoroute = autoroute,
		@vl_PREvt = PR
	from EXP..FMC_HIS
	where   numero = @va_NumEvt_in and cle = @va_CleEvt_in and 
		horodate_validation in ( select max(horodate_validation) from EXP..FMC_HIS
					where numero = @va_NumEvt_in and cle = @va_CleEvt_in )

	if @@rowcount != 1
		return XDC_NOK

	/*A calculer distance a l'evenement */
	exec @vl_Status = XZAO;22 @va_Autoroute_in, @va_PR_in, @vl_Autoroute, @vl_PREvt, 
				@vl_Distance output

	if @vl_Status != XDC_OK
		select	@vl_Distance = 0

	if @va_NumAction_in = null
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
		select @va_NumAction_out = isnull( max(numero), 0) + 1 from EXP..ACT_GEN
		where sit = @va_Site_in

		/*A inserer une actio de type PMV sur mobile */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		values( @va_NumAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_PMV_REMORQUE, @va_HoroDebut_in,
			@va_HoroDebut_in, null, XDC_NON, null,
			@va_HoroFin_in, @va_Operateur_in, @va_Site_in, @vl_Distance, @va_Site_in)
commit tran
		/*A inserer le complement de l'actio */
		if exists (select * from EXP..ACT_GEN
				where numero = @va_NumAction_out and sit = @va_Site_in)
			insert EXP..ACT_RMQ (actio, PR, sens, message, sit, autoroute)
			values (@va_NumAction_out, @va_PR_in, @va_Sens_in, @va_Message_in, @va_Site_in,
					@va_Autoroute_in)
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

		/*A tester si l'action existe */
		if not exists (select * from EXP..ACT_GEN, EXP..ACT_RMQ
				where	EXP..ACT_GEN.numero = @va_NumAction_in and 
					EXP..ACT_GEN.type = XDC_ACT_PMV_REMORQUE and
					EXP..ACT_GEN.sit = @va_Site_in and
					EXP..ACT_GEN.evenement = @va_NumEvt_in and
					EXP..ACT_GEN.cle = @va_CleEvt_in and
					EXP..ACT_RMQ.actio = @va_NumAction_in and 
					EXP..ACT_RMQ.sit = @va_Site_in)
			return XDC_NOK

		/*A modification d'une actio */
		update EXP..ACT_GEN set 
			heure_fin =  @va_HoroFin_in,
			heure_lancement = @va_HoroDebut_in,
			heure_succes = @va_HoroDebut_in,
			distance_evt = @vl_Distance,
			operateur = @va_Operateur_in
		where numero = @va_NumAction_in and sit = @va_Site_in

		/*A modification du complement d'une actio */
		update EXP..ACT_RMQ set sens = @va_Sens_in, message = @va_Message_in, PR = @va_PR_in, 
					autoroute = @va_Autoroute_in
		where actio = @va_NumAction_in and sit = @va_Site_in

		/*A numero d'actio */
		select @va_NumAction_out = @va_NumAction_in
	end


	return XDC_OK
go
