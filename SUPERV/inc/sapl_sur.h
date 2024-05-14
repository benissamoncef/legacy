/*E*/
/* Fichier : $Id: sapl_sur.h,v 1.4 2020/11/03 17:52:36 pc2dpdy Exp $        Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 17:52:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR * FICHIER sapl_sur.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Module appartenant a la tache tsaplg
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  10 Oct 1994	: Creation
* T.Milleville  09 Avr 1995 	: Ajout fonction callbak 
*	sp_starttmp_cb V1.2
*
******************************************************/

#ifndef SAPL_SUR
#define SAPL_SUR

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void sp_dgstatus_cb ( ) ;
extern void sp_genetat_ssys ( ) ;
extern void sp_genetat_mach ( ) ;
extern void sp_etat_cb ( ) ;
extern void sp_starttmp_cb ( ) ;
extern int sp_dgstatus ( ) ;
extern void sp_watch_cb();
#endif

