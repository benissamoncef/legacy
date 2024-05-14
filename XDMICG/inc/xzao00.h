/*X Fichier : $Id: xzao00.h,v 1.0 2023/10/03 14:37:49 claudel Exp $      Release : $Revision: 1.0 $        Date : $Date: 2023/10/03 14:37:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao00.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL   03/10/23    : Creation pour CNA
------------------------------------------------------ */

#ifndef xzao00
#define xzao00

/* fichiers inclus */
#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

/*#define XZAOC_XZAO15_RPC_NAME "XZAO"#define XZAOC_XZAO15_NB_PARM_RETURN 0*/
#define XZAOC_XZAO00_RPC_NAME "XZAO00"
#define XZAOC_XZAO00_NB_PARM_RETURN 2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */
/*extern int XZAO15_PMV_amont();*/
extern int XZAO00_SitePrincipal(XDY_Mot *, 
				                XDY_NomSite );




#endif
