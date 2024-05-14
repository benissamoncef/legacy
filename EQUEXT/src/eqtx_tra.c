/*E*/
/*  Fichier : $Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2011/03/17 14:40:26 $
-------------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TEQTX * FICHIER eqtx_tra.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant des eqt QTX
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	25 Oct 1996	: Creation
* Orengo.A	Version 1.2	05 Nov 1996	: Changement du nom des equipements
* Orengo.A	Version 1.3	05 Nov 1996	: Nodif suite au passade des tests
* Mismer.D	version 1.4	13 Jan 1997	: Modification protocole (DEM/1393)
* Mismer.D	version 1.5	14 Jan 1997	: Modification protocole (DEM/1393)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	25/02/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK ou NOK pour plusieurs fonctions  1.7
* JPL	25/02/11 : Migration architecture HP ia64 (DEM 975) : adequation format scanf et type de donnee  1.8
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzamc.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"
#include 	"ex_mala.h"
#include 	"etim_don.h"

#include	"eqtx_don.h"
#include	"eqtx_cfg.h"
#include	"eqtx_cmd.h"
#include	"eqtx_dir.h"
#include	"eqtx_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

static int em_traitement_trame_maitre_donnees ( char *, int );
static int em_envoi_donnees_vers_Base_Donnees( char * );
int em_envoi_fichier_vers_Base_Donnees( char * );
static int em_traitement_validitee_trame_esclave_alarmes_alertes ( char *, int );
static int em_traitement_trame_esclave_alarmes_alertes(
						char *, char *, int, int, int, char, int);
static int eg_envoi_alarme_err_crc( XDY_Horodate, XDY_TypeAlarme, XDY_Booleen, char *, int);
static int eg_envoi_acquittement( XDY_Eqt, int);

/* definition de fonctions externes */

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame et traitement
--------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame_maitre( int va_socket, int va_indice_table_eqt)

/*
* ARGUMENTS EN ENTREE :
*   
*	va_socket		:	socket de lecture
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_traitement_trame_maitre" ;
   int vl_indice_table_eqt = 0;
   int i = 0;
   int vl_IndCnx = 0;
   int vl_LgMsg  = 0;
   char	*pl_trame;
   char vl_entete1[10];
   char vl_entete2[10];
   char vl_adrrgs[10];
   int vl_typetrame;
   char vl_Msg[256];
   int vl_param;
   int vl_nbnonrep;
   int vl_nberrcrc;
   XDY_Horodate vl_horodate;
   int vl_nocmd;   
   char vl_horod1[20],vl_date[20];
   char vl_horod2[9],vl_heure[9];
   XZSMT_Horodate pl_Horodatedecompo;
   int vl_typ_rep;
   char vl_arg[6];

    /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket Maitre. " );
      return (XDC_NOK);
   }
   else
   {
      XZST_03EcritureTrace( XZSTC_INFO, " --------------- message : %s", vl_Msg);
      
      /*A Scan du message provenant de la socket */
      
      vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
      		vl_entete2, vl_horod1, vl_horod2, &vl_nbnonrep, &vl_nberrcrc,
      				&vl_nocmd, vl_adrrgs, &vl_typetrame, &vl_typ_rep);
      					 
      if (vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING,
         		" Lecture message socket maitre impossible mes=%s",vl_Msg);
         return (XDC_NOK);
      }
      					 
      /*A Conversion horodate */
      
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      /* Recherche l'�quipement concern�e */
      for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
      {
         if ( (vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide == Valide) &&
              !strcmp(vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS, vl_adrrgs) )
         {
            vl_indice_table_eqt = i;
            break;
         }
      }
      
      /* Si l'�quipement concern�e ne peut pas etre touve, retourne erreur*/
      if ( i == NB_EQT_MAX_TEQTX )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "Equipement inconnu  mes=%s",vl_Msg);
         return (XDC_NOK);
      }
      
      /*A Gestion des trames provenant de telmi */

      /* Si l'equipement n'est pas declare HS */
   
      if(( vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Etat_Service &
							        XDC_EQT_HS) != XDC_EQT_HS )
      {
         /* Alors */
         /* Traitement des alarmes transmission avec l'equipement */
      
         if( ex_traite_alarme_com( vg_donnees_systeme_qtx.NomMachine,
	   vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].TypeEqt,
	      vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Numero,
            			vl_typ_rep, vl_nbnonrep, vl_nberrcrc, vl_typetrame,
            			            vg_donnees_systeme_qtx.Mode_Fonct,
					    vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].SiteGestion) == XDC_NOK)
         {
            XZST_03EcritureTrace( XZSTC_WARNING,
               " Erreur trame non transmise ou non comprise sur l'eqt no %d",
               		vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Numero);
         }
      }
      /* Finsi */
      /* On se positionne sur le debut de la trame */
      
      pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;

      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {
         /*B C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
         {
            /* Appel de ex_mrcr */
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeQTXFLcr, vl_typ_rep );
            break;
         }
         
         /*B Recuperation des donnees meteo */
         case XZEXC_TRAME_DONNEES_METEO:
            /* Traitement de la TRAME recue */
            
            if ( (vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].TypeEqt == XDC_EQT_MET) &&
                 (vl_typ_rep != XZEXC_REP_NAK) )
            {
               em_traitement_trame_maitre_donnees ( strtok (pl_trame,XZEXC_FIN_MSG_SOCK), vl_indice_table_eqt );
            }
            else
            {
               XZST_03EcritureTrace( XZSTC_WARNING,"Reponse incorrecte pour Eqt %d", vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Numero );
               return (XDC_NOK);
            }
            break;
            
      }
   }

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame donnees et traitement
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int em_traitement_trame_maitre_donnees ( char *pa_trame_utile, int va_indice_table_eqt )

/*
* ARGUMENTS EN ENTREE :
*   
* pa_trame_utile 	: Pointeur sur la trame recu partie utile
* va_indice_table_eqt	: Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  Decodage de la trame et traitement
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_traitement_trame_maitre_donnees";
   static char *msg_erreur="Erreur de recuperation donnees Trame Maitre:" ;
   char vl_date_debut[9];
   char vl_heure_debut[9];
   char vl_date_fin[9] = "";
   char vl_heure_fin[9] = "";
   char vl_date[11];
   int vl_type_pluie;
   int vl_nb_elements_recuperes;
   int vl_jour;
   int vl_mois;
   int vl_annee;
   char *pl_nom_fichier_historique;
   char *pl_nom_fichier_envoi;
   int vl_NumEqt;
   FILE *fp;
   
   vl_nb_elements_recuperes=sscanf(pa_trame_utile, " %d %s %s %s %s",&vl_type_pluie,
   				vl_date_debut, vl_heure_debut, vl_date_fin,vl_heure_fin);
   				
   if( vl_nb_elements_recuperes < 3 )
   {
      /* Probleme de recuperation des elements de la trame */
      
      XZST_03EcritureTrace( XZSTC_WARNING,
               			" %s Nombre d'arguments trame invalides <%s> ", msg_erreur, pa_trame_utile );
      return (XDC_NOK);
   }
   else
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Arguments trame mesure meteo Eqt:%d db<%s> hd<%s> df<%s> hf<%s>", 
               			           vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero,
               			           vl_date_debut, vl_heure_debut, vl_date_fin,vl_heure_fin );
               			
      /* Recuperer le nom du fichier envoi */
      pl_nom_fichier_envoi=em_retourne_nom_fichier_envoi_qtx(vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero);
      
      /* Recuperation de la date */
         
      sscanf(vl_date_debut,"%d/%d/%d", &vl_jour, &vl_mois, &vl_annee);
      if(vl_annee>90)
      {
         vl_annee+=1900;
      }
      else
      {
         vl_annee+=2000;
      }
         
      /* Creation du fichier envoi */
         
      fp=fopen(pl_nom_fichier_envoi,"w");
      fprintf(fp, FORMAT_FICHIER_RECEPTION_DONNEES_METEO, vl_jour, vl_mois, vl_annee);
      fclose(fp);
         
      /* Creation du fichier historique */
         
      pl_nom_fichier_historique=em_retourne_nom_fichier_historique_qtx( vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero, "valides");
         
      vl_NumEqt=vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero;
               
      /* Creation et ecriture dans le fichier historique */
         
      fp=fopen( pl_nom_fichier_historique, "w");
      sprintf(vl_date, "%d/%d/%d", vl_jour, vl_mois, vl_annee);
      if ( !strlen (vl_date_fin) || !strlen (vl_heure_fin) )
      {
         strcpy ( vl_heure_fin, "23:59:59" );
      }
      fprintf(fp, XZEAC_FORMAT_FICHIER_HISTORIQUE_DONNEES,
         	  vl_NumEqt, (vl_type_pluie<4) ? XDC_VRAI : XDC_FAUX, vl_date,
          		vl_heure_debut, vl_date, vl_heure_fin, (vl_type_pluie<4) ? vl_type_pluie : 0 );
      fclose(fp);
      em_envoi_donnees_vers_Base_Donnees( pl_nom_fichier_historique);
   }

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*		Creation du fichier de donnees meteo invalides
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_creation_fichier_donnees_meteo_invalides( int va_NumEqt )

/*
* ARGUMENTS EN ENTREE :
*
* va_NumEqt : Numero de la station meteo
*   
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_creation_fichier_donnees_meteo_invalides ";
   char *vl_heure="00:00:00";
   int vl_type_pluie=0;
   char vl_date[11];
   int vl_jour;
   int vl_mois;
   int vl_annee;
   FILE *fp;
   char *pl_nom_fichier_historique;
   int vl_jourSemaine;
   XDY_Horodate vl_Horodate;
   XZSMT_Horodate pl_Horodatedecompo;
   XZSMT_Horodate pl_heure;

   /* Recuperer la date systeme */
      
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
   XZSM_11ConversionHorodate( vl_Horodate, &pl_Horodatedecompo);
   sscanf( pl_Horodatedecompo, "%d/%d/%d %s", &vl_jour, &vl_mois, &vl_annee, pl_heure);
      
   /* Creation du nom du fichier historique */
         
   pl_nom_fichier_historique=em_retourne_nom_fichier_historique_qtx( va_NumEqt, "invalides");
               
   /* Si le fichier n'existe pas */
         
   if( access( pl_nom_fichier_historique, F_OK)==-1)
   {
      /* Alors */
      /* Creation et ecriture dans le fichier historique */
         
      fp=fopen( pl_nom_fichier_historique,"w");
      sprintf(vl_date, "%d/%d/%d", vl_jour, vl_mois, vl_annee);
      fprintf(fp, XZEAC_FORMAT_FICHIER_HISTORIQUE_DONNEES,
         		    va_NumEqt, XDC_FAUX, vl_date,
          			vl_heure, vl_date, vl_heure, vl_type_pluie);
      fclose(fp);
   }

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Envoi les datas lues dans le fichier passe en argument vers la Base de Donnees
*				via le RT SERVER
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int em_envoi_donnees_vers_Base_Donnees( char *pa_nom_fichier_historique )

/*
* ARGUMENTS EN ENTREE :
*   
*	pa_nom_fichier_historique : Nom du fichier historique
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_envoi_donnees_vers_Base_Donnees";
   char vl_contenu_fichier[2000];
   char *pl_contenu_fichier;
   int vl_c;
   FILE *fp;
   
   fp=fopen(pa_nom_fichier_historique, "r");
   
   /* Si le fichier n'existe pas */
   
   if(fp==NULL)
   {
      /* Alors */
      /* Un message d'erreur */
      
      XZST_03EcritureTrace( XZSTC_WARNING,
	      " Impossible d'ouvrir le fichier '%s' pour l'envoi vers le RT SERVER",
      								pa_nom_fichier_historique);
   }
   else
   {
      /* Sinon */
      /* Recuperation du contenu du fichier */
      
      pl_contenu_fichier=vl_contenu_fichier;
      while((vl_c=fgetc(fp))!=EOF)
      {
         *pl_contenu_fichier=(char)vl_c;
         pl_contenu_fichier++;
      }
      *pl_contenu_fichier='\0';
      
      /* Fermeture du fichier */
      
      fclose(fp);
           
      /*B Envoi du message d'etat */
      
      if( !TipcSrvMsgWrite( XDG_DONNEES_METEO_QTX, XDM_IdentMsg(XDM_DONNEES_METEO_QTX),
                   XDM_FLG_SRVMSGWRITE, T_IPC_FT_STR, pa_nom_fichier_historique,
                          	   T_IPC_FT_STR, vg_donnees_systeme_qtx.NomMachine,
                          			T_IPC_FT_STR, vl_contenu_fichier, NULL))
      {
         XZST_03EcritureTrace(XZSTC_WARNING,
         			"Envoi message XDM_DONNEES_METEO_QTX non effectue." );
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_MESSAGE,
         		"Envoi message XDM_DONNEES_METEO_QTX effectue sur %s", XDG_DONNEES_METEO_QTX );
      }
   }
   /* Finsi */
   
   return(XDC_OK);
}      
   
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Envoi les datas lues dans le fichier passe en argument vers la Base de Donnees
*				via le RT SERVER
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_envoi_fichier_vers_Base_Donnees( char *pa_nom_fichier)

/*
* ARGUMENTS EN ENTREE :
*   
*	pa_nom_fichier : Nom du fichier a traiter
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_envoi_fichier_vers_Base_Donnees";
   
   /* Si c'est un fichier historique */
   
   if( ( strstr( pa_nom_fichier, NOM_FICHIER_EQTX_HISTORIQUE_DONNEES))!=NULL)
   {
      /* Alors */
      /* Envoyer les donnees vers la base de donnees */
            
      em_envoi_donnees_vers_Base_Donnees( pa_nom_fichier );
   }
   /* Finsi */
   
   return(XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame esclave et traitement
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame_esclave( int va_socket, int va_indice_table_eqt)

/*
* ARGUMENTS EN ENTREE :
*   
*	va_socket		:	Socket de lecture
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_traitement_trame_esclave" ;
   int vl_IndCnx = 0;
   int vl_LgMsg  = 0;
   char *pl_trame;
   char *pl_trame_utile;
   char vl_entete1[10];
   char vl_entete2[10];
   char vl_adrrgs[10];
   int vl_typetrame;
   char vl_Msg[256];
   int vl_param;
   int vl_nbnonrep;
   int vl_nberrcrc;
   XDY_Horodate vl_horodate;
   int vl_nocmd;   
   char vl_horod1[20],vl_date[20];
   char vl_horod2[9],vl_heure[9];
   XZSMT_Horodate pl_Horodatedecompo;
   int vl_typ_rep;
   char vl_arg[6];

    /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket esclave. " );
      return (XDC_NOK);
   }
   else
   {
      /*A Si deconnexion tache esclave alors fin de la tache */
      if( vl_IndCnx == XZEXC_DECONNEC)
      {
         /* Ecriture trace . */
         XZST_03EcritureTrace( XZSTC_WARNING,
         		"Perte connexion avec la tache esclave alors : fin de la tache. ");
         ec_sortir();
      }
      
      XZST_03EcritureTrace( XZSTC_INFO, " --------------- message : %s", vl_Msg);
      
      /*A Scan du message provenant de la socket */
      
      vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
      		vl_entete2, vl_horod1, vl_horod2, &vl_nbnonrep, &vl_nberrcrc,
      				&vl_nocmd, vl_adrrgs, &vl_typetrame, &vl_typ_rep);
      					 
      if( vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING,
         			" Lecture message socket esclave impossible mes=%s",vl_Msg);
         return (XDC_NOK);
      }
      					 
      /*A Conversion horodate */
      
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);

      /*A Gestion des trames provenant de telei */

      if( vl_typetrame == XZEXC_TRAME_TELEI_KO)
      {
         /* Envoi de l'alarme */
         
         sprintf(vl_arg,"%d",vl_nberrcrc);
         
         if( !(vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Etat_Service
         								& XDC_EQT_HORS_SRV))
         {
            eg_envoi_alarme_err_crc( vl_horodate,
            		XZAMC_ERR_CRC_TELEI, XDC_VRAI, vl_arg, va_indice_table_eqt);
         }
         XZST_03EcritureTrace( XZSTC_WARNING, " Err crc provenant de l'equipement");
         return (XDC_NOK);
      }
      else
      {
         /* Envoi fin de l'alarme */
         
         sprintf(vl_arg,"%d",vl_nberrcrc);
         
         if( !(vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Etat_Service
         								& XDC_EQT_HORS_SRV))
         {
            eg_envoi_alarme_err_crc( vl_horodate,
            		XZAMC_ERR_CRC_TELEI, XDC_FAUX, vl_arg, va_indice_table_eqt);
         }
      }
	 
      /*A On se positionne sur le debut de la trame */
      pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;

      /* Si la trame est une trame Alarme / Alerte */
      
      if( ( pl_trame_utile=strstr(pl_trame,"TC E"))!=NULL)
      {
         em_traitement_validitee_trame_esclave_alarmes_alertes(
         					strtok(pl_trame_utile,XZEXC_FIN_MSG_SOCK),  va_indice_table_eqt);
      }
   }

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame alarmes / alertes et traitement
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int em_traitement_validitee_trame_esclave_alarmes_alertes ( char *pa_trame_utile,
								int va_indice_table_eqt)
/*
* ARGUMENTS EN ENTREE :
*   
*	pa_trame_utile		:	Pointeur sur la treme recue partie utile
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  Decodage de la trame et traitement
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_traitement_validitee_trame_esclave_alarmes_alertes";
   static char *msg_erreur="Erreur de recuperation donnees Trame Esclave Alarmes / Alertes:";
   int vl_indice_table_eqt = 0;
   int i = 0;
   char vl_date[9];
   char vl_heure[9];
   int vl_type_evenement = 0;
   int vl_code_systeme = 0;
   int vl_numero_evenement = 0;
   char vl_etat_evenement;
   int vl_nb_elements_recuperes;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   char vl_cle_timer[256];
   
   vl_nb_elements_recuperes=sscanf( pa_trame_utile, "TC E %s %s %d %d %d %c", vl_date,
                                      vl_heure, &vl_type_evenement, &vl_code_systeme,
     						&vl_numero_evenement,&vl_etat_evenement);
   
   if( (vl_nb_elements_recuperes < 4) ||
       ((vl_nb_elements_recuperes != 6) && (vl_type_evenement)) )
   {
      /* Probleme de recuperation des elements de la trame et envoi d'un NACK */
      
      XZST_03EcritureTrace( XZSTC_WARNING, " %s Nombre d'arguments trame invalides <%s>", msg_erreur, pa_trame_utile );

      eg_envoi_acquittement( XZEXC_TRAME_TELEI_NAK, va_indice_table_eqt);
   }
   else
   {
      /* Verification de la coherence des donnees lues */
      
      if(vl_type_evenement!=0 && vl_type_evenement!=1 && vl_type_evenement!=2)
      {
         /* Probleme de recuperation du type d'evenement et envoi d'un NACK */
      
         XZST_03EcritureTrace( XZSTC_WARNING, " %s Type d'evenement inconnu", msg_erreur);

         eg_envoi_acquittement( XZEXC_TRAME_TELEI_NAK, va_indice_table_eqt);
      }
      else
      {
         if( vl_type_evenement && vl_etat_evenement!='D' && vl_etat_evenement!='F' && vl_etat_evenement!='0' )
         {
            /* Probleme de recuperation du code evenement et envoi d'un NACK */
      
            XZST_03EcritureTrace( XZSTC_WARNING,
                                   " %s Type de code d'evenement inconnu", msg_erreur);

            eg_envoi_acquittement( XZEXC_TRAME_TELEI_NAK, va_indice_table_eqt);
         }
         else
         {
            for(  i=0; i!=NB_EQT_MAX_TEQTX ;i++)
            {
               if( (vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide == Valide) &&
                   (vg_donnees_systeme_qtx.DonneesEqt[i].code_systeme_gere == vl_code_systeme) )
               {
                  vl_indice_table_eqt = i;
                  break;
               }
            }
            if ( i == NB_EQT_MAX_TEQTX )
            {
               /* Probleme de recuperation du code systeme et envoi d'un NACK */
      
               XZST_03EcritureTrace( XZSTC_WARNING,
                    " %s Le code systeme n'est pas celui attendu par la tache, trame:<%s>", msg_erreur, pa_trame_utile );

               eg_envoi_acquittement( XZEXC_TRAME_TELEI_NAK, va_indice_table_eqt);
            }
            else
            {
               /* Acquitement de la bonne reception du message */
               eg_envoi_acquittement( XZEXC_TRAME_TELEI_ACK, vl_indice_table_eqt);

               /* Traitement des alarmes et des alertes */
               em_traitement_trame_esclave_alarmes_alertes( vl_date, vl_heure,
			      vl_type_evenement, vl_code_systeme, vl_numero_evenement,
			        		   vl_etat_evenement, vl_indice_table_eqt);
			        		   
               /* Arret puis relancement du timer du test ligne avec le protocole Esclave */
               
      	       sprintf( vl_cle_timer, "%s%02d", CLE_CYCLE_TIMER_QTX_LIGNE_ESCLAVE,
									vl_indice_table_eqt);
               ETIM_ANNUL_REVEIL(0, vl_cle_timer, vg_SockTIM, pl_MsgTimer);
               lancement_timer_eqtx( EQTXC_REVEIL_LIGNE_ESCLAVE, vl_cle_timer );
            }
         }
      }
   }

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame alarmes / alertes et traitement
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int em_traitement_trame_esclave_alarmes_alertes( char *va_date, char *va_heure,
	       int va_type_evenement, int va_code_systeme, int va_numero_evenement,
					char va_etat_evenement, int va_indice_table_eqt)
/*
* ARGUMENTS EN ENTREE :
*   
*	va_date			:	Date du message
*	va_heure		:	Heure du message
*	va_type_evenement	:	Type d'evenement Ok, Alerte ou Alarme
*	va_code_systeme		:	Code du systeme
*	va_numero_evenement	:	Numero de l'evenement
*	va_etat_evenement	:	Etat de l'evenement Debut 'D' ou Fin 'F'
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  Decodage de la trame et traitement
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : em_traitement_trame_esclave_alarmes_alertes";
   XDY_Booleen vl_Etat;
   int vl_jour;
   int vl_mois;
   int vl_annee;
   XDY_Horodate vl_Horodate;
   XZSMT_Horodate vl_chaine_horodate;
   EQTX_ALARMES_ALERTES_SYSTQTX *pl_alarmes_alertes;
     
   /* Preparation des arguments pour le passage aux procedures ex_... */
   
   /* Si le va_etat_evemenent est 'D' */
   
   if(va_etat_evenement!='F')
   {
      /* Alors */
      /* C'est un etat actif */
      
      vl_Etat=XDC_VRAI;
   }
   else
   {
      /* Sinon */
      /* C'est un etat inactif */
      
      vl_Etat=XDC_FAUX;
   }
   /* Finsi */
   
   /* Recuperation de l'horodate depuis le message */
   
   sscanf(va_date, "%d/%d/%d", &vl_jour, &vl_mois, &vl_annee);
   if(vl_annee>90)
   {
      vl_annee+=1900;
   }
   else
   {
      vl_annee+=2000;
   }
   sprintf( vl_chaine_horodate, "%d/%d/%d %s", vl_jour, vl_mois, vl_annee, va_heure);
   XZSM_13ConversionHorodateSec( vl_chaine_horodate, &vl_Horodate);
   
   /* Case selon le type de message Alarme ,Alerte, Tout Ok */
   
   switch( va_type_evenement)
   {
      case 0:
         /* Rien a faire Tout est OK */
         /* Ce message sert a connaitre uniquement l'etat de l'equipement en ligne */
         
         break;
      
      case 1:
         /* C'est une alarme */
         /* Si l'equipement n'est pas declare HS et ni inactif */
         
         if( vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Etat_Service==0)
         {
            /* Alors */
            pl_alarmes_alertes=
               vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].ptr_alarmes_alertes;
                 
            if ( (pl_alarmes_alertes+va_numero_evenement)->Numero_Alarme )
               ex_env_alarme( vg_donnees_systeme_qtx.NomMachine,
                 vl_Horodate, vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].TypeEqt,
                   vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero,
	             (pl_alarmes_alertes+va_numero_evenement)->Numero_Alarme, vl_Etat,NULL,
		     vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].SiteGestion);
         }
         /* Finsi */
         break;
         
      case 2:
         /* C'est une alerte */
         /* Si l'equipement n'est pas declare HS et ni inactif */
         
         if( vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Etat_Service==0
         							&& vl_Etat==XDC_VRAI)
         {
            /* Alors */
         
            pl_alarmes_alertes=
               vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].ptr_alarmes_alertes;
                 
            if ( (pl_alarmes_alertes+va_numero_evenement)->Numero_Alerte )
               ex_env_alerte( vl_Horodate,
                        vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].TypeEqt,
                          vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero,
	                    (pl_alarmes_alertes+va_numero_evenement)->Numero_Alerte,
	                      vg_donnees_systeme_qtx.Mode_Fonct, 
			       vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].SiteGestion,
	                       (pl_alarmes_alertes+va_numero_evenement)->Texte_Alerte);
         }
         /* Finsi */
         break;
   }               
   /* Si l'equipement n'est pas declare HS */
         
   if(( vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Etat_Service &
							        XDC_EQT_HS) != XDC_EQT_HS )
   {
      /* Alors */
      /* Supression de l'alarme relative a la liaison */
      
      ex_env_alarme( vg_donnees_systeme_qtx.NomMachine, vl_Horodate,
                   vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].TypeEqt,
                        vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero,
				vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt]
					.ptr_alarmes_alertes->Numero_Alarme, XDC_FAUX,NULL,
					vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].SiteGestion);
   }
   /* Finsi */

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Envoi de alarme CRC
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int eg_envoi_alarme_err_crc( XDY_Horodate va_Horodate, XDY_TypeAlarme va_TypeAlarme,
			XDY_Booleen va_Etat, char *pa_ArgAlarme, int va_indice_table_eqt  )

/*
* ARGUMENTS EN ENTREE :
*   
*	va_Horodate		:	Horodate de l'evenement
*	va_TypeAlarme		:	Type de l'alarme (Ici Toujours erreur CRC)
*	va_etat			:	Etat de l'alarme
*	va_ArgAlarme		:	Commentaire de l'alarme
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : eg_envoi_alarme_err_crc";
   
   /* Si l'equipement n'est pas declare HS et ni inactif */
         
   if( vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Etat_Service==0 &&
   							va_TypeAlarme==XZAMC_ERR_CRC_TELEI)
   {
      /* Alors */
      
      ex_env_alarme(vg_donnees_systeme_qtx.NomMachine,
         va_Horodate, vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].TypeEqt,
	    vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].Numero, va_TypeAlarme,
								    va_Etat, pa_ArgAlarme,
			vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].SiteGestion   );
   }
   /* Finsi */

   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'un acquittement a TELEI
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int eg_envoi_acquittement( XDY_Eqt va_Acqt, int va_indice_table_eqt)

/*
* ARGUMENTS EN ENTREE :
*
*	va_Acqt			:	Message d'acquitement ou de non acquitement  
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_tra.c,v 1.8 2011/03/17 14:40:26 gesconf Exp $ : eg_envoi_acquittement" ;

   /* Acquitement telei */
     
   EnvoyerTrame( "\0", va_Acqt,
	vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].AdresseRGS,
	    XZEXC_NOCMD, XZEXC_PRIORITEFAIBLE,
		vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].SocketEsclave);

   return (XDC_OK);
}
