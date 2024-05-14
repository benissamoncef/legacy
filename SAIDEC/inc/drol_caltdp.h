/* Fichier : $Id: drol_caltdp.h,v 1.7 2017/10/10 17:36:45 devgfi Exp $        Release : $Revision: 1.7 $        Date : $Date: 2017/10/10 17:36:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN  * FICHIER drol_caltdp.h
******************************************************
* DESCRIPTION DU MODULE :
* module de calcul des temps de parcours
*
******************************************************
* HISTORIQUE :
*
*  Guilhou  03 fev 1997 : Creation
* JPL		14/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.3
* VR	28/11/11 : Ajout recherche_dest_pmv (DEM/1014)
* VR	09/06/12 : Ajout XZADT_CONFIG_PMA  XZAD_recherche_dest_pmva(DEM/1014 PMA)
* JPL	06/10/17 : Remplacement de xzao400 par xzao4001 (DEM 1236)  1.7
*******************************************************/

#ifndef drol_caltdp
#define drol_caltdp

/* fichiers inclus */
#include <xdf.h>
#include <xdy.h>
#include <xzst.h>
#include <xzsm.h>
#include <xzao4001.h>
#include <xzao401.h>
#include <xzad05.h>
#include <xzea.h>
#include <xzez.h>
#include <xzae.h>
#include <xzao463.h>
#include <xzao469.h>


/*definitions de constantes exportees */

/* definitions de types exportes */
typedef struct {
	XDY_Entier      Numero;
	XDY_Octet                       Destination;
	XZEZT_Liste_Zdp *               Graphe_Destination;
	XDY_Octet                       DestinationLointaine;
	XZEZT_Liste_Zdp *               Graphe_DestinationLointaine;
	XDY_Octet                       DestinationNoeud;
	XZEZT_Liste_Zdp *               Graphe_DestinationNoeud;
	void *                          suivant;
}
XZADT_CONFIG_PMV;

typedef struct {
	XDY_Entier      				Numero;
	XDY_Octet                       Dest_un_proche;
	XZEZT_Liste_Zdp *               Graphe_Dest_un_proche;
	XDY_Octet                       Dest_un_loin;
	XZEZT_Liste_Zdp *               Graphe_Dest_un_Loin;
	XDY_Octet                       Dest_deux_proche;
	XZEZT_Liste_Zdp *               Graphe_Dest_deux_proche;
	XDY_Octet                       Dest_deux_loin;
	XZEZT_Liste_Zdp *               Graphe_Dest_deux_loin;
	void *                          suivant;
}
XZADT_CONFIG_PMA;

typedef struct S_XZADT_Liste_Zdp
{
  XDY_ZDP			zdp;
  struct S_XZADT_Liste_Zdp*	suivant;
} XZADT_Liste_Zdp;

typedef struct S_XZADT_Liste_Destinations
{
  XDY_Destination_SAD			destination;
  struct S_XZADT_Liste_Destinations*	suivant;
} XZADT_Liste_Destinations;

/* declaration des donnees globales */
extern XDY_District     vg_site;

typedef struct S_XZAOT_Liste_PMV_Destinations
{
  XZAOT_PMV_DEST_TDP	destination_pmv;
  struct S_XZAOT_Liste_PMV_Destinations*	suivant;
} XZAOT_Liste_PMV_Destinations;


/* declaration des fonctions externes */

/* Fichier : @(#)drol_caltdp.h	1.3	Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZAD_TDP 
******************************************************
* DESCRIPTION DE LA FONCTION :
* initialise le module de calcul des temps de parcours
* et des destinations
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD20_Init_Calcul_TdP() 
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
extern int XZAD20_Init_Calcul_TdP();



/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZAD_TDP 
******************************************************
* DESCRIPTION DE LA FONCTION :
* met a jour le TDP 6 minutes pour une ZDP
* 
******************************************************
* SEQUENCE D'APPEL :
* XZAD21_Maj_TdP_ZdP(va_numero_zdp,
*		       va_tdp)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD21_Maj_TdP_ZdP(	XDY_Entier 	va_numero_zdp,
				XDY_TDP 	va_tdp);


/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* retourne le TDP 6 minutes pour une zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD22_Lire_TdP_ZdP(va_numero_zdp,
*			   va_site_zdp,
*			   *pa_tdp) 
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

extern int XZAD22_Lire_TdP_ZdP(	XDY_Entier 	va_numero_zdp,
				XDY_Mot 	va_site_zdp,
			       	XDY_TDP 	*pa_tdp);

/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* calcule le temps de parcours entre un PMV et une
* destination
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD23_Calculer_TdP_PMV_Dest(va_pr_pmv, 
*				va_autoroute_pmv,
*			        va_sens_pmv, 
*				va_destination,
*				pa_tdp)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
*
******************************************************/

extern int XZAD23_Calculer_TdP_PMV_Dest( 
                          XDY_PR 		va_pr_pmv,
                          XDY_Autoroute 	va_autoroute_pmv,
			  XDY_Sens 		va_sens_pmv,
                          int			va_destination,
			  XZEZT_Liste_Zdp	*pa_zdp,
			  XDY_TDP		*pa_tdp, char *va_nom,
        int       numPMV,
        int       typeEqt); 


/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* met a jour l'etat d'un franchissement de seuil
* pour une zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD24_Maj_Franchissement(	va_numero,
				va_type_franchissement)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD24_Maj_Franchissement(	XDY_Entier	va_numero,
					XDY_Octet	va_type_franchissement);



/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche les zones perturbees et revenues a la 
* normale et appelle la fonction utilisateur
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD26_Calculer_Zones_PMV(	va_numero_fmc,
				va_cle_fmc,
				pa_fct_util,
				va_pr,
				va_prtete,
				va_sens,
				va_type_fmc)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD26_Calculer_Zones_PMV(	XDY_Entier 	va_numero_fmc,
					XDY_Mot		va_cle_fmc,
					XDY_Fonction	pa_fct_util,
					XDY_PR		va_pr,
					XDY_PR		va_prtete,
					XDY_Sens	va_sens,
					XDY_Octet	va_type_fmc);

/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD30_Libere_Ressources_Calcul_TdP();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD30_Libere_Ressources_Calcul_TdP();



/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* abonnement au TDP pour les ZDP d'un site
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD27_Abonnement_TDP(        va_site,
				pa_fct_util,
				va_datagroup)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int XZAD27_Abonnement_TDP(       XDY_Mot         va_site,
					XDY_Fonction    pa_fct_util,
					XDY_Datagroup   va_datagroup);


/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD28_Libelle_Destination(va_dest,pa_libelle);
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int XZAD27_Libelle_Destination(XDY_Entier        va_dest,
					char            *pa_libelle);


#endif

/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* verifie qu'un PMV est sur une ZDP
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD28_PMV_Sur_ZDP(        va_autoroute_pmv,
                                va_sens_pmv,
                                va_pr_pmv)
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int XZAD28_PMV_Sur_ZDP(  XDY_Autoroute va_autoroute_pmv,
                                XDY_Sens va_sens_pmv,
                                XDY_PR va_pr_pmv);

/* Fichier : @(#)drol_caltdp.h	1.3        Release : 1.3        Date : 09/30/97
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* reinitialise l'etat d'un franchissement de seuil
*  des zdp
*
******************************************************
* SEQUENCE D'APPEL :
* XZAD29_Reinit_Franchissement()
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int XZAD29_Reinit_Franchissement();


extern int XZAD_recherche_dest_fmc(XDY_Autoroute va_autoroute,
			XDY_PR va_pr_debut, 
			XDY_PR va_pr_fin, 
			XDY_Sens va_sens, 
			XDY_Destination_SAD       *pa_dest);

extern int XZAD_recherche_dest_pmv(XDY_Entier va_numero, 
			XZADT_CONFIG_PMV *pa_dest
			);

extern int XZAD_recherche_dest_pmva(XDY_Entier va_numero,
			XZADT_CONFIG_PMA *pa_dest, XDY_Sens va_sens,
			XDY_Autoroute va_autoroute,
			XDY_PR va_pr
			);

extern int XZAD30_Calculer_Traversee( 
                          XDY_Autoroute         va_autoroute_pmv,
                          XDY_PR                va_pr_pmv,
                          XDY_Sens              va_sens_pmv,
                          XDY_PR                va_pr_min,
                          XDY_PR                va_pr_max,
                          XZEZT_Liste_Zdp       *pa_zdp,
                          XDY_TDP               *pa_tdp);
/* fin de fichier */
