/*E*/
/*Fichier : $Id: xzed.h,v 1.3 1995/05/24 17:54:31 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/24 17:54:31 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZED  * FICHIER xzed.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzed.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E		version 1.1	17 Oct 1994	: Creation
* Nagiel.E		version 1.2	16 Nov 1994	:
* Fontaine.C	version 1.3	24 Mai 1995	: Reprise entete, historique
* GGY	20/11/23 : Ajout XZED03_Demande_Enregistrement_DAI (DEM483)
------------------------------------------------------ */

#ifndef XZED
#define XZED

/* fichiers inclus  */

#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzec.h"

#include "xzst.h"

/* definitions de constantes exportees */


/* definitions de types exportes */

typedef XDY_EMes_DAI		XZEDT_EMes_DAI;
typedef	XDY_EConf_Voies_DAI	XZEDT_Config_Voies;
                                
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage sur PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZED01_Config_Voies_DAI (	XZEDT_Config_Voies  va_Config_Voies,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Config_Voies	Configuration des voies
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
*   Permet de configurer les DAI 
*
* FONCTION 
*   Envoie le message XDM_EConfig_Voies_DAI sur le 
*   datagroup XDG_EConf_Voie_DAI_<NomMachine>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZED02_Demande_Init_DAI (	XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					XDY_Eqt		va_NumEqt,
					int *		pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Eqt	va_NumEqt		: Numero d'analyseur
* XDY_Nom	pa_NomSite		: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir les mesures DAI d'un analysuer d'un site
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups correspondant
*   Envoi du message demande d'init
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZED03_Demande_Enregistrement_DAI (	XDY_Eqt			vl_NumEqt,
										XDY_Nom			vl_cmd,
										XDY_LongEntier	vl_Duree,
										int *			pa_Resultat );
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Eqt			vl_NumEqt		: Numero de l'equipement
* XDY_Nom			vl_cmd			: Peut valoir 'D' ou 'A' : demarrage / arret de l'enregistrement
* XDY_LongEntier	vl_Duree		: Duree de l'enregistrement en secondes (<3600)
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat				: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour declencher l'enregistrement de la video
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups correspondant
*   Envoi du message demande d'enregistrement video
*
------------------------------------------------------*/

#endif
