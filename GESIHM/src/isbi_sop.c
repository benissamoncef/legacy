/*E*/
/*Fichier : $Id: isbi_sop.c,v 1.7 2020/07/15 09:44:26 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date :  $Date: 2020/07/15 09:44:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSOP * FICHIER isyn_sop.c
-----------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Prise en compte des actions operateur sur Dataviews
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	13 Sep 1994	: Creation
* VR		30 mai 2012 : PRV DEM 1016
* JMG		13/08/12 : reactivation MSOP_Affichage_Video 1.4
* JMG		15/03/13 : projeter
* JMG		08/11/16 : SAGA 1.5
* LCL	27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sop.h"
#include "isbi_sco.h"
/* definitions de constantes */
#define CM_MAX_NOM_AIRE 20
#define CM_SEPAR_ECH " "     /*separateur du nom de l'echangeur */
#define CM_SEPAR_AIR "_"     /*separateur du nom de l'aire */
#define CM_IDENT     "ISYN_SOP_"

#define CM_NB_AUTOROUTE 8
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : isyn_sop" ;

extern XZSCT_NomMachine vm_NomMachine;
extern XDY_Nom vm_NomMachineSS;
extern XZSCT_NomSite vm_NomSite;

/* declaration de fonctions internes */
void MSOP_receptCommandeBouton();
void MSOP_receptCommandeEquipement();
void MSOP_receptCommandeMobile();
void MSOP_receptCommandeObjet();
void MSOP_receptCommandeTigmot();

int MSOP_Envoyer();
int MSOP_InfoInterv();
int MSOP_tdp();
int MSOP_Vide_Equipement();
int MSOP_Vide_Mobile();
int MSOP_Vide_Objet();
int MSOP_Iris_Vision();
int MSOP_Affichage_Video();

int MSOP01_utilisation_equipement();
int MSOP02_synthese_fmc();
int MSOP03_informations_symbole();
int MSOP04_piloter();
int MSOP06_piloter_PMV();
int MSOP07_annuaire();
int MSOP08_piloter_camera();
int MSOP09_fmc();
int MSOP10_choisir_courbe();
int MSOP11_piloter_echangeur();
int MSOP12_piloter_tunnel();
int MSOP19_liste_astreinte();
int MSOP20_liste_Alarmes();
int MSOP21_liste_Indispo();
int MSOP26_choisir_video();
int MSOP27_projeter();  /*RETRO*/

/* definition de fonctions externes */

void MSOP_receptCommandeTigmot(pa_type_action,
	  pa_chaine)

	    int pa_type_action;
      char *pa_chaine;
{
   XZSCT_NomSSysteme     vl_ssys;
   XZSCT_NomTache	vl_tache;
   XDY_NomMachine	vl_machine;
   int vl_cmd;
   int vl_type_action;
   int vl_fonction, vl_message, vl_info, vl_sgbd, vl_regle, vl_ecran, vl_interface;
   int vl_d1, vl_d2, vl_d3;
   int                   vl_nivTrace    =0;
      int                   vl_visu        = XZSTC_NECRAN;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_receptCommandeTigmot: typeAction=%d; chaine=%s" , pa_type_action, pa_chaine );

   sscanf(pa_chaine,"%d",&vl_type_action);
   pa_chaine+=2;
   switch (vl_type_action) {
    case CM_TIGMOT_CMD_SOUS_SYSTEME:
      sscanf(pa_chaine,"%[^|]|%[^|]|%d",vl_machine,vl_ssys,&vl_cmd);
      XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : machine %s, sous-systeme %s, commande %d",
				vl_machine, vl_ssys,vl_cmd);
      XZSP_02CmdSSyst(vl_machine, vl_ssys,vl_cmd);
      break;

    case CM_TIGMOT_CMD_ARRET:
      XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : ARRET");
      MSMA_arret();
      break;

    case CM_TIGMOT_CMD_TACHE:
      sscanf(pa_chaine,"%[^|]|%[^|]|%d",vl_machine,vl_tache,&vl_cmd);
      XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : machine %s, tache %s, commande %d",
      				vl_machine,vl_tache, vl_cmd);
	XZSP_03CmdTache( vl_machine, vl_tache,vl_cmd);
	break;

    case CM_TIGMOT_CMD_TRACE:
      sscanf(pa_chaine,"%[^|]|%[^|]|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",vl_machine,vl_tache,
      		&vl_fonction,&vl_message,&vl_info,&vl_sgbd,
		&vl_d1,&vl_d2,&vl_d3,&vl_interface,&vl_regle,&vl_ecran);
	 XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : machine %s, tache %s, commnde fonction %d message %d info %d sgbd %d d1 %d d2 %d d3 %d interface %d regle %d ecran %d",
	 			vl_machine, vl_tache, vl_fonction, vl_message, vl_info, vl_sgbd, vl_d1, vl_d2, vl_d3, vl_interface, vl_regle, vl_ecran);
      if (vl_fonction==1)
        vl_nivTrace = vl_nivTrace | XZSTC_FONCTION;
      if (vl_message==1)
        vl_nivTrace = vl_nivTrace |XZSTC_MESSAGE;
	if (vl_info==1)
		vl_nivTrace = vl_nivTrace | XZSTC_INFO;
	if (vl_sgbd==1)
		vl_nivTrace = vl_nivTrace | XZSTC_SGBD;
	if (vl_d1==1)
		vl_nivTrace = vl_nivTrace | XZSTC_DEBUG1;
	if (vl_d2==1)
		vl_nivTrace = vl_nivTrace | XZSTC_DEBUG2;
	if (vl_d3==1)
		vl_nivTrace = vl_nivTrace | XZSTC_DEBUG3;
	if (vl_interface==1)
		vl_nivTrace = vl_nivTrace | XZSTC_INTERFACE	;
   	if (vl_regle==1)
		vl_nivTrace = vl_nivTrace | XZSTC_REGLE;
	if (vl_ecran==1)
		vl_visu = XZSTC_ECRAN;

   	XZST_06CmdNivVisuTrace( vl_machine, vl_tache,
			vl_nivTrace, vl_visu);
   	break;
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  traite la reception d'une commande Bouton:
*    aiguille vers la macro correspondante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MSOP_receptCommandeBouton(
		  pa_type_action,
          pa_chaine)

          int pa_type_action;
		  char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_objet : nom de l'objet
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   V_CONTINUE_TRAVERSAL
*   V_HALT_TRAVERSAL
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sop.c	1.73 09/02/10 :MSOP_receptCommandeMobile ";

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_receptCommandeBouton: typeAction=%d; chaine=%s" , pa_type_action, pa_chaine );

   XZST_03EcritureTrace(   XZSTC_INFO, "MSOP_receptCommandeBouton: typeAction=%d; chaine=%s" , pa_type_action, pa_chaine );

   /*A Test de l'existence de l'objet ou pas */
   switch( pa_type_action )
   {
	case CM_ACTION_BINTERV:
		MSOP19_liste_astreinte(pa_chaine);
		break;
	case CM_ACTION_BALARM:
		MSOP20_liste_Alarmes(pa_chaine);
		break;
	case CM_ACTION_BETAT:
		MSOP21_liste_Indispo(pa_chaine);
		break;
	case CM_ACTION_RAPPELCOURBE:
		MSCO_rappel_courbe(pa_chaine);
		break;
	case 299 :
		MSOP12_piloter_tunnel(pa_type_action,0,pa_chaine);
		break;
	default:
	   XZST_03EcritureTrace(   XZSTC_WARNING, "MSOP_receptCommandeBouton: typeAction(%d) Inconnu" , pa_type_action);
	   break;
   }
       XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_receptCommandeObjet" );
       return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  traite la reception d'une commande Mobile:
*    aiguille vers la macro correspondante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MSOP_receptCommandeEquipement(
		  pa_bouton,
		  pa_type_equipement,
          pa_num_equipement)

          int pa_bouton;
		  int pa_type_equipement;
		  int pa_num_equipement;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_objet : nom de l'objet
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   V_CONTINUE_TRAVERSAL
*   V_HALT_TRAVERSAL
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sop.c	1.73 09/02/10 :MSOP_receptCommandeMobile ";
   int vl_type=0;
   int (*fonction[3])();
   int vl_a_garder=XDC_OUI;
   int vl_type_equipement;


   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSOP_receptCommandeEquipementt bouton:%d; typeObj=%d; numObj=%d" , pa_bouton, pa_type_equipement, pa_num_equipement );

   if (pa_bouton < 0 || pa_bouton > 2)
	   return;

   fonction[0] = MSOP_Vide_Equipement ;
   fonction[1] = MSOP_Vide_Equipement ;
   fonction[2] = MSOP_Vide_Equipement ;

   /*A Test de l'existence de l'objet ou pas */
   switch( pa_type_equipement )
   {
   	   case CM_EQUIP_DAI:
		   vl_type_equipement = CM_ORD_DAI;
		   fonction[0] = MSOP_Vide_Equipement ; //MSOP_fca ne fait rien vers Applix;
		   fonction[1] = MSOP_Vide_Equipement ;
		   fonction[2] = MSOP_Vide_Equipement ;
	   break;
	   case CM_EQUIP_SAGA_PMV_1_18:
	   case CM_EQUIP_SAGA_PMV_2_18:
	   case CM_EQUIP_SAGA_PMV_2_15:
	   case CM_EQUIP_SAGA_PMV_3_15:
	   case CM_EQUIP_SAGA_PMV_2_18_P:
	   case CM_EQUIP_SAGA_PMV_3_15_P:
	   case CM_EQUIP_SAGA_BRA:
	   case CM_EQUIP_SAGA_BAF:
	   case CM_EQUIP_SAGA_PMVA:
		vl_type_equipement = pa_type_equipement;
		fonction[0] = MSOP_Vide_Equipement ;
		fonction[1] = MSOP_Vide_Equipement ;
		fonction[2] = MSOP04_piloter ;
		break;
	   case CM_EQUIP_CAMERA:
		   vl_type_equipement = CM_ORD_CAMERA;
           fonction[0] = MSOP_Vide_Equipement ;
           fonction[1] = MSOP_Affichage_Video ;
           fonction[2] = MSOP_Vide_Equipement ; //MSOP04_piloter;
           break;
	   case CM_EQUIP_NAV:
		   vl_type_equipement = CM_ORD_NAV;
           fonction[0] = MSOP01_utilisation_equipement ;
           fonction[1] = MSOP_Vide_Equipement ;
           fonction[2] = MSOP04_piloter ; //MSOP04_piloter_NAV
		   break;
	   case CM_EQUIP_BAF:
		   vl_type_equipement = CM_ORD_BAF;
	       fonction[0] = MSOP01_utilisation_equipement ;
	       fonction[1] = MSOP_Vide_Equipement ;
	       fonction[2] = MSOP04_piloter ;//MSOP04_piloter_BAF
	       break;
	   case CM_EQUIP_BAD:
		   vl_type_equipement = CM_ORD_BAD;
	       fonction[0] = MSOP01_utilisation_equipement ;
	       fonction[1] = MSOP_Vide_Equipement ;
	       fonction[2] = MSOP04_piloter;//MSOP04_piloter_BAD
	       break;
	   case CM_EQUIP_PRV:
		   vl_type_equipement = CM_ORD_PRV;
	       fonction[0] = MSOP01_utilisation_equipement ;
	       fonction[1] = MSOP_Vide_Equipement ;
	       fonction[2] = MSOP04_piloter;
	       break;
	   case CM_EQUIP_PAU:
		   vl_type_equipement = CM_ORD_PAU;
           fonction[0] = MSOP_Vide_Equipement ;
           fonction[1] = MSOP_Vide_Equipement ;
           fonction[2] = MSOP04_piloter ;//MSOP05_piloter_BRAU
		   break;
	   case CM_EQUIP_PMV:
   		   vl_type_equipement = CM_ORD_PMV;
           fonction[0] = MSOP01_utilisation_equipement ;
           fonction[1] = MSOP_Vide_Equipement ; //MSOP06_activerinactiver_PMV ;
           fonction[2] = MSOP06_piloter_PMV ;
		   break;
	   case CM_EQUIP_PMVA:
		   vl_type_equipement = CM_ORD_PMVA ;
           fonction[0] = MSOP01_utilisation_equipement ;
           fonction[1] = MSOP_Vide_Equipement;
           fonction[2] = MSOP04_piloter; //MSOP06_piloter_PMVA ;
		   break;
	   case CM_EQUIP_LIT:
   	   case CM_EQUIP_CANTON:
		   vl_type_equipement = -1;
		   break;
/*case	CM_EQUIP_PAT			0*/
/*case	CM_EQUIP_REF			0*/
	   case CM_EQUIP_TRAV_PREVUS:
		   vl_type_equipement = CM_ORD_TRAVAUX_FIXES_PREVUS ;
		   break;
/*case	CM_EQUIP_FERM		19*/
	   case CM_EQUIP_PAL:
		   vl_type_equipement = CM_ORD_PANALT;
           fonction[0] = MSOP01_utilisation_equipement ;
           fonction[1] = MSOP_Vide_Equipement ;
           fonction[2] = MSOP04_piloter; //MSOP06_piloter_Panneau_alerte ;
		   break;
	   case CM_EQUIP_ZDP:
		   vl_type_equipement = CM_ORD_ZDP;
           fonction[0] = MSOP_tdp ;
           fonction[1] = MSOP_Vide_Equipement ;
           fonction[2] = MSOP_Vide_Equipement ;
		   break;
	   default:
   		   vl_type_equipement = -1;
   		   XZST_03EcritureTrace(   XZSTC_WARNING, "Type d'equipement non reconnu : %d" , pa_type_equipement);
   		   break;
//case CM_ORD_ZDP [0]-> MSOP_tdp
//case CM_ORD_STRADA [0] -> MSOP_graph_strada_g (ou d)
/*
 *
 *          case CM_ORD_PANALT :
               fonction[0] = MSOP01_utilisation_equipement ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP06_piloter_Panneau_alerte ;
               break;
         case CM_ORD_FERM :
               fonction[0] = MSOP01_utilisation_equipement ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP06_piloter_Fermeture ;
               break;
        case CM_ORD_PEAGE :
               fonction[0] = MSOP03_informations_symbole ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP07_annuaire ;
               break;
         case CM_ORD_ITPC :
               fonction[0] = MSOP03_informations_symbole ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP_Vide ;
               break;
         case CM_ORD_VILLE:
               fonction[0] = MSOP_Vide ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP07_annuaire ;
               break;
         case CM_ORD_PANPOL :
               vl_a_garder = XDC_NON;
               fonction[0] = MSOP_Vide ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP_Vide ;
               break;
         case CM_ORD_PENTE :
               vl_a_garder = XDC_NON;
               fonction[0] = MSOP_Vide ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP_Vide ;
               break;
         case CM_ORD_TUNNEL:
               fonction[0] = MSOP03_informations_symbole ;
               fonction[1] = MSOP_vue_tunnel ;
               fonction[2] = MSOP12_piloter_tunnel ;
               break;
         case CM_ORD_PORTAIL:
               fonction[0] = MSOP03_informations_symbole ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP_Vide ;
               break;
         case CM_ORD_AIRE :
               fonction[0] = MSOP03_informations_symbole ;
               fonction[1] = MSOP_fca ;
               fonction[2] = MSOP07_annuaire ;
               break;
         case CM_ORD_PASSAGE 	:
               fonction[0] = MSOP03_informations_symbole ;
               fonction[1] = MSOP_Vide ;
               fonction[2] = MSOP07_annuaire ;
               break;

 *
*/


   }


XZST_03EcritureTrace(   XZSTC_DEBUG1, "type equip = %d, num equip = %d", vl_type_equipement, pa_num_equipement );

	if (vl_type_equipement != -1)
    {
    	  (fonction[pa_bouton])(vl_type_equipement, pa_num_equipement);
    }


    XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_receptCommandeEquipement" );
    return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  traite la reception d'une commande Mobile:
*    aiguille vers la macro correspondante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSOP_receptCommandeMobile(
		  pa_bouton,
		  pa_type_objet,
          pa_nom_objet)
          
          int pa_bouton;
		  int pa_type_objet;
		  char *pa_nom_objet;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_objet : nom de l'objet
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   V_CONTINUE_TRAVERSAL
*   V_HALT_TRAVERSAL
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sop.c	1.73 09/02/10 :MSOP_receptCommandeMobile ";
   int vl_type=0;
   int (*fonction[3])();
   int vl_a_garder=XDC_OUI;

   
   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSOP_receptCommandeMobile *pa_nom_objet:%s;bouton:%d;" , pa_nom_objet, pa_bouton );

   if (pa_bouton < 0 || pa_bouton > 2)
	   return;

   fonction[0] = MSOP_Vide_Mobile ;
   fonction[1] = MSOP_Vide_Mobile ;
   fonction[2] = MSOP_Vide_Mobile ;

   /*A Test de l'existence de l'objet ou pas */
//      switch( pa_type_objet )
   switch( vl_type=MSRA_Type_obj( pa_nom_objet ) )
         {
	 case CM_ORD_CAMERA:
               fonction[0] = MSOP_Affichage_Video;
               fonction[1] = MSOP_Vide_Mobile ;
               fonction[2] = MSOP08_piloter_camera ;
		break;
         case CM_ORD_TRAVAUX_MOBILES_PREVUS:
         case CM_ORD_TRAVAUX_MOBILES_REELS:
         case CM_ORD_TRAVAUX_FIXES_PREVUS:
         case CM_ORD_TRAVAUX_FIXES_REELS:
         case CM_ORD_TRAVAUX_MOBILES_TETE_PREVUS:
         case CM_ORD_TRAVAUX_MOBILES_TETE_REELS :
         case CM_ORD_TRAVAUX_MOBILES_QUEUE_PREVUS :
         case CM_ORD_TRAVAUX_MOBILES_QUEUE_REELS :
               fonction[0] = MSOP02_synthese_fmc ;
               fonction[1] = MSOP_Vide_Mobile ;
               fonction[2] = MSOP09_fmc ;
               break;
         case CM_ORD_INTERVENANT_GENDARMERIE:
         case CM_ORD_INTERVENANT_POMPIER:
         case CM_ORD_INTERVENANT_DEPANNEUR:
         case CM_ORD_INTERVENANT_FOURGON_SIS:
         case CM_ORD_INTERVENANT_PAT_ESCOTA:
         case CM_ORD_INTERVENANT_AUTRE:
         case CM_ORD_PMV_MOBILE:
         case CM_ORD_AFFICH_GARE:
             fonction[0] = MSOP_Vide_Mobile ;
             fonction[1] = MSOP_Vide_Mobile ;
             fonction[2] = MSOP_InfoInterv ;
             break;
         case CM_ORD_FMC_ACCIDENT:
         case CM_ORD_FMC_ACCIDENT_VL 	:
         case CM_ORD_FMC_ACCIDENT_PL 	:
         case CM_ORD_FMC_ACCIDENT_VLPL 	:
         case CM_ORD_FMC_ACCIDENT_CAR 	:
         case CM_ORD_FMC_ACCIDENT_VLCAR :
         case CM_ORD_FMC_ACCIDENT_PLCAR :
         case CM_ORD_FMC_ACCIDENT_VLPLCAR:
         case CM_ORD_FMC_ACCIDENT_REM 	:
         case CM_ORD_FMC_ACCIDENT_VLREM :
         case CM_ORD_FMC_ACCIDENT_PLREM :
         case CM_ORD_FMC_ACCIDENT_VLPLREM:
         case CM_ORD_FMC_ACCIDENT_CARREM :
         case CM_ORD_FMC_ACCIDENT_VLCARREM:
         case CM_ORD_FMC_ACCIDENT_PLCARREM:
         case CM_ORD_FMC_ACCIDENT_VLPLCARREM :
         case CM_ORD_FMC_ACCIDENT_MOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLMOTO 	:
         case CM_ORD_FMC_ACCIDENT_PLMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLPLMOTO 	:
         case CM_ORD_FMC_ACCIDENT_CARMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLCARMOTO 	:
         case CM_ORD_FMC_ACCIDENT_PLCARMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLPLCARMOTO 	:
         case CM_ORD_FMC_ACCIDENT_REMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLREMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_PLREMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLPLREMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_CARREMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLCARREMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_PLCARREMMOTO 	:
         case CM_ORD_FMC_ACCIDENT_VLPLCARREMMOTO :
         case CM_ORD_FMC_VEHICULE_CONTRE_SENS:
         case CM_ORD_FMC_VEHICULE_ARRETE:
         case CM_ORD_FMC_VEHICULE_PANNE:
         case CM_ORD_FMC_VEHICULE_FEU:
         case CM_ORD_FMC_CIRQUE :
         case CM_ORD_FMC_CONVOI_EXCEPTIONNEL:
         case CM_ORD_FMC_CONVOI_MILITAIRE:
         case CM_ORD_FMC_VEHICULE_LENT :
         case CM_ORD_FMC_MANIFESTATION:
         case CM_ORD_FMC_GREVE_ESCOTA:
         case CM_ORD_FMC_OPERATION_ESCARGOT :
         case CM_ORD_FMC_INCIDENT_SOUS_CONSC :
         case CM_ORD_FMC_EBOULEMENT :
         case CM_ORD_FMC_INCENDIE :
         case CM_ORD_FMC_INONDATION :
         case CM_ORD_FMC_OBSTACLE :
         case CM_ORD_FMC_CHAUSSEE_DETERIOREE :
         case CM_ORD_FMC_FLAQUE_HUILE :
         case CM_ORD_FMC_ANIMAL_ERRANT :
         case CM_ORD_FMC_FRONTIERE_FERMEE :
         case CM_ORD_FMC_ALTERCATION_USAGERS :
         case CM_ORD_FMC_AGRESSION :
         case CM_ORD_FMC_MALAISE_USAGER:
         case CM_ORD_FMC_HOLD_UP:
         case CM_ORD_FMC_PIETON_CHAUSSEE :
         case CM_ORD_FMC_AUTRE :
         case CM_ORD_FMC_VEHICULE_MARCHE_ARR :
         case CM_ORD_FMC_TRAITEMENT_CHAUSSEES :
         case CM_ORD_FMC_ECHANGEUR_DECONSEILLE :
         case CM_ORD_FMC_CHAUSSEE_GLISSANTE :
//   ??? case CM_ORD_FMC_ZONE_STOCKAGE :
         case CM_ORD_FMC_METEO_CHUTE_NEIGE:
         case CM_ORD_FMC_METEO_NEIGE_CHAUSSEE:
         case CM_ORD_FMC_METEO_GRELE_CHAUSSEE:
         case CM_ORD_FMC_METEO_PLUIE:
         case CM_ORD_FMC_METEO_VENT:
         case CM_ORD_FMC_METEO_BROUILLARD:
         case CM_ORD_FMC_METEO_BROUILLARD_GIVRANT :
         case CM_ORD_FMC_METEO_GRELE:
         case CM_ORD_FMC_METEO_VERGLAS:
         case CM_ORD_FMC_METEO_AUTRE:
	 case CM_ORD_FMC_REGULATION_VITESSE:
         case XDC_SYN_FMC_FERM_AIRE:
               fonction[0] = MSOP02_synthese_fmc ;
               fonction[1] = MSOP_Vide_Mobile ;
               fonction[2] = MSOP09_fmc ;
               break;
         case CM_ORD_BASCBOU :
         case CM_ORD_INVERSE :
         case CM_ORD_NEUTRA :
         case CM_ORD_BOUCHON :
               fonction[0] = MSOP02_synthese_fmc ;
               fonction[1] = MSOP_Vide_Mobile ;
               fonction[2] = MSOP09_fmc ;
               break;

         default : /*B Par defaut on associe la fonction vide aux objets */
               vl_a_garder = XDC_NON;
               fonction[0] = MSOP_Vide_Mobile ;
               fonction[1] = MSOP_Vide_Mobile ;
               fonction[2] = MSOP_Vide_Mobile ;
               break;
         }
      
      if (vl_a_garder == XDC_OUI)
      {
    	  (fonction[pa_bouton])(pa_nom_objet);
      }

      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_receptCommandeMobile" );
   return;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  traite la reception d'une commande Objet:
*    aiguille vers la macro correspondante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MSOP_receptCommandeObjet(
		  pa_bouton,
		  pa_type_equipement,
          pa_num_equipement,
          pa_chaine)

          int pa_bouton;
		  int pa_type_equipement;
		  int pa_num_equipement;
		  char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_objet : nom de l'objet
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   V_CONTINUE_TRAVERSAL
*   V_HALT_TRAVERSAL
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sop.c	1.73 09/02/10 :MSOP_receptCommandeMobile ";
   int vl_type=0;
   int (*fonction[3])();
   int vl_type_equipement;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_receptCommandeObjet bouton:%d; typeObj=%d; numObj=%d; chaine=%s" , pa_bouton, pa_type_equipement, pa_num_equipement, pa_chaine );

   if (pa_bouton < 0 || pa_bouton > 2)
	   return;
   fonction[0] = MSOP_Vide_Objet ;
   fonction[1] = MSOP_Vide_Objet ;
   fonction[2] = MSOP_Vide_Objet ;

   /*A Test de l'existence de l'objet ou pas */
   switch( pa_type_equipement )
   {
   /*case	CM_EQUIP_RESEAU		0 */
	case CM_EQUIP_PR:
		   vl_type_equipement = CM_ORD_PR;
		   fonction[0] = MSOP_Iris_Vision ;
		   fonction[1] = MSOP_Vide_Objet ;
		   fonction[2] = MSOP_Vide_Objet ;
		   break;

	case CM_EQUIP_RESEAU_ELECT:
	case CM_EQUIP_RESEAU_ELECT_MT:
	case CM_EQUIP_RESEAU_ELECT_BT:
	case CM_EQUIP_RESEAU_ELECT_HT:
	case CM_EQUIP_RESEAU_GAZ:
	case CM_EQUIP_RESEAU_EAU:
	case CM_EQUIP_RESEAU_AUTRE:
	   vl_type_equipement = CM_ORD_RESEAU;
	   fonction[0] = MSOP03_informations_symbole ;
	   fonction[1] = MSOP_Vide_Objet ;
	   fonction[2] = MSOP07_annuaire ;
	   break;
	case CM_EQUIP_ECHANGEUR:
	   vl_type_equipement = CM_ORD_ECHANGEUR;
	   fonction[0] = MSOP_Vide_Objet; //MSOP_vue_detaillee ; ne fait rien dans applix
	   fonction[1] = MSOP_Vide_Objet ;
	   fonction[2] = MSOP11_piloter_echangeur ;
	   break;
	case CM_EQUIP_LTV:
        if ( strcmp( vm_NomSite , XDC_NOM_SITE_CI ) != NULL )
     	   vl_type_equipement = -1;
        else
        {
     	   vl_type_equipement = CM_ORD_LTV;
           fonction[0] = MSOP_Vide_Objet ;
           fonction[1] = MSOP_Vide_Objet ;
           fonction[2] = MSOP26_choisir_video ;
        }
        break;
	case CM_EQUIP_TUBE:
	   vl_type_equipement = CM_ORD_TUNNEL;
	   fonction[0] = MSOP03_informations_symbole ;
	   fonction[1] = MSOP_Vide_Objet ;
	   fonction[2] = MSOP12_piloter_tunnel ;
	   break;

	case CM_EQUIP_PEAGE :
		vl_type_equipement = CM_ORD_PEAGE;
		fonction[0] = MSOP03_informations_symbole ;
		fonction[1] = MSOP_Vide_Objet ;
	    fonction[2] = MSOP07_annuaire ;
	   break;
     case CM_EQUIP_ITPC :
 		vl_type_equipement = CM_ORD_ITPC;
           fonction[0] = MSOP03_informations_symbole ;
           fonction[1] = MSOP_Vide_Objet ;
           fonction[2] = MSOP_Vide_Objet ;
           break;

     case CM_EQUIP_VILLE:
 		vl_type_equipement = CM_ORD_VILLE;
           fonction[0] = MSOP_Vide_Objet ;
           fonction[1] = MSOP_Vide_Objet ;
           fonction[2] = MSOP07_annuaire ;
           break;

     case CM_EQUIP_AIRE :
 		vl_type_equipement = CM_ORD_AIRE;
           fonction[0] = MSOP03_informations_symbole ;
           fonction[1] = MSOP_Vide_Objet; //MSOP_fca ;
           fonction[2] = MSOP07_annuaire ;
           break;
     case CM_EQUIP_PORTAIL:
 		vl_type_equipement = CM_ORD_PORTAIL;
           fonction[0] = MSOP03_informations_symbole ;
           fonction[1] = MSOP_Vide_Objet ;
           fonction[2] = MSOP_Vide_Objet ;
           break;
     case CM_EQUIP_RADT:
		   vl_type_equipement = CM_ORD_RADT;
		   fonction[0] = MSOP_Vide_Objet; //MSOP_fca ; : Ne fait rien vers Applix !!?
	       fonction[1] = MSOP_Vide_Objet;
	       fonction[2] = MSOP10_choisir_courbe ;
 		   break;
 	case CM_EQUIP_RESEAU_PI:
 	case CM_EQUIP_RESEAU_PS:
 		vl_type_equipement = CM_ORD_PASSAGE;
           fonction[0] = MSOP03_informations_symbole ;
           fonction[1] = MSOP_Vide_Objet ;
           fonction[2] = MSOP07_annuaire ;
           break;
	default:
	   vl_type_equipement = -1;
	   break;
   }
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "type equip = %d", vl_type_equipement );

   	if (vl_type_equipement != -1)
       {
       	  (fonction[pa_bouton])(vl_type_equipement, pa_num_equipement, pa_chaine);
       }


       XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_receptCommandeObjet" );
       return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoie d'un Message NEWTASK a la tache relais pour ecran texte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_Envoyer(
          pa_macro,
          pa_arg)

          char *pa_macro;
          char *pa_arg;

/*
* ARGUMENTS EN ENTREE :
*   pa_macro : nom macro applix
*   pa_arg : argument du message NEWTASK
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Envoie d'un Message NEWTASK a la tache relais
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 :MSOP_Envoyer ";
   XZSCT_Datagroup         vl_Dg ;
   char                    vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Envoyer pa_macro:%s; pa_arg:%s;" , pa_macro,pa_arg);

   /*A envoie du message XDM_IAX_NEWTASK */
   if ( pa_arg[0] == '\000' )
      strcpy( vl_arg , CM_IDENT);
   else
      sprintf( vl_arg , "%s|%s", CM_IDENT , pa_arg );

   XZST_03EcritureTrace(   XZSTC_MESSAGE ,  "NEWTASK %s %s " , pa_macro , vl_arg );
   XDG_EncodeDG2(vl_Dg , XDG_IAX_TEXTE , vm_NomMachine );
   if (!TipcSrvMsgWrite(vl_Dg
                        ,XDM_IdentMsg(XDM_IAX_NEWTASK)
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,COM_NEWTASK
			,T_IPC_FT_STR,pa_macro
			,T_IPC_FT_STR,vl_arg
			,T_IPC_FT_INT4,0
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
             ,"MSOP_Envoyer:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
	     ,vl_Dg ,XDM_IdentMsg(XDM_IAX_NEWTASK) ) ;
     return XDC_NOK;
     }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Envoyer %s",vl_Dg );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande d'affichage d'infos d'une intervenant ou
*   d'un affichage
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP_InfoInterv(
          pa_nom)

          char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_InfoInterv";
   int  vl_type;
   int vl_fmc_numero;
   int vl_fmc_cle;
   int vl_no_act_numero;
   int vl_no_act_cle;
   char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN MSOP_InfoInterv :  pa_nom:%s" , pa_nom );
   sscanf( pa_nom , "Mo%d|%d|%d|%d|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_no_act_numero , &vl_no_act_cle );
   sprintf( vl_arg , "%d|%d|%d|%d" , vl_fmc_numero , vl_fmc_cle , vl_no_act_numero , vl_no_act_cle );

   /*A envoie du message XDM_IAX_NEWTASK */
   switch( MSRA_Type_obj( pa_nom ) )
      {
         case CM_ORD_INTERVENANT_GENDARMERIE:
 	   MSOP_Envoyer( XDC_MACRO_MTIA , vl_arg );
 	   break;
         case CM_ORD_INTERVENANT_POMPIER:
 	   MSOP_Envoyer( XDC_MACRO_MTIA , vl_arg );
 	   break;
         case CM_ORD_INTERVENANT_DEPANNEUR:
 	   MSOP_Envoyer( XDC_MACRO_MTID , vl_arg );
 	   break;
         case CM_ORD_INTERVENANT_FOURGON_SIS:
 	   MSOP_Envoyer( XDC_MACRO_MTFO , vl_arg );
 	   break;
         case CM_ORD_INTERVENANT_PAT_ESCOTA:
 	   MSOP_Envoyer( XDC_MACRO_MTIP , vl_arg );
 	   break;
         case CM_ORD_INTERVENANT_AUTRE:
 	   MSOP_Envoyer( XDC_MACRO_MTIA , vl_arg );
 	   break;
         case CM_ORD_PMV_MOBILE:
 	   MSOP_Envoyer( XDC_MACRO_MTMO , vl_arg );
 	   break;
         case CM_ORD_AFFICH_GARE:
 	   MSOP_Envoyer( XDC_MACRO_MTGA , vl_arg );
 	   break;
 	 default:
 	   break;
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_InfoInterv" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande d'affichage de graphe tdp
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_tdp(
          pa_type_equipement,
          pa_num_equipement)

		  int pa_type_equipement;
		  int pa_num_equipement;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_tdp";

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_tdp pa_num:%d" , pa_num_equipement );
   XZST_03EcritureTrace(   XZSTC_WARNING, "MSOP_tdp....." );


//   if ( MSCO04_init_courbe_tdp(pa_num_equipement) == XDC_NOK )
	if ( MSCO04_demarer_courbe_TDP(pa_num_equipement) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Pb apres init courbe tdp" );
      return XDC_NOK;
      }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_tdp" );

   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction vide qui ne fait rien
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_Vide_Equipement(
		  pa_type,
          pa_num)

          int pa_type;
		  int pa_num;

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Fonction vide qui ne fait rien
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_Vide";
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Vide_Equipement pa_type:%d" , pa_type );
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Vide_Equipement" );
   return XDC_OK;
}


MSOP_Affichage_Video(
          pa_nom)

		  char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Fonction vide qui ne fait rien
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_Vide";
   char vl_commande[255];
   char vl_nom[50];
   char vl_info[50];
   char vl_ip[50];
   char vl_protocole[50];
   char vl_port[50];
   char vl_encodeur[50];

   if (sscanf(pa_nom,"%[^|]|%[^|]|",
		vl_nom,vl_info) != 2) {
	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Affichage_Video  format incorrect %s",pa_nom);
        return (XDC_NOK);
  }

  if (sscanf(vl_info,"%[^-]-%[^-]-%[^-]-%s",
			vl_ip,vl_protocole,vl_port,vl_encodeur) != 4) {
	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Affichage_Video  format incorrect camera %s",vl_info);
        return (XDC_NOK);
   }

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSOP_Affichage_Video %s ip=%s//protocole=%s//port=%s//encodeur=%s",
			vl_nom,vl_ip,vl_protocole,vl_port,vl_encodeur);

   sprintf(vl_commande, "/produits/migrazur/appliSD/exec/VideoCamera.sh %s %s %s %s &", vl_ip,vl_protocole,vl_port,vl_encodeur);

   system(vl_commande);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Affichage_Video %s",vl_commande );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction vide qui ne fait rien
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_Vide_Mobile(
          pa_nom)

		  char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Fonction vide qui ne fait rien
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_Vide";
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Vide_Mobile pa_nom:%s" , pa_nom );
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Vide_Mobile" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction vide qui ne fait rien
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_Vide_Objet(
		  pa_type,
          pa_num,
          pa_chaine)

          int pa_type;
		  int pa_num;
		  char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Fonction vide qui ne fait rien
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_Vide";
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Vide_Objet pa_type:%d" , pa_type );
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Vide_Objet" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction vide qui ne fait rien
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_Iris_Vision(
		pa_type,
        pa_num,
        pa_chaine)

        int pa_type;
		int pa_num;
		char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Fonction vide qui ne fait rien
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP_Vide";
int vl_autoroute;
int vl_pr;
char vl_commande[255];
char vl_cpr[20];

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Iris_Vision pa_type:%d, pa_num:%d, pa_chaine:%s" , pa_type, pa_num, pa_chaine );

	sscanf( pa_chaine , "%d|%d" , &vl_autoroute, &vl_pr);

	//traitement du PR
	if (vl_pr < 10000)
		sprintf(vl_cpr, "00%d", vl_pr);
	else if (vl_pr < 100000)
		sprintf(vl_cpr, "0%d", vl_pr);
	else
		sprintf(vl_cpr, "%d", vl_pr);

	sprintf(vl_commande, "/produits/migrazur/appliSD/exec/Irisvision.sh %d %s &",vl_autoroute, vl_cpr );

	system(vl_commande);

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Iris_Vision" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*    Envoi d'un message NEWTASK MTUE Typeeqt_NumeroEqt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP01_utilisation_equipement(
          pa_type_equipement,
          pa_num_equipement)

		  int pa_type_equipement;
		  int pa_num_equipement;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP01_utilisation_equipement";  XDY_TypeEqt vl_type;
   char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP01_utilisation_equipement pa_typeEquip:%d, pa_num_equip:%d" , pa_type_equipement, pa_num_equipement );
   /*A */
   switch( pa_type_equipement )
         {
         case CM_ORD_DAI :
               vl_type = XDC_EQT_DAI;
               break;
         case CM_ORD_CAMERA :
               vl_type = XDC_EQT_CAM;
               break;
         case CM_ORD_NAV :
               vl_type = XDC_EQT_NAV;
               break;
         case CM_ORD_BAF :
               vl_type = XDC_EQT_BAF;
               break;
         case CM_ORD_BAD :
               vl_type = XDC_EQT_BAD;
               break;
         case CM_ORD_PAU :
               vl_type = XDC_EQT_PAU;
               break;
         case CM_ORD_RADT :
               vl_type = XDC_EQT_RAD;
               break;
         case CM_ORD_PMV :
               vl_type = XDC_EQT_PMV;
               break;
         case CM_ORD_PMVA :
               vl_type = XDC_EQT_PMVA;
               break;
         case CM_ORD_PMV_PICTO :
               vl_type = XDC_EQT_PMV;
               break;
         case CM_ORD_PANALT :
               vl_type = XDC_EQT_PAL;
               break;
         case CM_ORD_FERM :
               vl_type = XDC_EQT_NAV;
               break;
         case CM_ORD_PRV:
        	 	 vl_type = XDC_EQT_PRV;
        	 	 break;
         default :
               XZST_03EcritureTrace(   XZSTC_WARNING, "Type d'equipement inattendu : %d" , pa_type_equipement );
               return XDC_NOK;
               break;
         }
   if (pa_num_equipement==0)
	   return XDC_OK;

   sprintf( vl_arg , "%d|%d" , vl_type , pa_num_equipement );

   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTUE,vl_arg  );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP01_utilisation_equipement" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande d'affichage de la synthese de la fmc associee a l'objet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP02_synthese_fmc(
          pa_nom)
//		  pa_type,

//	int pa_type;
	char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP02_synthese_fmc";
   int  vl_type;
   char vl_arg[256];
   int vl_fmc_basc_numero;
   int vl_fmc_basc_cle;
   int vl_fmc_numero;
   int vl_fmc_cle;
   int vl_no_act_numero;
   int vl_no_act_cle;
   int vl_idportion;
   char vl_sens;
   int vl_voie;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP02_synthese_fmc pa_nom:%s" , pa_nom );

//   switch( pa_type )
   switch( MSRA_Type_obj( pa_nom ) )
      {
      case CM_ORD_BASCBOU :
         sscanf( pa_nom , "Mz%d|%d|%d|%d|%d|%d|%c|%d|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle, &vl_fmc_basc_numero , &vl_fmc_basc_cle , &vl_idportion , &vl_sens , &vl_voie );
         vl_no_act_numero=0;
         vl_no_act_cle=0;
         break;
      case CM_ORD_INVERSE :
      case CM_ORD_NEUTRA :
      case CM_ORD_BOUCHON :
         sscanf( pa_nom , "Mz%d|%d|%d|%d|%c|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_idportion , &vl_sens );
         vl_no_act_numero=0;
         vl_no_act_cle=0;
         break;
      default :
         /* Cas d'un mobile sur echang ou section courante */
         if ( pa_nom[5] == 'e' )
           sscanf( pa_nom , "Mo%de|%d|%d|%d|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_no_act_numero , &vl_no_act_cle );
         else
           sscanf( pa_nom , "Mo%d|%d|%d|%d|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_no_act_numero , &vl_no_act_cle );
         break;
      }
   sprintf( vl_arg , "%d|%d " , vl_fmc_numero , vl_fmc_cle );
   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTSE , vl_arg );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP02_synthese_fmc" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Traite la demande d'information sur un symbole du synoptique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP03_informations_symbole(
        pa_type_equipement,
        pa_num_equipement,
        pa_chaine)

		  int pa_type_equipement;
		  int pa_num_equipement;
		  char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 :MSOP03_information_symbole ";
   char vl_nom[256];
   char pl_symbole[256];
   char *pl_info;
   long vl_lno;
   int  vl_no;
   int  vl_no_auto;
   int  vl_sens;
   int  vl_g1;
   int  vl_g2;
   int  vl_pr;
   XZSCT_Datagroup         vl_Dg ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP03_information_symbole pa_type_equipement:%d, pa_chaine=%s" , pa_type_equipement, pa_chaine);
   switch ( pa_type_equipement )
      {
      case CM_ORD_PEAGE :
         sprintf( pl_symbole , "peage|%d" , pa_num_equipement); /* "peage_" + no */
         break;
      case CM_ORD_ITPC :
         sscanf( pa_chaine , "%d|%d" , &vl_no , &vl_no_auto );
         sprintf( pl_symbole ,"itpc|%d_A%d" , vl_no , vl_no_auto);
         break;
      case CM_ORD_TUNNEL :
    	 sprintf(pl_symbole, "tunnel|%s", pa_chaine);
         break;
      case CM_ORD_PORTAIL :
         sscanf( pa_chaine , "%ld|%d|%d" , &vl_lno , &vl_sens , &vl_no_auto );
         sprintf( pl_symbole , "acces|%ld_%d_%d" , vl_lno , vl_sens , vl_no_auto );
         break;
      case CM_ORD_AIRE :
         sprintf( pl_symbole , "aire|%d" , pa_num_equipement); /* "aire_" + no */
         break;
      case CM_ORD_RESEAU :
         sprintf( pl_symbole , "reseau|%d" , pa_num_equipement );
         break;
      case CM_ORD_PASSAGE :
         sprintf( pl_symbole , "passsup|%d" , pa_num_equipement );
         break;
      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , " Objet %d non attendu " , pa_type_equipement );
         break;
      }

   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTIO, pl_symbole );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP03_information_symbole" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande de pilotage d'une camera
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP08_piloter_camera(
          char *pa_nom)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c 1.76 07/04/12 : MSOP08_piloter_camera"; int vl_no;
  int vl_no_auto;
  char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP08_piloter_camera pa_nom:%s" , pa_nom );
   sscanf( pa_nom , "camera_%d_A%d" , &vl_no , &vl_no_auto );

   sprintf( vl_arg , "%d|%d" , vl_no , vl_no_auto );
   /*A envoie du message XDM_IAX_SENDPOKE */
   MSOP_Envoyer_Poke( COM_CANAL_MTCA_RAPIDE , vl_arg );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP08_piloter_camera" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Traite la demande de pilotage d'une NAV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP04_piloter(
          pa_type_equipement,
          pa_num_equipement)

		  int pa_type_equipement;
		  int pa_num_equipement;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP04_piloter_NAV"; int vl_no;
  char *macro;
  char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSOP04_piloter_NAV pa_type_equipement:%d" , pa_type_equipement );

   switch (pa_type_equipement)
   {
	   case CM_ORD_BAF:
		   macro = XDC_MACRO_MTBAF;
		   break;
	   case CM_ORD_NAV:
		   macro = XDC_MACRO_MTNA;
		   break;
	   case CM_ORD_BAD:
		   macro = XDC_MACRO_MTBAD;
		   break;
	   case CM_ORD_PAU:
		   macro = XDC_MACRO_MTRA;
		   break;
	   case CM_ORD_CAMERA:
		   macro = XDC_MACRO_MTCA;
		   break;
	   case CM_ORD_PMVA:
		   macro = XDC_MACRO_MTPA;
		   break;
	   case CM_ORD_PRV :
		   macro = XDC_MACRO_MTPRV;
		   break;

	   case CM_ORD_PANALT:
		   macro = XDC_MACRO_MTPP;
		   break;

	   case CM_EQUIP_SAGA_PMV_1_18:
	   case CM_EQUIP_SAGA_PMV_2_18:
	   case CM_EQUIP_SAGA_PMV_2_15:
	   case CM_EQUIP_SAGA_PMV_3_15:
	   case CM_EQUIP_SAGA_PMV_2_18_P:
	   case CM_EQUIP_SAGA_PMV_3_15_P:
		macro = XDC_MACRO_SAGA_PMV;
		break;

	   case CM_EQUIP_SAGA_PMVA:
		macro = XDC_MACRO_SAGA_PMVA;
		break;

	  case CM_EQUIP_SAGA_BAF:
		macro = XDC_MACRO_SAGA_BAF;
		break;

	    case CM_EQUIP_SAGA_BRA:
		macro = XDC_MACRO_SAGA_BRA;
		break;
/*
	   case :
		   macro = ;
		   break;
*/
		   default:
		   XZST_03EcritureTrace(XZSTC_WARNING, " MSOP04_piloter Equipement non concerne : %d", pa_type_equipement );
		   break;
   }
   sprintf( vl_arg , "%d" , pa_num_equipement );
   MSOP_Envoyer( macro, vl_arg );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP04_piloter" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Traite la demande de pilotage d'un PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP06_piloter_PMV(
          pa_type_equipement,
          pa_num_equipement)

		int pa_type_equipement;
		int pa_num_equipement;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP06_piloter_PMV"; int vl_no;
  int vl_no_auto;
  char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP06_piloter_PMV pa_nom:");
   if ( pa_num_equipement == 0 )
   return XDC_OK;

   sprintf( vl_arg , "%d", pa_num_equipement );
   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTPM , vl_arg );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP06_piloter_PMV" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Traite la demande d'affichage d'une fiche annuaire
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP07_annuaire(
        pa_type_equipement,
        pa_num_equipement,
        pa_chaine)

		  int pa_type_equipement;
		  int pa_num_equipement;
		  char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 :MSOP07_annuaire "; int vl_no;
  int vl_no_auto;
  XDY_TypeEqt vl_type;
  char vl_chaine_no[256];
  char *pl_no;
  char *pl_nom;
   int  vl_g1;
   int  vl_g2;
   int  vl_pr;
  char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP07_annuaire pa_type_equipement:%d" , pa_type_equipement );
   switch (pa_type_equipement)
      {

   	  case CM_ORD_RESEAU :
    	 strcpy( vl_chaine_no , pa_chaine );
         pl_no = strtok( vl_chaine_no , CM_SEPAR_ECH );
         sscanf( pl_no , "reseau_%d" , &vl_no  );
         pl_nom = strtok( NULL , CM_SEPAR_ECH );
         vl_type = XDC_RES_RT;
         break;
      case CM_ORD_PASSAGE :
         strcpy( vl_chaine_no , pa_chaine );
         pl_no = strtok( vl_chaine_no , CM_SEPAR_ECH );
         sscanf( pl_no , "passsup_%d" , &vl_no  );
         pl_nom = strtok( NULL , CM_SEPAR_ECH );
         vl_type = XDC_RES_PS;
         break;
      case CM_ORD_PEAGE :
    	  strcpy( vl_chaine_no , pa_chaine );
    	  pl_nom = vl_chaine_no;
    	 vl_type = XDC_RES_PEAGE;
         break;
      case CM_ORD_VILLE :
         strcpy( vl_chaine_no , pa_chaine );
         pl_nom = vl_chaine_no;
         vl_type = XDC_RES_VILLE;
         break;
      case CM_ORD_AIRE :
         strcpy( vl_chaine_no , pa_chaine );
        /* pl_no = strtok( vl_chaine_no , CM_SEPAR_AIR );
         pl_nom = strtok( NULL , CM_SEPAR_AIR );
         pl_no = strtok( NULL , CM_SEPAR_AIR );*/
         pl_nom = pa_chaine ;
         vl_type = XDC_RES_AIRE;
         break;

      default :
         XZST_03EcritureTrace(   XZSTC_WARNING , " Objet %d non attendu " , pa_type_equipement );
         break;

      }
   if ( vl_type == XDC_RES_VILLE )
      sprintf( vl_arg , "%d|0|%s" , vl_type , pl_nom);
   else
      sprintf( vl_arg , "%d|%d|%s" , vl_type , pa_num_equipement, pl_nom);

   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTFA , vl_arg );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP07_annuaire" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande d'affichage de la fmc associee a l'objet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP09_fmc(
          pa_nom)
//pa_type,

//		int pa_type;
        char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP09_fmc";
   int  vl_type;
   int vl_fmc_basc_numero;
   int vl_fmc_basc_cle;
   int vl_fmc_numero;
   int vl_fmc_cle;
   int vl_no_act_numero;
   int vl_no_act_cle;
   int vl_idportion;
   char vl_sens;
   int vl_voie;
   char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP09_fmc pa_nom:%s" , pa_nom );
//   switch( pa_type )
   switch( MSRA_Type_obj( pa_nom ) )
      {
      case CM_ORD_BASCBOU :
         sscanf( pa_nom , "Mz%d|%d|%d|%d|%d|%d|%c|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle, &vl_fmc_basc_numero , &vl_fmc_basc_cle , &vl_idportion , &vl_sens , &vl_voie );
         vl_no_act_numero=0;
         vl_no_act_cle=0;
         break;
      case CM_ORD_INVERSE :
      case CM_ORD_NEUTRA :
      case CM_ORD_BOUCHON :
         sscanf( pa_nom , "Mz%d|%d|%d|%d|%c|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_idportion , &vl_sens );
         vl_no_act_numero=0;
         vl_no_act_cle=0;
         break;
      default :
         /* Cas d'un mobile sur echang ou section courante */
         if ( pa_nom[5] == 'e' )
           sscanf( pa_nom , "Mo%de|%d|%d|%d|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_no_act_numero , &vl_no_act_cle );
         else
           sscanf( pa_nom , "Mo%d|%d|%d|%d|%d|" , &vl_type , &vl_fmc_numero , &vl_fmc_cle , &vl_no_act_numero , &vl_no_act_cle );
         break;
      }
   sprintf( vl_arg , "%d|%d|%d|%d|%d" , vl_fmc_numero , vl_fmc_cle , vl_no_act_numero , vl_no_act_cle , vl_type );
   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTMC , vl_arg );
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP09_fmc" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande d'affichage d'une courbe
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP10_choisir_courbe(
	      pa_type_equipement,
          pa_num_equipement,
		  pa_chaine)

		  int pa_type_equipement;
		  int pa_num_equipement;
		  char *pa_chaine;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP10_choisir_courbe";
//  int vl_no;
//  int vl_prd;
//  int vl_no_portion;
//   int    vl_JourSemaine;
//   XDY_Horodate vl_HorodateSec;

	char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP10_choisir_courbe pa_type_equipement=%d, pa_num_equipement=%d, pa_chaine=%s" , pa_type_equipement, pa_num_equipement, pa_chaine );

   /*A Verification que l'on est sous une vue troncon */
/*
   if ( MSVU_Type_vue_courante() != 'T' )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, "On ne peut visualiser les courbes uniquement avec une vue Troncon !");
      return XDC_NOK;
      }
   switch( pa_nom[1] )
      {
      case 'Q' :
         sscanf( pa_nom , "tQradt_%d_%d_%d" , &vl_no ,&vl_no_portion , &vl_prd);
         break;
      case 'V' :
         sscanf( pa_nom , "tVradt_%d_%d_%d" , &vl_no ,&vl_no_portion , &vl_prd );
         break;
      case 'a' :
         sscanf( pa_nom , "radt_%d_%d_%d" , &vl_no ,&vl_no_portion , &vl_prd);
         break;
      default:
        XZST_03EcritureTrace(   XZSTC_WARNING, " Infoscomptage : Nom radt incoh���rent %s;" , pa_nom);
       return XDC_NOK;
      }
*/

   sprintf( vl_arg , "%d" , pa_num_equipement );
//   vl_arg[strlen( vl_arg )-1]='\000';
   vl_arg[strlen( vl_arg )]='\000';

   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTCV , vl_arg );

   /* todo : A ENLEVER */
/*
   XDY_Horodate vl_horodeb;
   tm_varcaract vl_varcaract[4];

   if (MSCO_liberer_fenetre(0) == XDC_NOK){
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO01_demarer_courbe_ft : ABANDON");
		return XDC_NOK;
	}

   if ( XZSM_13ConversionHorodateSec( "22/03/2011 06:00:00" , &vl_horodeb) != XDC_OK )
   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", vl_horodeb );
       return XDC_NOK;
   }

   vl_varcaract[0].no_station = vl_varcaract[1].no_station = vl_varcaract[2].no_station = vl_varcaract[3].no_station = pa_num_equipement;
   strcpy(vl_varcaract[0].nom_station, "station test");
   strcpy(vl_varcaract[1].nom_station, "station test");
   strcpy(vl_varcaract[2].nom_station, "station test");
   strcpy(vl_varcaract[3].nom_station, "station test");
   vl_varcaract[0].mesure = XDC_RADT_Q;
   vl_varcaract[1].mesure = XDC_RADT_V;
   vl_varcaract[2].mesure = XDC_RADT_TO;
   vl_varcaract[3].mesure = XDC_RADT_PL;
   vl_varcaract[0].sens = vl_varcaract[1].sens = vl_varcaract[2].sens = vl_varcaract[3].sens = 1;
   vl_varcaract[0].voie = vl_varcaract[1].voie = vl_varcaract[2].voie = vl_varcaract[3].voie = 1;
extern int vm_fenetre[CM_NB_MAX_FENETRES];

	vm_fenetre[0] = CM_COURBE_FT;
	if (MSCO01_init_courbe_temps(&vl_varcaract,
							 4,
							 1,
							 "22/03/2011 06:00:00",
							 1,
							 1,
							 0) == XDC_NOK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO01_demarer_courbe_ft : ABANDON");
		vm_fenetre[0] = CM_COURBE_AUCUNE;
		return XDC_NOK;
	}
*/
   /* FIN A ENLEVER */

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP10_choisir_courbe" );
   return XDC_OK;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande de pilotage d'un echangeur
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP11_piloter_echangeur(
		  pa_type,
		  pa_num,
          pa_nom)

        int pa_type;
		int pa_num;
		char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 :MSOP11_piloter_echangeur ";  char *pl_nom_ech,
         pl_nom[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP11_piloter_echangeur pa_nom:%s" , pa_nom );
   strcpy( pl_nom , pa_nom );
  /* pl_nom_ech = strtok( pl_nom , CM_SEPAR_AIR );
   pl_nom_ech = strtok( NULL   , CM_SEPAR_AIR );*/

   /*A envoie du message XDM_IAX_NEWTASK */
   /*MSOP_Envoyer( XDC_MACRO_MTEC , pl_nom_ech );*/
   MSOP_Envoyer( XDC_MACRO_MTEC , pl_nom );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP11_piloter_echangeur" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande de pilotage d'un tunnel
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP12_piloter_tunnel(
          pa_type,
          pa_num,
		  pa_nom)

		  int pa_type;
		  int pa_num;
          char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 :MSOP11_piloter_tunnel ";
  char pl_nom[256];
  int vl_ind_fin=0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP11_piloter_tunnel pa_nom:%s" , pa_nom );

   /*strcpy( pl_nom , &pa_nom[strlen("tunnel_")] );*/
   strcpy( pl_nom , pa_nom);
   /* On enleve le _NORD ou _SUD de la fin */
   vl_ind_fin = strlen( pl_nom ) - strlen( "_S2" );
   if ( strncmp( &pl_nom[vl_ind_fin] , "_S2" , strlen( "_S2" ) ) == NULL )
      {
      pl_nom[vl_ind_fin] = ' ';
      }
   else
      {
      vl_ind_fin = strlen( pl_nom ) - strlen( "_S1" );
      pl_nom[vl_ind_fin] = ' ';
      }

   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTTU , pl_nom );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP11_piloter_tunnel" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de la liste des astreintes a l'IHM Textuelle
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
MSOP19_liste_astreinte(
          pa_nom)

          char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP19_liste_astreinte";
  int vl_no_auto;
  int vl_type;
  long  vl_pr;
  char vl_arg[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP19_liste_astreinte pa_nom:%s" , pa_nom );

   sscanf( pa_nom , "interv_%d_%d_%d" , &vl_type , &vl_pr , &vl_no_auto );

   switch ( vl_type )
      {
      case 1 :
      	vl_type = XDC_AST_SIS;
      	break;
      case 2 :
      	vl_type = XDC_AST_DEPANN;
      	break;
      case 3 :
      	vl_type = XDC_AST_GENDARM;
      	break;
      case 4 :
      	vl_type = XDC_AST_ESCOTA;
      	break;
      default:
      	XZST_03EcritureTrace(   XZSTC_WARNING , " Type d'astreinte non prevu :%d", vl_type);
      	break;
      }
   sprintf( vl_arg , "%d|%d|%d" , vl_type , vl_no_auto , vl_pr);

   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTLS , vl_arg );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP19_liste_astreinte" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de la liste des astreintes a l'IHM Textuelle
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
MSOP20_liste_Alarmes(
          pa_nom)

          char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP20_liste_Alarmes";
   float vl_zero=0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP20_liste_Alarmes pa_nom:%s" , pa_nom );

   /*A Envoi message a MTLM */

   MSOP_Envoyer( XDC_MACRO_MTLM , "" );

   /*ARemise a zero du bouton*/
/*
   if ( MSVU_Affecte_vag( pa_nom , (ADDRESS) &vl_zero , V_F_TYPE ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_zero , pa_nom );
      return XDC_NOK;
      }
   MSVU_maj_vue();
*/
   XZIS01_Positionner_Indicateur_Alarme(FALSE);
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP20_liste_Alarmes" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de la liste des astreintes a l'IHM Textuelle
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
MSOP21_liste_Indispo(
          pa_nom)

          char *pa_nom;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP21_liste_Indispo";
   float vl_zero=0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP21_liste_Indispo pa_nom:%s" , pa_nom );

   /*A Envoi message a MTEI */

   MSOP_Envoyer( XDC_MACRO_MTEI , "" );


   /*ARemise a zero du bouton*/
/*
   if ( MSVU_Affecte_vag( pa_nom , (ADDRESS) &vl_zero , V_F_TYPE ) == XDC_NOK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_zero , pa_nom );
      return XDC_NOK;
      }

   MSVU_maj_vue();
*/
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP21_liste_Indispo" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Traite la demande de choix d'une video d'un lt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int MSOP26_choisir_video(
		  pa_type,
		  pa_num,
          pa_nom_vue)

          int pa_type;
	      int pa_num;
		  char *pa_nom_vue;

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c	1.73 09/02/10 : MSOP26_choisir_video ";
  int vl_no, vl_no_district;
  char vl_arg[256];
//  char *pl_nom_vue=NULL;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP26_choisir_video pa_num:%s" , pa_num );
//   sscanf( pa_nom , "ltv_%d" , &vl_no);
//   MSVU_Cherche_NomVueSup(&pl_nom_vue);
   vl_no = pa_num;

//   if ( pl_nom_vue == NULL )
   if ( pa_nom_vue == NULL )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, "MSOP26_choisir_video: Impossible de recuperer le nom du district du ltv" );
      return XDC_NOK;
      }

   if ( strcmp( pa_nom_vue , XDC_NOM_DISTRICT_VC ) == NULL )
      vl_no_district = XDC_VC;
   else
   if ( strcmp( pa_nom_vue , XDC_NOM_DISTRICT_DP ) == NULL )
      vl_no_district = XDC_DP;
   else
   if ( strcmp( pa_nom_vue , XDC_NOM_DISTRICT_CA ) == NULL )
      vl_no_district = XDC_CA;
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, "MSOP26_choisir_video: nom du district du ltv inconnu :%s", pa_nom_vue );
      return XDC_NOK;
      }

   sprintf( vl_arg , "%d|%d" , vl_no , vl_no_district);
   /*A envoie du message XDM_IAX_NEWTASK */
   MSOP_Envoyer( XDC_MACRO_MTCACI , vl_arg );
   printf( "Envoi de %s a %s\n", vl_arg , XDC_MACRO_MTCACI );


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP26_choisir_video" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*    Envoi le Messsage contenant les donnees pour affichage de
*    la vue par le serveur
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSOP27_projeter()

/*
* ARGUMENTS EN ENTREE :
*   pa_null : pour rester coherent avec le reste des procedure de clic
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   0 pas trouve
*   >0 type de l'objet
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{
float vl_radt_visi, vl_pmv_visi, vl_camera_visi , vl_dai_visi, vl_pau_visi, vl_strada_visi,
         vl_reseau_pi_visi, vl_reseau_eau_visi, vl_reseau_elect_visi, vl_reseau_gaz_visi,
         vl_reseau_autre_visi, vl_lit_visi, vl_pat_visi, vl_nav_visi, vl_ltv_visi, vl_travaux_visi,
         vl_pmva_visi, vl_baf_visi, vl_bad_visi, vl_prv_visi, vl_canton_visi;
        char vl_str_visi[256];
#ifndef _TIBCO_EMS
  char   *vl_tab_radt[50],
          *vl_tab_aires[50],
          *vl_tab_dai[5];
   T_STR      vl_textpostit[1];
#else
  XDY_Nom   vl_tab_radt[50],
          vl_tab_aires[50],
          vl_tab_dai[5];
   XDY_Nom      vl_textpostit[1];
#endif
   T_INT2     vl_visipostit[1];
   T_INT2     vl_prespostit[1];

char *pl_nom_vue="";
   XZSCT_Datagroup vl_Dg;

sprintf( vl_str_visi , "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",
            (int) vl_radt_visi, (int) vl_pmv_visi, (int) vl_camera_visi , (int) vl_dai_visi, (int) vl_pau_visi,
            (int) vl_reseau_pi_visi, (int) vl_reseau_eau_visi,
                (int) vl_reseau_elect_visi, (int) vl_reseau_gaz_visi, (int) vl_reseau_autre_visi,
            (int) vl_lit_visi , (int) vl_pat_visi , (int) vl_nav_visi, (int) vl_ltv_visi , (int) vl_travaux_visi,
            (int) vl_strada_visi, (int) vl_pmva_visi, (int) vl_baf_visi, (int) vl_bad_visi, (int) vl_prv_visi, (int) vl_canton_visi);


   XZST_03EcritureTrace(   XZSTC_MESSAGE ,  "XDM_ISYN_SERVEUR"  );
   XDG_EncodeDG2(vl_Dg , XDG_ISYN_SERVEUR , vm_NomMachineSS );

   if (!TipcSrvMsgWrite(vl_Dg
                        ,XDM_IdentMsg(XDM_ISYN_SERVEUR)
                        ,XDM_FLG_SRVMSGWRITE
                        ,T_IPC_FT_STR, pl_nom_vue
                        ,T_IPC_FT_STR_ARRAY , vl_tab_dai , 0
#ifdef _TIBCO_EMS
			,XDC_Lon_Nom
#endif
                        ,T_IPC_FT_STR_ARRAY , vl_tab_radt , 0
#ifdef _TIBCO_EMS
			,XDC_Lon_Nom
#endif
                        ,T_IPC_FT_STR_ARRAY , vl_tab_aires , 0
#ifdef _TIBCO_EMS
			,XDC_Lon_Nom
#endif
                       ,T_IPC_FT_STR, (T_STR) vl_str_visi
                        ,T_IPC_FT_INT2, (T_INT2) 0
                        ,T_IPC_FT_STR,  ""
                        ,T_IPC_FT_INT2, (T_INT2) 0
                       ,T_IPC_FT_STR,  ""
                        ,T_IPC_FT_INT2 , (T_INT2) 0
                        ,T_IPC_FT_INT2_ARRAY , vl_prespostit , 0
                        ,T_IPC_FT_INT2_ARRAY , vl_visipostit , 0
                        ,T_IPC_FT_STR_ARRAY , vl_textpostit , 0
#ifdef _TIBCO_EMS
			,XDC_Lon_Nom
#endif
                        ,T_IPC_FT_INT2 , (T_INT2)0
                        ,T_IPC_FT_STR , (T_STR)  ""
                        ,NULL) )
    {
    /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
             ,"MSOP_Envoi_Affiche_Serveur:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)"
             ,vl_Dg ,XDM_IdentMsg(XDM_ISYN_SERVEUR) ) ;
     return XDC_NOK;
     }


  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi d'un Message SENDPOKE a la tache relais pour ecran texte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSOP_Envoyer_Poke(
          int pa_canal,
          char *pa_arg)

/*
* ARGUMENTS EN ENTREE :
*   pa_canal : numero de canal (poke) Applix
*   pa_arg : texte (arguments) du message SENDPOKE
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   Envoie d'un Message SENDPOKE a la tache relais
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sop.c 1.76 07/04/12 :MSOP_Envoyer_Poke ";
   XZSCT_Datagroup         vl_Dg ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSOP_Envoyer_Poke pa_canal:%d; pa_arg:%s;", pa_canal, pa_arg);

   /*A envoie du message XDM_IAX_SENDPOKE */
   XZST_03EcritureTrace(   XZSTC_MESSAGE ,  "SENDPOKE %d %s " , pa_canal , pa_arg );
   XDG_EncodeDG2(vl_Dg , XDG_IAX_TEXTE , vm_NomMachine );
   if (!TipcSrvMsgWrite(vl_Dg
                        ,XDM_IdentMsg(XDM_IAX_SENDPOKE)
                        ,XDM_FLG_SRVMSGWRITE
                        ,T_IPC_FT_INT4,COM_SENDPOKE
                        ,T_IPC_FT_INT4,pa_canal
                        ,T_IPC_FT_STR,pa_arg
                        ,NULL) )
     {
        /* Affichage Trace */
        XZST_03EcritureTrace(XZSTC_WARNING
                ,"MSOP_Envoyer_Poke: canal %d, arg %s, erreur TipcSrvMsgWrite"
                ,pa_canal, pa_arg ) ;
        return XDC_NOK;
     }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSOP_Envoyer_Poke" );
   return XDC_OK;
}

