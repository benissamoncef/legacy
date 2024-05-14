/*E*/
/* Fichier : $Id: xzad10.h,v 1.2 2010/11/30 16:40:12 gesconf Exp $	      Release : $Revision: 1.2 $        Date : $Date: 2010/11/30 16:40:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN  * FICHIER xzad.h
******************************************************
* DESCRIPTION DU MODULE :
* module de calcul des temps de parcours
*
******************************************************
* HISTORIQUE :
*
*  Guilhou  03 fev 1997      : Creation
*  JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.2
*******************************************************/

#ifndef xzad10
#define xzad10

/* fichiers inclus */
#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include <xzst.h>
#include <xzsm.h>

/*definitions de constantes exportees */

/* definitions de types exportes */

/* declaration des donnees globales */


/* declaration des fonctions externes */
extern int XZAD10_Seuil_TDP(            XDY_Eqt         va_zdp,
					XDY_Mot         va_site,
					XDY_Octet       va_typefrn,
					XDY_Horodate    va_horodate);

#endif
