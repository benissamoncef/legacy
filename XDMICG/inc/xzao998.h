/*E*/
/*Fichier :  $Id: xzao998.h,v 1.1 2008/02/13 13:24:39 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:39 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao998.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ   	15 Nov 2007	: Creation dem 662
------------------------------------------------------ */

#ifndef xzao998
#define xzao998

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO998_RPC_NAME "XZAO;998"

#define XZAOC_XZAO998_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqVEN;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO998_Ajouter_Seq_VEN ( XDY_Basedd,
						             XZAOT_ConfSeqVEN,
						             XDY_ResConf *);



#endif
