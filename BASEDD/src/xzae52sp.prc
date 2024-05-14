/*E*/
/*  Fichier : $Id: xzae52sp.prc,v 1.7 1998/09/22 13:27:30 gaborit Exp $      Release : $Revision: 1.7 $        Date : $Date: 1998/09/22 13:27:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae52.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/09/94	: Modif position param entree
*                         test ne fonctionne plus
* C.T.	10/11/94	: Modif bugs applix (V 1.4) 
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.7)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante fermeture echangeur
* 
* Sequence d'appel
* SP	XZAE52_Lire_Fiche_Fermeture_Echangeur
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Booleen	va_Entree1_out
* XDY_Booleen	va_Entree2_out
* XDY_Booleen	va_Sortie1_out
* XDY_Booleen	va_Sortie2_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
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
* Select dans la table FERMETURE_ECHANGEUR
* where numero evenement and horodate
----------------------------------------------------*/

use PRC
go

create procedure XZAE;52
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Entree1_out		bit = XDC_NON	output,
	@va_Entree2_out		bit = XDC_NON	output,
	@va_Sortie1_out		bit = XDC_NON	output,
	@va_Sortie2_out		bit = XDC_NON	output
as
	declare @vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A lecture des infos de fermeture d'echangeur */
#ifdef HIST
	select
		@va_Entree1_out		= entree1,
		@va_Entree2_out		= entree2,
		@va_Sortie1_out		= sortie1,
		@va_Sortie2_out		= sortie2
	from	HIS..FMC_ECH
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;52
				@va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in, @va_Entree1_out output,
				@va_Entree2_out output, @va_Sortie1_out output, @va_Sortie2_out output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end 
	else
		select
			@va_Entree1_out		= entree1,
			@va_Entree2_out		= entree2,
			@va_Sortie1_out		= sortie1,
			@va_Sortie2_out		= sortie2
		from	EXP..FMC_ECH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		@va_Entree1_out		= entree1,
		@va_Entree2_out		= entree2,
		@va_Sortie1_out		= sortie1,
		@va_Sortie2_out		= sortie2
	from	EXP..FMC_ECH
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 
#endif
#endif

	/*A test si les infos ont ete trouvees */
	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
