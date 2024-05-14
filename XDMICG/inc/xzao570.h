/*E*/
/* Fichier : $Id: xzao570.h,v 1.1 2012/07/04 17:56:22 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:56:22 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao570.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao570
#define xzao570

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO570_RPC_NAME "XZAO570"

#define XZAOC_XZAO570_NB_PARM_RETURN 1 

/* definitions de types exportes */

typedef struct {

   	XDY_Eqt		Numero;  	
   	XDY_TypeEqt	Type;
   	XDY_NomParamRegul		Nom;
	XDY_Entier	Valeur;

} XZAOT_Terdt;


/* delaration de fonctions */
extern int XZAO570_Params_Regulation(XDY_Entier , 
					XDY_Entier,
					 XDY_FonctionInt ,
					 XDY_Basedd 
					);

#endif
