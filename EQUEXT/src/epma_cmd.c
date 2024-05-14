/* Fichier : epma_cmd.c
------------------------------------------------------
* GTIE * PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMA * FICHIER epma_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de traitement des commandes recues par la tache TEPMA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	25/01/13	: ajout TDP
* PNI	25/03/2013	: suppression des blancs pour l'alterna DEM1061 1.5
* NDE	03/08/2015	: nouvelle logique gestion affichage par defaut DEM1135 1.6
* PNI	26/08/2015	: NAK de certains PMVA sur chaine vide DEM1142 1.7
* JPL	09/11/15	: Sur commande Neutre 3 POINTS, memoriser une commande TEXTE (retour reconnu par les IHM) 1.8
* PNI	18/08/17 	: correctif sur clignotement  (ecrase par cmde contraste) DEM1233 1.9
* JPL	14/02/18	: Migration Linux 64 bits (DEM 1274) : Copie et troncature des lignes d'affichage PMVA  1.10
* PNI	26/06/18	: correctif suppression des blans a la fin v1.11 DEM 1298
* JMG	03/10/18	: IP suppression SAGA DEM1306 1.12
* GR    09/07/2020      : EMS 1.13
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* JPL	10/02/23	: Commande du mode de contraste "Auto" sur demande Normal et a l'Initialisation;
*			  Suppression des commandes de contraste non manuelles (SAE-429)  1.15
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

#include "epma_cfg.h"
#include "epma_tra.h"
#include "epma_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "epma_cmd.c 1.5";

/* declaration de fonctions internes */
void supprime_blancs_fin(char *);

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM PMVA.
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
   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif

   EPMVA_DONNEES_PMVA *		pl_listepma;
   

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
  
   for ( pl_listepma = pg_debutPMVA; pl_listepma != NULL; pl_listepma = pl_listepma->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XDC_EQT_PMVA,
                          	T_IPC_FT_INT2, pl_listepma->Config.Numero.NoEqt,
                          	T_IPC_FT_INT2, pl_listepma->Contraste,
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
   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPMVA_DONNEES_PMVA *		pl_listepma;
   

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
  
   for ( pl_listepma = pg_debutPMVA; pl_listepma != NULL; pl_listepma = pl_listepma->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_listepma->Config.Numero.NoEqt, XDC_EQT_PMVA ); 
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

void	ep_Init_IHM_PMVA_cb ( 
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
*  XDM_EInit_IHM_PMV 
*
* FONCTION 
*  Pour touts les PMV, envoyer l'etat PMV par XDM_ETAT_PMV dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   EPMVA_DONNEES_PMVA *		pl_DonCrt;
   int				vl_NbLigne = 0;
   char 			vl_Contraste [3];
  

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_PMVA_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_PMV */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EInit_IHM_PMV non lu." );
      XZST_10ArchiverMsgSyst(0,"ep_Init_IHM_PMVA_cb : Message XDM_EInit_IHM_PMVA recu avec parametre(s) invalide(s)",NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PMVA_cb");
      return;
   }
	        
	       
   /*A Pour chaque pmv : envoi du msg XDM_ETAT_PMVA */
  
   for ( pl_DonCrt = pg_debutPMVA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_DonCrt->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_PMVA),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_DonCrt->Config.Numero.NoEqt,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Texte1,
                          	T_IPC_FT_INT2, pl_DonCrt->Etat.Affichage1,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Alternat1,
                          	T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign1,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Texte2,
                          	T_IPC_FT_INT2, pl_DonCrt->Etat.Affichage2,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Alternat2,
                          	T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign2,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Texte3,
                          	T_IPC_FT_INT2, pl_DonCrt->Etat.Affichage3,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Alternat3,
                          	T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign3,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Texte4,
                          	T_IPC_FT_INT2, pl_DonCrt->Etat.Affichage4,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Alternat4,
                          	T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign4,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Texte5,
                          	T_IPC_FT_INT2, pl_DonCrt->Etat.Affichage5,
                          	T_IPC_FT_STR,  pl_DonCrt->Etat.Alternat5,
                          	T_IPC_FT_CHAR, pl_DonCrt->Etat.Clign5,
                          	T_IPC_FT_CHAR, pl_DonCrt->Etat.Flash,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PMVA non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_PMVA effectue. %s/%s/%s/%s/%s",
				pl_DonCrt->Etat.Texte1,
				pl_DonCrt->Etat.Texte2,
				pl_DonCrt->Etat.Texte3,
				pl_DonCrt->Etat.Texte4,
				pl_DonCrt->Etat.Texte5);
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_PMVA_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande affichage PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	ep_cmd_PMVA_cb ( T_IPC_CONN			va_Cnx,
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
#ifndef _TIBCO_EMS   
   T_INT2			vl_Numero;
   T_INT2			vl_Affichage1;
   T_INT2			vl_Clign1; 
   T_INT2			vl_Affichage2;
   T_INT2			vl_Clign2; 
   T_INT2			vl_Affichage3;
   T_INT2			vl_Clign3;
   T_INT2			vl_Affichage4;
   T_INT2			vl_Clign4;
   T_INT2			vl_Flash;
   T_CHAR			vl_Flash2;
   T_INT2			vl_Affichage5;
   T_INT2			vl_Clign5;
   T_INT4			vl_NoCMD;
   T_STR			vl_RecTexte1;
   T_STR			vl_RecTexte2;
   T_STR			vl_RecTexte3;
   T_STR			vl_RecTexte4;
   T_STR			vl_RecTexte5;
#else
   XDY_Mot			vl_Numero;
   XDY_Mot			vl_Affichage1;
   XDY_Mot			vl_Clign1; 
   XDY_Mot			vl_Affichage2;
   XDY_Mot			vl_Clign2; 
   XDY_Mot			vl_Affichage3;
   XDY_Mot			vl_Clign3;
   XDY_Mot			vl_Affichage4;
   XDY_Mot			vl_Clign4;
   XDY_Mot			vl_Flash;
   XDY_Octet		vl_Flash2;
   XDY_Mot			vl_Affichage5;
   XDY_Mot			vl_Clign5;
   XDY_Entier		vl_NoCMD;
   XDY_Ligne_PMV	       	vl_RecTexte1;
   XDY_Ligne_PMV       		vl_RecTexte2;
   XDY_Ligne_PMV		vl_RecTexte3;
   XDY_Ligne_PMV		vl_RecTexte4;
   XDY_Ligne_PMV		vl_RecTexte5;
#endif

#ifndef _TIBCO_EMS
   T_STR			vl_Alternat1;
   T_STR			vl_Alternat2;
   T_STR			vl_Alternat3;
   T_STR			vl_Alternat4;
   T_STR			vl_Alternat5;
#else
   XDY_Ligne_PMV	       	vl_Alternat1;
   XDY_Ligne_PMV       		vl_Alternat2;
   XDY_Ligne_PMV		vl_Alternat3;
   XDY_Ligne_PMV		vl_Alternat4;
   XDY_Ligne_PMV		vl_Alternat5;
#endif   


   XDY_Ligne_PMVA		vl_Texte1 = XDC_CHAINE_VIDE;
   XDY_Ligne_PMVA		vl_Texte2 = XDC_CHAINE_VIDE;
   XDY_Ligne_PMVA		vl_Texte3 = XDC_CHAINE_VIDE;
   XDY_Ligne_PMVA		vl_Texte4 = XDC_CHAINE_VIDE;
   XDY_Ligne_PMVA		vl_Texte5 = XDC_CHAINE_VIDE;
   EPMVA_DONNEES_PMVA		*pl_donneepma;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   XZEXT_Trame 			pl_mlcr = "",pl_mlcr2 = "",vl_compstr = "";
   int				vl_TypeAff = 0;
   XZEXT_Trame 			pl_mlcr_lg1 = "";
   XZEXT_Trame 			pl_mlcr_lg2 = "";
   XZEXT_Trame 			pl_mlcr_lg3 = "";
   XZEXT_Trame 			pl_mlcr_lg4 = "";
   XZEXT_Trame 			pl_mlcr_lg5 = "";
   char		vl_pwd[50];
        
        /*A Lecture du message */

   	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_STR,  &vl_RecTexte1
                          ,T_IPC_FT_INT2, &vl_Affichage1
                          ,T_IPC_FT_STR,  &vl_Alternat1
                          ,T_IPC_FT_INT2, &vl_Clign1
                          ,T_IPC_FT_STR,  &vl_RecTexte2
                          ,T_IPC_FT_INT2, &vl_Affichage2
                          ,T_IPC_FT_STR,  &vl_Alternat2
                          ,T_IPC_FT_INT2, &vl_Clign2
                          ,T_IPC_FT_STR,  &vl_RecTexte3
                          ,T_IPC_FT_INT2, &vl_Affichage3
                          ,T_IPC_FT_STR,  &vl_Alternat3
                          ,T_IPC_FT_INT2, &vl_Clign3
                          ,T_IPC_FT_STR,  &vl_RecTexte4
                          ,T_IPC_FT_INT2, &vl_Affichage4
                          ,T_IPC_FT_STR,  &vl_Alternat4
                          ,T_IPC_FT_INT2, &vl_Clign4
                          ,T_IPC_FT_STR,  &vl_RecTexte5
                          ,T_IPC_FT_INT2, &vl_Affichage5
                          ,T_IPC_FT_STR,  &vl_Alternat5
                          ,T_IPC_FT_INT2, &vl_Clign5
                          ,T_IPC_FT_INT2, &vl_Flash
                          ,T_IPC_FT_INT4, &vl_NoCMD
                          ,NULL))
	          
      	{ 
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_PMVA non lu \n" );
           XZST_10ArchiverMsgSyst(0,"ep_cmd_PMVA_cb : Message XDM_CMD_PMVA recu avec parametre(s) invalide(s)",NULL);
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_cmd_PMVA_cb");
           return;
	}
	
	strncpy ( vl_Texte1,  vl_RecTexte1, XDC_Lon_Ligne_PMVA-1 );   
	vl_Texte1[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	strncpy ( vl_Texte2,  vl_RecTexte2, XDC_Lon_Ligne_PMVA-1 );   
	vl_Texte2[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	strncpy ( vl_Texte3,  vl_RecTexte3, XDC_Lon_Ligne_PMVA-1 );   
	vl_Texte3[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	strncpy ( vl_Texte4,  vl_RecTexte4, XDC_Lon_Ligne_PMVA-1 ); 
	vl_Texte4[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	strncpy ( vl_Texte5,  vl_RecTexte5, XDC_Lon_Ligne_PMVA-1 ); 
	vl_Texte5[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;

        /* Trace du message en entree de la fonction */
        XZST_03EcritureTrace(XZSTC_WARNING
 	     ,"IN Callback ep_cmd_PMVA_cb, msg= %d T1\"%s\" %d A1\"%s\" %d T2\"%s\" %d A2\"%s\" %d T3\"%s\" %d A3\"%s\" %d T4\"%s\" %d A4\"%s\" %d %d %d",
                           		vl_Numero,
                           		vl_Texte1,
                           		vl_Affichage1,
                           		vl_Alternat1,
                           		vl_Clign1,
                           		vl_Texte2,
                           		vl_Affichage2,
                           		vl_Alternat2,
                           		vl_Clign2,
                           		vl_Texte3,
                           		vl_Affichage3,
                           		vl_Alternat3,
                           		vl_Clign3,
                           		vl_Texte4,
                           		vl_Affichage4,
                           		vl_Alternat4,
                           		vl_Clign4,
                           		vl_Flash,
                           		vl_NoCMD );
 		                            
	/*A Lecture vl_horodate en cas d'erreur */
	
	XZSM_07LireHorodate( XZSMC_TYPE_RTWORKS , &vl_js , &vl_horodate );
	
	/*A Recherche du No d'equipement dans la structure config_pmv en memoire */
	for ( pl_donneepma = pg_debutPMVA; pl_donneepma != NULL ; 
	         pl_donneepma = pl_donneepma->Suivant )
	{
	   if (pl_donneepma->Config.Numero.NoEqt == vl_Numero)
	   {
	      break;
	   }
	}
  	
  	/* No d'equipement introuvable */
	if (pl_donneepma == NULL)
	{
	   /* Trace du message incorrect */
           XZST_03EcritureTrace(XZSTC_WARNING, "Message XDM_ECMD_PMVA incorrect (erreur equipement %d) .",
                           		vl_Numero );

	   /* Trace : erreur equipement introuvable */
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMVA_cb : erreur equipement" );
 		
	   /* Envoi du CR NOK a la base */
           XZST_03EcritureTrace(XZSTC_INTERFACE
 		,"Appel xzat250_CompteRendu_PMVA avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
	   if ( vl_NoCMD != 0 ) xzat250_CompteRendu_PMVA(vl_NoCMD,vl_horodate,XDC_NOK,0);
	   XZEZ06_CompteRendu_TDP_PMVA(vl_NoCMD,XDC_NOK,vl_Numero,vg_NomMachine);

	   return;
	}
 			
  	/* Si equipement HS ou Inactif -> pas d'envoi au picto */
	if ( ((pl_donneepma->Etat_Service & XDC_EQT_HORS_SRV) != 0) ||
	     (((pl_donneepma->Etat_Service & XDC_EQT_INACTIF) != 0) && (vl_NoCMD != 0)) ||
	     (((pl_donneepma->Etat_Service & XDC_EQT_INHIBE) != 0) && (vl_NoCMD != 0)))
	{
	   /* Trace du message incorrect */
           XZST_03EcritureTrace(XZSTC_WARNING, "Message XDM_ECMD_PMVA incorrect (equipement hs ou inactif Eqt:%d) .",
                           		vl_Numero );

	   /* Trace : erreur equipement introuvable */
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMVA_cb : equipement hs ou inactif" );
	   /* Envoi du CR NOK a la base */
           XZST_03EcritureTrace(XZSTC_INTERFACE, "Appel xzat250_CompteRendu_PMVA avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
	   if ( vl_NoCMD != 0 ) xzat250_CompteRendu_PMVA(vl_NoCMD,vl_horodate,XDC_NOK,pl_donneepma->Config.SiteGestion);
	   XZEZ06_CompteRendu_TDP_PMVA(vl_NoCMD,XDC_NOK,vl_Numero,vg_NomMachine);
	   return;
	}
 
 	if (!strcmp(pl_donneepma->Config.AdresseIP, XZECC_IP_VIDE))
		sprintf(vl_pwd,"ID=%s ",vg_idpassw);
	else	
		sprintf(vl_pwd,"%s ",pl_donneepma->Config.IdLCR);
		
	/*A Si clignotement invalide -> envoi du CR NOK a la base */
	if (((vl_Clign1 != XDC_PMVA_PASCLIGNO) && 
	     (vl_Clign1 != XDC_PMVA_CLIGNOLENT) && 
	     (vl_Clign1 != XDC_PMVA_CLIGNORAPIDE)) ||
            ((vl_Clign2 != XDC_PMVA_PASCLIGNO) && 
	     (vl_Clign2 != XDC_PMVA_CLIGNOLENT) && 
	     (vl_Clign2 != XDC_PMVA_CLIGNORAPIDE)) ||
            ((vl_Clign3 != XDC_PMVA_PASCLIGNO) && 
	     (vl_Clign3 != XDC_PMVA_CLIGNOLENT) && 
	     (vl_Clign3 != XDC_PMVA_CLIGNORAPIDE)) ||
            ((vl_Clign4 != XDC_PMVA_PASCLIGNO) && 
	     (vl_Clign4 != XDC_PMVA_CLIGNOLENT) && 
	     (vl_Clign4 != XDC_PMVA_CLIGNORAPIDE)) ||
            ((vl_Clign5 != XDC_PMVA_PASCLIGNO) && 
	     (vl_Clign5 != XDC_PMVA_CLIGNOLENT) && 
	     (vl_Clign5 != XDC_PMVA_CLIGNORAPIDE)))
	{
	   /* Trace du message incorrect */
           XZST_03EcritureTrace(XZSTC_WARNING
 		,"Message XDM_ECMD_PMVA incorrect (erreur clignotement Eqt:%d Cl1:%d Cl2:%d Cl3:%d Cl4:%d Cl5:%d ) .",
                           		vl_Numero,
                           		vl_Clign1,
                           		vl_Clign2,
                           		vl_Clign3,
                           		vl_Clign4 ,
                           		vl_Clign5);

	
	   /* Trace : erreur clignotement */
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMVA_cb : erreur clignotement" );
	   /* Envoi du CR NOK a la base */
           XZST_03EcritureTrace(XZSTC_INTERFACE
 		,"Appel xzat250_CompteRendu_PMVA avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
	   if ( vl_NoCMD != 0 ) xzat250_CompteRendu_PMVA(vl_NoCMD,vl_horodate,XDC_NOK,pl_donneepma->Config.SiteGestion);
	   XZEZ06_CompteRendu_TDP_PMVA(vl_NoCMD,XDC_NOK,vl_Numero,vg_NomMachine);
	   return;
	}
	
	/*A Si Type d'affichage invalide -> envoi du CR NOK a la base */
	if (((vl_Affichage1 != vl_Affichage2) || 
	     (vl_Affichage1 != vl_Affichage3) || 
	     (vl_Affichage2 != vl_Affichage3) || 
	     (vl_Affichage3 != vl_Affichage4)) ||
	    ((vl_Affichage1 != XDC_PMVA_TEXTE) && 
	     (vl_Affichage1 != XDC_PMVA_ETEINDRE) && 
	     (vl_Affichage1 != XDC_PMVA_DEFAUT) && 
	     (vl_Affichage1 != XDC_PMVA_TEMPERATURE) && 
	     (vl_Affichage1 != XDC_PMVA_HEURE) &&
	     (vl_Affichage1 != XDC_PMVA_3POINTS)) ||
	    ((vl_Flash != XDC_PMVA_FLASH) && 
	     (vl_Flash != XDC_PMVA_PASFLASH)) )
	{
	   /* Trace du message incorrect */
           XZST_03EcritureTrace(XZSTC_WARNING
 		,"Message XDM_ECMD_PMVA incorrect (erreur affichage Eqt:%d Af1:%d Af2:%d Af3:%d Af4:%d flash:%d) .",
                           		vl_Numero,
                           		vl_Affichage1,
                           		vl_Affichage2,
                           		vl_Affichage3,
                           		vl_Affichage4,
                           		vl_Flash );

	   /* Trace : erreur affichage */
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMVA_cb : erreur affichage" );
	   /* Envoi du CR NOK a la base */
           XZST_03EcritureTrace(XZSTC_INTERFACE
 		,"Appel xzat25_CompteRendu_PMV avec %d %d %d",vl_NoCMD,vl_horodate,XDC_NOK );
	   if ( vl_NoCMD != 0 ) xzat250_CompteRendu_PMVA(vl_NoCMD,vl_horodate,XDC_NOK, pl_donneepma->Config.SiteGestion);
	   XZEZ06_CompteRendu_TDP_PMVA(vl_NoCMD,XDC_NOK,vl_Numero,vg_NomMachine);
	   return; 
	}
		

	/* On tronque les textes-alternatif-pmv si ceux-ci sont trop grand */
	vl_Alternat1[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	vl_Alternat2[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	vl_Alternat3[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	vl_Alternat4[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;
	vl_Alternat5[XDC_Lon_Ligne_PMVA-1]=XDC_FIN_CHN;

	/* Si demande d'affichage de l'heure 'standard' -> envoi de la trame correspondante */
	if ( vl_Affichage1 == XDC_PMVA_DEFAUT )
        {
	    vl_TypeAff = atoi ( pl_donneepma->AffDefaut[0] );
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PMVA_cb = Aff:%d  Def:<%s>" ,vl_TypeAff, pl_donneepma->AffDefaut[0] );
	    if ( vl_TypeAff == XDC_PMVA_3POINTS )
	    {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5 = XDC_PMVA_3POINTS;
	    }
	    else if ( vl_TypeAff == XDC_PMVA_HEURE )
	    {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5 = XDC_PMVA_HEURE;
	       sprintf(pl_mlcr,"P %sAM=1 AF=0 AM=2 AF=0 AM=3 AF=HR1 AM=4 AF=0 AM=5 AF=0 AM=6 AF=0 DV=0", vl_pwd);
	    }
	    else if ( vl_TypeAff == XDC_PMVA_ETEINDRE )
	    {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5 =XDC_PMVA_ETEINDRE;
	       sprintf(pl_mlcr,"P %sAM=* AF=0", vl_pwd);
	    }
	    else
	    {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5 = XDC_PMVA_TEXTE;
	       vl_Flash = XDC_PMVA_PASFLASH;
	       strcpy ( vl_Texte1, pl_donneepma->AffDefaut[0] );
	       strcpy ( vl_Texte2, pl_donneepma->AffDefaut[1] );
	       strcpy ( vl_Texte3, pl_donneepma->AffDefaut[2] );
	       strcpy ( vl_Texte4, pl_donneepma->AffDefaut[3] );
	       strcpy ( vl_Texte5, pl_donneepma->AffDefaut[4] );
	       strcpy ( vl_Alternat1, "" );
	       strcpy ( vl_Alternat2, "" );
	       strcpy ( vl_Alternat3, "" );
	       strcpy ( vl_Alternat4, "" );
	       strcpy ( vl_Alternat5, "" );
	    }
	}
	
	if ( (vl_Affichage1 == XDC_PMVA_ETEINDRE) || 
	     (vl_Affichage1 == XDC_PMVA_TEMPERATURE) || 
	     (vl_Affichage1 == XDC_PMVA_HEURE) )
        {
	    vl_Flash = XDC_PMVA_PASFLASH;
	    vl_Clign1 = vl_Clign2 = vl_Clign3 = vl_Clign4 = vl_Clign5 = XDC_PMVA_PASCLIGNO;
	    
	    if ( vl_Affichage1 == XDC_PMVA_HEURE )
	    {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5 = XDC_PMVA_HEURE;

	          sprintf(pl_mlcr,"P %sAM=1 AF=0 AM=2 AF=0 AM=3 AF=HR1 AM=4 AF=0 AM=5 AF=0 AM=6 AF=0 DV=0", vl_pwd);
	    }
 	
	    if ( vl_Affichage1 == XDC_PMVA_ETEINDRE )
	    {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5= XDC_PMVA_ETEINDRE;
	       sprintf(pl_mlcr,"P %sAM=* AF=0", vl_pwd);
	    }
	 
 	
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ep_cmd_PMVA_cb = trame HEURE : %s" ,pl_mlcr);
	    
	    /*A Envoyer trame a la tache TELMi */
            if ( !vg_Simulation ) {
		 if (!strcmp(pl_donneepma->Config.AdresseIP, XZECC_IP_VIDE))
           	 	EnvoyerTrame( pl_mlcr,
              		       XZEXC_TRAME_AFF,
              		       pl_donneepma->Config.AdresseRGS,
              		       XZEXC_NOCMD,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
            	else
			EnvoyerTrameIP( pl_mlcr,
				XZEXC_TRAME_AFF,
				pl_donneepma->Config.AdresseIP,
				pl_donneepma->Config.Port,
				pl_donneepma->Config.AdresseRGS,
				XZEXC_NOCMD,
				XZEXC_PRIORITEFAIBLE,
				pl_donneepma->Socket);
	   }
         }
	 else
         { 
	     if (vl_Affichage1 == XDC_PMVA_3POINTS)
	     {
	       vl_Affichage1 = vl_Affichage2 = vl_Affichage3 = vl_Affichage4 = vl_Affichage5 = XDC_PMVA_TEXTE;

	       vl_Flash = XDC_PMVA_PASFLASH;
	       vl_Clign1 = vl_Clign2 = vl_Clign3 = vl_Clign4 = vl_Clign5 = XDC_PMVA_PASCLIGNO;

	       strcpy ( vl_Texte1, XDC_CHAINE_VIDE );
	       strcpy ( vl_Texte2, XDC_CHAINE_VIDE );
	       strcpy ( vl_Texte3, XDC_CHAINE_VIDE );
	       strcpy ( vl_Texte4, XDC_CHAINE_VIDE );
	       strcpy ( vl_Texte5, "            ..." );

	       strcpy ( vl_Alternat1, "" );
	       strcpy ( vl_Alternat2, "" );
	       strcpy ( vl_Alternat3, "" );
	       strcpy ( vl_Alternat4, "" );
	       strcpy ( vl_Alternat5, "" );
	     }

	     /*A Composition de la trame LCR d'affichage de la premiere ligne pmv*/
	
	     /* Composition de la trame sans txt alternat*/
             memset(vl_compstr,' ',XDC_Lon_Ligne_PMVA);
             vl_compstr[XDC_Lon_Ligne_PMVA] = XDC_FIN_CHN;
	     if ( !strncmp(vl_compstr,vl_Alternat1,XDC_Lon_Ligne_PMVA-1) || !strlen(vl_Alternat1) )
	     {
                strcpy(vl_Alternat1,"");
		 if (pl_donneepma->Config.Type==1)
	        sprintf(pl_mlcr_lg1," AM=1.0 AF=\042%s\042 %s EC=%s DV=0",
	   	       vl_Texte1,
	   	      (vl_Clign1==XDC_PMVA_PASCLIGNO)?"":(vl_Clign1==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A" );
		else
	        sprintf(pl_mlcr_lg1," AM=%d AF=\042%s\042 %s EC=%s DV=0",
	               1,
	   	       vl_Texte1,
	   	      (vl_Clign1==XDC_PMVA_PASCLIGNO)?"":(vl_Clign1==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A" );
	     }
	     else
	     {
	         /* Composition de la trame avec txt alternat*/
		 if (pl_donneepma->Config.Type==1)
	         sprintf(pl_mlcr_lg1," AM=1.0 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   				vl_Texte1,
	   				vl_Alternat1,
	   				vg_clignolent,
			"A");
		 else
	         sprintf(pl_mlcr_lg1," AM=%d AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	         			1,
	   				vl_Texte1,
	   				vl_Alternat1,
	   				vg_clignolent,
			"A");
	     }				
	
             /*A Composition de la trame LCR d'affichage de la deuxieme ligne pmv*/
	
	     /* Composition de la trame sans txt alternat*/
	     if ( !strncmp(vl_compstr,vl_Alternat2,XDC_Lon_Ligne_PMVA-1) || !strlen(vl_Alternat2) )
	     {
                 strcpy(vl_Alternat2,"");
		 if (pl_donneepma->Config.Type==1)
	         sprintf(pl_mlcr_lg2," AM=1.1 AF=\042%s\042 %s EC=%s DV=0",
	   	         vl_Texte2,
	   	         (vl_Clign2==XDC_PMVA_PASCLIGNO)?"":(vl_Clign2==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
		 else
	         sprintf(pl_mlcr_lg2," AM=%d AF=\042%s\042 %s EC=%s DV=0",
	                 2,
	   	         vl_Texte2,
	   	         (vl_Clign2==XDC_PMVA_PASCLIGNO)?"":(vl_Clign2==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
	   	
	      }
	      else
	      {
	         /* Composition de la trame avec txt alternat*/
		 if (pl_donneepma->Config.Type==1)
	         	sprintf(pl_mlcr_lg2," AM=1.1 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   				vl_Texte2,
	   				vl_Alternat2,
	   				vg_clignolent,
			"A");
	      	else
	         	sprintf(pl_mlcr_lg2," AM=%d AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	                 		2,
	   				vl_Texte2,
	   				vl_Alternat2,
	   				vg_clignolent,
			"A");
	      }				
	
              /*A Composition de la trame LCR d'affichage de la troisieme ligne pmv*/

	      /* Composition de la trame sans txt alternat*/
	      if ( !strncmp(vl_compstr,vl_Alternat3,XDC_Lon_Ligne_PMVA-1) || !strlen(vl_Alternat3) )
	      {
                 strcpy(vl_Alternat3,"");
		 if (pl_donneepma->Config.Type==1)
	         sprintf(pl_mlcr_lg3," AM=1.2 AF=\042%s\042 %s EC=%s DV=0",
	   	         vl_Texte3,
	   	         (vl_Clign3==XDC_PMVA_PASCLIGNO)?"":(vl_Clign3==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
		 else
	         sprintf(pl_mlcr_lg3," AM=%d AF=\042%s\042 %s EC=%s DV=0",
	                 3,
	   	         vl_Texte3,
	   	         (vl_Clign3==XDC_PMVA_PASCLIGNO)?"":(vl_Clign3==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
	   	
	      }
	      else
	      {
	         /* Composition de la trame avec txt alternat*/
		 if (pl_donneepma->Config.Type==1)
	         	sprintf(pl_mlcr_lg3," AM=1.2 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   				vl_Texte3,
	   				vl_Alternat3,
	   				vg_clignolent,
	   				"A");
	         else
	         	sprintf(pl_mlcr_lg3," AM=%d AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	                 		3,
	   				vl_Texte3,
	   				vl_Alternat3,
	   				vg_clignolent,
	   				"A");
	      }				
	

	         if ( !strncmp(vl_compstr,vl_Alternat4,XDC_Lon_Ligne_PMVA-1) || !strlen(vl_Alternat4) )
	         {
                    strcpy(vl_Alternat4,"");
		 if (pl_donneepma->Config.Type==1)
	            sprintf(pl_mlcr_lg4," AM=1.3 AF=\042%s\042 %s EC=%s DV=0",
	   	         vl_Texte4,
	   	         (vl_Clign4==XDC_PMVA_PASCLIGNO)?"":(vl_Clign4==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
		    else
	            sprintf(pl_mlcr_lg4," AM=4 AF=\042%s\042 %s EC=%s DV=0",
	   	         vl_Texte4,
	   	         (vl_Clign4==XDC_PMVA_PASCLIGNO)?"":(vl_Clign4==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
	   	
	         }
	         else
	         {
	            /* Composition de la trame avec txt alternat*/
		 if (pl_donneepma->Config.Type==1)
	            sprintf(pl_mlcr_lg4," AM=1.3 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   	   			vl_Texte4,
	   				vl_Alternat4,
	   				vg_clignolent,
	   				"A");
		 else
	            sprintf(pl_mlcr_lg4," AM=4 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   	   			vl_Texte4,
	   				vl_Alternat4,
	   				vg_clignolent,
	   				"A");

	         }		
	
		 if ( !strncmp(vl_compstr,vl_Alternat5,XDC_Lon_Ligne_PMVA-1) || !strlen(vl_Alternat5) )
	         {
                    strcpy(vl_Alternat5,"");
		 if (pl_donneepma->Config.Type==1)
	            sprintf(pl_mlcr_lg5," AM=1.4 AF=\042%s\042 %s EC=%s DV=0",
	   	         vl_Texte5,
	   	         (vl_Clign5==XDC_PMVA_PASCLIGNO)?"":(vl_Clign5==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
		    else
	            sprintf(pl_mlcr_lg5," AM=5 AF=\042%s\042 %s EC=%s DV=0",
	   	         vl_Texte5,
	   	         (vl_Clign5==XDC_PMVA_PASCLIGNO)?"":(vl_Clign5==XDC_PMVA_CLIGNOLENT)?vg_clignolent:vg_clignorapide,
			"A");
	   	
	         }
	         else
	         {
	            /* Composition de la trame avec txt alternat*/
		 if (pl_donneepma->Config.Type==1)
	            sprintf(pl_mlcr_lg5," AM=1.4 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   	   			vl_Texte5,
	   				vl_Alternat5,
	   				vg_clignolent,
	   				"A");
		 else
	            sprintf(pl_mlcr_lg5," AM=5 AF=\042%s\042/\042%s\042 %s EC=%s DV=0",
	   	   			vl_Texte5,
	   				vl_Alternat5,
	   				vg_clignolent,
	   				"A");
	         }		

		if (vl_Flash==XDC_PMVA_PASFLASH)
			vl_Flash2 = 0;
		else
			vl_Flash2 = 1;

              sprintf ( pl_mlcr2, "P %s%s%s%s ", vl_pwd, pl_mlcr_lg1, pl_mlcr_lg2, pl_mlcr_lg3);
	      XZST_03EcritureTrace(XZSTC_WARNING, "Callback ep_cmd_PMVA_cb = trame : %s" ,pl_mlcr2);
 	
	      /*A Envoyer trame a la tache TELMi */
	
              if ( !vg_Simulation ) {
		 if (!strcmp(pl_donneepma->Config.AdresseIP, XZECC_IP_VIDE))
           	 	EnvoyerTrame( pl_mlcr2,
              		       XZEXC_TRAME_AFF,
              		       pl_donneepma->Config.AdresseRGS,
              		       XZEXC_NOCMD,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
              	else
           	 	EnvoyerTrameIP( pl_mlcr2,
              		       XZEXC_TRAME_AFF,
              		       pl_donneepma->Config.AdresseIP,
              		       pl_donneepma->Config.Port,
              		       pl_donneepma->Config.AdresseRGS,
              		       XZEXC_NOCMD,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
		}

	      /* Ecriture dans fichier Trace_CMD_PMV de la commande */    
      	      if ( !vg_Simulation ) 
      		ex_ecriture_trace_cmd ( XDC_EQT_PMVA,
                              vl_Numero, 
                              pl_mlcr2,
                              NULL,
                              vl_NoCMD );

		 if (pl_donneepma->Config.Type==1)
              sprintf ( pl_mlcr, "P %s%s%s AM=3 AF=%d EC=%s ", vl_pwd, pl_mlcr_lg4,pl_mlcr_lg5 , vl_Flash2,
			"A");
	      else
              sprintf ( pl_mlcr, "P %s%s%s AM=6 AF=%d EC=%s ", vl_pwd, pl_mlcr_lg4,pl_mlcr_lg5 , vl_Flash2,
			"A");
	      XZST_03EcritureTrace(XZSTC_WARNING, "Callback ep_cmd_PMVA_cb = trame : %s" ,pl_mlcr);
 	
	      /*A Envoyer trame a la tache TELMi */
	
              if ( !vg_Simulation ) {
		 if (!strcmp(pl_donneepma->Config.AdresseIP, XZECC_IP_VIDE))
           	 	EnvoyerTrame( pl_mlcr,
              		       XZEXC_TRAME_AFF,
              		       pl_donneepma->Config.AdresseRGS,
              		       XZEXC_NOCMD,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
           	else   		       
           	 	EnvoyerTrameIP( pl_mlcr,
              		       XZEXC_TRAME_AFF,
              		       pl_donneepma->Config.AdresseIP,
              		       pl_donneepma->Config.Port,
              		       pl_donneepma->Config.AdresseRGS,
              		       XZEXC_NOCMD,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
	   }
           }

      /* Ecriture dans fichier Trace_CMD_PMV de la commande */    
      if ( !vg_Simulation )
      	ex_ecriture_trace_cmd ( XDC_EQT_PMVA,
                              vl_Numero, 
                              pl_mlcr,
                              NULL,
                              vl_NoCMD );
   
      /*A Demande de relecture pour contr�le de la bonne execution et envoi contraste */
      if ( !vg_Simulation ) {
		 if (!strcmp(pl_donneepma->Config.AdresseIP, XZECC_IP_VIDE))
           	 	EnvoyerTrame( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PMVA,
              		       pl_donneepma->Config.AdresseRGS,
              		       (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
		else
           	 	EnvoyerTrameIP( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PMVA,
              		       pl_donneepma->Config.AdresseIP,
              		       pl_donneepma->Config.Port,
              		       pl_donneepma->Config.AdresseRGS,
              		       (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE,
              		       XZEXC_PRIORITEFAIBLE,
              		       pl_donneepma->Socket);
	}
       

       /* Mise a jour memoire etat commande */
       pl_donneepma->Etat_Commande.Numero = vl_Numero;
       pl_donneepma->NumDerCmd            = (vl_NoCMD != XZEXC_NOCMD) ? vl_NoCMD : XZEXC_CMD_HORS_BASE;
	supprime_blancs_fin(vl_Texte1);
      strcpy(pl_donneepma->Etat_Commande.Texte1,vl_Texte1);
       pl_donneepma->Etat_Commande.Affichage1=vl_Affichage1;
	supprime_blancs_fin(vl_Alternat1);
       strcpy(pl_donneepma->Etat_Commande.Alternat1,vl_Alternat1);
       pl_donneepma->Etat_Commande.Clign1=vl_Clign1;

	supprime_blancs_fin(vl_Texte2);
       strcpy(pl_donneepma->Etat_Commande.Texte2,vl_Texte2);
       pl_donneepma->Etat_Commande.Affichage2=vl_Affichage2;
	supprime_blancs_fin(vl_Alternat2);
       strcpy(pl_donneepma->Etat_Commande.Alternat2,vl_Alternat2);
       pl_donneepma->Etat_Commande.Clign2=vl_Clign2;

	supprime_blancs_fin(vl_Texte3);
       strcpy(pl_donneepma->Etat_Commande.Texte3,vl_Texte3);
       pl_donneepma->Etat_Commande.Affichage3=vl_Affichage3;
	supprime_blancs_fin(vl_Alternat3);
       strcpy(pl_donneepma->Etat_Commande.Alternat3,vl_Alternat3);
       pl_donneepma->Etat_Commande.Clign3=vl_Clign3;
       pl_donneepma->Etat_Commande.Flash=vl_Flash;


	supprime_blancs_fin(vl_Texte4);
          strcpy(pl_donneepma->Etat_Commande.Texte4,vl_Texte4);
          pl_donneepma->Etat_Commande.Affichage4=vl_Affichage4;
	supprime_blancs_fin(vl_Alternat4);
          strcpy(pl_donneepma->Etat_Commande.Alternat4,vl_Alternat4);
          pl_donneepma->Etat_Commande.Clign4=vl_Clign4;

	supprime_blancs_fin(vl_Texte5);
  	         strcpy(pl_donneepma->Etat_Commande.Texte5,vl_Texte5);
          pl_donneepma->Etat_Commande.Affichage5=vl_Affichage5;
	supprime_blancs_fin(vl_Alternat5);
          strcpy(pl_donneepma->Etat_Commande.Alternat5,vl_Alternat5);
          pl_donneepma->Etat_Commande.Clign5=vl_Clign5;


       if ( vg_Simulation )
       {
          memcpy ( &pl_donneepma->Etat, &pl_donneepma->Etat_Commande, sizeof(XDY_Etat_PMVA) );
          ep_envoi_affichage_PMVA ( pl_donneepma->Etat, vl_Numero, 5 );
	  if ( vl_NoCMD != 0 ) xzat250_CompteRendu_PMVA(vl_NoCMD,vl_horodate,XDC_OK,pl_donneepma->Config.SiteGestion);
	  XZEZ06_CompteRendu_TDP_PMVA(vl_NoCMD,XDC_OK,vl_Numero,vg_NomMachine);
       }
       /* Fin du callback */
       XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_cmd_PMVA_cb " );
 	
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le pmv
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	em_envoyer_contraste_pmva(   	EPMVA_DONNEES_PMVA *		pa_ListePMVA,
                         		int                            va_Contraste,
                         		int				va_NumCmd )
/*
* ARGUMENTS EN ENTREE :
*  pa_ListePMVA : pointeur sur liste pmv
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
   XZEXT_Trame 			vl_lcr = "";
   char 			vl_Contraste [3];
   XDY_Datagroup		pl_DG;
   int				vl_ValRet = XDC_OK;
   char	vl_pwd[20];

 	if (!strcmp(pa_ListePMVA->Config.AdresseIP, XZECC_IP_VIDE))
		sprintf(vl_pwd,"ID=%s ",vg_idpassw);
	else	
		sprintf(vl_pwd,"%s ",pa_ListePMVA->Config.IdLCR);
               /*A Choix de la commande en fonction du contexte jour nuit */
               switch ( va_Contraste )
               {
                  case XDC_CONTRASTEBRILLANT :
                      strcpy ( vl_Contraste, "S" );
                      break;
                  case XDC_CONTRASTENORMAL :
                      /* SAE-429 l'etat normal est le mode Automatique, qq. soit le contexte Jour/Nuit */
                      strcpy ( vl_Contraste, "A" );
                      break;
               }
                   

               /*A envoi de la trame de contraste */
	       sprintf( vl_lcr,"P %sAM=* EC=%s", vl_pwd, vl_Contraste );
               if ( !vg_Simulation ) {
		 if (!strcmp(pa_ListePMVA->Config.AdresseIP, XZECC_IP_VIDE)) {
           	  EnvoyerTrame( vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListePMVA->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListePMVA->Socket);
               
	       /*A envoi de la trame de relecture */
           	  vl_ValRet = EnvoyerTrame( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PMVA,
        		        pa_ListePMVA->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePMVA->Socket);
		}
		else {
           	  EnvoyerTrameIP( vl_lcr,
            		        XZEXC_TRAME_CONTRASTE,
                                pa_ListePMVA->Config.AdresseIP,
                                pa_ListePMVA->Config.Port,
                                pa_ListePMVA->Config.AdresseRGS,
              		        XZEXC_NOCMD,
              		        XZEXC_PRIORITEFAIBLE,
                                pa_ListePMVA->Socket);
               
	       /*A envoi de la trame de relecture */
           /*	  vl_ValRet = EnvoyerTrameIP( "PS",
        		        XZEXC_TRAME_ETAT_AFF_PMVA,
        		        pa_ListePMVA->Config.AdresseIP,
        		        pa_ListePMVA->Config.Port,
        		        pa_ListePMVA->Config.AdresseRGS,
        		        va_NumCmd,
		 	        XZEXC_PRIORITEFAIBLE,
        		        pa_ListePMVA->Socket);*/
		}

		}

     	       if ( vg_Simulation )
     	       {
     	          pa_ListePMVA->Contraste = va_Contraste;
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite) ;			
     	          TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_Contraste),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, XDC_EQT_PMVA,
                          	T_IPC_FT_INT2, pa_ListePMVA->Config.Numero.NoEqt,
                          	T_IPC_FT_INT2, pa_ListePMVA->Contraste,
                          	NULL);
		}

 	       /*A Si envoi a telmi impossible alors init connexion socket avec telmi */       
               if ( vl_ValRet != XDC_OK ) ec_init_cnx ();	
                       
               pa_ListePMVA->ContrasteCmd = va_Contraste;
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
#ifdef _TIBCO_EMS
   XDY_Mot 			va_Contraste;		 
   XDY_Mot 			va_NoEqt;		
   XDY_Mot 			va_TypeEqt;		
#else
   T_INT2 			va_Contraste;		 
   T_INT2 			va_NoEqt;		
   T_INT2 			va_TypeEqt;		
#endif
   EPMVA_DONNEES_PMVA *		pl_ListePMVA;
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
      case XZECC_TYPE_EQT_PMVA:
         /*B pour tous les pmv envoi de la trame de contraste */
         for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL; pl_ListePMVA = pl_ListePMVA->Suivant )
         {
	    if ( (va_NoEqt == XZEPC_SURB_EQTALL) || (pl_ListePMVA->Config.Numero.NoEqt == va_NoEqt) )
	    {
	       /*A Composition de la trame contraste et envoi */
               em_envoyer_contraste_pmva(pl_ListePMVA,va_Contraste,XZEXC_CMD_HORS_BASE);
            }
            if ( pl_ListePMVA->Config.Numero.NoEqt == va_NoEqt ) break;
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

   EPMVA_DONNEES_PMVA *		pl_ListePMVA;
   

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
   
   {  
         
      /*A si l'eqt est un pmv */
      if (va_TypeEqt ==  XZECC_TYPE_EQT_PMVA)
      {
         /*B Recherche du No d'equipement dans la structure config_pmv en memoire */
         for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
	 {
	    if (pl_ListePMVA->Config.Numero.NoEqt == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListePMVA->Etat_Service &= va_Etat;
      	       else
      	          pl_ListePMVA->Etat_Service += va_Etat;

/*               if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
               {
                 pl_ListePMVA->Etat_Service = va_Etat;
               }
               if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
               {
                  pl_ListePMVA->Etat_Activite = va_Etat;
               }*/
	       break;
    	    }
	 }
      }      
   }
   

   for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1
 		,"servpmva:%d ",pl_ListePMVA->Etat_Service);
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
   XZEXT_Trame 			vl_lcr = "";
   char				vl_Contraste [3];
   EPMVA_DONNEES_PMVA *		pl_ListePMVA;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
      vg_Contexte=XDC_JOUR;
   else
      vg_Contexte=XDC_NUIT;
   
   XZST_03EcritureTrace(XZSTC_INTERFACE, "Nouveau contexte : %d", vg_Contexte );
 		
   
   
// SAE-429  Plus de pilotage de luminosite
//   /*B Pour tout les PMV en memoire */
//   for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
//	    pl_ListePMVA = pl_ListePMVA->Suivant )
//   {
//       /* Envoyer nouveau contraste */
//       em_envoyer_contraste_pmva( pl_ListePMVA, pl_ListePMVA->Contraste, XZEXC_CMD_HORS_BASE );
//   }
   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Mise a jour du contraste de tous les PMVA pilotes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_maj_contraste_tous (int va_Contraste )
 
/*
* ARGUMENTS EN ENTREE :
*   consigne de contraste.
*  
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Envoie la consigne de contraste a tous les equipements pilotes.
*
------------------------------------------------------*/
{
	EPMVA_DONNEES_PMVA	*pl_ListePMVA;

	XZST_03EcritureTrace(XZSTC_INFO, "MAJ contraste : %d", va_Contraste );

	/* Pour tous les PMVA en memoire */
	for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
	         pl_ListePMVA = pl_ListePMVA->Suivant )
	{
		/* Envoyer nouveau contraste */
		em_envoyer_contraste_pmva( pl_ListePMVA, pl_ListePMVA->Contraste, XZEXC_CMD_HORS_BASE );
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
#ifndef _TIBCO_EMS   
   T_INT2                       vl_TypeEqt;
   T_INT2                       vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2                       vl_Periode;
   T_REAL8                      vl_HrDebut;
   T_REAL8                      vl_HrFin;
#else
   XDY_Mot                       vl_TypeEqt;
   XDY_Mot                       vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot                       vl_Periode;
   XDY_Horodate                      vl_HrDebut;
   XDY_Horodate                      vl_HrFin;
#endif
   
    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    #else
    XDY_NomMachine		vl_NomMachine;
    #endif   

   EPMVA_DONNEES_PMVA             *pl_ListePMVA;
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
        XZST_03EcritureTrace(XZSTC_WARNING,"IN ep_CMD_LCR %d %d",vl_TypeEqt,vl_NoEqt);


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
        if ( (vl_TypeEqt == XDC_EQT_PMVA) &&
             (ex_test_autorisation_eqt ( &pg_DebutListePMVAFLcr,
                                         &pl_eqt,
                                         XDC_EQT_PMVA) == XDC_NOK) )
        {
           return;
        }
              for ( pl_ListePMVA = pg_debutPMVA; pl_ListePMVA != NULL ;
                    pl_ListePMVA = pl_ListePMVA->Suivant )
              {
                 if ( (pl_ListePMVA->Config.Numero.NoEqt == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
                 {
		    if (strcmp(pl_ListePMVA->Config.AdresseIP,XZECC_IP_VIDE)) {
                    ex_init_lcr_ip (       &pg_DebutListePMVAFLcr,
                                        pl_eqt,
                                        pl_ListePMVA->Socket,
                                        pl_ListePMVA->Config.AdresseRGS,
                                        pl_ListePMVA->Config.Numero.NoEqt,
                                        "PMVA_FLcr_NoEqt:",
                                        vg_SockTIM,
					pl_ListePMVA->Config.AdresseIP,
					pl_ListePMVA->Config.Port);
			}
			else
                    ex_init_lcr (       &pg_DebutListePMVAFLcr,
                                        pl_eqt,
                                        pl_ListePMVA->Socket,
                                        pl_ListePMVA->Config.AdresseRGS,
                                        pl_ListePMVA->Config.Numero.NoEqt,
                                        "PMVA_FLcr_NoEqt:",
                                        vg_SockTIM);

                 }
		 XZST_03EcritureTrace(XZSTC_FONCTION,"pl_ListePMVA->Config.Numero.NoEqt %d",
		 				pl_ListePMVA->Config.Numero.NoEqt);
                 if (pl_ListePMVA->Config.Numero.NoEqt == vl_NoEqt)
                   break;
              }
              if ( (pl_ListePMVA == NULL) && (vl_NoEqt!=XDC_EQTALL) )
              {
                 XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
              }
        /* Fin du callback */
        XZST_03EcritureTrace(XZSTC_WARNING, "OUT Callback ep_CMD_LCR" );

}

void supprime_blancs_fin(char *Ligne)

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


        /*A on supprime les espaces de fin de ligne */
        k = strlen(Ligne);
	for (i=0,j=0; i<k;i++)
	{
		if ( Ligne[i] != ' ')
/*			return;*/
			break;
	}
	/*if ( j != k-1)*/	/* Il n'y a pas que des blancs */
	{
	i=k;
        for (j=i-1;j>=0;j--) {
                if ( Ligne[j] == ' ' )
                        Ligne[j] = XDC_FIN_CHN;
                else break;
        }
	}
}

