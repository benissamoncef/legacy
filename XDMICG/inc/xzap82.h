/*E*/
/*Fichier : $Id: xzap82.h,v 1.1 2012/07/04 11:00:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:00:51 $
------------------------------------------------------
* GTIE   PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao FICHIER xzap82.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzap82
#define xzap82

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP82_RPC_NAME "XZAP82"

#define XZAOC_XZAP82_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP82_Ajout_Prop_PRV  (	XDY_Mot,
                          		XDY_Mot,
                          		XDY_District,
                          		XDY_Phrase,
                          		XDY_Octet,
                          		XDY_Eqt,
                          		XDY_Commentaire,
					XDY_Bandeau,
					XDY_VitesseRegulTxt,
					XDY_Nom,
					XDY_Booleen );



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
