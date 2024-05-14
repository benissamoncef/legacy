/*E*/
/* Fichier : $Id: xzao720.h,v 1.6 2019/01/30 11:56:31 pc2dpdy Exp $        $Revision: 1.6 $        $Date: 2019/01/30 11:56:31 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao720.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
* JMG   31/10/07        : ajout site de gestion 
* LCL	02/10/18	: Suppression SAGA DEM1306 1.3
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.5
* JMG	29/01/19	: ajout type 1.6
------------------------------------------------------ */

#ifndef xzao720
#define xzao720

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO720_RPC_NAME "XZAO720"

#define XZAOC_XZAO720_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {

   	XDY_NomEqt	NomPMVA;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	LiaisonPMVA;
   	XDY_Rgs		RgsPMVA;
	XDY_NomSite	NomSite;
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	Port;
	char		IdLCR[50];
	XDY_Booleen	Supprime;
   	XDY_Octet	Type;
} XZAOT_ConfPMVA;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO720_Ajouter_PMVA (	XDY_Basedd,
					XZAOT_ConfPMVA,
					XDY_ResConf *);

#endif
