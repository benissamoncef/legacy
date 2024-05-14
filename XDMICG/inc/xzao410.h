/*E*/
/*Fichier : $Id: xzao410.h,v 1.2 2007/11/19 17:10:36 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/11/19 17:10:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao410.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	27 Fev 1997	: Creation (DEM/1395)
* JMG   31/10/07        : ajout site de gestion 
------------------------------------------------------ */

#ifndef xzao410
#define xzao410

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO410_RPC_NAME "XZAO;410"

#define XZAOC_XZAO410_NB_PARM_RETURN 14

/* definitions de types exportes */

typedef struct {
	
    XDY_Eqt       	Numero;
    XDY_Autoroute	Autoroute;
    XDY_PR	       	PR;
    XDY_Sens       	Sens;
    XDY_NomEqt      	NomMachine;
    XDY_District	SiteGestion;
} XZAOT_PMV_TDP;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO410_Lire_Liste_PMV_TDP (	XDY_FonctionInt,
					XDY_Entier * );

#endif
