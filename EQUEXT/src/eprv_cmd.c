/*E*/
/*Fichier : $Id: eprv_cmd.c,v 1.3 2020/11/03 18:12:20 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 18:12:20 $
------------------------------------------------------
------------------------------------------------------
* GTIE * PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de traitement des commandes recues par la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
* JMG	13/08/12	: correction trace 1.2
* JMG	15/05/20	: EMS 1.3
* GR    10/07/20        : EMS 1.4
* ABE	31/05/21	: PRV sur IP DEM-SAE283 1.5
* ABE	25/10/21	: Fix appel init_lcr_ip pour PRV sur ip DEM-SAE283 1.6
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "eprv_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
void supprime_blancs_fin(char *);

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PRV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_Init_IHM_contraste_cb ( 
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
*  Pour touts les PMV et picto , envoyer l'etat  par XDM_ETAT_Contraste dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      : ep_Init_IHM_contraste_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPRV_DONNEES_PRV *		pl_listeprv;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_contraste_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_Contraste */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Contraste non lu" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_contraste_cb");
      return;
   }
	       

   /*A Pour chaque pmv : envoi du msg XDM_ETAT_Contraste si il est en service et actif */
  
   for ( pl_listeprv = pg_debutPRV; pl_listeprv != NULL; pl_listeprv = pl_listeprv->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_listeprv->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XDC_EQT_PRV,
                          	T_IPC_FT_INT2, pl_listeprv->Config.Numero.NoEqt,
                          	T_IPC_FT_INT2, pl_listeprv->Contraste,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_Contraste effectue." );
	 }
      }	
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_contraste_cb ");
	       
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
*  Pour touts les PMV et picto , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPRV_DONNEES_PRV *		pl_listeprv;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");
		  
   /*A Recuperation des parametres du message  */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Liaison non lu \n" );
      XZST_10ArchiverMsgSyst(0,"ep_Init_IHM_contraste_cb : Message XDM_EInit_IHM_Liaison recu avec parametre(s) invalide(s)",NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb");
      return;
   }
	       

   /*A Pour chaque pmv : envoi du msg XDM_ETAT_LIAISON */
  
   for ( pl_listeprv = pg_debutPRV; pl_listeprv != NULL; pl_listeprv = pl_listeprv->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_listeprv->Config.Numero.NoEqt, XDC_EQT_PRV ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_Init_IHM_PRV_cb ( 
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
*  XDM_EInit_IHM_PMV 
*
* FONCTION 
*  Pour touts les PMV, envoyer l'etat PMV par XDM_ETAT_PMV dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      : ep_Init_IHM_PRV_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPRV_DONNEES_PRV *		pl_DonCrt;
   int				vl_NbLigne = 0;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_PRV_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_PRV */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_PRV non lu." );
      XZST_10ArchiverMsgSyst(0,"ep_Init_IHM_PRV_cb : Message XDM_EInit_IHM_PRV recu avec parametre(s) invalide(s)",NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PRV_cb");
      return;
   }
	        
	       
   /*A Pour chaque prv : envoi du msg XDM_ETAT_PRV */

   for ( pl_DonCrt = pg_debutPRV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_INFO,"%d",pl_DonCrt->Config.Numero.NoEqt);
      /*if ( !(pl_DonCrt->Etat_Service & XDC_EQT_HORS_SRV) )
      {*/
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_PRV),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_DonCrt->Config.Numero.NoEqt,
                         	T_IPC_FT_STR,  pl_DonCrt->Etat.Bandeau,
                         	T_IPC_FT_STR,  pl_DonCrt->Etat.Vitesse,
                           	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PRV non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_PRV effectue." );
	 }
      /*}	*/
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PRV_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande affichage PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	ep_cmd_PRV_cb ( T_IPC_CONN			va_Cnx,
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
*  XDM_ECMD_PMV
*
* FONCTION 
*  Demande d'affichage PMV
*
------------------------------------------------------*/
{
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      :  ep_cmd_PRV_cb " ;

#ifndef _TIBCO_EMS   
   T_STR			vl_Vitesse;
   T_STR			vl_Bandeau;
   T_STR			vl_NomMachine;
   T_INT4			vl_NoCMD;
   T_INT2			vl_Numero;
#else
   XDY_VitesseRegulTxt		vl_Vitesse;
   XDY_Bandeau                  vl_Bandeau;
   XDY_NomMachine		vl_NomMachine;
   XDY_Entier			vl_NoCMD;
   XDY_Mot			vl_Numero;
#endif

   char			str_vitesse[20];

   EPRV_DONNEES_PRV		*pl_donneeprv;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   XZEXT_Trame 			pl_mlcr = "",
   				vl_compstr = "";
   int				vl_TypeAff = 0;
   XZEXT_Trame 			pl_mlcr_lg1 = "";
   char			str_bandeau[20];

   T_IPC_MSG msg = data->msg;
        
        /*A Lecture du message */
	XZST_03EcritureTrace( XZSTC_INFO, "ep_cmd_PRV_cb : Avant affichage message");

	XZST_03EcritureTrace( XZSTC_INFO, "ep_cmd_PRV_cb: Lecture du message %s", data->msg);

   	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_STR,  &vl_Bandeau
                          ,T_IPC_FT_STR,  &vl_Vitesse
                          ,T_IPC_FT_INT4, &vl_NoCMD
                          ,NULL))
	          
      	{ 
      	   /* Trace : Impossibilite de lire le message */
   			XZST_03EcritureTrace( XZSTC_INFO, "ep_cmd_PRV_cb: num : %d \n vitesse : %s \n bandeau : %s \n noCMD : %d",
   													   vl_Numero,
   													   vl_Vitesse,
   													   vl_Bandeau,
   													   vl_NoCMD);

           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_PRV non lu \n" );
           XZST_10ArchiverMsgSyst(0,"ep_cmd_PRV_cb : Message XDM_CMD_PRV recu avec parametre(s) invalide(s)",NULL);
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PRV_cb");
           return;
	}
   	else
   	{
   		XZST_03EcritureTrace( XZSTC_WARNING, "ep_cmd_PRV_cb: message lu: num : %d \n vitesse : %s \n bandeau : %s \n noCMD : %d",
   		   													   vl_Numero,
   		   													   vl_Vitesse,
   		   													   vl_Bandeau,
   		   													   vl_NoCMD);
   	}
	
	supprime_blancs_fin(vl_Vitesse);
	supprime_blancs_fin(vl_Bandeau);
        /* Trace du message en entree de la fonction */
    XZST_03EcritureTrace(XZSTC_WARNING
 	     ,"IN Callback ep_cmd_PRV_cb, msg= %d V\"%s\" B\"%s\" %d",
                           		vl_Numero,
                           		vl_Vitesse,
                           		vl_Bandeau,
                           		vl_NoCMD );
 		                            
	/*A Lecture vl_horodate en cas d'erreur */
	XZSM_07LireHorodate( XZSMC_TYPE_RTWORKS , &vl_js , &vl_horodate );

	/*A Recherche du No d'equipement dans la structure config_prv en memoire */
	for ( pl_donneeprv = pg_debutPRV; pl_donneeprv != NULL ; 
	         pl_donneeprv = pl_donneeprv->Suivant )
	{
		if (pl_donneeprv->Config.Numero.NoEqt == vl_Numero)
	      break;
	}
  	
	if (pl_donneeprv == NULL)
	{
	   /* Trace du message incorrect */
           XZST_03EcritureTrace(XZSTC_WARNING, "ep_cmd_PRV_cb: Message XDM_ECMD_PRV incorrect (erreur equipement %d) .",
                           		vl_Numero );

	   /* Trace : erreur equipement introuvable */
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PRV_cb : erreur equipement" );
 		
	   /* Envoi du CR NOK a la base */
           XZST_03EcritureTrace(XZSTC_INTERFACE
 		,"ep_cmd_PRV_cb: Appel xzat520_CompteRendu_PRV avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
	   if ( vl_NoCMD != 0 ) xzat520_CompteRendu_PRV(vl_NoCMD,vl_horodate,XDC_NOK,0);
	   return;
	}
 			
  	/* Si equipement HS ou Inactif -> pas d'envoi au picto */
	/*if ( ((pl_donneeprv->Etat_Service & XDC_EQT_HORS_SRV) != 0) ||
	     (((pl_donneeprv->Etat_Service & XDC_EQT_INACTIF) != 0) && (vl_NoCMD != 0)) ||
	     (((pl_donneeprv->Etat_Service & XDC_EQT_INHIBE) != 0) && (vl_NoCMD != 0)))
	{
           XZST_03EcritureTrace(XZSTC_WARNING, "ep_cmd_PRV_cb: Message XDM_ECMD_PMV incorrect (equipement hs ou inactif Eqt:%d) .",
                           		vl_Numero );

           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PRV_cb : equipement hs ou inactif" );
           XZST_03EcritureTrace(XZSTC_INTERFACE, "ep_cmd_PRV_cb Appel xzat520_CompteRendu_PRV avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
	   if ( vl_NoCMD != 0 ) xzat520_CompteRendu_PRV(vl_NoCMD,vl_horodate,XDC_NOK,pl_donneeprv->Config.SiteGestion);
	   return;
	}
 	*/	

		if (!strcmp(vl_Vitesse ,"L0"))
		{
			strcpy(str_vitesse,"0");
		}
		else
		{
			strcpy(str_vitesse, vl_Vitesse);
		}

		if(!strcmp(vl_Bandeau , ""))
		{
			strcpy(str_bandeau , "0");
		}

		else
		{
			sprintf(str_bandeau, "\"%s\"",vl_Bandeau);
		}

		XZST_03EcritureTrace(XZSTC_WARNING, "vitesse = %s // bandeau = %s", str_vitesse, str_bandeau);

		sprintf(pl_mlcr_lg1,"P ID=SAE/MIG AM=1.3 AF=%s AM=1.2 AF=%s",
				str_vitesse,
				str_bandeau);

	   // sprintf(pl_mlcr,        "P ID=%s %s", vg_idpassw, pl_mlcr_lg1 );
	      XZST_03EcritureTrace(XZSTC_WARNING, "Callback ep_cmd_PRV_cb = trame : %s" ,pl_mlcr_lg1);
 	
      /*A Envoyer trame a la tache TELMi */	
	if ( !vg_Simulation )
	{
		/*A Envoi de la trame */ 
		if (!strcmp(pl_donneeprv->Config.AdresseIP, XZECC_IP_VIDE)) 
		{ 
			if (EnvoyerTrame(    pl_mlcr_lg1, 
				XZEXC_TRAME_AFF, 
				pl_donneeprv->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pl_donneeprv->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
				return; 
			} 
		} 
		else 
		{ 
			if (EnvoyerTrameIP(    pl_mlcr_lg1, 
				XZEXC_TRAME_AFF, 
				pl_donneeprv->Config.AdresseIP, 
				pl_donneeprv->Config.Port, 
				pl_donneeprv->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pl_donneeprv->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
				return; 
			}
		}
	}
              		       
            
      /* Ecriture dans fichier Trace_CMD_PRV de la commande */
      ex_ecriture_trace_cmd ( XDC_EQT_PRV,
                              vl_Numero, 
                              pl_mlcr_lg1,
                              NULL,
                              vl_NoCMD );
   
      /*A Demande de relecture pour contr�le de la bonne execution et envoi contraste */
         if ( em_envoyer_contraste_prv ( pl_donneeprv, 
                                      pl_donneeprv->Contraste,
                                      (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE) == XDC_NOK )
         {
			XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" );
			if ( vl_NoCMD != 0 )
				xzat520_CompteRendu_PRV(vl_NoCMD,vl_horodate,XDC_NOK,pl_donneeprv->Config.SiteGestion);
			XZST_10ArchiverMsgSyst(0," ep_cmd_PRV_cb: Impossible d'envoyer la commande", NULL);
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PRV_cb");
			return;
         }
       

       /* Mise a jour memoire etat commande */
       pl_donneeprv->NumDerCmd = (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE;
       strcpy(pl_donneeprv->Etat_Commande.Vitesse,vl_Vitesse);
       strcpy(pl_donneeprv->Etat_Commande.Bandeau,vl_Bandeau);
       pl_donneeprv->Etat_Commande.NoCMD  = vl_NoCMD;

       if ( vg_Simulation )
       {
          memcpy ( &pl_donneeprv->Etat, &pl_donneeprv->Etat_Commande, sizeof(XDY_Etat_PRV) );
          ep_envoi_affichage_PRV ( pl_donneeprv->Etat, vl_Numero, 5 );
          if ( vl_NoCMD != 0 )
        	  xzat520_CompteRendu_PRV(vl_NoCMD,vl_horodate,XDC_OK,pl_donneeprv->Config.SiteGestion);
       }
       /* Fin du callback */
       XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PRV_cb " );
 	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le pmv
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	em_envoyer_contraste_prv(   	EPRV_DONNEES_PRV *		pa_ListePMV,
                         		int                            va_Contraste,
                         		int				va_NumCmd )
/*
* ARGUMENTS EN ENTREE :
*  pa_ListePMV : pointeur sur liste pmv
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
*  Envoi de la trame de contraste a telmi pour le pmv
*
------------------------------------------------------*/
{
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      :  em_envoyer_contraste_pmv " ;

   XZEXT_Trame 			vl_lcr = "";
   char 			vl_Contraste [3];
   XDY_Datagroup		pl_DG;
   int				vl_ValRet = XDC_OK;

               /*A Choix de la commande en fonction du contexte jour nuit */
               switch ( va_Contraste )
               {
                  case XDC_CONTRASTEBRILLANT :
                      strcpy ( vl_Contraste, "S" );
                      break;
		  default:
                      strcpy ( vl_Contraste, (vg_Contexte == XDC_JOUR) ? "J1" : (vg_Contexte == XDC_NUIT) ? "N1" : "A" );
                      break;
               }
                   

               /*A envoi de la trame de contraste */
	       sprintf( vl_lcr,"P ID=%s AM=* EC=%s", vg_idpassw, vl_Contraste );
                if ( !vg_Simulation )
		{
			/*A Envoi de la trame */ 
			if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE)) 
			{ 
				if (EnvoyerTrame(vl_lcr,
            		        	XZEXC_TRAME_CONTRASTE,
					pa_ListePMV->Config.AdresseRGS,
					XZEXC_NOCMD, 
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePMV->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
					return; 
				} 
			} 
			else 
			{ 
				if (EnvoyerTrameIP(vl_lcr,
            		        	XZEXC_TRAME_CONTRASTE,
					pa_ListePMV->Config.AdresseIP, 
					pa_ListePMV->Config.Port, 
					pa_ListePMV->Config.AdresseRGS, 
					XZEXC_NOCMD, 
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePMV->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
					return; 
				}
			}
		}
           	 /* EnvoyerTrame( vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListePMV->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListePMV->Socket);*/
		               

		/*A Envoi de la trame */ 
		if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE)) 
		{ 
			if (EnvoyerTrame("ST",
				XZEXC_TRAME_ETAT_AFF_PRV, 
				pa_ListePMV->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pa_ListePMV->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
				return; 
			} 
		} 
		else 
		{ 
			if (EnvoyerTrameIP("ST",
				XZEXC_TRAME_ETAT_AFF_PRV, 
				pa_ListePMV->Config.AdresseIP, 
				pa_ListePMV->Config.Port, 
				pa_ListePMV->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pa_ListePMV->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
				return; 
			}
		}

           	 /* vl_ValRet = EnvoyerTrame( "ST",
        		        XZEXC_TRAME_ETAT_AFF_PRV,
        		        pa_ListePMV->Config.AdresseRGS,
        		        XZEXC_NOCMD,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePMV->Socket);*/

	       /*A envoi de la trame de relecture */
               if ( !vg_Simulation )
		{
			if (!strcmp(pa_ListePMV->Config.AdresseIP, XZECC_IP_VIDE)) 
			{ 
				if (EnvoyerTrame( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PRV,
					pa_ListePMV->Config.AdresseRGS, 
					va_NumCmd,
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePMV->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
					return; 
				} 
			} 
			else 
			{ 
				if (EnvoyerTrameIP( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PRV,
					pa_ListePMV->Config.AdresseIP, 
					pa_ListePMV->Config.Port, 
					pa_ListePMV->Config.AdresseRGS, 
					va_NumCmd,
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePMV->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PRV" ); 
					return; 
				}
			}
		}
           	 /* vl_ValRet = EnvoyerTrame( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PRV,
        		        pa_ListePMV->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePMV->Socket);*/

     	       if ( vg_Simulation )
     	       {
     	          pa_ListePMV->Contraste = va_Contraste;
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite) ;			
     	          TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XDC_EQT_PRV,
                          	T_IPC_FT_INT2, pa_ListePMV->Config.Numero.NoEqt,
                          	T_IPC_FT_INT2, pa_ListePMV->Contraste,
                          	NULL);
		}

 	       /*A Si envoi a telmi impossible alors init connexion socket avec telmi */       
               if ( vl_ValRet != XDC_OK ) ec_init_cnx ();	
                       
               pa_ListePMV->ContrasteCmd = va_Contraste;
               return ( vl_ValRet );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de changement de contraste .
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_contraste_cb ( 
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
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      : ep_contraste_cb " ;

#ifdef _TIBCO_EMS
   XDY_Mot 			va_Contraste;		 
   XDY_Mot 			va_NoEqt;		
   XDY_Mot 			va_TypeEqt;		
#else
   T_INT2 			va_Contraste;		 
   T_INT2 			va_NoEqt;		
   T_INT2 			va_TypeEqt;		
#endif
   EPRV_DONNEES_PRV *		pl_ListePMV;
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
        XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_Contraste non lu \n" );
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb");
        return;
      } 
	       
      switch(va_TypeEqt)
      {
      case XZECC_TYPE_EQT_PRV:
         /*B pour tous les pmv envoi de la trame de contraste */
         for ( pl_ListePMV = pg_debutPRV; pl_ListePMV != NULL; pl_ListePMV = pl_ListePMV->Suivant )
         {
	    if ( (va_NoEqt == XZEPC_SURB_EQTALL) || (pl_ListePMV->Config.Numero.NoEqt == va_NoEqt) )
	    {
	       /*A Composition de la trame contraste et envoi */
               em_envoyer_contraste_prv(pl_ListePMV,va_Contraste,XZEXC_CMD_HORS_BASE);
            }
            if ( pl_ListePMV->Config.Numero.NoEqt == va_NoEqt ) break;
         }
         break;
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

void	ep_controle_cb ( 
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
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      : ep_controle_cb  " ;

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

   EPRV_DONNEES_PRV *		pl_ListePMV;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu \n" );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_INFO, "callback ep_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
	/*A si l'eqt est un prv */
	if (va_TypeEqt ==  XZECC_TYPE_EQT_PRV)
	{
		/*B Recherche du No d'equipement dans la structure config_pmv en memoire */
		for ( pl_ListePMV = pg_debutPRV; pl_ListePMV != NULL ;
		pl_ListePMV = pl_ListePMV->Suivant )
		{
			if (pl_ListePMV->Config.Numero.NoEqt == va_NumEqt)
			{
				if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
					pl_ListePMV->Etat_Service &= va_Etat;
				else
					pl_ListePMV->Etat_Service += va_Etat;

				if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
				{
					pl_ListePMV->Etat_Service = va_Etat;
				}
				if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
				{
					pl_ListePMV->Etat_Service = va_Etat;
				}
				break;
			}
		}
	}

   


   for ( pl_ListePMV = pg_debutPRV; pl_ListePMV != NULL ; 
	         pl_ListePMV = pl_ListePMV->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servprv:%d ",pl_ListePMV->Etat_Service);
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_controle_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Changement de contexte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_journuit ( XZEAT_JourNuit va_contexte, int va_resultat)
 
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
   static char *version = "@(#)eprv_cmd.c	1.29 05/31/00      : ep_journuit  " ;
   XZEXT_Trame 			vl_lcr = "";
   char				vl_Contraste [3];
   EPRV_DONNEES_PRV *		pl_ListePMV;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
      vg_Contexte=XDC_JOUR;
   else
      vg_Contexte=XDC_NUIT;
   
   XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d", vg_Contexte );
 		
   /*B Pour tout les PMV en memoire */
   for ( pl_ListePMV = pg_debutPRV; pl_ListePMV != NULL ; 
	    pl_ListePMV = pl_ListePMV->Suivant )
   {
       /* Envoyer nouveau contraste */
       em_envoyer_contraste_prv( pl_ListePMV, pl_ListePMV->Contraste, XZEXC_CMD_HORS_BASE );
   }
   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande d'affichage picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void    ep_CMD_LCR ( T_IPC_CONN                 va_Cnx,
                     T_IPC_CONN_PROCESS_CB_DATA         data,
                     T_CB_ARG                           arg)


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
   static char *version = "@(#)epmv_cmd.c       1.29 05/31/00      : ep_CMD_LCR" ;

#ifndef _TIBCO_EMS   
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2                       vl_TypeEqt;
   T_INT2                       vl_NoEqt;
   T_INT2                       vl_Periode;
   T_REAL8                      vl_HrDebut;
   T_REAL8                      vl_HrFin;
   T_STR			vl_NomMachine;
#else
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot                      vl_TypeEqt;
   XDY_Mot                      vl_NoEqt;
   XDY_Mot                      vl_Periode;
   XDY_Horodate                 vl_HrDebut;
   XDY_Horodate                 vl_HrFin;
   XDY_NomMachine		vl_NomMachine;
#endif   


   EPRV_DONNEES_PRV             *pl_ListePMV;
   INFO_EQT_FEN_LCR             pl_eqt, *pl_ListeEqt;
   XZEXT_MSG_SOCKET             pl_MsgTimer;
   XZEXT_CLE_PROG               vl_clef;

        /*A Lecture du message */
   XZST_03EcritureTrace(XZSTC_WARNING ,"IN :ep_CMD_LCR"  );
        if(!TipcMsgRead(data->msg,
                        T_IPC_FT_INT2,  &vl_TypeEqt,
                        T_IPC_FT_INT2,  &vl_NoEqt,
                        T_IPC_FT_STR,   &vl_FicCmdLcr,
                        T_IPC_FT_STR,   &vl_FicResLcr,
                        T_IPC_FT_INT2,  &vl_Periode,
                        T_IPC_FT_REAL8, &vl_HrDebut,
                        T_IPC_FT_REAL8, &vl_HrFin,
                        T_IPC_FT_STR,   &vl_NomMachine,
                        NULL))

        {
           /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ELCR non lu \n" );
           XZST_10ArchiverMsgSyst(0,"ep_CMD_LCR : Message XDM_ELCR(Pmv Picto) recu avec parametre(s) invalide(s)",NULL);
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_CMD_LCR");
           return;
        }

        /* Trace du message en entree de la fonction */
        XZST_03EcritureTrace(XZSTC_WARNING,"IN ep_CMD_LCR");


        /*A Construction de la liste globale des eqt a piloter par la fenetre LCR */

        pl_eqt.TypeEqt=vl_TypeEqt;
        strcpy(pl_eqt.NomFichCmdesLCR,vl_FicCmdLcr);
        strcpy(pl_eqt.NomFichResLCR,vl_FicResLcr);
        pl_eqt.Periode=vl_Periode;
        pl_eqt.HrDebut=vl_HrDebut;
        pl_eqt.HrFin=vl_HrFin;
        strcpy(pl_eqt.NomMachine,vl_NomMachine);
        pl_eqt.NoEqt=vl_NoEqt;
        pl_eqt.Position=0;
        pl_eqt.Suivant=NULL;

        /* Recherche si pas de commande LCR en cours sur equipement concerne */
        if ( (vl_TypeEqt == XDC_EQT_PRV) &&
             (ex_test_autorisation_eqt ( &pg_DebutListePRVFLcr,
                                         &pl_eqt,
                                         XDC_EQT_PRV) == XDC_NOK) )
        {
           return;
        }
              for ( pl_ListePMV = pg_debutPRV; pl_ListePMV != NULL ;
                    pl_ListePMV = pl_ListePMV->Suivant )
              {
                 if ( (pl_ListePMV->Config.Numero.NoEqt == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
                 {
                     if (strcmp(pl_ListePMV->Config.AdresseIP, XZECC_IP_VIDE)) {
                        ex_init_lcr_ip (  &pg_DebutListePRVFLcr,
                                          pl_eqt,
                                          pl_ListePMV->Socket,
                                          pl_ListePMV->Config.AdresseRGS,
                                          pl_ListePMV->Config.Numero.NoEqt,
                                          "PRV_FLcr_NoEqt:",
                                          vg_SockTIM,
                                          pl_ListePMV->Config.AdresseIP,
                                          pl_ListePMV->Config.Port);
                     }
                     else
                     {
                        ex_init_lcr (   &pg_DebutListePRVFLcr,
                                        pl_eqt,
                                        pl_ListePMV->Socket,
                                        pl_ListePMV->Config.AdresseRGS,
                                        pl_ListePMV->Config.Numero.NoEqt,
                                        "PRV_FLcr_NoEqt:",
                                        vg_SockTIM );
                     }

                 }
                 if (pl_ListePMV->Config.Numero.NoEqt == vl_NoEqt)
                   break;
              }
              if ( (pl_ListePMV == NULL) && (vl_NoEqt!=XDC_EQTALL) )
              {
                 XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
              }
        /* Fin du callback */
        XZST_03EcritureTrace(XZSTC_WARNING, "OUT Callback ep_CMD_LCR" );

}

void supprime_blancs_fin(char *c)

/*
* ARGUMENTS EN ENTREE :
*       T_STR           Ligne;
*
* ARGUMENTS EN SORTIE :
*       T_STR           Ligne;
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   parcours de la chaine
*
------------------------------------------------------*/
{
        int     i=0, j=0, k=0;
	int trouve =0;

	int q;
	q=strlen(c); 
	while(q>1 && c[--q] == ' ') 
		c[q]='\0';
		
	q=0;
	while (q<strlen(c)) {
	  if (c[q]!=' ') {trouve=1; break;}
	  q++;
	}
	if (trouve==0)
	 	c[0]='\0'; 
}

