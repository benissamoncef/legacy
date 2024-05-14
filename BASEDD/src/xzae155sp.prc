/*E*/
/*  Fichier : $Id: xzae155sp.prc,v 1.2 2020/12/07 12:13:43 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/12/07 12:13:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae155sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Fournit les commentaires non perime
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	05 Mar 1999 : creation
* PNI	30/11/20 : Plus de transmission de commentaires dans les passerelles
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE155_LireCommentaires
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE155
go

create procedure XZAE155
	@va_NumEvt_in 		int=null,
	@va_CleEvt_in		tinyint
	
as

#ifndef HIST
	select texte
	from EXP..FMC_COM
	where evenement=@va_NumEvt_in and cle=@va_CleEvt_in and horodate_fin = null and texte like "%CONDITION%METEO%"
	order by numero
#endif
	
	return XDC_OK
go

