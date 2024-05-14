/*E*/
/*Fichier :  $Id: xzao1002.h,v 1.1 2008/02/13 13:24:26 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:26 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1002.h
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

#ifndef xzao1002
#define xzao1002

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1002_RPC_NAME "XZAO;1002"

#define XZAOC_XZAO1002_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqINC;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1002_Ajouter_Seq_INC ( XDY_Basedd,
						             XZAOT_ConfSeqINC,
						             XDY_ResConf *);



#endif
