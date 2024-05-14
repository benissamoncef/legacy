/*E*/
/*  Fichier : $Id: xzac20sp.prc,v 1.9 2020/10/13 17:21:03 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/10/13 17:21:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/09/94	: Creation (appel a ACRP19 non teste)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.4)
* JMG           02/10/09        : secto DEM887	1.5
* JPL	02/11/09	: Degroupee; preciser au poste TFM l'ajout d'un nouveau message (DEM 917) 1.6
*  JMG           03/03/17 : regio DEM1220
* LCL   25/03/20   : MOVIS Ajout site local DEM-SAE93
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer  un morceau de texte de tfm a une actio deja existante
* 
* Sequence d'appel
* SP	XZAC20_Ecrire_Texte_TFM
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Texte		va_Texte_in
* XDY_Booleen		va_FinTexte_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect ou 
*                 pb a l'appel de XZIT06
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Il faut que l'actio existe deja : utiliser avant XZAC19
* 
* Fonction
* Inserer dans la table ACT_TXT_TFM
* Appelle XZIT06 pour prevenir l'animateur radio 
* quand le texte est fini
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC20' and type = 'P')
	drop procedure XZAC20
go


create procedure XZAC20
	@va_NumAction_in	int	= null,
	@va_Site_in		tinyint	= null,
	@va_Texte_in		T_TEXTE	= null,
	@va_FinTexte_in		bit,
	@va_NomSiteLocal_in	char(2)	= null
as
	declare @vl_Morceau	smallint,
		@vl_Cmd_TFM	tinyint,
		@vl_Status	int

	/*! transfo en param�tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumAction_in = null or @va_Site_in = null or 
	   @va_Texte_in = null
		return XDC_ARG_INV

	/*A tester si le site est correct */
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

	/*A prevenir l'animateur radio si c'est le dernier morceau de texte */
	if @va_FinTexte_in = XDC_OUI
	begin
		select @vl_Cmd_TFM = XDC_AJOUT
		if @va_Site_in = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP19 @vl_Cmd_TFM, @va_NumAction_in, @va_Site_in
		else if @va_Site_in = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP19 @vl_Cmd_TFM, @va_NumAction_in, @va_Site_in
		else if @va_Site_in = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP19 @vl_Cmd_TFM, @va_NumAction_in, @va_Site_in
		else if @va_Site_in = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP19 @vl_Cmd_TFM, @va_NumAction_in, @va_Site_in

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status
	end

	return XDC_OK

go
