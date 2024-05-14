/*E*/
/*Fichier : $Id: xzao861.h,v 1.1 2018/06/19 10:14:13 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/06/19 10:14:13 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao861.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	11/05/18	: Creation GABEB 1.1 DEM1283
------------------------------------------------------ */

#ifndef xzao861
#define xzao861

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO861_RPC_NAME "XZAO861"

#define XZAOC_XZAO861_NB_PARM_RETURN 0

/* definitions de types exportes */

				   
typedef struct {
	XDY_Eqt			NumeroCAP;
	XDY_NomEqt		NomCAP;
	XDY_NomAuto		Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Mot			Type;
	XDY_Rgs			AdresseRgs;
	XDY_AdresseIP		AdresseIP;
	XDY_Port		PortMaitre;
	XDY_Port		PortEsclave;
	XDY_Octet		PointCaracteristique;
	XDY_NomEqt		NomServeur;
	XDY_District		SiteGestion;
} XZAOT_CAP;	


	
/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO861_Lire_Liste_CAP (	XDY_Basedd,
					XDY_NomEqt,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
