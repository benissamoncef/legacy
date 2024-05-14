/*E*/
/*Fichier : @(#)xzap11sp.h	1.2      Release : 1.2        Date : 09/28/05
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap11sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	13 Oct 1994	: Creation
* JMG	29/08/05	ajout PMVA/BAF/BAD 1.2
* VR    15/12/11  : ajout PRV (DEM/1016)
* LCL	15/03/18  : ajout controleur de feux CTRL_FEUX 1.4 DEM1284
------------------------------------------------------ */

#ifndef xzap11sp
#define xzap11sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

#define XZAPC_TypePMV		1
#define XZAPC_TypeNAV		2
#define XZAPC_TypeTube		3
#define XZAPC_TypeEchangeur	4
#define XZAPC_TypeTFM		5
#define XZAPC_TypeFAX		6
#define XZAPC_TypeAppel		7

#define XZAPC_TypePMVA         9
#define XZAPC_TypeBAF				10
#define XZAPC_TypeBAD				11
#define XZAPC_TypePRV				12
#define XZAPC_TypeCFE				13

#define XZAP11_Lire_PA	XZAP;11

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
*
* ARGUMENTS EN SORTIE :
*
* XZAPT_PropPA 		va_PropPA_out
*/

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
