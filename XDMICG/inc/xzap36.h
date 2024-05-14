/*E*/
/*  Fichier : @(#)xzao907.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap36.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/06/2021 : Création DEM-SAE155 1.1
------------------------------------------------------ */

#ifndef xzap36								
#define xzap36

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAP36_RPC_NAME "XZAP36"

#define XZAOC_XZAP36_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

int XZAP36_ajout_Prop_IMU(		XDY_Mot 		va_NumPA_in,
					XDY_Mot 		va_Ordre_in,
					XDY_District		va_District_in,
					XDY_Eqt 		va_NoEqt_in,
					XDY_Octet		va_PrioriteEvt_in,
					XDY_Commentaire		va_Remarque_in,
					XDY_Texte		va_Explication_in,
					XDY_Booleen		va_Realisable_in,
					XDY_Texte		va_Chaine_in,
					XDY_Entier		va_Intervalle_in,
					XDY_Entier		va_Cycles_in,
					XDY_Entier		va_Duree_in,
					XDY_District		va_Sit_in,
					XDY_Nom			va_Libelle_in );
						

extern tg_Connexion  *XZAGV_Connexion;

#endif
