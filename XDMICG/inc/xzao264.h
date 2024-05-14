/*E*/
/*Fichier : $Id: xzao264.h,v 1.1 2018/06/19 09:59:25 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/06/19 09:59:25 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao264.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL		26/02/18	: Creation GABEB 1.1 DEM1283
------------------------------------------------------ */

#ifndef xzao264
#define xzao264

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO264_RPC_NAME "XZAO264"

#define XZAOC_XZAO264_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	XDY_NomEqt	NomCAP;
	XDY_Eqt		NumeroCAP;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
} XZAOT_CAPSyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO264_Liste_CAP_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
