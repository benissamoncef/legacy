/*E*/
/*Fichier : $Id: xzao208.h,v 1.2 1995/01/25 11:21:27 mercier Exp $    Release : $Revision: 1.2 $        Date : $Date: 1995/01/25 11:21:27 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao208.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
**********
* mercier	25 Jan 1995	: Modif id keywords	V 1.2
------------------------------------------------------ */

#ifndef xzao208
#define xzao208

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO208_RPC_NAME "XZAO;208"

#define XZAOC_XZAO208_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumCapteur;
	XDY_NomEqt	NomCapteur;
	XDY_Octet	Autoroute;
	XDY_Entier	PR;
	XDY_Sens	Sens;

} XZAOT_Capteur;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO208_Liste_Capteurs (	XDY_Basedd,
					XDY_NomEqt,
					XDY_Octet,
					XDY_FonctionInt );

#endif
