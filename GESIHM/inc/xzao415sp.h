/*E*/
/*  Fichier : $Id: xzao415sp.h,v 1.1 1998/01/21 14:07:31 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/01/21 14:07:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao415sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao415sp.h >
*
------------------------------------------------------
*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des districts
* 
* Sequence d'appel
* SP	XZAO415_ListeDistricts
* 
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
*
* XZAOT_District	Liste
*
* VALEUR RENDUE :
*
* Retourne la liste des districts
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
* Select * de la table RES_DIS (DISTRICT)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	19 Dec 1997	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao415sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzao415sp.prc	1.1 08/25/94 : xzao415sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

 
#define XZAO415_ListeDistricts	"XZAO;415" 
