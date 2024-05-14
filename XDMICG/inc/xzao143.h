/*E*/
/* Fichier : $Id: xzao143.h,v 1.3 2018/11/06 20:07:04 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/06 20:07:04 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao143.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* JPL		26/10/18 	: Ajout indicateur de suppression (DEM 1305)  1.2
* JPL		06/11/18 	: Procedure degroupee  1.3
* GGY		07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------ */

#ifndef xzao143
#define xzao143

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO143_RPC_NAME "XZAO143"

#define XZAOC_XZAO143_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt		Nom;
   	XDY_NomEqt		NomServeur;
   	XDY_Octet		LiaisonMaitre;
   	XDY_Octet		LiaisonEsclave;
   	XDY_Rgs			RGS;
	XDY_Booleen		Supprime;
	XDY_AdresseIP	AdresseIpEsclave;
	XDY_PortIP		PortIpEsclave;
	XDY_AdresseIP	AdresseIpMaitre;
	XDY_PortIP 		PortIpMaitre;
} XZAOT_ConfPIRAU;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO143_Ajouter_PIRAU (	XDY_Basedd,
					XZAOT_ConfPIRAU,
					XDY_ResConf *);



#endif
