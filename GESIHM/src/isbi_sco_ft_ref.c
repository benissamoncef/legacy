/*E*/
/*Fichier : $Id: isbi_sco_ft_ref.c,v 1.4 2016/10/11 11:13:41 pc2dpdy Exp $     Release : $Revision: 1.4 $        Date : $Date: 2016/10/11 11:13:41 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MSCO * FICHIER isbi_sco_ft_ref.c
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permettant l'affichage de courbes F(t) avec reference
*  au sein du synoptique
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	17/08/07 : Creation (squelette pour satisfaction du link) 1.1
* PNI	25/09/2013 : Ajout de la date dans le titre 1.3 DEM1085
* JMG	11/01/16	 : correction erreur compil 1.4
-----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"
#include "search.h"

/* definitions de constantes */

#define CM_NB_MAX_LIGNE 4

#define CM_UNE_MINUTE 60
#define CM_3HEURES    (3600 * 3)
#define CM_24HEURES   (3600 * 24)

#define CM_SLOT_SCROLL 1


#define CM_Y_DELTA_LEGEND 1000
#define CM_X_DELTA_LEGEND1 6600
#define CM_X_DELTA_LEGEND2 7600
#define CM_X_MARGE_LEGEND   100
#define CM_X_MARGE_TEXT_LEGEND   1200
#define CM_Y_MARGE_LEGEND   1600
#define CM_NB_LIGNES_LEGEND 2
#define CM_LG_LIGNE_LEGEND  800

#define CM_X_DATE1 ( CM_LL_X_GRAPH_FT_REF + CM_X_MARGE_LEGEND + CM_X_DELTA_LEGEND1 )
#define CM_Y_DATE1 ( CM_UR_Y_GRAPH_FT_REF + CM_Y_MARGE_LEGEND - 400 )
#define CM_X_DATE2 ( CM_LL_X_GRAPH_H4_FT_REF + CM_X_MARGE_LEGEND + CM_X_DELTA_LEGEND2 )
#define CM_Y_DATE2 ( CM_UR_Y_GRAPH_H4_FT_REF + CM_Y_MARGE_LEGEND - 400 )

#define CM_MARGE -16000

#define CM_COULEUR_LIGNE1   8
#define CM_COULEUR_LIGNE2   2
#define CM_COULEUR_LIGNE3   5
#define CM_COULEUR_LIGNE4   13
#define CM_LARGEUR_LIGNE    1


#define CM_COULEUR_GRILLE   113


#define CM_MAX_TYPE      4
#define CM_VARIABLE_Q   "Q"
#define CM_MIN_Q         0
#define CM_MAX_Q      10000
#define CM_STEP_Q     1000
#define CM_VARIABLE_V   "V"
#define CM_MIN_V         0
#define CM_MAX_V       200
#define CM_STEP_V       25
#define CM_VARIABLE_TO  "TO"
#define CM_MIN_TO         0
#define CM_MAX_TO        100
#define CM_STEP_TO      12.5
#define CM_VARIABLE_PL  "PL"
#define CM_MIN_PL         0
#define CM_MAX_PL       100
#define CM_STEP_PL     12.5


#define CM_X_ECHELLE_GAUCHE (-14500)
#define CM_X_ECHELLE_DROITE (-12000)

#define CM_NB_TICS	10

#define CM_Y_TITRE       15500
#define CM_Y_ETAT_ACQ    13500

#define CM_COEF_Q   1.0
#define CM_COEF_V  40.0
#define CM_COEF_PL 80.0
#define CM_COEF_TO 80.0

#define CM_SEUIL_MAX 32767

#define CM_ETAT_DONNEES_OK   0
#define CM_ETAT_PERTE_RADT   1
#define CM_ETAT_PERTE_REF_BD 2
#define CM_BUFF_SOCK 600


static int tm_index_couleur[CM_NB_MAX_LIGNE]={CM_COULEUR_LIGNE1,CM_COULEUR_LIGNE2,CM_COULEUR_LIGNE3,CM_COULEUR_LIGNE4};

extern char c_correspFenetre[];
/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : isbi_sco_ft_ref" ;

static int       vm_hauteur_graph[CM_NB_MAX_FENETRES];
static int       vm_largeur_graph[CM_NB_MAX_FENETRES];

extern XDY_Horodate vm_HorodateSecInit[CM_NB_MAX_FENETRES];
extern XDY_Horodate vm_Horodate_deltatr[CM_NB_MAX_FENETRES];
extern XDY_Horodate vm_HorodateSecInit_H1;

XDY_Horodate vm_Horodate_Ft_ref_Init[CM_NB_MAX_FENETRES]={0.0,0.0};
XDY_Horodate vm_Horodate_Ft_Reference[CM_NB_MAX_FENETRES]={0.0,0.0};
XDY_Horodate vm_Horodate_Ft_ref_H4[CM_NB_MAX_FENETRES]={0.0,0.0};
XDY_Horodate vm_Horodate_Ft_ref_Appel[CM_NB_MAX_FENETRES]={0.0,0.0};


static int vm_max_Q[4]={0, 1000 , 3000 , 10000 };
static int vm_max_V[4]={0, 200 , 200 , 200 };
static int vm_max_TO[4]={0, 10 , 20 , 30 };
static int vm_max_PL[4]={0, 100 , 100 , 100 };


static int vm_t_periode[CM_NB_MAX_FENETRES] = {0,0};

char vl_msg[CM_BUFF_SOCK];


extern mon_XZIST_LigneDonnees vm_donnees_H4_ref[CM_NB_MAX_FENETRES][CM_MAX_COURBES_FT_REF][XZISC_NB_LIG_MESURES];
extern int vm_nb_donnees_futures[CM_NB_MAX_FENETRES];


/* Donnees partagees avec les autres graphes F(t) */
extern tm_caracteristique_courbe vm_caracteristique_courbe[CM_NB_MAX_FENETRES];

extern XDY_Octet vm_donnees_arrivees;
extern XDY_Horodate vm_Horodate_deltatr[CM_NB_MAX_FENETRES];
extern XDY_Horodate vm_horodate_courbe;

/* Donnees pour gestion du reticule */

/* declaration de fonctions internes */


/* definition de fonctions externes */
extern int MSVU_afficher_courbe();


/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*   Initialisation des courbes temps avec reference
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_init_courbe_ft_ref(
                    pa_varcaract,
                    va_nb_courbes,
                    va_tempsreel,
                    va_horodeb,
                    va_fenetre,
                    va_type_ech,
                    va_periode )

                    tm_varcaract *pa_varcaract;
                    XDY_Octet   va_nb_courbes;
                    XDY_Booleen va_tempsreel;
                    char        *va_horodeb;
                    XDY_Octet   va_fenetre;
                    XDY_Octet	va_type_ech;
                    XDY_Booleen	va_periode;


/*
* ARGUMENTS EN ENTREE :
*  pa_varcaract : tableau des caracteristique des variables des courbes ( chauss,voie,var)
*  va_nb_courbes: Nombre de courbes
*  va_tempsreel : XDC_OUI attendu puisque c'est une courbe tempsreel
*  va_horodeb : date de reference + heure courante
*  va_fenetre : fenetre d'affichage de la courbe droite ou gauche CM_FENETRE_DROITE , CM_FENETRE_GAUCHE
*  va_type_ech: type d'echelle
*  va_periode: type de periode 1mn=XDC_OUI 6mn=XDC_NON
*
* ARGUMENTS EN SORTIE :
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
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : MSCO_init_courbe_ft_ref " ;

   char vl_nom[64];
   char vl_nom_var[64];
   char vl_nom_vag[64];
   char vl_nom_vag_ref[64];
   char vl_nom_vag_ref_H4[64];
   int i=0,j=0;

   int    vl_JourSemaine;
   XDY_Horodate vl_HorodateSec;
   int vl_Horodate_Appel_Int, vl_HeureCouranteSec;
   int vl_HorodateSec_Ref_Int;
   XZSMT_Horodate vl_horodate;
   XZSMT_Horodate vl_date,vl_datedeb;
   char vl_legend[64];
   char vl_mesure[64];

   int vl_nb_couleur;
	int vl_nb_courbes;

   char   vl_format[64];
   char  *pl_format;
   char  *pl_blank;
   float vl_coef=0.0;
   float	vl_start=0.0, vl_incr=0.0;
   int vl_max_QVTOPL;


   XZST_03EcritureTrace( XZSTC_WARNING,
                         "IN  : MSCO_init_courbe_ft_ref  va_nb_courbes:%d; va_horodeb:%s; va_fenetre:%d; va_periode:%d" ,
                         va_nb_courbes, va_horodeb,va_fenetre,(int) va_periode);


   /*A Si la fenetre en arg n'est ni gauche ni droite, on impose que ce soit la droite */
   if (( va_fenetre-1 != CM_FENETRE_GAUCHE ) && (va_fenetre-1 != CM_FENETRE_DROITE ))
      va_fenetre = CM_FENETRE_DROITE;

   if (( va_nb_courbes < 1 ) || ( va_nb_courbes > CM_MAX_COURBES_FT_REF ))
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " Nombre de courbes incorrect : %d" , va_nb_courbes);
      return XDC_NOK;
      }

   vl_nb_courbes = va_nb_courbes*2;

   vm_memo_courbes[vm_nb_courbes].type=CM_COURBE_FT_REF;
   vm_memo_courbes[vm_nb_courbes].fenetre=va_fenetre;
   vm_nb_courbes=vm_nb_courbes+1;

   /*A  creation vag et dsv */
    for ( i= 0; i < va_nb_courbes ; i++)
      {
      /*A Insertion vag */
      sprintf( vl_nom_vag , "%d_%d_%d_%d",
                  pa_varcaract[i].no_station , pa_varcaract[i].sens , pa_varcaract[i].voie , pa_varcaract[i].mesure);
      sprintf( vl_nom_vag_ref , "Ft_ref_%d_%d", i, va_fenetre);
      sprintf( vl_nom_vag_ref_H4 , "Ft_ref_H4_%d_%d", i, va_fenetre);

      switch ( pa_varcaract[i].mesure )
        {
        case XDC_RADT_Q :
           vl_coef = (float)( CM_MAX_Q / (float)vm_max_Q[va_type_ech]);
           break;
        case XDC_RADT_V :
           vl_coef = (float) (CM_MAX_Q / (float)vm_max_V[va_type_ech]);
           break;
        case XDC_RADT_TO :
           vl_coef = (float) (CM_MAX_Q / (float)vm_max_TO[va_type_ech]);
           break;
        case XDC_RADT_PL :
           vl_coef = (float) (CM_MAX_Q / (float)vm_max_PL[va_type_ech]);
           break;
        }

      if ( MSCO_Insere_vag( vl_nom_vag , vl_coef , va_fenetre) != XDC_OK )
         XZST_03EcritureTrace(   XZSTC_WARNING , "Erreur a l'insertion de la vag %s en table" , vl_nom_vag );
      if ( MSCO_Insere_vag( vl_nom_vag_ref , vl_coef , va_fenetre) != XDC_OK )
         XZST_03EcritureTrace(   XZSTC_WARNING , "Erreur a l'insertion de la vag %s en table" , vl_nom_vag_ref );
      if ( MSCO_Insere_vag( vl_nom_vag_ref_H4 , vl_coef , va_fenetre) != XDC_OK )
         XZST_03EcritureTrace(   XZSTC_WARNING , "Erreur a l'insertion de la vag %s en table" , vl_nom_vag_ref_H4 );

      }

   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   /* Arrondir la date presente a la date du dernier slot de temps */
   vl_Horodate_Appel_Int = ((int) vl_HorodateSec / CM_DUREE_SLOTIME_FT_REF) * CM_DUREE_SLOTIME_FT_REF;
   XZSM_11ConversionHorodate(vl_Horodate_Appel_Int , &vl_horodate);
   strncpy(vl_date,vl_horodate,10);
   vl_date[10]=0;
   vl_HeureCouranteSec = vl_Horodate_Appel_Int % CM_24HEURES;
   vm_Horodate_Ft_ref_Appel[va_fenetre-1] = (XDY_Horodate) vl_Horodate_Appel_Int;
   vm_Horodate_Ft_ref_Init[va_fenetre-1] = (XDY_Horodate) (vl_Horodate_Appel_Int - CM_3HEURES);
   XZSM_11ConversionHorodate( vm_Horodate_Ft_ref_Init[va_fenetre-1] , &vl_horodate);

   if ( XZSM_13ConversionHorodateSec( va_horodeb , &vl_HorodateSec) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", va_horodeb );
      MSCO_arret_courbe_temps_ref(va_fenetre);
      return XDC_NOK;
      }
   vl_HorodateSec_Ref_Int = ((int) vl_HorodateSec / CM_24HEURES) * CM_24HEURES;
   strncpy(vl_datedeb,va_horodeb,10);
   va_horodeb[10]=0;
   vm_Horodate_Ft_ref_H4[va_fenetre-1] = (XDY_Horodate) (vl_HorodateSec_Ref_Int + vl_HeureCouranteSec);
   XZSM_11ConversionHorodate( vm_Horodate_Ft_ref_H4[va_fenetre-1],&vl_horodate);
   vm_Horodate_Ft_Reference[va_fenetre-1] = (XDY_Horodate) (vl_HorodateSec_Ref_Int + vl_HeureCouranteSec - CM_3HEURES);
   XZSM_11ConversionHorodate( vm_Horodate_Ft_Reference[va_fenetre-1],&vl_horodate);

   /* Temps present dans la periode de mesures, definissant le passage a la mesure suivante */
   vm_t_periode[va_fenetre-1] = vl_Horodate_Appel_Int % CM_PERIODE_MESURES;

   XZST_03EcritureTrace( XZSTC_WARNING , "MSCO_init_courbe_ft_ref: Date appel=%f (heure=%d), Debut=%f, Reference=%f",
                           vm_Horodate_Ft_ref_Appel[va_fenetre-1],
                           vl_HeureCouranteSec,
                           vm_Horodate_Ft_ref_Init[va_fenetre-1],
                           vm_Horodate_Ft_Reference[va_fenetre-1] );

  sprintf(vl_msg,"%d|%d|%c|%d!%d!%d!Trafic en fonction du Temps avec Ref. (%s-%s)",CMD_FLASH_INIT_COURBE, CM_COURBE_FT_REF, c_correspFenetre[va_fenetre-1],va_tempsreel,vl_Horodate_Appel_Int,va_nb_courbes,vl_date,vl_datedeb);
  if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
  {
     XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	return XDC_NOK;
}

   MSCO_Afficher_etat_acq_FT_Ref (CM_ETAT_DONNEES_OK, va_fenetre);

   XZST_03EcritureTrace( XZSTC_WARNING , "vl_date '%s', vl_datedeb '%s'",
			vl_date,vl_datedeb);

   /* Afficher les dates de debut et de reference */
   XZSM_11ConversionHorodate(vm_Horodate_Ft_ref_Appel[va_fenetre-1] , &vl_horodate);
   vl_horodate[10]='\0';
	/**TODO*/

   XZSM_11ConversionHorodate(vm_Horodate_Ft_ref_H4[va_fenetre-1] , &vl_horodate);
   vl_horodate[10]='\0';
	/**TODO*/

   vm_Horodate_deltatr[va_fenetre-1] = 0.0;

   vm_caracteristique_courbe[va_fenetre-1].no_station = 0;
   for ( j = 0; j < CM_MAX_COURBES_FT_REF ; j++ )
      {
      i=j%2;
      strcpy( vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station , pa_varcaract[i].nom_station );
      vm_caracteristique_courbe[va_fenetre-1].varcaract[j].no_station = pa_varcaract[i].no_station;
      vm_caracteristique_courbe[va_fenetre-1].varcaract[j].mesure = pa_varcaract[i].mesure;
      vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie = pa_varcaract[i].voie;
      vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens = pa_varcaract[i].sens;

      if (j < vl_nb_courbes)
      {
                  switch ( vm_caracteristique_courbe[va_fenetre-1].varcaract[j].mesure )
                         {
                         case XDC_RADT_Q :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie == 0 )
                                   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_Q , 	
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens );
				else
                                  sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_Q , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie);
                                strcpy(vl_mesure, CM_VARIABLE_Q);
                                vl_max_QVTOPL = vm_max_Q[va_type_ech];
				break;

                         case XDC_RADT_V :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie == 0 )
                                   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_V , 	
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens );
				else
                                  sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_V , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie);
                                strcpy(vl_mesure, CM_VARIABLE_V);
                                vl_max_QVTOPL = vm_max_V[va_type_ech];
				break;

                         case XDC_RADT_TO :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie == 0 )
                                   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_TO , 	
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens );
				else
                                  sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_TO , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie);
                                strcpy(vl_mesure, CM_VARIABLE_TO);
                                vl_max_QVTOPL = vm_max_TO[va_type_ech];
				break;


                         case XDC_RADT_PL :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie == 0 )
                                   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_PL , 	
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens );
				else
                                  sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_PL , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].nom_station, 
						vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie);
                                strcpy(vl_mesure, CM_VARIABLE_PL);
                                vl_max_QVTOPL = vm_max_PL[va_type_ech];
				break;

			 }
			sprintf(vl_msg, "%d|%d|%c|%d!%d!%d!%d!%s!%s!%d", CMD_FLASH_INIT_COURBE, CM_COURBE_FT_DETAIL, c_correspFenetre[va_fenetre-1],
                                        vm_caracteristique_courbe[va_fenetre-1].varcaract[j].no_station ,
                                        vm_caracteristique_courbe[va_fenetre-1].varcaract[j].sens ,
                                        vm_caracteristique_courbe[va_fenetre-1].varcaract[j].voie ,
                                        vm_caracteristique_courbe[va_fenetre-1].varcaract[j].mesure,
                                        vl_mesure ,vl_legend, vl_max_QVTOPL);
			if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
                        {
                           XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
                           return XDC_NOK;
                        }
		}
	}

   vm_caracteristique_courbe[va_fenetre-1].nb_courbes = vl_nb_courbes;
   vm_caracteristique_courbe[va_fenetre-1].tempsreel = XDC_OUI;
   vm_caracteristique_courbe[va_fenetre-1].horodeb = 0;              /* voir table des heures debut par fenetre et par courbe */
   vm_caracteristique_courbe[va_fenetre-1].fenetre = va_fenetre;
   vm_caracteristique_courbe[va_fenetre-1].echelle = va_type_ech;
   vm_caracteristique_courbe[va_fenetre-1].periode = va_periode;


   MSCO_init_donnees_ft_ref(&vm_caracteristique_courbe[va_fenetre-1]);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_init_courbe_ft_ref" );
   return XDC_OK;
}



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Arret de la courbe temps avec reference
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_arret_courbe_temps_ref(
             va_fenetre)

             XDY_Octet va_fenetre;


/*
* ARGUMENTS EN ENTREE :
*   va_fenetre : fenetre dans laquelle est la courbe a effacer
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Fin d'affichage de la courbe ou affichage d'une autre dans la fenetre
*
* FONCTION
*
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 :  MSCO_arret_courbe_temps_ref " ;

   char vl_nom_vag[64];
   char vl_nom[64];
   int i=0,j=0;

   XDY_Booleen vl_detruite;
   int vl_cr = XDC_OK;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :  MSCO_arret_courbe_temps_ref" );

   /*A Arret des abt */
   if ( vm_caracteristique_courbe[va_fenetre-1].tempsreel == XDC_OUI )
      {
      MSCO_Abt( &vm_caracteristique_courbe[va_fenetre-1] ,XZEAC_FIN_ABONNEMENT );
      MSCO_Arret_raf_courbe(va_fenetre);
      vm_caracteristique_courbe[va_fenetre-1].tempsreel = XDC_NON;
      }
   vm_t_periode[va_fenetre-1] = 0;

  if ( MSCO_arret_recuperation_FT_Ref (va_fenetre) != XDC_OK )
      {
      vl_cr = XDC_NOK;
      }


   /*A  Destruction vag */
    for ( i= 0; i < vm_caracteristique_courbe[va_fenetre-1].nb_courbes ; i++)
      {
      XZST_03EcritureTrace(   XZSTC_DEBUG1 , "Destruction variable %d", i);
      sprintf( vl_nom_vag , "%d_%d_%d_%d" , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].no_station ,
                                            vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens ,
                                            vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie ,
                                            vm_caracteristique_courbe[va_fenetre-1].varcaract[i].mesure );
      if ( MSCO_Detruire_vag( vl_nom_vag , &vl_detruite ) != XDC_OK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Erreur a la destruction de la vag %s" , vl_nom_vag );
         vl_cr = XDC_NOK;
         }


         }


      sprintf( vl_nom_vag , "Ft_ref_%d_%d" , i, va_fenetre );
      if ( MSCO_Detruire_vag( vl_nom_vag , &vl_detruite ) != XDC_OK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Erreur a la destruction de la vag %s" , vl_nom_vag);
         vl_cr = XDC_NOK;
         }

      sprintf( vl_nom_vag, "Ft_ref_H4_%d_%d" , i, va_fenetre );
      if ( MSCO_Detruire_vag( vl_nom_vag, &vl_detruite ) != XDC_OK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Erreur a la destruction de la vag %s" , vl_nom_vag);
         vl_cr = XDC_NOK;
         }

	sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_ARRET_COURBE, CM_COURBE_FT_REF, c_correspFenetre[va_fenetre]);
	if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
                   {
                   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
                   return ;
                   }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :  MSCO_arret_courbe_temps_ref" );
   return (vl_cr);
}



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*    Affiche l'etat de l'acquisition de donnees temps reel
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Afficher_etat_acq_FT_Ref (
          va_etat_acq,
          va_fenetre)

          XDY_Octet va_etat_acq;
          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*   va_etat_acq : Indicateur d'etat (OK ou NOK)
*
*
* ARGUMENTS EN SORTIE :
*
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
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : MSCO_Afficher_etat_acq_FT_Ref" ;

   char vl_chaine[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :MSCO_Afficher_etat_acq_FT_Ref " );

   switch ( va_etat_acq )
      {
      case CM_ETAT_DONNEES_OK :
         strcpy( vl_chaine , " ");
         break;
      case CM_ETAT_PERTE_RADT :
         strcpy( vl_chaine , "Perte de donnees RADT");
         break;
      case CM_ETAT_PERTE_REF_BD :
         strcpy( vl_chaine , "Perte d'acces aux donnees de reference");
         break;
      }

	/*TODO*/
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :MSCO_Afficher_etat_acq_FT_Ref " );
   return XDC_OK;
}



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*    Rafraichissement du graphe d'une fenetre apres initialisation
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_raf_courbe_ft_ref_init(
          va_fenetre)

          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*   va_fenetre : fenetre ou est affiche le graphe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Au lancement du graphe, apres initialisation des valeurs des courbes
*
* FONCTION
*   Visualisation des valeurs courantes des courbes apres leur mise a jour
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : MSCO_raf_courbe_ft_ref_init" ;

char vl_nom_var[64];
tm_caracteristique_courbe *pl_caract = &vm_caracteristique_courbe[va_fenetre-1];
int j;
float vl_valeur;
XDY_Horodate vl_horodate;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_raf_courbe_ft_ref_init" );


  for ( j=0; j < pl_caract->nb_courbes ; j++)
  {
    if (pl_caract->varcaract[j].no_station != 0){
      sprintf( vl_nom_var , "%d_%d_%d_%d" , pl_caract->varcaract[j].no_station ,
                                                                                                   pl_caract->varcaract[j].sens ,
                                                                                                   pl_caract->varcaract[j].voie ,
                                                                                                   pl_caract->varcaract[j].mesure );
      MSCO_Valeur_vag_horodate(vl_nom_var, &vl_valeur,&vl_horodate);

      // envoi donnée
/*sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, 
					c_correspFenetre[pl_caract->fenetre-1], vl_horodate, pl_caract->varcaract[j].no_station, 
					pl_caract->varcaract[j].sens, pl_caract->varcaract[j].voie, pl_caract->varcaract[j].mesure, vl_valeur);
	if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
                  {
                           XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
                           return ;
                   }
    */
    }

  }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_raf_courbe_ft_ref_init" );

   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*    Rafraichissement periodique d'un graphe F(t) avec reference
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_raf_courbe_ft_ref(
          va_fenetre)

          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*   va_fenetre : fenetre ou est affiche le graphe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   A chaque reveil par le mecanisme de timer
*
* FONCTION
*   Visualisation des valeurs courantes des courbes apres leur mise a jour
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : MSCO_raf_courbe_ft_ref" ;
   char vl_nom_var[64];
   int j=0;

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN  : MSCO_raf_courbe_ft_ref" );

   /*A Si on a recu des donnees alors on les affiche sinon on se debranche du temps reel ! */
   if ( ( vm_donnees_arrivees > 0 ) && ( vm_nb_donnees_futures[va_fenetre-1] > 0 ) )
      {
      vm_t_periode[va_fenetre-1] = (vm_t_periode[va_fenetre-1] + CM_DUREE_SLOTIME_FT_REF) % CM_PERIODE_MESURES;

      /*A A chaque periode de mesures */
      if ( vm_t_periode[va_fenetre-1] == 0 )
         {
         /*A mettre a jour les donnees de reference */
         for ( j=0; j < vm_caracteristique_courbe[va_fenetre-1].nb_courbes ; j++ )
            {
            sprintf( vl_nom_var , "Ft_ref_%d_%d" , j, va_fenetre );
            if ( (vm_donnees_H4_ref[va_fenetre-1][j][0].Validite == XDC_RADT_CORRECT) ||
                 (vm_donnees_H4_ref[va_fenetre-1][j][0].Validite == XDC_RADT_RECONST) )
               {
               MSCO_Maj_vag( vl_nom_var , (float) vm_donnees_H4_ref[va_fenetre-1][j][0].Valeur );
               }
            else
               {
               MSCO_Maj_vag( vl_nom_var , (float) (-1) );
               }
            }
         }

      /*A Rafraichir les deux graphes */
	MSCO_raf_graphe_ft_ref(va_fenetre);

      /*A A chaque periode de mesures seulement mettre a jour les donnees du graphe H+4 */
      if ( vm_t_periode[va_fenetre-1] == 0 )
         {
         MSCO_Maj_Courbe_H4( &vm_caracteristique_courbe[va_fenetre-1]);
         }
      MSCO_Raf_Courbe_H4(  &vm_caracteristique_courbe[va_fenetre-1]);

	// Envoi Fin de données
         //FIN MAJ
         sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_FIN_DONNEES_COURBE, CM_COURBE_FT, c_correspFenetre[va_fenetre-1]);
         if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
         {
                   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
                   return ;
         }

      vm_Horodate_deltatr[va_fenetre-1] = vm_Horodate_deltatr[va_fenetre-1] + CM_UNE_MINUTE*1.2;

      vm_donnees_arrivees = vm_donnees_arrivees - 1;
      XZST_03EcritureTrace( XZSTC_INFO , "Donnees recues:->%d fen:%d\n", vm_donnees_arrivees , va_fenetre);
      }
   else
      {
      if ( vm_nb_donnees_futures[va_fenetre-1] > 0 )
         XZST_03EcritureTrace( XZSTC_INFO , "Perte de donnees RADT (%d), fenetre %d, arret TR", vm_donnees_arrivees , va_fenetre);
      else
         XZST_03EcritureTrace( XZSTC_INFO , "Donnees de reference non recues, fenetre:%d", va_fenetre);

      MSCO_Arret_raf_courbe(va_fenetre);
      MSCO_Abt( &vm_caracteristique_courbe[va_fenetre-1] , XZEAC_FIN_ABONNEMENT );
      MSCO_arret_recuperation_FT_Ref (va_fenetre);

      vm_caracteristique_courbe[va_fenetre-1].tempsreel = XDC_NON;
      if ( vm_nb_donnees_futures[va_fenetre-1] > 0 )
         MSCO_Afficher_etat_acq_FT_Ref (CM_ETAT_PERTE_RADT, va_fenetre);
      else
         MSCO_Afficher_etat_acq_FT_Ref (CM_ETAT_PERTE_REF_BD, va_fenetre);
      }

   XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : MSCO_raf_courbe_ft_ref" );
   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*    Initialisation du graphe de reference H+4 dans une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Init_Courbe_FT_Ref_H4(
          va_fenetre)

          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*   va_fenetre : fenetre dans laquelle est la courbe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Au lancement de la courbe, apres la lecture des donnees en base
*
* FONCTION
*
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : MSCO_Init_Courbe_FT_Ref_H4" ;


   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : MSCO_Init_Courbe_FT_Ref_H4 " );

   /*A Afficher les premieres donnees H+4 */
   if (MSCO_Maj_Courbe_H4_premiere(  &vm_caracteristique_courbe[va_fenetre-1] ) != XDC_OK)
      {
      XZST_03EcritureTrace(  XZSTC_WARNING , " Erreur d'affichage des donnees H+4 ");
      return XDC_NOK;
      }

   XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : MSCO_Init_Courbe_FT_Ref_H4 " );

   return XDC_OK;
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*    Rafraichissement d'un objet graphe dans une fenetre
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_raf_graphe_ft_ref(
          va_fenetre)

          XDY_Octet va_fenetre;

/*
* ARGUMENTS EN ENTREE :
*   va_fenetre : fenetre ou est affiche le graphe
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   Au lancement du graphe, apres initialisation des valeurs des courbes
*
* FONCTION
*   Simple mise a jour de la visualisation de l'objet
*
-----------------------------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_ft_ref.c	1.15 10/03/07 : MSCO_raf_graphe_ft_ref" ;

    int j;
        tm_caracteristique_courbe *pl_caract = &vm_caracteristique_courbe[va_fenetre-1];
        XDY_Horodate vl_horodate;
        char vl_nom_var[256];
        float vl_valeur;

  for ( j=0; j < pl_caract->nb_courbes ; j++)
        {
	if (pl_caract->varcaract[j].no_station != 0){
		sprintf( vl_nom_var , "%d_%d_%d_%d" , pl_caract->varcaract[j].no_station ,
                                                                                                   pl_caract->varcaract[j].sens ,
                                                                                                   pl_caract->varcaract[j].voie ,
                                                                                                   pl_caract->varcaract[j].mesure );
		MSCO_Valeur_vag_horodate(vl_nom_var, &vl_valeur,&vl_horodate);

		/*TODO horodate*/
		// envoi donnée
                   sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, c_correspFenetre[pl_caract->fenetre-1], 
					vl_horodate, pl_caract->varcaract[j].no_station, 
					pl_caract->varcaract[j].sens, pl_caract->varcaract[j].voie, pl_caract->varcaract[j].mesure, vl_valeur);
		if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
                   {
                           XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
                           return ;
                   }

           }
   }

   return XDC_OK;
}
