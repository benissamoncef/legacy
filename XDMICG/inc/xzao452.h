/*E*/
/*  Fichier : $Id: xzao452.h,v 1.3 2020/06/10 10:07:54 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2020/06/10 10:07:54 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao452.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JMG   31/10/07        : ajout site de gestion 
* JMG	01/06/19	passage IP DEM1333
------------------------------------------------------ */

#ifndef xzao452
#define xzao452

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO452_RPC_NAME "XZAO452"

#define XZAOC_XZAO452_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroMod;
	XDY_Eqt		NumeroBAF;
	char		Code[3];

} XZAOT_ModuleBAF;

typedef struct {
	
	XDY_Eqt		NumeroBAF;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
	XDY_District	SiteGestion;
	XDY_PortIP	Port;
	XDY_AdresseIP	AdresseIP;
} XZAOT_BAF;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO452_Liste_BAF_Machine (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
