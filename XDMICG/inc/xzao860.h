/*E*/
/*Fichier : $Id: xzao860.h,v 1.1 2018/04/18 17:51:26 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/04/18 17:51:26 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao860.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	23/02/18	: CreationCTRL_FEUX 1.1 DEM1284
------------------------------------------------------ */

#ifndef xzao860
#define xzao860

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO860_RPC_NAME "XZAO860"

#define XZAOC_XZAO860_NB_PARM_RETURN 0

/* definitions de types exportes */

 
				   
typedef struct {
	XDY_Eqt			NumeroCFE;
	XDY_AdresseIP		AdresseIP;
	XDY_Port		Port;
	XDY_NomEqt		NomServeur;
	XDY_District		SiteGestion;
	
} XZAOT_CFE;	


	
/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO860_Lire_Liste_CFE (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
