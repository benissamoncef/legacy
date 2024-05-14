/*E*/
/*Fichier : $Id: xzao260.h,v 1.1 2018/05/11 13:03:00 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/05/11 13:03:00 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao260.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL		26/02/18	: Creation CTRL_FEUX 1.1
------------------------------------------------------ */

#ifndef xzao260
#define xzao260

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO260_RPC_NAME "XZAO260"

#define XZAOC_XZAO260_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	XDY_NomEqt	NomCFE;
	XDY_Eqt		NumeroCFE;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
} XZAOT_CFESyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO260_Liste_CFE_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
