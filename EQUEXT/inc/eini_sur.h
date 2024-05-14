/*E*/
/*  Fichier : $Id: eini_sur.h,v 1.5 2010/12/07 12:37:15 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/12/07 12:37:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_sur.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere la creation des fichiers de config pour la surveillance
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1995	: Creation
* Fontaine.C	version 1.2	19 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	29 Jun 1995	: Ajout ei_verif_connexion_tache
* Mismer.D  	version 1.4	26 Avr 1996	: Ajout traitement LT isol≈e (DEM/1052)
* JMG		version 1.5	22/06/10	: portage linux DEM/934
------------------------------------------------------ */

#ifndef eini_sur
#define eini_sur

/* fichiers inclus */

#include "eini_don.h"
#include "xzamc.h"

/* definitions de constantes exportees */

/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de variables globales */

extern  EINI_DONNEES_FEN_LCR *	pg_DebMacLCR;
extern  XZEXT_MASK *		vg_SockMask;
extern  int			vg_SockTETIM;
extern	EINI_MACHINE *		pg_ListeMachine;

/* delaration de fonctions externes */

extern void	Analyse_Dg_Status(
	T_IPC_CONN 				   va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA 		   data,
	T_CB_ARG 				   arg);

extern void	ei_traitement_mode_isole ( 
	XDY_NomMachine, 
	int );

#endif
