/*E*/
/*  Fichier : $Id: xzae122sp.prc,v 1.2 1997/12/01 15:36:14 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/01 15:36:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae122sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture de la liste des evts
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
* SP   XZAE122_Lect_Ada_evt
*
* Arguments en entree
*
* Arguments en sortie
* char(50)      va_refevt_out
* datetime	va_horo_out
* char(50)	va_type_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture de la liste des evts        
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE122' and type = 'P')
	drop procedure XZAE122
go

create procedure XZAE122
as
	/*A Recherche les infos associees a un evt */
	select distinct a.reference, a.horodate, a.type
	from EXP..ADA_SYN a
	having a.horodate in 
		(select min(b.horodate) from EXP..ADA_SYN b
		 where b.reference=a.reference)

	return XDC_OK

go

