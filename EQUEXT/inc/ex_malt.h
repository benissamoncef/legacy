/*E*/
/*Fichier : $Id: ex_malt.h,v 1.6 2007/11/19 18:01:08 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2007/11/19 18:01:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_malt.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier d'envoi des alertes
*  commun au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Oct 1994	: Creation
* Mismer.D	version 1.2	23 Nov 1994	:
* Mismer.D	version 1.3	28 Nov 1994	: Suppression des #define XDF
* Volcic.F	version 1.4	19 Avr 1995	: 
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* JMG   13/11/07        : ajout site de gestion DEM715
*
------------------------------------------------------ */
#ifndef EX_MALT
#define EX_MALT

/* fichiers inclus */

#include <stdio.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/time.h>

#include "xdm.h"
#include "xdy.h"
#include "xdf.h"
#include "xzst.h"
#include "xzss.h"
#include "xzsc.h"
#include "xzaac.h"
#include "ex_mdon.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'une alerte vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

/*extern	int ex_env_alerte  ( 	XDY_Horodate	vl_Horodate,
				XDY_TypeEqt	vl_TypeEqt,
				XDY_Eqt		vl_NumEqt,
				XDY_TypeAlerte	vl_Type,
				int		vl_Mode,
				XDY_District	vl_SiteGestion,
				... );*/


/*
* ARGUMENTS EN ENTREE :
*
*  
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Recherche du texte de l'alerte et mise a jour de celui-ci
*  Construction du message et envoi du message si pas mode isole
*  Si mode isole ou erreur envoi msg alors ecriture du message dans fichier
*
------------------------------------------------------*/

extern void ex_liberation_memoire_alerte ();



#endif
