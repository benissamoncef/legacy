/*E*/
/*Fichier :  $Id: xzao982.h,v 1.1 2008/02/13 13:24:36 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:36 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao982.h
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

#ifndef xzao982
#define xzao982

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO982_RPC_NAME "XZAO;982"

#define XZAOC_XZAO982_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomSequence;
	XDY_Octet   Type;
	XDY_NomEqt	NomSIG;
	XDY_Booleen	Creation;
	
} XZAOT_ConfSeqAutSIG;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO982_Ajouter_SeqAut_SIG (	XDY_Basedd,
						XZAOT_ConfSeqAutSIG,
						XDY_ResConf *);



#endif
