/*E*/
/*  Fichier : $Id: xzao174.h,v 1.1 1994/10/05 19:12:52 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:52 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao174.h
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

#ifndef xzao174
#define xzao174

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO174_RPC_NAME "XZAO;174"

#define XZAOC_XZAO174_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqEch;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO174_Ajouter_Sequence_Ech (	XDY_Basedd,
						XZAOT_ConfSeqEch,
						XDY_ResConf *);



#endif
