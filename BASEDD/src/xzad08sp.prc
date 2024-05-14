/*E*/
/*  Fichier : $Id: xzad08sp.prc,v 1.2 2018/05/03 13:44:07 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/05/03 13:44:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fournit le liste des franchissements haut
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	24/02/97	: Creation	(dem/tdp) (V 1.1)
* JPL	03/05/18	: Suppression de la procedure avant compilation  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* fournit le liste des franchissement haut
* 
* Sequence d'appel
* SP	XZAD08_Liste_Franchissement_Haut
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* 
* Fonction
* Consultation de la table TDP_FRN
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAD08' and type = 'P')
	drop procedure XZAD08
go


create procedure XZAD08
	
as
	declare  @vl_Retour int


	select @vl_Retour = XDC_OK


	/*A recherche des franchissement sans date de fin */
		select distinct
			zdp
		
		from 	EXP..FRN_TDP 
		where	
			fin = null
	return @vl_Retour
go






