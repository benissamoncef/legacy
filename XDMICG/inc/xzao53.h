/*E*/
/*  Fichier : $Id: xzao53.h,v 1.3 1998/01/23 14:46:56 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/01/23 14:46:56 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao53.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* C.T.		22 Jan 1998	: Modif struct de la fonction (1545) V1.2
------------------------------------------------------ */

#ifndef xzao53
#define xzao53

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO53_RPC_NAME "XZAO;53"

#define XZAOC_XZAO53_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct
{	
   	XDY_Octet	Numero;
   	XDY_Nom		NomDistrict;
   	XDY_NomSite	Abreviation;
   	XDY_Nom		Type;
   	XDY_NomMachine	Serveur;
   	XDY_NomMachine	Machine;

} XZAOT_ConfDistrict;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO53_Ajouter_District ( 	XDY_Basedd,
					XZAOT_ConfDistrict,
					XDY_ResConf *);



#endif
