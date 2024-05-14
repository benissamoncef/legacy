/*E*/
/*  Fichier : $Id: xzao65.h,v 1.1 1994/10/05 19:13:30 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:30 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao65.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao65
#define xzao65

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO65_RPC_NAME "XZAO;65"

#define XZAOC_XZAO65_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomMachine	NomMachine;
   	XDY_NomMachine	NomSuppleant;
   		
} XZAOT_ConfSuppl;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO65_Ajouter_Suppleant ( 	XDY_Basedd,
					XZAOT_ConfSuppl,
					XDY_ResConf *);

#endif
