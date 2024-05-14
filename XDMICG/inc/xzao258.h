/*E*/
/* Fichier : $Id: xzao258.h,v 1.2 2018/10/26 16:20:10 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/10/26 16:20:10 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao258.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	06/02/2018	: Creation (CTRL_FEUX) DEM1284
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.2
------------------------------------------------------ */

#ifndef xzao258
#define xzao258

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO258_RPC_NAME "XZAO258"

#define XZAOC_XZAO258_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_NomEqt	NomCFE;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_AdresseIP	AdresseIP;
   	int	Port;
	XDY_NomEqt	NomServeur;
	XDY_NomSite	NomSite;
	XDY_Booleen	Supprime;
} XZAOT_ConfCFE;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO258_Ajouter_CFE (	XDY_Basedd,
					XZAOT_ConfCFE,
					XDY_ResConf *);

#endif
