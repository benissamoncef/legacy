/*E*/
/*  Fichier : $Id: xzat23sp.prc,v 1.7 2021/03/08 13:36:50 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2021/03/08 13:36:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat23sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix V 1.2
* C.T.	14/03/95	: Modif longueur texte reveil (V 1.3)
* C.T.	30/01/96	: Modif de la clause d'update de date de fin (V1.4-1.5)
*                         pour mettre a jour uniquement si elle est nulle
* C.P.	08/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.6) (dem / 1536)
* LCL	22/01/21	: MOVIS PRA DEM-SAE93 1.7
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action en base
* 
* Sequence d'appel
* SP	XZAT23_CompteRendu_DemiEch
* 
* Arguments en entree
* XDY_Mot	va_NumAction_in
* XDY_Horodate	va_Horodate_in
* XDY_Booleen	va_ResultatAction_in
*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: action non trouvee ou 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code d'erreur de XZAR03 si pb a son appel
* 
* Conditions d'utilisation
* 
* Fonction
* mise a jour de ACTION et appeler XZAR03 si l'action a echouee
-----------------------------------------------------*/

use PRC
go

create procedure XZAT;23
	@va_NumAction_in	int = null,
	@va_Horodate_in		datetime = null,
	@va_ResultatAction_in	bit
as
	declare @vl_Status int, @vl_Site T_SITE, @vl_NomEqt T_NOM_MACHINE,
		@vl_Operateur T_OPERATEUR, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_TexteReveil char(100), @vl_FinAction bit, @vl_NumEqt T_EQUIPEMENT

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Horodate_in = null
		return XDC_ARG_INV

	/*A initialisation variable locale */
	select @vl_FinAction = XDC_NON

	/*A recherche le site */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 
        /* vl_Site est le site sur lequel s'execute la proc */
        /* En cas de PRA, on doit trouver le site de l'equipement */
        select @vl_Site = sit
        from EXP..ACT_GEN
        where numero=@va_NumAction_in and
              type=XDC_ACT_ECHANGEUR and
              heure_fin is null and
              heure_succes is null and
              heure_echec is null

        if @vl_Site is null
           	return XDC_NOK
	
	/*A test quel compte rendu a t-on ? celui du lancement de */
	/*A la commande ou de la fin de la commande               */
	if @va_NumAction_in < 0
		select @vl_FinAction = XDC_OUI, @va_NumAction_in = - @va_NumAction_in

	/*A verifier existance action et recherche numero equipement */
	select @vl_NumEqt = equipement from EXP..ACT_GEN
	where numero = @va_NumAction_in and sit = @vl_Site and type = XDC_ACT_ECHANGEUR

	if @@rowcount != 1
		return XDC_NOK

	/*A si action a reussi */
	if @va_ResultatAction_in = XDC_OK
	begin
		/*A action en cours */
		if @vl_FinAction = XDC_NON
		begin
			/*A mettre a jour la date de succes */
			update EXP..ACT_GEN set heure_succes = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site 

			if @@rowcount != 1
				return XDC_NOK
		end
		else
		begin
			/*A mettre a jour la date de fin */
			update EXP..ACT_GEN set heure_fin  = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site and
				heure_fin is null
		end

		/*A positionner heure fin des actions precedentes */
		update EXP..ACT_GEN set heure_fin  = @va_Horodate_in
		where	equipement = @vl_NumEqt and type = XDC_EQT_ECHANGEUR and sit = @vl_Site and
			heure_fin is null and numero < @va_NumAction_in
	end

	/*A si l'action a echouee : mettre a jour la date d'echec et prevenir */
	if @va_ResultatAction_in = XDC_NOK
	begin
		/*A action en cours */
		if @vl_FinAction = XDC_NON 
		begin
			/*B mettre a jour la date d'echec */
			/*B et de lancement fin           */
			update EXP..ACT_GEN set 
				heure_echec = @va_Horodate_in,
				heure_lancement_fin = @va_Horodate_in,
				heure_fin = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site 
		
			
			if @@rowcount != 1
				return XDC_NOK
		end

		/*B rechercher les informations sur l'action et l'equipement */
		select
			@vl_NumEvt = EXP..ACT_GEN.evenement,
			@vl_CleEvt = EXP..ACT_GEN.cle,
			@vl_Operateur = EXP..ACT_GEN.operateur,
			@vl_NomEqt = CFG..EQT_GEN.nom
		from EXP..ACT_GEN, CFG..EQT_GEN
		where 	EXP..ACT_GEN.numero = @va_NumAction_in and 
			EXP..ACT_GEN.sit = @vl_Site and
			CFG..EQT_GEN.numero = EXP..ACT_GEN.equipement and
			CFG..EQT_GEN.type = XDC_EQT_ECHANGEUR

		if @@rowcount != 1
			return XDC_NOK

		/*B prevenir */
		select @vl_TexteReveil = XDC_REV_CMD_EQT + @vl_NomEqt
		exec @vl_Status = XZAR;03 @vl_NumEvt, @vl_CleEvt, 
					@vl_Operateur, @vl_TexteReveil
		if @vl_Status != XDC_OK
			return @vl_Status
	end

	return XDC_OK

go
