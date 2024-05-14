/*E*/
/*Fichier : $Id: xzao322sp.h,v 1.1 1996/11/25 11:04:59 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/11/25 11:04:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao322sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao322sp.h" */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des types de carburants
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

/* 
* Sequence d'appel
* SP	XZAO322_Lire_Carburants
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Retourne la liste des types de carburants
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Retourne les parametres numero de type, nom du carburant, supprime
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	17 Oct 1996	: Creation
------------------------------------------------------*/
 
#define XZAO322_Lire_Carburants	"XZAO;322" 
