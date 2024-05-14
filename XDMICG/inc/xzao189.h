/*E*/
/*  Fichier : $Id: xzao189.h,v 1.2 2016/07/13 17:51:40 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2016/07/13 17:51:40 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao189.h
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

#ifndef xzao189
#define xzao189

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO189_RPC_NAME "XZAO;189"

#define XZAOC_XZAO189_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		Nom;
	XDY_Mot		NumType;
	XDY_Booleen	Critique;
	XDY_Booleen	Majeure;
	XDY_Booleen	Mineure;

} XZAOT_ConfTypeAlarme;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO189_Ajouter_Type_Alarme (	XDY_Basedd,
						XZAOT_ConfTypeAlarme,
						XDY_ResConf * );



#endif
