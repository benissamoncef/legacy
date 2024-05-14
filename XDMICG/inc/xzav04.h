/*E*/
/*Fichier :  @(#)xzav.h	1.1      Release : 1.1        Date : 09/02/12
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER xzav04.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       14 Nov 2011     : Creation GABEB (DEM/1283)
------------------------------------------------------ */

#ifndef xzav04
#define xzav04

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include "xdg.h"
#include "xdm.h"
#include "xdy.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzav01.h"

/* definitions de constantes exportees */

#define XZAVC_XZAV04_RPC_NAME "XZAV04"
#define XZAVC_XZAV04_NB_PARM_RETURN 1

typedef struct {
	XDY_Horodate		Horodate;
	XDY_Octet			Type;	/* XZAVC_REG_TRACE_ ...*/
	XZAVT_LibelleTrace	Libelle;
	XDY_Eqt				Echangeur;
	XDY_Octet			Cab1;
	XDY_Entier                      Alerte;
	XDY_Entier			Numero_FMC;
	XDY_Octet			Cle_FMC;
	XDY_Octet                       Acquittement;
	XDY_Octet			Contexte;
} XZAVT_ConfTraceCAB;        




/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAV04_Tracer_GABEB ( 
                               XZAVT_ConfTraceCAB);

#endif
