/*E*/
/*Fichier : $Id: isbi_sco_tdp.c,v 1.4 2020/11/03 17:49:17 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 17:49:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSCO * FICHIER isyn_sco_tdp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permettant l'affichage de graphe temps de parcours au sein du synoptique
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	06 Dec 1996	: Creation 
* PNI	25/09/2013	: Ajout de la date dans le titre 1.3 DEM1085
* JMG	08/06/20: EMS 1.4
* GR    10/07/2020 : EMS 1.5
------------------------------------------------------*/ 

/* fichiers inclus */

#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"


/* definitions de constantes */
#define CM_SEUIL_MAX 32767
#define CM_BUFF_SOCK 600
#define NB_ZONES_TDP_MAX 10

/* declaration de fonctions internes */
//int MSCO_arret_courbe_tdp();
void MSCO_Recup_donnees_tdp();
void MSCO_Recup_donnees_tdp_mz();
void MSCO_Recep_tdp_init();
void MSCO_Recep_tdp();
void envoyerDonnees();

XDY_Horodate vm_horodate_TDP;

extern char c_correspFenetre[];




static XDY_Horodate vm_Horodate;

static XDY_Horodate vm_HorodateDeb;
static XDY_Horodate vm_HorodateFin;

static float  vm_duree=24.0;


static tm_varcaractdp vm_courbes_tdp[NB_ZONES_TDP_MAX];

static XDY_EMes_TdP_6mn vm_donnees_mz[NB_ZONES_TDP_MAX];
static XDY_Horodate vm_Horodate_Courant;
static int vm_nb_donnees_recues = 0;
static int vm_temps_reel=0;
static T_CB vm_callback_tdp;
int vm_multizone=XDC_NON;
int vm_no_courbe_init = 0;
int vm_nb_zones_tdp;

int vm_seuil_bas;
int vm_seuil_haut;
int vm_seuil_max;

static XDY_Octet vm_type_courbe=0;

char vl_msg[CM_BUFF_SOCK];


/*X*/ 
/*------------------------------------------------------
* SERVICE RENDU : 
*   Initialisation de la courbe tdp
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO04_init_courbe_tdp(
                    va_no_tdp)
                    
                    XDY_Eqt va_no_tdp;
		    

/*
* ARGUMENTS EN ENTREE :
*  va_no_eqt : Numero de la zone tdp
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
{static char *version = "@(#)isyn_sco_tdp.c	1.10 09/17/07 : MSCO04_init_courbe_tdp " ;

   int code_retour=0;
   XZSCT_Datagroup vl_datagroup_init ;
   char vl_nom_var[256];
   char vl_nom[40];
   char vl_titre[256];
   int i=0,j=0;
   
   int    vl_JourSemaine;
   XDY_Horodate vl_HorodateSec;
   XDY_Horodate vl_HorodateDeb;
   
   T_BOOL vl_etat;

   XDY_Horodate vl_HorodateFin=0;
   XDY_Octet     vl_duree=24;
   XZSMT_Horodate vl_HorodateInv,vl_Horodate;
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, 
   "IN  : MSCO04_init_courbe_tdp: va_no_tdp:%d", va_no_tdp);
   
   
   if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                        (int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
      return XDC_NOK;
      }
   
   if ( XZSM_11ConversionHorodate( vl_HorodateSec , &vl_HorodateInv) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %d en chaine", vl_HorodateSec );
      return XDC_NOK;
      }
      
   vl_HorodateInv[10]='\000';

   sprintf( vl_Horodate ,"%s %s",vl_HorodateInv,"00:00:00");

   if ( XZSM_13ConversionHorodateSec( vl_Horodate , &vl_HorodateDeb) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", vl_Horodate );
      return XDC_NOK;
      }
   
   vm_HorodateFin = vl_HorodateDeb + 24*3600;
   vm_HorodateDeb = vl_HorodateDeb;
   vm_Horodate = vm_HorodateDeb;
   vm_duree = (float) 24;
   
    vm_type_courbe = CM_COURBE_TDP;
    vm_multizone = XDC_NON;
    vm_nb_zones_tdp = 1;
    vm_courbes_tdp[0].no_zone = va_no_tdp;
    strcpy(vm_courbes_tdp[0].nom_zone, "");
   
   
   XDG_EncodeDG2(vl_datagroup_init , XDG_IDONNEES_TDP , vm_NomMachine );
   XZST_03EcritureTrace( XZSTC_DEBUG1,
			   "Abonnement sur %s :\n " ,vl_datagroup_init);

   /*A Abonnement XDG_IDONNEES_TDP+vm_NomMachine pour le callback si ce n'est deja fait */
   TipcSrvDgGetRecv( vl_datagroup_init , &vl_etat );
   if ( vl_etat == FALSE )
       {
       if (! XDG_JoindreDG(vl_datagroup_init) )
          {	
          /* Ecriture Trace */
          XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_datagroup_init);
          return XDC_NOK;
          }
       }
   /*A  Armement Callback sur Reception XDM_IDONNEES_TDP */
   if ( (vm_callback_tdp = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IDONNEES_TDP ),
	   			   MSCO_Recup_donnees_tdp,(T_CB_ARG) NULL )) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				      "Callback sur XDM_IDONNEES_tdp_TRA non declenchee. %d\n " , vm_callback_tdp );
      return XDC_NOK;
      }	
    /*A Appel a XZAS... pour demander la recuperation des donnees des courbes */
	if ( XZAS13_Init_TDP( vl_datagroup_init , va_no_tdp , vl_HorodateDeb ) != XDC_OK )
	 {
	  /* Ecriture Trace */
	  XZST_03EcritureTrace( XZSTC_WARNING,
				  "Probleme a l'appel de XZAS13_Init_TDP");
	  return XDC_NOK;
	  }

	/*A Abonnement au donnees TDP */
       XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_TDP , vm_NomMachine );
       XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XXZEA35_Abt_TdP_6mn_Zone " );
       XZEA35_Abt_TdP_6mn_Zone( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSCO_Recep_tdp_init ,  va_no_tdp , &code_retour );
       XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA35_Abt_TdP_6mn_Zone %d " , code_retour );
	

        XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO04_init_courbe_tdp" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe tdp
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
MSCO04_init_courbe_tdp_mz(
				    va_chaine)

                    char *va_chaine;

/*
* ARGUMENTS EN ENTREE :
*  va_chaine : chaine des zones tdp
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
{static char *version = "@(#)isyn_sco_tdp.c	1.10 09/17/07 : MSCO04_init_courbe_tdp_mz " ;

   int code_retour=0;
   int i;
   XZSCT_Datagroup vl_datagroup_init ;
   int    vl_JourSemaine;
   XDY_Horodate vl_HorodateSec;
   XZSMT_Horodate vl_HorodateInv,vl_Horodate;
   XDY_Horodate vl_HorodateDeb;
   char *vl_tampon;
   char pl_chaine[255];
   T_BOOL vl_etat;
   char vl_titre[255];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO04_init_courbe_tdp_mz va_chaine=%s", va_chaine );

   /* Reinit des seuils cumules */
   vm_seuil_bas = 0;
   vm_seuil_haut = 0;
   vm_seuil_max = 0;

   /*A Lecture de l'heure courante */
   if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                        (int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
      return XDC_NOK;
      }

   if ( XZSM_11ConversionHorodate( vl_HorodateSec , &vl_HorodateInv) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %d en chaine", vl_HorodateSec );
      return XDC_NOK;
      }

   vl_HorodateInv[10]='\000';

/*   sprintf( vl_Horodate ,"%s %s",vl_HorodateInv,"00:00:00");*/
   XZST_03EcritureTrace(   XZSTC_INFO , "HORODATE = %s", vl_Horodate );

   if ( XZSM_13ConversionHorodateSec( vl_Horodate , &vl_HorodateDeb) != XDC_OK )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", vl_Horodate );
      return XDC_NOK;
      }

   vm_type_courbe = CM_COURBE_TDP;
   vm_multizone = XDC_OUI;

   /*init du tableau des courbes a partir de la chaine*/
   strcpy(pl_chaine, va_chaine);
   vl_tampon = strtok( pl_chaine , "|" );
   vm_nb_zones_tdp = atoi(vl_tampon);

   vl_tampon = strtok(NULL, "|");
   XZST_03EcritureTrace(   XZSTC_WARNING , "horodate courbe %s",vl_tampon);
   strcpy(vl_Horodate,vl_tampon);
   if (!strcmp(vl_Horodate,"01/01/1900 00:00:00")) {
  	vm_temps_reel=1; 
   	sprintf( vl_Horodate ,"%s %s",vl_HorodateInv,"00:00:00");
   }
   else {
      vm_temps_reel=0;
      strcpy(vl_HorodateInv, vl_tampon);
      vl_HorodateInv[10]='\000';
   }
       XZST_03EcritureTrace(   XZSTC_WARNING , "REEL %d, horo %s",vm_temps_reel,vl_Horodate);
       if ( XZSM_13ConversionHorodateSec( vl_Horodate , &vl_HorodateDeb) != XDC_OK )
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", vl_Horodate );
	     return XDC_NOK;
	}
	vm_horodate_TDP = vl_HorodateDeb;

      vm_HorodateFin = vl_HorodateDeb + 24*3600;
   vm_HorodateDeb = vl_HorodateDeb;
   vm_duree = (float) 24;
   vm_Horodate = vm_HorodateDeb;
   vl_Horodate[10]='\0';
   XZST_03EcritureTrace(   XZSTC_WARNING, "MSCO04_init_courbe_tdp_mz nb_zones=%d", vm_nb_zones_tdp );
   for(i=0; i < vm_nb_zones_tdp; i++)
   {
	   vl_tampon = strtok(NULL, "|");
	   vm_courbes_tdp[i].no_zone = atoi(vl_tampon);
	   vl_tampon = strtok(NULL, "|");
	   XZST_03EcritureTrace(   XZSTC_WARNING , "NO ZONE %d",vm_courbes_tdp[i].no_zone);
	   strcpy(vm_courbes_tdp[i].nom_zone, vl_tampon);
	   XZST_03EcritureTrace(   XZSTC_WARNING , "nom_zone %s",vm_courbes_tdp[i].nom_zone);
   }
   XZST_03EcritureTrace(   XZSTC_WARNING, "MSCO04_init_courbe_tdp_mz nb_zones=%d", vm_nb_zones_tdp );

   sprintf( vl_titre,"Graphe TDP multi-zones 6mn: %s -> %s (%s)", vm_courbes_tdp[0].nom_zone, vm_courbes_tdp[vm_nb_zones_tdp-1].nom_zone, vl_Horodate);
   sprintf(vl_msg, "|%d|%d|%c|%d!%s!%f!%f", CMD_FLASH_INIT_COURBE, CM_COURBE_TDP_MZ,c_correspFenetre[CM_FENETRE_LARGE] ,vm_nb_zones_tdp ,vl_titre, vm_HorodateDeb, vm_HorodateFin);

XZST_03EcritureTrace(   XZSTC_WARNING, "%s",vl_msg);
   vm_nb_courbes = vm_nb_zones_tdp;

   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
   {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
       return ;
   }

   //Armement CB Recup
   /* ==> Mettre Abonnement + callback de sco_tdp */
   XDG_EncodeDG2(vl_datagroup_init , XDG_IDONNEES_TDP , vm_NomMachine );

   /*A Abonnement XDG_IDONNEES_TDP+vm_NomMachine pour le callback si ce n'est deja fait */
   TipcSrvDgGetRecv( vl_datagroup_init , &vl_etat );
   if ( vl_etat == FALSE )
   {
      if (! XDG_JoindreDG(vl_datagroup_init) )
      {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
		   "Abonnement sur %s non effectuee. \n " ,vl_datagroup_init);
         return XDC_NOK;
      }
    }
    /*A  Armement Callback sur Reception XDM_IDONNEES_TDP */
    vm_no_courbe_init = 0;
    XZST_03EcritureTrace(   XZSTC_WARNING, "MSCO04_init_courbe_tdp_mz traitement courbe=%d", vm_no_courbe_init );
    XZST_03EcritureTrace(   XZSTC_WARNING, ">> pose CB");
    if ( (vm_callback_tdp = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IDONNEES_TDP ),
 			   MSCO_Recup_donnees_tdp_mz,(T_CB_ARG) NULL )) == NULL)
    {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
			      "Callback sur %u non declenchee. \n " , XDM_IdentMsg(XDM_IDONNEES_TDP));
      return XDC_NOK;
    }



    XZST_03EcritureTrace( XZSTC_WARNING, "appel XZAS13 pour %d",vm_courbes_tdp[vm_no_courbe_init].no_zone);
    if ( XZAS13_Init_TDP( vl_datagroup_init , vm_courbes_tdp[vm_no_courbe_init].no_zone , vm_HorodateDeb ) != XDC_OK )
 	{
  		XZST_03EcritureTrace( XZSTC_WARNING,"Probleme a l'appel de XZAS13_Init_TDP");
  		return XDC_NOK;
  	}

   // La fin de l'init se fait dans MSCO_Recup_donnees_tdp_mz
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO04_init_courbe_tdp_mz" );
return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 MSCO04_arret_courbe_tdp()


/*
* ARGUMENTS EN ENTREE :
* va_fenetre : fenetre de visu de la courbe
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
{static char *version = "@(#)isyn_sco_tdp.c	1.10 09/17/07 :  MSCO_arret_courbe_tdp " ;

   int code_retour=0;
   int i;
   XZSCT_Datagroup vl_datagroup_init ;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :  MSCO_arret_courbe_tdp" );

   /*A  DesArmement Callback sur Reception  */
   if (vm_callback_tdp  != NULL )
      if ( TutCbDestroy( vm_callback_tdp  ) == FALSE )
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
	    			   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_FT);
         return XDC_NOK;
         }

   if (vm_multizone == XDC_OUI)
   {
	   for (i=0; i<vm_nb_zones_tdp; i++)
	   {
		   XZEA35_Abt_TdP_6mn_Zone( XZEAC_FERMER_ABONNEMENT , vl_datagroup_init , MSCO_Recep_tdp ,  vm_courbes_tdp[i].no_zone , &code_retour );
	   }
	   sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_ARRET_COURBE, CM_COURBE_TDP_MZ, c_correspFenetre[CM_FENETRE_LARGE]);
	   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	       {
	       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	       return ;
	       }

	   vm_nb_zones_tdp = 0;
   }
   else
   {
	   if (vm_nb_zones_tdp > 0)
	   {
		   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XXZEA35_Abt_TdP_6mn_Zone " );
	//	   XZEA35_Abt_TdP_6mn_Zone( XZEAC_FERMER_ABONNEMENT , vl_datagroup_init , MSCO_Recep_tdp ,  vm_no_tdp , &code_retour );
		   XZEA35_Abt_TdP_6mn_Zone( XZEAC_FERMER_ABONNEMENT , vl_datagroup_init , MSCO_Recep_tdp_init ,  vm_courbes_tdp[0].no_zone , &code_retour );
		   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA35_Abt_TdP_6mn_Zone %d " , code_retour );

		   sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_ARRET_COURBE, CM_COURBE_TDP, c_correspFenetre[CM_FENETRE_LARGE]);
		   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
		       {
		       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
		       return ;
		       }
		   vm_nb_zones_tdp = 0;
	   }
   }

   vm_type_courbe = CM_COURBE_AUCUNE;
   vm_multizone = XDC_NON;
   vm_nb_zones_tdp = 0;
   vm_callback_tdp = NULL;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :  MSCO_arret_courbe_tdp" );
   return XDC_OK;
}

void MSCO_Recup_donnees_tdp(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*  pa_caract : Caracteristiques de la courbe.
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
{static char *version = "@(#)isyn_sco_tdp.c	1.10 09/17/07 : MSCO_Recup_donnees_tdp " ;
 T_INT4 vl_prdeb;
 T_INT4 vl_prfin;
 T_INT2 vl_sens;
#ifndef _TIBCO_EMS
 T_INT2 *pl_tdp;

#ifndef _TIBCO_EMS 
 T_STR vl_Autoroute;
#else 
 XDY_NomAuto vl_Autoroute;
#endif

#else
 T_INT2 pl_tdp[255];
 XDY_Nom vl_Autoroute;
#endif
 T_INT2 vl_seuil_max,vl_seuil_haut,vl_seuil_bas;
 T_INT4  vl_nb_Donnees;
 int i=0,k=0;
 char vl_titre[255];
 XDY_EMes_TdP_6mn     vl_donnees;
 XDY_Horodate vl_Horodate;
 XZSMT_Horodate vl_HorodateInv;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recup_donnees_tdp" );
   
   /*A RECUPERE LES DONNEES et bufferiser*/
   /* Recuperation des donnees du message */
   if (!TipcMsgRead(pa_Data->msg
			,T_IPC_FT_STR,&vl_Autoroute
			,T_IPC_FT_INT2,&vl_sens
			,T_IPC_FT_INT4,&vl_prdeb
			,T_IPC_FT_INT4,&vl_prfin
			,T_IPC_FT_INT2,&vl_seuil_max
			,T_IPC_FT_INT2,&vl_seuil_bas
			,T_IPC_FT_INT2,&vl_seuil_haut
			,T_IPC_FT_INT2_ARRAY, &pl_tdp , &vl_nb_Donnees
			, NULL) )
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_tdp: Impossible de recuperer le message");
      return;
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"MSCO_Recup_donnees_tdp: Recuperation  message: \n autoroute %s\n sens %d\n vl_prdeb %d\n vl_prfin %d\n vl_seuil_max %d\n vl_seuil_haut %d\n vl_seuil_bas %d",vl_Autoroute
			,vl_sens
			,vl_prdeb
			,vl_prfin
			,vl_seuil_max
			,vl_seuil_haut
			,vl_seuil_bas);
      }
      
   /*A  DesArmement Callback sur Reception  */
   if ( TutCbDestroy( vm_callback_tdp ) == FALSE ) 
      {	
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Desarmement de la Callback sur %u non effectuee. \n " ,XDM_IdentMsg(XDM_IDONNEES_TDP));
      return;
      }	
   vm_callback_tdp=NULL;


     if ( XZSM_11ConversionHorodate( vm_HorodateDeb , &vl_HorodateInv) != XDC_OK )
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en chaine ", vm_HorodateDeb  );
	     return XDC_NOK;
	}
   vl_HorodateInv[10]='\000';


   if ( vl_Autoroute == NULL ) 
    sprintf( vl_titre,"    Graphe Temps de Parcours 6mn (%s)", vl_HorodateInv);
  else
    sprintf( vl_titre,"    Graphe Temps de Parcours 6mn %s %d,%d,sens %d (%s)",vl_Autoroute,vl_prdeb,vl_prfin,vl_sens,   vl_HorodateInv);

   sprintf(vl_msg, "|%d|%d|%c|%d!%s!%f!%f!%d!%d!%d", CMD_FLASH_INIT_COURBE, CM_COURBE_TDP, c_correspFenetre[CM_FENETRE_LARGE], vm_courbes_tdp[0].no_zone, vl_titre, vm_HorodateDeb, vm_HorodateFin, vl_seuil_bas, vl_seuil_haut, vl_seuil_max);

   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
       {
       XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
       return ;
       }

	 //Horodate de ref arrondi au 6 minutes precedente
	 vl_Horodate = ((int) (vm_Horodate/(6*60)))*6*60;
	 vl_Horodate = vm_HorodateDeb;
	 // Calcul de H-6h
	/* vl_Horodate = vl_Horodate-6*60*60; // Horodate de Ref - 6 * 60 minutes*/
/* Envoi des donnees recues */
	 for (i=0; i < vl_nb_Donnees ; i++ )
	 {
	   vl_donnees.Dispo =  XDC_VRAI;
	   vl_donnees.Horodate = vl_Horodate + i*6*60;
	   vl_donnees.Temps = (XDY_TDP)pl_tdp[i];
	   vl_donnees.Numero = vm_courbes_tdp[0].no_zone;
	   MSCO_Recep_tdp_init(vl_donnees);
	 }

	 MSCO00_Signaler_Fin_Init(CM_FENETRE_LARGE, CM_COURBE_TDP);
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_tdp" );
   return ;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Recuperation des donnees pour les courbes a partir de la bd
   pour tdp multi-zones.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Recup_donnees_tdp_mz(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_vide)

/*
* ARGUMENTS EN ENTREE :
*  Arguments d'une callback
*  pa_caractdp : Caracteristiques de la courbe: no_zone
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
{static char *version = "@(#)isyn_sco_tdp_bd.c	1.14 12/18/96 : MSCO_Recup_donnees_TDP " ;
 T_INT4 vl_prdeb;
 T_INT4 vl_prfin;
 T_INT2 vl_sens;
#ifndef _TIBCO_EMS
 T_INT2 *pl_tdp;
 T_STR vl_Autoroute;
#else
 T_INT2 pl_tdp[255];
 XDY_Nom vl_Autoroute;
#endif
 T_INT2 vl_seuil_max,vl_seuil_haut,vl_seuil_bas;
 T_INT4  vl_nb_Donnees;
 int i=0;
 T_BOOL vl_etat=0;
 char vl_titre[255];
 XZSCT_Datagroup vl_datagroup_init ;
 int code_retour=XDC_OK;
 char vl_chaine[10];
 XDY_EMes_TdP_6mn vl_donnees;
 XDY_Horodate vl_Horodate;

 XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recup_donnees_TDP_mz" );

 XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Recup_donnees_TDP_mz : CB declenche courbe=%d", vm_no_courbe_init);

 /*A RECUPERE LES DONNEES et bufferiser*/
 /* Recuperation des donnees du message */
 if (!TipcMsgRead(pa_Data->msg
                        ,T_IPC_FT_STR,&vl_Autoroute
                        ,T_IPC_FT_INT2,&vl_sens
                        ,T_IPC_FT_INT4,&vl_prdeb
                        ,T_IPC_FT_INT4,&vl_prfin
                        ,T_IPC_FT_INT2,&vl_seuil_max
                        ,T_IPC_FT_INT2,&vl_seuil_bas
                        ,T_IPC_FT_INT2,&vl_seuil_haut
                        ,T_IPC_FT_INT2_ARRAY, &pl_tdp , &vl_nb_Donnees
                        , NULL) )
  {
  XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_tdp_mz: Impossible de recuperer le message");
  return;
  }
  else
  {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_tdp_mz: Recuperation  message: \n autoroute %s\n sens %d\n vl_prdeb %d\n vl_prfin %d\n vl_seuil_max %d\n vl_seuil_haut %d\n vl_seuil_bas %d\n vl_nb_Donnees %d ",vl_Autoroute
                        ,vl_sens
                        ,vl_prdeb
                        ,vl_prfin
                        ,vl_seuil_max
                        ,vl_seuil_haut
                        ,vl_seuil_bas
                        ,vl_nb_Donnees);
  }

 if ( vl_Autoroute == NULL )
	 sprintf( vl_titre,"    Graphe Temps de Parcours 6mn ");
 else
	 sprintf( vl_titre,"    Graphe Temps de Parcours 6mn %s %d,%d,sens %d",vl_Autoroute,vl_prdeb,vl_prfin,vl_sens);

 sprintf(vl_msg, "|%d|%d|%c|%d!%s!%f!%f!%d!%d!%d", CMD_FLASH_INIT_COURBE, CM_COURBE_TDP, c_correspFenetre[CM_FENETRE_LARGE], vm_courbes_tdp[vm_no_courbe_init].no_zone ,vl_titre, vm_HorodateDeb, vm_HorodateFin, vl_seuil_bas, vl_seuil_haut, vl_seuil_max);

 if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
 {
     XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
     return ;
 }


 	 //Horodate de ref arrondi au 6 minutes precedente
 	 vl_Horodate = ((int) (vm_Horodate/(6*60)))*6*60;
	 vl_Horodate = vm_HorodateDeb;
 /* Envoi des donnees recues */
	 for (i=0; i < vl_nb_Donnees ; i++ )
	 {
	   vl_donnees.Dispo =  XDC_VRAI;
	   vl_donnees.Horodate = vl_Horodate + i*6*60;
	   vl_donnees.Temps = (XDY_TDP)pl_tdp[i];
	   vl_donnees.Numero = vm_courbes_tdp[vm_no_courbe_init].no_zone;
	   MSCO_Recep_tdp_init(vl_donnees);
	 }

  vm_seuil_max = vl_seuil_max;
  vm_seuil_bas = vm_seuil_bas + vl_seuil_bas;
  vm_seuil_haut = vm_seuil_haut + vl_seuil_haut;

  if (vm_multizone == XDC_OUI)
  {
	  vm_no_courbe_init++;
	  if (vm_no_courbe_init < vm_nb_zones_tdp)
	  {
		  XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Recup_donnees_TDP_mz traitement courbe=%d", vm_no_courbe_init );
		   //Armement CB Recup
		   /* ==> Mettre Abonnement + callback de sco_tdp */
		   XDG_EncodeDG2(vl_datagroup_init , XDG_IDONNEES_TDP , vm_NomMachine );

		    XZST_03EcritureTrace(   XZSTC_DEBUG1, ">> init TDP");
    XZST_03EcritureTrace( XZSTC_WARNING, "appel XZAS13 pour %d",vm_courbes_tdp[vm_no_courbe_init].no_zone);
		    if ( XZAS13_Init_TDP( vl_datagroup_init , vm_courbes_tdp[vm_no_courbe_init].no_zone , vm_Horodate ) != XDC_OK )
		 	{
		  		XZST_03EcritureTrace( XZSTC_WARNING,"Probleme a l'appel de XZAS13_Init_TDP");
		  		return ;
		  	}
	  }
	  else
	  {/* Fin de la periode d'init*/
		  XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Recup_donnees_TDP_mz Fin Recup des donnees TDP MZ" );

		  XZST_03EcritureTrace(   XZSTC_DEBUG1, ">> Desarm CB" );
		  /*A  DesArmement Callback sur Reception  */
		   if ( vm_callback_tdp != NULL )
		   {
		      if ( TutCbDestroy( vm_callback_tdp ) == FALSE )
		      {
		         /* Ecriture Trace */
		         XZST_03EcritureTrace( XZSTC_WARNING,
						   "Desarmement de la Callback sur %u non effectuee. \n " ,XDM_IDONNEES_TDP);
		         return ;
		      }
		      else
		         vm_callback_tdp = NULL;
		   }

		   MSCO00_Signaler_Fin_Init(CM_FENETRE_LARGE, CM_COURBE_TDP_MZ);

// init des donnees de traitement 6min
		   vm_Horodate_Courant = 0;
		   vm_nb_donnees_recues = 0;

		   XZST_03EcritureTrace(   XZSTC_DEBUG1, ">> ABO TDP" );
		   /* Abonnement aux TDP */
		   for ( i = 0; i < vm_nb_zones_tdp; i++)
		   {
			   // init des donnees de traitement 6min
			   vm_donnees_mz[i].Horodate = 0;
			   vm_donnees_mz[i].Numero = vm_courbes_tdp[i].no_zone;

			   /*A Abonnement XZEA35_Abt_TdP_6mn_Zone  */
		      sprintf (vl_chaine, "%d" ,  vm_courbes_tdp[i].no_zone);
		      XDG_EncodeDG3(vl_datagroup_init , XDG_IABTINIT_TDP , vl_chaine  , vm_NomMachine );
			if (vm_temps_reel==1) {
		      XZEA35_Abt_TdP_6mn_Zone(XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSCO_Recep_tdp, vm_courbes_tdp[i].no_zone , &code_retour );
		      XZST_03EcritureTrace(   XZSTC_FONCTION , "XZEAC_DEBUT_ABONNEMENT pour la zone %d", vm_courbes_tdp[i].no_zone);
  			}
		   }
		   for (; i<NB_ZONES_TDP_MAX; i++)
			   vm_donnees_mz[i].Numero = -1;

	  }
  }

  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_TDP" );
   return ;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception des donn���es temps reel du tdp
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  MSCO_Recep_tdp_init(XDY_EMes_TdP_6mn     va_donnees )

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
{static char *version = "@(#)isyn_sco_tdp.c	1.10 09/17/07 : MSCO_Recep_tdp" ;
   XZSMT_Horodate vl_horodate;
   int temps;


    XZSM_11ConversionHorodate( va_donnees.Horodate, &vl_horodate);


   if ( va_donnees.Dispo == XDC_VRAI )
	   temps=(int)va_donnees.Temps;
   else
	   temps=0;

   sprintf(vl_msg, "|%d|%d|%c|%d!%f!%d", CMD_FLASH_DONNEES_COURBE, CM_COURBE_TDP, c_correspFenetre[CM_FENETRE_LARGE] , va_donnees.Numero ,va_donnees.Horodate,temps);

      if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
          {
          XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
          return ;
          }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_Recep_tdp_init" );

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception des donn���es temps reel du tdp
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  MSCO_Recep_tdp(XDY_EMes_TdP_6mn     va_donnees )

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
{static char *version = "@(#)isyn_sco_tdp.c	1.10 09/17/07 : MSCO_Recep_tdp" ;
   XZSMT_Horodate vl_horodate;
   int temps;
   int i, index;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recep_tdp va_donnees:%d", va_donnees.Temps );

   if(vm_Horodate_Courant == 0)
	   vm_Horodate_Courant = va_donnees.Horodate;
   else if (vm_Horodate_Courant != va_donnees.Horodate)
   {/* nouvelle horodate recue => il faut envoyer les donn��es*/
	   envoyerDonnees();
	   vm_nb_donnees_recues = 0;
   }

   /*rech de la station*/
   index = -1;
   for (i=0; i< NB_ZONES_TDP_MAX; i++){
	   if (vm_donnees_mz[i].Numero == va_donnees.Numero){
		   index = i;
		   break;
	   }
   }

   if (index == -1){
	   XZST_03EcritureTrace(   XZSTC_WARNING, "MSCO_Recep_tdp - Numero de ZDP non attendu : %d", va_donnees.Numero);
	   return;
   }

   if ( va_donnees.Dispo == XDC_VRAI )
	   vm_donnees_mz[index].Temps = va_donnees.Temps;
   else
	   vm_donnees_mz[index].Temps = 0;
   vm_donnees_mz[index].Horodate = va_donnees.Horodate;
   vm_donnees_mz[index].Horodate = ((int) (vm_donnees_mz[index].Horodate/(6*60)))*6*60;
   vm_nb_donnees_recues ++;

   if (vm_nb_donnees_recues == vm_nb_courbes)
   {/* nb donnees attendues atteint => envoi des donn��es */
	   envoyerDonnees();
	   vm_nb_donnees_recues = 0;
	 MSCO00_Signaler_Fin_Init(CM_FENETRE_LARGE, CM_COURBE_TDP);
   }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_Recep_tdp" );

}


void envoyerDonnees()
{
int i;
	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN   : envoyerDonnees" );

	/*sprintf(vl_msg, "|%d|%d|%c|%f", CMD_FLASH_DONNEES_COURBE, CM_COURBE_TDP_MZ, c_correspFenetre[CM_FENETRE_LARGE], vm_Horodate_Courant);*/
	for (i=0; i<vm_nb_courbes; i++)
	{
	   sprintf(vl_msg, "%s!%d!%d", vl_msg, vm_donnees_mz[i].Numero, vm_donnees_mz[i].Temps);
   	sprintf(vl_msg, "|%d|%d|%c|%d!%f!%d", CMD_FLASH_DONNEES_COURBE, CM_COURBE_TDP, c_correspFenetre[CM_FENETRE_LARGE] , 
				vm_donnees_mz[i].Numero ,vm_donnees_mz[i].Horodate, vm_donnees_mz[i].Temps);
	
	send_message_courbe(vl_msg, CM_BUFF_SOCK);

	   /*Reinit des donnees*/
	   vm_donnees_mz[i].Temps = 0;
	}

	/*Reinit des donnees*/
	vm_Horodate_Courant = 0;


	/*
	if (vm_nb_courbes > 0){
		if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
			return ;
		}
	}*/

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT   : envoyerDonnees" );
}

