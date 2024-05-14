/*E*/
/*Fichier : $Id: xzao422.h,v 1.3 2020/06/15 08:46:59 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/06/15 08:46:59 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao422.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22 Jan 1998	: Creation
* JMG   31/10/07        : ajout site de gestion 
* JMG	14/06/19 	: IP 1.3 DEM1337
------------------------------------------------------ */

#ifndef xzao422
#define xzao422

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO422_RPC_NAME "XZAO;422"

#define XZAOC_XZAO422_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroPAL;
	XDY_Octet	NumeroType;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_District	SiteGestion;	
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	Port;
} XZAOT_PAL;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO422_Liste_PAL_Machine (	XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
