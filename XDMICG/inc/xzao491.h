/*E*/
/*Fichier : $Id: xzao491.h,v 1.2 2021/05/03 13:59:35 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2021/05/03 13:59:35 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao491.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	27/07/20	: Creation IMU 1.1 DEM-SAE155
------------------------------------------------------ */

#ifndef xzao491
#define xzao491

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO491_RPC_NAME "XZAO491"

#define XZAOC_XZAO491_NB_PARM_RETURN 0

/* definitions de types exportes */

				   
typedef struct {
	XDY_Eqt			NumeroIMU;
	XDY_NomEqt		NomIMU;
	XDY_NomAuto		Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Texte		AdresseEndPointOPCUA;
	XDY_Texte		UserOPCUA;
	XDY_Texte		PasswordOPCUA;
	XDY_NomEqt		NomBICE;
	XDY_NomEqt		NomServeur;
	XDY_Port		PortMaitre;
	XDY_Entier		SousType;
	XDY_NomMachine		NomMachine;
	XDY_Mot			SiteGestion;
} XZAOT_IMU_491;


	
/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO491_Lire_Liste_IMU (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
