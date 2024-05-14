/*E*/
/* Fichier : $Id: xzao265.h,v 1.2 2018/11/05 13:07:39 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/11/05 13:07:39 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao265.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	11/05/2018	: Creation (GABEB) DEM1283
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.2
------------------------------------------------------ */

#ifndef xzao265
#define xzao265

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO265_RPC_NAME "XZAO265"

#define XZAOC_XZAO265_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_NomEqt	NomCAP;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
	XDY_Mot		Type;
	XDY_Rgs		AdresseRgs;
   	XDY_AdresseIP	AdresseIP;
   	int		PortMaitre;
	int		PortEsclave;
	XDY_Octet	PointCaracteristique;
	XDY_NomEqt	NomServeur;
	XDY_NomSite	NomSite;
	XDY_Booleen	Supprime;
} XZAOT_ConfCAP;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO265_Ajouter_CAP (	XDY_Basedd,
					XZAOT_ConfCAP,
					XDY_ResConf *);

#endif
