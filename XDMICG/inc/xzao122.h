/*E*/
/*Fichier : @(#)xzao122.h	1.5      Release : 1.5        Date : 07/13/16
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao122.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic        25 Nov 1994     : Ajout Structure XZAOT_District (v 1.3)
* volcic        30 Nov 1994     : Modif entete (v 1.4)
* JPL		13/07/2016	: Utilisation de XZAO30 degroupee ; interface identique  1.5
------------------------------------------------------ */

#ifndef xzao122
#define xzao122

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO122_RPC_NAME "XZAO30"

#define XZAOC_XZAO122_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct
{

   XDY_Octet	Numero;
   XDY_Nom	Nom;
   XDY_NomSite	Code;
   
} XZAOT_District;   

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO122_Liste_Districts (	XDY_Basedd,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
