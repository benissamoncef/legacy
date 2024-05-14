/*E*/
/*  Fichier : $Id: xzag52sp.h,v 1.1 1996/10/10 09:53:00 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/10 09:53:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag52sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	09/10/95	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures peasage de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG28_Purge_Mesures_Pesage_Base_Historique
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
* Suppression des lignes vielles de plus de 5 ans
-------------------------------------------------------*/

 
#define XZAG28_Purge_Mesures_Pesage_Base_Historique	"XZAG;52" 
