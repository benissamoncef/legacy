/*E*/
/*  Fichier : $Id: xzao51.h,v 1.3 1999/02/26 11:32:12 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1999/02/26 11:32:12 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao51.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* Torregrossa	30 Oct 1995	: Modif interface pour prise en compte
*                                 sens de l'autoroute (A57) v1.2
* niepceron	02 Dec 1998	: Ajout colonne escota dem/1724 1.3
------------------------------------------------------ */

#ifndef xzao51
#define xzao51

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO51_RPC_NAME "XZAO;51"

#define XZAOC_XZAO51_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO51_Ajouter_Autoroute ( 	XDY_Basedd,
					XDY_NomAuto,
					XDY_Mot,
					XDY_Octet,
					XDY_ResConf *);



#endif
