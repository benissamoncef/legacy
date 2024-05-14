/*	Fichier : 	$Id: xzae861.h
------------------------------------------------------
* GTIE	*	PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae861.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	18/01/24	:	Creation (DEM-483)
------------------------------------------------------*/
#ifndef xzae861
#define xzae861

/* fichiers inclus */
#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAEC_XZAE861_RPC_NAME "XZAE861"
#define XZAEC_XZAE861_NB_PARM_RETURN 1

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

extern int XZAE861_Basculement_Tunnel (	XDY_Autoroute	va_NumAutoroute_in,
										XDY_PR			va_PRDebut_in,
										XDY_PR			va_PRFin_in,
										XDY_Booleen		*va_BasculementTunnel_out);

extern tg_Connexion	*XZAGV_Connexion;
extern tg_Context	* XZAGV_Context;

#endif
