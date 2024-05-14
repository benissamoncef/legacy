/*E*/
/* Fichier : $Id: ecfe_cmd.c,v 1.2 2020/11/03 18:09:31 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2020/11/03 18:09:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le modules des callbacks
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* GR    08/07/2020 : EMS 1.2 
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "ecfe_cmd.h"
#include "ex_mdia.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */





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
*  Pour touts les CFE , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: ecfe_cmd.c,v 1.2 2020/11/03 18:09:31 pc2dpdy Exp $ :  ep_Init_IHM_Liaison_cb " ;

#ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
#else
   XDY_Datagroup pl_DGInit;
#endif

   ECFE_DONNEES_CFE *		pl_ListeCFE;
   

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
	        
   /*A Pour chaque CFE : envoi du msg XDM_ETAT_LIAISON  */
  
   for ( pl_ListeCFE = pg_debutCFE; pl_ListeCFE != NULL; pl_ListeCFE = pl_ListeCFE->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListeCFE->Config.Numero, XDC_EQT_CFE ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM CFE.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_CFE_cb ( 
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
*  XDM_EInit_IHM_CFE 
*
* FONCTION 
*  Pour touts les CFE, envoyer l'etat CFE par XDM_ETAT_CFE dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: ecfe_cmd.c,v 1.2 2020/11/03 18:09:31 pc2dpdy Exp $ : ep_Init_IHM_CFE_cb" ;

#ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup pl_DGInit;
   #endif

   ECFE_DONNEES_CFE *		pl_ListeCFE;
   int i;
   static char *		pl_Tab[XDC_Lon_Nb_Mod_Pal];

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_CFE_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_CFE */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: ep_Init_IHM_CFE_cb messsage XDM_EInit_IHM_CFE non lu." );
      return;
   }
	        
	       
   /*A Pour chaque CFE : envoi du msg XDM_ETAT_CFE */
  
   for ( pl_ListeCFE = pg_debutCFE; pl_ListeCFE != NULL; pl_ListeCFE = pl_ListeCFE->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_ListeCFE->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_CFE),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeCFE->Config.Numero,
                          	T_IPC_FT_INT4, pl_ListeCFE->Etat,
				T_IPC_FT_STR,  pl_ListeCFE->Libelle,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_CFE non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_CFE effectue. %d %d",
					pl_ListeCFE->Config.Numero, pl_ListeCFE->Etat);
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_CFE_cb ");
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande pal
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_CFE_cb ( 
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
* Fonction executee sur reception du message XDM_ECMD_CFE
*
* FONCTION 
*  Piloter le module concern�
*
------------------------------------------------------*/
{
   static char *version = "$Id: ecfe_cmd.c,v 1.2 2020/11/03 18:09:31 pc2dpdy Exp $ : en_cmd_CFE_cb" ;
   
#ifndef _TIBCO_EMS
   T_STR			vl_nom;
   T_INT2			vl_NoEqtCFE;
   T_INT4			vl_plan;
   T_INT4			vl_NoCMD;
#else
   XDY_LibellePlanDeFeux        vl_nom;
   XDY_Mot			vl_NoEqtCFE;
   XDY_Mot			vl_plan;
   XDY_Entier			vl_NoCMD;
#endif

   ECFE_DONNEES_CFE *	  	pl_listeCFE;   
   int  			vl_Index = 0, 
   				vl_ptr = 0;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate			vl_horodate = (XDY_Horodate) 0.0;
   T_INT4			vl_Taille = 0;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_CFE_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_CFE */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtCFE
                          ,T_IPC_FT_INT2,  &vl_plan 
                          ,T_IPC_FT_INT4, &vl_NoCMD
			  ,T_IPC_FT_STR, &vl_nom
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_cmd_CFE_cb messsage XDM_ECMD_CFE non lu." );
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd cfe = %d plan:(%d) cmd=%d"
      				     ,vl_NoEqtCFE
      				     ,vl_plan
      				     ,vl_NoCMD);
      
   }
   
   
   /*A Verif des parametres en entree */
   
   for ( pl_listeCFE = pg_debutCFE; pl_listeCFE != NULL; pl_listeCFE = pl_listeCFE->Suivant )
   {
      if ( pl_listeCFE->Config.Numero == vl_NoEqtCFE) break;
   }
   
   /* Numero eqt pal invalide */
   if (pl_listeCFE == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_CFE_cb Message XDM_ECMD_CFE recu avec no eqt pal invalide");
      xzat521_CompteRendu_CFE(vl_NoCMD,vl_horodate,XDC_NOK, 0);
      return;
   }
   

   /* Verif de l'etat d'activite et de l'etat de service */
   if ( pl_listeCFE->Etat_Service & XDC_EQT_HORS_SRV )
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_CFE_cb : Commande refuse car eqt hors service ");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_CFE_cb ");
      xzat521_CompteRendu_CFE(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeCFE->Config.SiteGestion);
      return;
   }

  
   /*A Construction de ma trame */
  /*TODO DIASER*/

   
   if (vl_plan > 0)
   	sprintf(pl_mlcr,"DiCD0%d00",vl_plan);
   else
   	strcpy(pl_mlcr,"DiFD0000");
   XZST_03EcritureTrace(XZSTC_INFO, "en_cmd_CFE_cb : Commande <%s> ",pl_mlcr);
   	   		
   /*A Envoi de la trame */
   if (EnvoyerTrameDiaser(    pl_mlcr,
        		XZEXC_TRAME_CMD_CFE,
        		pl_listeCFE->Config.AdresseIP,
        		pl_listeCFE->Config.Port,
		 	vl_NoCMD,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeCFE->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande CFE" );
      xzat521_CompteRendu_CFE(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeCFE->Config.SiteGestion);
      return;
   }

   /* Ecriture dans fichier Trace_CMD_CFE de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_CFE,
                           vl_NoEqtCFE, 
                           pl_mlcr,
                           NULL,
                           vl_NoCMD );
                           
   /* Mise a jour memoire commande CFE */
   pl_listeCFE->EtatCmd = vl_plan;
   pl_listeCFE->NumCmd = (vl_NoCMD == XZEXC_NOCMD) ? XZEXC_NOCMD : vl_NoCMD;      
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_CFE_cb ");
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
   static char *version = "$Id: ecfe_cmd.c,v 1.2 2020/11/03 18:09:31 pc2dpdy Exp $ : en_controle_cb  " ;

#ifndef _TIBCO_EMS
   T_STR			va_NomMachine;
   T_INT4			va_Etat;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
#else
   XDY_NomMachine               va_NomMachine;
   XDY_Entier			va_Etat;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
#endif

   ECFE_DONNEES_CFE *		pl_ListeCFE;
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
   
      /*A si l'eqt est une CFE */
      if (va_TypeEqt ==  XZECC_TYPE_EQT_CFE)
      {
         /*B Recherche du No d'equipement dans la structure config_CFE en memoire */
         for ( pl_ListeCFE = pg_debutCFE; pl_ListeCFE != NULL ; 
	         pl_ListeCFE = pl_ListeCFE->Suivant )
	 {
	    if (pl_ListeCFE->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeCFE->Etat_Service &= va_Etat;
      	       else
      	          pl_ListeCFE->Etat_Service |= va_Etat;
	       break;
    	    }
	 }
      }   
   

   /* Trace des Differents etats service et activite */
   for ( pl_ListeCFE = pg_debutCFE; pl_ListeCFE != NULL ; 
	         pl_ListeCFE = pl_ListeCFE->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servCFE:%d ",pl_ListeCFE->Etat_Service);
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_controle_cb ");
}
