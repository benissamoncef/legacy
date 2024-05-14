/*E*/
/*  Fichier : @(#)xzac10sp.prc	1.5      Release : 1.5        Date : 01/07/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	05/09/94	: Creation
* C.T.	13/09/94	: Rajout du test sur le site
*                         et du param d'entree TypeAstreinte
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	17/12/94	: Corection appel a XZAC;11  (V1.4)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.5)
* JMG   21/11/07        : ajout envoi de mail DEM/718 1.8
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC10_Ajouter_Dest_Fax
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* XDY_Entier	va_Interlocuteur_in
* XDY_Octet	va_TypeAstreinte_in
* XDY_Tel	va_NumeroTalFax_in
* XDY_Horodate	va_HoroLancement_in
* XDY_Operateur	va_Operateur_in
* 
* Arguments en sortie
* XDY_Entier	va_NumAction_out
* 
* Service rendu
* Inserer une actio d'envoi d'un fax deja envoye a un autre destinataire avce XZAC09.
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Verifier l'existence de l'actio et du fax lie a l'actio passee en entree
* Appeler XZAC11
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAC;10
	@va_NumAction_in	int		= null,
	@va_Site_in		tinyint		= null,
	@va_Interlocuteur_in	smallint	= null,
	@va_TypeAstreinte_in	tinyint		= null,
	@va_NumeroTelFax_in	char(100)	= null,
	@va_HoroLancement_in	datetime	= null,
	@va_Operateur_in	T_OPERATEUR	= null,
	@va_NumAction_out	int		= null	output,
	@va_NomSiteLocal_in	char(2)		= null
as
	declare @vl_CleEvt tinyint, @vl_NumEvt int, @vl_Status int, @vl_texte char(250)

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_Interlocuteur_in = 0 select @va_Interlocuteur_in = null
	if @va_TypeAstreinte_in = 0 select @va_TypeAstreinte_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_HoroLancement_in = XDC_DATE_NULLE select @va_HoroLancement_in = null

	/*A Controle les paraletres obligatoires */
	if @va_NumAction_in = null or @va_Site_in = null or
	   @va_Interlocuteur_in = null or @va_NumeroTelFax_in = null or
	   @va_HoroLancement_in = null or  @va_Operateur_in = null
		return XDC_ARG_INV

	/*A tester si le site est correct */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
	begin
		if not exists ( select numero from CFG..RES_DIS
				where serveur = @@servername and type != XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK
	end
	else
	begin
		if not exists ( select numero from CFG..RES_DIS
				where code = @va_NomSiteLocal_in and type != XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK
	end

	/*A rechercher l'evenement cause de l'actio */
	/*A avec verification de l'existance de l'actio et du fax */
	select 
		@vl_NumEvt = EXP..ACT_GEN.evenement, 
		@vl_CleEvt = EXP..ACT_GEN.cle
	from EXP..ACT_GEN, EXP..ACT_FAX
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_FAX.actio = EXP..ACT_GEN.numero and 
		EXP..ACT_FAX.sit = EXP..ACT_GEN.sit and
		EXP..ACT_FAX.morceau = 1

	/*A test si actio trouvee */
	if @@rowcount != 1
		return XDC_NOK

	/*A inserer le nouveau destinataire */
	/*! attention : nouvelle actio creee */
	exec @vl_Status = XZAC;11	@vl_NumEvt, 
					@vl_CleEvt, 
					@va_Interlocuteur_in,
					@va_TypeAstreinte_in,
					null,
					@va_NumeroTelFax_in, 
					XDC_ACT_FAX,
					@va_HoroLancement_in, 
					@va_Operateur_in,
					null,
					null,
					@va_NumAction_out output,
					0,
					@va_NomSiteLocal_in

	if @vl_Status != XDC_OK
		return @vl_Status


	/*A mise ajour du numero d'actio fax */
	update EXP..ACT_APL set action_fax = @va_NumAction_in
	where actio = @va_NumAction_out and sit = @va_Site_in

	return XDC_OK
go
