/*E*/
/*Fichier : $Id: itre_son.h,v 1.5 2007/03/26 14:23:17 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2007/03/26 14:23:17 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_son.h				*
*************************************************
* DESCRIPTION :
* module generateur de sons pour les alertes;
* a partir de 

(c)Copyright 1992 Hewlett-Packard Co.,  All Rights Reserved.


 * player - command-line audio file player
 *   Aug. 28 1991
 *    by three unknown, unsung audio programmers
 *     (well, only two are unsung)

*************************************************
* HISTORIQUE :
* Niepceron 06 Jul 1995 : Creation
* Niepceron 07 Jul 1995 : Ajout de sons v1.4
*************************************************
*/


#ifndef MISO
#define MISO

#include <stdlib.h>
#include <stdio.h>

/*
#include <audio/Alib.h>
#include "itre_utilson.h"
*/

#include "xzst.h"
#include "xdc.h"


#define ITRE_SON_BASE	0

#define ITRE_SON_DRING		1
#define ITRE_SON_CARILLON	2
#define ITRE_SON_3DRINGS	3
#define ITRE_SON_TADA		4

#define ITRE_FIC_SONBASE "tada.wav"

#define ITRE_FIC_SONTADA 	"tada.wav"
#define ITRE_FIC_SONDRING	"dring.wav"
#define ITRE_FIC_SONCARILLON	"carillon.wav"
#define ITRE_FIC_SON3DRINGS	"3drings.wav"

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  	Lance le type de son pass≈ en arg vers les enceintes externes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int itr_LanceSon( int   va_num_son );

/*
* ARGUMENTS EN ENTREE :
*   va_num_son : numero du type de son a lancer
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/


#endif
