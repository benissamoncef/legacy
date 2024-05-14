/*E*/
/* Fichier : $Id: xzao492.h,v 1.3 2021/05/03 14:00:21 pc2dpdy Exp $        $Revision: 1.3 $        $Date: 2021/05/03 14:00:21 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao492.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 02/08/2022 : Creation NIC et IS DEM-SAE403
------------------------------------------------------ */

#ifndef xzao507
#define xzao507

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO507_RPC_NAME "XZAO507"

#define XZAOC_XZAO507_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {
    XDY_Entier Numero;
   	XDY_NomEqt	Nom;
    XDY_Mot     Type;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
} XZAOT_ConfNIC_IS;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO507_Ajouter_NIC_IS (	XDY_Basedd,
					XZAOT_ConfNIC_IS,
					XDY_ResConf *);

#endif
