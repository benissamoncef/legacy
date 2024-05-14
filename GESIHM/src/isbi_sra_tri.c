/*E*/
/*Fichier : $Id: isbi_sra_tri.c,v 1.3 2013/08/28 09:28:35 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2013/08/28 09:28:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isyn_sra_tri.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques : Fonction de tri des objets par ordre 
*   d'apparition a l'ecran
*   
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	23 Sep 1994	: Creation
* JMG 		22/08/12 : correction pb clic fmc 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sra_tri.h"

/* definitions de constantes */


#define  CM_ORD_CREATION 0
#define  CM_ORD_MODIFICATION 1
#define  CM_ORD_SUPPRESSION 2
#define  CM_LONGUEUR_IDENT   10

/* definitions de types locaux */

typedef char DEB_NOM[80];


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isyn_sra_tri.c	1.29 06/29/07 : isyn_sra_tri" ;


static  DEB_NOM vm_MSRA_tabordobj[CM_NB_TYPE_OBJET_ORDRE+1]; /*Tableau contenant le debut de nom de chaque type d'objet */



/* declaration de fonctions internes */
int   MSRA_init_tabordobj();


int   MSRA_Type_obj();

/* definition de fonctions externes */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Initialisation du tableau tabordobj
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_init_tabordobj(
          )

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
*   Initialisation du tableau des debut de noms des objets
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sra_tri.c	1.29 06/29/07 : MSRA_init_tabordobj";
	char vl_ident[CM_LONGUEUR_IDENT];
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_init_tabordobj" );
   
   /*A Initialisation du tableau */
   strcpy( vm_MSRA_tabordobj[0] , "" );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_MOBILES_PREVUS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_MOBILES_PREVUS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_MOBILES_REELS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_MOBILES_REELS] , vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_MOBILES_TETE_PREVUS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_MOBILES_TETE_PREVUS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_MOBILES_TETE_REELS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_MOBILES_TETE_REELS] , vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_MOBILES_QUEUE_PREVUS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_MOBILES_QUEUE_PREVUS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_MOBILES_QUEUE_REELS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_MOBILES_QUEUE_REELS] , vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_FIXES_PREVUS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_FIXES_PREVUS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_TRAVAUX_FIXES_REELS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TRAVAUX_FIXES_REELS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_INTERVENANT_GENDARMERIE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INTERVENANT_GENDARMERIE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_INTERVENANT_POMPIER );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INTERVENANT_POMPIER] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_INTERVENANT_DEPANNEUR );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INTERVENANT_DEPANNEUR] ,vl_ident  );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_INTERVENANT_FOURGON_SIS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INTERVENANT_FOURGON_SIS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_INTERVENANT_PAT_ESCOTA );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INTERVENANT_PAT_ESCOTA] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_INTERVENANT_AUTRE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INTERVENANT_AUTRE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_PMV_MOBILE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PMV_MOBILE] , vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VL );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VL] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PL );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PL] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPL );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPL] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_CAR );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_CAR] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLCAR );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLCAR] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLCAR );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLCAR] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLCAR );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLCAR] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_REM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_REM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_CARREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_CARREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLCARREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLCARREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLCARREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLCARREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLCARREM );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLCARREM] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_MOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_MOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_CARMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_CARMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLCARMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLCARMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLCARMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLCARMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLCARMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLCARMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_REMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_REMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLREMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLREMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLREMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_CARREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_CARREMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLCARREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLCARREMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_PLCARREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_PLCARREMMOTO] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ACCIDENT_VLPLCARREMMOTO );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ACCIDENT_VLPLCARREMMOTO] , vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_VEHICULE_CONTRE_SENS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_VEHICULE_CONTRE_SENS] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_VEHICULE_ARRETE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_VEHICULE_ARRETE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_VEHICULE_PANNE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_VEHICULE_PANNE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_VEHICULE_FEU );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_VEHICULE_FEU] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_CIRQUE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_CIRQUE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_CONVOI_EXCEPTIONNEL );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_CONVOI_EXCEPTIONNEL] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_CONVOI_MILITAIRE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_CONVOI_MILITAIRE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_VEHICULE_LENT );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_VEHICULE_LENT] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_MANIFESTATION );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_MANIFESTATION] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_GREVE_ESCOTA );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_GREVE_ESCOTA] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_OPERATION_ESCARGOT );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_OPERATION_ESCARGOT] ,vl_ident  );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_INCIDENT_SOUS_CONSC );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_INCIDENT_SOUS_CONSC] ,vl_ident  );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_EBOULEMENT );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_EBOULEMENT] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_INCENDIE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_INCENDIE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_INONDATION );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_INONDATION] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_OBSTACLE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_OBSTACLE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_CHAUSSEE_DETERIOREE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_CHAUSSEE_DETERIOREE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_FLAQUE_HUILE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_FLAQUE_HUILE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ANIMAL_ERRANT );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ANIMAL_ERRANT] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_FRONTIERE_FERMEE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_FRONTIERE_FERMEE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ALTERCATION_USAGERS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ALTERCATION_USAGERS] ,vl_ident  );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_AGRESSION );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_AGRESSION] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_MALAISE_USAGER );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_MALAISE_USAGER] ,vl_ident  );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_HOLD_UP );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_HOLD_UP] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_PIETON_CHAUSSEE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_PIETON_CHAUSSEE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_AUTRE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_AUTRE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_VEHICULE_MARCHE_ARR );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_VEHICULE_MARCHE_ARR] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_TRAITEMENT_CHAUSSEES );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_TRAITEMENT_CHAUSSEES] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_ECHANGEUR_DECONSEILLE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_ECHANGEUR_DECONSEILLE] , vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_CHAUSSEE_GLISSANTE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_CHAUSSEE_GLISSANTE] , vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_CHUTE_NEIGE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_CHUTE_NEIGE] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_NEIGE_CHAUSSEE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_NEIGE_CHAUSSEE] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_GRELE_CHAUSSEE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_GRELE_CHAUSSEE] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_PLUIE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_PLUIE] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_VENT_FORT  );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_VENT] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_BROUILLARD );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_BROUILLARD] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_BROUILLARD_GIVRANT);
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_BROUILLARD_GIVRANT] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_GRELE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_GRELE] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_VERGLAS );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_VERGLAS] , vl_ident);
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_METEO_AUTRE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_METEO_AUTRE], vl_ident );
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_FMC_REGULATION_VITESSE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FMC_REGULATION_VITESSE], vl_ident );
   
   sprintf( vl_ident , "Mo%03d" , XDC_SYN_GARE );
   strcpy( vm_MSRA_tabordobj[CM_ORD_AFFICH_GARE] , vl_ident );

   strcpy( vm_MSRA_tabordobj[CM_ORD_AFF_VOIE] , "MoAF" );
   
   strcpy( vm_MSRA_tabordobj[CM_ORD_DAI] , "dai" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_DAITXT] , "tdai" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_CAMERA] , "camera" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_CAMERATXT] , "tcam" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_NAV] , "nav" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PAU] , "pau" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_RADTXTQ] , "tQradt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_RADTXTV] , "tVradt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_RADTTXT] , "tradt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_RADT] , "radt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_ECHANGEURTXT ] , "techan" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_CECHANGEUR ] , "cechan" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_QECHANGEUR] , "qechang" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_ECHANGEUR ] , "echan" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PMV] , "pmv" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PMV_PICTO] , "ppmv" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PMVTXT] , "tpmv" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PANALT] , "alt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FERM] , "ferm" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PEAGE] , "peage" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PEAGETXT] , "tpeage" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_ITPC] , "itpc" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_VILLE] , "ville" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PANPOLTXT] , "tpann" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PANPOL] , "pann" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PENTE] , "pente" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PENTETXT] , "tpente" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_TUNNEL] , "tunnel" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_VIADUC] , "via" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PORTAIL] , "acces" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PETROL] , "petr" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_CANTON] , "canton" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_AIRE] , "aire" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_RESEAU] , "reseau" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PASSAGE] , "passsup" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_EXTINC] , "extinc" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_FAV] , "fav" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_REFUGE] , "refuge" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_LTV] , "ltv" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_LIT] , "lit" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_ZDP] , "zdp" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BETAT] , "etat" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BALARM] , "alarme" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BBOUCHON] , "bouchon" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BINTERV] , "interv" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BEVT] , "evt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_NAVIG] , "navig" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_SIGNALE] , "signale" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_SIGNALT] , "signalt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_SERVEUR] , "serveur" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_IGN] , "ign" );

   strcpy( vm_MSRA_tabordobj[CM_ORD_BAU] , "bau" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_MASK] , "mask" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BASCBOU] , "Mz003" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_INVERSE ] , "Mz002" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_NEUTRA] , "Mz001" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BOUCHON] , "Mz000" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_ZRADT] , "zradt" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_ZOOM] ,  "zzoom" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PORTION] , "portion" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_JONCTION] , "jportion" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_SEGMENT] , "segment" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PORTEUSE] , "PORTEUSE" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PORTEUSEPORTION] , "Pportion" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_GRAPHE] , "graph" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_GRAPHE_INRETS] , "igraph" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_GRAPHE_TDP] , "tgraph" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_IZOOM] ,  "izoom" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_SYN] ,  "syn" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BGAUCHE] ,  "gau" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BDROITE] ,  "droit" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BQUIT] ,  "quit" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_BIMP] ,  "imp" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_POSEIT] ,  "postit" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PASTILLE] ,  "past" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_CARTE] ,  "carte" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PR] ,  "pr_" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_STRADA],"strada");
   strcpy( vm_MSRA_tabordobj[CM_ORD_GRAPHE_STRADA],"sgraph" );

   strcpy( vm_MSRA_tabordobj[CM_ORD_PMVA],"pmva");
   strcpy( vm_MSRA_tabordobj[CM_ORD_PMVAD],"pmvad");
   strcpy( vm_MSRA_tabordobj[CM_ORD_BAF],"baf");
   strcpy( vm_MSRA_tabordobj[CM_ORD_BAD],"bad");
   strcpy( vm_MSRA_tabordobj[CM_ORD_PMVATXT] , "tpmva" );
   strcpy( vm_MSRA_tabordobj[CM_ORD_PRV] , "prv" );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_init_tabordobj" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Renvoie le type de l'objet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA_Type_obj(
          pa_nom_obj)
          
          char * pa_nom_obj;

/*
* ARGUMENTS EN ENTREE :
*   
*
*
* ARGUMENTS EN SORTIE :
*   pa_nom_obj : Nom de l'objet a traiter
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
*  Recherche dans le tableau vm_MSRA_tabordobj le debut de
*  nom correspondant a l'objet
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sra_tri.c	1.29 06/29/07 : MSRA_Type_obj";
   int ord;
   int code_retour;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Type_obj" );

   ord = CM_NB_TYPE_OBJET_ORDRE;
   
   /* Recherche le nom de 'l'objet1 dans le tableau des objets  et retour l'indice du tableau */
   if (pa_nom_obj != NULL )
     while ( (ord > 0) && ( strncmp ( vm_MSRA_tabordobj[ord] , pa_nom_obj, strlen(vm_MSRA_tabordobj[ord]) ) != NULL) )
      {
      ord--;
      }
   else
      ord = CM_ORD_QECHANGEUR;
      
   code_retour = ord;  
   XZST_03EcritureTrace(   XZSTC_WARNING, "OUT  : MSRA_Type_obj code retour:%d",code_retour );
   
   return ( code_retour );
}

