/*E*/
/*Fichier : @(#)xzao312.h	1.2      Release : 1.2        Date : 11/17/08
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao312.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	29 Jui 1995	: Creation
* JMG 11/11/08 : parametres sortie DEM/835
------------------------------------------------------ */

#ifndef xzao312
#define xzao312

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO312_RPC_NAME "XZAO;312"

#define XZAOC_XZAO312_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO312_Lire_Sortie_Ech (	XDY_Octet,
					XDY_Octet *);

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    *XZAGV_Context;

#endif
