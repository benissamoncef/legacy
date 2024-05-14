/*E*/
/*  Fichier : $Id: xzae121sp.prc,v 1.1 1997/05/13 18:00:56 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/13 18:00:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae121sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture des infos associees a un evt
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  20/06/96        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE121_Lect_Ada_syn
*
* Arguments en entree
* char(50)      va_refevt_in
*
* Arguments en sortie
* int 		va_version_out
* datetime	va_horo_out
* char(250)	va_ligne_out   
* char(50)	va_type_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

create procedure XZAE;121
	@va_refevt_in	char(50)
as
	/*A Recherche les infos associees a un evt */
	select version, horodate, ligne, type
	from EXP..ADA_SYN
	where reference = @va_refevt_in

	return XDC_OK

go

