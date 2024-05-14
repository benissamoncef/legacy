/*E*/
/*Fichier : $Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $       Release : $Revision: 1.16 $     Date : $Date: 2020/11/03 18:09:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module implementant les callback pour la tache TEGTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1 	19 Dec 1994	: Creation
* Mismer.D	version 1.2 	23 Jan 1995	:
* Volcic.F	version 1.3	09 Avr 1995 	: Modif cb J/N
* Volcic.F	version 1.4 	09 Avr 1995	: RAS
* Volcic.F	version 1.5	09 Avr 1995	: RAS
* Volcic.F	version 1.6	04 Mai 1995	:
* Volcic.F	version 1.7	04 Mai 1995	: RAS, Reprise entete et historisque
* Mismer.D	version 1.8 	08 Aou 1995	: Utilisation constante XDC_NBMAX_ETOR
* Mismer.D	version 1.9 	10 Oct 1995	: Ajout etat de service ETOR
* Mismer.D	version 1.10 	07 Fev 1995	: Ajout suppression alarme eqt HS
* Mismer.D	version 1.12 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	17/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.14
* JPL	05/06/18 : Migration Linux 64 bits (DEM 1274) : Arguments complets pour appel XZST_10ArchiverMsgSyst  1.15
* GR 08/07/2020 : EMS 1.16
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_mala.h"

#include "egtc_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM GTC.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	eg_Init_IHM_GTC_cb ( 
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
*  XDM_EInit_IHM_GTC 
*
* FONCTION 
*  Pour toutes les entrees TOR, envoyer l'etat par XDM_ETAT_GTC dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
static char *version = "$Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $ : eg_Init_IHM_GTC_cb";

   #ifndef _TIBCO_EMS
 T_STR      	pl_DGInit = "";
   #else
   XDY_Datagroup    pl_DGInit = "";
   #endif

   T_INT2	vl_TypeEqt = 0;
   T_INT2	vl_NumEqt = 0;
   T_INT2	vl_Etat_Etor = 0;
   int		i = 0;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: eg_Init_IHM_GTC_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_GTC */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING
 		,"messsage XDM_EInit_IHM_GTC non lu \n" );
 		
      XZST_10ArchiverMsgSyst(0,"eg_Init_IHM_GTC_cb : Message XDM_EInit_IHM_GTC recu avec parametre(s) invalide(s)", NULL);
      
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: eg_Init_IHM_GTC_cb");
      return;
   }
	        
	       
   /*A Pour chaque entree tor : envoi du msg XDM_ETAT_GTC */
  
   for ( i = 0; i < XDC_NB_ETOR; i++ )
   {
      /* si l'eqt est en service et actif */
      if ( !(vg_donneesGTC.Etat_Service) && !(vg_donneesGTC.ETOR[i].Etat_Service) ) 
      {
     	 vl_TypeEqt   = XDC_EQT_GTC;
     	 vl_NumEqt    = vg_donneesGTC.ETOR[i].NoEqtETor;
     	 vl_Etat_Etor = vg_donneesGTC.ETOR[i].Etat;
     	 if ( vl_NumEqt != 0 )
	 {
		if (!TipcSrvMsgWrite ( pl_DGInit,
      				        XDM_IdentMsg(XDM_ETAT_GTC),
                        	  	XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_INT2, vl_TypeEqt,
              	        	    	T_IPC_FT_INT2, vl_NumEqt,
                	          	T_IPC_FT_INT2, vl_Etat_Etor,
                          		NULL))
      		{
         		XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_GTC non effectue ." );
		}
	 	else
      	 	{
         		XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_GTC effectue ." );
         	}			
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:  eg_Init_IHM_GTC_cb");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	eg_Init_IHM_JourNuit_cb ( 
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
*  XDM_EInit_IHM_JourNuit 
*
* FONCTION 
*  envoyer l'etat par XDM_ETAT_Jour_Nuit dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
static char *version = "$Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $ : eg_Init_IHM_JourNuit_cb";

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup    pl_DGInit;
   #endif

   int				i;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: eg_Init_IHM_JourNuit_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_JourNuit */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING
 		,"messsage XDM_EInit_IHM_GTC non lu \n" );
 		
      XZST_10ArchiverMsgSyst(0,"eg_Init_IHM_JourNuit_cb : Message XDM_EInit_IHM_JourNuit recu avec parametre(s) invalide(s)", NULL);
      
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: eg_Init_IHM_JourNuit_cb");
      return;
   }
	        
	       
   /*A Recherche CELLULE JN et envoi du msg XDM_ETAT_JourNuit */
  
   for ( i = 0; i < XDC_NB_ETOR; i++ )
   {
      /* si l'eqt est en service et actif */
      if ( !(vg_donneesGTC.Etat_Service) && !(vg_donneesGTC.ETOR[i].Etat_Service) && (vg_donneesGTC.ETOR[i].Alerte == XZAAC_TOR_0) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_JourNuit),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, (vg_donneesGTC.ETOR[i].Etat==1) ? XDC_NUIT : XDC_JOUR,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_JourNuit non effectue" );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_JourNuit effectue" );
	 }
	 break;
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:  eg_Init_IHM_JourNuit_cb");
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
*  Pour touts les GTC , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $ :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup    pl_DGInit;
   #endif   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");
		  
   /*A Recuperation des parametres du message  */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Liaison non lu \n" );
      XZST_10ArchiverMsgSyst(0,"ep_Init_IHM_Liaison_cb : Message XDM_EInit_IHM_Liaison recu avec parametre(s) invalide(s)",NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb");
      return;
   }
	        
   /*A envoi du msg XDM_ETAT_LIAISON  */
  
   ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, vg_donneesGTC.Numero, XDC_EQT_GTC ); 

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	eg_controle_cb ( 
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
static char *version = "$Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $ : eg_controle_cb";

#ifndef _TIBCO_EMS
   T_STR		    va_NomMachine;
   T_INT4			va_Etat;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
#else
   XDY_NomMachine          va_NomMachine;
   XDY_Entier			va_Etat;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
#endif   

   int				vl_index = 0;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: eg_controle_cbe ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu." );
      return;
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING," callback ep_controle_cb, msg= %d %s %d %d",
                          va_Etat, va_NomMachine, va_TypeEqt, va_NumEqt);
   
   /*A si l'eqt est une mini-GTC */
   if ((va_TypeEqt == XDC_EQT_GTC) && (va_NumEqt == vg_donneesGTC.Numero))
   {
      if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
         vg_donneesGTC.Etat_Service &= va_Etat;
      else
         vg_donneesGTC.Etat_Service |= va_Etat;
      if ( (vg_donneesGTC.Etat_Service & XDC_EQT_HORS_SRV) || 
           (vg_donneesGTC.Etat_Service & XDC_EQT_INACTIF))
         ex_sup_alarme_eqt_HS ( vg_NomMachine, XDC_EQT_GTC, va_NumEqt,XZSC_22NumeroSiteLocal() );
   }   
   
   if ( va_TypeEqt == XDC_EQT_TOR )
   {
      for ( vl_index = 0; vl_index < XDC_NB_ETOR; vl_index++ )
      {
         if ( vg_donneesGTC.ETOR[vl_index].NoEqtETor == va_NumEqt )
         {
             if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
                vg_donneesGTC.ETOR[vl_index].Etat_Service &= va_Etat;
             else
                vg_donneesGTC.ETOR[vl_index].Etat_Service |= va_Etat;
             
             if ( (vg_donneesGTC.ETOR[vl_index].Etat_Service & XDC_EQT_HORS_SRV) || 
                  (vg_donneesGTC.ETOR[vl_index].Etat_Service & XDC_EQT_INACTIF))
                ex_sup_alarme_eqt_HS ( vg_NomMachine, XDC_EQT_TOR, va_NumEqt,XZSC_22NumeroSiteLocal() );
             break;
         }
      }   
   }
   
   /* Trace des Differents etats service et activite */
      XZST_03EcritureTrace(XZSTC_DEBUG1, "servgtc:%d", vg_donneesGTC.Etat_Service );
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: eg_controle_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	eg_CMD_LCR ( T_IPC_CONN			va_Cnx,
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
static char *version = "$Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $ : eg_CMD_LCR";

#ifndef _TIBCO_EMS   
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2			vl_TypeEqt;
   T_INT2			vl_NoEqt;
   T_INT2			vl_Periode;
   T_REAL8			vl_HrDebut;
   T_REAL8			vl_HrFin;
   T_STR			vl_NomMachine;
#else
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot			vl_TypeEqt;
   XDY_Mot			vl_NoEqt;
   XDY_Mot			vl_Periode;
   XDY_Horodate			vl_HrDebut;
   XDY_Horodate			vl_HrFin;
   XDY_NomMachine               vl_NomMachine;
#endif

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
   			T_IPC_FT_REAL8, &vl_HrDebut,
   			T_IPC_FT_REAL8, &vl_HrFin,
   			T_IPC_FT_STR,	&vl_NomMachine,
	          	NULL))

      	{       
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING
 	   ,"messsage XDM_ELCR non lu \n" );
         				
           XZST_10ArchiverMsgSyst(0,"ep_CMD_LCR : Message XDM_ELCR(GTC) recu avec parametre(s) invalide(s)", NULL);
      
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_CMD_LCR");
           return;
	}
	      
        XZST_03EcritureTrace(XZSTC_WARNING, "IN eg_CMD_LCR");
	
       
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
        if ( (vl_TypeEqt == XDC_EQT_GTC) &&
             (ex_test_autorisation_eqt ( &pg_DebutListeGTCFLcr, 
        				 &pl_eqt,
        				 XDC_EQT_GTC) == XDC_NOK) )
        {
           return;
        }


	   if (vl_TypeEqt == XDC_EQT_GTC)
	   {
	      if ( (vg_donneesGTC.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	      {
        	    ex_init_lcr ( 	&pg_DebutListeGTCFLcr,
        	    			pl_eqt,
        	    			vg_donneesGTC.Socket,
        	    			vg_donneesGTC.AdresseRGS,
        	    			vg_donneesGTC.Numero,
        	    			"GTC_FLcr_NoEqt:",
        	    			vg_SockTIM );
        	    			
              }
              else
	      {
                 XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
              }
	   }
           else
	   {
              XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
           }
        
   	/* Fin du callback */
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback eg_CMD_LCR" );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Commande d'une sortie TOR sur reception du message XDM_ECMD_GTC en provenance d'une tache TEDAI.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	eq_commande_sortie_TOR ( 
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
*  XDM_ECMD_GTC 
*
* FONCTION 
*  Commande d'une sortie TOR sur reception du message XDM_ECMD_GTC en provenance d'une tache TEDAI.
*
------------------------------------------------------*/
{
static char *version = "$Id: egtc_cmd.c,v 1.16 2020/11/03 18:09:34 pc2dpdy Exp $ : eq_commande_sortie_TOR";

#ifdef _TIBCO_EMS
   XDY_Mot			va_Mot;
   XDY_Mot			va_Bit;
   XDY_Mot			va_Etat;
   XDY_Entier			va_Duree;
#else
   T_INT2			va_Mot;
   T_INT2			va_Bit;
   T_INT2			va_Etat;
   T_INT4			va_Duree;
#endif
   XDY_Texte			vl_TrameCommande = "";
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: eq_commande_sortie_TOR ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_GTC */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2,&va_Mot
                          ,T_IPC_FT_INT2,&va_Bit
                          ,T_IPC_FT_INT2,&va_Etat
                          ,T_IPC_FT_INT4,&va_Duree
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_GTC non lu." );
      return;
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING,"IN: Callback eq_commande_sortie_TOR, Mot:%d Bit:%d Etat:%d Duree:%d",
                          va_Mot, va_Bit, va_Etat, va_Duree );
   
   sprintf ( vl_TrameCommande, "P ID=%s AM=%d.%d AF=%d DV=%d", 
   	     vg_idpassw, va_Mot, va_Bit, va_Etat, va_Duree );
   EnvoyerTrame ( vl_TrameCommande,
                  XZEXC_TRAME_CMD_GTC,
                  vg_donneesGTC.AdresseRGS,
                  XZEXC_NOCMD,
	          XZEXC_PRIORITEFAIBLE,
                  vg_donneesGTC.Socket );
     
   /* Ecriture dans fichier Trace_CMD_GTC de la commande */    
   ex_ecriture_trace_cmd ( XDC_EQT_GTC,
                           vg_donneesGTC.Numero, 
                           vl_TrameCommande,     
                           NULL,
                           0 );
   
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: eq_commande_sortie_TOR ");
}
