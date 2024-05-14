/*E*/
/* Fichier : @(#)xzao53sp.h	$Id: xzao53sp.h,v 1.1 1994/10/05 19:45:10 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:45:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzao53sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une District dans la table DISOROUTE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP	XZAO53SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_District_in
* XDY_Abreviation	va_Abreviation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf		va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une autoroute en base
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
* Insertion dans la table RES_DIS (DISTRICT)
*
* MECANISMES :
*
------------------------------------------------------ */

#ifndef xzao53sp
#define xzao53sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

#define XZAO53_Ajouter_District	XZAO;53

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
