/*E*/
/*  Fichier : $Id: xzag51sp.h,v 1.1 1996/10/10 09:52:58 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/10 09:52:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag51sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	09/10/96	: Creation	(dem/1227) (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures pesage de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG20_Purge_Mesures_Pesage_Base_Exploitation
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

 
#define XZAG20_Purge_Mesures_Pesage_Base_Exploitation	"XZAG;51" 
