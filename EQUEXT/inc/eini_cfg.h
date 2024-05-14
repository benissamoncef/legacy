/*E*/
/* Fichier : $Id: eini_cfg.h,v 1.7 2019/01/29 11:06:06 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2019/01/29 11:06:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere les structures de donnees de la tache TEINI.
*  La variable globale pg_ListeMachine contient le pointeur de debut de liste.
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	14 Dec 1994	:
* Fontaine.C	version 1.3	19 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.4	19 Mai 1995	: Ajout Utilisateurs
* XXX	13/02/2008 : Nombre de fichiers de configuration augmente a 6  1.5
* JPL	15/02/2012 : Ajout declaration fonctions ec_rang_eqt, ec_write_fic (avec DEM 1018)  1.6
* JMG	07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.7
------------------------------------------------------ */

#ifndef eini_cfg
#define eini_cfg

/* fichiers inclus */

#include "eini_don.h"
#include "eini_lcr.h"

/* definitions de constantes exportees */

/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de variables globales */

extern	EINI_MACHINE *		pg_ListeMachine;
extern	EINI_MACHINE *		pg_MachineCrt;
extern	XDY_Fichier		vg_FdConf[6];

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_mise_a_jour_config_ligne ( 	int		va_NumLiaison,
						char *		pa_NomServeur,
						int		va_Protocole,
						int		va_Sens,
						XDY_Eqt		va_TypeEqt,
						XDY_Eqt		va_NumEqt,
						int		va_vitesse);

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Ecrire les parametres dans le fichier de config
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Appel du module de MALL et des modules de Mzzz selon le message XDM_ECONFIG_EQPT.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_config ( XDY_NomMachine	pa_NomMachine, int va_TypeEqt );

/*
* ARGUMENTS EN ENTREE :
*  XDY_NomMachine	pa_NomMachine	: Nom de la machine (une ou toutes)
*  int 			va_TypeEqt	: Type d'equipement concernee (un ou tous)

*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel≈e par ed_config_cb et au lancement de la tche TEINI
*
* FONCTION 
*  Appel de MALL et appel sequentiel des modules Mzzz selon le message specifie
*  Appel de MSYN en donnant le nom de la machine (une ou toutes) et le type de configuration
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_all ( XDY_NomMachine	pa_NomMachine, int va_TypeEqt );

/*
* ARGUMENTS EN ENTREE :
*  XDY_NomMachine	pa_NomMachine	: Nom de la machine (une ou toutes)
*  int 			va_TypeEqt	: Type d'equipement concernee (un ou tous)
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR : 
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION 
*   Recherche de la liste des machines du site concerne
*   Pour chaque machine initialise les fichier de config (XDF_Config_Ligne et 
*   XDF_Presence_Equip)
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_eqt (		XDY_NomMachine	pa_NomMachine,
				int		va_NumEqt );

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine
*   int			va_NumEqt
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Detruire les anciens fichiers
*   Creer les fichiers de config
*   Lire en base de donnees les parametres de configuration du type d'equipement concerne
*   Ecrire les donnees dans le fichier de configuration
*   Fermer les fichiers de configuration
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_RAU (	XZAOT_PAU va_PAU );

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Ecrire les parametres dans le fichier de config
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_config_PMV (	XZAOT_PMV va_PMV, XDY_NomMachine  pa_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Ecrire les parametres dans le fichier de config
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_liste_machine (	XDY_NomMachine	pa_NomMachine,
				int		va_NumOrdre,
				void * 		pa_Arg );

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine
*   int			va_NumOrdre
*   void * 		pa_Arg
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Reservation memoire
*   Insertion de la nouvelle machine 
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Ecriture d'une ligne en fichier de configuration.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_write_fic (	XDY_Fichier	va_Fd,
				char *		pa_TexteFile,
				int		va_LgTexte,
				char *		pa_NomFic );

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Localisation des donnees d'un type d'equipements
*  dans la configuration.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_rang_eqt ( int	va_TypeEqt );

/*
* ARGUMENTS EN ENTREE :
*   int			va_TypeEqt
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction creation du fichier des profils utilisateurs maintenance
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_creation_fich_utilisateurs ( );

/*
* ARGUMENTS EN ENTREE :
*   int			va_TypeEqt
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
------------------------------------------------------*/

#endif
