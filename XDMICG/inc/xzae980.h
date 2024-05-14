/*E*/
/*  Fichier : 	$Id: xzae980.h,v 1.1 2012/05/29 10:14:31 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2012/05/29 10:14:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae980.c
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
#define XZAEC_XZAE980_RPC_NAME "XZAE980"

#define XZAEC_XZAE980_NB_PARM_RETURN 5


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE980SP
*  Retourne les parametres
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE980_Recherche_FMC_Liee (
				XDY_Evt_Commu 	   va_Id_Commu,
				XDY_Synthese_Commu va_synthese,
				XDY_Tendance_Commu va_tendance,
				XDY_Evt 	       *pa_NumEvt,
				XDY_Booleen	       *pa_Encours,
				XDY_Synthese_Commu *pa_Synthese,
				XDY_Tendance_Commu *pa_Tendance);
				

/*
* Arguments en entree
* XDY_Evt_Commu        va_Evt_Commu_in		: evenement communautaire
*
* Arguments en sortie
* XDY_Evt          va_NumEvt : FMC
* XDY_Booleen         va_Encours   : indicateur en cours
* XDY_Tendance_Comm   va_Tendance  : tendance out
* XDY_Synthese_Commu  va_Syntheset  : synthese out
*
* Code retour
* XDC_OK
* XDC_NOK		: pas de FMC liee
* XDC_ARG_INV		: parametres d'entree null
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/
