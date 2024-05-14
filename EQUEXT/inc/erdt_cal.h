/*E*/
/*Fichier : $Id: erdt_cal.h,v 1.13 1996/10/09 20:41:25 mismer Exp $      Release : $Revision: 1.13 $        Date : $Date: 1996/10/09 20:41:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cal.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier etablissant les calculs RADT
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	20 dec 1994	: Creation
* Volcic.F	version 1.2	21 Mar 1995	:
* Volcic.F	version 1.3	22 Mar 1995	:
* Fontaine.C	version 1.4	22 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.5	08 Jun 1995	: Modif parametres er_Mesures_Glissantes
* Volcic.F	version 1.6	04 Jui 1995	: Ajout de la fonction er_Envoi_Mesures_HIS 
* Volcic.F	version 1.7	04 Jui 1995	: Modifs 
* Volcic.F	version 1.8	27 Sep 1995	: Suppr er_Fich_Mesures 
* Volcic.F	version 1.9	20 Oct 1995	: Ajout variable XDY_Horodate dans er_Extrapolation 
* Volcic.F	version 1.10	11 Dec 1995	: Ajout variable XDY_Mot dans er_Niveau_Service
* Volcic.F	version 1.11	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* D.Mismer	version 1.12	19 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.13	04 Sep 1996	: Modif pour mesures pesage (DEM/1227)
------------------------------------------------------ */

#ifndef erdt_cal
#define erdt_cal

/* fichiers inclus */

#include "erdt_don.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


int er_Mesures_Glissantes ( 	XDY_Horodate,
				XDY_Eqt,
				XDY_Entier,
				XDY_Booleen );

int er_Extrapolation ( 	XDY_Entier,
			XDY_Entier,
			XDY_Horodate,
			XDY_Eqt,
			XDY_Octet,
			XDY_Voie,
			XDY_Entier,
			XDY_Entier,
			XDY_Mot );
			
int er_Calcul_Chaussee ( 	XDY_Eqt,
				XDY_Mot,
				XDY_Entier,
				XDY_Horodate );
			  
			  
int er_Niveau_Service ( XDY_Eqt, 
			XDY_Mot, 
			XDY_Entier ,
			XDY_Horodate,
			XDY_Entier);
			  

int er_Seuils_Niveau ( char[9] );

int er_Fichier_Mesures ( XDY_Eqt, XDY_Entier, XDY_Horodate );

int er_Stockage_Mesures ( XDY_Eqt, XDY_Entier, XDY_Horodate, XDY_Entier );

int er_Maj_Fichier_heure ( XDY_Eqt, XDY_Horodate );

int er_Env_Mes_6mn ( XDY_Eqt, XDY_Entier );

int er_Envoi_Mesure ( XDY_Eqt, XDY_Booleen );

int er_Stockage_Pesage ( ERDT_MESURE_PESAGE, XDY_Entier );

int er_Maj_Fichier_heure_Pesage ( XDY_Eqt );

int er_Envoi_Mesure_Pesage ( XDY_Eqt, XDY_Booleen, XDY_Entier );

int er_Recopie_Fichier_Mesure_Pesage ( XDY_Eqt, XDY_Entier );

#endif
