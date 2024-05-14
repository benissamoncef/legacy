/*E*/
/* Fichier : $Id: xzao421.h,v 1.5 2020/05/19 12:12:24 pc2dpdy Exp $        $Revision: 1.5 $        $Date: 2020/05/19 12:12:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao421.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.		21 Jan 1998	: Creation (1545) (v1.1)
* JMG   31/10/07        : ajout site de gestion 
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.3
* JPL	07/11/18 : Procédure dégroupée  1.4
* LCL	09/06/19	: PAL/IP DEM1337 1.5
------------------------------------------------------ */

#ifndef xzao421
#define xzao421

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO421_RPC_NAME "XZAO421"

#define XZAOC_XZAO421_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomPAL;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
        XDY_NomSite     NomSite;        
	XDY_Booleen	Supprime;
        XDY_PortIP      Port;
        XDY_AdresseIP   AdresseIP;
	
} XZAOT_AjoutPAL;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO421_Ajouter_PAL (XDY_Basedd,
				XZAOT_AjoutPAL,
				XDY_ResConf *);



#endif
