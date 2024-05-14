/*E*/
/* Fichier : $Id: ereg_util.h,v 1.4 2013/08/28 09:19:53 pc2dpdy Exp $    Release : $Revision: 1.4 $        Date :  $Date: 2013/08/28 09:19:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		26/03/2012 : Creation DEM/1016
* JMG           17/05/13 : fonction reinit_contexte 1.6
------------------------------------------------------ */

#ifndef ereg_util
#define ereg_util

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include "ereg_cfg.h"
#include "ereg_trace.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */
#define rtrim(c) \
	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0'
/*	int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0';if (c[0]==' ') c[0]='\0'*/

/* declarations de variables globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  reinit contexte des zones
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void reinit_contexte();
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  recherche d'un PRV dans une liste chain�e de PRV � partir d'un num�ro d'�quipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_prv(XDY_Eqt va_numEquipement, T_Liste_PRV *pa_listePRV, T_PRV **pa_result);
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt : numero d'�quipement
*  T_Liste_PRV* : liste chain�e des PRV
*
* ARGUMENTS EN SORTIE :
*  T_PRV* : pointeur vers le PRV trouv�. NULL si non trouv�
*
* CODE RETOUR :
*  XDC_OK : un PRV a �t� trouv�
*  XDC_NOK : aucun PRV trouv�
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche du PRV qui a le num�ro d'equipement pass� dans en parametre
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'une STATION dans une liste chainee de Stations apartir d'un numero de station
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_station(XDY_Entier va_numStation, XDY_Octet va_sens,T_Liste_Station *pa_liste, T_STATION **pa_result);
/*
* ARGUMENTS EN ENTREE :
*  XDY_Entier va_numStation 	: numero de la station
*  T_Liste_Station *pa_liste    : liste chainee
*
* ARGUMENTS EN SORTIE :
*  T_STATION **pa_result        : renvoie un pointeur vers la Station trouvee. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK :
*  XDC_NOK :
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche de la Station qui a le numero passe en parametre
*
------------------------------------------------------*/

XDY_Entier rechercher_condition(XDY_Entier va_numero, T_Liste_Condition_Zone *pa_liste, T_CONDITION **pa_result);




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  indique si un PRV est critique pour un scenario donn�
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen est_prv_critique_pour_scenario(T_PRV va_Prv, T_SCENARIO va_scenario);
/*
* ARGUMENTS EN ENTREE :
*  T_PRV 	  va_Prv 		: PRV concern�
*  T_SCENARIO va_scenario	: Scenario Concern�
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_VRAI si le PRV est critique pour le sc�nario
*  XDC_FAUX si le PRV n'est pas critique pour le sc�nario
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche parmis les commandes du scenario celle qui concerne le PRV et renvoi le booleen critique
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  indique si un scenario contient un PRV critique en panne
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen scenario_has_prv_critique_en_panne(T_SCENARIO *pa_scenario);
/*
* ARGUMENTS EN ENTREE :
*  T_SCENARIO *pa_scenario	: pointeur vers le Scenario Concern�
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_VRAI si un PRV critique en panne a �t� trouve
*  XDC_FAUX si aucun PRV critique en panne a �t� trouve
*
* CONDITION D'UTILISATION
*	aucune
*
* FONCTION
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'un SCENARIO dans une liste chainee de Zones a partir d'un ID scenario
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_scenario(XDY_Eqt va_idScenario, T_Liste_ZonesRegul *pa_listeZones, T_SCENARIO *pa_result);
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt      va_idScenario 	: id du scenario
*  T_Liste_Zone *pa_listeZones  : liste chainee des Zones
*
* ARGUMENTS EN SORTIE :
*  T_SCENARIO*       pa_result        : renvoie un pointeur vers le Scenario. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche du SCENARIO qui a l'id passe dans en parametre
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'une ZONE dans la liste chainee des Zones a partir d'un numero de Zone
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_zone(XDY_Eqt va_numeroZone, T_ZONE_REGUL **pa_result);
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt      va_numeroZone 	: numero de la zone
*
* ARGUMENTS EN SORTIE :
*  T_ZONE_REGUL *pa_result  	: renvoie un pointeur vers la zone. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche de la ZONE qui a le numero passe en parametre
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

/*
int genererAlerte (T_ZONE_REGUL *pa_zone,
						XDY_Octet va_sous_type_alerte,
						XDY_Entier va_type);

int genererAlerte_Old (T_ZONE_REGUL *pa_zone,
						XDY_Octet va_sous_type_alerte,
							XDY_Entier va_scenario,
							T_Contexte va_contexte,
							   XDY_Entier va_type);
*/

/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucunoid passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade)
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen isRegulationEnCours (T_ZONE_REGUL *pa_zone);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/

int genererTexteAlerteReveil(T_REGULATION va_regulation, char *pa_result);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Stationoid passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade)
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int creerFMCRegulation (T_ZONE_REGUL *pa_zone, T_Contexte va_contexte, XDY_Entier va_num_alerte);

/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int terminerFmcRegulation (T_ZONE_REGUL *pa_zone, XDY_Booleen va_avec_PA);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int updateFmcRegulation (T_ZONE_REGUL *pa_zone, XDY_Booleen);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int reinitTempoForcage (T_ZONE_REGUL *pa_zone);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Stationoid passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade)
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int annulerForcage (T_ZONE_REGUL *pa_zone);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int declencherForcage (T_ZONE_REGUL *pa_zone);

int declencherArret (T_ZONE_REGUL *pa_zone);

/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int updateRegulationProposee (T_ZONE_REGUL *pa_zone,
								T_REGULATION va_regulation);


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Calcule de la valeur Entiere du contexte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Entier calculerContexte (T_Contexte va_contexte);
/*
* ARGUMENTS EN ENTREE :
* T_Contexte va_contexteoid passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade)
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  XDY_Entier : la valeur du contexte
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Calcul de la valeur entiere du contexte par application des masques de bit
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  comparer contexte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen comparerContexte (T_Contexte va_contexte1, T_Contexte va_contexte2);
/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
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
*   creation de la liste des zones
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'une ZONE dans la liste chainee des Zones a partir d'un numero de Zone
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade, char *va_str);
/*
* ARGUMENTS EN ENTREE :
*  T_ZONE_REGUL      pa_zone 	: zone
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Passe la zone en mode d�grad�e si toutes les stations de la zone n'ont pas de donn�es trafic
*   et si la zone est en mode horaire forc�e
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  indique si le contexte de regulation a change
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen isNewContext (T_ZONE_REGUL *pa_zone);
/*
* ARGUMENTS EN ENTREE :
*  T_ZONE_REGUL      pa_zone 	: zone
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/


#endif
