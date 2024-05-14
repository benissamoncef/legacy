/*E*/
/*Fichier :  $Id: xzao1000.h,v 1.1 2008/02/13 13:24:26 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:26 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1000.h
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

#ifndef xzao1000
#define xzao1000

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1000_RPC_NAME "XZAO;1000"

#define XZAOC_XZAO1000_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqENR;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1000_Ajouter_Seq_ENR ( XDY_Basedd,
						             XZAOT_ConfSeqENR,
						             XDY_ResConf *);



#endif
