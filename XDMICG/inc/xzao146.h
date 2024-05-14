/*E*/
/*  Fichier : $Id: xzao146.h,v 1.1 1994/10/05 19:12:33 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:33 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao146.h
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

#ifndef xzao146
#define xzao146

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO146_RPC_NAME "XZAO;146"

#define XZAOC_XZAO146_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;

} XZAOT_ConfITPC;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO146_Ajouter_ITPC (	XDY_Basedd,
					XZAOT_ConfITPC,
					XDY_ResConf *);



#endif
