/*E*/
/*Fichier : $Id: xzes.h,v 1.5 1995/05/24 17:54:47 cornu Exp $      Release : $Revision: 1.5 $        Date :$Date: 1995/05/24 17:54:47 $ 
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZES  * FICHIER xzes.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzes.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	30 Dec 1994	: Creation
* Gabert.P	version 1.2	05 Jan 1995	:
* Gabert.P	version 1.3	05 Jan 1995	:
* Gabert.P	version 1.4	16 Jan 1995	:
* Fontaine.C	version 1.5	24 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef XZES
#define XZES

/* fichiers inclus  */

#include <xdy.h>
#include <xdc.h>
#include "xzst.h"
#include "xdm.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

typedef struct {
	        XDY_Entier	NoEvt;
                XDY_Entier	CleEvt;
		XDY_Horodate	Horodate;

	} XZEST_FIC_SAT;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*-------------------------------------------
* SERVICE RENDU : 
*  Demande de creation d'un fichier evenement
*
---------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZES01_Creer_Fichier_SATIR (XZEST_FIC_SAT, XDY_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Fichier_SAT	Info permettant de retrouver l'evenement en BD
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la saisie d'un fichier ACCIDENT
*
* FONCTION 
*   Construction du message XDM_ECMD_FIC en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SATIR_<NomMachine> du site concerne
*
------------------------------------------------------*/

#endif
