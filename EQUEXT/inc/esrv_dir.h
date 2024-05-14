/*X*/
/*Fichier : $Id: esrv_dir.h,v 1.7 1995/06/20 19:31:19 volcic Exp $      Release : $Revision: 1.7 $        Date : $Date: 1995/06/20 19:31:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESRV * FICHIER esrv_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TESRV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Aug 1994	: Creation
* Mismer.D	version 1.2	23 Nov 1994	: Modif ed_arreter_tache_eqp
* Mismer.D	version 1.3	25 Jan 1995	:
* Mismer.D	version 1.4	26 Jan 1995	:
* Mismer.D	version 1.5	09 Fev 1995	:
* Fontaine.C	version 1.6	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.7	02 Jun 1995	: modif fichier a inclure
*
------------------------------------------------------ */

#ifndef esrv_dir
#define esrv_dir

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
#include 	"esrv_cfg.h"
#include 	"etim_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

int		vg_ConfigTerm = XDC_NOK;
int		vg_InitTranserver = XDC_FAUX;

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




#endif
