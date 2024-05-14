/*E*/
/* Fichier : $Id: xzao498.h,v 1.2 2021/05/03 14:01:51 pc2dpdy Exp $        $Revision: 1.2 $        $Date: 2021/05/03 14:01:51 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao498.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	02/10/2020	: Creation DEM-SAE155 1.1
* ABE	15/12/2020	: Homogeneisation des noms de struct DEM-SAE155 1.2
------------------------------------------------------ */

#ifndef xzao498
#define xzao498

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO498_RPC_NAME "XZAO498"

#define XZAOC_XZAO498_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
   	XDY_Eqt		NumeroEqt;
	XDY_Texte	Libelle;
} XZAOT_ConfMsgIMU;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO498_Ajouter_Msg_IMU (	XDY_Basedd,
					XZAOT_ConfMsgIMU,
					XDY_ResConf *);

#endif
