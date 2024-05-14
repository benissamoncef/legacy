/*E*/
/*  Fichier : $Id: xzao414sp.h,v 1.3 2008/02/13 13:24:04 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2008/02/13 13:24:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao414sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao414sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO414SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
* 	va_Nom_out		T_NOM 		: nom site
* 	va_numero_out		T_SITE		: numero site
* 	va_type_out		tinyint		: type pc
* 	va_serveur_out		char(6)		: nom serveur SQL
* 	va_machine_out		T_NOM_MACHINE 	: nom machine du serveur
*
*
* VALEUR RENDUE :
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
* rechercher les caracteristiques du site
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	10 Dec 1997	: Creation
* JMG	10/07/07 ---------------> plus de point virgule
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao414sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzao414sp.prc	1.1 12/10/97 : xzao414sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

 
#define XZAO414_Info_District	"XZAO414" 
