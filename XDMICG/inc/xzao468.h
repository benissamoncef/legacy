/*E*/
/* Fichier : $Id: xzao468.h,v 1.1 2012/11/08 09:54:24 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/11/08 09:54:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao468.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05 Juin 2012	: Creation (DEM/1014 PMA)
------------------------------------------------------ */

#ifndef xzao468
#define xzao468

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO468_RPC_NAME "XZAO468"

#define XZAOC_XZAO468_NB_PARM_RETURN 14

/* definitions de types exportes */

typedef struct {

    XDY_Eqt       	Numero;
    XDY_Autoroute	Autoroute;
    XDY_PR	       	PR;
    XDY_Sens       	Sens;
    XDY_NomEqt      	NomMachine;
    XDY_District	SiteGestion;
} XZAOT_PMA_TDP;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO468_Lire_Liste_PMVA_TDP (	XDY_FonctionInt,
					XDY_Entier * );

#endif
