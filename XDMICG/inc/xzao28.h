/*E*/
/*  Fichier : $Id: xzao28.h,v 1.1 2013/02/11 16:05:21 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/02/11 16:05:21 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao28.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Retourne les donnees d'une autoroute, a partir de son numero ou de son nom.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	08/02/13 : Creation.  1.1
*
-----------------------------------------------------------------------------------------*/


#ifndef XZAO28_H
#define XZAO28_H


/* Fichiers inclus */

#include "xzao123.h"



/* Definitions de constantes */



/* Definitions de types exportes */



/* Declaration de donnees globales */



/* Definition de fonctions externes */

extern int	XZAO28_Autoroute ( XDY_Autoroute va_numero,
                        XDY_NomAuto va_nom,
                        XZAOT_Autoroute *pa_autoroute );


#endif
