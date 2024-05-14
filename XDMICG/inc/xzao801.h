/*E*/
/*  Fichier : @(#)xzao801.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao801.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao801
#define xzao801

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO801_RPC_NAME "XZAO801"

#define XZAOC_XZAO801_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO801_Verif_Sequence_Autorisee (XDY_Entier              ,
					XDY_Entier      ,
				XDY_Entier      ,
					XDY_Octet  *           );
						



#endif
