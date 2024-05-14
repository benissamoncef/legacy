/*E*/
/* Fichier : $Id: exa0_cmd.h,v 1.2 1995/05/23 18:39:11 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 18:39:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache
*  TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef exa0_cmd
#define exa0_cmd

/* fichiers inclus */

#include "exa0_don.h"
#include "exa0_cfg.h"
#include "exa0_ctl.h"
#include "exa0_tra.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info de la tache traitement TENIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ea_traitement_cmd ( int	va_NumSock );

/*
* ARGUMENTS EN ENTREE :
*   
* int	va_NumSock	: Numero de la socket TENIC
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur evenement socket TENIC
*
* FONCTION 
*
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Positionnement d'un flag dans la table des semaphore de l'automate
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ea_ecriture_flag (	short	va_Adresse,
				short 	va_Valeur );

/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*
------------------------------------------------------*/




#endif
