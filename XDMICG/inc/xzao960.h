/*
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao960.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GR 30/07/2020  : creation TDP_FCD DEM-SAE152 1.
* ABE 24/09/2020 : Appel par callback acli DEM-SAE152 1.2
------------------------------------------------------ */

#ifndef xzao960
#define xzao960

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO960_RPC_NAME "XZAO960"

#define XZAOC_XZAO960_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO960_insert_TDP_FCD (XDY_Horodate,
				   XDY_PR,
				   XDY_PR,
				   XDY_Sens,
				   XDY_Entier,
                                   XDY_Octet,
                                   XDY_NomAuto);

#endif
