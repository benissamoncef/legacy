/*E*/
/* Fichier : $Id: xzao80.h,v 1.6 2020/11/03 18:04:36 pc2dpdy Exp $        $Revision: 1.6 $        $Date: 2020/11/03 18:04:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao80.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* JMG   31/10/07        : ajout site de gestion
* JPL	26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.3
* JPL	06/11/18 : Procédure dégroupée  1.4
* LCL	11/06/19 : DAI/IP DEM1333 1.5
* CGR	11/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.6
------------------------------------------------------ */

#ifndef xzao80
#define xzao80

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO80_RPC_NAME "XZAO80"

#define XZAOC_XZAO80_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	NomDAI;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	LiaisonMaitre;
   	XDY_Octet	LiaisonEsclave;
   	XDY_Rgs		Rgs; 
	XDY_NomSite	NomSite;   	
	XDY_Booleen	Supprime;
	XDY_AdresseIP	Adresse_IP;
	XDY_Port	Port;
	XDY_AdresseIP	Adresse_IPE;
	XDY_Port	PortE;
	XDY_AdresseIP	Adresse2_IP;
	XDY_Port	Port2;
	XDY_AdresseIP	Adresse2_IPE;
	XDY_Port	Port2E;
} XZAOT_ConfDAI;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO80_Ajouter_DAI (	XDY_Basedd,
				XZAOT_ConfDAI,
				XDY_ResConf *);

#endif
