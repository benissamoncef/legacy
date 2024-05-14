/*E*/
/* Fichier : $Id: sapl_ini.h,v 1.2 2011/01/12 15:50:42 gesconf Exp $	Release : $Revision: 1.2 $        Date : $Date: 2011/01/12 15:50:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI * FICHIER sapl_ini.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Module directeur de la tache tsaplg
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  10 Oct 1994 	: Creation
* JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : ajout fonction 'sp_initTabSurvTache'  1.2
******************************************************/

#ifndef SAPL_INI
#define SAPL_INI

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void sp_init_cb();
extern void sp_init();
extern void sp_fait_rien();

extern void sp_initTabSurvTache (XZSCT_NomTache va_NomTache);

#endif

