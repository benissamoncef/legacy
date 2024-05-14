/*E*/
/* Fichier : $Id: xzao257.h,v 1.2 2018/10/26 16:06:46 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/10/26 16:06:46 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao257.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.2
* ABE	31/05/21	: PRV suur IP DEM-SAE283 1.3
------------------------------------------------------ */

#ifndef xzao257
#define xzao257

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO257_RPC_NAME "XZAO257"

#define XZAOC_XZAO257_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_NomEqt	NomPRV;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	LiaisonPRV;
   	XDY_Octet	TypePRV;
   	XDY_Rgs		RgsPRV;
	XDY_NomSite	NomSite;
	XDY_Mot		VitesseNominale;
	char 		NomZone[50];
	XDY_Booleen	Critique;
	XDY_Booleen	BretelleEntree;
	XDY_Booleen	RappelNominal;
	XDY_Booleen	Supprime;
	XDY_AdresseIP	Adresse_IP;
	XDY_Entier	Port;
} XZAOT_ConfPRV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO257_Ajouter_PRV (	XDY_Basedd,
					XZAOT_ConfPRV,
					XDY_ResConf *);

#endif
