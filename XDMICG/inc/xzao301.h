/*E*/
/*Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao149.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
------------------------------------------------------ */

#ifndef xzao301
#define xzao301

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO301_RPC_NAME "XZAO301"

#define XZAOC_XZAO301_NB_PARM_RETURN 0

/* definitions de types exportes */
typedef struct {
	XDY_Eqt		NumeroEchangeur;
        XDY_Nom_Ech             NomEchangeur;
        XDY_Octet       NumeroAutoroute;
        XDY_PR          PR;
} XZAOT301_Echangeur;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO301_Liste_Echangeurs(XDY_Basedd,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
