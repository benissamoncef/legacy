/*E*/
/*Fichier : $Id: isbi_sco_ft.c,v 1.3 2013/11/22 10:01:59 pc2dpdy Exp $     Release : $Revision: 1.3 $        Date : $Date: 2013/11/22 10:01:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSCO * FICHIER isbi_sco_ft.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier isbi_sco_ft.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	25/09/2013	: Ajout de la date dans le titre 1.3 DEM1085
-----------------------------------------------------*/

/* fichiers inclus */
#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"

/* definitions de constantes */
#define CM_BUFF_SOCK  600
#define CM_NB_FENTRES 2

#define CM_UNE_MINUTE 60

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

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
tm_memocv vm_memo_courbes[CM_NB_MAX_FENETRES];
int       vm_nb_courbes=0;
static XDY_Octet vm_nb_fenetre_active=0;
XDY_Octet vm_donnees_arrivees=0;

XDY_Horodate vm_HorodateSecInit[CM_NB_MAX_FENETRES]={0,0};
XDY_Horodate vm_Horodate_deltatr[CM_NB_MAX_FENETRES]={0,0};
XDY_Horodate vm_HorodateSecInit_H1=0;
//static XDY_Horodate vm_HorodateRef[CM_NB_FENTRES];
//static XDY_Horodate vm_HorodateRefInit[CM_NB_FENTRES];
//static XDY_Horodate vm_HorodateDeb[CM_NB_FENTRES];
//static XDY_Horodate vm_HorodateFin[CM_NB_FENTRES];
static float  vm_duree=24.0;

static XDY_Booleen vm_echelle_Q;
static XDY_Booleen vm_echelle_V;
static XDY_Booleen vm_echelle_TO;
static XDY_Booleen vm_echelle_PL;

static vm_typetemps[CM_NB_FENTRES] = {NULL,NULL};

//static tm_varcaract vm_courbes_ft[CM_MAX_COURBES_FT];
static tm_caracteristique_courbe vm_fenetres_ft[CM_NB_FENTRES];
tm_caracteristique_courbe vm_caracteristique_courbe[CM_NB_MAX_FENETRES]={{0,0,NULL,0,XDC_NON,0.0,0,0},
									{0,0,NULL,0,XDC_NON,0.0,0,0}};


static T_CB vm_callback_ft[CM_NB_FENTRES];
int vm_no_courbe_init_ft[CM_NB_FENTRES] = {0,0};

char vl_msg[CM_BUFF_SOCK];

tm_Tab_LignesDonnees vm_donneesbd[CM_MAX_COURBES_FT];
//DATAGROUP vm_dgp_selectionne[CM_NB_MAX_FENETRES]={NULL,NULL,NULL};
int 	 vm_slot[CM_NB_MAX_FENETRES]={0,0,0};
static XDY_Octet vm_fenetre;

static int vm_max_Q[4]={0, 1000 , 3000 , 10000 };
static int vm_max_V[4]={0, 200 , 200 , 200 };
static int vm_max_TO[4]={0, 10 , 20 , 30 };
static int vm_max_PL[4]={0, 100 , 100 , 100 };


/* declaration de variables externes */
extern char c_correspFenetre[];
extern XDY_Horodate vm_horodate_courbe;
extern float vm_donnees_H1[CM_NB_DONNEES_H1][CM_MAX_COURBES_FT][CM_NB_MAX_FENETRES];

/* declaration de fonctions internes */
extern void MSRA03_raf_RADT();
void MSCO02_raf_vag();

/* definition de fonctions externes */


int MSCO01_arret_courbe_temps(
		va_fenetre)

             XDY_Octet va_fenetre;


/*
* ARGUMENTS EN ENTREE :
* va_fenetre : fenetre dans laquelle est la courbe a effacer
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
{static char *version = "@(#)isyn_sco.c	1.44 11/19/10 :  MSCO01_arret_courbe_temps " ;

   char vl_nom_var[256];
   char vl_nom_vag[256];
   char vl_nom[256];
   int i=0,j=0;

   XDY_Booleen vl_detruite;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :  MSCO01_arret_courbe_temps" );

   vm_echelle_Q = XDC_NON;
   vm_echelle_V = XDC_NON;
   vm_echelle_TO = XDC_NON;
   vm_echelle_PL = XDC_NON;

   sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_ARRET_COURBE, CM_COURBE_FT, c_correspFenetre[va_fenetre -1]);
   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
 	   {
 	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
 	   return XDC_NOK;
 	   }

   /*A  Destruction vag et dsv */
//    for ( i= 1; i <= vm_caracteristique_courbe[va_fenetre-1].nb_courbes ; i++)
    for ( i= 1; i <= vm_caracteristique_courbe[va_fenetre -1].nb_courbes ; i++)
      {
      XZST_03EcritureTrace(   XZSTC_DEBUG1 , "Destruction variable %d", i);
      /*A Insertion vag */
      sprintf( vl_nom_vag , "%d_%d_%d_%d" , vm_caracteristique_courbe[va_fenetre -1].varcaract[i-1].no_station ,
                                            vm_caracteristique_courbe[va_fenetre -1].varcaract[i-1].sens ,
                                            vm_caracteristique_courbe[va_fenetre -1].varcaract[i-1].voie ,
                                            vm_caracteristique_courbe[va_fenetre -1].varcaract[i-1].mesure );
      if ( MSCO_Detruire_vag( vl_nom_vag , &vl_detruite ) != XDC_OK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la destruction de la var %s dans la table des vag courbes" , vl_nom_var );
         }

      }

   /*A Arret des abt */
   if ( vm_caracteristique_courbe[va_fenetre -1].tempsreel == XDC_OUI )
      {
      MSCO_Arret_raf_courbe(va_fenetre);
      MSCO_Abt( &vm_caracteristique_courbe[va_fenetre -1] ,XZEAC_FIN_ABONNEMENT );
      vm_caracteristique_courbe[va_fenetre -1].tempsreel = XDC_NON;
      }

   /*A Destruction de l'objet graph dans la thash des fonction operateur*/
  if ( MSCO_arret_recuperation(va_fenetre) == XDC_NOK )
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
				   "Desarmement de la Recuperation de donnees %s non effectuee. \n " ,XDG_IDONNEES_FT);
         return XDC_NOK;
         }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :  MSCO01_arret_courbe_temps" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO01_init_courbe_temps(
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
*  va_tempsreel : XDC_OUI si c'est une courbe tempsreel, XDC_NON si c'est une courbe temps diff
*  va_horodeb : horodate de debut pour une courbe en temps diff
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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO01_init_courbe_temps " ;

   int code_retour=0;
   char vl_nom_vag[256];
   int i=0,j=0;
   char vl_legend[100];
   char vl_mesure[3];
   int vl_max_QVTOPL;
   struct tm               vl_Enr ;
   int    vl_JourSemaine;
   XDY_Horodate vl_HorodateSec;

   float vl_coef=0.0;
   XZSMT_Horodate vl_horodate;
   XZSMT_Horodate vl_date;
   XDY_Booleen vl_periode;
   char vl_horo_ronde[25];
   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSCO01_init_courbe_temps  va_nb_courbes:%d; va_tempsreel:%d;va_horodeb:%s;va_fenetre:%d;va_periode:%d" ,
   va_nb_courbes, va_tempsreel,va_horodeb,va_fenetre,(int) va_periode);

   if (va_periode==1)
     vl_periode=XDC_OUI;
   else
     vl_periode=XDC_NON;
   vm_echelle_Q = XDC_NON;
   vm_echelle_V = XDC_NON;
   vm_echelle_TO = XDC_NON;
   vm_echelle_PL = XDC_NON;

   vm_slot[va_fenetre-1]=0;

   /*A Si la fenetre en arg n'est ni gauche ni droite, on impose que ce soit la droite */
   if (( va_fenetre-1 != CM_FENETRE_GAUCHE ) && (va_fenetre-1 != CM_FENETRE_DROITE ))
      va_fenetre = CM_FENETRE_DROITE+1;

   if (( va_nb_courbes < 1 ) || ( va_nb_courbes > CM_MAX_COURBES_FT ))
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " Nombre de courbes incorrect : %d" , va_nb_courbes);
      return XDC_NOK;
      }

   vm_fenetre = va_fenetre;


   vm_memo_courbes[vm_nb_courbes].type=CM_COURBE_FT;
   vm_memo_courbes[vm_nb_courbes].fenetre=vm_fenetre;
   vm_nb_courbes=vm_nb_courbes+1;

   /*A  creation vag et dsv */
    for ( i= 0; i < va_nb_courbes ; i++)
      {
      /*A Insertion vag */
      sprintf( vl_nom_vag , "%d_%d_%d_%d" , pa_varcaract[i].no_station , pa_varcaract[i].sens , pa_varcaract[i].voie , pa_varcaract[i].mesure );
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
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de l'insertion de la var %s dans la table des vag courbes" , vl_nom_vag );
         }

      }

  sscanf(va_horodeb
          ,"%02u/%02u/%04u %02u:%02u:%02u"
          ,&vl_Enr.tm_mday
          ,&vl_Enr.tm_mon
          ,&vl_Enr.tm_year
          ,&vl_Enr.tm_hour
          ,&vl_Enr.tm_min
          ,&vl_Enr.tm_sec);
   vl_Enr.tm_min = floor(vl_Enr.tm_min/6+1) * 6;
   sprintf(vl_horo_ronde,"%02u/%02u/%04u %02u:%02u:%02u"
          ,vl_Enr.tm_mday
          ,vl_Enr.tm_mon
          ,vl_Enr.tm_year
          ,vl_Enr.tm_hour
          ,vl_Enr.tm_min
          ,vl_Enr.tm_sec);
    XZSM_13ConversionHorodateSec( vl_horo_ronde , &vl_HorodateSec);
   XZST_03EcritureTrace(   XZSTC_WARNING , " HORO %s %lf",vl_horo_ronde, vl_HorodateSec);

/*   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);*/
   if ( va_tempsreel == XDC_OUI )
      {
      vm_HorodateSecInit[va_fenetre-1] = vl_HorodateSec - CM_6HEURES;

      vm_HorodateSecInit_H1 = vl_HorodateSec;
      }
   else
      {
      if ( XZSM_13ConversionHorodateSec( va_horodeb , &vm_HorodateSecInit[va_fenetre-1]) != XDC_OK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", va_horodeb );
         MSCO01_arret_courbe_temps(va_fenetre);
         return XDC_NOK;
         }

	vm_HorodateSecInit[va_fenetre-1] = vl_HorodateSec;
      vm_HorodateSecInit_H1 = vm_HorodateSecInit[va_fenetre-1] + CM_6HEURES;
      }

   vm_typetemps[va_fenetre-1] = va_tempsreel;
  strncpy(vl_date,va_horodeb,10);
  vl_date[10]=0;
   sprintf(vl_msg, "%d|%d|%c|%d!%f!%d!Trafic en fonction du Temps (%10s)!%d", CMD_FLASH_INIT_COURBE, CM_COURBE_FT, c_correspFenetre[va_fenetre-1], va_tempsreel, vm_HorodateSecInit[va_fenetre-1],va_nb_courbes, vl_date ,vl_periode?1:6);
   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   return XDC_NOK;
   }
	
   sleep(1);

   vm_Horodate_deltatr[va_fenetre-1] = 0.0;

   vm_caracteristique_courbe[va_fenetre-1].no_station = 0;
   for ( i = 0; i < CM_MAX_COURBES_FT ; i++ )
      {
      strcpy( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station , pa_varcaract[i].nom_station );
      vm_caracteristique_courbe[va_fenetre-1].varcaract[i].no_station = pa_varcaract[i].no_station;
      vm_caracteristique_courbe[va_fenetre-1].varcaract[i].mesure = pa_varcaract[i].mesure;
      vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie = pa_varcaract[i].voie;
      vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens = pa_varcaract[i].sens;
	 XZST_03EcritureTrace(   XZSTC_WARNING ,"pa_varcaract[i].no_station %d",
					pa_varcaract[i].no_station);

      if (i < va_nb_courbes)
      {
		  /* Legende */
		  switch ( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].mesure )
			 {
			 case XDC_RADT_Q :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie == 0 )
				   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_Q , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens );
				else
				   sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_Q , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie);
				strcpy(vl_mesure, CM_VARIABLE_Q);
				vl_max_QVTOPL = vm_max_Q[va_type_ech];
				break;
			 case XDC_RADT_V :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie == 0 )
				   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_V , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens );
				else
				   sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_V , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie);
				strcpy(vl_mesure, CM_VARIABLE_V);
				vl_max_QVTOPL = vm_max_V[va_type_ech];
				break;
			 case XDC_RADT_TO :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie == 0 )
				   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_TO , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens );
				else
				   sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_TO , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie);
				strcpy(vl_mesure, CM_VARIABLE_TO);
				vl_max_QVTOPL = vm_max_TO[va_type_ech];
				break;
			 case XDC_RADT_PL :
				if ( vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie == 0 )
				   sprintf( vl_legend , "%2s (%s sens %d ch.)" , CM_VARIABLE_PL , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens );
				else
				   sprintf( vl_legend , "%2s (%s sens %d v %d)" , CM_VARIABLE_PL , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].nom_station, vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens , vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie);
				strcpy(vl_mesure, CM_VARIABLE_PL);
				vl_max_QVTOPL = vm_max_PL[va_type_ech];
				break;
			 }
			sprintf(vl_msg, "%d|%d|%c|%d!%d!%d!%d!%s!%s!%d", CMD_FLASH_INIT_COURBE, CM_COURBE_FT_DETAIL, c_correspFenetre[va_fenetre-1],
					vm_caracteristique_courbe[va_fenetre-1].varcaract[i].no_station ,
					vm_caracteristique_courbe[va_fenetre-1].varcaract[i].sens ,
					vm_caracteristique_courbe[va_fenetre-1].varcaract[i].voie ,
					vm_caracteristique_courbe[va_fenetre-1].varcaract[i].mesure,
					vl_mesure ,vl_legend, vl_max_QVTOPL);

			if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
			{
			   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
			   return XDC_NOK;
			}
      }
   }

   vm_caracteristique_courbe[va_fenetre-1].nb_courbes = va_nb_courbes;
   vm_caracteristique_courbe[va_fenetre-1].tempsreel = va_tempsreel;
   vm_caracteristique_courbe[va_fenetre-1].horodeb = vm_HorodateSecInit[va_fenetre-1];
   vm_caracteristique_courbe[va_fenetre-1].fenetre = va_fenetre;
   vm_caracteristique_courbe[va_fenetre-1].echelle = va_type_ech;
   vm_caracteristique_courbe[va_fenetre-1].periode = vl_periode;

   MSCO_init_donnees_ft(&vm_caracteristique_courbe[va_fenetre-1]);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO01_init_courbe_temps" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO03_raf_courbe_temps_init(
          va_fenetre)

          XDY_Octet va_fenetre;

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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO03_raf_courbe_temps_init" ;
   int i=0;

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSCO03_raf_courbe_temps_init" );

   for (i=0; i < (( CM_6mn ) / ( CM_COEF_AFF ) ); i++ )
      {
      MSCO_MemoDernieresDonnees(&vm_caracteristique_courbe[va_fenetre-1]);
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO03_raf_courbe_temps_init" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Abt(
         pa_caract,
         va_debutfin)

         tm_caracteristique_courbe *pa_caract;
         XDY_Booleen va_debutfin;

/*
* ARGUMENTS EN ENTREE :
*  pa_caract : Caracteristiques de la courbe.
* va_debutfin: indicateur de debut ou de fin d'abonnement
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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_Abt" ;
   int i=0;
  XZSCT_Datagroup vl_datagroup_init ;
  int code_retour=XDC_OK;
  char vl_chaine[10];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Abt" );



   for ( i = 0; i < pa_caract->nb_courbes ; i++)
      {

      /*A Si la machine est un PC simplifie, taitement en 6mn */
      if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
      {
      if ( ( va_debutfin !=XZEAC_DEBUT_ABONNEMENT ) &&  ( MSCO_ExisteAbt( &(pa_caract->varcaract[i]) , pa_caract->fenetre-1, vm_nb_fenetre_active) == XDC_OUI ) ||
           ( (pa_caract->varcaract[i].voie == XDC_RADT_CHAUSSEE ) //&&
//                      ( (MSRA_RADT_Animee( pa_caract->varcaract[i].no_station , pa_caract->varcaract[i].sens) == XDC_OK ) || (MSOP_Panneau_Abonne( pa_caract->varcaract[i].no_station, pa_caract->varcaract[i].sens ) == XDC_OK )) )
        		   )
          )
         {
         XZST_03EcritureTrace(   XZSTC_INFO, "L'abt existe deja ");
	 }
      else
         {
         sprintf( vl_chaine ,"%d%d%d", pa_caract->varcaract[i].no_station,
   				 pa_caract->varcaract[i].sens,
                                 pa_caract->varcaract[i].voie  );
         XDG_EncodeDG3(vl_datagroup_init , XDG_IABTINIT_RADT , vl_chaine , vm_NomMachine );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA32_Abt_Mesures_6mn_parVoie " );
         XZEA32_Abt_Mesures_6mn_parVoie( va_debutfin , vl_datagroup_init , MSRA03_raf_RADT ,  pa_caract->varcaract[i].no_station , pa_caract->varcaract[i].sens , pa_caract->varcaract[i].voie , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA32_Abt_Mesures_6mn_parVoie %d " , code_retour );
         if ( code_retour != XDC_OK )
             {
             XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
             return code_retour;
             }
         }
      }
      else
      {
      /*A Abonnement XZEA02_Abt_Mesures_6gl_Voie  */
      sprintf( vl_chaine ,"%d%d%d", pa_caract->varcaract[i].no_station,
   				 pa_caract->varcaract[i].sens,
                                 pa_caract->varcaract[i].voie  );
      XDG_EncodeDG3(vl_datagroup_init , XDG_IABTINIT_RADT_COURBES , vl_chaine , vm_NomMachine );

      XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA02_Abt_Mesures_6gl_Voie " );
      if ( va_debutfin == XZEAC_FIN_ABONNEMENT )
         {
         if ( MSCO_ExisteAbt( &(pa_caract->varcaract[i]) , pa_caract->fenetre-1, vm_nb_fenetre_active) == XDC_NON )
            {	/* ihm9 */
            XZST_03EcritureTrace(XZSTC_INFO , "Desabonnement %s - fenetre %d, periode %d",
                                    vl_datagroup_init, pa_caract->fenetre, pa_caract->periode);
                  if ( pa_caract->periode == XDC_OUI )
                  	XZEA01_Abt_Mesures_6gl_Voie ( va_debutfin ,
             						     vl_datagroup_init ,
                                                             MSCO02_raf_vag ,
                                                             (char *) vm_NomSite ,
                                                             pa_caract->varcaract[i].no_station,
                                                             pa_caract->varcaract[i].sens,
                                                             pa_caract->varcaract[i].voie,
                                                             &code_retour );
                  else
                  	         XZEA32_Abt_Mesures_6mn_parVoie( va_debutfin ,
                  	         				vl_datagroup_init ,
                  	         				MSCO02_raf_vag ,
                  	         				pa_caract->varcaract[i].no_station ,
                  	         				pa_caract->varcaract[i].sens ,
                  	         				pa_caract->varcaract[i].voie ,
                  	         				&code_retour );
            }
         }
      else
         {
         if (( MSCO_ExisteAbt( &(pa_caract->varcaract[i]) , pa_caract->fenetre-1, vm_nb_fenetre_active+1) == XDC_NON ) || ( va_debutfin == XZEAC_DEBUT_ABONNEMENT ) )
            {/* ihm9 */
            XZST_03EcritureTrace (XZSTC_INFO , "Abonnement %s - fenetre %d, periode %d",
                                     vl_datagroup_init, pa_caract->fenetre, pa_caract->periode);
                  if ( pa_caract->periode == XDC_OUI )
                  	XZEA01_Abt_Mesures_6gl_Voie ( va_debutfin ,
             						     vl_datagroup_init ,
                                                             MSCO02_raf_vag ,
                                                             (char *) vm_NomSite ,
                                                             pa_caract->varcaract[i].no_station,
                                                             pa_caract->varcaract[i].sens,
                                                             pa_caract->varcaract[i].voie,
                                                             &code_retour );
                  else
                  	         XZEA32_Abt_Mesures_6mn_parVoie( va_debutfin ,
                  	         					vl_datagroup_init ,
                  	         					MSCO02_raf_vag ,
                  	         					pa_caract->varcaract[i].no_station ,
                  	         					pa_caract->varcaract[i].sens ,
                  	         					pa_caract->varcaract[i].voie ,
                  	         					&code_retour );
            }
         }

      XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA02_Abt_Mesures_6gl_voie %d " , code_retour );
      if ( code_retour != XDC_OK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
         code_retour= XDC_OK;
         }
      }
      }

      if ( va_debutfin == XZEAC_DEBUT_ABONNEMENT)
         {
         vm_nb_fenetre_active = vm_nb_fenetre_active + 1;
         vm_donnees_arrivees = vm_donnees_arrivees+vm_nb_fenetre_active;
         }
      else
         {
         if ( vm_nb_fenetre_active > 0 )
            vm_nb_fenetre_active = vm_nb_fenetre_active - 1;
         else
            vm_nb_fenetre_active = 0;
         }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Abt" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO02_raf_vag(
          va_etat_TRA,
          pa_result
          )

          XDY_EMes_RADT va_etat_TRA;
          int *pa_result;
/*
* ARGUMENTS EN ENTREE :
*   va_etat_TRA : Descriptif de l'etat du TRAfic (donnees radt) Q V TO PL
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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO02_raf_vag" ;
   char vl_nom_var[256];
   float vl_zero=0.0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO02_raf_vag" );

   XZST_03EcritureTrace(   XZSTC_WARNING , "MSCO02_raf_vag: NO:%d Sens:%d Voie:%d ", va_etat_TRA.Numero, va_etat_TRA.Sens,
                                         va_etat_TRA.Voie  );

   XZST_03EcritureTrace(   XZSTC_WARNING , "MSCO02_raf_vag: H:%lf Q:%d V:%d TO:%d PL:%d",
					 va_etat_TRA.Horodate,
                                         va_etat_TRA.Mesures.ValQ.Val,
                                         va_etat_TRA.Mesures.ValV.Val,
                                         va_etat_TRA.Mesures.ValTO.Val,
                                         va_etat_TRA.Mesures.ValPL.Val );


   sprintf( vl_nom_var , "%d_%d_%d_%d" , va_etat_TRA.Numero , va_etat_TRA.Sens,
                                         va_etat_TRA.Voie   , XDC_RADT_Q );
   if ( va_etat_TRA.Mesures.ValQ.Indic == XDC_RADT_INCONNU )
      MSCO_Maj_vag_horodate( vl_nom_var , (float) vl_zero, va_etat_TRA.Horodate);
   else
      MSCO_Maj_vag_horodate( vl_nom_var , (float) va_etat_TRA.Mesures.ValQ.Val, va_etat_TRA.Horodate);

   sprintf( vl_nom_var , "%d_%d_%d_%d" , va_etat_TRA.Numero , va_etat_TRA.Sens,
                                         va_etat_TRA.Voie   , XDC_RADT_V );
   if ( va_etat_TRA.Mesures.ValV.Indic == XDC_RADT_INCONNU )
      MSCO_Maj_vag_horodate( vl_nom_var , (float) vl_zero, va_etat_TRA.Horodate);
   else
      MSCO_Maj_vag_horodate( vl_nom_var , (float) va_etat_TRA.Mesures.ValV.Val, va_etat_TRA.Horodate);

   sprintf( vl_nom_var , "%d_%d_%d_%d" , va_etat_TRA.Numero , va_etat_TRA.Sens,
                                         va_etat_TRA.Voie   , XDC_RADT_TO );
   if ( va_etat_TRA.Mesures.ValTO.Indic == XDC_RADT_INCONNU )
      MSCO_Maj_vag_horodate( vl_nom_var , (float) vl_zero, va_etat_TRA.Horodate);
   else
      MSCO_Maj_vag_horodate( vl_nom_var , (float) va_etat_TRA.Mesures.ValTO.Val, va_etat_TRA.Horodate);

   sprintf( vl_nom_var , "%d_%d_%d_%d" , va_etat_TRA.Numero , va_etat_TRA.Sens,
                                         va_etat_TRA.Voie   , XDC_RADT_PL );
   if ( va_etat_TRA.Mesures.ValPL.Indic == XDC_RADT_INCONNU )
      *pa_result=MSCO_Maj_vag_horodate( vl_nom_var , (float) vl_zero, va_etat_TRA.Horodate);
   else
      *pa_result=MSCO_Maj_vag_horodate( vl_nom_var , (float) va_etat_TRA.Mesures.ValPL.Val, va_etat_TRA.Horodate);

   /* Si vm_arrivee n'a pas ete lu ou mis a jour par des donnees 6mn alors on affecte vm_donnees_arrivees = vm_nb_fenetre_activesinon on affect le nb de fen. active par 6 */
   if ( va_etat_TRA.Periode == 1 )
      {
      if ( vm_donnees_arrivees < 6 )
   	vm_donnees_arrivees = vm_nb_fenetre_active;
      }
   else
   	vm_donnees_arrivees = vm_nb_fenetre_active*va_etat_TRA.Periode;

   XZST_03EcritureTrace(   XZSTC_INFO , " Reception donnees:%d",vm_donnees_arrivees );

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO02_raf_vag *pa_result:%d",*pa_result );
   return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Init_Courbe_H1(
          va_fenetre)

          XDY_Octet va_fenetre;


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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_Init_Courbe_H1" ;
   tm_donnees_H1 vl_donnees_courbes[CM_NB_DONNEES_H1];
   tm_donnees_H1 vl_donnees_H1[CM_NB_DONNEES_H1];

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSCO_Init_Courbe_H1" );

   /*A Affichage des donnees H1 */
   if (  MSCO_Maj_Courbe_H1_premiere( &vm_caracteristique_courbe[va_fenetre-1] ) != XDC_OK ) /* V1.22 */
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , " Probleme lors de l'affichage des donnees H+1 ");
      return XDC_NOK;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Init_Courbe_H1" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de savoir si une autre fenetre f(t) a le meme abt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_ExisteAbt(
          pa_caract,
          va_fenetre,
          va_nb_activ)
	tm_varcaract    *pa_caract;
	XDY_Octet	va_fenetre;
	XDY_Octet	va_nb_activ;

/*
* ARGUMENTS EN ENTREE :
*   pa_caract : caracteristique de l'abt a arreter
*   va_fenetre :  pour laquelle on arret l'abt
*   va_nb_activ : nb de fenetres actives
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OUI : il existe une autre fenetre qui a cet abt
*   XDC_NON : il n'existe pas une autre fenetre qui a cet abt
*
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_ExisteAbt " ;
   XDY_Octet vl_autre_fenetre=0;
   tm_varcaract    *pl_autre_caract;
   int vl_ind=0;
   int vl_retour=0;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_ExisteAbt " );


   vl_autre_fenetre= (va_fenetre+1) % 2;

   /*A Si l'autre fenetre n'est pas temps reel alors retourne non */
   if ( ( vm_caracteristique_courbe[vl_autre_fenetre].tempsreel == XDC_NON ) ||
        ( vm_caracteristique_courbe[vl_autre_fenetre].periode != vm_caracteristique_courbe[va_fenetre].periode ))
      {
      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_ExisteAbt " );
      return XDC_NON;
      }

   pl_autre_caract = vm_caracteristique_courbe[vl_autre_fenetre].varcaract;

   vl_ind=0;
   while ( ( vl_ind < vm_caracteristique_courbe[vl_autre_fenetre].nb_courbes ) &&
         ( ( pl_autre_caract[vl_ind].no_station != pa_caract->no_station ) ||
           ( pl_autre_caract[vl_ind].sens != pa_caract->sens ) ||
           ( pl_autre_caract[vl_ind].voie != pa_caract->voie )  ))
     {
     vl_ind++;
     }

   /*A Si on trouve dans l'autre fenetre les meme caract d'abt alors on repond oui */
   if ( vl_ind < vm_caracteristique_courbe[vl_autre_fenetre].nb_courbes )
     {
      vl_retour=XDC_OUI;
     }
   /*A Sinon on repond non */
   else
     {
      vl_retour=XDC_NON;
     }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_ExisteAbt " );
   return vl_retour;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de savoir si une  fenetre f(t) a l'abt de meme cracateristique
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Existe_Courbe_Abt(
               XDY_Eqt va_no_station,
               XDY_Voie va_voie,
               XDY_Sens va_sens )

/*
* ARGUMENTS EN ENTREE :
*   va_no_station : numero de la station
*   va_voie :  voie de la station
*   va_sens : sens de la station
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   XDC_OUI : il existe une  fenetre qui a cet abt
*   XDC_NON : il n'existe pas une  fenetre qui a cet abt
*
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_Existe_Courbe_Abt " ;
   XDY_Octet vl_autre_fenetre=0;
   tm_varcaract    vl_caract;
   int vl_ind=0;
   int vl_retour=0;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Existe_Courbe_Abt " );


   vl_caract.no_station=va_no_station;
   vl_caract.voie=va_voie;
   vl_caract.sens=va_sens;
   strcpy( vl_caract.nom_station , "");

   if ( ( MSCO_ExisteAbt( &vl_caract, CM_FENETRE_DROITE , vm_nb_fenetre_active ) == XDC_OUI ) ||
      ( MSCO_ExisteAbt( &vl_caract, CM_FENETRE_GAUCHE , vm_nb_fenetre_active ) == XDC_OUI ) )
     {
      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Existe_Courbe_Abt OUI" );
      vl_retour=XDC_OUI;
     }
   /*A Sinon on repond non */
   else
     {
      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Existe_Courbe_Abt NON" );
      vl_retour=XDC_NON;
     }
  return   vl_retour;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO03_raf_courbe_temps(
          va_fenetre)

          XDY_Octet va_fenetre;

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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO03_raf_courbe_temps" ;
	tm_caracteristique_courbe *pl_caract = &vm_caracteristique_courbe[va_fenetre-1];
	int j,t;
	XDY_Horodate vl_horodate;
	char vl_nom_var[256];
	float vl_valeur;

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSCO03_raf_courbe_temps" );


   /*A Si on a recu des donnes alors on les affiche sinon on se debranche du temps reel ! */
   if ( vm_donnees_arrivees > 0 )
      {


	   vl_horodate = vm_HorodateSecInit[va_fenetre-1] + CM_6HEURES + vm_Horodate_deltatr[va_fenetre-1];
	 XZST_03EcritureTrace(   XZSTC_WARNING, "vm_HorodateSecInit[va_fenetre-1]  %lf",vm_HorodateSecInit[va_fenetre-1] );
	XZST_03EcritureTrace(   XZSTC_WARNING, "vm_Horodate_deltatr[va_fenetre-1] %lf",vm_Horodate_deltatr[va_fenetre-1]);
      /*A Mise a jour de la courbe H+1 */
      MSCO_MemoDernieresDonnees( &vm_caracteristique_courbe[va_fenetre-1] );

      /*MSCO_Maj_Courbe_H1(&vm_caracteristique_courbe[va_fenetre-1]); */ /*JM*/

      vm_Horodate_deltatr[va_fenetre-1] = vm_Horodate_deltatr[va_fenetre-1] + CM_UNE_MINUTE*1.2;

	   	for ( j=0; j < CM_MAX_COURBES_FT ; j++ )
	   	   {
	   		   if (pl_caract->varcaract[j].no_station != 0){
	   			   sprintf( vl_nom_var , "%d_%d_%d_%d" , pl_caract->varcaract[j].no_station ,
	   													   pl_caract->varcaract[j].sens ,
	   													   pl_caract->varcaract[j].voie ,
	   													   pl_caract->varcaract[j].mesure );

	   			   /*MSCO_Valeur_vag(vl_nom_var, &vl_valeur);*/
				MSCO_Valeur_vag_horodate(vl_nom_var, &vl_valeur, &vl_horodate);	
	   			   //F(T)
	   			      //   horodate = vm_HorodateSecInit[va_fenetre-1] + vm_Horodate_deltatr[va_fenetre-1]
	   			      /*
	   			   	* envoyer la vag
	   			   	*/

	   			   // envoi donnée
	   			   sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, c_correspFenetre[pl_caract->fenetre-1], vl_horodate, pl_caract->varcaract[j].no_station, pl_caract->varcaract[j].sens, pl_caract->varcaract[j].voie, pl_caract->varcaract[j].mesure, vl_valeur);
	   			   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	   			   {
	   				   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   				   return ;
	   			   }

				   /* JM*/
				   /*
	   			   for (t=0; t < CM_NB_DONNEES_H1 ; t++ ){
					   sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_EXTRPOLEE, c_correspFenetre[pl_caract->fenetre-1], vl_horodate + (t+1)*CM_COEF_AFF*CM_UNE_MINUTE, pl_caract->varcaract[j].no_station, pl_caract->varcaract[j].sens, pl_caract->varcaract[j].voie, pl_caract->varcaract[j].mesure, vl_valeur);
					   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
					   {
						   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
						   return ;
					   }
	   			   } */
	   		   }
	   	   }

		 //FIN MAJ
		 sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_FIN_DONNEES_COURBE, CM_COURBE_FT, c_correspFenetre[pl_caract->fenetre-1]);
		 if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
		 {
			   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
			   return ;
		 }


      vm_donnees_arrivees = vm_donnees_arrivees - 1;
       XZST_03EcritureTrace(   XZSTC_WARNING , "Affich donnees recues:->%d fen:%d\n", vm_donnees_arrivees , va_fenetre);
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_INFO , "pas de donnees recues:%d fen:%d, arret tr", vm_donnees_arrivees , va_fenetre);
      MSCO_Arret_raf_courbe(va_fenetre);
      MSCO_Abt( &vm_caracteristique_courbe[va_fenetre-1] , XZEAC_FIN_ABONNEMENT );
      vm_caracteristique_courbe[va_fenetre-1].tempsreel = XDC_NON;
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO03_raf_courbe_temps" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO03_raf_courbe_temps6mn(
          va_fenetre)

          XDY_Octet va_fenetre;

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
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO03_raf_courbe_temps6mn" ;
   int i=0;
   int j, t;
   tm_caracteristique_courbe *pl_caract = &vm_caracteristique_courbe[va_fenetre-1];
   XDY_Horodate vl_horodate;
   char vl_nom_var[256];
   float vl_valeur;

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSCO03_raf_courbe_temps6mn" );


   /*A Si on a recu des donnes alors on les affiche sinon on se debranche du temps reel ! */
   if ( vm_donnees_arrivees > 0 )
      {
	   vl_horodate = vm_HorodateSecInit[va_fenetre-1] + CM_6HEURES + vm_Horodate_deltatr[va_fenetre-1];

	   for (i=0; i < (( CM_6mn ) / ( CM_COEF_AFF ) ); i++ )
	   {
		  /*A Mise a jour de la courbe H+1 */
		  MSCO_MemoDernieresDonnees( &vm_caracteristique_courbe[va_fenetre-1] );

/*		  MSCO_Maj_Courbe_H1(&vm_caracteristique_courbe[va_fenetre-1]);*/ /*JM*/

		  vm_Horodate_deltatr[va_fenetre-1] = vm_Horodate_deltatr[va_fenetre-1] + CM_UNE_MINUTE*CM_COEF_AFF;
	   }

	   	for ( j=0; j < CM_MAX_COURBES_FT ; j++ )
	   	   {
		XZST_03EcritureTrace(   XZSTC_WARNING, "%d",pl_caract->varcaract[j].no_station);
	   		   if (pl_caract->varcaract[j].no_station != 0){
	   			   sprintf( vl_nom_var , "%d_%d_%d_%d" , pl_caract->varcaract[j].no_station ,
	   						   pl_caract->varcaract[j].sens ,
	   						   pl_caract->varcaract[j].voie ,
	   						   pl_caract->varcaract[j].mesure );

	   			   MSCO_Valeur_vag(vl_nom_var, &vl_valeur);

	   		/*	   MSCO_Envoyer_Donnees(j, vl_horodate, pl_caract, vl_valeur);*/

	   			   // envoi donnée
	   			   sprintf(vl_msg, "|%d|%d|%c|%f!%d!%d!%d!%d!%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT_REEL, c_correspFenetre[pl_caract->fenetre-1], vl_horodate, pl_caract->varcaract[j].no_station, pl_caract->varcaract[j].sens, pl_caract->varcaract[j].voie, pl_caract->varcaract[j].mesure, vl_valeur);
	   			   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	   			   {
	   				   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   				   return ;
	   			   }
	   		   }
	   	   }

   //FIN MAJ
   sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_FIN_DONNEES_COURBE, CM_COURBE_FT, c_correspFenetre[pl_caract->fenetre-1]);
   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   return ;
   }




   vm_donnees_arrivees = vm_donnees_arrivees - 6;
   XZST_03EcritureTrace(   XZSTC_INFO , "Affich donnees recues:->%d fen:%d\n", vm_donnees_arrivees , va_fenetre);
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_INFO , "pas de donnees recues:%d fen:%d, arret tr", vm_donnees_arrivees , va_fenetre);
      MSCO_Arret_raf_courbe(va_fenetre);
      MSCO_Abt( &vm_caracteristique_courbe[va_fenetre-1] , XZEAC_FIN_ABONNEMENT );
      vm_caracteristique_courbe[va_fenetre-1].tempsreel = XDC_NON;
      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO03_raf_courbe_temps6mn" );
   return XDC_OK;
}

#if 0
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO01_init_courbe_temps(
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
*  va_tempsreel : XDC_OUI si c'est une courbe tempsreel, XDC_NON si c'est une courbe temps diff
*  va_horodeb : horodate de debut pour une courbe en temps diff
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
{static char *version = "@(#)isbi_sco_ft.c	1.43 09/28/07 : MSCO01_init_courbe_temps " ;

	char vl_titre[100];
	int i;
	int    vl_JourSemaine;
	XDY_Horodate vl_HorodateSec;
	XZSCT_Datagroup vl_datagroup ;
	T_BOOL vl_etat;

	XZST_03EcritureTrace(XZSTC_WARNING, "IN  : MSCO01_init_courbe_temps  va_nb_courbes:%d; va_tempsreel:%d;va_horodeb:%s;va_fenetre:%d;va_periode:%d" ,
										va_nb_courbes, va_tempsreel,va_horodeb,va_fenetre,(int) va_periode);

	if (( va_nb_courbes < 1 ) || ( va_nb_courbes > CM_MAX_COURBES_FT ))
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , " Nombre de courbes incorrect : %d" , va_nb_courbes);
		return XDC_NOK;
	}

	vm_fenetres_ft[va_fenetre-1].no_station = 0;
	vm_fenetres_ft[va_fenetre-1].nb_courbes = va_nb_courbes;
	vm_fenetres_ft[va_fenetre-1].horodeb = vm_HorodateRefInit[va_fenetre-1];
	vm_fenetres_ft[va_fenetre-1].periode = va_periode;
	vm_fenetres_ft[va_fenetre-1].tempsreel = va_tempsreel;
	vm_fenetres_ft[va_fenetre-1].fenetre = va_fenetre;
	vm_fenetres_ft[va_fenetre-1].echelle = va_type_ech;
	for ( i = 0; i < CM_MAX_COURBES_FT ; i++ )
	{
		strcpy( vm_fenetres_ft[va_fenetre-1].varcaract[i].nom_station , pa_varcaract[i].nom_station );
		vm_fenetres_ft[va_fenetre-1].varcaract[i].no_station = pa_varcaract[i].no_station;
		vm_fenetres_ft[va_fenetre-1].varcaract[i].mesure = pa_varcaract[i].mesure;
		vm_fenetres_ft[va_fenetre-1].varcaract[i].voie = pa_varcaract[i].voie;
		vm_fenetres_ft[va_fenetre-1].varcaract[i].sens = pa_varcaract[i].sens;
	}

	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);

	if ( va_tempsreel == XDC_OUI )
	{
		vm_HorodateRef[va_fenetre-1] = vl_HorodateSec;
		vm_HorodateRefInit[va_fenetre-1] = vl_HorodateSec - CM_6HEURES;
	}
	else
	{
		if ( XZSM_13ConversionHorodateSec( va_horodeb , &vm_HorodateRefInit[va_fenetre-1]) != XDC_OK )
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", va_horodeb );
			MSCO01_arret_courbe_ft(va_fenetre);
			return XDC_NOK;
		}

		vm_HorodateRef[va_fenetre-1] = vm_HorodateRefInit[va_fenetre-1] + CM_6HEURES;
	}


	sprintf(vl_titre, "Trafic en Fonction du Temps");
	sprintf(vl_msg, "|%d|%d|%d!%d!%s", CMD_FLASH_INIT_COURBE, CM_COURBE_FT, c_correspFenetre[va_fenetre-1], vm_fenetres_ft[va_fenetre-1].nb_courbes, vl_titre);

	if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	{
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   return XDC_NOK;
	}


	//Lancement recup des donnees
    /*A Abonnement XDG_IDONNEES_FT+vm_NomMachine pour le callback */
    XDG_EncodeDG2( vl_datagroup , XDG_IDONNEES_FT , vm_NomMachine );
    TipcSrvDgGetRecv( vl_datagroup , &vl_etat );
    if ( vl_etat == FALSE )
    {
       if (! XDG_JoindreDG(vl_datagroup) )
          {
          XZST_03EcritureTrace( XZSTC_WARNING, "Abonnement sur %s non effectuee. \n " ,vl_datagroup);
          return XDC_NOK;
          }
    }

    /*A Armement Callback sur Reception XDM_IDONNEES_FT */
    if ((vm_callback_ft[va_fenetre-1] = TipcSrvProcessCbCreate( XDM_IdentMsg( XDM_IDONNEES_FT ),
    															MSCO_Recup_donnees_FT,
                                                                (T_CB_ARG) &(vm_fenetres_ft[va_fenetre-1]))) == NULL)
	{
       XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur %d non declenchee. \n ", XDM_IDONNEES_FT);
       return XDC_NOK;
	}

    /*A Demander les donnees pour la courbe (la variable) de numero indique */
    vm_no_courbe_init_ft[va_fenetre-1] = 0;
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Demande donnees %s : %d_%d_%d_%d",
                            vl_datagroup ,
                            vm_fenetres_ft[va_fenetre-1].no_station ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].sens ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].voie ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].mesure );

    /* Si on est sur PC simplifie et station hors district alors on fait la demande au CI */
    if ( ( vm_TypeMachine != XZSCC_TYPEM_PCS ) ||
         ( MSRA_RADT_AnimeeSurDistrict( vm_fenetres_ft[va_fenetre-1].no_station ,
        		 	 	 	 	 	    vm_fenetres_ft[va_fenetre-1].varcaract[0].sens ) == XDC_OK)  )
       {
       XZAS06_Fonction_De_T( vl_datagroup ,
    		   	   	   	    vm_fenetres_ft[va_fenetre-1].no_station ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].sens ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].voie ,
                            vm_fenetres_ft[va_fenetre-1].horodeb,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].mesure );
       }
    else
       {
       XZST_03EcritureTrace(   XZSTC_INFO, "Demande des donnees %s au CI" , vl_datagroup);
       XZAS06_Fonction_De_T_CI( vl_datagroup ,
                            vm_fenetres_ft[va_fenetre-1].no_station ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].sens ,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].voie ,
                            vm_fenetres_ft[va_fenetre-1].horodeb,
                            vm_fenetres_ft[va_fenetre-1].varcaract[0].mesure );
       }

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO01_init_courbe_temps" );
	return XDC_OK;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Recuperation des donnees pour les courbes a partir de la bd
   pour ft.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSCO_Recup_donnees_FT(
        tm_caracteristique_courbe *pa_caract,
        T_INT2                     va_Existe,
        T_REAL8                    va_HorodateDebut,
        T_INT4                    *pa_valeur,
        T_INT2                    *pa_validite)

/*
* ARGUMENTS EN ENTREE :
*   pa_caract        : Caracteristiques de la courbe
*   va_Existe        : Indicateur de presence des donnees
*   va_HorodateDebut : Horodate de la premiere valeur
*   pa_valeur        : Liste des valeurs des donnees
*   pa_validite      : Liste des valeurs de validite des donnees.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*   Sur message de reception des donnees
*
* FONCTION
*   Stockage et affichage des donnees recues
*
------------------------------------------------------*/
{
int vl_Existe = va_Existe;
double vl_HorodateDebut = va_HorodateDebut;
int *pl_valeur = pa_valeur;
int *pl_validite = pa_validite;
int i=0,j=0;
int vl_delta;
XZSCT_Datagroup vl_datagroup ;
T_BOOL vl_etat;
char vl_titre[100];


XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSCO_Recup_donnees_FT" );

	j=vm_no_courbe_init_ft[pa_caract->fenetre-1];
	vl_delta=0;
	vl_delta = (int) ((vl_HorodateDebut-((tm_caracteristique_courbe *) pa_caract)->horodeb) / ( 60*6));
	/*A Si l'horodate renvoyee est diff que celle envoyee alors on ne fait plus de temps reel et on recale le tableau de donnees*/
	if ( vl_delta != 0 )
	  {
	  /************((tm_caracteristique_courbe *) pa_caract)->tempsreel = XDC_NON;
	  MSCO_Afficher_typetemps(XDC_NON, ((tm_caracteristique_courbe *) pa_caract)->fenetre );  **************/
	  if (vl_delta < 0 )
		 {
		 if ( abs(vl_delta) > XZISC_NB_LIG_MESURES )
			{
			for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
			   {
			   vm_donneesbd[j][i].Valeur = (float)XDC_RADT_CUMUL_INCONNU;
			   vm_donneesbd[j][i].Validite = XDC_RADT_INCONNU;
			   }
			vl_delta = XZISC_NB_LIG_MESURES+1;
			}
		 else
			{
			XZST_03EcritureTrace(XZSTC_WARNING,"remplissage de %d a %d avec inconnu",XZISC_NB_LIG_MESURES-abs(vl_delta),XZISC_NB_LIG_MESURES );
			for (i=XZISC_NB_LIG_MESURES-abs(vl_delta); i < XZISC_NB_LIG_MESURES ; i++ )
			   {
			   vm_donneesbd[j][i].Valeur = (float)XDC_RADT_CUMUL_INCONNU;
			   vm_donneesbd[j][i].Validite = XDC_RADT_INCONNU;
			   }
			}
		 /*A recomposition du tableau de donnees par variable de la courbe */
		 /*A i les donnees sont inexistantes on rempli le tableau avec XDC_RADT_CUMUL_INCONNU et XDC_RADT_INCONNU */
		 if ( vl_Existe == XDC_VRAI )
			{
			for (i=0; i < XZISC_NB_LIG_MESURES-abs(vl_delta) ; i++ )
			   {
			   vm_donneesbd[j][i].Valeur=(float)pl_valeur[i+abs(vl_delta)];
			   vm_donneesbd[j][i].Validite=(XDY_Octet)pl_validite[i+abs(vl_delta)];
			   }
			}
		 else
			{
			for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
			   {
			   vm_donneesbd[j][i].Valeur=XDC_RADT_CUMUL_INCONNU;
			   vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU ;
			   }
			}
		 }
	  else
		 {
		 if ( vl_delta > XZISC_NB_LIG_MESURES )
			vl_delta = XZISC_NB_LIG_MESURES;
		 for (i=0; i < vl_delta; i++ )
			{
			vm_donneesbd[j][i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
			vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU;
			}

		 /*A recomposition du tableau de donnees par variable de la courbe */
		 /*A i les donnees sont inexistantes on rempli le tableau avec XDC_RADT_CUMUL_INCONNU et XDC_RADT_INCONNU */
		 if ( vl_Existe == XDC_VRAI )
			{
			for (i=vl_delta; i < XZISC_NB_LIG_MESURES ; i++ )
			   {
			   vm_donneesbd[j][i].Valeur=(float)pl_valeur[i-vl_delta];
			   vm_donneesbd[j][i].Validite=(XDY_Octet)pl_validite[i-vl_delta];
			   }
			}
		 else
			{
			for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
			   {
			   vm_donneesbd[j][i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
			   vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU ;
			   }
			}
		 } /*fin else du if  (vl_delta < 0 ) */
	  }

	else
	  {
	  /*A recomposition du tableau de donnees par variable de la courbe */
	  /*A i les donnees sont inexistantes on rempli le tableau avec XDC_RADT_CUMUL_INCONNU et XDC_RADT_INCONNU */
	  if ( vl_Existe == XDC_VRAI )
		 {
		 for (i=vl_delta; i < XZISC_NB_LIG_MESURES ; i++ )
			{
			vm_donneesbd[j][i].Valeur=(float)pl_valeur[i-vl_delta];
			vm_donneesbd[j][i].Validite=(XDY_Octet)pl_validite[i-vl_delta];
			}
		 }
	  else
		 {
		 for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
			{
			vm_donneesbd[j][i].Valeur=(float)XDC_RADT_CUMUL_INCONNU;
			vm_donneesbd[j][i].Validite=(XDY_Octet)XDC_RADT_INCONNU ;
			}
		 }
	  }

	vm_no_courbe_init_ft[pa_caract->fenetre-1]++;
	if (vm_no_courbe_init_ft[pa_caract->fenetre-1] < vm_fenetres_ft[pa_caract->fenetre-1].nb_courbes)
	{
		XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Recup_donnees_FT traitement courbe=%d", vm_no_courbe_init_ft[pa_caract->fenetre-1] );

		//Lancement recup des donnees Courble suivante
	    XDG_EncodeDG2( vl_datagroup , XDG_IDONNEES_FT , vm_NomMachine );

	    XZST_03EcritureTrace( XZSTC_DEBUG1, "Demande donnees %s : %d_%d_%d_%d",
	                            vl_datagroup ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].no_station ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].sens ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].voie ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].mesure );

	    /* Si on est sur PC simplifie et station hors district alors on fait la demande au CI */
	    if ( ( vm_TypeMachine != XZSCC_TYPEM_PCS ) ||
	         ( MSRA_RADT_AnimeeSurDistrict( vm_fenetres_ft[pa_caract->fenetre-1].no_station ,
	        		 	 	 	 	 	    vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].sens ) == XDC_OK)  )
	       {
	       XZAS06_Fonction_De_T( vl_datagroup ,
	    		   	   	   	    vm_fenetres_ft[pa_caract->fenetre-1].no_station ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].sens ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].voie ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].horodeb,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].mesure );
	       }
	    else
	       {
	       XZST_03EcritureTrace(   XZSTC_INFO, "Demande des donnees %s au CI" , vl_datagroup);
	       XZAS06_Fonction_De_T_CI( vl_datagroup ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].no_station ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].sens ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].voie ,
	                            vm_fenetres_ft[pa_caract->fenetre-1].horodeb,
	                            vm_fenetres_ft[pa_caract->fenetre-1].varcaract[vm_no_courbe_init_ft[pa_caract->fenetre-1]].mesure );
	       }

	}
	else
	{
		XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Recup_donnees_FT Fin traitement recup");
		if ( vm_callback_ft[pa_caract->fenetre-1] != NULL )
		  {
		  /*A Desarmer le callback en place */
		  if ( TutCbDestroy( vm_callback_ft[pa_caract->fenetre-1] ) == FALSE )
			 {
			 XZST_03EcritureTrace( XZSTC_WARNING, "Desarmement de la Callback sur %d non effectuee ",
									  XDM_IDONNEES_FT);
			 return XDC_NOK;
			 }
		  else
			 {
			  vm_callback_ft[pa_caract->fenetre-1] = NULL;
			 }
		  }
		}

		// envoi des donn��es:
//		MSCO_Afficher_les_donnees_recuperees(pa_caract->fenetre-1, vm_no_courbe_init_ft[pa_caract->fenetre-1]);
		MSCO_Afficher_les_donnees_recuperees(pa_caract->fenetre-1);

    /*A Init des valeurs du graph H+1 */
//		if ( MSCO_Init_Courbe_H1(pa_caract->fenetre) == XDC_NOK )
//       {
//       XZST_03EcritureTrace( XZSTC_WARNING, "Probleme lors de l'init du graphe H+1");
//       return XDC_NOK;
//       }

    /*A On se branche sur le temps reel */
    if (pa_caract->tempsreel == XDC_OUI )
       {
//       MSCO_Abt( pa_caract ,XZEAC_DEBUT_ABONNEMENT );
//       MSCO_Lancement_raf_courbe(pa_caract->fenetre);
       }



	return;
	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_Recup_donnees_FT" );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Afficher les donnees recuperees de la base et bufferisees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Afficher_les_donnees_recuperees(va_fenetre)

						XDY_Octet   va_fenetre;
//						int         va_no_courbe;
/*
* ARGUMENTS EN ENTREE :
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
*   Creation de la table de hash
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_ft_bd.c	1.2 09/24/07 : MSCO_Afficher_les_donnees_recuperees " ;
 int i=0;
 int j=0;
 int k=0;
  char vl_nom_var[256];
  float vl_res=0.0;
   float *pl_valeur;
   XZSMT_Horodate vl_horodate;
   XDY_Horodate vl_horodatesec=0;
   char *pl_valeur_texte;


   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  :MSCO_Afficher_les_donnees_recuperees " );

   for (i=0; i < XZISC_NB_LIG_MESURES ; i++ )
      {
      for ( j=0; j < CM_MAX_COURBES_FT ; j++ )
         {
         sprintf( vl_nom_var , "%d!%d!%d!%d" , vm_fenetres_ft[va_fenetre].varcaract[j].no_station ,
        		 	 	 	 	 	 	 	   vm_fenetres_ft[va_fenetre].varcaract[j].sens ,
        		 	 	 	 	 	 	 	   vm_fenetres_ft[va_fenetre].varcaract[j].voie ,
        		 	 	 	 	 	 	 	   vm_fenetres_ft[va_fenetre].varcaract[j].mesure );

         if ( (vm_donneesbd[j][i].Validite == XDC_RADT_CORRECT) ||
              (vm_donneesbd[j][i].Validite == XDC_RADT_RECONST) )
            {
            sprintf( vl_nom_var , "%s!%f" , vl_nom_var, (float)vm_donneesbd[j][i].Valeur );
            }
         else
            {
            sprintf( vl_nom_var , "%s!%f" , vl_nom_var, (float)-1 );
            }
         sprintf(vl_msg, "%d|%d|%s", CMD_FLASH_DONNEES_COURBE, CM_COURBE_FT, vl_nom_var);
		 if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
		 {
		    XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
		    return ;
		 }

         if ( i >= XZISC_NB_LIG_MESURES-CM_UNE_HEURE_DE_DONNEES )
            {
//        	 MSCO_Calcul_Donnees_H1( (float)vm_donneesbd[j][i].Valeur ,
//                                    (float)vm_donneesbd[j][i+CM_54_MINUTES_DE_DONNEES].Valeur ,
//                                    &vl_res );
            vm_donneesbd[j][i+CM_UNE_HEURE_DE_DONNEES].Valeur=(float)vl_res;
            }
         }

      }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :MSCO_Afficher_les_donnees_recuperees  " );
   return ;
}
#endif
