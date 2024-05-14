/*E*/
/*  Fichier : @(#)xzae556sp.prc	1.25      Release : 1.25        Date : 02/05/08
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae556sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/835
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE556_Bouchon_Conseq
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* 
* Arguments en sortie
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
---------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE556' and type = 'P')
	drop procedure XZAE556
go

create procedure XZAE556
	@va_type_in		tinyint = null,
	@va_autoroute_in	tinyint	= null,
	@va_PR_in		int = null,
	@va_sens_in		tinyint	= null,
	@va_distance_in		int = null,
	@va_presence_out	int output
as
		declare  @vl_nb int

	/*A Controle le parametre obligatoire */
	if (@va_type_in is null or @va_autoroute_in is null or 
	    @va_PR_in is null or @va_sens_in is null or @va_distance_in is null) 
		return XDC_ARG_INV


	/*! cas ou la procedure est creee au CI */

	/*A construire la liste des evenements et  des actions */
	if @va_sens_in = XDC_SENS_SUD
	begin
	select @vl_nb=count(*) from EXP..FMC_GEN g, EXP..FMC_HIS_DER h
	where type = @va_type_in
	and debut is not null
	and fin is null
	and g.numero=h.numero
	and g.cle = h.cle
	and h.autoroute  = @va_autoroute_in
	and h.sens = @va_sens_in
	and h.PR >= @va_PR_in
	and h.PR <= @va_PR_in + @va_distance_in
	end
	else
	begin
	select @vl_nb=count(*) from EXP..FMC_GEN g, EXP..FMC_HIS_DER h
	where type = @va_type_in
	and debut is not null
	and fin is null
	and g.numero=h.numero
	and g.cle = h.cle
	and h.autoroute  = @va_autoroute_in
	and h.sens = @va_sens_in
	and h.PR <= @va_PR_in
	and h.PR >= @va_PR_in - @va_distance_in
	end
	select @va_presence_out = @vl_nb
	return XDC_OK
go
