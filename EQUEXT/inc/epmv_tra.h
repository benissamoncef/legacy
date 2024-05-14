/*E Fichier : $Id: epmv_tra.h,v 1.6 2011/03/18 10:56:16 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/03/18 10:56:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	06 Oct 1994	: Creation
* Volcic.F	version 1.2	10 Mai 1995	:
* Fontaine.C	version 1.3	18 Mai 1995	: Reprise entete, historique
* Mismer.D      version 1.4     29 Aou 1996     : Ajout ligne direction pour PMV 4 lignes (DEM/1162)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : declaration fonction ep_envoi_affichage_Picto  1.6
------------------------------------------------------ */

#ifndef epmv_tra
#define epmv_tra

/* fichiers inclus */

#include "xzex.h"
#include "xzst.h"
#include "xdc.h"
#include "xdg.h"
#include "epmv_don.h"
#include "epmv_cmd.h"

/* definitions de constantes exportees a deplace ds xdc.h */

extern XDY_NomSite        	vg_NomSite;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int  em_traitement_trame(int);

extern void ep_envoi_affichage_PMV ( 	XDY_Etat_PMV, int, int , XDY_District);

extern void ep_envoi_affichage_Picto ( 	XDY_Etat_Picto, int , XDY_District);

#endif
