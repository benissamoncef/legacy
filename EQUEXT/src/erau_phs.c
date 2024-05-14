/*E*/
/*  Fichier : $Id: erau_phs.c,v 1.8 2010/12/07 12:07:02 pc2dpdy Exp $      Release : $Revision: 1.8 $       Date : $Date: 2010/12/07 12:07:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_phs.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module traitement des alertes PAU HS tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	28 Oct 1996	: Creation pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.2	19 Nov 1996	: Modif pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.3	05 Dec 1996	: Suite modif pour Alerte PAU HS (DEM/1308)
* Mismer.D	version 1.4	10 Dec 1996	: Correction Line Feed dans texte alerte pour Alerte PAU HS (DEM/1308)
* Diaz.L  	version 1.5    09 Oct 1998     : Modif du au PC Niv.2
* JMG   13/11/07        : ajout site de gestion DEM715 version 1.7
* JMG	26/06/10	: portage Linux DEM/934
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "erau_phs.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: erau_phs.c,v 1.8 2010/12/07 12:07:02 pc2dpdy Exp $ $Revision: 1.8 $ $Date: 2010/12/07 12:07:02 $ : erau_phs" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



                  
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Lecture du  fichier liste PAU pour alerte PAU HS
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_fichier_info_PAU ( void )

/*
* ARGUMENTS EN ENTREE :
*   va_Eqt : Info PAU a ajouter 
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
*   rien 
*
* FONCTION 
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
    XZEXT_CONFIG_PAU	  *pl_PAU = NULL, *pl_PAUALiberer = NULL, *pl_AncPAU = NULL, vl_PAU;
    ERAU_CONFIG_VOISIN	  *pl_Voisin;
    char 		  pl_NomFichier[XDC_PATH_ABS_SIZE];
    FILE		  *vl_Fd = NULL;
    XDY_Texte		  pl_Ligne = "";
    int			  vl_NbParam = 0;
    int			  vl_Existe = XDC_FAUX;
    XDY_NomMachine	  vl_NomMachine = "";
    XDY_Datagroup	  pl_NomDG = "";

   /* Lecture du fichier de config des PAU pour alerte PAU HS */
   sprintf ( pl_NomFichier, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_PAU, vg_NomMachine );
   if ( ( vl_Fd = fopen ( pl_NomFichier, "r" ) ) == NULL )
   {
       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
       return ( XDC_NOK );
   }
   for ( pl_PAU = pg_DebInfoPAU; pl_PAU != NULL; free(pl_PAUALiberer) )
   {
      pl_PAUALiberer = pl_PAU;
      pl_PAU         = pl_PAU->Suivant;
   }  
   pg_DebInfoPAU = NULL;
   
   vg_Config_PAU_HS.Nb_PAU = 0;
   /*A Lecture du fichier integralement */
   while ( fgets ( pl_Ligne, sizeof(pl_Ligne), vl_Fd ) != NULL )
      if ( pl_Ligne[0] != XDF_COMMENTAIRE ) 
      {
    	 vl_NbParam = sscanf ( pl_Ligne,"%[^,],\t%hd,\t%[^,],\t%[^,],\t%hd,\t%d,\t%hd,\n",
    					vl_PAU.LibelleEqt,
    					&vl_PAU.NumeroEqt,
    					vl_PAU.Maitre,
    					vl_PAU.NomEqt,
    					&vl_PAU.NumeroAutoroute,
    					&vl_PAU.PR,
    					&vl_PAU.Sens );
         if ( vl_NbParam == 7 )
         {
            /*A Allocation memoire pour la nouvelle structure */
            vl_PAU.Alerte = XDC_FAUX;
            vl_PAU.Alarme = XDC_FAUX;
            
            if ( ( pl_PAU = (XZEXT_CONFIG_PAU *) malloc ( sizeof(XZEXT_CONFIG_PAU) ) ) == NULL)
            {
               /* Ecriture trace */ 
               XZST_03EcritureTrace ( XZSTC_WARNING, " ec_ajouter_info_PAU OUT: Allocation memoire impossible");
               fclose ( vl_Fd );         
               return XDC_NOK;
            }
            memcpy ( pl_PAU, &vl_PAU, sizeof(XZEXT_CONFIG_PAU) );
   
            /*A Ajout de la structure en fin de liste */
            pl_PAU->Suivant = NULL;
            if ( pl_AncPAU == NULL)
               pg_DebInfoPAU = pl_PAU;
            else
               pl_AncPAU->Suivant = pl_PAU;
            pl_AncPAU = pl_PAU;
            
            if ( strcmp(vl_PAU.Maitre, vg_NomMachine) )
            {
               for ( pl_Voisin = pg_DebListeVoisin, vl_Existe = XDC_FAUX; pl_Voisin != NULL; pl_Voisin = pl_Voisin->Suivant )
                  if ( !strcmp(vl_PAU.Maitre, pl_Voisin->Machine) ) vl_Existe = XDC_VRAI;  
               if ( !vl_Existe )
               {
                  /*A Allocation memoire pour la nouvelle structure */
                  if ( ( pl_Voisin = (ERAU_CONFIG_VOISIN *) malloc ( sizeof(ERAU_CONFIG_VOISIN) ) ) == NULL)
                  {
                      /* Ecriture trace */ 
                      XZST_03EcritureTrace ( XZSTC_WARNING, " ec_ajouter_info_PAU OUT: Allocation memoire impossible");
                      fclose ( vl_Fd );         
                      return XDC_NOK;
                   }
                   /*A Ajout de la structure en tete de liste */
                   strcpy ( pl_Voisin->Machine, vl_PAU.Maitre );
                   pl_Voisin->Suivant = pg_DebListeVoisin;
                   pg_DebListeVoisin  = pl_Voisin;
               }  
            }
            else
              vg_Config_PAU_HS.Nb_PAU++;
         }
      	 else
      	    XZST_03EcritureTrace(XZSTC_WARNING, "Ligne <%s> incorrecte " ,pl_Ligne );
      }
   fclose ( vl_Fd );    
            
   /* Lecture du fichier de config des alertes PAU HS */
   sprintf ( pl_NomFichier, "%s/%s", XZEXC_PATH_CONFIG, XDF_CONF_ALERTE_PAU_HS );
   if ( ( vl_Fd = fopen ( pl_NomFichier, "r" ) ) == NULL )
   {
       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_NomFichier ); 
       return ( XDC_NOK );
   }
   
   /*A Lecture du fichier integralement */
   while ( fgets ( pl_Ligne, sizeof(pl_Ligne), vl_Fd ) != NULL )
      if ( pl_Ligne[0] != XDF_COMMENTAIRE ) 
      {
    	 vl_NbParam = sscanf ( pl_Ligne,"%s\t%hd\t%hd\t%[^\t]\t%hd\t%hd\t%[^\t]\n",
    					vl_NomMachine,
    					&vg_Config_PAU_HS.Nb_PAU_Consecutif,
    					&vg_Config_PAU_HS.Num_Alt_Consecutif,
    					vg_Config_PAU_HS.Texte_Alt_Consecutif,
    					&vg_Config_PAU_HS.Nb_PAU_HS,
    					&vg_Config_PAU_HS.Num_Alt_HS,
    					vg_Config_PAU_HS.Texte_Alt_HS );
         if ( (vl_NbParam == 7) && (!strcmp(vl_NomMachine, vg_NomMachine)) )
         {
            strcpy ( vg_Config_PAU_HS.Texte_Alt_HS, strtok ( vg_Config_PAU_HS.Texte_Alt_HS, "\t\n\r") );
            XZST_03EcritureTrace ( XZSTC_WARNING, "Lecture config alerte PAU OK : %d/%d/%s/%d/%d/%s", 
    					vg_Config_PAU_HS.Nb_PAU_Consecutif,
    					vg_Config_PAU_HS.Num_Alt_Consecutif,
    					vg_Config_PAU_HS.Texte_Alt_Consecutif,
    					vg_Config_PAU_HS.Nb_PAU_HS,
    					vg_Config_PAU_HS.Num_Alt_HS,
    					vg_Config_PAU_HS.Texte_Alt_HS );
            break;
         }
      }
   fclose ( vl_Fd );    

  if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_PAU , 0xFF , XDG_ECMD_RAU ) == XDC_NOK )
  {
     XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PAU" );
  }
  
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :ec_lire_fichier_info_PAU");
  return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*  Reception des etats des alarmes des PAU
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_Etat_PAU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_IETAT_DISPO_EQT
*
* FONCTION 
*  Memo des etats des alarmes des PAU 
*
------------------------------------------------------*/
{
static char *version = "@(#)erau_phs.c	1.4 12/10/96 : er_Etat_PAU_cb " ;
   XZEXT_CONFIG_PAU	  *pl_PAU;
#ifdef _TIBCO_EMS
   XDY_Entier		  va_Etat_PAU;
   XDY_Mot		  va_Type;
   XDY_Mot		  va_Num_PAU;
#else
   T_INT4		  va_Etat_PAU;
   T_INT2		  va_Type;
   T_INT2		  va_Num_PAU;
#endif
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s", version );
		        		  
   /*A Recuperation des parametres du message XDM_IETAT_DISPO_EQT */
   if( !TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT2, &va_Type, 
                     T_IPC_FT_INT2, &va_Num_PAU, 
                     T_IPC_FT_INT4 ,&va_Etat_PAU, 
	             NULL) )
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"%s : messsage XDM_IETAT_DISPO_EQT non lu", version );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: %s", version );
      return;
   }

   XZST_03EcritureTrace(XZSTC_WARNING, "IN : %s  PAU:%d Etat:%d", version, va_Num_PAU, va_Etat_PAU );
      
   /* MAJ de l'etat des PAU */ 
   for ( pl_PAU = pg_DebInfoPAU; pl_PAU != NULL; pl_PAU = pl_PAU->Suivant )
     if ( va_Num_PAU == pl_PAU->NumeroEqt )
     {
         pl_PAU->Alarme = (va_Etat_PAU & XDC_EQT_HS) ? XDC_EQT_HS : (va_Etat_PAU & XDC_EQT_MAJEUR ) ? XDC_VRAI : XDC_FAUX;
         break;
     }
            	        
   ec_Detect_Alerte_PAU ();             
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: %s", version );
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Recherche la presence de n PAU consecutif en alarme et 
*  du nombre de PAU HS total puis envoi des alerte correspondante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_Detect_Alerte_PAU ( void )

/*
* ARGUMENTS EN ENTREE :
*   aucun 
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
*   rien 
*
* FONCTION 
*  Recherche la presence de n PAU consecutif en alarme et 
*  du nombre de PAU HS total puis envoi des alerte correspondante
*
------------------------------------------------------*/
{
static char *version = "@(#)erau_phs.c	1.4 12/10/96 : ec_Detect_Alerte_PAU " ;
    XZEXT_CONFIG_PAU	  *pl_PAU, *pl_AncPAU, *pl_DebPAU, *pl_TmpPAU;
    ERAU_CONFIG_VOISIN	  *pl_Voisin;
    int			  vl_Index = 0;
    int			  vl_CptAlarme = 0;
    int			  vl_CptAlerte = 0;
    int			  vl_Local = 0;
    int			  vl_CptAlarmeConsecutive = 0;
    XDY_Texte		  vl_TexteAlerte = "";
    XDY_Horodate	  vl_horodate;
    int			  vl_js;
    FILE *		  vl_fd;
    
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s", version );
    
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );
   for ( pl_PAU = pl_AncPAU = pg_DebInfoPAU; pl_PAU != NULL; pl_AncPAU = pl_PAU, pl_PAU = pl_PAU->Suivant )
   {
      /* Si le PAU est hors service on passe au suivant */
      if ( pl_PAU->Alarme == XDC_EQT_HORS_SRV ) continue;
      
      /* RAZ alerte si disparition alarme */
      if ( pl_PAU->Alerte &&
           (pl_PAU->Sens == pl_AncPAU->Sens) && (pl_PAU->NumeroAutoroute == pl_AncPAU->NumeroAutoroute) )
      {
         if ( !vl_CptAlarmeConsecutive ) pl_DebPAU = pl_PAU;
         vl_CptAlarmeConsecutive++;
      }
      if ( !pl_PAU->Alarme || !pl_PAU->Alerte || (pl_PAU->Suivant == NULL) ||
           (pl_PAU->Sens != pl_AncPAU->Sens) || (pl_PAU->NumeroAutoroute != pl_AncPAU->NumeroAutoroute) )
      {
         if ( vl_CptAlarmeConsecutive < vg_Config_PAU_HS.Nb_PAU_Consecutif )
         {
            for ( pl_TmpPAU = pl_DebPAU, vl_Index = 0; vl_Index < vl_CptAlarmeConsecutive; vl_Index++ )
            {
               pl_TmpPAU->Alerte = XDC_FAUX;
               pl_TmpPAU = pl_TmpPAU->Suivant;
            }
         }
         vl_CptAlarmeConsecutive = 0;
      }
   }
      
   vl_CptAlarmeConsecutive = 0;
   for ( pl_PAU = pl_AncPAU = pg_DebInfoPAU; pl_PAU != NULL; pl_AncPAU = pl_PAU, pl_PAU = pl_PAU->Suivant )
   {
      /* Si le PAU est hors service on passe au suivant */
      if ( pl_PAU->Alarme == XDC_EQT_HORS_SRV ) continue;
      
      /* Si alarme en cours incrementation du nombre d'alarme */
      if ( !strcmp(pl_PAU->Maitre, vg_NomMachine) && pl_PAU->Alarme ) vl_CptAlarme++;
      
      /* Si presence alarme et Sens et autoroute identique au precedent incrementation du nombre d'alarme consecutives */
      if ( pl_PAU->Alarme &&
           (pl_PAU->Sens == pl_AncPAU->Sens) && (pl_PAU->NumeroAutoroute == pl_AncPAU->NumeroAutoroute) )
      {
         if ( !vl_CptAlarmeConsecutive ) pl_DebPAU = pl_PAU;
         vl_CptAlarmeConsecutive++;
      }
      /* Sinon pas alarme alors RAZ compteur d'alarme consecutives */
      if ( !pl_PAU->Alarme || (pl_PAU->Suivant == NULL) ||
           (pl_PAU->Sens != pl_AncPAU->Sens) || (pl_PAU->NumeroAutoroute != pl_AncPAU->NumeroAutoroute) )
      {
         /* Si compteur d'alarme consecutives superieur au seuil alors envoi alerte */
         if ( vl_CptAlarmeConsecutive >= vg_Config_PAU_HS.Nb_PAU_Consecutif )
         {
            for ( pl_TmpPAU = pl_DebPAU, vl_CptAlerte = 0, vl_Local = XDC_FAUX, vl_Index = 0;
                  vl_Index < vl_CptAlarmeConsecutive; 
                  vl_Index++ )
            {
               if ( !pl_TmpPAU->Alerte ) vl_CptAlerte++;  
               if ( !strcmp(pl_TmpPAU->Maitre,vg_NomMachine) ) vl_Local = XDC_VRAI;
               pl_TmpPAU = pl_TmpPAU->Suivant;
            }
            if ( vl_Local && (vl_CptAlerte >= vg_Config_PAU_HS.Nb_PAU_Consecutif ) )
            {
               for ( pl_TmpPAU = pl_DebPAU, vl_Index = 0;; )
               {
                  pl_TmpPAU->Alerte = XDC_VRAI;
                  if ( ++vl_Index < vl_CptAlarmeConsecutive)
                     pl_TmpPAU = pl_TmpPAU->Suivant;
                  else
                     break;
               }
               sprintf ( vl_TexteAlerte, vg_Config_PAU_HS.Texte_Alt_Consecutif, 
               				 pl_DebPAU->LibelleEqt, 
            				 pl_TmpPAU->LibelleEqt );
               ex_env_alerte  (  vl_horodate,
                                 XDC_EQT_PAU,
				 pl_DebPAU->NumeroEqt,
                                 vg_Config_PAU_HS.Num_Alt_Consecutif,
                                 vg_Mode_Fonct,
				 XZSC_22NumeroSiteLocal(),
                                 vl_TexteAlerte,
				 NULL);
            }
         }
         vl_CptAlarmeConsecutive = 0;
      }
   }
   
   /* Si le nombre total de PAU HS du district est superieur au seuil critique alors envoi alerte correspondante */
   XZST_03EcritureTrace(XZSTC_INFO, "PAU HS:%d seuil:%d",vl_CptAlarme, vg_Config_PAU_HS.Nb_PAU_HS );
   if ( (vl_CptAlarme > vg_Config_PAU_HS.Nb_PAU_HS) && !vg_Config_PAU_HS.Alerte )
   {
      ex_env_alerte  (     vl_horodate,
                           XDC_EQT_RAU,
			   vg_donsysrau.Config.Numero,
                           vg_Config_PAU_HS.Num_Alt_HS,
                           vg_Mode_Fonct,
			   XZSC_22NumeroSiteLocal(),
                           vg_Config_PAU_HS.Texte_Alt_HS,
			   NULL);
      vg_Config_PAU_HS.Alerte = XDC_VRAI;
   }
   
   if ( vl_CptAlarme < vg_Config_PAU_HS.Nb_PAU_HS ) vg_Config_PAU_HS.Alerte = XDC_FAUX;

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s", version );
  
   return ( XDC_OK );       
}
