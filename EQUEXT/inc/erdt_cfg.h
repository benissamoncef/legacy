/*E*/
/* Fichier : $Id: erdt_cfg.h,v 1.8 2012/07/04 14:28:32 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2012/07/04 14:28:32 $
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'initialisation des structures en memoires de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	09 Dec 1994	: Creation
* Volcic.F	version 1.2	13 Fev 1995	:
* Volcic.F	version 1.3	04 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.4	13 Jun 1995	: Ajout de int er_recherche_type_RADT
* Volcic.F	version 1.5	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.7	04 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* VR		version 1.8 22 Fev 2012 : Ajout fonction er_lire_params_qualif (DEM/1016)
*
------------------------------------------------------ */

#ifndef erdt_cfg
#define erdt_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "erdt_don.h"
#include "erdt_cmd.h"
#include "erdt_cal.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

int er_init_tab_mes( );

int er_lire_config_DIV( );

int er_lire_config_RADT( XDY_NomMachine );

int er_lire_config_PtMes( XDY_NomMachine );
			  
int er_lire_config_Chaussee( XDY_NomMachine );
			  
int er_recherche_type_RADT ( );

void er_controle( int, XDY_NomMachine );

int er_lire_idpassw( XDY_NomMachine );

int er_init( XDY_NomMachine );

void er_sortir ( );

void er_Lancement_Timer ( int );

void er_Lecture_derniere_heure ( ERDT_DONNEES_RADT * );

int er_lire_params_qualif	( void );

#endif
