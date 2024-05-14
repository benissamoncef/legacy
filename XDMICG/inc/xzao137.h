/*E*/
/*  Fichier : $Id: xzao137.h,v 1.1 1994/10/05 19:12:17 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:17 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao137.h
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

#ifndef xzao137
#define xzao137

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO137_RPC_NAME "XZAO;137"

#define XZAOC_XZAO137_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		RGS;
   	
} XZAOT_ConfSyVideo;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO137_Ajouter_SystemeVideo (	XDY_Basedd,
						XZAOT_ConfSyVideo,
						XDY_ResConf *);


#endif
