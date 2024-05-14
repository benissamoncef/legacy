/*E*/
/*Fichier :  $Id: xzao996.h,v 1.1 2008/02/13 13:24:39 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:39 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao996.h
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

#ifndef xzao996
#define xzao996

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO996_RPC_NAME "XZAO;996"

#define XZAOC_XZAO996_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqECL;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO996_Ajouter_Seq_ECL ( XDY_Basedd,
						             XZAOT_ConfSeqECL,
						             XDY_ResConf *);



#endif
