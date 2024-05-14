#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaoc.h"
/*E*/
/* Fichier : $Id: xzao913sp.prc,v 1.2 2021/04/12 06:06:57 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2021/04/12 06:06:57 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao913sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Tracage des basculements du controle d'une zone RAU au CI.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	19/04/17 : Creation  1.1
* PNI	12/04/21 : insert dans EXP et non HIS SAE-263 1.2
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Si l'etat indique en entree indique un basculement au CI alors
* cree une nouvelle entree de basculement a la date courante pour la zone
* sinon marque la fin du dernier basculement en cours pour la zone.
*
* RESTRICTIONS :
*  - Destinee a etre executee en base HIStorique uniquement.
*
* -----------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAO913_Tracer_Basculement_RAU
*
* Arguments en entree
* char(30)		va_Zone_in
* char(5)		va_Etat_in
*
* Arguments en sortie
*  Aucun
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase
*
* Conditions d'utilisation
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO913' and type = 'P')
	drop procedure XZAO913
go


create procedure XZAO913
	@va_Zone_in		char(30),
	@va_Etat_in		char(5)
as

declare	@vl_Debut_Dernier_Bascul	datetime,
	@vl_Fin_Dernier_Bascul		datetime


	/*A Controle des parametres d'entree obligatoire */
	if @va_Zone_in = XDC_CHAINE_VIDE
		return XDC_ARG_INV

	if ( @va_Etat_in <> "LOC" )  and  ( @va_Etat_in <> "CI" )
		return XDC_ARG_INV


	/*A Si l'etat indique est un retour a la normale (mode LOCAL) */
	/*A alors marquer la fin du basculement en cours pour la zone */
	if upper (@va_Etat_in) = "LOC"
	begin
		select	@vl_Debut_Dernier_Bascul = debut,
			@vl_Fin_Dernier_Bascul = fin
		from	EXP..BAS_RAU_TRA
		where	zone = @va_Zone_in
		having	zone = @va_Zone_in
		   and	debut = max (debut)

		/*B S'il n'y a aucun basculement pour cette zone ... */
		if @@rowcount = 0
			return XDC_NOK

		/*B Si le dernier basculement est deja termine alors c'est une erreur */
		if @vl_Fin_Dernier_Bascul is not null
			return XDC_NOK

		update	EXP..BAS_RAU_TRA
		set	fin = getdate()
		where	zone = @va_Zone_in  and  debut = @vl_Debut_Dernier_Bascul
	end


	/*A Si l'etat indique est un basculement au CI          */
	/*A alors memoriser le nouveau basculement pour la zone */
	if upper (@va_Etat_in) = "CI"
	begin
		insert EXP..BAS_RAU_TRA (zone, debut, fin)
		values (@va_Zone_in, getdate(), NULL)
	end

	return XDC_OK
go
