/*E Fichier : @(#)acli_XZEA_XZAS_FU.h	1.1      Release : 1.1        Date : 12/16/94
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_FctUtil_XZEA_XZAS * FICHIER ACLI_FctUtil_XZEA_XZAS.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Contient les fonctions utilisateurs de TACLI qui sont passees
* aux fonction de EQUEST (XZEA*) et qui permet d appeler les fonctions
* XZAS de mise en base des donnees de mesures 6mn, mesure Poids, 
* Niveau de services
****************************
* 	acli_XZEA03_XZAS10_FU est la fonction utilisateur appelee
*	par xzea03 . Elle permet a TACLI de mettre en base les mesures 6mn.
****************************
* 	acli_XZEA04_XZAS10_FU est la fonction utilisateur appelee
*	par xzea04 . Elle permet a TACLI de mettre en base les mesures 6mn.
****************************
* 	acli_XZEA05_XZAS12_FU est la fonction utilisateur appelee
*	par xzea05 . Elle permet a TACLI de mettre en base les niveaux de services.
****************************
* 
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef ACLI_FctUtil_XZEA_XZAS
#define ACLI_FctUtil_XZEA_XZAS

/* fichiers inclus */
	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
		/* sybase*/
	
	#include 	"cstypes.h"
	#include	"ctpublic.h"
	
	/* includes applicatifs Migrazur */
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xzag.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include	"asql_com.h"
	#include	"xzea.h"
	#include	"xzas10.h"
	#include	"xzas11.h"
	#include	"xzas12.h"
	
/*
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void  acli_XZEA03_XZAS10_FU (XZEAT_EMes_RADT ,	int *);
extern void  acli_XZEA04_XZAS10_FU (XZEAT_EMes_RADT ,	int *);
extern void  acli_XZEA05_XZAS12_FU (XDY_ENiveau_Service,int *);
extern void  acli_XZEA07_XZAS11_FU ( XDY_EMes_Poids ,	int *);
#endif
