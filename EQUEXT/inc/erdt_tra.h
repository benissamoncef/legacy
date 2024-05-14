/*E*/
/*Fichier : $Id: erdt_tra.h,v 1.22 2019/03/12 15:20:40 pc2dpdy Exp $      Release : $Revision: 1.22 $        Date : $Date: 2019/03/12 15:20:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier traitement de la tache TERDT
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	20 Dec 1994	: Creation
* Volcic.F	version 1.2	20 Jan 1995	: Ajout def. fonction
* Volcic.F	version 1.3	07 Fev 1995	: Ajout def. fonction 
* Volcic.F	version 1.4	20 Mar 1995	:
* Volcic.F	version 1.5	21 Mar 1995	:
* Volcic.F	version 1.6	22 Mar 1995	:
* Fontaine.C	version 1.7	22 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.8	08 Jun 1995	: modif parametres er_Mesures_Glissantes
* Volcic.F	version 1.9	04 Jui 1995	: modif parametres er_recherche_eqt
* Volcic.F	version 1.10	27 Sep 1995	: Suppr parametres er_Fich_Mesures
* Volcic.F	version 1.11	01 Oct 1995	: Modifs
* Volcic.F	version 1.12	20 Oct 1995	: Ajout variable XDY_Horodate dans er_Extrapolation 
* Volcic.F	version 1.13	11 Dec 1995	: Ajout variable XDY_Mot dans er_Niveau_Service
* Volcic.F      version 1.14    14 Mai 1996     : Correction er_mesures_valides pb sur correction (DEM/1017)
* Volcic.F	version 1.15	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.16	20 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.17	04 Sep 1996	: Modif pour mesures pesage (DEM/1227)
*
* JMG   13/11/07 : ajout site de gestion DEM715  1.18
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : modifications mineures  1.19
* JPL	15/02/18 : Migration Linux 64 bits (DEM 1274) : Declaration 'er_lit_fichier_simu'; suppression fonction inutile  1.21
* JMG	15/03/19 : passage IP 1.22
---------------------------------------------------- */

#ifndef erdt_tra
#define erdt_tra

/* fichiers inclus */

#include "xzaac.h"

#include "erdt_don.h"
#include "erdt_cal.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

int er_recherche_index (	XDY_Eqt,
				XDY_Mot,
				XDY_Mot,
		  		XDY_Entier *,
		  		XDY_Entier * );

int er_dernier_index (	XDY_Eqt,
			XDY_Entier,
			XDY_Entier *,
		  	XDY_Horodate * );

int er_mesures_valides (	XDY_Eqt, 
				XDY_Sens );

int er_recherche_eqt ( char  		*,
		       XDY_Entier	 ,
		       XDY_Mot		*,
		       XDY_Mot		*,
		       XDY_Mot		*,
		       XDY_Horodate	*,
		       XDY_Entier	*,
		       XDY_District	*,
		       ERDT_DONNEES_RADT *);

int  er_traitement_trame( int );

extern int er_Mesures_Glissantes ( 	XDY_Horodate,
					XDY_Eqt,
					XDY_Entier,
					XDY_Booleen );

extern int er_Extrapolation ( 	XDY_Entier,
				XDY_Entier,
		        	XDY_Horodate,
				XDY_Eqt,
				XDY_Sens,
				XDY_Voie,
				XDY_Entier,
				XDY_Entier,
				XDY_Mot );

extern void er_lit_fichier_simu ( XDY_Horodate );

extern int er_Calcul_Chaussee ( XDY_Eqt, XDY_Mot, XDY_Entier, XDY_Horodate );
			  
extern int er_Niveau_Service ( XDY_Eqt, XDY_Mot, XDY_Entier , XDY_Horodate, XDY_Entier);
			  
extern int er_Seuils_Niveau ( char[9] );

extern int er_Stockage_Mesures ( XDY_Eqt, XDY_Entier, XDY_Horodate, XDY_Entier );

extern int er_Env_Mes_6mn ( XDY_Eqt, XDY_Entier );

extern int er_Envoi_Mesure ( XDY_Eqt, XDY_Booleen );

extern int er_Fichier_Mesures ( XDY_Eqt, XDY_Entier, XDY_Horodate );

extern int er_Maj_Fichier_heure ( XDY_Eqt, XDY_Horodate );

#endif

