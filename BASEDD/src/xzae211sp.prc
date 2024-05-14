/*E*/
/*  Fichier : $Id: xzae211sp.prc,v 1.2 2019/09/12 17:24:37 devgfi Exp $        $Revision: 1.2 $        $Date: 2019/09/12 17:24:37 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae211sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des fiches Main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	30/08/12 : Creation (DEM 1037)  1.1
* JPL	05/09/19 : Retour du code de nature de l'incendie (DEM 1352)  1.2
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne les champs d'une fiche Main courante de classe Incendie.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAE211_Lire_Fiche_Incendie
*
* Arguments en entree :
*
* XDY_Horodate		va_Horodate_in	: par defaut heure courante (non utilisee)
* XDY_Entier		va_NumEvt_in	: numero de l'evenement
* XDY_Octet		va_CleEvt_in	: cle de l'evenement
*
* Arguments en sortie :
* XDY_Octet		va_Feu_Maitrise_out
* XDY_Horodate		va_Date_Maitrise_out
* XDY_Octet		va_Code_Nature_out
*
*
* Code retour :
*
* XDC_OK
* XDC_NOK           : pas de complement de fiche main courante
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* Conditions d'utilisation :
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
* Fonction :
*
*  Select dans la table INCENDIE
*  where numero evenement and horodate
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE211' and type = 'P')
	drop procedure XZAE211
go


create procedure XZAE211
	@va_Horodate_in		datetime	= null,
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Feu_Maitrise_out	tinyint		= null	output,
	@va_Date_Maitrise_out	datetime	= null	output,
	@va_Code_Nature_out	tinyint		= null	output
as

declare	@vl_Status	int


	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumEvt_in = 0			select @va_NumEvt_in = null
	if @va_CleEvt_in = 0			select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE	select @va_Horodate_in = null


	/*A Controle de validite des parametres d'entree obligatoires */
	if @va_NumEvt_in is null or @va_CleEvt_in is null
		return XDC_ARG_INV


	/*A lecture des infos sur l'incendie */
#ifdef HIST
	select	@va_Feu_Maitrise_out	= feu_maitrise,
		@va_Date_Maitrise_out	= horodate_maitrise,
		@va_Code_Nature_out	= code_nature
	from	HIS..FMC_INC
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE211
				@va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
				@va_Feu_Maitrise_out output,
				@va_Date_Maitrise_out output,
				@va_Code_Nature_out output
			if @vl_Status = null
				return XDC_PRC_INC
			else return @vl_Status
		end
	else
		select
			@va_Feu_Maitrise_out	= feu_maitrise,
			@va_Date_Maitrise_out	= horodate_maitrise,
			@va_Code_Nature_out	= code_nature
		from	EXP..FMC_INC
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		@va_Feu_Maitrise_out	= feu_maitrise,
		@va_Date_Maitrise_out	= horodate_maitrise,
		@va_Code_Nature_out	= code_nature
	from	EXP..FMC_INC
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in
#endif
#endif

	/*A test si les infos ont ete trouvees */
	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
