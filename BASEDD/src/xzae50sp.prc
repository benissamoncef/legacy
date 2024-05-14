/*E*/
/*  Fichier : $Id: xzae50sp.prc,v 1.6 1998/09/22 13:27:27 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/09/22 13:27:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae50.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree
*                         (test ne fonctionne plus)
* C.T.	10/11/94	: Modif bugs applix (V 1.4) 
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.6)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante incident sous concessionnaire
* 
* Sequence d'appel
* SP	XZAE50_Lire_Fiche_Sous_Concess
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* 
* Arguments en sortie
* XDY_Nom	va_Nom_out
* XDY_Booleen	va_Ferme_out
* XDY_Booleen	va_Penurie_out
* XDY_Octet	va_Carburant_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
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
* Select dans la table SS_CONCESSIONNAIRE
* where numero evenement and horodate
------------------------------------------------------*/

use PRC
go

create procedure XZAE;50
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_Nom_out		T_NOM = null	output,
	@va_Ferme_out		bit = XDC_NON	output,
	@va_Penurie_out		bit = XDC_NON	output,
	@va_Carburant_out	tinyint = null	output
as
	declare @vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/* test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A lecture des infos sur le sous concessionnaire */
#ifdef HIST
	select
		@va_Nom_out		= nom_ss_concessionnaire,
		@va_Ferme_out		= ferme,
		@va_Penurie_out		= penurie,
		@va_Carburant_out	= carburant
	from HIS..FMC_CNC
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;50
				@va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in, @va_Nom_out output,
				@va_Ferme_out output, @va_Penurie_out output, @va_Carburant_out output	
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		select
			@va_Nom_out		= nom_ss_concessionnaire,
			@va_Ferme_out		= ferme,
			@va_Penurie_out		= penurie,
			@va_Carburant_out	= carburant
		from EXP..FMC_CNC
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		@va_Nom_out		= nom_ss_concessionnaire,
		@va_Ferme_out		= ferme,
		@va_Penurie_out		= penurie,
		@va_Carburant_out	= carburant
	from EXP..FMC_CNC
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#endif
#endif

	/*A test si les infos ont ete trouvees */
	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
