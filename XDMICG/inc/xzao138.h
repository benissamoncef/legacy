/*E*/
/*  Fichier : $Id: xzao138.h,v 1.1 1994/10/05 19:12:19 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:19 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao138.h
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

#ifndef xzao138
#define xzao138

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO138_RPC_NAME "XZAO;138"

#define XZAOC_XZAO138_NB_PARM_RETURN 1

/* definitions de types exportes */
	
typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_NomEqt	NomSystemeVideo;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	LiaisonsFixes;
	XDY_Octet	LiaisonsCycles;

} XZAOT_ConfLTVideo;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO138_Ajouter_LTVideo (	XDY_Basedd,
					XZAOT_ConfLTVideo,
					XDY_ResConf *);


#endif
