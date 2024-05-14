/*E*/
/*Fichier :  @(#)xzav.h	1.1      Release : 1.1        Date : 09/02/12
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER xzav01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       14 Nov 2011     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzav553
#define xzav553

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include "xdg.h"
#include "xdm.h"
#include "xdy.h"
#include "xzsc.h"
#include "xzst.h"

/* definitions de constantes exportees */

#define XZAVC_XZAV01_RPC_NAME "XZAV01"
#define XZAVC_XZAV01_NB_PARM_RETURN 1

#define XZAVC_REG_TRACE_PARAM					1
#define XZAVC_REG_TRACE_FONCTIONNEMENT		2
#define XZAVC_REG_TRACE_DETECTION				3
#define XZAVC_REG_TRACE_AJUSTEMENT			4
#define XZAVC_REG_TRACE_FORCAGE				5
#define XZAVC_REG_TRACE_ALERTE				6
#define XZAVC_REG_TRACE_REVEIL				7
#define XZAVC_REG_TRACE_VALIDATION			8
#define XZAVC_REG_TRACE_APPLICATION			9

#define XZAVC_REG_LON_TRACE		200
/* definitions de types exportes */

typedef char		XZAVT_LibelleTrace[XZAVC_REG_LON_TRACE];   /* Nom (eqt,utilisateur,...) */

typedef struct {
	XDY_Horodate		Horodate;
	XDY_Octet			Type;	/* XZAVC_REG_TRACE_ ...*/
	XZAVT_LibelleTrace	Libelle;
	XDY_Octet			Simulation;
	XDY_Eqt				Zone;
	XDY_Entier			Condition;
	XDY_Entier			Alerte;
	XDY_Entier			Numero_FMC;
	XDY_Octet			Cle_FMC;
	XDY_Eqt				Scenario;
	XDY_Octet			Acquittement;
	XDY_Eqt				station;
	XDY_Sens			sens;
} XZAVT_ConfTraceReg;        




/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAV01_Tracer_Regulation ( XDY_Basedd,
                               XZAVT_ConfTraceReg);

#endif
