/*E*/
/*  Fichier : @(#)xzac109sp.prc	1.1    Release : 1.1        Date : 12/13/07
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	31/10/07	creation
* LCL   25/03/20   : MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer une actio d'envoi de mail a un destinataire (uniquement pour les mail 
* "actio", c'est a dire envoyes pour l'exploitation de l'autoroute, et lies a 
* une FMC).
* 
* Sequence d'appel
* SP	XZAC109_Ajouter_Mail
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Texte		Texte
* XDY_Entier		va_Interlocuteur_in
* XDY_Octet		va_TypeAstreinte_in
* XDY_Tel		va_NumeroTalMail_in
* XDY_Horodate		va_HoroLancement_in
* XDY_Operateur		va_Operateur_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Utiliser pour le premier destinataire d'un mail. Les autres 
* destinataires sont ajoutes avec XZAC10.
* Utiliser XZAC54 pour ajouter du texte au mail dans le cas 
* ou il y a plus de 250 caracteres 
* 
* Fonction
* Appeler XZAC11 pour ajouter le destinataire
* Inserer dans la table TEXTE_MAIL
-------------------------------------------------------*/

use PRC
go

create procedure XZAC109
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Texte_in		T_TEXTE		= null,
	@va_Interlocuteur_in	smallint	= null,
	@va_TypeAstreinte_in	tinyint		= null,
	@va_AdresseMail_in	char(100)	= null,
	@va_HoroLancement_in	datetime	= null,
	@va_Operateur_in	T_OPERATEUR	= null,
	@va_NumAction_out	int		= null	output,
	@va_NomSiteLocal_in            char(2)         = null
as
	declare @vl_Site tinyint, 
		@vl_Status int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_TypeAstreinte_in = 0 select @va_TypeAstreinte_in = null
	if @va_Interlocuteur_in = 0 select @va_Interlocuteur_in = null
	if @va_HoroLancement_in = XDC_DATE_NULLE select @va_HoroLancement_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or 
	   @va_Interlocuteur_in = null or @va_AdresseMail_in = null or
	   @va_HoroLancement_in = null or
	   @va_Texte_in = null or @va_Operateur_in = null
		return XDC_ARG_INV


	/*A inserer la nouvelle actio */
	exec @vl_Status = XZAC111	@va_NumEvt_in, 
					@va_CleEvt_in, 
					@va_Interlocuteur_in,
					@va_TypeAstreinte_in,
					null,
					@va_AdresseMail_in, 
					XDC_ACT_MAIL,
					@va_HoroLancement_in, 
					@va_Operateur_in,
					null,
					null,
					@va_NumAction_out output

	if @vl_Status != XDC_OK
		return @vl_Status

	/*A recherche du site */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero 
        	from CFG..RES_DIS
        	where serveur = @@servername
	else
		select @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in


        if @vl_Site is null
                return XDC_NOK


	/*B inserer le texte du mail */
	insert EXP..ACT_MAIL (actio, sit, morceau, texte)
	values (@va_NumAction_out, @vl_Site, 1, @va_Texte_in)

	return XDC_OK

go
