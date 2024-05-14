/*E*/
/*Fichier : $Id: edai_adt.c,v 1.2 2007/11/20 08:35:05 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/11/20 08:35:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_adt.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion alerte DAI Tunnels de la tache TEDAI.
* 
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Nov 1996	: Creation pour alerte DAI tunnel (DEM/1309)
* JMG   13/11/07        : ajout site de gestion DEM715
*
------------------------------------------------------*/

/* fichiers inclus */

#include "edai_adt.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: edai_adt.c,v 1.2 2007/11/20 08:35:05 pc2dpdy Exp $ : edai_adt" ;
EDAI_CONFIG_ALERTE_DAI_TUNNEL	vg_ConfAlerteTunnel[EDAIC_MAX_ALERTE_TUNNEL];

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_CONF_ALERTE_DAI_TUNNEL_<NomMachine>
*  et met a jour le tableau de config des alertes DAI tunnels
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Alerte_Tunnel	( XDY_NomMachine		pa_NomMachine )
			  
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
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
   static char *version = "$Id: edai_adt.c,v 1.2 2007/11/20 08:35:05 pc2dpdy Exp $ : ec_lire_config_Alerte_Tunnel" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbAnaly = 0;
   int				vl_Index = 0;
   int				vl_NbParam  = 0;
   
  
      for ( vl_Index = 0; vl_Index < EDAIC_MAX_ALERTE_TUNNEL; vg_ConfAlerteTunnel[vl_Index].Analyseur[0] = 0, vl_Index++ );
      
      /*A Ouverture du fichier de configuration des Analyseurs */
      /* Construction du nom du fichier XDF_CONF_ALERTE_DAI_TUNNEL_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_CONF_ALERTE_DAI_TUNNEL, pa_NomMachine );
      
      /* Ouverture du fichier XDF_CONF_ALERTE_DAI_TUNNEL_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Pas de fichier de parametrage des alertes DAI Tunnels (%s) \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( (ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF) &&
              (vl_NbAnaly < EDAIC_MAX_ALERTE_TUNNEL) ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*B Extraire les parametre de l'analyseur */

             vl_NbParam = sscanf ( pl_LigneFich, "%hd\t%hd\t%s\t%hd\t%hd\t%hd\t%d\t%hd\t%[^\t]\n",  
					&vg_ConfAlerteTunnel[vl_NbAnaly].Analyseur[0],
					&vg_ConfAlerteTunnel[vl_NbAnaly].Analyseur[1],
					vg_ConfAlerteTunnel[vl_NbAnaly].LT,
					&vg_ConfAlerteTunnel[vl_NbAnaly].Mot,
					&vg_ConfAlerteTunnel[vl_NbAnaly].Bit,
					&vg_ConfAlerteTunnel[vl_NbAnaly].Etat,
					&vg_ConfAlerteTunnel[vl_NbAnaly].Duree,
					&vg_ConfAlerteTunnel[vl_NbAnaly].Alerte,
					vg_ConfAlerteTunnel[vl_NbAnaly].Libelle );
             
             /* Si tout les parametre ont ete lu */
             if ( vl_NbParam == 9 )
             {
                 /* Incrementation du nombre d'analyseur */
                 vg_ConfAlerteTunnel[vl_NbAnaly].EtatAlerte[0] = XDC_FAUX;
                 vg_ConfAlerteTunnel[vl_NbAnaly].EtatAlerte[1] = XDC_FAUX;
                 vl_NbAnaly++;
             }
             else
	        XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur lecture ligne fichier <%s> <%s>", pl_PathEtFic, pl_LigneFich );
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( vl_Index = 0; ((vl_Index < EDAIC_MAX_ALERTE_TUNNEL) && (vg_ConfAlerteTunnel[vl_Index].Analyseur[0] != 0)) ; vl_Index++ )
      {
	 XZST_03EcritureTrace ( XZSTC_WARNING, "Config alerte DAI tunnel: Cam1:%hd Cam2:%hd LT:%s Mot:%hd Bit:%hd Etat:%hd Duree:%d Alerte:%d Lib:%s",  
					vg_ConfAlerteTunnel[vl_Index].Analyseur[0],
					vg_ConfAlerteTunnel[vl_Index].Analyseur[1],
					vg_ConfAlerteTunnel[vl_Index].LT,
					vg_ConfAlerteTunnel[vl_Index].Mot,
					vg_ConfAlerteTunnel[vl_Index].Bit,
					vg_ConfAlerteTunnel[vl_Index].Etat,
					vg_ConfAlerteTunnel[vl_Index].Duree,
					vg_ConfAlerteTunnel[vl_Index].Alerte,
					vg_ConfAlerteTunnel[vl_Index].Libelle );
	 
      }			
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
      
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction detecte si une alerte DAI tunnel doit etre envoyer
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_detect_Alerte_DAI_Tunnel( EDAI_DONNEES_ANALYSEUR *	pa_DonneeAnalyseur,
				  XDY_Horodate			va_Horodate  )
			  
/*
* ARGUMENTS EN ENTREE :
*  pa_DonneeAnalyseur 	: Etat de l'analyseur
*  va_Horodate       	: Horodate
*
* ARGUMENTS EN SORTIE :
*   aucun
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
   static char *version = "$Id: edai_adt.c,v 1.2 2007/11/20 08:35:05 pc2dpdy Exp $ : ec_detect_Alerte_DAI_Tunnel" ;

   int				vl_Index = 0;
   int				vl_Analyseur = 0;
   XDY_Datagroup                pl_DG;
   
  
      for ( vl_Index = 0; ((vl_Index < EDAIC_MAX_ALERTE_TUNNEL) && (vg_ConfAlerteTunnel[vl_Index].Analyseur[0] != 0)) ; vl_Index++ )
      {
         vl_Analyseur = XDC_FAUX;
         if ( pa_DonneeAnalyseur->Config.NoAnaly == vg_ConfAlerteTunnel[vl_Index].Analyseur[0] )
         {
            vg_ConfAlerteTunnel[vl_Index].EtatAlerte[0] = pa_DonneeAnalyseur->Alerte;
            vl_Analyseur = XDC_VRAI;
         }
         if ( pa_DonneeAnalyseur->Config.NoAnaly == vg_ConfAlerteTunnel[vl_Index].Analyseur[1] )
         {
            vg_ConfAlerteTunnel[vl_Index].EtatAlerte[1] = pa_DonneeAnalyseur->Alerte;
            vl_Analyseur = XDC_VRAI;
         }
      
         if ( vl_Analyseur && 
              vg_ConfAlerteTunnel[vl_Index].EtatAlerte[0] && 
              vg_ConfAlerteTunnel[vl_Index].EtatAlerte[1] )
         {
            /* Si apparation d'alerte alors envoi alerte pilotage panneau tete de tunnel */
            ex_env_alerte  ( 		va_Horodate,
                             		XDC_EQT_ANA,
                             		pa_DonneeAnalyseur->Config.Numero,
                             		vg_ConfAlerteTunnel[vl_Index].Alerte,
                             		XZEXC_MODE_NORMAL,
					pa_DonneeAnalyseur->Config.SiteGestion,
                             		vg_ConfAlerteTunnel[vl_Index].Libelle,
                             		NULL );

            /* et envoi commande pilotage tete de tunnel via mini GTC */
     	    XDG_EncodeDG2 ( pl_DG, XDG_ECMD_GTC,vg_ConfAlerteTunnel[vl_Index].LT );
     	    if (!TipcSrvMsgWrite ( 	pl_DG,
      			           	XDM_IdentMsg(XDM_ECMD_GTC),
                          	   	XDM_FLG_SRVMSGWRITE,
               			   	T_IPC_FT_INT2,vg_ConfAlerteTunnel[vl_Index].Mot,
               			   	T_IPC_FT_INT2,vg_ConfAlerteTunnel[vl_Index].Bit,
               			   	T_IPC_FT_INT2,vg_ConfAlerteTunnel[vl_Index].Etat,
               			   	T_IPC_FT_INT4,vg_ConfAlerteTunnel[vl_Index].Duree,
                          	   	NULL))
      	    {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ECMD_GTC non effectue sur %s" ,pl_DG );
	    }
	    else
      	    {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ECMD_GTC avec Mot:%d Bit:%d Etat:%d Duree:%d effectue sur %s" ,
               			   	vg_ConfAlerteTunnel[vl_Index].Mot,
               			   	vg_ConfAlerteTunnel[vl_Index].Bit,
               			   	vg_ConfAlerteTunnel[vl_Index].Etat,
               			   	vg_ConfAlerteTunnel[vl_Index].Duree,
         				pl_DG );
	    }
	 }
     }
      
}

