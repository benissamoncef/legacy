/*E*/
/* Fichier : $Id: xzao402.h,v 1.2 2011/06/22 10:47:49 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/06/22 10:47:49 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao402.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	10 Fev 1997	: Creation (DEM/1395)
* JMG		22/06/11	: suppression ; dans XZAO;402
------------------------------------------------------ */

#ifndef xzao402
#define xzao402

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO402_RPC_NAME "XZAO402"

#define XZAOC_XZAO402_NB_PARM_RETURN 14

/* definitions de types exportes */

typedef struct {
	
    XDY_Eqt       	Numero;
    XDY_Nom      	LibType;
    XDY_Rgs       	RGS;
    XDY_Octet       	Liaison;
    XDY_Autoroute	NumeroAutoroute;
    XDY_NomEqt      	NomAutoroute;
    XDY_PR	       	PR;
    XDY_Sens       	Sens;
    XDY_NomEqt      	NomServeur;
    XDY_NomEqt      	NomMachine;

} XZAOT_EQT_TDP;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO402_Lire_Liste_Eqt_TDP (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
