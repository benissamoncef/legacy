/*E*/
/*Fichier :  @(#)xzao563.h	1.1      Release : 1.1        Date : 15/03/12
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao563.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       15 Mars 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao563
#define xzao563

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO563_RPC_NAME "XZAO563"

#define XZAOC_XZAO563_NB_PARM_RETURN 1 

/* definitions de types exportes */

typedef struct {

	XDY_Eqt	Station;
	XDY_Eqt	Zone;
	XDY_NomAuto	Autoroute;
	XDY_PR	PR;
	XDY_Sens	Sens;
	XDY_Nom	Nom;

} XZAOT_Comptage;


/* delaration de fonctions */
extern int XZAO563_Comptage(XDY_Eqt , 
					XDY_FonctionInt,
					 XDY_Basedd ,
					 XDY_Entier*
					);

#endif
