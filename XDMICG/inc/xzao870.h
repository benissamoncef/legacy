/*E*/
/*Fichier : $Id: xzao870.h,v 1.1 2012/07/04 11:00:18 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:00:18 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao870.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
* ABE	31/05/21	: PRV suur IP DEM-SAE283 1.3
------------------------------------------------------ */

#ifndef xzao870
#define xzao870

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO870_RPC_NAME "XZAO870"

#define XZAOC_XZAO870_NB_PARM_RETURN 0

/* definitions de types exportes */

 
				   
typedef struct {
	XDY_Eqt			NumeroPRV;
	XDY_Octet		Liaison;
	XDY_Rgs			RGS;
	XDY_NomEqt		NomServeur;
	XDY_Entier		Type;
	XDY_Entier		Vitesse_nominale;
	XDY_Entier		Zone;
	XDY_Octet		Critique;
	XDY_Octet		NumeroTypePRV;
	XDY_District	SiteGestion;
	XDY_AdresseIP	Adresse_IP;
	XDY_Entier	Port;	
} XZAOT_PRV;	


	
/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO870_Lire_Liste_PRV (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
