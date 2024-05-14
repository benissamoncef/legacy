/*E*/
/* Fichier : $Id: sapl_com.h,v 1.2 2011/01/12 15:50:37 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2011/01/12 15:50:37 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM * FICHIER sapl_com.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Module de la tache tsaplg
*
******************************************************
* HISTORIQUE :
*
* T;Milleville  10 Oct 1994	: Creation
* JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : ajout fonction 'sp_startTache_Inter'  1.2
******************************************************/

#ifndef SAPL_COM
#define SAPL_COM

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */
extern void sp_stopTache();
extern int sp_startTache();
extern void sp_stopSSysteme();
extern void sp_startSSysteme();
extern void sp_stopAppli();
extern void sp_startAppli();

extern int sp_startTache_Inter (XZSCT_NomTache va_maTache,
                                XZSCT_NomMachine va_NomMachine);


#endif

