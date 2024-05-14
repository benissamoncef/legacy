/*E*/
/* Fichier : $Id: ebaf_cmd.c,v 1.16 2020/11/03 18:09:29 pc2dpdy Exp $     Release : $Revision: 1.16 $       Date : $Date: 2020/11/03 18:09:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAF * FICHIER ebaf_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
* PNI	13/03/07	S�paration en 2 cmde pour les BAF� 2 barri�res 1.2 1.3
* PNI	14/05/07	Ajout Pilotage global (feux+barriere) pour l'ouverture uniqt1.4
* PNI	11/10/07	Rejet de la commande demandant simultan�ment l'abaissement de la barri�re et l'extinction des feux DEM712 1.5
* PNI	15/10/07	Modif envoi PS apres cmd pour executaion de 4 PS pour attendre l'execution compl�te de la manoeuvre barri�re DEM712 1.6
* PNI	15/11/07	Suppression du refus de traitement de commande DEM725 v1.8
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	27/01/09	: Renvoie etat 0 � l'ihm si etat module > 1 (2) v1.10 DEM862
* JPL	23/11/09	: Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908) 1.11
* PNI	15/10/07	Modif envoi PS apres cmd pour execution de 6 PS pour attendre l'execution compl�te de la manoeuvre barri�re DEM920 1.12
* JMG	12/12/10	portage linux 1.13
* JPL	22/02/18	Migration Linux 64 bits (DEM 1274) : Test de rejet en cas d'activation d'une lisse sans les feux  1.14
* JMG	12/06/19	: IP DEM1332 1.15
* Jmg	14/05/20 : EMS 1.16
* GR    08/07/2020 : EMS 1.17
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------ */


/* fichiers inclus */

#include "ebaf_cmd.h"

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
*  Pour touts les BAF , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 :  ep_Init_IHM_Liaison_cb " ;

#ifndef _TIBCO_EMS
   T_STR	       	pl_DGInit;
#else
   XDY_Datagroup   pl_DGInit;
#endif

   EBAF_DONNEES_BAF *		pl_ListeBAF;
   

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
	        
   /*A Pour chaque BAF : envoi du msg XDM_ETAT_LIAISON  */
  
   for ( pl_ListeBAF = pg_debutBAF; pl_ListeBAF != NULL; pl_ListeBAF = pl_ListeBAF->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListeBAF->Config.Numero, XDC_EQT_BAF ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM BAF.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_BAF_cb ( 
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
*  XDM_EInit_IHM_BAF 
*
* FONCTION 
*  Pour touts les BAF, envoyer l'etat BAF par XDM_ETAT_BAF dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 : ep_Init_IHM_BAF_cb" ;
#ifndef _TIBCO_EMS   
   T_STR			pl_DGInit;
#else
   XDY_Datagroup   pl_DGInit;
#endif

   EBAF_DONNEES_BAF *		pl_ListeBAF;
   int i;
   char **pl;
#ifndef _TIBCO_EMS
   static char *			pl_Tab[XZEXC_nb_module_max_BAF];
#else
   XDY_Nom 			pl_Tab[XZEXC_nb_module_max_BAF];
#endif
   char                 vl_str[1024];
	

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_BAF_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_BAF */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: ep_Init_IHM_BAF_cb messsage XDM_EInit_IHM_BAF non lu." );
      return;
   }
	        
	       
   /*A Pour chaque BAF : envoi du msg XDM_ETAT_BAF */
  
   for ( pl_ListeBAF = pg_debutBAF; pl_ListeBAF != NULL; pl_ListeBAF = pl_ListeBAF->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_ListeBAF->Etat_Service & XDC_EQT_HORS_SRV) )
      {
         XZST_03EcritureTrace(XZSTC_INFO,"CLUCHAGUE baf no:%d",pl_ListeBAF->Config.Numero);
	      strcpy(vl_str,"");
         for (i=0;i < XZEXC_nb_module_max_BAF;i++)
         {        
                if (strcmp(vl_str,""))
                        sprintf(vl_str,"%s|%s",vl_str, pl_ListeBAF->Etat.Module[i]);
                else
                        sprintf(vl_str,"%s",pl_ListeBAF->Etat.Module[i]);
#ifndef _TIBCO_EMS
            pl_Tab[i] = pl_ListeBAF->Etat.Module[i];
#else
            strcpy(pl_Tab[i] , pl_ListeBAF->Etat.Module[i]);
#endif
	    /*
            XZST_03EcritureTrace(XZSTC_INFO,"CLUCHAGUE avant baf etat module no:%d=%s",i,pl_Tab[i]);*/
#ifdef _HPUX_SOURCE
	   if (atoi(pl_Tab[i]) > 1) {
		strcpy(pl_Tab[i],"0");
		strcpy(pl_ListeBAF->Etat.Module[i],"0");
	    }
#endif	    
            XZST_03EcritureTrace(XZSTC_INFO,"CLUCHAGUE baf etat module no:%d=%s",i,pl_Tab[i]);
         }
         
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_BAF),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeBAF->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeBAF->Config.Type,
                          	T_IPC_FT_STR_ARRAY ,  pl_Tab , XZEXC_nb_module_max_BAF,
#ifdef _TIBCO_EMS
				XDC_Lon_Nom,
#endif
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_BAF non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_BAF effectue no:%d type:%d nb modules:%d ",pl_ListeBAF->Config.Numero,
		pl_ListeBAF->Config.Type,XZEXC_nb_module_max_BAF);
	 }
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_BAF_EMS),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeBAF->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeBAF->Config.Type,
                          	T_IPC_FT_STR , vl_str,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_BAF_EMS non effectue vl_str=[%s].",vl_str );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_BAF_EMS effectue no:%d type:%d nb modules:%d modules:[%s]",pl_ListeBAF->Config.Numero,
		pl_ListeBAF->Config.Type,XZEXC_nb_module_max_BAF,vl_str);
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_BAF_cb ");
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_BAF_cb ( 
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
* Fonction executee sur reception du message XDM_ECMD_BAF
*
* FONCTION 
*  Piloter le module concern�
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 : en_cmd_BAF_cb" ;
   
#ifndef _TIBCO_EMS
   T_STR			vl_Etat;
   T_INT2			vl_Module;
   T_INT2			vl_Flash;
   T_INT4			vl_NoCMD;
   T_INT2			vl_NoEqtBAF;
#else
   XDY_Etat_Module  		vl_Etat;
   XDY_Mot			vl_Module;
   XDY_Mot			vl_Flash;
   XDY_Entier			vl_NoCMD;
   XDY_Mot			vl_NoEqtBAF;
#endif

   EBAF_DONNEES_BAF *	  	pl_listeBAF;   
   int				vl_Index,vl_js;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate			vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_BAF_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_BAF */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtBAF
                          ,T_IPC_FT_INT2, &vl_Module
                          ,T_IPC_FT_STR,  &vl_Etat
                          ,T_IPC_FT_INT2, &vl_Flash
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_cmd_BAF_cb messsage XDM_ECMD_BAF non lu." );
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd baf = %d %d %s %d %d"
      				     ,vl_NoEqtBAF
      				     ,vl_Module
      				     ,vl_Etat
      				     ,vl_Flash
      				     ,vl_NoCMD);
      
   }
   
   
   /*A Verif des parametres en entree */
   
   for ( pl_listeBAF = pg_debutBAF; pl_listeBAF != NULL; pl_listeBAF = pl_listeBAF->Suivant )
   {
      if ( pl_listeBAF->Config.Numero == vl_NoEqtBAF) break;
   }
   
   /* Numero eqt nav invalide */
   if (pl_listeBAF == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_BAF_cb Message XDM_ECMD_BAF recu avec no eqt nav invalide");
      return;
   }
   

   /* Verif de l'etat d'activite et de l'etat de service */
   if ( (pl_listeBAF->Etat_Service & XDC_EQT_HORS_SRV) || 
        (pl_listeBAF->Config.Liste_Module[vl_Module].Etat_Service & XDC_EQT_HORS_SRV))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_BAF_cb : Message BAF ou Module commande invalide");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAF_cb ");
      return;
   }


   /*A Recherche si Module valide */
   
   if ((vl_Module >= XZEXC_nb_module_max_BAF) || ( pl_listeBAF->Config.Liste_Module[vl_Module].NoModule == 0))
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_DEBUG1, "eOUT: n_cmd_BAF_cb : Message XDM_ECMD_BAF recu avec No Module nav invalide");
      return;
   }
   
   /*A Construction de ma trame */
   sprintf(pl_mlcr,"P ID=%s AM=%s AF=%s",
   			      vg_idpassw,
	                      pl_listeBAF->Config.Liste_Module[vl_Module].Module,
	   	              vl_Etat);
	   		
   /*A Envoi de la trame */
   if (!strcmp(pl_listeBAF->Config.AdresseIP, XZECC_IP_VIDE)) {	
   if (EnvoyerTrame(    pl_mlcr,
        		XZEXC_TRAME_CMD_BAF,
        		pl_listeBAF->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAF" );
      return;
   }
  }
  else {
   if (EnvoyerTrameIP(    pl_mlcr,
        		XZEXC_TRAME_CMD_BAF,
			pl_listeBAF->Config.AdresseIP,
			pl_listeBAF->Config.Port,
        		pl_listeBAF->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAF" );
      return;
   }

  }
   /* Ecriture dans fichier Trace_CMD_BAF de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_BAF,
                           vl_NoEqtBAF, 
                           pl_mlcr,
                           NULL,
                           vl_Module+1 );
                           
   /*A envoi de la trame de relecture */
   if (!strcmp(pl_listeBAF->Config.AdresseIP, XZECC_IP_VIDE)) {	
   if (EnvoyerTrame(    "PS",
        		XZEXC_TRAME_ETAT_POS_BAF,
        		pl_listeBAF->Config.AdresseRGS,
		 	vl_Module+1,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      return;
   }
   }
   else {
   if (EnvoyerTrameIP(    "PS",
        		XZEXC_TRAME_ETAT_POS_BAF,
			pl_listeBAF->Config.AdresseIP,
			pl_listeBAF->Config.Port,
        		pl_listeBAF->Config.AdresseRGS,
		 	vl_Module+1,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      return;
   }

   }
   
   /* Mise a jour memoire commande BAF */
   pl_listeBAF->NumCmd=vl_NoCMD;
   strcpy ( pl_listeBAF->EtatCmd.Module[vl_Module], vl_Etat ); 
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAF_cb ");
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
   static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 : en_controle_cb  " ;

#ifndef _TIBCO_EMS 
   T_STR			va_NomMachine;
   T_INT4			va_Etat;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
#else
   XDY_NomMachine 		va_NomMachine;
   XDY_Entier			va_Etat;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
#endif
   
   EBAF_DONNEES_BAF *		pl_ListeBAF;
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
   XZST_03EcritureTrace(XZSTC_INFO,"callback ep_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
      /*A si l'eqt est une BAF */
      if (va_TypeEqt ==  XZECC_TYPE_EQT_BAF)
      {
         /*B Recherche du No d'equipement dans la structure config_BAF en memoire */
         for ( pl_ListeBAF = pg_debutBAF; pl_ListeBAF != NULL ; 
	         pl_ListeBAF = pl_ListeBAF->Suivant )
	 {
	    if (pl_ListeBAF->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeBAF->Etat_Service &= va_Etat;
      	       else
      	          pl_ListeBAF->Etat_Service |= va_Etat;
	       break;
    	    }
	 }
      }   
   
      /*A si l'eqt est un module BAF */
      if (va_TypeEqt ==  XDC_EQT_MODBAF)
      {
         /*B Recherche du No d'equipement dans la structure config_BAF en memoire */
         for ( pl_ListeBAF = pg_debutBAF; pl_ListeBAF != NULL ; 
	         pl_ListeBAF = pl_ListeBAF->Suivant )
	 {
            for( vl_Index = 0; vl_Index != XZEXC_nb_module_max_BAF; vl_Index++)
            {        	             	       
      	       if ( pl_ListeBAF->Config.Liste_Module[vl_Index].NoModule == va_NumEqt )
      	       {
      	          if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	             pl_ListeBAF->Config.Liste_Module[vl_Index].Etat_Service &= va_Etat;
      	          else
      	             pl_ListeBAF->Config.Liste_Module[vl_Index].Etat_Service |= va_Etat;
      	          
/*                  if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
                  {
                     pl_ListeBAF->Config.Liste_Module[vl_Index].Etat_Service = va_Etat;
                  }
                  if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
                  {
                     pl_ListeBAF->Config.Liste_Module[vl_Index].Etat_Activite = va_Etat;
                  }*/
	          break;
	       }
    	    }
	    if ( pl_ListeBAF->Config.Liste_Module[vl_Index].NoModule == va_NumEqt )
	       break;
	 }
      }   

   /* Trace des Differents etats service et activite */
   for ( pl_ListeBAF = pg_debutBAF; pl_ListeBAF != NULL ; 
	         pl_ListeBAF = pl_ListeBAF->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servBAF:%d ",pl_ListeBAF->Etat_Service);
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
   static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 : en_journuit  " ;
   XZEXT_Trame 			vl_lcr = "";
   char				vl_Contraste [3];
   EBAF_DONNEES_BAF *		pl_ListeBAF;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
      vg_Contexte = XDC_JOUR;
   else
      vg_Contexte = XDC_NUIT;
   
   XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d",vg_Contexte );
 		   
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
static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 : en_CMD_LCR";

#ifndef _TIBCO_EMS   
   T_INT2			vl_TypeEqt;
   T_INT2			vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_STR			vl_NomMachine;
   T_INT2			vl_Periode;
   T_REAL8			vl_HrDebut;
   T_REAL8			vl_HrFin;
#else
   XDY_Mot			vl_TypeEqt;
   XDY_Mot			vl_NoEqt;
   XDY_Mot			vl_Periode;
   XDY_Horodate			vl_HrDebut;
   XDY_Horodate			vl_HrFin;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_NomMachine 		vl_NomMachine;
#endif

   EBAF_DONNEES_BAF		*pl_ListeBAF;
   INFO_EQT_FEN_LCR	        pl_eqt,*pl_ListeEqt;
   XZEXT_MSG_SOCKET		pl_MsgTimer;
   XZEXT_CLE_PROG   		vl_clef;

        XZST_03EcritureTrace(XZSTC_FONCTION, "IN ep_CMD_LCR");
   
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
        if ( (vl_TypeEqt == XDC_EQT_BAF) &&
             (ex_test_autorisation_eqt ( &pg_DebutListeBAFFLcr, 
        				 &pl_eqt,
        				 XDC_EQT_BAF) == XDC_NOK) )
        {
           return;
        }

	   if (vl_TypeEqt == XDC_EQT_BAF)
	   {
	      for ( pl_ListeBAF = pg_debutBAF; pl_ListeBAF != NULL ; 
	            pl_ListeBAF = pl_ListeBAF->Suivant )
	      {
	         if ( (pl_ListeBAF->Config.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	         {
		    if (!strcmp(pl_ListeBAF->Config.AdresseIP,XZECC_IP_VIDE)) 
        	    	ex_init_lcr ( 	&pg_DebutListeBAFFLcr,
        	    			pl_eqt,
        	    			pl_ListeBAF->Socket,
        	    			pl_ListeBAF->Config.AdresseRGS,
        	    			pl_ListeBAF->Config.Numero,
        	    			"BAF_FLcr_NoEqt:",
        	    			vg_SockTIM );
        	    else
        	    	ex_init_lcr_ip ( 	&pg_DebutListeBAFFLcr,
        	    			pl_eqt,
        	    			pl_ListeBAF->Socket,
        	    			pl_ListeBAF->Config.AdresseRGS,
        	    			pl_ListeBAF->Config.Numero,
        	    			"BAF_FLcr_NoEqt:",
        	    			vg_SockTIM,
					pl_ListeBAF->Config.AdresseIP,
					pl_ListeBAF->Config.Port);

                 }
                 if (pl_ListeBAF->Config.Numero == vl_NoEqt)
                   break;
	      }
              if ( (pl_ListeBAF == NULL) && (vl_NoEqt!=XDC_EQTALL) )
              {
                 XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
              }
   	   }
     
	
   	/* Fin du callback */
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_CMD_LCR" );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_BAF_globale_cb ( 
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
* Fonction executee sur reception du message XDM_ECMD_BAF
*
* FONCTION 
*  Piloter le module concern�
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebaf_cmd.c	1.20 01/10/05 : en_cmd_BAF_cb" ;
   
#ifndef _TIBCO_EMS
   T_INT2			vl_NoEqtBAF;
   T_STR			vl_Etat1;
   T_STR			vl_Etat2;
   T_STR			vl_Etat3;
   T_STR			vl_Etat4;
   T_STR			vl_Etat5;
   T_STR			vl_Etat6;
   T_INT2			vl_Flash;
   T_INT2			vl_Module=0;
   T_INT4			vl_NoCMD;
#else
   XDY_Mot			vl_NoEqtBAF;
   XDY_Etat_Module              vl_Etat1;
   XDY_Etat_Module		vl_Etat2;
   XDY_Etat_Module		vl_Etat3;
   XDY_Etat_Module		vl_Etat4;
   XDY_Etat_Module		vl_Etat5;
   XDY_Etat_Module		vl_Etat6;
   XDY_Mot			vl_Flash;
   XDY_Mot			vl_Module=0;
   XDY_Entier			vl_NoCMD;
#endif

   EBAF_DONNEES_BAF *	  	pl_listeBAF;   
   int				vl_Index,vl_js;
   XZEXT_Trame			pl_mlcr1;
   XZEXT_Trame			pl_mlcr;
   XDY_Horodate			vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_BAF_cb");
		  
   /*A Recuperation des parametres du message XDM_ECMD_BAF */
   if(!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2, &vl_NoEqtBAF
                          ,T_IPC_FT_STR,  &vl_Etat1
                          ,T_IPC_FT_STR,  &vl_Etat2
                          ,T_IPC_FT_STR,  &vl_Etat3
                          ,T_IPC_FT_STR,  &vl_Etat4
                          ,T_IPC_FT_STR,  &vl_Etat5
                          ,T_IPC_FT_STR,  &vl_Etat6
                          ,T_IPC_FT_INT2, &vl_Flash
                          ,T_IPC_FT_INT4, &vl_NoCMD
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_cmd_BAF_cb messsage XDM_ECMD_BAF non lu." );
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd baf = %d %s %s %s %s %s %s %d %d"
      				     ,vl_NoEqtBAF
      				     ,vl_Etat1
      				     ,vl_Etat2
      				     ,vl_Etat3
      				     ,vl_Etat4
      				     ,vl_Etat5
      				     ,vl_Etat6
      				     ,vl_Flash
      				     ,vl_NoCMD);
      
   }
   
   
   /*A Verif des parametres en entree */
   
   for ( pl_listeBAF = pg_debutBAF; pl_listeBAF != NULL; pl_listeBAF = pl_listeBAF->Suivant )
   {
      if ( pl_listeBAF->Config.Numero == vl_NoEqtBAF) break;
   }
   
   /* Numero eqt nav invalide */
   if (pl_listeBAF == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_BAF_cb Message XDM_ECMD_BAF recu avec no eqt nav invalide");
      return;
   }
   

   /* Verif de l'etat d'activite et de l'etat de service */
   if ( (pl_listeBAF->Etat_Service & XDC_EQT_HORS_SRV) || 
        (pl_listeBAF->Config.Liste_Module[0].Etat_Service & XDC_EQT_HORS_SRV))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_BAF_cb : Message BAF ou Module commande invalide");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAF_cb ");
      return;
   }

  


   if (pl_listeBAF->Config.Type==2)  { 
     /* si la fermeture est demand� sans le feux d'alerte alors rejet de la commande */
     if ( ((vl_Etat1[0]=='0') && (vl_Etat3[0]!='0'))  ||  ((vl_Etat4[0]=='0') && (vl_Etat6[0]!='0')) )
     {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_BAF_cb : la fermeture est demand� sans le feux d'alerte ==> rejet de la commande");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAF_cb ");
      return;
     }

	/* test si pilotage global feux+barrieres */
     if ( (vl_Etat1[0]=='1') && (vl_Etat2[0]=='1') && (vl_Etat3[0]=='1') && (vl_Etat4[0]=='1') && (vl_Etat5[0]=='1') && (vl_Etat6[0]=='1') )
	sprintf(pl_mlcr,"P ID=%s AM=*.* AF=1",vg_idpassw);
     else
     if ( (vl_Etat1[0]=='0') && (vl_Etat2[0]=='0') && (vl_Etat3[0]=='0') && (vl_Etat4[0]=='0') && (vl_Etat5[0]=='0') && (vl_Etat6[0]=='0') )
	sprintf(pl_mlcr,"P ID=%s AM=*.* AF=0",vg_idpassw);
     else {
     sprintf(pl_mlcr1,"P ID=%s AM=%s AF=%s AM=%s AF=%s AM=%s AF=%s",
   			      vg_idpassw,
	                      pl_listeBAF->Config.Liste_Module[0].Module, vl_Etat1,
	                      pl_listeBAF->Config.Liste_Module[1].Module, vl_Etat2,
	                      pl_listeBAF->Config.Liste_Module[2].Module, vl_Etat3
			      );
     /*A Envoi de la trame */
     if (!strcmp(pl_listeBAF->Config.AdresseIP, XZECC_IP_VIDE)){
     if (EnvoyerTrame(    pl_mlcr1,
        		XZEXC_TRAME_CMD_BAF,
        		pl_listeBAF->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
      {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAF" );
      return;
      }
     }
     else {
     if (EnvoyerTrameIP(    pl_mlcr1,
        		XZEXC_TRAME_CMD_BAF,
			pl_listeBAF->Config.AdresseIP,
			pl_listeBAF->Config.Port,
        		pl_listeBAF->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
      {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAF" );
      return;
      }

     }

     /* Ecriture dans fichier Trace_CMD_BAF de la commande */    
     ex_ecriture_trace_cmd ( XDC_EQT_BAF,
                           vl_NoEqtBAF, 
                           pl_mlcr1,
                           NULL,
                            vl_Module+1 );
     sprintf(pl_mlcr,"P ID=%s AM=%s AF=%s AM=%s AF=%s AM=%s AF=%s",
   			      vg_idpassw,
	                      pl_listeBAF->Config.Liste_Module[3].Module, vl_Etat4,
	                      pl_listeBAF->Config.Liste_Module[4].Module, vl_Etat5,
	                      pl_listeBAF->Config.Liste_Module[5].Module, vl_Etat6
			      );
     }
   }
   else	{ 

     /* si la fermeture est demand� sans le feux d'alerte alors rejet de la commande */
     if ((vl_Etat1[0]=='0')  && (vl_Etat3[0]!='0') )
     {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_BAF_cb : la fermeture est demand� sans le feux d'alerte ==> rejet de la commande");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAF_cb ");
      return;
     }

	/* test si pilotage global feux+barriere pour simple barriere */
     if ( (vl_Etat1[0]=='1') && (vl_Etat2[0]=='1') && (vl_Etat3[0]=='1'))
	sprintf(pl_mlcr,"P ID=%s AM=*.* AF=1",vg_idpassw);
     else
     if ( (vl_Etat1[0]=='0') && (vl_Etat2[0]=='0') && (vl_Etat3[0]=='0') )
	sprintf(pl_mlcr,"P ID=%s AM=*.* AF=0",vg_idpassw);
     else {
     sprintf(pl_mlcr,"P ID=%s AM=%s AF=%s AM=%s AF=%s AM=%s AF=%s",
   			      vg_idpassw,
	                      pl_listeBAF->Config.Liste_Module[0].Module, vl_Etat1,
	                      pl_listeBAF->Config.Liste_Module[1].Module, vl_Etat2,
	                      pl_listeBAF->Config.Liste_Module[2].Module, vl_Etat3
			      );
     }
   }
   /*A Envoi de la trame */
     if (!strcmp(pl_listeBAF->Config.AdresseIP, XZECC_IP_VIDE)){
   if (EnvoyerTrame(    pl_mlcr,
        		XZEXC_TRAME_CMD_BAF,
        		pl_listeBAF->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAF" );
      return;
   }
   }
   else {
   if (EnvoyerTrameIP(    pl_mlcr,
        		XZEXC_TRAME_CMD_BAF,
        		pl_listeBAF->Config.AdresseIP,
        		pl_listeBAF->Config.Port,
        		pl_listeBAF->Config.AdresseRGS,
		 	0,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAF" );
      return;
   }

   }
 
   /* Ecriture dans fichier Trace_CMD_BAF de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_BAF,
                           vl_NoEqtBAF, 
                           pl_mlcr,
                           NULL,
                           vl_Module+1 );
                           
   /*A envoi de la trame de relecture */
     if (!strcmp(pl_listeBAF->Config.AdresseIP, XZECC_IP_VIDE)){
   if (EnvoyerTrame(    "PS",
        		XZEXC_TRAME_ETAT_POS_BAF,
        		pl_listeBAF->Config.AdresseRGS,
		 	8,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      return;
   }
   }
   else {
   if (EnvoyerTrameIP(    "PS",
        		XZEXC_TRAME_ETAT_POS_BAF,
        		pl_listeBAF->Config.AdresseIP,
        		pl_listeBAF->Config.Port,
        		pl_listeBAF->Config.AdresseRGS,
		 	8,
		 	XZEXC_PRIORITEFAIBLE,
        		pl_listeBAF->Socket) == XDC_NOK ) 
   {
      return;
   }

   }
   
   /* Mise a jour memoire commande BAF */
   pl_listeBAF->NumCmd=vl_NoCMD;
   strcpy ( pl_listeBAF->EtatCmd.Module[0], vl_Etat1 ); 
   strcpy ( pl_listeBAF->EtatCmd.Module[1], vl_Etat2 ); 
   strcpy ( pl_listeBAF->EtatCmd.Module[2], vl_Etat3 ); 
   strcpy ( pl_listeBAF->EtatCmd.Module[3], vl_Etat4 ); 
   strcpy ( pl_listeBAF->EtatCmd.Module[4], vl_Etat5 ); 
   strcpy ( pl_listeBAF->EtatCmd.Module[5], vl_Etat6 ); 
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAF_cb ");
}
