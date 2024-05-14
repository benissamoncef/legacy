/*E*/
/* Fichier : $Id: xzao554.h,v 1.2 2012/07/04 17:54:13 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2012/07/04 17:54:13 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao554.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       14 Nov 2011     : Creation (DEM/1014)
------------------------------------------------------ */

#ifndef xzao554
#define xzao554

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO554_RPC_NAME "XZAO554"

#define XZAOC_XZAO554_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

	XDY_Autoroute	Autoroute;
	XDY_Sens		Sens;
	XDY_PR			PR;
	XDY_Eqt				Numero;
	XDY_TypeEqt			Type;
	XDY_VitesseRegul	Vitesse_Nominale;
	XDY_Entier				Zone;
	XDY_Booleen			Critique;
	XDY_Booleen			Inhibition;
  
} XZAOT_ConfEqtPRV;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO554_Ajouter_Equipement ( XDY_Basedd,
                                        XZAOT_ConfEqtPRV,
                                        XDY_ResConf *);

#endif
