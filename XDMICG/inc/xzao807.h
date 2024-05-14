/*E*/
/*Fichier : $Id: xzao807.h,v 1.1 2018/04/18 17:50:52 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/04/18 17:50:52 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao807.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	22 Jan 18	: Creation CTRL_FEUX 1.1 DEM1284
------------------------------------------------------ */

#ifndef xzao807
#define xzao807

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO807_RPC_NAME "XZAO807"

#define XZAOC_XZAO807_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO807_Liste_CFE_Machine (	XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
