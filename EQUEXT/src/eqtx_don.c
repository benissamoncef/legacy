/*E*/
/*  Fichier : $Id: eqtx_don.c,v 1.4 2018/10/26 15:16:44 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/10/26 15:16:44 $
----------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR 
----------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
----------------------------------------------------------------------------------------
* MODULE TERAU * FICHIER eqtx_don.c
----------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de definition de donnees du systeme qtx
*
----------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	19 Oct 1996	: Creation
* Orengo.A	version 1.2	06 Nov 1996	: Modification du nom des equipements
* Mismer.D	version 1.3	13 Jan 1997	: Modification protocole (DEM/1393)
* PNI		v1.4		26/10/18	: delta 
--------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include	<dirent.h>

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzex.h>

#include "etim_don.h"

#include "eqtx_don.h"
#include "eqtx_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eqtx_don.c,v 1.4 2018/10/26 15:16:44 pc2dpdy Exp $ : eqtx_don.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	retourne le nom du fichier historique formatee en fonction de la date
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *em_retourne_nom_fichier_envoi_qtx( int va_NumEqt )

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
   static char *version = "$Id: eqtx_don.c,v 1.4 2018/10/26 15:16:44 pc2dpdy Exp $ : em_retourne_nom_fichier_envoi_qtx";
   static char vl_path[100]={'\0'};
   
   sprintf(vl_path,"%s/%s_%d\0",XZEXC_PATH_FIC_DYN, NOM_FICHIER_EQTX_RECEPTION_DONNEES_METEO, va_NumEqt );
   return(vl_path);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	retourne le nom du fichier historique formatee en fonction de la date
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *em_retourne_nom_fichier_historique_qtx( int va_NumEqt, char *va_extention)

/*
* ARGUMENTS EN ENTREE :
*   
* va_NumEqt : Numero de la station meteo
* va_extention : Extention du fichier historique
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
   static char *version = "$Id: eqtx_don.c,v 1.4 2018/10/26 15:16:44 pc2dpdy Exp $ : em_retourne_nom_fichier_historique_qtx";
   static char vl_path[100];
   int vl_jour;
   int vl_mois;
   int vl_annee;
   int vl_jourSemaine;
   XDY_Horodate vl_Horodate;
   XZSMT_Horodate pl_Horodatedecompo;
   XZSMT_Horodate pl_heure;

   /* Recuperer la date systeme */
      
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
   XZSM_11ConversionHorodate( vl_Horodate, &pl_Horodatedecompo);
   sscanf( pl_Horodatedecompo, "%d/%d/%d %s", &vl_jour, &vl_mois, &vl_annee, pl_heure);
      
   sprintf(vl_path, "%s/%s_%d_%d_%d_%d.%s\0", XZEXC_PATH_FIC_DYN,
		  NOM_FICHIER_EQTX_HISTORIQUE_DONNEES, vl_annee, vl_mois, vl_jour, va_NumEqt, va_extention);

   return(vl_path);
}

/*X*/
/*--------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Lecture des fichiers d'un repertoire
*
----------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_lecture_repertoire( char *pa_nom_repertoire, int (*fa_proc_repertoire)(char *))

/*
* ARGUMENTS EN ENTREE :
*
*	pa_nom_repertoire : Nom du repertoire a lire
*	fa_proc_repertoire : Fonction a executer
*		Cette procedure possede un seul argument le nom du fichier a traiter
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
*	Ouvre un repertoire en lecture et exexcute la fonction passe en argument
*	pour tous les fichiers trouves 
*
--------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_don.c,v 1.4 2018/10/26 15:16:44 pc2dpdy Exp $ : em_lecture_repertoire";
   char pl_NomFichier[XDC_PATH_ABS_SIZE];
   DIR *pl_repert;
   struct dirent *pl_unFic;

   /* Le repertoire est-il accessible */
   
   if( access(pa_nom_repertoire, R_OK)!=0)
   {
       XZST_03EcritureTrace( XZSTC_WARNING,
			       "Acces repertoire '%s' impossible", pa_nom_repertoire);
       return(XDC_NOK);
   }

   /* Ouverture du repertoire */
   
   if ( (pl_repert = opendir(pa_nom_repertoire) ) == NULL)
   {
      XZST_03EcritureTrace( XZSTC_WARNING,
			"Ouverture repertoire '%s' impossible", pa_nom_repertoire);
      return(XDC_NOK);
   }

   /* Lecture du repertoire */
   while ( ( pl_unFic = readdir(pl_repert) ) != NULL )
   {
      if ( *pl_unFic->d_name != '.' )
      {
         /* Execution de la procedure passee en argument */
         
         sprintf(pl_NomFichier, "%s/%s", pa_nom_repertoire, pl_unFic->d_name);
         (*fa_proc_repertoire)(pl_NomFichier);
      }
   }   
   /* Fermeture du repertoire */
   closedir(pl_repert);
   
   return(XDC_OK);
} 

/*X*/
/*--------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Lancement d'un timer une fois  
*
----------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int lancement_timer_eqtx( int va_duree, char *va_cle)

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
   static char *version = "$Id: eqtx_don.c,v 1.4 2018/10/26 15:16:44 pc2dpdy Exp $ : lancement_timer_eqtx";
   int vl_jourSemaine;
   double vl_Horodate;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   
   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
   ETIM_PROG_REVEIL( vl_Horodate+(double)va_duree, va_cle, vg_SockTIM, pl_MsgTimer)
   
   return(XDC_OK);
}
