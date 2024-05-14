/*E*/
/*  Fichier : @(#)xzae46sp.prc	1.8      Release : 1.8        Date : 10/09/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae46sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* B.G.	17/01/95	: Modif date nulle (V 1.6)
* C.T.	14/06/95	: Correction erreur nom base au CI (V 1.5)
* C.T.	27/09/96	: Modif interface pour ajout de colonne
*                         largeur,hauteur,poids,contenu (FMC6) (V1.6)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.9)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante vehicule lent a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE46_Lire_Fiche_VehLent
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante
* 
* Arguments en sortie
* XDY_Mot	va_Longueur_out
* XDY_Octet	va_Vitesse_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans la table VEHICULE_LENT
* where numero evenement and horodate
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAE;46
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_Longueur_out	smallint = null	output,
	@va_Vitesse_out		tinyint = null output,
	@va_Hauteur_out		smallint = null output,
	@va_Largeur_out		smallint = null output,
	@va_Poids_out		smallint = null output,
	@va_Contenu_out		char(25) = null output
as

	declare @vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date servant a la recherche du complement de la fiche main courante */
	if @va_Horodate_in = null
	begin
		select @va_Horodate_in = XDC_DATE_INFINIE
	end

	
	/*A lecture de la vitesse et de la longueur du vehicule lent */
#ifdef HIST
	select
		@va_Longueur_out	= longueur,
		@va_Vitesse_out		= vitesse,
		@va_Hauteur_out		= hauteur,
		@va_Largeur_out		= largeur,
		@va_Poids_out		= poids,
		@va_Contenu_out		= contenu
	from HIS..FMC_VLN
	where	numero = @va_NumEvt_in and 
		cle = @va_CleEvt_in and 
		horodate in 
		(select max(horodate) from HIS..FMC_VLN
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and 
			horodate <= @va_Horodate_in) 

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;46
					@va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in, @va_Longueur_out output,
					@va_Vitesse_out output, @va_Hauteur_out output, @va_Largeur_out output, @va_Poids_out output, @va_Contenu_out output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		select
			@va_Longueur_out	= longueur,
			@va_Vitesse_out		= vitesse,
			@va_Hauteur_out		= hauteur,
			@va_Largeur_out		= largeur,
			@va_Poids_out		= poids,
			@va_Contenu_out		= contenu
		from EXP..FMC_VLN
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and 
			horodate in 
			(select max(horodate) from EXP..FMC_VLN
			where	numero = @va_NumEvt_in and 
				cle = @va_CleEvt_in and 
				horodate <= @va_Horodate_in) 
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		@va_Longueur_out	= longueur,
		@va_Vitesse_out		= vitesse,
		@va_Hauteur_out		= hauteur,
		@va_Largeur_out		= largeur,
		@va_Poids_out		= poids,
		@va_Contenu_out		= contenu
	from EXP..FMC_VLN
	where	EXP..FMC_VLN.numero = @va_NumEvt_in and 
		EXP..FMC_VLN.cle = @va_CleEvt_in and 
		EXP..FMC_VLN.horodate in 
			(select max(horodate) from EXP..FMC_VLN
			where	EXP..FMC_VLN.numero = @va_NumEvt_in and 
				EXP..FMC_VLN.cle = @va_CleEvt_in and 
				EXP..FMC_VLN.horodate <= @va_Horodate_in) 
#endif
#endif

	/*A test si une fiche a ete trouvee sinon erreur */
	if @@rowcount = 0
		return XDC_NOK

	return XDC_OK
go
