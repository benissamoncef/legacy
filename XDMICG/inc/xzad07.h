/*E*/
/* Fichier : $Id: xzad07.h,v 1.1 1997/05/15 09:58:07 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/15 09:58:07 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD * FICHIER xzad07.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	27 Fev 1997	: Creation (DEM/1396)
------------------------------------------------------ */

#ifndef xzad07
#define xzad07

/* fichiers inclus */
#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include "asql_com.h"
#include <xzst.h>
#include <xzsm.h>

/* definitions de constantes exportees */

#define XZADC_XZAD07_RPC_NAME "XZAD07"

#define XZADC_XZAD07_NB_PARM_RETURN 20

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern tg_Connexion  *XZAGV_Connexion;

/* delaration de fonctions externes */

extern int XZAD07_Liste_PMV_TDP_Actif (	XDY_FonctionInt,
					XDY_Entier * );

#endif
