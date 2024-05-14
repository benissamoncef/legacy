/*E*/
/*Fichier :  $Id: xzao1014.h,v 1.1 2008/02/13 13:24:29 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:29 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1014.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ   	15 Nov 2007	: Creation DEM 662
------------------------------------------------------ */

#ifndef xzao1014
#define xzao1014

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1014_RPC_NAME "XZAO1014"

#define XZAOC_XZAO1014_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_NomEqt	NomEXP;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqAutEXP;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1014_Ajouter_SeqAut_EXP (	XDY_Basedd,
						XZAOT_ConfSeqAutEXP,
						XDY_ResConf *);



#endif
