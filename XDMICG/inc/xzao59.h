/*E*/
/*  Fichier : $Id: xzao59.h,v 1.1 1994/10/05 19:13:22 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:22 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao59.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao59
#define xzao59

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO59_RPC_NAME "XZAO;59"

#define XZAOC_XZAO59_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct
{
   	XDY_Nom		NomPortion;
   	XDY_PR		PRdebut;
   	XDY_Sens	Sens;
   	XDY_Octet	NombreVoies;
   	XDY_Octet	LargeurBAU;
   	XDY_Booleen	EnTunnel;
   	XDY_Nom		NomTunnel;
   	XDY_Mot		Gabarit;
   	XDY_Booleen	Creation;

} XZAOT_ConfSegment;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO59_Ajouter_Segment ( 	XDY_Basedd,
					XZAOT_ConfSegment,
					XDY_ResConf *);


#endif
