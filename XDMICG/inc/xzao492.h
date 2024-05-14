/*E*/
/* Fichier : $Id: xzao492.h,v 1.3 2021/05/03 14:00:21 pc2dpdy Exp $        $Revision: 1.3 $        $Date: 2021/05/03 14:00:21 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao492.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	27/07/2020	: Creation DEM-SAE155 1.1
* ABE	09/02/21	: Ajout  Sous type DEM-SAE155 1.2
------------------------------------------------------ */

#ifndef xzao492
#define xzao492

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO492_RPC_NAME "XZAO492"

#define XZAOC_XZAO492_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_NomEqt	NomIMU;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
	XDY_Texte	AdresseEndPointOPCUA;
   	XDY_Texte	UserOPCUA;
   	XDY_Texte	PasswordOPCUA;
	XDY_NomEqt	NomBICE;
	XDY_NomEqt	NomServeur;
	XDY_NomSite	NomSite;
	XDY_Booleen	Supprime;
	XDY_Port	PortMaitre;
	XDY_Entier	SousType;
	XDY_NomMachine  NomMachine;
} XZAOT_ConfIMU;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO492_Ajouter_IMU (	XDY_Basedd,
					XZAOT_ConfIMU,
					XDY_ResConf *);

#endif
