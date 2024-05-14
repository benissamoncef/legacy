/*E*/
/*  Fichier : $Id: xzao140.h,v 1.1 1994/10/05 19:12:23 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:23 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao140.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao140
#define xzao140

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO140_RPC_NAME "XZAO;140"

#define XZAOC_XZAO140_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_CodeEqt	Code;
   	XDY_NomEqt	NomSysVideo;
	XDY_NomEqt	MachineOperateur;
	
} XZAOT_ConfMoniteur;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO140_Ajouter_Moniteur (	XDY_Basedd,
					XZAOT_ConfMoniteur,
					XDY_ResConf *);


#endif
