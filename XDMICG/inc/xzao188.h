/*E*/
/*  Fichier : $Id: xzao188.h,v 1.2 2016/07/13 17:51:46 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2016/07/13 17:51:46 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao188.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	05 Nov 1996	: Creation
* JPL		13/07/2016	: Declaration du nom de la fonction conformement a son utilisation  1.2
------------------------------------------------------ */

#ifndef xzao188
#define xzao188

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO188_RPC_NAME "XZAO;188"

#define XZAOC_XZAO188_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		Nom;
	XDY_Mot		NumType;

} XZAOT_ConfTypeAlerte;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO188_Ajouter_Type_Alerte (	XDY_Basedd,
						XZAOT_ConfTypeAlerte,
						XDY_ResConf * );



#endif
