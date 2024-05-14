/*E*/
/*  Fichier : $Id: xzao168.h,v 1.1 1994/10/05 19:12:46 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:46 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao168.h
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

#ifndef xzao168
#define xzao168

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO168_RPC_NAME "XZAO;168"

#define XZAOC_XZAO168_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqTube;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO168_Ajouter_Sequence_Tube (	XDY_Basedd,
						XZAOT_ConfSeqTube,
						XDY_ResConf *);


#endif
