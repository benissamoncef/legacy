/*E*/
/* Fichier : $Id: xzao407.h,v 1.3 2018/11/07 11:09:11 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/07 11:09:11 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao407.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       10 Fev 1997     : Creation (DEM/1395)
* JPL		26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.2
* JPL		07/11/18 : Procédure dégroupée  1.3
------------------------------------------------------ */

#ifndef xzao407
#define xzao407

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO407_RPC_NAME "XZAO407"

#define XZAOC_XZAO407_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	Nom;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	Type;
   	XDY_NomEqt	NomServeur;
   	XDY_Rgs		Rgs;
   	XDY_Octet	Liaison;
	XDY_Booleen	Supprime;

} XZAOT_ConfEqtUGTP;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO407_Ajouter_Eqt_TDP (	XDY_Basedd,
					XZAOT_ConfEqtUGTP,
					XDY_ResConf * );



#endif
