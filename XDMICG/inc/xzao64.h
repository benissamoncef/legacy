/*E*/
/*  Fichier : $Id: xzao64.h,v 1.1 1994/10/05 19:13:28 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:28 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao64.h
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

#ifndef xzao64
#define xzao64

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO64_RPC_NAME "XZAO;64"

#define XZAOC_XZAO64_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomMachine	NomMachine;
   	XDY_Nom		NomDistrict;
   		
} XZAOT_ConfMachine;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO64_Ajouter_Machine ( 	XDY_Basedd,
					XZAOT_ConfMachine,
					XDY_ResConf *);

#endif
