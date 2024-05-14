/*E Fichier : $Id: xzag15.h,v 1.4 1997/05/15 09:58:26 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/05/15 09:58:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAG05 * FICHIER XZAG15.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	21 Nov 1995	: Creation
* Torregrossa	09 Sep 1996	: Modif inetrface de la fonction 
*                                 XZAG_Purge_Base_Exploitation pour
*                                 ajouter le type de pc (RADT 1.2)
* Niepceron	09 Oct 1996	: Ajout de XZAG51 (dem/1227) v1.3
* Niepceron     27 Fev 1997	: Ajout de XZAG54 (dem/1396) v1.4
------------------------------------------------------ */

#ifndef XZAG15
#define XZAG15

/* fichiers inclus */
	/* Include systeme */
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
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAG10C_RPC_NAME "XZAG;10"
#define XZAG15C_RPC_NAME "XZAG;15"
#define XZAG16C_RPC_NAME "XZAG;16"
#define XZAG17C_RPC_NAME "XZAG;17"
#define XZAG18C_RPC_NAME "XZAG;18"
#define XZAG19C_RPC_NAME "XZAG;19"
#define XZAG20C_RPC_NAME "XZAG;20"
#define XZAG21C_RPC_NAME "XZAG;21"
#define XZAG22C_RPC_NAME "XZAG;22"
#define XZAG51C_RPC_NAME "XZAG;51"
#define XZAG54C_RPC_NAME "XZAG;54"

#define XZAG10C_NB_PARM_RETURN 1
#define XZAG15C_NB_PARM_RETURN 0
#define XZAG16C_NB_PARM_RETURN 0
#define XZAG17C_NB_PARM_RETURN 0
#define XZAG18C_NB_PARM_RETURN 0
#define XZAG19C_NB_PARM_RETURN 0
#define XZAG20C_NB_PARM_RETURN 0
#define XZAG21C_NB_PARM_RETURN 0
#define XZAG22C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAG15_Purge_Evts_Base_Exploitation (XDY_Horodate, XDY_Booleen *);
extern int XZAG_Purge_Morceau (CS_CHAR *, XDY_Octet, char *, CS_DATETIME *);
extern int XZAG_Purge_Base_Exploitation (XDY_Horodate, XDY_Entier);
#endif
