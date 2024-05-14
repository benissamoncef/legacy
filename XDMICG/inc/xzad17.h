/*E*/
/* Fichier : $Id: xzad17.h,v 1.1 2005/09/13 18:11:01 gesconf Exp $    Release : $Revision: 1.1 $       Date : $Date: 2005/09/13 18:11:01 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD * FICHIER xzad17.h
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

#ifndef xzad17
#define xzad17

/* fichiers inclus */
#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include "asql_com.h"
#include <xzst.h>
#include <xzsm.h>

/* definitions de constantes exportees */

#define XZADC_XZAD17_RPC_NAME "XZAD17"

#define XZADC_XZAD17_NB_PARM_RETURN 20

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern tg_Connexion  *XZAGV_Connexion;

/* delaration de fonctions externes */

extern int XZAD17_Liste_PMVA_TDP_Actif (	XDY_FonctionInt,
					XDY_Entier * );

#endif
