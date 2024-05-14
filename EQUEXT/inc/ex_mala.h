/*E*/
/*Fichier : $Id: ex_mala.h,v 1.13 2021/11/24 15:24:36 devgfi Exp $      Release : $Revision: 1.13 $        Date : $Date: 2021/11/24 15:24:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mala.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier d'envoi des alarmes
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	26 Oct 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	: Suppression des #define XDF
* Mismer.D	version 1.3	09 Dec 1994	:
* Mismer.D	version 1.4	06 Jan 1995	:
* Mismer.D	version 1.5	09 Fev 1995	:
* Mismer.D	version 1.6	10 Mai 1995	:
* Fontaine.C	version 1.7	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.8	04 Oct 1995	: Ajout Aknoledge alarme
* Mismer.D	version 1.9	07 Fev 1996	: Ajout fonction ex_sup_alarme_eqt_HS
* Mismer.D	version 1.10	13 Nov 1996	: Modif METEO (libelle alarme) (DEM/1232)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	30/11/11        : Ajout declaration de "ex_env_alerte_localisee" (DEM 1013)  1.12
* JPL	17/11/21        : Ajout declaration de "ex_traiter_alarmes" (SAE_339)  1.13
------------------------------------------------------ */

#ifndef EX_MALA
#define EX_MALA

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
#include "xzsm.h"
#include "xzamc.h"
#include "ex_mdon.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
typedef struct
{
	XDY_Horodate 	Horodate;
	XDY_TypeEqt	TypeEqt;
	XDY_Eqt		NumEqt;
	XDY_TypeAlarme	TypeAlarme;
	XDY_Booleen	Etat;
	XDY_NomMachine	NomMachine;
	XDY_District	SiteGestion;
        void *          Suivant;
} EX_MALAT_ALARME ;

typedef	struct	
{
	int		Numero;
	XDY_TxtAlarme	Libelle;
	XDY_Mot		Urgence;
	void *		Suivant;
}	EX_MALAT_INFO_ALARME ;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'une alarme vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_env_alarme  ( 	XDY_NomMachine	pa_NomMachine,
				XDY_Horodate	va_Horodate,
				XDY_TypeEqt	va_TypeEqt,
				XDY_Eqt		va_NumEqt,
				XDY_TypeAlarme	va_Type,
				XDY_Booleen	va_Etat,
				XDY_Texte	va_ArgAlarme,
				XDY_District	va_SiteGestion);

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
*  Construction du message et envoi du message si pas mode isole
*  Si mode isole ou erreur envoi msg alors ecriture du message dans fichier
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'une alerte vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ex_env_alerte  ( 	XDY_Horodate	va_Horodate,
				XDY_TypeEqt	va_TypeEqt,
				XDY_Eqt		va_NumEqt,
				XDY_TypeAlerte	va_Type,
				int		va_Mode,
				XDY_District	va_SiteGestion,
				... );


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



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'une alerte localisee vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ex_env_alerte_localisee  ( 	XDY_Horodate	va_Horodate,
					XDY_TypeEqt	va_TypeEqt,
					XDY_Eqt		va_NumEqt,
					XDY_TypeAlerte	va_Type,
					int		va_Mode,
					XDY_District	va_SiteGestion,
					char		*va_autoroute,
					XDY_PR		va_PR,
					XDY_Sens	va_sens,
					... );


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



extern int ex_traite_alarme_com ( 	XDY_NomMachine,
					int,
					int,
      					int,
      					int,
      					int,
      					int,
      					int,
      					int );


extern int ex_env_etat_liaison ( 	XDY_NomMachine,
					XDY_Datagroup,
					XDY_Eqt,
					XDY_Eqt );
 
extern void ex_traiter_alarmes ( );

 
extern void ex_init_alarme     ( 	XDY_NomMachine,
 					char *  );


extern void ex_sup_alarme_eqt_HS (	XDY_NomMachine,
					XDY_TypeEqt,
					XDY_Eqt,
					XDY_District);



#endif
