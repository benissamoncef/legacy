/*E*/
/* Fichier : $Id: xzao503.h,v 1.1 2021/05/03 14:02:26 pc2dpdy Exp $        $Revision: 1.1 $        $Date: 2021/05/03 14:02:26 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao503.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	02/10/2020	: Creation DEM-SAE155 1.1
------------------------------------------------------ */

#ifndef xzao503
#define xzao503

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO503_RPC_NAME "XZAO503"

#define XZAOC_XZAO503_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_Entier	 Id;
	XDY_Texte      Libelle;
	XDY_Autoroute	 Autoroute;
	XDY_Sens	 Sens;
	XDY_PR	 PR_debut;
	XDY_PR	 PR_fin;
} XZAOT_Conf_Zon_SONO;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO503_Ajouter_Zon_SONO (	XDY_Basedd,
					XZAOT_Conf_Zon_SONO,
					XDY_ResConf *);

#endif
