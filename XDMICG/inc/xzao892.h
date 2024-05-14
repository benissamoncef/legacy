/*E*/
/*Fichier :  $Id: xzao892.h,v 1.1 2008/02/13 13:24:36 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:36 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao892.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	10/08/2022	: Creation DEM-SAE406
------------------------------------------------------ */

#ifndef xzao892
#define xzao892

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO892_RPC_NAME "XZAO892"

#define XZAOC_XZAO892_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Entier	NumeroEquipment;
	XDY_Entier	ScenarioTerrain;
	XDY_Entier	ScenarioRetourNormal;
	
} XZAOT_ConfSeqRetSIG;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO892_Ajouter_SeqRet_SIG (	XDY_Basedd,
						XZAOT_ConfSeqRetSIG,
						XDY_ResConf *);



#endif
