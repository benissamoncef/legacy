/*E*/
/*  Fichier : 	$Id: xzae982.h,v 1.1 2012/05/29 10:15:43 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2012/05/29 10:15:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae982.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU       14/02/12     : DEM 1015 - Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */
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
#define XZAEC_XZAE982_RPC_NAME "XZAE982"

#define XZAEC_XZAE982_NB_PARM_RETURN 1

