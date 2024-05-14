/*E*/
/* Fichier : @(#)xzad80.h	1.1    Release : 1.1       Date : 09/13/05
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD * FICHIER xzad80.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzad80
#define xzad80

/* fichiers inclus */
#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include "asql_com.h"
#include <xzst.h>
#include <xzsm.h>

/* definitions de constantes exportees */

#define XZADC_XZAD80_RPC_NAME "XZAD80"

#define XZADC_XZAD80_NB_PARM_RETURN 20

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern tg_Connexion  *XZAGV_Connexion;

/* delaration de fonctions externes */

extern int XZAD80_Liste_PRV_TDP_Actif (	XDY_FonctionInt,
					XDY_Entier * );

#endif
