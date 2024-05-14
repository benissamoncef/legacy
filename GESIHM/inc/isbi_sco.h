/*E*/
/*E Fichier : $Id: isbi_sco.h,v 1.3 2012/05/29 10:08:11 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2012/05/29 10:08:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSCO * FICHIER MSCO.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	08 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef MSCO
#define MSCO

/* fichiers inclus */

#include "xzst.h"
#include "xzsc.h"
#include "xzsm.h"
#include "xzea.h"
#include "xzis.h"
#include "xdg.h"
#include "xdy.h"


/* definitions de constantes exportees */

#define CM_COURBE_AUCUNE	0
#define CM_COURBE_FT 		1
#define CM_COURBE_INRETS	2
#define CM_COURBE_TDP		3
#define CM_COURBE_STRA		4
#define CM_COURBE_FT_REF	5
#define CM_COURBE_TDP_MZ	6
#define CM_COURBE_FT_DETAIL 10

#define CM_COURBE_FT_REEL      11
#define CM_COURBE_FT_EXTRPOLEE 12

#define CM_PERIODE_MESURES	360


#define CM_6mn		 6
#define CM_COEF_AFF 	1.2
#define CM_NB_SLOTIME  (int)(360/CM_COEF_AFF)

#define CM_POSIT_HEURE 11
#define CM_LG_HEURE    5

#define CM_NB_SLOTIME_H1  (int)(60/CM_COEF_AFF)
#define CM_NB_DONNEES_H1 50 /* =  (60/CM_COEF_AFF) */

#define CM_NB_MAX_FENETRES 3
#define CM_FENETRE_GAUCHE 0
#define CM_FENETRE_DROITE 1
#define CM_FENETRE_LARGE 2

#define CM_MAX_COURBES_FT 4
#define CM_MAX_COURBES_FT_REF 2 /* parametres des graphes temps avec reference */
#define CM_MAX_COURBES_TDP 10

#define CM_6HEURES 6*60*60

#define CM_LL_X_GRAPH  -13500
#define CM_LL_Y_GRAPH  -9000
#define CM_UR_X_GRAPH   8500 /*10000*/
#define CM_UR_Y_GRAPH   8000

#define CM_LL_X_GRAPH_H1  10600 /*10100*/
#define CM_LL_Y_GRAPH_H1  -9000
#define CM_UR_X_GRAPH_H1   16300 /*15700*/ /*17500*/
#define CM_UR_Y_GRAPH_H1   8000


#define CM_COULEUR_BOUCHON  8
#define CM_COULEUR_EVT     12

#define CM_54_MINUTES_DE_DONNEES 9
#define CM_UNE_HEURE_DE_DONNEES 10

#define CM_NB_SLOT_TDP 240


#define CM_NB_SLOTIME_FT_REF_PAR_H 50 /*(int) (60/CM_COEF_AFF)*/

#define CM_DUREE_SLOTIME_FT_REF (3600 / CM_NB_SLOTIME_FT_REF_PAR_H)

#define CM_NB_SLOTIME_FT_REF    (3 * CM_NB_SLOTIME_FT_REF_PAR_H)
#define CM_NB_SLOTIME_H4_FT_REF (4 * CM_NB_SLOTIME_FT_REF_PAR_H)

#define CM_NB_MESURES_FT_REF    (3 * CM_UNE_HEURE_DE_DONNEES)
#define CM_NB_MESURES_FT_REF_H4 (4 * CM_UNE_HEURE_DE_DONNEES)

#define CM_LL_X_GRAPH_FT_REF  -12400
#define CM_LL_Y_GRAPH_FT_REF  -9000
#define CM_UR_X_GRAPH_FT_REF   1100
#define CM_UR_Y_GRAPH_FT_REF   8000

#define CM_LL_X_GRAPH_H4_FT_REF  -100
#define CM_LL_Y_GRAPH_H4_FT_REF  -9000
#define CM_UR_X_GRAPH_H4_FT_REF   15500
#define CM_UR_Y_GRAPH_H4_FT_REF   8000


/* definitions de types exportes */
typedef struct {
               XDY_Texte nom_station;
               XDY_Eqt no_station;
               XDY_Octet mesure;
               XDY_Voie voie;
               XDY_Sens sens;
               } tm_varcaract;
               
typedef struct {
               XDY_Eqt no_zone;
	       char nom_zone[25];
               } tm_varcaractdp;
               
typedef struct {
               XDY_Booleen   periode;
               XDY_Eqt    no_station;
               tm_varcaract varcaract[CM_MAX_COURBES_FT]; /*DEM513 */
               XDY_Octet   nb_courbes;
               XDY_Booleen tempsreel;
               XDY_Horodate horodeb;
               XDY_Octet   fenetre;
               XDY_Octet   echelle;
              } tm_caracteristique_courbe;

typedef struct {
               tm_varcaractdp *varcaractdp;
               XDY_Octet   nb_courbes;
              } tm_caracteristique_courbe_tdp;

typedef struct {
               XDY_Octet mesure;
               XDY_Mot valeur;
               } tm_une_donnee_H1;
               
typedef struct {
               XDY_Octet type;
               XDY_Octet fenetre;
               } tm_memocv;


typedef tm_une_donnee_H1 tm_donnees_H1[CM_MAX_COURBES_FT];
     
typedef struct {
        float Valeur;
        XDY_Octet Validite;
	XDY_Horodate Horodate;
	} mon_XZIST_LigneDonnees;    
      
typedef struct {
        float Valeur;
        XDY_Octet Validite;
	} mon_XZIST_LigneDonnees_tdp;    

typedef mon_XZIST_LigneDonnees tm_Tab_LignesDonnees[XZISC_NB_LIG_MESURES+CM_UNE_HEURE_DE_DONNEES];
typedef mon_XZIST_LigneDonnees_tdp tm_Tab_LignesDonnees_tdp[XZISC_NB_LIG_MESURES+CM_UNE_HEURE_DE_DONNEES];

typedef char tm_chaine256[256]; 


/* definition de macro exportees */

/* declarations de donnees globales */

/* memorisation du nb et du type de courbes */
extern tm_memocv vm_memo_courbes[CM_NB_MAX_FENETRES];
extern int       vm_nb_courbes;

extern XZSCT_NomMachine vm_NomMachine;
extern XZSCT_NomSite vm_NomSite;
extern int	    vm_TypeMachine;

extern XDY_Booleen vm_courbe_valide;

extern char      vm_chaine_vide[1];
extern tm_chaine256 vm_lib_bouchon[CM_NB_MAX_FENETRES];
extern tm_chaine256 vm_lib_evt[CM_NB_MAX_FENETRES];

extern XDY_Horodate vm_horodeb_bouchon[CM_NB_MAX_FENETRES],vm_horofin_bouchon[CM_NB_MAX_FENETRES];
extern XDY_Horodate vm_horodeb_evt[CM_NB_MAX_FENETRES],vm_horofin_evt[CM_NB_MAX_FENETRES];

extern int       vm_existe_bouchon[CM_NB_MAX_FENETRES];
extern int       vm_existe_evt[CM_NB_MAX_FENETRES];

/* delaration de fonctions externes */

#endif
