/*E*/
/*Fichier : $Id: xzao77.h,v 1.4 1995/08/08 15:53:30 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/08/08 15:53:30 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao77.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	27 Dec 1994	: Modification de NumeroAlarme et NumeroAlerte (v1.3)
* volcic	08 Aou 1995	: Ajout XZAOC_NB_ETOR (v1.4)
------------------------------------------------------ */

#ifndef xzao77
#define xzao77

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO77_RPC_NAME "XZAO;77"

#define XZAOC_XZAO77_NB_PARM_RETURN 1

#define XZAOC_NB_ETOR	XDC_NB_ETOR

/* definitions de types exportes */

typedef struct {

        XDY_Mot		NumeroAlarme;
   	XDY_Octet	NumeroAlerte;
   	XDY_Eqt		NumeroEqt;
   	XDY_NomEqt	NomETOR;
   	
} XZAOT_LisETOR;

typedef struct {

   	XDY_Eqt		NumeroMiniGTC;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		Rgs;
   	XZAOT_LisETOR	ListeETOR[XDC_NBMAX_ETOR];
   	
} XZAOT_MiniGTC;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO77_Lire_Liste_Mini_GTC (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
