/*E*/
/*  Fichier : $Id: erau_phs.h,v 1.2 1996/12/05 22:40:19 mismer Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/12/05 22:40:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_phs.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	28 Oct 1996	: Creation pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.2	05 Nov 1996	: Modif pour Alerte PAU HS (DEM/1308)
*
------------------------------------------------------ */

#ifndef erau_phs
#define erau_phs

/* fichiers inclus */

#include	"erau_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int  ec_lire_fichier_info_PAU ( void );
extern void er_Etat_PAU_cb ( T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG );
extern int  ec_Detect_Alerte_PAU ( void );
 
#endif
