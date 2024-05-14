/*E*/
/*  Fichier : @(#)xzap23.h	$Id: xzap23.h,v 1.1 1999/02/26 11:26:15 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:26:15 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap23.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron 	25/11/1998	: Creation
------------------------------------------------------ */

#ifndef xzap23
#define xzap23

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP23_RPC_NAME "XZAP;23"

#define XZAOC_XZAP23_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAP23SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAP23_Ajout_Prop_Exporter(XDY_Mot 	NumPA_In,
				XDY_Mot 	Ordre_In,
				XDY_District	District_In,
				XDY_Nom 	Libelle_In,
				XDY_Phrase	Remarque_In,
				XDY_Octet	Gravite_In,
				XDY_Octet	Trafic_In);


/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Horodate		Horodate
* XDY_Nom 		Libelle
* XDY_Phrase		Remarque 
* XDY_Octet		Gravite_In
* XDY_Octet		Trafic_In
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
------------------------------------------------------*/


extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
