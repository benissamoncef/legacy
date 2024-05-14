/*E*/
/*  Fichier : @(#)xzac200sp.prc	1.4      Release : 1.4        Date : 01/07/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac200sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.Hilmarcher 29/10/03: Creation
* JMG	22/01/19	: calcul CI 1.2
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
* CGR	06/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.4
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer  un morceau de texte de tfm a une actio deja existante
* 
* Sequence d'appel
* SP	XZAC200_Texte_TFM
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Texte		va_Texte_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect  
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Il faut que l'actio existe deja : utiliser d'abord la proc  XZAC190
* 
* Fonction
* Inserer dans la table ACT_TXT_TFM
-------------------------------------------------------*/

use PRC
go

drop procedure XZAC200
go

create procedure XZAC200
	@va_NumAction_in	int	= null,
	@va_Site_in		tinyint	= null,
	@va_Texte_in		T_TEXTE	= null,
	@va_NomSiteLocal_in	char(2)	= null
as
	declare @vl_Morceau 	smallint,
		@vl_Status 	int,
		@vl_Site 	tinyint,
		@vl_serveur_pra char(6)

	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero
	        from CFG..RES_DIS
	        where serveur = @@servername
	else
		select @vl_Site = numero
	        from CFG..RES_DIS
		where code = @va_NomSiteLocal_in

	if (@vl_Site != XDC_CI) and (@vl_Site != XDC_CA)
	/* Traitement au CI pour Escota, en local sinon */
	begin
		/* Init serveur PRA */
		select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=@va_Site_in
		
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC200
                                @va_NumAction_in, XDC_CI, @va_Texte_in,"CI"
		else
			exec @vl_Status = SQL_CI.PRC..XZAC200
				@va_NumAction_in, XDC_CI, @va_Texte_in,"CI"
		return @vl_Status
	end

	/*! transfo en param�tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumAction_in = null or @va_Site_in = null or 
	   @va_Texte_in = null
		return XDC_ARG_INV

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

	/*A verifier l'existence de l'actio */
	if not exists ( select * from EXP..ACT_TFM
			where actio = @va_NumAction_in and sit = @va_Site_in)
		return XDC_NOK

	/*A recherche du numero de morceau suivant */
	select @vl_Morceau = isnull(max(morceau), 0) + 1 
	from EXP..ACT_TXT_TFM
	where actio = @va_NumAction_in and sit = @va_Site_in

	/*B inserer le texte du tfm */
	insert EXP..ACT_TXT_TFM (actio, sit, morceau, texte)
	values (@va_NumAction_in, @va_Site_in, @vl_Morceau, @va_Texte_in)


	return XDC_OK

go
