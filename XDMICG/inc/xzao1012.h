/*E*/
/*Fichier :  $Id: xzao1012.h,v 1.1 2008/02/13 13:24:28 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:28 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1012.h
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

#ifndef xzao1012
#define xzao1012

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1012_RPC_NAME "XZAO1012"

#define XZAOC_XZAO1012_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqEXP;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1012_Ajouter_Seq_EXP ( XDY_Basedd,
						             XZAOT_ConfSeqEXP,
						             XDY_ResConf *);



#endif
