/*E*/
/*Fichier : $Id: xzae198.h,v 1.1 2020/07/09 08:37:05 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/07/09 08:37:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE198 * FICHIER XZAE198.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/06/2020	Creation DEM 130 V1.0
------------------------------------------------------ */

#ifndef XZAE198
#define XZAE198

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

/* Include Sybase */
#include <ctpublic.h>
#include <cstypes.h>
	
/* Include applicatif Migrazur */
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAE198C_RPC_NAME "XZAE198"

#define XZAE198C_NB_PARM_RETURN 2


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE198_Infos_obligatoires : recupere les infos 
*  de coordonée en fonction du PR/axe
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE198_Lire_LOC_GPS (     XDY_Autoroute      va_autoroute_in ,
                                        XDY_PR		 va_pr_in,
                                        char*		va_lat_out,
                                        char*		va_lon_out);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_autoroute_in
* XDY_Octet		va_pr_in
*				
* PARAMETRES EN SORTIE :
* XDY_Mot       *va_lat_out
* XDY_Mot      *va_lon_out

* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* - va_pr_in, va_pr_in sont obligatoires
*

* FONCTION :
*
* Extraire les coordonnées de la talbe LOC_GPS
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
