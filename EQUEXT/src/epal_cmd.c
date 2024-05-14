/*E*/
/* Fichier : $Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $     Release : $Revision: 1.6 $      Date :  $Date: 2020/11/03 18:12:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le modules des callbacks
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	14 Jan 1998	: Creation
* Mismer.D      version 1.2     29 avr 2004     : Ajout pilotage par scenario
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	19/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.4
* JMG	14/06/19 : IP 1.5
* JMG	11/05/20 : EMS 1.6
* GR    09/07/2020 : EMS 1.7
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* GGY    31/05/2023  :  Ajout test PAL IP dans en_CMD_LCR (DEM-471) 
------------------------------------------------------*/

/* fichiers inclus */

#include "epal_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM pal.
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
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_Contraste 
*
* FONCTION 
*  Pour toutes les pal , envoyer l'etat  par XDM_ETAT_Contraste dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : en_Init_IHM_contraste_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif

   EPAL_DONNEES_PAL *		pl_listePAL;
   

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
	        
   /*A Pour chaque pal : envoi du msg XDM_ETAT_Contraste si il est en service et actif */
  
   for ( pl_listePAL = pg_debutPAL; pl_listePAL != NULL; pl_listePAL = pl_listePAL->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_listePAL->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XZECC_TYPE_EQT_PAL,
                          	T_IPC_FT_INT2, pl_listePAL->Config.Numero,
                          	T_IPC_FT_INT2, (pl_listePAL->ContrasteEtat == 'S') ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL,
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
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_Liaison
*
* FONCTION 
*  Pour touts les PAL , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif
   
   EPAL_DONNEES_PAL *		pl_ListePAL;
   

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
	        
   /*A Pour chaque PAL : envoi du msg XDM_ETAT_LIAISON  */
  
   for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL; pl_ListePAL = pl_ListePAL->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListePAL->Config.Numero, XDC_EQT_PAL ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PAL.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_PAL_cb ( 
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
*  Fonction appelee lors de la reception du message
*  XDM_EInit_IHM_PAL 
*
* FONCTION 
*  Pour touts les PAL, envoyer l'etat PAL par XDM_ETAT_PAL dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : ep_Init_IHM_PAL_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPAL_DONNEES_PAL *		pl_ListePAL;
   int i;
#ifndef _TIBCO_EMS
static char *		pl_Tab[XDC_Lon_Nb_Mod_Pal];
#else
   XDY_Nom 		pl_Tab[XDC_Lon_Nb_Mod_Pal];
#endif
   char			vl_str[1024];

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_PAL_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_PAL */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: ep_Init_IHM_PAL_cb messsage XDM_EInit_IHM_PAL non lu." );
      return;
   }
	        
	       
   /*A Pour chaque PAL : envoi du msg XDM_ETAT_PAL */
  
   for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL; pl_ListePAL = pl_ListePAL->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_ListePAL->Etat_Service & XDC_EQT_HORS_SRV) )
      {
        strcpy(vl_str,"");
        for (i=0;i < XDC_Lon_Nb_Mod_Pal;i++)
	{       
		if (strcmp(vl_str,""))
			sprintf(vl_str,"%s|%s",vl_str, pl_ListePAL->Etat[i]);
		else
			sprintf(vl_str,"%s",pl_ListePAL->Etat[i]);
#ifndef _TIBCO_EMS
	      pl_Tab[i] = pl_ListePAL->Etat[i];
#else
	      strcpy(pl_Tab[i] , pl_ListePAL->Etat[i]);
#endif
	      XZST_03EcritureTrace(XZSTC_DEBUG1,"Module %d <%s>.",i, pl_ListePAL->Etat[i] );
	}

     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_PAL),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListePAL->Config.Numero,
                          	T_IPC_FT_INT2, 0,
                          	T_IPC_FT_STR_ARRAY ,  pl_Tab , XDC_Lon_Nb_Mod_Pal,
#ifdef _TIBCO_EMS
				XDC_Lon_Nom,
#endif
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_PAL non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_PAL effectue." );
	 }
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_PAL_EMS),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListePAL->Config.Numero,
                          	T_IPC_FT_INT2, 0,
                          	T_IPC_FT_STR ,  vl_str ,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_PAL_EMS non effectue. vl_str=[%s]", vl_str );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_PAL_EMS effectue. vl_str=[%s]",vl_str );
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PAL_cb ");
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande pal
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_PAL_cb ( 
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
* Fonction executee sur reception du message XDM_ECMD_PAL
*
* FONCTION 
*  Piloter le module concerne
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : en_cmd_PAL_cb" ;
   
   #ifndef _TIBCO_EMS   
   T_INT2			vl_NoEqtPAL;
   T_STR			vl_Etat;
   T_INT4			vl_NoCMD;
   #else
   XDY_Mot			vl_NoEqtPAL;
   XDY_Etat_Module_PAL          vl_Etat;
   XDY_Entier			vl_NoCMD;
   #endif

   EPAL_DONNEES_PAL *	  	pl_listePAL;   
   int  			vl_Index = 0, 
   				vl_ptr = 0;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate			vl_horodate = (XDY_Horodate) 0.0;
   T_INT4			vl_Taille = 0;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_PAL_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_PAL */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtPAL
                          ,T_IPC_FT_STR,  &vl_Etat 
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_cmd_PAL_cb messsage XDM_ECMD_PAL non lu." );
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_WARNING, "mes cmd pal = %d M:(%s) cmd=%d"
      				     ,vl_NoEqtPAL
      				     ,vl_Etat
      				     ,vl_NoCMD);
      
   }
   
   
   /*A Verif des parametres en entree */
   
   for ( pl_listePAL = pg_debutPAL; pl_listePAL != NULL; pl_listePAL = pl_listePAL->Suivant )
   {
      if ( pl_listePAL->Config.Numero == vl_NoEqtPAL) break;
   }
   
   /* Numero eqt pal invalide */
   if (pl_listePAL == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_PAL_cb Message XDM_ECMD_PAL recu avec no eqt pal invalide");
      xzat28_CompteRendu_Pal(vl_NoCMD,vl_horodate,XDC_NOK, 0);
      return;
   }
   

   /* Verif de l'etat d'activite et de l'etat de service */
   if ( pl_listePAL->Etat_Service & XDC_EQT_HORS_SRV )
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_PAL_cb : Commande refuse car eqt hors service ");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_PAL_cb ");
      xzat28_CompteRendu_Pal(vl_NoCMD,vl_horodate,XDC_NOK, pl_listePAL->Config.SiteGestion);
      return;
   }

  
   /*A Construction de ma trame */
  vl_ptr = sprintf ( pl_mlcr, "PM ID=%s AF=%s\0", vg_idpassw, strtok(vl_Etat," ") );

   
   XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_PAL_cb : Commande <%s> ",pl_mlcr);
   	   		
   /*A Envoi de la trame */
   if (!strcmp(pl_listePAL->Config.AdresseIP, XZECC_IP_VIDE)) {
   if (EnvoyerTrame(    pl_mlcr,
        		XZEXC_TRAME_CMD_PAL,
        		pl_listePAL->Config.AdresseRGS,
		 	XZEXC_NOCMD,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listePAL->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PAL" );
      xzat28_CompteRendu_Pal(vl_NoCMD,vl_horodate,XDC_NOK, pl_listePAL->Config.SiteGestion);
      return;
   }
   }
   else
   {
   if (EnvoyerTrameIP(    pl_mlcr,
        		XZEXC_TRAME_CMD_PAL,
        		pl_listePAL->Config.AdresseIP,
        		pl_listePAL->Config.Port,
        		pl_listePAL->Config.AdresseRGS,
		 	XZEXC_NOCMD,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listePAL->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PAL" );
      xzat28_CompteRendu_Pal(vl_NoCMD,vl_horodate,XDC_NOK, pl_listePAL->Config.SiteGestion);
      return;
   }
   }


   /* Ecriture dans fichier Trace_CMD_PAL de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_PAL,
                           vl_NoEqtPAL, 
                           pl_mlcr,
                           NULL,
                           vl_NoCMD );
                           
   /*A envoi de la trame de relecture */
   if (!strcmp(pl_listePAL->Config.AdresseIP, XZECC_IP_VIDE)) {
   	if (EnvoyerTrame(    "ST",
        		XZEXC_TRAME_STAT_COMP,
        		pl_listePAL->Config.AdresseRGS,
		 	(vl_NoCMD == 0) ? XZEXC_CMD_LT_PAL : vl_NoCMD,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listePAL->Socket) == XDC_NOK ) 
   	{
      		xzat28_CompteRendu_Pal(vl_NoCMD,vl_horodate,XDC_NOK, pl_listePAL->Config.SiteGestion);
      		return;
   	}
   }
   else
   	{
   	if (EnvoyerTrameIP(    "ST",
        		XZEXC_TRAME_STAT_COMP,
        		pl_listePAL->Config.AdresseIP,
        		pl_listePAL->Config.Port,
        		pl_listePAL->Config.AdresseRGS,
		 	(vl_NoCMD == 0) ? XZEXC_CMD_LT_PAL : vl_NoCMD,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listePAL->Socket) == XDC_NOK ) 
   	{
      		xzat28_CompteRendu_Pal(vl_NoCMD,vl_horodate,XDC_NOK, pl_listePAL->Config.SiteGestion);
      		return;
   	}
   }


   
   /* Mise a jour memoire commande PAL */
   strcpy ( pl_listePAL->EtatCmd, strtok(vl_Etat, " ") ); 
   pl_listePAL->NumCmd = (vl_NoCMD == XZEXC_NOCMD) ? XZEXC_CMD_LT_PAL : vl_NoCMD;      
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_PAL_cb ");
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le PAL
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	em_envoyer_contraste_PAL(   	EPAL_DONNEES_PAL *		pa_ListePAL,
                         		int                             va_Contraste,
                         		int				va_NumCmd )
/*
* ARGUMENTS EN ENTREE :
*  pa_ListePAL : pointeur sur liste PAL
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
*  Envoi de la trame de contraste a telmi pour le PAL
*
------------------------------------------------------*/
{
   static char *version = "@(#)epal_cmd.c	1.2 05/06/04 :  em_envoyer_contraste_PAL " ;

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
   		if (!strcmp(pa_ListePAL->Config.AdresseIP, XZECC_IP_VIDE)) 
               		EnvoyerTrame(	vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListePAL->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListePAL->Socket );
              	else
               		EnvoyerTrameIP(	vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListePAL->Config.AdresseIP,
                                pa_ListePAL->Config.Port,
                                pa_ListePAL->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListePAL->Socket );

               /*A envoi de la trame de relecture */
               if ( va_NumCmd != XZEXC_NOCMD )
               {
   		if (!strcmp(pa_ListePAL->Config.AdresseIP, XZECC_IP_VIDE)) 
                  {
                 	if (EnvoyerTrame(    "PS",
        		        XZEXC_TRAME_ETAT_POS_PAL,
        		        pa_ListePAL->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePAL->Socket) == XDC_NOK ) 
                  	{
                     	return(XDC_NOK);
                  	}
                  }
		  else
                  {
                 	if (EnvoyerTrameIP(    "PS",
        		        XZEXC_TRAME_ETAT_POS_PAL,
        		        pa_ListePAL->Config.AdresseIP,
        		        pa_ListePAL->Config.Port,
        		        pa_ListePAL->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePAL->Socket) == XDC_NOK ) 
                  	{
                     	return(XDC_NOK);
                  	}
                  }

               }
        		        
               if ( va_NumCmd != XZEXC_NOCMD )
                  pa_ListePAL->ContrasteCmd = (va_Contraste == XDC_CONTRASTEBRILLANT) ? 'S' : (vg_Contexte == XDC_JOUR) ? 'J' : (vg_Contexte == XDC_NUIT) ? 'N' : 'A';
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
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_Contraste 
*
* FONCTION 
*
*  suivant le type d'equipement envoi du message de contraste
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : ep_contraste_cb " ;

#ifdef _TIBCO_EMS
   XDY_Mot 			va_Contraste;		 
   XDY_Mot 			va_NoEqt;		
   XDY_Mot 			va_TypeEqt;		
#else
   T_INT2 			va_Contraste;		 
   T_INT2 			va_NoEqt;		
   T_INT2 			va_TypeEqt;		
#endif
   EPAL_DONNEES_PAL *		pl_ListePAL;
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
	       
   if ( va_TypeEqt == XZECC_TYPE_EQT_PAL )
   {
      
      /*B Recherche du No d'equipement dans la structure config_PAL en memoire */
      for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL ; 
	         pl_ListePAL = pl_ListePAL->Suivant )
      {
         if ((pl_ListePAL->Config.Numero == va_NoEqt) ||
	     (va_NoEqt == XZEXC_SURB_EQTALL))
	 {
	    /*B Composition de la trame contraste et envoi */
	    pl_ListePAL->Contraste = va_Contraste;
	    em_envoyer_contraste_PAL(pl_ListePAL,va_Contraste,XZEXC_CMD_HORS_BASE);
            if ( pl_ListePAL->Config.Numero == va_NoEqt ) break;
	 }
      }
   }
   
   /*A Trace sortie fonction ep_contraste_cb */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb" );
  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contrele.
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
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : en_controle_cb  " ;

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

   EPAL_DONNEES_PAL *		pl_ListePAL;
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
   
      /*A si l'eqt est une PAL */
      if (va_TypeEqt ==  XZECC_TYPE_EQT_PAL)
      {
         /*B Recherche du No d'equipement dans la structure config_PAL en memoire */
         for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL ; 
	         pl_ListePAL = pl_ListePAL->Suivant )
	 {
	    if (pl_ListePAL->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListePAL->Etat_Service &= va_Etat;
      	       else
      	          pl_ListePAL->Etat_Service |= va_Etat;
	       break;
    	    }
	 }
      }   
   

   /* Trace des Differents etats service et activite */
   for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL ; 
	         pl_ListePAL = pl_ListePAL->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servPAL:%d ",pl_ListePAL->Etat_Service);
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
   static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : en_journuit  " ;
   XZEXT_Trame 			vl_lcr = "";
   char				vl_Contraste [3];
   EPAL_DONNEES_PAL *		pl_ListePAL;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
      vg_Contexte = XDC_JOUR;
   else
      vg_Contexte = XDC_NUIT;
   
   XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d",vg_Contexte );
 		   
   /*B Pour tout les PAL en memoire */
   for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL ; 
	    pl_ListePAL = pl_ListePAL->Suivant )
   {
       /* Envoyer nouveau contraste */
       em_envoyer_contraste_PAL ( pl_ListePAL, pl_ListePAL->Contraste, XZEXC_CMD_HORS_BASE );
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
*  Fonction appelee lors de la reception du message
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
static char *version = "$Id: epal_cmd.c,v 1.6 2020/11/03 18:12:15 pc2dpdy Exp $ : en_CMD_LCR";

#ifndef _TIBCO_EMS   
   T_INT2			vl_TypeEqt;
   T_INT2			vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2			vl_Periode;
   T_REAL8			vl_HrDebut;
   T_REAL8			vl_HrFin;
#else
   XDY_Mot			vl_TypeEqt;
   XDY_Mot			vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot			vl_Periode;
   XDY_Horodate			vl_HrDebut;
   XDY_Horodate			vl_HrFin;
#endif   

    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    #else
    XDY_NomMachine		vl_NomMachine;
    #endif

   EPAL_DONNEES_PAL		*pl_ListePAL;
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
           XZST_03EcritureTrace(XZSTC_WARNING,"OUT: ep_CMD_LCR messsage XDM_ELCR non lu." );
           return;
	}
	      
        XZST_03EcritureTrace(XZSTC_FONCTION, "IN ep_CMD_LCR");
	       
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
        if ( (vl_TypeEqt == XDC_EQT_PAL) &&
             (ex_test_autorisation_eqt ( &pg_DebutListePALFLcr, 
        				 &pl_eqt,
        				 XDC_EQT_PAL) == XDC_NOK) )
        {
           return;
        }

      if (vl_TypeEqt == XDC_EQT_PAL)
	   {
	      for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL ; pl_ListePAL = pl_ListePAL->Suivant )
	      {
	         if ( (pl_ListePAL->Config.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	         {
		         if (!strcmp(pl_ListePAL->Config.AdresseIP,XZECC_IP_VIDE))
               { 
             	   XZST_03EcritureTrace(XZSTC_WARNING, "en_CMD_LCR: ex_init_lcr vl_NoEqt=%d", vl_NoEqt);
                  ex_init_lcr ( 	&pg_DebutListePALFLcr,
                        pl_eqt,
                        pl_ListePAL->Socket,
                        pl_ListePAL->Config.AdresseRGS,
                        pl_ListePAL->Config.Numero,
                        "PAL_FLcr_NoEqt:",
                        vg_SockTIM );
               }
        	      else
               {
        	    	   XZST_03EcritureTrace(XZSTC_WARNING, "en_CMD_LCR: ex_init_lcr_ip vl_NoEqt=%d", vl_NoEqt);
                  ex_init_lcr_ip ( 	&pg_DebutListePALFLcr,
        	    			   pl_eqt,
        	    			   pl_ListePAL->Socket,
        	    			   pl_ListePAL->Config.AdresseRGS,
        	    			   pl_ListePAL->Config.Numero,
        	    			   "PAL_FLcr_NoEqt:",
        	    			   vg_SockTIM,
					         pl_ListePAL->Config.AdresseIP,
					         pl_ListePAL->Config.Port);
               } 			
            }
                 
            if (pl_ListePAL->Config.Numero == vl_NoEqt)
            {
               XZST_03EcritureTrace(XZSTC_WARNING, "en_CMD_LCR: break vl_NoEqt=%d", vl_NoEqt);
               break;
            }
	      }
         
               
         if ( (pl_ListePAL == NULL) && (vl_NoEqt!=XDC_EQTALL) )
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "en_CMD_LCR: XZEX02_Retour_LCR");
            XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
         }
   	}

   	/* Fin du callback */
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback en_CMD_LCR" );
}
