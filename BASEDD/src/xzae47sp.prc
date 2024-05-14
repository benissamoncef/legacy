/*E*/
/*  Fichier : $Id: xzae47sp.prc,v 1.7 2019/04/01 17:52:16 devgfi Exp $        $Revision: 1.7 $        $Date: 2019/04/01 17:52:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae47sp.prc
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
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.6)
* JPL	15/03/19	: Retour des données validées à l'horodate indiquée (DEM 1327)  1.7
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante manifestation 
* 
* Sequence d'appel
* SP	XZAE47_Lire_Fiche_Manifestation
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XZAET_Manifestation	Description
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
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
* Select dans la table MANIFESTATION
* where numero evenement and horodate
------------------------------------------------------*/

use PRC
go

create procedure XZAE;47
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Nombre_out		smallint = null	output,
	@va_Perception_out	bit = XDC_NON output,
	@va_Violence_out	bit = XDC_NON output
as

	declare @vl_Status int
	
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	if @va_Horodate_in is null select @va_Horodate_in = XDC_DATE_INFINIE

	/*A lecture des infos sur une manifestation */

#ifdef HIST
	select
		@va_Nombre_out		= nombre_de_manifestants,
		@va_Perception_out	= perception_peage,
		@va_Violence_out	= violence
	from HIS..FMC_MNF
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		validation in ( select max(validation) from HIS..FMC_MNF
				where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
					validation <= @va_Horodate_in )
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;47
				@va_Horodate_in	, @va_NumEvt_in, @va_CleEvt_in, @va_Nombre_out output,
				@va_Perception_out output, @va_Violence_out output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		select
			@va_Nombre_out		= nombre_de_manifestants,
			@va_Perception_out	= perception_peage,
			@va_Violence_out	= violence
		from EXP..FMC_MNF
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			validation in ( select max(validation) from EXP..FMC_MNF
					where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
						validation <= @va_Horodate_in )
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		@va_Nombre_out		= nombre_de_manifestants,
		@va_Perception_out	= perception_peage,
		@va_Violence_out	= violence
	from EXP..FMC_MNF
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in and
	validation in ( select max(validation) from EXP..FMC_MNF
			where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
				validation <= @va_Horodate_in )
#endif
#endif

	if @@rowcount = 0
		return XDC_NOK

	return XDC_OK
go
