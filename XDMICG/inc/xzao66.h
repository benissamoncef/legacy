/*E*/
/*  Fichier : $Id: xzao66.h,v 1.1 1994/10/05 19:13:32 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:32 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao66.h
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

#ifndef xzao66
#define xzao66

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO66_RPC_NAME "XZAO;66"

#define XZAOC_XZAO66_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomMachine	NomServeur;
   	XDY_NomMachine	NomMaitre;
   		
} XZAOT_ConfST;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO66_Ajouter_Serveur_Terminaux ( 	XDY_Basedd,
						XZAOT_ConfST,
						XDY_ResConf *);

#endif
