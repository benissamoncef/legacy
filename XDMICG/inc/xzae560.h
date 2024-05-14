/*E*/
/*Fichier : @(#)xzae560.h	1.1      Release : 1.1        Date : 02/02/12
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE560 * FICHIER XZAE560.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZAE560
#define XZAE560

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
	
/* Include Sybase */
#include <ctpublic.h>
#include <cstypes.h>

/* Include applicatif Migrazur */
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAEC_XZAE560_RPC_NAME "XZAE560"

#define XZAE560C_NB_PARM_RETURN 2


/* definitions de types exportes */
extern tg_Connexion  *XZAGV_Connexion;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE560_Bouchon_Conseq : nb de bouchons consequence
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
					
int XZAE560_Creer_FMC_Regulation (XDY_Autoroute	va_Autoroute_in,
				   XDY_Sens	 		va_Sens_in,
				   XDY_PR			va_PR_Debut_in,
				   XDY_PR			va_Pr_Fin_in,
				   XDY_Eqt 			va_Contexte_in,
				   XDY_Eqt		va_Scenario_in,
				   XDY_Eqt		va_Scenario_cible_in,
				   XDY_Booleen		va_IndicateurHoraire_in,
				   XDY_Eqt		va_Mesure_in,
				   char *			va_Transitoire_in,
				   XDY_Eqt		va_Zone_in,
				   XDY_Entier	va_alerte_in,
				   XDY_Entier		*va_Num_FMC_out,
				   XDY_Octet		*va_Cle_FMC_out
										 );
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
#endif
