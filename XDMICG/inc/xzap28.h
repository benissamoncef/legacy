/*E*/
/*  Fichier : $Id: xzap28.h,v 1.2 2012/11/08 09:55:41 pc2dpdy Exp $      Release : $Revision: 1.2 $       Date : $Date: 2012/11/08 09:55:41 $
------------------------------------------------------
* GTIE   PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao FICHIER xzap28.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	 Creation
* VR	08/06/12	Ajout destination TDP PMVA (DEM/1014 PMA)
------------------------------------------------------ */

#ifndef xzap28
#define xzap28

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP28_RPC_NAME "XZAP28"

#define XZAOC_XZAP28_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP28_Ajout_Prop_PMVA  (	XDY_Mot,
                          		XDY_Mot,
                          		XDY_District,
                          		XDY_Phrase,
                          		XDY_Octet,
                          		XDY_Eqt,
                          		XDY_Commentaire,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Ligne_PMVA,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                             		XDY_Nom,
					XDY_Booleen );



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
