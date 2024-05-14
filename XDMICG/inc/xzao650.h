/*E*/
/*  Fichier : $Id: xzao650.h,v 1.1 2016/12/08 16:19:55 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/12/08 16:19:55 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao650.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		15 Nov 2016	: Creation (LOT23)
------------------------------------------------------ */

#ifndef xzao650
#define xzao650

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO650_RPC_NAME "XZAO650"

#define XZAOC_XZAO650_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Octet	Numero;
   	XDY_Nom		NomViaduc;
   	XDY_NomAuto	Autoroute;
   	XDY_Sens	Sens;
   	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	XDY_NomSite	NomSite;
	
} XZAOT_ConfViaduc;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO650_Ajouter_Viaduc (	XDY_Basedd,
					XZAOT_ConfViaduc,
					XDY_ResConf *);



#endif
