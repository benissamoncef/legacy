/*E*/
/*Fichier : $Id: xzao418.h,v 1.2 2020/05/19 14:36:29 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/05/19 14:36:29 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao418.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22 Jan 1998	: Creation
* LCL	11/06/19	: PAL/IP DEM1337 1.2
------------------------------------------------------ */

#ifndef xzao418
#define xzao418

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO418_RPC_NAME "XZAO;418"

#define XZAOC_XZAO418_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomPAL;
	XDY_Eqt		NumeroPAL;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
	XDY_Octet	Type;
	XDY_AdresseIP	AdresseIP;
	XDY_Port	Port;
} XZAOT_PALSyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO418_Liste_PAL_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
