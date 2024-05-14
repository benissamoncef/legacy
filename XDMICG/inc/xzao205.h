/*E*/
/*Fichier : $Id: xzao205.h,v 1.2 2007/11/19 17:10:36 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/11/19 17:10:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao205.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	06 Nov 1996	: Creation
* JMG   31/10/07        : ajout site de gestion 
------------------------------------------------------ */

#ifndef xzao205
#define xzao205

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO205_RPC_NAME "XZAO;205"

#define XZAOC_XZAO205_NB_PARM_RETURN 14

/* definitions de types exportes */

typedef struct {
	
    XDY_Eqt       	Numero;
    XDY_TypeEqt       	Type;
    XDY_Nom      	LibType;
    XDY_Rgs       	RGS;
    XDY_Octet       	LiaisonMaitre;
    XDY_Octet       	LiaisonEsclave;
    XDY_Octet        	Code;
    XDY_Autoroute	NumeroAutoroute;
    XDY_NomEqt      	NomAutoroute;
    XDY_PR	       	PR;
    XDY_Sens       	Sens;
    XDY_NomEqt      	NomServeur;
    XDY_NomEqt      	NomMachine;
    XDY_District	SiteGestion;
} XZAOT_EQT_LCR;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO205_Lire_Liste_Eqt_Generique_LCR (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
