/*E*/
/*X  Fichier : $Id: xzae43sp.prc,v 1.6 1998/09/22 13:26:23 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/09/22 13:26:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae43sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.6)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante poste operateur
* 
* Sequence d'appel
* SP	XZAE43_Lire_Fiche_Poste_Operateur
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* 
* Arguments en sortie
* XDY_Nom		va_Operateur_out
* 
* Code retour
* XDC_OK
* XDC_NOK     : pas de complement trouve
* XDC_ARG_INV : arguments en entree obligatoires non values
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
* Select dans la table POSTE_OPERATEUR
* where numero evenement and horodate
------------------------------------------------------*/

use PRC
go

create procedure XZAE;43
	@va_NumEvt_in		int,
	@va_CleEvt_in		tinyint,
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_Operateur_out	T_NOM output
as
	declare @vl_Status int, @vl_Operateur smallint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A Controle les arguments en entree obligatoire */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

#ifdef HIST
	select @vl_Operateur = operateur from HIS..FMC_OPR
        where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#else

#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;43
				@va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in, @va_Operateur_out output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		/*B recherche du numero de l'operateur */
		select @vl_Operateur = operateur from EXP..FMC_OPR
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A recherche du numero de l'operateur */
	select @vl_Operateur = operateur from EXP..FMC_OPR
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

#endif

#endif

	if @@rowcount = 0
		return XDC_NOK

	/*A recherche du nom de l'operateur */
	select @va_Operateur_out = nom from CFG..OPR_NOM
	where numero = @vl_Operateur

	if @@rowcount = 0
		return XDC_NOK

	return XDC_OK
go
