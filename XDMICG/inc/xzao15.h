/*X Fichier : $Id: xzao15.h,v 1.12 1995/01/17 14:37:49 torregrossa Exp $      Release : $Revision: 1.12 $        Date : $Date: 1995/01/17 14:37:49 $
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
* C.T.          13 Jan 1995     : Ajout de la date en entree (V1.25)
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
#define XZAOC_XZAO15_NB_PARM_RETURN 6

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
				XDY_Mot ,
				XDY_Mot *, 
				XDY_Octet *, 
				XDY_Autoroute *, 
				XDY_PR *, 
				XDY_Sens *, 
				XDY_Entier *,
				XDY_Horodate);




#endif
