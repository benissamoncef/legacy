/*Fichier : $Id: epic_cmd.c
------------------------------------------------------
------------------------------------------------------
* GTIE * PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPIC * FICHIER epic_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de traitement des commandes recues par la tache TEPIC
*
------------------------------------------------------
* HISTORIQUE :
*
* A.BENSEDDIK  	17/08/23		: Creation (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include "epic_cmd.h"

/* definitions de constantes */
/* definitions de types locaux */
/* definition de macro locales */
/* declaration de variables locales */
/* declaration de fonctions internes */
void supprime_blancs_fin(char *);
/* definition de fonctions externes */

/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PIC.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_Init_IHM_contraste_cb (	T_IPC_CONN 					va_Cnx,
								T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
								T_CB_ARG					pa_ARG)
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelée lors de la réception du message
*  XDM_EInit_IHM_Contraste 
*
* FONCTION 
*  Pour touts les PIC , envoyer l'etat  par XDM_ETAT_Contraste dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
	static char *version = "@(#)epic_cmd.c	:	ep_Init_IHM_contraste_cb" ;

	#ifndef _TIBCO_EMS
		T_STR pl_DGInit;
	#else
		XDY_Datagroup pl_DGInit;
	#endif   

	EPIC_DONNEES_PIC *		pl_listepic;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_contraste_cb ");
		  
	/* Recuperation des parametres du message XDM_EInit_IHM_Contraste */
	if(!TipcMsgRead(	pa_Data -> msg,
					T_IPC_FT_STR,  &pl_DGInit,
					NULL))
 	{       
 		/* Trace : Impossibilite de lire le message */
 		XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_Contraste non lu" );
 		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_contraste_cb");
 		return;
 	}

	/* Pour chaque picto : envoi du msg XDM_ETAT_Contraste si il est en service et actif */
	for ( pl_listepic = pg_debutPIC; pl_listepic != NULL; pl_listepic = pl_listepic->Suivant )
	{
		/* si l'eqt est en service et actif */
		if ( !(pl_listepic->Etat_Service & XDC_EQT_HORS_SRV) )
		{
			if (!TipcSrvMsgWrite (	pl_DGInit,
									XDM_IdentMsg(XDM_ETAT_Contraste),
									XDM_FLG_SRVMSGWRITE,
									T_IPC_FT_INT2, XDC_EQT_PIC,
									T_IPC_FT_INT2, pl_listepic->Config.Numero.NoEqt,
									T_IPC_FT_INT2, pl_listepic->Contraste,
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


/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM Liaison.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void	ep_Init_IHM_Liaison_cb (	T_IPC_CONN 					va_Cnx,
									T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
									T_CB_ARG					pa_ARG)
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
*  Fonction appelée lors de la réception du message
*  XDM_EInit_IHM_Liaison
*
* FONCTION 
*  Pour touts les pictos , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPIC_DONNEES_PIC *		pl_listepic;
   

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
	       

   /*A Pour chaque picto : envoi du msg XDM_ETAT_LIAISON */
  
   for ( pl_listepic = pg_debutPIC; pl_listepic != NULL; pl_listepic = pl_listepic->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_listepic->Config.Numero.NoEqt, XDC_EQT_PIC ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PIC.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_Init_IHM_PIC_cb ( 
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
*  XDM_EInit_IHM_PIC 
*
* FONCTION 
*  Pour touts les Picto, envoyer l'etat PIC par XDM_ETAT_PIC dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      : ep_Init_IHM_PIC_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPIC_DONNEES_PIC *		pl_DonCrt;
   int				vl_NbLigne = 0;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_PIC_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_PIC */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_PIC non lu." );
      XZST_10ArchiverMsgSyst(0,"ep_Init_IHM_PIC_cb : Message XDM_EInit_IHM_PIC recu avec parametre(s) invalide(s)",NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PIC_cb");
      return;
   }
	        
	       
   /*A Pour chaque pic : envoi du msg XDM_ETAT_PIC */

   for ( pl_DonCrt = pg_debutPIC; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_INFO,"%d",pl_DonCrt->Config.Numero.NoEqt);
      /*if ( !(pl_DonCrt->Etat_Service & XDC_EQT_HORS_SRV) )
      {*/
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_PIC),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_DonCrt->Config.Numero.NoEqt,
                         	T_IPC_FT_STR,  pl_DonCrt->Etat.Bandeau,
                         	T_IPC_FT_STR,  pl_DonCrt->Etat.Flash,
                         	T_IPC_FT_STR,  pl_DonCrt->Etat.Picto,
                           	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PIC non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_PIC effectue." );
	 }
      /*}	*/
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PIC_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande affichage PIC.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	ep_cmd_PIC_cb ( T_IPC_CONN			va_Cnx,
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
*  XDM_ECMD_PIC
*
* FONCTION 
*  Demande d'affichage PIC
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      :  ep_cmd_PIC_cb " ;

#ifndef _TIBCO_EMS   
   T_STR			vl_NomMachine;
   T_INT4			vl_NoCMD;
   T_INT2			vl_Numero;
#else
   XDY_NomMachine		vl_NomMachine;
   XDY_Entier			vl_NoCMD;
   XDY_Mot			vl_Numero;
#endif

   EPIC_DONNEES_PIC		*pl_donneepic;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   XZEXT_Trame 			pl_mlcr = "",
   				vl_compstr = "";
   int				vl_TypeAff = 0;
   XZEXT_Trame 			pl_mlcr_lg1 = "";

   T_IPC_MSG msg = data->msg;
        
        /*A Lecture du message */
	XZST_03EcritureTrace( XZSTC_INFO, "ep_cmd_PIC_cb : Avant affichage message");

	XZST_03EcritureTrace( XZSTC_INFO, "ep_cmd_PIC_cb: Lecture du message %s", data->msg);

   	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_INT4, &vl_NoCMD
                          ,NULL))
	          
      	{ 
      	   /* Trace : Impossibilite de lire le message */
   			XZST_03EcritureTrace( XZSTC_INFO, "ep_cmd_PIC_cb: num : %d \n noCMD : %d",
   													   vl_Numero,
   													   vl_NoCMD);

           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_PIC non lu \n" );
           XZST_10ArchiverMsgSyst(0,"ep_cmd_PIC_cb : Message XDM_CMD_PIC recu avec parametre(s) invalide(s)",NULL);
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PIC_cb");
           return;
	}
   	else
   	{
         XZST_03EcritureTrace( XZSTC_WARNING, "ep_cmd_PIC_cb: message lu: num : %d \n noCMD : %d",
   		   													   vl_Numero,
   		   													   vl_NoCMD);
   	}
	
        /* Trace du message en entree de la fonction */
    XZST_03EcritureTrace(XZSTC_WARNING
 	     ,"IN Callback ep_cmd_PIC_cb, msg= %d %d",
                           		vl_Numero,
                           		vl_NoCMD );
 		                            
	/*A Lecture vl_horodate en cas d'erreur */
	XZSM_07LireHorodate( XZSMC_TYPE_RTWORKS , &vl_js , &vl_horodate );

	/*A Recherche du No d'equipement dans la structure config_pic en memoire */
	for (pl_donneepic = pg_debutPIC; pl_donneepic != NULL; pl_donneepic = pl_donneepic->Suivant )
	{
		if (pl_donneepic->Config.Numero.NoEqt == vl_Numero)
			break;
	}
  	
	if (pl_donneepic == NULL)
	{
		/* Trace du message incorrect */
		 XZST_03EcritureTrace(XZSTC_WARNING, "ep_cmd_PIC_cb: Message XDM_ECMD_PIC incorrect (erreur equipement %d) .", vl_Numero );

		/* Trace : erreur equipement introuvable */
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PIC_cb : erreur equipement" );
				
		/* Envoi du CR NOK a la base */
		XZST_03EcritureTrace(XZSTC_INTERFACE, "ep_cmd_PIC_cb: Appel xzat5201_CompteRendu_PIC avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
		if ( vl_NoCMD != 0 ) 
			xzat5201_CompteRendu_PIC(vl_NoCMD,vl_horodate,XDC_NOK,0);
		return;
	}
 			
	XZST_03EcritureTrace(XZSTC_WARNING, "Callback ep_cmd_PIC_cb = trame : %s" ,pl_mlcr_lg1);
 	
      /* Envoyer trame a la tache TELMi */	
	if ( !vg_Simulation )
	{
		/*A Envoi de la trame */ 
		if (!strcmp(pl_donneepic->Config.AdresseIP, XZECC_IP_VIDE)) 
		{ 
			if (EnvoyerTrame(    pl_mlcr_lg1, 
				XZEXC_TRAME_AFF, 
				pl_donneepic->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pl_donneepic->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
				return; 
			} 
		} 
		else 
		{ 
			if (EnvoyerTrameIP(    pl_mlcr_lg1, 
				XZEXC_TRAME_AFF, 
				pl_donneepic->Config.AdresseIP, 
				pl_donneepic->Config.Port, 
				pl_donneepic->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pl_donneepic->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
				return; 
			}
		}
	}
              		       
            
      /* Ecriture dans fichier Trace_CMD_PIC de la commande */
      ex_ecriture_trace_cmd ( XDC_EQT_PIC,
                              vl_Numero, 
                              pl_mlcr_lg1,
                              NULL,
                              vl_NoCMD );
   
      /*A Demande de relecture pour controle de la bonne execution et envoi contraste */
         if ( em_envoyer_contraste_pic ( pl_donneepic, 
                                      pl_donneepic->Contraste,
                                      (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE) == XDC_NOK )
         {
			XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PICTO" );
			if ( vl_NoCMD != 0 )
				xzat5201_CompteRendu_PIC(vl_NoCMD,vl_horodate,XDC_NOK,pl_donneepic->Config.SiteGestion);
			XZST_10ArchiverMsgSyst(0," ep_cmd_PIC_cb: Impossible d'envoyer la commande", NULL);
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PIC_cb");
			return;
         }
       

       /* Mise a jour memoire etat commande */
       pl_donneepic->NumDerCmd = (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE;
       pl_donneepic->Etat_Commande.NoCMD  = vl_NoCMD;

		if ( vg_Simulation )
		{
			memcpy ( &pl_donneepic->Etat, &pl_donneepic->Etat_Commande, sizeof(XDY_Etat_PIC) );
			ep_envoi_affichage_pic ( pl_donneepic->Etat, vl_Numero, 5 );
			if ( vl_NoCMD != 0 )
				xzat5201_CompteRendu_PIC(vl_NoCMD,vl_horodate,XDC_OK,pl_donneepic->Config.SiteGestion);
		}
       /* Fin du callback */
       XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PIC_cb " );
 	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	em_envoyer_contraste_pic(   	EPIC_DONNEES_PIC *		pa_ListePIC,
                         		int                            va_Contraste,
                         		int				va_NumCmd )
/*
* ARGUMENTS EN ENTREE :
*  pa_ListePIC : pointeur sur liste picto
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
*  Envoi de la trame de contraste a telmi pour le picto
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      :  em_envoyer_contraste_pic " ;

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
			if (!strcmp(pa_ListePIC->Config.AdresseIP, XZECC_IP_VIDE)) 
			{ 
				if (EnvoyerTrame(vl_lcr,
            		        	XZEXC_TRAME_CONTRASTE,
					pa_ListePIC->Config.AdresseRGS,
					XZEXC_NOCMD, 
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePIC->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
					return; 
				} 
			} 
			else 
			{ 
				if (EnvoyerTrameIP(vl_lcr,
            		        	XZEXC_TRAME_CONTRASTE,
					pa_ListePIC->Config.AdresseIP, 
					pa_ListePIC->Config.Port, 
					pa_ListePIC->Config.AdresseRGS, 
					XZEXC_NOCMD, 
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePIC->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
					return; 
				}
			}
		}
           	 /* EnvoyerTrame( vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListePIC->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListePIC->Socket);*/
		               

		/*A Envoi de la trame */ 
		if (!strcmp(pa_ListePIC->Config.AdresseIP, XZECC_IP_VIDE)) 
		{ 
			if (EnvoyerTrame("ST",
				XZEXC_TRAME_ETAT_AFF_PIC, 
				pa_ListePIC->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pa_ListePIC->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
				return; 
			} 
		} 
		else 
		{ 
			if (EnvoyerTrameIP("ST",
				XZEXC_TRAME_ETAT_AFF_PIC, 
				pa_ListePIC->Config.AdresseIP, 
				pa_ListePIC->Config.Port, 
				pa_ListePIC->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pa_ListePIC->Socket) == XDC_NOK ) 
			{ 
				XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
				return; 
			}
		}

           	 /* vl_ValRet = EnvoyerTrame( "ST",
        		        XZEXC_TRAME_ETAT_AFF_PIC,
        		        pa_ListePIC->Config.AdresseRGS,
        		        XZEXC_NOCMD,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePIC->Socket);*/

	       /*A envoi de la trame de relecture */
               if ( !vg_Simulation )
		{
			if (!strcmp(pa_ListePIC->Config.AdresseIP, XZECC_IP_VIDE)) 
			{ 
				if (EnvoyerTrame( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PIC,
					pa_ListePIC->Config.AdresseRGS, 
					va_NumCmd,
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePIC->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
					return; 
				} 
			} 
			else 
			{ 
				if (EnvoyerTrameIP( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PIC,
					pa_ListePIC->Config.AdresseIP, 
					pa_ListePIC->Config.Port, 
					pa_ListePIC->Config.AdresseRGS, 
					va_NumCmd,
					XZEXC_PRIORITEFAIBLE, 
					pa_ListePIC->Socket) == XDC_NOK ) 
				{ 
					XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande PIC" ); 
					return; 
				}
			}
		}
           	 /* vl_ValRet = EnvoyerTrame( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PIC,
        		        pa_ListePIC->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePIC->Socket);*/

     	       if ( vg_Simulation )
     	       {
     	          pa_ListePIC->Contraste = va_Contraste;
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite) ;			
     	          TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XDC_EQT_PIC,
                          	T_IPC_FT_INT2, pa_ListePIC->Config.Numero.NoEqt,
                          	T_IPC_FT_INT2, pa_ListePIC->Contraste,
                          	NULL);
		}

 	       /*A Si envoi a telmi impossible alors init connexion socket avec telmi */       
               if ( vl_ValRet != XDC_OK ) ec_init_cnx ();	
                       
               pa_ListePIC->ContrasteCmd = va_Contraste;
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
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      : ep_contraste_cb " ;

#ifdef _TIBCO_EMS
   XDY_Mot 			va_Contraste;		 
   XDY_Mot 			va_NoEqt;		
   XDY_Mot 			va_TypeEqt;		
#else
   T_INT2 			va_Contraste;		 
   T_INT2 			va_NoEqt;		
   T_INT2 			va_TypeEqt;		
#endif
   EPIC_DONNEES_PIC *		pl_ListePIC;
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
      case XZECC_TYPE_EQT_PIC:
         /*B pour tous les pictos envoi de la trame de contraste */
         for ( pl_ListePIC = pg_debutPIC; pl_ListePIC != NULL; pl_ListePIC = pl_ListePIC->Suivant )
         {
	    if ( (va_NoEqt == XZEPC_SURB_EQTALL) || (pl_ListePIC->Config.Numero.NoEqt == va_NoEqt) )
	    {
	       /*A Composition de la trame contraste et envoi */
               em_envoyer_contraste_pic(pl_ListePIC,va_Contraste,XZEXC_CMD_HORS_BASE);
            }
            if ( pl_ListePIC->Config.Numero.NoEqt == va_NoEqt ) break;
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
*  Fonction appelée lors de la réception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      : ep_controle_cb  " ;

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

   EPIC_DONNEES_PIC *		pl_ListePIC;
   

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
   
	/*A si l'eqt est un pic */
	if (va_TypeEqt ==  XZECC_TYPE_EQT_PIC)
	{
		/*B Recherche du No d'equipement dans la structure config_pic en memoire */
		for ( pl_ListePIC = pg_debutPIC; pl_ListePIC != NULL ;
		pl_ListePIC = pl_ListePIC->Suivant )
		{
			if (pl_ListePIC->Config.Numero.NoEqt == va_NumEqt)
			{
				if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
					pl_ListePIC->Etat_Service &= va_Etat;
				else
					pl_ListePIC->Etat_Service += va_Etat;

				if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
				{
					pl_ListePIC->Etat_Service = va_Etat;
				}
				if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
				{
					pl_ListePIC->Etat_Service = va_Etat;
				}
				break;
			}
		}
	}

   


   for ( pl_ListePIC = pg_debutPIC; pl_ListePIC != NULL ; 
	         pl_ListePIC = pl_ListePIC->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servpic:%d ",pl_ListePIC->Etat_Service);
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
   static char *version = "@(#)epic_cmd.c	1.29 05/31/00      : ep_journuit  " ;
   XZEXT_Trame 			vl_lcr = "";
   char				vl_Contraste [3];
   EPIC_DONNEES_PIC *		pl_ListePIC;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
      vg_Contexte=XDC_JOUR;
   else
      vg_Contexte=XDC_NUIT;
   
   XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d", vg_Contexte );
 		
   /*B Pour tout les PIC en memoire */
   for ( pl_ListePIC = pg_debutPIC; pl_ListePIC != NULL ; 
	    pl_ListePIC = pl_ListePIC->Suivant )
   {
       /* Envoyer nouveau contraste */
       em_envoyer_contraste_pic( pl_ListePIC, pl_ListePIC->Contraste, XZEXC_CMD_HORS_BASE );
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
   static char *version = "@(#)epic_cmd.c       1.29 05/31/00      : ep_CMD_LCR" ;

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


   EPIC_DONNEES_PIC             *pl_ListePIC;
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
        if ( (vl_TypeEqt == XDC_EQT_PIC) &&
             (ex_test_autorisation_eqt ( &pg_DebutListePICFLcr,
                                         &pl_eqt,
                                         XDC_EQT_PIC) == XDC_NOK) )
        {
           return;
        }
              for ( pl_ListePIC = pg_debutPIC; pl_ListePIC != NULL ;
                    pl_ListePIC = pl_ListePIC->Suivant )
              {
                 if ( (pl_ListePIC->Config.Numero.NoEqt == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
                 {
                     if (strcmp(pl_ListePIC->Config.AdresseIP, XZECC_IP_VIDE)) {
                        ex_init_lcr_ip (  &pg_DebutListePICFLcr,
                                          pl_eqt,
                                          pl_ListePIC->Socket,
                                          pl_ListePIC->Config.AdresseRGS,
                                          pl_ListePIC->Config.Numero.NoEqt,
                                          "PIC_FLcr_NoEqt:",
                                          vg_SockTIM,
                                          pl_ListePIC->Config.AdresseIP,
                                          pl_ListePIC->Config.Port);
                     }
                     else
                     {
                        ex_init_lcr (   &pg_DebutListePICFLcr,
                                        pl_eqt,
                                        pl_ListePIC->Socket,
                                        pl_ListePIC->Config.AdresseRGS,
                                        pl_ListePIC->Config.Numero.NoEqt,
                                        "PIC_FLcr_NoEqt:",
                                        vg_SockTIM );
                     }

                 }
                 if (pl_ListePIC->Config.Numero.NoEqt == vl_NoEqt)
                   break;
              }
              if ( (pl_ListePIC == NULL) && (vl_NoEqt!=XDC_EQTALL) )
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

