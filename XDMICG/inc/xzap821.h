/*Fichier : $Id: xzap821.h
------------------------------------------------------
* GTIE   PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao FICHIER xzap821.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
*  GGY	04/12/23	: Creation (DEM483)
------------------------------------------------------ */
#ifndef xzap821
#define xzap821

/* fichiers inclus */
#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAOC_XZAP821_RPC_NAME "XZAP821"
#define XZAOC_XZAP821_NB_PARM_RETURN 0

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

extern int XZAP821_Ajout_Prop_PIC (	XDY_Mot,
									XDY_Mot,
									XDY_District,
									XDY_Phrase,
									XDY_Octet,
									XDY_Eqt,
									XDY_Commentaire,
									XDY_Nom,
									XDY_Bandeau_PIC,
									XDY_Booleen,
									XDY_Picto_PIC,
									XDY_Booleen );


extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    *XZAGV_Context;

#endif
