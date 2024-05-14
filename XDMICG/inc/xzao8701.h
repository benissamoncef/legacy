/*E*/
/*Fichier : $Id: xzao8701.h,v 1.1 2023/08/18 11:00:18 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2023/08/18 11:00:18 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao8701.h
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

#ifndef xzao8701
#define xzao8701

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO8701_RPC_NAME "XZAO8701"

#define XZAOC_XZAO8701_NB_PARM_RETURN 0

/* definitions de types exportes */

 
				   
typedef struct {
	XDY_Eqt			NumeroPIC;
	XDY_Octet		Liaison;
	XDY_Rgs			RGS;
	XDY_NomEqt		NomServeur;
	XDY_District	SiteGestion;
	XDY_AdresseIP	Adresse_IP;
	XDY_Entier		Port;	
} XZAOT_PIC;	


	
/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO8701_Lire_Liste_PIC (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
