/*E*/
/*  Fichier : $Id: xzao141.h,v 1.1 1994/10/05 19:12:25 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:25 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao141.h
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

#ifndef xzao141
#define xzao141

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO141_RPC_NAME "XZAO;141"

#define XZAOC_XZAO141_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_CodeEqt	Code;
   	XDY_NomEqt	NomSysVideo;

} XZAOT_ConfMagneto;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO141_Ajouter_Magnetoscope (	XDY_Basedd,
						XZAOT_ConfMagneto,
						XDY_ResConf *);


#endif
