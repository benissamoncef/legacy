/*E*/
/* Fichier : @(#)xzao51sp.h	$Id: xzao51sp.h,v 1.1 1994/10/05 19:45:08 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:45:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzao51sp.h
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
*  Ajout d'une Autoroute dans la table AUTOROUTE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP	XZAO51SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_Autoroute_in
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
* Insertion dans la table RES_AUT (AUTOROUTE)
*
* MECANISMES :
*
------------------------------------------------------ */

#ifndef xzao51sp
#define xzao51sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

#define XZAO51_Ajouter_Autoroute	XZAO;51

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
