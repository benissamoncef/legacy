/*E*/
/* Fichier : @(#)etdp_don.c	1.13        Release : 1.13        Date : 02/14/12
----------------------------------------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR 
----------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
----------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_don.c
----------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de definition de donnees du systeme temps de parcours
*
----------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.3	06 Mai 1997	: Affichage TDP sur PMV  suite (DEM/1417)
* Mismer.D	version 1.4	05 Jun 1997	: Ajout alerte sur panne zone (DEM/1447)
* Mismer.D	version 1.5	23 Jun 1997	: Init timer MB au 6mn rondes + 30 sec (DEM/1456)
* Mismer.D      version 1.6     16 Oct 1997     : Modif pour affichage minutes pour TDP suite (DEM/1505)
* JMG		version 1.10	26/06/10	: linux DEM/934
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK ou NOK pour plusieurs fonctions  1.11
* JPL		07/03/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=)  1.12
* JMG		13/01/12	: tdp etendu DEM 1014
* PNI		19/12/12 : dans et_Stockage_Mesures_horaire agrandissement de pl_Ligne a 1250 DEM1059 v1.14
* ABE	   26/07/21 : TDP FCD Hybride sur trajets hors réseau, ajout fonction calcul si dest est hors reseau DEM-SAE318 1.15
* ABE	   26/07/21 : TDP FCD Hybride fix limite PR autoroute A8 de 15200 vers 17900 DEM-SAE318 1.15
* ABE	   26/07/21 : TDP FCD Hybride fix trajet A52 vers Marseille en passant par l'A501 DEM-SAE318 1.16
* ABE    17/10/22 : Deplacement de la fonction calcul_zone_hors_reseau vers XZAO pour appel par taneu DEM-SAE318
--------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include	<dirent.h>

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzex.h>
#include	<xzao.h>
#include	<xzez.h>

#include "ex_mlcr.h"
#include "etim_don.h"

#include "etdp_don.h"
#include "etdp_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)etdp_don.c	1.13 02/14/12 : etdp_don.c" ;

/* declaration de fonctions internes */
int et_Maj_Fichier_heure_TDP_1H ( XDY_Eqt );

/* definition de fonctions externes */
ETDP_MESURE_ZONE_TDP		vg_Mesure[ETDPC_NB_MAX_UGTP];


/*X*/
/*--------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Lancement d'un timer une fois  
*
----------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int lancement_timer_etdp( int va_duree, char *va_cle)

/*
* ARGUMENTS EN ENTREE :
*
*	va_duree : Duree en seconde de la temporisation avant de recevoir la cle
*	va_cle : Cle recue a la fin de la temporisation
*
* ARGUMENTS EN SORTIE :
*
*  aucun
*
* CODE RETOUR :
*
*  aucun
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*
--------------------------------------------------------------------------------------*/
{
   static char *version = "@(#)etdp_don.c	1.13 02/14/12 : lancement_timer_etdp" ;
   int 			vl_jourSemaine;
   int 			vl_Minutes;
   int 			vl_Secondes;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       pl_Horodatedecompo;
   XZEXT_MSG_SOCKET 	pl_MsgTimer;
   ldiv_t		vl_Result_Div;

   
  
   /* Arrondi aux 6 min pleines */
   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
   XZSM_11ConversionHorodate ( vl_Horodate, &pl_Horodatedecompo );

   if ( va_duree == ETDPC_REVEIL_6MN )
   {
      vl_Minutes  = atoi ( &pl_Horodatedecompo[ETDPC_DIXMINUTE_STRING] );
      vl_Secondes = atoi ( &pl_Horodatedecompo[ETDPC_SECONDE_STRING] );
      vl_Result_Div = ldiv ( vl_Minutes, 6 );
      vl_Horodate = vl_Horodate - ( vl_Result_Div.rem * 60 ) - vl_Secondes + 60;
      ETIM_PROG_REVEIL( vl_Horodate+(double)va_duree, ETDPC_TIMER_6MN_RAF, vg_SockTIM, pl_MsgTimer)
      vl_Horodate -= 30;
   }
   else
   {
      sprintf ( &pl_Horodatedecompo[ETDPC_DIXMINUTE_STRING], "%02d:01", vg_ConfigTDP.DelaiDdeHoraire );
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_Horodate);
   }
   
   ETIM_PROG_REVEIL( vl_Horodate+(double)va_duree, va_cle, vg_SockTIM, pl_MsgTimer)

   return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Stockage des mesures 6 minutes  : Mes_Traitees_
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Stockage_Mesures_6mn ( XDY_Eqt va_NumUGTP )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP : Numero de la UGTP
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Stockage des mesures 6 minutes  dans un fichier : Mes_Traitees_
*
------------------------------------------------------*/
{
static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Stockage_Mesures_6mn" ;

XDY_Mot   		vl_Index = 0;
FILE 			*vl_Fd;
XDY_Entier   		vl_Cpt = 0;
XDY_Texte		pl_PathEtFic = "";
XDY_Texte		pl_PathEtFicLect = "";
char            	pl_Ligne[1500] = "";

XZST_03EcritureTrace( XZSTC_FONCTION, "IN %s : UGTP = %d ", version, va_NumUGTP);
/*A Construction du nom du fichier */
sprintf ( pl_PathEtFic, "%s/%s_W%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_STOCK_6MN, vg_DonneesUGTP[va_NumUGTP].Numero );
vl_Fd = fopen ( pl_PathEtFic, "a+" );
if (vl_Fd == NULL)
{
   XZST_03EcritureTrace( XZSTC_WARNING, "%s Impossible de creer le fichier <%s>", version, pl_PathEtFic );
   XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s retourne 0", version);
   return (XDC_NOK);
}

vl_Cpt = sprintf ( pl_Ligne, "%.0lf", vg_DonneesUGTP[va_NumUGTP].Horodate6mn );
for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ ) 
  if ( (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].ChampValide == Valide) &&
       !(vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].Etat_Service & XDC_EQT_HS) ) 
     vl_Cpt += sprintf ( &pl_Ligne[vl_Cpt], "\t%d|%d|%d|%d|%d", 
          vg_Mesure[va_NumUGTP].Numero[vl_Index],
          vg_Mesure[va_NumUGTP].TDP6mn[vl_Index],
          vg_Mesure[va_NumUGTP].Validite6mn[vl_Index] ,
	  vg_Mesure[va_NumUGTP].TDP6mnRADT[vl_Index],
	  vg_Mesure[va_NumUGTP].Validite6mnRADT[vl_Index] );

	fprintf (vl_Fd , "%s\n", pl_Ligne );
fclose ( vl_Fd );
XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s retourne 0", version);

/*A Recopie du fichier ecriture si pas de fichier lecture */ 
sprintf ( pl_PathEtFicLect, "%s/%s_R%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_STOCK_6MN, vg_DonneesUGTP[va_NumUGTP].Numero );
if ( access ( pl_PathEtFicLect, F_OK ) ) rename ( pl_PathEtFic, pl_PathEtFicLect );

return (XDC_OK);
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Envoi des mesures 6 minutes (Lecture du fichier)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Envoi_Mesure_6mn ( XDY_Eqt va_NumUGTP, XDY_Booleen va_Val_Suivante )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP : Numero de la UGTP
*   va_Val_Suivante : Booleen pour designer s'il faut lire la valeur suivante ou renvoyer la derniere
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Envoi des mesures 6 minutes (Lecture du fichier)
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Envoi_Mesure_6mn" ;

   XDY_Datagroup 		pl_DG = "";
   char            		pl_Ligne[1500] = "";
   XDY_Horodate    		vl_Horodate;
   ETDP_MESURE_ZONE_TDP		vl_Mesure;
   int				vl_Zone 	= 0;
   int    			vl_Nb_Param 	= 0;
   long				vl_LongLigne	= 0;
   char				*pl_Texte;

   /* Variables locales pour gestion des fichiers */
   XDY_Texte			pl_PathEtFic = "";

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : %s UGTP = %d %s", version, va_NumUGTP, (va_Val_Suivante == XDC_VRAI) ? "suivant" : "renvoi"  );
   
   /*B Construction du nom du fichier de stockage des mesures */ 
   sprintf ( pl_PathEtFic, "%s/%s_R%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_STOCK_6MN, vg_DonneesUGTP[va_NumUGTP].Numero );

   if ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn == NULL )
   {
      if ( !access ( pl_PathEtFic, F_OK ) )
      {
         errno = 0;
         if ( (vg_DonneesUGTP[va_NumUGTP].Fd_6mn = fopen ( pl_PathEtFic, "r+" )) == NULL )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "OUT : %s Impossible d'ouvrir fic stock <%d> pour UGTP = %d", version, errno, va_NumUGTP );
            return (XDC_NOK);
         }
      }
      else
      {
         XZST_03EcritureTrace( XZSTC_INFO, "OUT : %s Pas de fic stock pour UGTP = %d", version, va_NumUGTP );
         return (XDC_OK);
      }
   }

   /*A Lecture du fichier jusqu'a plus de commentaire */
   while ( fgets ( pl_Ligne, sizeof(pl_Ligne), vg_DonneesUGTP[va_NumUGTP].Fd_6mn ) != NULL )
      if ( pl_Ligne[0] != XDF_COMMENTAIRE ) 
      {
         if ( va_Val_Suivante == XDC_VRAI )
         {
            fseek ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn, -strlen(pl_Ligne) , SEEK_CUR );
            pl_Ligne[0] = XDF_COMMENTAIRE;
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Memo mesure transmise pour UGTP = %d <%s>", va_NumUGTP, pl_Ligne );
            fprintf ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn, "%s", pl_Ligne );
            fflush ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn );
            va_Val_Suivante = XDC_FAUX;
         }
         else
            break;
      }
   
   /*A Si fin du fichier => fermeture de celui-ci et effacement */
   if (feof ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn ))
   {
      fclose ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn );
      XZSS_06DetruireFichier ( pl_PathEtFic ); 
      XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : %s Fin du fichier stock pour UGTP = %d", version, va_NumUGTP );
      vg_DonneesUGTP[va_NumUGTP].Fd_6mn = NULL;
      return (XDC_OK);
   }
   vl_LongLigne = strlen(pl_Ligne);
   
   XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : Mesure lue = %s", version, pl_Ligne);

   if ( ( vl_Nb_Param = sscanf ( pl_Ligne , "%lf", &vl_Horodate ) ) != 1 )
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"%s : Probleme Lecture ligne : %s ", version, pl_Ligne );
      return (XDC_NOK);
   } 
      
   pl_Texte = strtok(pl_Ligne,"\t");
   
   while ( (pl_Texte = strtok(NULL,"\t")) != NULL )
   {
      if ( ( vl_Nb_Param = sscanf ( pl_Texte, "%hd|%hd|%hd|%hd|%hd", 
          				&vl_Mesure.Numero[vl_Zone],
          				&vl_Mesure.TDP6mn[vl_Zone],
          				&vl_Mesure.Validite6mn[vl_Zone] ,
					&vl_Mesure.TDP6mnRADT[vl_Zone],
					&vl_Mesure.Validite6mnRADT[vl_Zone]) ) != 5 )
      {
         XZST_03EcritureTrace(XZSTC_WARNING,"%s : Probleme Lecture ligne : %s ", version, pl_Texte );
         return (XDC_NOK);
      } 
      XZST_03EcritureTrace(XZSTC_INFO,"%s : OK ligne : %s ", version, pl_Texte );
      
      vl_Zone++;
   }

   fseek ( vg_DonneesUGTP[va_NumUGTP].Fd_6mn, -vl_LongLigne , SEEK_CUR );

   /*B  Construction du DatGroup XDG_EMES_UGTP_6MN<NomSite> */
   sprintf ( pl_DG, "%s_%s", XDG_EMES_UGTP_6MN, vg_ConfigTDP.NomSite );
   
   /*B Envoi du message XDM_EMES_TDP_6MN en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_DG
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN)
                        ,XDM_FLG_SRVMSGWRITE
               		,T_IPC_FT_REAL8, 	vl_Horodate
               		,T_IPC_FT_INT2, 	vg_DonneesUGTP[va_NumUGTP].Numero
               		,T_IPC_FT_INT2, 	ETDPC_REVEIL_6MN 
               		,T_IPC_FT_STR, 		vg_ConfigTDP.NomMachine
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.Numero, 	vl_Zone 
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.TDP6mn, 	vl_Zone 
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.Validite6mn,	vl_Zone 
			,T_IPC_FT_INT2_ARRAY,   vl_Mesure.TDP6mnRADT,       vl_Zone
			,T_IPC_FT_INT2_ARRAY,   vl_Mesure.Validite6mnRADT,  vl_Zone
               		,NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"%s : Envoi message XDM_EMES_TDP_6MN non effectue ", version );
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : Envoi message XDM_EMES_TDP_6MN effectue avec succes", version );
   }         
   /*B Envoi du message */
   TipcSrvFlush();
   return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Envoi des mesures 6 minutes par zone 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Envoi_Mesure_6mn_par_zone ( XDY_Eqt va_NumUGTP )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP : Numero de l'UGTP
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Envoi des mesures 6 minutes par zone 
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Envoi_Mesure_6mn_par_zone" ;

   XDY_Datagroup 		pl_DG_Zone = "";
   XDY_Datagroup 		pl_DG_All = "";
   XZEZT_Liste_Zdp *		pl_Zone;
   XDY_Eqt			vl_NumDerZone = 0;

   /* Variables locales pour gestion des fichiers */

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : %s UGTP = %d ", version, va_NumUGTP  );
   
   /*B  Construction du DatGroup XDG_EMES_UGTP_6MN_<ALL> */
   sprintf ( pl_DG_All, "%s_%d", XDG_EMES_UGTP_6MN, XDC_EQTALL );

   /* Recherche de la zone dans la liste */
   for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
   {
            XZST_03EcritureTrace(XZSTC_DEBUG2,"zone %d valide %d",
	    		pl_Zone->zdp.Code_Zone,
			vg_DonneesUGTP[va_NumUGTP].Zone[pl_Zone->zdp.Code_Zone].ChampValide);
            XZST_03EcritureTrace(XZSTC_DEBUG2,"Etat %d",
	    	vg_DonneesUGTP[va_NumUGTP].Zone[pl_Zone->zdp.Code_Zone].Etat_Service & XDC_EQT_HS);

     if ( (pl_Zone->zdp.Maitre == vg_DonneesUGTP[va_NumUGTP].Numero) &&
          (vg_DonneesUGTP[va_NumUGTP].Zone[pl_Zone->zdp.Code_Zone].ChampValide == Valide) &&
          !(vg_DonneesUGTP[va_NumUGTP].Zone[pl_Zone->zdp.Code_Zone].Etat_Service & XDC_EQT_HS) )
     {
        /*B  Construction du DatGroup XDG_EMES_UGTP_6MN_<NumZone> */
        sprintf ( pl_DG_Zone, "%s_%d", XDG_EMES_UGTP_6MN, pl_Zone->zdp.Numero );
        vl_NumDerZone = pl_Zone->zdp.Numero;
   
         /*B Envoi du message XDM_EMES_TDP_6MN_PAR_ZONE en fournissant les valeur recues */
         if (!TipcSrvMsgWrite (  pl_DG_Zone
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE)
                        ,XDM_FLG_SRVMSGWRITE
               		,T_IPC_FT_REAL8, 	vg_DonneesUGTP[va_NumUGTP].Horodate6mn
               		,T_IPC_FT_INT2, 	pl_Zone->zdp.Numero	 
               		,T_IPC_FT_INT2, 	pl_Zone->zdp.TDP_Courant	 
               		,T_IPC_FT_INT2, 	vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone]
               		,NULL))
         {
            XZST_03EcritureTrace(XZSTC_WARNING,"%s : Envoi message XDM_EMES_TDP_6MN_PAR_ZONE non effectue ", version );
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : Envoi message XDM_EMES_TDP_6MN_PAR_ZONE effectue avec succes pour z:%d avec T:%d", 
            				version, pl_Zone->zdp.Numero, pl_Zone->zdp.TDP_Courant );
         }         
         
         sprintf ( pl_DG_Zone, "%s_V%d", XDG_EMES_UGTP_6MN, pl_Zone->zdp.Numero );
         TipcSrvMsgWrite ( pl_DG_Zone 
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE)
                        ,XDM_FLG_SRVMSGWRITE
               		,T_IPC_FT_REAL8, 	vg_DonneesUGTP[va_NumUGTP].Horodate6mn
               		,T_IPC_FT_INT2, 	pl_Zone->zdp.Numero	 
               		,T_IPC_FT_INT2, 	pl_Zone->zdp.TDP_Courant	 
               		,T_IPC_FT_INT2, 	vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone]
               		,NULL);
         
         TipcSrvMsgWrite (  pl_DG_All
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE)
                        ,XDM_FLG_SRVMSGWRITE
               		,T_IPC_FT_REAL8, 	vg_DonneesUGTP[va_NumUGTP].Horodate6mn
               		,T_IPC_FT_INT2, 	pl_Zone->zdp.Numero	 
               		,T_IPC_FT_INT2, 	pl_Zone->zdp.TDP_Courant	 
               		,T_IPC_FT_INT2, 	vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone]
               		,NULL);
      }
   }
   
   /*B Envoi du message XDM_EMES_TDP_6MN_PAR_ZONE de fin  */
   sprintf ( pl_DG_Zone, "%s_V%d", XDG_EMES_UGTP_6MN, vl_NumDerZone );
   TipcSrvMsgWrite (  pl_DG_Zone
                        ,XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE)
                        ,XDM_FLG_SRVMSGWRITE
               		,T_IPC_FT_REAL8, 	0.
               		,T_IPC_FT_INT2, 	0	 
               		,T_IPC_FT_INT2, 	0	 
               		,T_IPC_FT_INT2, 	0
               		,NULL);
   
   return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Stockage des mesures horaires dans un fichier 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Stockage_Mesures_horaire ( XDY_Eqt va_NumUGTP, XDY_Entier va_Mesures_Valides )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP : Numero de la UGTP
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*  Stockage des mesures horaire dans un fichier 
*
------------------------------------------------------*/
{
static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Stockage_Mesures_horaire" ;

XDY_Mot   		vl_Index = 0;
XDY_Mot   		vl_Cpt = 0;
FILE 			*vl_Fd;
XDY_Texte		pl_PathEtFic = "";
XDY_Texte		pl_PathEtFicLect = "";
char            	pl_Ligne[1250] = "";

XZST_03EcritureTrace( XZSTC_FONCTION, "IN %s : UGTP = %d ", version, va_NumUGTP );
/*A Construction du nom du fichier */
sprintf ( pl_PathEtFic, "%s/%s_W%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_STOCK_HORAIRE, vg_DonneesUGTP[va_NumUGTP].Numero );
vl_Fd = fopen ( pl_PathEtFic, "a+" );
if (vl_Fd == NULL)
{
   XZST_03EcritureTrace( XZSTC_WARNING, "%s Impossible de creer le fichier <%s>", version, pl_PathEtFic );
   XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s retourne NOK", version);
   return (XDC_NOK);
}

vl_Cpt = sprintf ( pl_Ligne, "%.0lf", vg_DonneesUGTP[va_NumUGTP].Horodate1H );
for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
  if ( (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].ChampValide == Valide) &&
       !(vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].Etat_Service & XDC_EQT_HS) )
     vl_Cpt += sprintf ( &pl_Ligne[vl_Cpt], "\t%d|%d|%d|%d", 
          vg_Mesure[va_NumUGTP].Numero[vl_Index],
          vg_Mesure[va_NumUGTP].TDPHoraire[vl_Index],
          vg_Mesure[va_NumUGTP].NbApp[vl_Index],
          vg_Mesure[va_NumUGTP].ValiditeHoraire[vl_Index] );
fprintf (vl_Fd , "%s\n", pl_Ligne );

fclose ( vl_Fd );
XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s retourne 0", version);

/*A Recopie du fichier ecriture si pas de fichier lecture */ 
sprintf ( pl_PathEtFicLect, "%s/%s_R%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_STOCK_HORAIRE, vg_DonneesUGTP[va_NumUGTP].Numero );
if ( access ( pl_PathEtFicLect, F_OK ) ) rename ( pl_PathEtFic, pl_PathEtFicLect );

/*B Ecriture de l'horodate des dernieres mesures pesage recues dans un fichier si mesures valides */
if ( va_Mesures_Valides )
   et_Maj_Fichier_heure_TDP_1H ( va_NumUGTP );

return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Mise a jour du fichier derniere heure d'acquisition des mesures horaires
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Maj_Fichier_heure_TDP_1H ( XDY_Eqt va_NumUGTP )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP 	: Numero de la UGTP
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Maj_Fichier_heure_TDP_1H" ;
   FILE 		*vl_Fd;
   XDY_Texte		pl_PathEtFic = "";
   XZSMT_Horodate	vl_heure_string_his = "";
   int			vl_cr = XDC_OK;

   /*A Stockage de la derniere heure d'acquisition du pesage*/ 
   XZST_03EcritureTrace( XZSTC_INFO, "%s : MAJ HR Pesage St %d ", version, va_NumUGTP ); 
   sprintf ( pl_PathEtFic, "%s/%s_%s_R%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_HEUR_TDP, vg_ConfigTDP.NomMachine, vg_DonneesUGTP[va_NumUGTP].Numero );
   if ( ( vl_Fd = fopen ( pl_PathEtFic, "w+" ) ) == NULL )
   {
        XZST_03EcritureTrace( XZSTC_WARNING, "%s : fichier %s. introuvable", version, pl_PathEtFic ); 
        vl_cr = XDC_NOK;
   }
   else
   {
      XZSM_11ConversionHorodate ( vg_DonneesUGTP[va_NumUGTP].Horodate_Histo, &vl_heure_string_his );
      fprintf ( vl_Fd, "%s \n", vl_heure_string_his );
   }
   
   fclose ( vl_Fd );

   return ( vl_cr );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Lecture du fichier derniere heure d'acquisition des mesures horaires
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Lecture_Fichier_heure_TDP_1H ( XDY_Eqt va_NumUGTP )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP 	: Numero de la UGTP
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Lecture_Fichier_heure_TDP_1H" ;
   FILE 		*vl_Fd;
   XDY_Texte		pl_PathEtFic = "";
   char			pl_Heures[50]	= "";
   XDY_Horodate		vl_horodate	= 0.0;
   int 			vl_jourSemaine;
   XDY_Horodate		vl_Date_Actuelle= 0.0;
   XZSMT_Horodate       pl_Horodatedecompo;

   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Date_Actuelle );
   XZSM_11ConversionHorodate ( vl_Date_Actuelle, &pl_Horodatedecompo );
   strcpy ( &pl_Horodatedecompo[ETDPC_DIXMINUTE_STRING], "00:01" );
   XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_Date_Actuelle);
   vg_DonneesUGTP[va_NumUGTP].Horodate_Histo = vl_Date_Actuelle - (23 * 3600);

   /*A Lecture de la derniere heure d'acquisition du pesage*/ 
   XZST_03EcritureTrace( XZSTC_INFO, "%s : LEC HR Pesage St %d ", version, va_NumUGTP ); 
   sprintf ( pl_PathEtFic, "%s/%s_%s_R%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_HEUR_TDP, vg_ConfigTDP.NomMachine, vg_DonneesUGTP[va_NumUGTP].Numero );
   if ( ( vl_Fd = fopen ( pl_PathEtFic, "a+" ) ) != NULL )
   {
	fgets ( pl_Heures, sizeof(pl_Heures), vl_Fd );
	fclose ( vl_Fd );
        XZSM_13ConversionHorodateSec(pl_Heures,&vl_horodate);
        if ( vl_horodate > (vl_Date_Actuelle - (24 * 3600)) )
        {
	   vg_DonneesUGTP[va_NumUGTP].Horodate_Histo =  vl_horodate;
	}
    }
    
    XZST_03EcritureTrace( XZSTC_WARNING, "%s : HORO Histo = %s => %lf  pour UGTP = %d", version,
              				pl_Heures,
              				vg_DonneesUGTP[va_NumUGTP].Horodate_Histo, 
              				va_NumUGTP );
   return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Envoi des mesures horaire (Lecture du fichier)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Envoi_Mesure_TDP_Horaire ( XDY_Eqt va_NumUGTP, XDY_Booleen va_Val_Suivante )
			  
/*
* ARGUMENTS EN ENTREE :
*
*   va_NumUGTP     : Numero de la UGTP
*   va_Val_Suivante : Booleen pour designer s'il faut lire la valeur suivante ou renvoyer la derniere
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
* Envoi des mesures horaire (Lecture du fichier)
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_don.c	1.13 02/14/12 : et_Envoi_Mesure_TDP_Horaire" ;

   char            		pl_Ligne[250] 	= "";
   XDY_Horodate			vl_Horodate	= 0;
   ETDP_MESURE_ZONE_TDP		vl_Mesure;
   int    			vl_Nb_Param 	= 0;
   int				vl_Zone 	= 0;
   long				vl_LongLigne	= 0;
   char				*pl_Texte;

   /* Variables locales pour gestion des fichiers */
   
   XDY_Texte			pl_PathEtFic 	= "";

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : %s UGTP = %d %s", version, va_NumUGTP, (va_Val_Suivante == XDC_VRAI) ? "suivant" : "renvoi"  );

   /*B Construction du nom du fichier de stockage des mesures */ 
   sprintf ( pl_PathEtFic, "%s/%s_R%d", XZEXC_PATH_FIC_DYN, ETDPC_FIC_STOCK_HORAIRE, vg_DonneesUGTP[va_NumUGTP].Numero );

   if ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire == NULL )
   {
      if ( !access ( pl_PathEtFic, F_OK ) )
      {
         errno = 0;
         if ( (vg_DonneesUGTP[va_NumUGTP].Fd_Horaire = fopen ( pl_PathEtFic, "r+" )) == NULL )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "OUT : %s Impossible d'ouvrir fic stock(%d) pour UGTP = %d", version, errno, va_NumUGTP );
            return (XDC_NOK);
         }
      }
      else
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "OUT : %s Pas de fic stock pour UGTP = %d", version, va_NumUGTP );
         return (XDC_OK);
      }
   }

   /*A Lecture du fichier jusqu'a plus de commentaire */
   while ( fgets ( pl_Ligne, sizeof(pl_Ligne), vg_DonneesUGTP[va_NumUGTP].Fd_Horaire ) != NULL )
      if ( pl_Ligne[0] != XDF_COMMENTAIRE ) 
      {
         if ( va_Val_Suivante == XDC_VRAI )
         {
            fseek ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire, -strlen(pl_Ligne) , SEEK_CUR );
            pl_Ligne[0] = XDF_COMMENTAIRE;
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Memo mesure transmise pour UGTP = %d <%s>", va_NumUGTP, pl_Ligne );
            fprintf ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire, "%s", pl_Ligne );
            fflush ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire );
            va_Val_Suivante = XDC_FAUX;
         }
         else
            break;
      }
   
   /*A Si fin du fichier => fermeture de celui-ci et effacement */
   if (feof ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire ))
   {
      fclose ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire );
      XZSS_06DetruireFichier ( pl_PathEtFic ); 
      vg_DonneesUGTP[va_NumUGTP].Fd_Horaire = NULL;
      XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : %s Fin du fichier stock pour UGTP = %d", version, va_NumUGTP );
      return (XDC_OK);
   }
         
   XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : Mesure lue = %s", version, pl_Ligne);
   vl_LongLigne = strlen(pl_Ligne);

   if ( ( vl_Nb_Param = sscanf ( pl_Ligne , "%lf", &vl_Horodate ) ) != 1 )
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"%s : Probleme Lecture ligne : %s ", version, pl_Ligne );
      return (XDC_NOK);
   } 
   
   pl_Texte = strtok(pl_Ligne,"\t");
   
   while ( (pl_Texte = strtok(NULL,"\t")) != NULL )
   {
      if ( ( vl_Nb_Param = sscanf ( pl_Texte , "%hd|%hd|%hd|%hd", 
          				&vl_Mesure.Numero[vl_Zone],
          				&vl_Mesure.TDPHoraire[vl_Zone],
          				&vl_Mesure.NbApp[vl_Zone],
          				&vl_Mesure.ValiditeHoraire[vl_Zone] ) ) != 4 )
      {
         XZST_03EcritureTrace(XZSTC_WARNING,"%s : Probleme Lecture ligne : %s ", version, pl_Texte );
         return (XDC_NOK);
      } 

      vl_Zone++;
   }

   fseek ( vg_DonneesUGTP[va_NumUGTP].Fd_Horaire, -vl_LongLigne , SEEK_CUR );
   
   /*B Envoi du message XDM_EMES_TDP_HORAIRE en fournissant les valeurs lues */
   if (!TipcSrvMsgWrite ( XDG_EMES_UGTP_HORAIRE
                        ,XDM_IdentMsg(XDM_EMES_TDP_HORAIRE)
                        ,XDM_FLG_SRVMSGWRITE
               		,T_IPC_FT_REAL8, 	vl_Horodate
               		,T_IPC_FT_INT2, 	vg_DonneesUGTP[va_NumUGTP].Numero
               		,T_IPC_FT_INT2, 	ETDPC_REVEIL_1H 
               		,T_IPC_FT_STR, 		vg_ConfigTDP.NomMachine
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.Numero, 		vl_Zone 
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.TDPHoraire, 		vl_Zone 
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.NbApp, 		vl_Zone 
               		,T_IPC_FT_INT2_ARRAY, 	vl_Mesure.ValiditeHoraire,	vl_Zone 
               		,NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"%s : Envoi message XDM_EMES_TDP_HORAIRE non effectue ", version );
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : Envoi message XDM_EMES_TDP_HORAIRE effectue avec succes", version );
   }         
   /*B Envoi du message */
   TipcSrvFlush();

   return (XDC_OK);
}


