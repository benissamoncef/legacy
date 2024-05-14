/*X Fichier : $Id: xzaoexpl.h,v 1.1 1994/10/20 11:32:12 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/20 11:32:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao15.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	17 Aug 1994	: Creation
* volcic	13 Oct 1994	: Modification pour bouchon SAIDEC (v 1.10)
------------------------------------------------------ */

#ifndef xzao15
#define xzao15

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
#define XZAOC_XZAO15_RPC_NAME "XZAO;15"
#define XZAOC_XZAO15_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */
/*extern int XZAO15_PMV_amont();*/
extern int XZAO15_PMV_amont(XDY_Autoroute , 
				XDY_PR , 
				XDY_Sens , 
				XDY_Octet ,
				XDY_Horodate, 
				XDY_Eqt *, 
				XDY_District *, 
				XDY_Autoroute *, 
				XDY_PR *, 
				XDY_Sens *, 
				XDY_Entier * ,
				XDY_Horodate *);




#endif
