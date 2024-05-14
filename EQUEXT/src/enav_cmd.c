/*E*/
/*Fichier : $Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $      Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 18:09:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le modules des callbacks
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	22 Nov 1994	: Creation
* Nagiel.E	version 1.2	21 Dec 1994	:
* Nagiel.E	version 1.3	21 Dec 1994	: RAS
* Mismer.D	version 1.4	23 Jan 1995	:
* Mismer.D	version 1.5	21 Fev 1995	:
* Mismer.D 	version 1.6	08 Mar 1995	:
* Mismer.D	version 1.7	17 Mar 1995	:
* Volcic.F	Version 1.8	04 Mai 1995	:
* Volcic.F	Version 1.9	10 Mai 1995	:
* Fontaine.C	Version 1.10	18 Mai 1995	: Reprise historique
* Mismer.D 	version 1.11	16 Aou 1995	: Ajout ecriture fichier trace commande
* Volcic.F	Version 1.12	?? ??? 1995	:
* Mismer.D 	version 1.13	02 Nov 1995	: Correction traitement cmd matrice
* Mismer.D 	version 1.14	28 Nov 1995	: Modif envoi etat matrice vers IHM
* Mismer.D 	version 1.15	18 Dec 1995	: Modif traces
* Mismer.D 	version 1.16	23 Mai 1996	: Suppression cmd EC= apres cmd scenario (DEM/1136)
* Mismer.D      version 1.17    11 Sep 1996     : Ajout periode variable en fonction NB eqt (RADT_W)
* Mismer.D	version 1.18	10 Jan 1997	: Modif pour matrice BRA (DEM/1419)
* Mismer.D	version 1.19	10 Jan 1998	: Modif pour FAU et FAC (DEM/1544)
* Cluchague	version 1.20	28 Dec 2004	: Plus de traces
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	27/09/19	: envoie ST apr�s refus commande v1.22
* JMG	14/05/20 : EMS 1.23
* GR    09/07/2020 : EMS 1.24
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "enav_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM nav.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_contraste_cb ( 
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
*  XDM_EInit_IHM_Contraste 
*
* FONCTION 
*  Pour toutes les nav , envoyer l'etat  par XDM_ETAT_Contraste dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_Init_IHM_contraste_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup    pl_DGInit;
   #endif   

   ENAV_DONNEES_NAV *		pl_listeNAV;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_Init_IHM_contraste_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_Contraste */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR, &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_Init_IHM_contraste_cb messsage XDM_EInit_IHM_Contraste non lu." );
      return;
   }
	        
   /*A Pour chaque nav : envoi du msg XDM_ETAT_Contraste si il est en service et actif */
  
   for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XZECC_TYPE_EQT_NAV,
                          	T_IPC_FT_INT2, pl_listeNAV->Config.Numero,
                          	T_IPC_FT_INT2, (pl_listeNAV->ContrasteEtat == 'S') ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_Contraste non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_Contraste effectue." );
	 }
      }	
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_Init_IHM_contraste_cb ");
	       
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM Liaison.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_Init_IHM_Liaison_cb ( 
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
*  XDM_EInit_IHM_Liaison
*
* FONCTION 
*  Pour touts les NAV , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup    pl_DGInit;
   #endif

   ENAV_DONNEES_NAV *		pl_ListeNAV;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");
		  
   /*A Recuperation des parametres du message  */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ep_Init_IHM_Liaison_cb messsage XDM_EInit_IHM_Liaison non lu \n" );
      return;
   }
	        
   /*A Pour chaque NAV : envoi du msg XDM_ETAT_LIAISON  */
  
   for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL; pl_ListeNAV = pl_ListeNAV->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListeNAV->Config.Numero, XDC_EQT_NAV ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM NAV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_NAV_cb ( 
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
*  XDM_EInit_IHM_NAV 
*
* FONCTION 
*  Pour touts les NAV, envoyer l'etat NAV par XDM_ETAT_NAV dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : ep_Init_IHM_NAV_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   


   ENAV_DONNEES_NAV *		pl_ListeNAV;
   int i;
   char **pl;
#ifndef _TIBCO_EMS
   static char *			pl_Tab[XZEXC_nb_module_max];
#else
   XDY_Nom 		pl_Tab[XZEXC_nb_module_max];
#endif
   char                 vl_str[1024];

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_NAV_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_NAV */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: ep_Init_IHM_NAV_cb messsage XDM_EInit_IHM_NAV non lu." );
      return;
   }
	        
	       
   /*A Pour chaque NAV : envoi du msg XDM_ETAT_NAV */
  
   for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL; pl_ListeNAV = pl_ListeNAV->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_ListeNAV->Etat_Service & XDC_EQT_HORS_SRV) )
      {
         XZST_03EcritureTrace(XZSTC_DEBUG1,"CLUCHAGUE nav no:%d",pl_ListeNAV->Config.Numero);
	      strcpy(vl_str,"");
         for (i=0;i < XZEXC_nb_module_max;i++)
         {        
                if (strcmp(vl_str,""))
                        sprintf(vl_str,"%s|%s",vl_str, pl_ListeNAV->Etat.Module[i]);
                else
                        sprintf(vl_str,"%s",pl_ListeNAV->Etat.Module[i]);
#ifndef _TIBCO_EMS
            pl_Tab[i] = pl_ListeNAV->Etat.Module[i];
#else
            strcpy(pl_Tab[i] , pl_ListeNAV->Etat.Module[i]);
#endif

            XZST_03EcritureTrace(XZSTC_DEBUG1,"CLUCHAGUE etat module no:%d=%s",i,pl_Tab[i]);
         }
         
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_NAV),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Type,
                          	T_IPC_FT_STR_ARRAY ,  pl_Tab , XZEXC_nb_module_max,
#ifdef _TIBCO_EMS
				XDC_Lon_Nom,
#endif
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_NAV non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_NAV effectue no:%d type:%d nb modules:%d ",pl_ListeNAV->Config.Numero,
		pl_ListeNAV->Config.Type,XZEXC_nb_module_max);
	 }
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_NAV_EMS),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Type,
                          	T_IPC_FT_STR , vl_str,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_NAV_EMS non effectue. vl_str=[%s]", vl_str );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_NAV effectue no:%d type:%d nb modules:%d vl_str=[%s]",pl_ListeNAV->Config.Numero,
		pl_ListeNAV->Config.Type,XZEXC_nb_module_max, vl_str);
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_NAV_cb ");
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM  Matrice NAV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_MAT_NAV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
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
*  XDM_EInit_IHM_MAT_NAV 
*
* FONCTION 
*  Pour touts la NAV concernee, envoyer l'etat matrice NAV par XDM_ETAT_MAT_NAV dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_Init_IHM_MAT_NAV_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit = "";
   T_INT2			vl_NumNAV   = 0;
   #else
   XDY_Datagroup                 pl_DGInit = "";
   XDY_Mot			vl_NumNAV   = 0;
   #endif


   ENAV_DONNEES_NAV *		pl_ListeNAV;
   int 				vl_index    = 0,
   				vl_indexMat = 0;
   T_INT2			pl_Tab[XDC_Lon_Params_Matrice];

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_Init_IHM_MAT_NAV_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_MAT_NAV */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
               T_IPC_FT_INT2, &vl_NumNAV,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_Init_IHM_MAT_NAV_cb messsage XDM_EInit_IHM_MAT_NAV non lu." );
      return;
   }
	        
	       
   /*A Pour la NAV concerne : envoi du msg XDM_ETAT_MAT_NAV */
   for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL; pl_ListeNAV = pl_ListeNAV->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( (pl_ListeNAV->Config.Numero == vl_NumNAV) && !(pl_ListeNAV->Etat_Service & XDC_EQT_HORS_SRV) )
      {
         /*A Envoi de la trame demande etat matrice (CFMT) */
         if (EnvoyerTrame(    	"CFMT",
        			XZEXC_TRAME_MAT_NAV,
        			pl_ListeNAV->Config.AdresseRGS,
		 		XZEXC_NOCMD,
		 		XZEXC_PRIORITEFAIBLE,
        			pl_ListeNAV->Socket) == XDC_NOK ) 
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande NAV" );
         }
         break;
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_Init_IHM_MAT_NAV_cb ");
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Emission Etat  Matrice NAV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Emission_MAT_NAV ( ENAV_DONNEES_NAV *	pa_ListeNAV, 
			      XDY_Datagroup		va_DgDest )
 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_Emission_MAT_NAV" ;

   T_INT2			pl_Tab[XDC_Lon_Params_Matrice];
   int 				vl_index    = 0,
   				vl_indexMat = 0;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : en_Emission_MAT_NAV_cb ");
		  
   for ( vl_indexMat = 0; vl_indexMat < ENAVC_NB_MATRICE; vl_indexMat++ )
   {
      for (vl_index = 0; vl_index < XDC_Lon_Params_Matrice; pl_Tab[vl_index] = 0, vl_index++);
      for (vl_index = 0; vl_index < XDC_Lon_Params_Matrice; vl_index++)
         if ( (pa_ListeNAV->Matrice.Conf[vl_indexMat][vl_index] > 0) && (pa_ListeNAV->Matrice.Conf[vl_indexMat][vl_index] <= XDC_Lon_Params_Matrice) )
            pl_Tab[(pa_ListeNAV->Matrice.Conf[vl_indexMat][vl_index])-1] = 1;
      if (!TipcSrvMsgWrite ( va_DgDest,
      		   	     XDM_IdentMsg(XDM_ETAT_MAT_NAV),
                             XDM_FLG_SRVMSGWRITE,
                             T_IPC_FT_INT2, pa_ListeNAV->Config.Numero,
                             T_IPC_FT_STR,  pa_ListeNAV->Matrice.Nom[vl_indexMat],
                             T_IPC_FT_INT2_ARRAY ,  pl_Tab , XDC_Lon_Params_Matrice,
                             NULL))
      {
         XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_MAT_NAV non effectue." );
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_MAT_NAV effectue." );
      }
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_Emission_MAT_NAV");
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_NAV_cb ( 
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
*
* Fonction executee sur reception du message XDM_ECMD_NAV
*
* FONCTION 
*  Piloter le module concern�
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_cmd_NAV_cb" ;
   
#ifndef _TIBCO_EMS   
   T_STR			vl_Etat;
   T_INT2			vl_NoEqtNAV;
   T_INT2			vl_Module;
   T_INT2			vl_Flash;
   T_INT4			vl_NoCMD;
#else
   XDY_Etat_Module              vl_Etat;
   XDY_Mot			vl_NoEqtNAV;
   XDY_Mot			vl_Module;
   XDY_Mot			vl_Flash;
   XDY_Entier			vl_NoCMD;
#endif

   ENAV_DONNEES_NAV *	  	pl_listeNAV;   
   int				vl_Index,vl_js;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate			vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_NAV_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_NAV */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtNAV
                          ,T_IPC_FT_INT2, &vl_Module
                          ,T_IPC_FT_STR,  &vl_Etat
                          ,T_IPC_FT_INT2, &vl_Flash
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_cmd_NAV_cb messsage XDM_ECMD_NAV non lu." );
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd nav = %d %d %s %d %d"
      				     ,vl_NoEqtNAV
      				     ,vl_Module
      				     ,vl_Etat
      				     ,vl_Flash
      				     ,vl_NoCMD);
      
   }
   
   
   /*A Verif des parametres en entree */
   
   for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
   {
      if ( pl_listeNAV->Config.Numero == vl_NoEqtNAV) break;
   }
   
   /* Numero eqt nav invalide */
   if (pl_listeNAV == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_NAV_cb Message XDM_ECMD_NAV recu avec no eqt nav invalide");
      return;
   }
   
   /*A si cmd en cours refus de la commande */
   if ( (*pl_listeNAV->Etat.Scenario != '\0') || ( pl_listeNAV->EtatCmd.Module[vl_Module][0] != '\0') )
   {
      /* Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande cmd refusee car commande en cours (%s,%s) %d",pl_listeNAV->Etat.Scenario,pl_listeNAV->EtatCmd.Module[vl_Module],pl_listeNAV->Config.Numero );

      EnvoyerTrame("ST\0",
                XZEXC_TRAME_STAT_COMP,
        	pl_listeNAV->Config.AdresseRGS,
		XZEXC_NOCMD,
		XZEXC_PRIORITEFAIBLE,
		pl_listeNAV->Socket);

      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_NAV_cb ");
      return;
   }

   /* Verif de l'etat d'activite et de l'etat de service */
   if ( (pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) || 
        (pl_listeNAV->Config.Liste_Module[vl_Module].Etat_Service & XDC_EQT_HORS_SRV))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_NAV_cb : Message NAV ou Module commande invalide");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_NAV_cb ");
      return;
   }


   /*A Recherche si Module valide */
   
   if ((vl_Module >= XZEXC_nb_module_max) || ( pl_listeNAV->Config.Liste_Module[vl_Module].NoModule == 0))
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "eOUT: n_cmd_NAV_cb : Message XDM_ECMD_NAV recu avec No Module nav invalide");
      return;
   }
   
   /*A Construction de ma trame */
   sprintf(pl_mlcr,"P ID=%s AM=%s AF=%s EC=%s",
   			      vg_idpassw,
	                      pl_listeNAV->Config.Liste_Module[vl_Module].Module,
	   	              vl_Etat,
	   	              (pl_listeNAV->Contraste == XDC_CONTRASTEBRILLANT ) ? "S" : ((vg_Contexte == XDC_JOUR) ? "J1" : (vg_Contexte == XDC_NUIT) ? "N1" : "A"));
	   		
   /*A Envoi de la trame */
   if (EnvoyerTrame(    pl_mlcr,
        		XZEXC_TRAME_CMD_NAV,
        		pl_listeNAV->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande NAV" );
      return;
   }

   /* Ecriture dans fichier Trace_CMD_NAV de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                           vl_NoEqtNAV, 
                           pl_mlcr,
                           NULL,
                           vl_Module+1 );
                           
   /*A envoi de la trame de relecture */
   if (EnvoyerTrame(    "PS",
        		XZEXC_TRAME_ETAT_POS_NAV,
        		pl_listeNAV->Config.AdresseRGS,
		 	vl_Module+1,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket) == XDC_NOK ) 
   {
      return;
   }

   
   /* Mise a jour memoire commande NAV */
   strcpy ( pl_listeNAV->EtatCmd.Module[vl_Module], vl_Etat ); 
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_NAV_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_Mat_NAV_cb ( 
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
*
* Fonction executee sur reception du message XDM_ECMD_MAT_SCEN
*
* FONCTION 
*  Piloter le module concern�
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_cmd_Mat_NAV_cb" ;
   
   
   #ifndef _TIBCO_EMS   
   T_STR			vl_Matrice;
   T_INT2			vl_NoEqtNAV;
   #else
   XDY_Texte                      vl_Matrice;
   XDY_Mot			vl_NoEqtNAV;
   #endif


#ifndef _TIBCO_EMS
   T_INT2			*pl_ListeParam;
   T_INT4			vl_Nb_Params_Mat;
   T_INT4			vl_NoCMD;
#else
   XDY_Mot			pl_ListeParam[XZENC_Nb_Params_Matrice];
   XDY_Entier			vl_Nb_Params_Mat;
   XDY_Entier			vl_NoCMD;
#endif
   
   ENAV_DONNEES_NAV *	  	pl_listeNAV;   
   int				vl_Index,vl_js;
   int				vl_mat[XZENC_Nb_Params_Matrice];
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate		        vl_horodate;
   char				vl_paramat[(XZENC_Nb_Params_Matrice+2)*3] = "";
   int				vl_cpt  = 0;
   int				vl_Type = 0;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_Mat_NAV_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_MAT_NAV */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtNAV
                          ,T_IPC_FT_STR,  &vl_Matrice
                          ,T_IPC_FT_INT2_ARRAY,  &pl_ListeParam , &vl_Nb_Params_Mat
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_MAT_NAV non lu." );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Mat_NAV_cb");
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "Commande matrice  = %d %s %d"
      				     ,vl_NoEqtNAV
      				     ,vl_Matrice
      				     ,vl_NoCMD);
      
   }
   
   
   /* recuperation du tableau de config matrice */ 
#ifndef _TIBCO_EMS
   for ( vl_Index = 0;
         (vl_Index < vl_Nb_Params_Mat);
         vl_mat[vl_Index] = *pl_ListeParam, vl_Index++, pl_ListeParam++);
#else
	for ( vl_Index = 0;
		(vl_Index < vl_Nb_Params_Mat);
		vl_Index++)
	vl_mat[vl_Index] = pl_ListeParam[vl_Index];
#endif

   /*A Verif des paramestes en entree */
   
   for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
   {
      if ( pl_listeNAV->Config.Numero == vl_NoEqtNAV) break;
   }
   
   /* Numero eqt nav invalide */
   if (pl_listeNAV == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_Mat_NAV_cb avec no eqt nav invalide");
      return;
   }
    
   /* Verif de l'etat de service */
   if ( pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV )
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_Mat_NAV_cb : Message NAV commandee invalide");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Mat_NAV_cb ");
      return;
   }
   
   /*A si scenario en cours refus de la commande */
   if ( *pl_listeNAV->Etat.Scenario != '\0' )
   {
      /* Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande refusee car scenario en cours" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Mat_NAV_cb");
      return;
   }

   /*A Construction de ma trame */
   if (!strcmp(vl_Matrice,ENAVC_MATRICE_NEUT))
   {
      sprintf(pl_mlcr,"CFMT ID=%s MAT2=%s",vg_idpassw, ENAVC_MATRICE_NEUT );
      vl_Type = 2;
   }
	   		
   if (!strcmp(vl_Matrice,ENAVC_MATRICE_PRES))
   {
      sprintf(pl_mlcr,"CFMT ID=%s MAT1=%s",vg_idpassw, ENAVC_MATRICE_PRES );
      vl_Type = 1;
   }
   
   for ( vl_Index = 0; vl_Index < vl_Nb_Params_Mat ; vl_Index++)
      if (vl_mat[vl_Index] != 0)
      {
         vl_cpt += sprintf(vl_paramat+vl_cpt, "%s%d", ENAVC_CAR_SEP_MATRICE, vl_mat[vl_Index] );
      }
   
   strcat(pl_mlcr,vl_paramat);
   sprintf(pl_listeNAV->Matrice.Param, "%s%s", vl_Matrice, vl_paramat );

   XZST_03EcritureTrace(XZSTC_INFO, "Commande matrice : %s", pl_mlcr);
	   		
   /*A Envoi de la trame */
   if (EnvoyerTrame(pl_mlcr,
        		XZEXC_TRAME_MAT_NAV,
        		pl_listeNAV->Config.AdresseRGS,
		 	XZEXC_NOCMD,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket) == XDC_NOK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande matrice nav" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Mat_NAV_cb");
      return;
   }

   /* Ecriture dans fichier Trace_CMD_NAV de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                           vl_NoEqtNAV, 
                           pl_mlcr,
                           NULL,
                           0 );
                           
   /*A envoi de la trame de relecture */
   if (EnvoyerTrame(    "CFMT",
        		XZEXC_TRAME_MAT_NAV,
        		pl_listeNAV->Config.AdresseRGS,
		 	vl_Type,
			XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket) == XDC_NOK ) 
   {
      return;
   }
   	

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Mat_NAV_cb ");
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande scen nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_SCEN_cb ( 
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
*
* Fonction executee sur reception du message XDM_ECMD_SCEN
*
* FONCTION 
*  Piloter la nav par scenario
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_cmd_SCEN_cb" ;
   
   #ifndef _TIBCO_EMS   
   T_STR			vl_Scenar;
   T_INT2			vl_NoEqtNAV;
   T_INT4			vl_NoCMD;
   #else
   char                 	vl_Scenar[20];
   XDY_Mot			vl_NoEqtNAV;
   XDY_Entier			vl_NoCMD;
   #endif

   ENAV_DONNEES_NAV *	  	pl_listeNAV;   
   ENAV_DONNEES_TYPE_NAV *	pl_listeTypeNAV;   
   ENAV_LISTE_SCEN *		pl_listeSCEN;
   
   int				vl_Index,vl_js,i;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate			vl_horodate;
   XDY_Scenario			vl_Scenario = "";

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_SCEN_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_NAV */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtNAV
                          ,T_IPC_FT_STR,  &vl_Scenar
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_NAV non lu." );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb");
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd nav = %d %s %d"
      				     ,vl_NoEqtNAV
      				     ,vl_Scenar
      				     ,vl_NoCMD);
      
   }
   
   /*A Verif des paramestes en entree */
   
   /* verif no eqt nav */
   for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
   {
      if ( pl_listeNAV->Config.Numero == vl_NoEqtNAV) break;
   }
   
   /* Numero eqt nav invalide */
   if (pl_listeNAV == NULL)
   {
      if ( vl_NoCMD != 0)
      {
         /* message administrateur systeme */
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_js,&vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD,vl_horodate,XDC_NOK,0);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb avec no eqt nav invalide");
      return;
   }
 
   /*A si cmd en cours refus de la commande */
   if ( *pl_listeNAV->Etat.Scenario != '\0' )
   {
      if ( vl_NoCMD != 0)
      {
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_js,&vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD,vl_horodate,XDC_NOK,pl_listeNAV->Config.SiteGestion);
      }
      /* Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande scenario %s refusee car commande en cours (%s) %d",vl_Scenar,pl_listeNAV->Etat.Scenario,pl_listeNAV->Config.Numero );
      EnvoyerTrame("ST\0",
	            XZEXC_TRAME_STAT_COMP,
	            pl_listeNAV->Config.AdresseRGS,
	            XZEXC_NOCMD,
	            XZEXC_PRIORITEFAIBLE,
	            pl_listeNAV->Socket);

      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb");
      return;
   }

   /* Verif de l'etat d'activite et de l'etat de service */
   if ((pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) || 
       ((pl_listeNAV->Etat_Service & XDC_EQT_INACTIF)&&(vl_NoCMD != 0)) ||
       ((pl_listeNAV->Etat_Service & XDC_EQT_INHIBE)&&(vl_NoCMD != 0)) )
   {
      if ( vl_NoCMD != 0)
      {
         /* message administrateur systeme */
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_js,&vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeNAV->Config.SiteGestion);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb commande pour eqt HS ou inactif");
      return;
   }
   
   /* verif no type nav */
   for ( pl_listeTypeNAV = pg_debutTypeNAV; pl_listeTypeNAV != NULL; pl_listeTypeNAV = pl_listeTypeNAV->Suivant )
   {
      if ( pl_listeTypeNAV->Numero == pl_listeNAV->Config.TypeNAV) break;
   }
   
   /* Numero type nav invalide */
   if (pl_listeTypeNAV == NULL)
   {
      if ( vl_NoCMD != 0)
      {
         /* message administrateur systeme */
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_js,&vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeNAV->Config.SiteGestion);
      }
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb avec no type nav invalide");
      return;
   }
 
   
   /* verif de la validite du scenario */
   
   for ( i = 0 ; i != XZEXC_NB_SCENAR_PREDEF ; i++)
   {
      if (!strcmp(vg_CompScenar[i].PseudoScenar,vl_Scenar))
      {
         strcpy(vl_Scenar,vg_CompScenar[i].NomScenar);
         strcpy(vl_Scenario,vg_CompScenar[i].PseudoScenar);
         break;
      } 
   }
   
   if ( i == XZEXC_NB_SCENAR_PREDEF )
   {
      for ( pl_listeSCEN = pl_listeTypeNAV->Liste; pl_listeSCEN != NULL; pl_listeSCEN = pl_listeSCEN->Suivant )
      {
         if ( !strcmp( pl_listeSCEN->Nom_SCEN , vl_Scenar)) break;
      }
   
      /* Numero scenar invalide */
      if (pl_listeSCEN == NULL)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_SCEN_cb avec nom senario invalide");
         if ( vl_NoCMD != 0)
         {
            /* message administrateur systeme */
            XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_js,&vl_horodate);
            xzat24_CompteRendu_Nav(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeNAV->Config.SiteGestion);
         }
         XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb avec nom senario invalide");
         return;
      }
   }
    
   /*A Construction de ma trame */
   sprintf(pl_mlcr,"PM ID=%s AF=%s", vg_idpassw, vl_Scenar);
	   		
   /*A Envoi de la trame */
   if (EnvoyerTrame(pl_mlcr,
        		XZEXC_TRAME_CMD_SCEN_NAV,
        		pl_listeNAV->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket) == XDC_NOK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING ,"Impossible d'envoyer la commande scenario" );
      if ( vl_NoCMD != 0)
      {
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_js,&vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeNAV->Config.SiteGestion);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb Impossible d'envoyer la commande");
      return;
    }

   /* Ecriture dans fichier Trace_CMD_NAV de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                           vl_NoEqtNAV, 
                           pl_mlcr,
                           NULL,
                           vl_NoCMD );
                              	
   /*A mise en memoire du scenario */
   strcpy(pl_listeNAV->Etat.Scenario,vl_Scenario);
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande scenario lancee (%s) %d",pl_listeNAV->Etat.Scenario,pl_listeNAV->Config.Numero );
   pl_listeNAV->NumCmd = (vl_NoCMD == 0) ? XZEXC_CMD_HORS_BASE : vl_NoCMD;      
               
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'arret scen nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_Arret_SCEN_cb ( 
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
*
* Fonction executee sur reception du message XDM_ECMD_ARRET_SCEN
*
* FONCTION 
*  Arret pilotage nav par scenario
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_cmd_Arret_SCEN_cb" ;
  
#ifdef _TIBCO_EMS
   T_INT2			vl_NoEqtNAV;
   T_INT4			vl_NoCMD;
#else
   XDY_Mot			vl_NoEqtNAV;
   XDY_Entier			vl_NoCMD;
#endif
   
   ENAV_DONNEES_NAV *	  	pl_listeNAV;   
   
   int				vl_Index,vl_js;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate		vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_Arret_SCEN_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_ARRET_SCEN */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtNAV
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_ARRET_SCEN non lu." );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Arret_SCEN_cb");
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "Demande Arret Scenario ");
      
   }
   
   /*A Verif des paramestes en entree */
   
   /* verif no eqt nav */
   for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
   {
      if ( pl_listeNAV->Config.Numero == vl_NoEqtNAV) break;
   }
   
   /* Numero eqt nav invalide */
   if (pl_listeNAV == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb avec no eqt nav invalide");
      return;
   }
   

   /*A Construction de ma trame */
   sprintf(pl_mlcr,"BRK ID=%s", vg_idpassw);
	   		
   /*A Envoi de la trame */
   if (EnvoyerTrame(	pl_mlcr,
        		XZEXC_TRAME_ARRET_SCEN_NAV,
        		pl_listeNAV->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket) == XDC_NOK ) 
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_Arret_SCEN_cb Impossible d'envoyer la commande arret scenario" );
      return;
    }

   /* Ecriture dans fichier Trace_CMD_NAV de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                           vl_NoEqtNAV, 
                           pl_mlcr,
                           NULL,
                           0 );
   
    EnvoyerTrame( 	"ST",
        		XZEXC_TRAME_STAT_COMP,
        		pl_listeNAV->Config.AdresseRGS,
		 	XZEXC_CMD_AU_NAV,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeNAV->Socket);
        		
        		
   pl_listeNAV->ArretUrg = XZEXC_CMD_AU_NAV;
   	
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb ");
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le NAV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	em_envoyer_contraste_NAV(   	ENAV_DONNEES_NAV *		pa_ListeNAV,
                         		int                             va_Contraste,
                         		int				va_NumCmd )
/*
* ARGUMENTS EN ENTREE :
*  pa_ListeNAV : pointeur sur liste NAV
*  va_Contraste : Contraste a envoyer
*
* ARGUMENTS EN SORTIE :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*
*  Envoi de la trame de contraste a telmi pour le NAV
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ :  em_envoyer_contraste_NAV " ;

   XZEXT_Trame 			vl_lcr = "";
   char 			vl_Contraste [3];

               /*A Choix de la commande en fonction du contexte jour nuit */
               switch ( va_Contraste )
               {
                  case XDC_CONTRASTEBRILLANT :
                      strcpy ( vl_Contraste, "S" );
                      break;
                  case XDC_CONTRASTENORMAL :
                      strcpy ( vl_Contraste, (vg_Contexte == XDC_JOUR) ? "J1" : (vg_Contexte == XDC_NUIT) ? "N1" : "A" );
                      break;
               }

               /*A envoi de la trame de contraste */
	       sprintf( vl_lcr,"P ID=%s AM=* EC=%s", vg_idpassw, vl_Contraste );
               EnvoyerTrame(	vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListeNAV->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListeNAV->Socket );
               
               /*A envoi de la trame de relecture */
               if ( va_NumCmd != XZEXC_NOCMD )
               {
                 if (EnvoyerTrame(    "PS",
        		        XZEXC_TRAME_ETAT_POS_NAV,
        		        pa_ListeNAV->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListeNAV->Socket) == XDC_NOK ) 
                  {
                     return(XDC_NOK);
                  }
               }
        		        
               if ( va_NumCmd != XZEXC_NOCMD )
                  pa_ListeNAV->ContrasteCmd = (va_Contraste == XDC_CONTRASTEBRILLANT) ? 'S' : (vg_Contexte == XDC_JOUR) ? 'J' : (vg_Contexte == XDC_NUIT) ? 'N' : 'A';
               return ( XDC_OK );
}








/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de changement de contraste .
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_contraste_cb ( 
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
*  XDM_ECMD_Contraste 
*
* FONCTION 
*
*  suivant le type d'equipement envoi du message de contraste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : ep_contraste_cb " ;

#ifdef _TIBC_EMS
   T_INT2 			va_Contraste;		 
   T_INT2 			va_NoEqt;		
   T_INT2 			va_TypeEqt;		
#else
   XDY_Mot 			va_Contraste;		 
   XDY_Mot 			va_NoEqt;		
   XDY_Mot 			va_TypeEqt;		
#endif
   ENAV_DONNEES_NAV *		pl_ListeNAV;
   XZEXT_Trame 			vl_lcr = "";
  
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_contraste_cb");
		  
   /* Recuperation des parametres du message XDM_ECMD_Contraste */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &va_Contraste
                          ,T_IPC_FT_INT2, &va_NoEqt
                          ,T_IPC_FT_INT2, &va_TypeEqt
	       		  ,NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_Contraste non lu." );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb");
      return;
   } 
	       
   if ( va_TypeEqt == XZECC_TYPE_EQT_NAV )
   {
      
      /*B Recherche du No d'equipement dans la structure config_NAV en memoire */
      for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; 
	         pl_ListeNAV = pl_ListeNAV->Suivant )
      {
         if ((pl_ListeNAV->Config.Numero == va_NoEqt) ||
	     (va_NoEqt == XZEXC_SURB_EQTALL))
	 {
	    /*B Composition de la trame contraste et envoi */
	    pl_ListeNAV->Contraste = va_Contraste;
	    em_envoyer_contraste_NAV(pl_ListeNAV,va_Contraste,XZEXC_CMD_HORS_BASE);
            if ( pl_ListeNAV->Config.Numero == va_NoEqt ) break;
	 }
      }
   }
   
   /*A Trace sortie fonction ep_contraste_cb */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb" );
  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_controle_cb ( 
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
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_controle_cb  " ;

    #ifndef _TIBCO_EMS
    T_STR			va_NomMachine;
   T_INT4			va_Etat;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
    #else
    XDY_NomMachine		va_NomMachine;
   XDY_Entier			va_Etat;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
    #endif 

   ENAV_DONNEES_NAV *		pl_ListeNAV;
   int				vl_Index;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_EQPT non lu." );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING,"callback ep_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
      /*A si l'eqt est une NAV */
      if (va_TypeEqt ==  XZECC_TYPE_EQT_NAV)
      {
         /*B Recherche du No d'equipement dans la structure config_NAV en memoire */
         for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; 
	         pl_ListeNAV = pl_ListeNAV->Suivant )
	 {
	    if (pl_ListeNAV->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeNAV->Etat_Service &= va_Etat;
      	       else
      	          pl_ListeNAV->Etat_Service |= va_Etat;
	       break;
    	    }
	 }
      }   
   
      /*A si l'eqt est un module NAV */
      if (va_TypeEqt ==  XDC_EQT_MOD)
      {
         /*B Recherche du No d'equipement dans la structure config_NAV en memoire */
         for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; 
	         pl_ListeNAV = pl_ListeNAV->Suivant )
	 {
            for( vl_Index = 0; vl_Index != XZEXC_nb_module_max; vl_Index++)
            {        	             	       
      	       if ( pl_ListeNAV->Config.Liste_Module[vl_Index].NoModule == va_NumEqt )
      	       {
      	          if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	             pl_ListeNAV->Config.Liste_Module[vl_Index].Etat_Service &= va_Etat;
      	          else
      	             pl_ListeNAV->Config.Liste_Module[vl_Index].Etat_Service |= va_Etat;
      	          
/*                  if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
                  {
                     pl_ListeNAV->Config.Liste_Module[vl_Index].Etat_Service = va_Etat;
                  }
                  if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
                  {
                     pl_ListeNAV->Config.Liste_Module[vl_Index].Etat_Activite = va_Etat;
                  }*/
	          break;
	       }
    	    }
	    if ( pl_ListeNAV->Config.Liste_Module[vl_Index].NoModule == va_NumEqt )
	       break;
	 }
      }   

   /* Trace des Differents etats service et activite */
   for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; 
	         pl_ListeNAV = pl_ListeNAV->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servNAV:%d ",pl_ListeNAV->Etat_Service);
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_controle_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Changement de contexte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_journuit ( XZEAT_JourNuit va_contexte, int va_resultat)
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   vl_resultat
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Met a jour la variable vg_contexte
*
------------------------------------------------------*/
{
   static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_journuit  " ;
   XZEXT_Trame 			vl_lcr = "";
   char				vl_Contraste [3];
   ENAV_DONNEES_NAV *		pl_ListeNAV;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
      vg_Contexte = XDC_JOUR;
   else
      vg_Contexte = XDC_NUIT;
   
   XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d",vg_Contexte );
 		   
   /*B Pour tout les NAV en memoire */
   for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; 
	    pl_ListeNAV = pl_ListeNAV->Suivant )
   {
       /* Envoyer nouveau contraste */
       em_envoyer_contraste_NAV ( pl_ListeNAV, pl_ListeNAV->Contraste, XZEXC_CMD_HORS_BASE );
   }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	en_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg)
		     
		     
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
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
static char *version = "$Id: enav_cmd.c,v 1.23 2020/11/03 18:09:40 pc2dpdy Exp $ : en_CMD_LCR";

#ifndef _TIBCO_EMS   
   T_INT2			vl_TypeEqt;
   T_INT2			vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2			vl_Periode;
   T_REAL8			vl_HrDebut;
   T_REAL8			vl_HrFin;
    T_STR			vl_NomMachine;
#else
   XDY_Mot			vl_TypeEqt;
   XDY_Mot			vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot			vl_Periode;
   XDY_Horodate			vl_HrDebut;
   XDY_Horodate			vl_HrFin;
    XDY_NomMachine		vl_NomMachine;
#endif   

   ENAV_DONNEES_NAV		*pl_ListeNAV;
   INFO_EQT_FEN_LCR	        pl_eqt,*pl_ListeEqt;
   XZEXT_MSG_SOCKET		pl_MsgTimer;
   XZEXT_CLE_PROG   		vl_clef;
   
   	/*A Lecture du message */

   	if(!TipcMsgRead(data->msg,
   			T_IPC_FT_INT2,	&vl_TypeEqt,
   			T_IPC_FT_INT2,	&vl_NoEqt,
   			T_IPC_FT_STR,	&vl_FicCmdLcr,
   			T_IPC_FT_STR,	&vl_FicResLcr,
   			T_IPC_FT_INT2,	&vl_Periode,
   			T_IPC_FT_REAL8,&vl_HrDebut,
   			T_IPC_FT_REAL8,&vl_HrFin,
   			T_IPC_FT_STR,	&vl_NomMachine,
	          	NULL))

      	{       
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_CMD_LCR messsage XDM_ELCR non lu." );
           return;
	}
	      
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN en_CMD_LCR");
	       
	/*A Construction de la liste globale des eqt a piloter par la fenetre LCR */
		
        pl_eqt.TypeEqt=vl_TypeEqt;
        strcpy(pl_eqt.NomFichCmdesLCR,vl_FicCmdLcr);
        strcpy(pl_eqt.NomFichResLCR,vl_FicResLcr);
        pl_eqt.Periode=vl_Periode;
        pl_eqt.HrDebut=vl_HrDebut;
        pl_eqt.HrFin=vl_HrFin;
        strcpy(pl_eqt.NomMachine,vl_NomMachine);
        pl_eqt.Position=0;
        pl_eqt.Suivant=NULL;
        pl_eqt.NoEqt=vl_NoEqt;
        
        /* Recherche si pas de commande LCR en cours sur equipement concerne */
        if ( (vl_TypeEqt == XDC_EQT_NAV) &&
             (ex_test_autorisation_eqt ( &pg_DebutListeNAVFLcr, 
        				 &pl_eqt,
        				 XDC_EQT_NAV) == XDC_NOK) )
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_CMD_LCR ex_test_autorisation_eqt retourne XDC_NOK." );           
            return;
        }

	   if (vl_TypeEqt == XDC_EQT_NAV)
	   {
	      for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; 
	            pl_ListeNAV = pl_ListeNAV->Suivant )
	      {
	         if ( (pl_ListeNAV->Config.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	         {
        	      ex_init_lcr ( 	&pg_DebutListeNAVFLcr,
        	    			pl_eqt,
        	    			pl_ListeNAV->Socket,
        	    			pl_ListeNAV->Config.AdresseRGS,
        	    			pl_ListeNAV->Config.Numero,
        	    			"NAV_FLcr_NoEqt:",
        	    			vg_SockTIM );
        	    			
            }
            if (pl_ListeNAV->Config.Numero == vl_NoEqt)
               break;
	      }
         if ( (pl_ListeNAV == NULL) && (vl_NoEqt!=XDC_EQTALL) )
         {
            XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
         }
   	}
      else
         XZST_03EcritureTrace(XZSTC_WARNING,"en_CMD_LCR : le type recu par message n'est pas XDC_EQT_PAL (1) : %d",vl_TypeEqt );
	
   	/* Fin du callback */
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback en_CMD_LCR" );
 	
}
