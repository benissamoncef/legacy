/*E*/
/* Fichier : $Id: xzao2571.h,v 1.2 2023/08/18 16:06:46 devgfi Exp $        $Revision: 1.2 $        $Date: 2023/08/18 16:06:46 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao2571.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABK		18/08/23	: Creation (DEM/483)
------------------------------------------------------ */

#ifndef xzao2571
#define xzao2571

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO2571_RPC_NAME "XZAO2571"

#define XZAOC_XZAO2571_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_NomEqt		NomPIC;
   	XDY_NomAuto		Autoroute;
   	XDY_PR			PR;
   	XDY_Sens		Sens;
   	XDY_NomEqt		NomServeur;
   	XDY_Octet		LiaisonPIC;
   	XDY_Rgs			RgsPIC;
	XDY_NomSite		NomSite;
	XDY_Booleen		Supprime;
	XDY_AdresseIP	Adresse_IP;
	XDY_Entier		Port;
} XZAOT_ConfPIC;

/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */
extern int XZAO2571_Ajouter_PIC (	XDY_Basedd,
									XZAOT_ConfPIC,
									XDY_ResConf *);

#endif
