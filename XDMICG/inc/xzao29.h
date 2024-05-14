/*E*/
/*  Fichier : $Id: xzao29.h,v 1.1 2013/02/11 16:05:21 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/02/11 16:05:21 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao29.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Retourne si la zone est hors réseau Escota
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* ABE   17/10/2022 : Creation DEM-SAE318
*
-----------------------------------------------------------------------------------------*/


#ifndef XZAO29_H
#define XZAO29_H


/* Fichiers inclus */

#include "xzao123.h"



/* Definitions de constantes */



/* Definitions de types exportes */



/* Declaration de donnees globales */



/* Definition de fonctions externes */


int XZAO29_calcul_zone_hors_reseau (XDY_Autoroute		va_autoroute_dest,
			XDY_PR				va_pr_dest,
			XDY_Sens			va_sens_o,
			XDY_Sens			va_sens_d,
			XDY_PR				*va_pr_limite,
            XDY_Autoroute     *va_autoroute_limite,
            XDY_Sens			   *va_sens_limite);

#endif
