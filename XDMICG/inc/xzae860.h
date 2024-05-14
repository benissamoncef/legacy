/*E*/
/*  Fichier : 	$Id: xzae860.h,v 1.1 2009/10/22 15:55:11 pc2dpdy Exp $  Release : $Revision: 1.1 $        Date : $Date: 2009/10/22 15:55:11 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae860.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	30/11/08	creation
------------------------------------------------------*/

#ifndef xzae860
#define xzae860

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP860_RPC_NAME "XZAE860"

#define XZAOC_XZAP860_NB_PARM_RETURN 11

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE860_Lire_Config_Terrain (       XDY_Autoroute   va_NumAutoroute_in,
                                XDY_PR  va_PRDebut_in,
                                XDY_Sens        va_Sens_in,
                                XDY_PR  va_PRFin_in,
                                XDY_Octet       *va_VR_out,
                                XDY_Octet       *va_VM2_out,
                                XDY_Octet       *va_VM1_out,
                                XDY_Octet       *va_VL_out,
                                XDY_Octet       *va_BAU_out,
                                XDY_Octet       *va_VR_I_out,
                                XDY_Octet       *va_VM2_I_out,
                                XDY_Octet       *va_VM1_I_out,
                                XDY_Octet       *va_VL_I_out,
                                XDY_Octet       *va_BAU_I_out,
                                XDY_Octet       *va_Nombre_ZI_out );



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
