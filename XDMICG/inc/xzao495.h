/*E*/
/* Fichier : $Id: xzao495.h,v 1.1 2020/11/04 08:20:44 pc2dpdy Exp $        $Revision: 1.1 $        $Date: 2020/11/04 08:20:44 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao495.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	02/10/2020	: Creation DEM-SAE155 1.1
------------------------------------------------------ */

#ifndef xzao495
#define xzao495

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO495_RPC_NAME "XZAO495"

#define XZAOC_XZAO495_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_Eqt		NumeroEqt;
	XDY_Entier	Id;
	XDY_Texte	Libelle;
} XZAOT_Conf_Zon_IMU;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO495_Ajouter_Zon_IMU (	XDY_Basedd,
					XZAOT_Conf_Zon_IMU,
					XDY_ResConf *);

#endif
