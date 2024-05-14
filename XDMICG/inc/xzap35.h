/*E*/
/*  Fichier : @(#)xzao907.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap35.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	11/05/2021 : Création DEM-SAE244 1.1
------------------------------------------------------ */

#ifndef xzap35								
#define xzap35

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAP35_RPC_NAME "XZAP35"

#define XZAOC_XZAP35_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP35_ajout_Prop_SONO(	XDY_Mot 		va_NumPA_in,
					XDY_Mot 		va_Ordre_in,
					XDY_District		va_District_in,
					XDY_Eqt 		va_NoEqt_in,
					XDY_Octet		va_PrioriteEvt_in,
					XDY_Commentaire		va_Remarque_in,
					XDY_Texte		va_Explication_in,
					XDY_Booleen		va_Realisable_in,
					XDY_Texte		va_Message_in,
					XDY_Autoroute		va_Autoroute_in,
					XDY_Sens		va_Sens_in,
					XDY_PR			va_PR_deb_in,
					XDY_PR			va_PR_fin_in,
					XDY_Nom			va_Libelle_in );
						

extern tg_Connexion  *XZAGV_Connexion;

#endif
