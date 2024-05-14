/*X*/
/*Fichier : $Id: esur_dir.h,v 1.4 1995/12/13 15:05:34 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/12/13 15:05:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESUR * FICHIER esur_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TESUR
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Aug 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	19 Jun 1995	: Modif surveillance LT voisin
* Mismer.D	version 1.4	05 Dec 1995	: Ajout variable vg_RepriseEnCours
*
------------------------------------------------------ */

#ifndef esur_dir
#define esur_dir

/* fichiers inclus */
#include	<stdio.h>

#include	<xdc.h>
#include	<xzsc.h>
#include	<xzsm.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"xzec.h"
#include 	"xzex.h"
#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"esur_cfg.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XDY_NomMachine vg_NomMachine;
int            vg_ConfigTerm     = XDC_NOK;
int            vg_RepriseEnCours = XDC_NOK;

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction effectuant les op≈rations de terminaison de la tache.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_arreter ( void );

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*  Fonction d≈clench≈e par SUPERV lors de l'arret de la tache
*  TESRV
* 
* FONCTION 
*  Envoi d'une demande d'arret » la tache timer TETIM et
*  aux taches ≈quipements
*  Attente infinie sur tous les PID des taches lanc≈es
*  Fermeture de la socket d'≈coute
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de transmettre la demande d'arret aux taches traitement.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ed_arreter_tache_eqpt ( void );

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*  
* 
* FONCTION 
*  Pour chaque equipement transmettre la demmande d'arret a la tache traitement.
*
------------------------------------------------------*/

extern void			  Analyse_Dg_Status(
	T_IPC_CONN 				   va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA 		   data,
	T_CB_ARG 				   arg);



#endif
