/*E*/
/* Fichier : $Id: eini_dir.h,v 1.8 2008/02/13 13:57:47 devgtie Exp $      Release : $Revision: 1.8 $        Date : $Date: 2008/02/13 13:57:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere les structures de dir nees de la tache TEINI.
*  La variable globale pg_ListeMachine contient le pointeur de debut de liste.
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	14 Dec 1994	:
* Mismer.D	version 1.3	09 Fev 1995	:
* Mismer.D	version 1.4	14 Fev 1995	:
* Fontaine.C	version 1.5	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	05 Sep 1996	: Ajout variable globale vg_Type_Machine (RADT_W)
* Mismer.D	version 1.7	23 Oct 1996	: Modif pour Alerte PAU HS (DEM/1282)
*
------------------------------------------------------ */

#ifndef eini_dir
#define eini_dir

/* fichiers inclus */

#include "eini_sur.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */


/* declarations de donnees globales */

EINI_MACHINE *		pg_ListeMachine 	= NULL;
EINI_MACHINE *		pg_MachineCrt   	= NULL;
EINI_DONNEES_FEN_LCR *	pg_DebMacLCR   		= NULL;

XDY_Fichier		vg_FdConf[6]		= { 0, 0, 0, 0, 0, 0 };
int			vg_Type_Machine		= 0;
XDY_NomAuto     	vg_NomAutoroute[350];
/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction effectuant les operations de terminaison de la tache.
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
*  Fonction declenchee par SUPERV lors de l'arret de la tache
*  TEINI
* 
* FONCTION 
*  Deconnexion de la base de dirnees (appel XZAG03)
*  Fermeture des fichier de configuration
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_syn_cb (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG );
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_EINIT
*
* FONCTION 
*  Appel de MSYN en donnant le nom de la machine (une ou toutes) et le type de configuration
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Appel du module de MALL et des modules de Mzzz selon le message XDM_ECONFIG_EQPT.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ed_config_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECONFIG_EQPT
*
* FONCTION 
*  Appel de MALL et appel sequentiel des modules Mzzz selon le message specifie
*  Appel de MSYN en dirnant le nom de la machine (une ou toutes) et le type de configuration
*
------------------------------------------------------*/


#endif
