/*E*/
/*  Fichier : $Id: xzao142.h,v 1.1 1994/10/05 19:12:27 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:27 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao142.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao142
#define xzao142

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO142_RPC_NAME "XZAO;142"

#define XZAOC_XZAO142_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		RGS;
   	XDY_NomEqt	MachineOperateur;
   	XDY_Tel		NumeroRepondeur;
   	
} XZAOT_ConfMAA;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO142_Ajouter_MAA (	XDY_Basedd,
					XZAOT_ConfMAA,
					XDY_ResConf *);


#endif
