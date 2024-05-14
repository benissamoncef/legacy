/*E*/
/*Fichier : $Id: evid_don.c,v 1.6 2017/03/27 12:20:59 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2017/03/27 12:20:59 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module des donnees internes a la tache VIDEO
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	28 Oct 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	:
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise historique
* JPL		03/11/16 : Suppression listes des LTV, moniteurs et magnetos (DEM 1189)  1.5
* JPL		24/03/17 : Definition de donnees publiques  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "evid_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_don.c,v 1.6 2017/03/27 12:20:59 devgfi Exp $ : evid_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EVID_DONNEES_CAMERA, ed_ins_liste_Camera, pg_debutCamera, Config.Numero, Suivant, Config)
EX_FONCT_SUPPRIMER ( EVID_DONNEES_CAMERA, ed_sup_liste_Camera, pg_debutCamera, Config.Numero, Suivant)	

//EX_FONCT_INSERER   ( EVID_DONNEES_MONITEUR, ed_ins_liste_Moniteur, pg_debutMoniteur, Config.Numero, Suivant, Config)
//EX_FONCT_SUPPRIMER ( EVID_DONNEES_MONITEUR, ed_sup_liste_Moniteur, pg_debutMoniteur, Config.Numero, Suivant)	


/* definition de variables publiques */

EVID_DONNEES_SYSTVIDEO	vg_sysvid[ MAX (EVIDC_SYSV1, EVIDC_SYSV2) + 1 ];
int			vg_noSV;

INFO_EQT_FEN_LCR	*pg_DebutListeVIDFLcr = NULL;

int			vg_Mode_Fonct;
int			vg_SockTIM;


/* definition de fonctions publiques */

