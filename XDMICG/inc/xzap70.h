/*E*/
/*  Fichier : @(#)xzap70.h	$Id: xzap70.h,v 1.2 2017/10/10 08:28:12 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2017/10/10 08:28:12 $
------------------------------------------------------
* GTIE   PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao FICHIER xzap70.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	 02/01/17 creation
* JMG	20/06/17 : convergence 1.2
------------------------------------------------------ */

#ifndef xzap70
#define xzap70

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP70_RPC_NAME "XZAP70"

#define XZAOC_XZAP70_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP70_Ajout_Prop_SAGA  (	XDY_Mot,
                          		XDY_Mot,
                          		XDY_District,
                          		XDY_Phrase,
                          		XDY_Octet,
                          		XDY_Eqt,
                          		XDY_Commentaire,
                          		XDY_NomTache,
					XDY_NomTache,
					XDY_NomTache,
					XDY_NomTache,
					XDY_NomTache,
                          		XDY_Nom,
                          		XDY_NomSysteme,
                          		XDY_NomSysteme,
					XDY_Octet,
					XDY_Mot,
					XDY_Mot,
					XDY_Mot,
					XDY_Mot,
					XDY_Booleen,
					int);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
