/*E*/
/*Fichier :  $Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      Release : $Revision: 1.25 $        Date : $Date: 2021/06/21 06:52:15 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des Callbacks de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Oct 1994	: Creation
* Nagiel.E	version 1.2	01 Dec 1994	:
* Nagiel.E	version 1.3	22 Dec 1994	:
* Nagiel.E	version 1.4	05 Jan 1995	:
* Mismer.D	version 1.5	11 Jan 1995	:
* Nagiel.E	version 1.6	16 Jan 1995	:
* Nagiel.E	version 1.7	16 Jan 1995	:
* Mismer.D	version 1.8	17 Mar 1995	:
* Volcic.F	version 1.9	22 Mar 1995	: Modifs apres essais
* Volcic.F	version 1.10	05 Avr 1995	:
* Volcic.F	version 1.11	04 Mai 1995	:
* Fontaine.C	version 1.12	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.13	20 Jui 1995	: Ajout traitement commande en cours pour configuration 
*						  et modification de l'etat des voies
* Volcic.F	version 1.14	25 Oct 1995	: Suppression commentaire 
* Volcic.F	version 1.15	15 Nov 1995	: Ajout trace reception config voie DAI
* Volcic.F	version 1.16	15 Fev 1996	: Modif config voie DAI (DEM/751)
* Mismer.D	version 1.17 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
* Mismer.D	version 1.18 	07 Jan 1997	: Ajout trace config voie sur travaux (DEM/1155)
* Mismer.D      version 1.19    30 Mai 2000     : Modif pour config avec 2 DAI sur un LT (DEM/102)
* Mismer.D      version 1.20    30 Mai 2000     : Modif pour config avec 2 DAI sur un LT (DEM/102)
* JPL		version 1.21	28/06/18 : Migration Linux 64 bits (DEM 1274) : Arguments complets pour appel XZST_10ArchiverMsgSyst  1.21
* JMG		version 1.22 	06/06/19 : passage IP DEM1333 1.22
* JMG				version 1.23 15/04/20 : EMS 1.23
* JMG		version 1.24 	05/05/21 correction CFIV neutralisation 
* jmg		version 1.25	21/06/21 : reactivation trace si IP
* P?I				25/11/21 : coorection de l'envoi du ST pour config des voies 1.27
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "edai_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      : edai_cmd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
void conf_voie_IP(EDAI_DONNEES_DAI *pa_listeDAI,EDAI_DONNEES_ANALYSEUR *pa_listeAnaly);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM DAI.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_Init_IHM_Mes_DAI_cb ( 
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
*  Fonction appele lors de la rception du message
*  XDM_EInit_IHM_Mes_DAI
*
* FONCTION 
*  Pour 1 ou tous les Analyseurs , envoyer l'etat  par XDM_EMES_DAI
*
------------------------------------------------------*/
{
   static char *version = "$Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      : ed_Init_IHM_Mes_DAI_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup pl_DGInit;
   #endif

   T_INT2			vl_NumEqt;
   EDAI_DONNEES_ANALYSEUR *	pl_listeAnaly;   
   double                       vl_Instant_T;
   int                     	vl_jourSemaine;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_Init_IHM_Mes_DAI_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_DAI */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
               T_IPC_FT_INT2, &vl_NumEqt,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING," messsage XDM_EInit_IHM_DAI non lu" );
 		
      XZST_10ArchiverMsgSyst(0,"ed_Init_IHM_Mes_DAI_cb : Message XDM_EInit_IHM_DAI recu avec parametre(s) invalide(s)", NULL);
      
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Init_IHM_Mes_DAI_cb ");
      return;
   }
	        
   /*A Pour 1 ou tous les analyseurs : envoi du msg XDM_EMES_DAI si il est en service et actif */
  
   for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
   {
      /* si l'analyseur est en service et actif */
      if ( !pl_listeAnaly->Etat_Service )
      {
         /* pour ts les eqt ou un eqt precis */
         if ((vl_NumEqt==0) || (vl_NumEqt==pl_listeAnaly->Config.Numero))
         { 
            /* provisoire */
         	 XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Instant_T );
	         pl_listeAnaly->Mes.Horodate=vl_Instant_T;
                 
     	    if (!TipcSrvMsgWrite ( pl_DGInit,
      			           XDM_IdentMsg(XDM_EMES_DAI),
                          	   XDM_FLG_SRVMSGWRITE,
               			   T_IPC_FT_REAL8,       pl_listeAnaly->Mes.Horodate,
               			   T_IPC_FT_INT2,        pl_listeAnaly->Config.Numero,
               			   T_IPC_FT_INT2,        pl_listeAnaly->Config.NoAnaly,
               			   T_IPC_FT_INT2,        pl_listeAnaly->Mes.Sens,
               			   T_IPC_FT_INT2,        pl_listeAnaly->Mes.Chaussee,
               			   T_IPC_FT_INT2_ARRAY,  &pl_listeAnaly->Mes.VitesseVoie,
                                       (sizeof(pl_listeAnaly->Mes.VitesseVoie)/sizeof(XDY_Mot)),
               			   T_IPC_FT_INT2_ARRAY,  &pl_listeAnaly->Mes.LongueurVoie,
                                       (sizeof(pl_listeAnaly->Mes.LongueurVoie)/sizeof(XDY_Mot)),
               			   T_IPC_FT_INT2,        pl_listeAnaly->Mes.TO,
               			   T_IPC_FT_INT2,        pl_listeAnaly->Mes.VitesseMoyenne,
                          	   NULL))
      	    {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EMES_DAI non effectue" );
	    }
	    else
      	    {
         	XZST_03EcritureTrace(XZSTC_INFO,"Envoi message XDM_EMES_DAI effectue sur %s" ,pl_DGInit);
	    }
	 }
      }	
   }  
  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Init_IHM_Mes_DAI_cb ");
	       
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
*  Fonction appele lors de la rception du message
*  XDM_EInit_IHM_Liaison
*
* FONCTION 
*  Pour touts les DAI , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      :  ep_Init_IHM_Liaison_cb " ;

#ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif
   
   EDAI_DONNEES_DAI *		pl_ListeDAI;
   

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
	        
   /*A Pour chaque DAI : envoi du msg XDM_ETAT_LIAISON  */
  
   for ( pl_ListeDAI = pg_debutDAI; pl_ListeDAI != NULL; pl_ListeDAI = pl_ListeDAI->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListeDAI->Config.Numero, XDC_EQT_DAI ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contrle.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_controle_cb ( 
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
*  Fonction appele lors de la rception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      : ed_controle_cb" ;
   
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

   EDAI_DONNEES_DAI *		pl_ListeDAI;
   EDAI_DONNEES_ANALYSEUR *	pl_ListeAnaly;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_EQPT non lu \n" );
 		
      XZST_10ArchiverMsgSyst(0,"ed_controle_cb : Message XDM_ECMD_EQPT recu avec parametre(s) invalide(s)", NULL);
      
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_controle_cb ");
      return;
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING
        ,"callback ep_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
   {
      if (va_TypeEqt ==  XZECC_TYPE_EQT_ANA)
      {
         /*A Recherche du No d'equipement dans la structure Analy en memoire */
         for ( pl_ListeAnaly = pg_debutAnaly; pl_ListeAnaly != NULL ; 
	         pl_ListeAnaly = pl_ListeAnaly->Suivant )
	 {
	    /*A Affectation de l'etat */
	    if (pl_ListeAnaly->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeAnaly->Etat_Service &= va_Etat;
      	       else
      	          pl_ListeAnaly->Etat_Service |= va_Etat;
     	       break;
    	    }
	 }
      }      
       if (va_TypeEqt ==  XZECC_TYPE_EQT_DAI)
      {
         /*A Recherche du No d'equipement dans la structure DAI en memoire */
         for ( pl_ListeDAI = pg_debutDAI; pl_ListeDAI != NULL ; 
	         pl_ListeDAI = pl_ListeDAI->Suivant )
	 {
	    /*A Affectation de l'etat */
	    if (pl_ListeDAI->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeDAI->Etat_Service &= va_Etat;
      	       else
      	          pl_ListeDAI->Etat_Service |= va_Etat;
	       break;
    	    }
	 }
      }      
   }
   
   /* Trace des Differents etats service et activite */
   for ( pl_ListeAnaly = pg_debutAnaly; pl_ListeAnaly != NULL; pl_ListeAnaly = pl_ListeAnaly->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "servAnaly:%d", pl_ListeAnaly->Etat_Service );
   }
   for ( pl_ListeDAI = pg_debutDAI; pl_ListeDAI != NULL; pl_ListeDAI = pl_ListeDAI->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "servdai:%d ", pl_ListeDAI->Etat_Service);
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_controle_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de config de voies analyseur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_Conf_Voie_DAI_cb ( 
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
*  Fonction appele lors de la rception du message
*  XDM_EConf_Voie_DAI
*
* FONCTION 
*  Pour 1 Analyseur , envoyer trames de config a la dai
*
------------------------------------------------------*/
{
   static char *version = "$Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      : ed_Conf_Voie_DAI_cb" ;
   
#ifndef _TIBCO_EMS
   T_INT2			*pl_Etat;
   T_INT2			vl_NoEqtDAI;
   T_INT2			vl_NoEqtAnaly;
   T_INT2			vl_NoAnaly;
   T_INT4			vl_nbvoie;
#else
   XDY_Mot			pl_Etat[5];
   XDY_Mot			vl_NoEqtDAI;
   XDY_Mot			vl_NoEqtAnaly;
   XDY_Mot			vl_NoAnaly;
   XDY_Entier			vl_nbvoie;
#endif
   EDAI_DONNEES_ANALYSEUR *	pl_listeAnaly;   
   EDAI_DONNEES_DAI *		pl_listeDAI;   
   int				vl_Index = 0;
   int				vl_Cpt = 0;
   XDY_Voie			vl_config[5];
   XZEXT_Trame			pl_mlcr = "";
   int				vl_jourSemaine	= 0;
   XDY_Horodate			vl_Secondes = 0.0;
   XDY_Horodate			vl_TimeOut = 0.0;
   int vl_deja=0;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ed_Conf_Voie_DAI_cb");
		        		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_DAI */
   if(!TipcMsgRead(pa_Data -> msg
 			  ,T_IPC_FT_INT2,&vl_NoEqtDAI
 			  ,T_IPC_FT_INT2,&vl_NoEqtAnaly
 			  ,T_IPC_FT_INT2,&vl_NoAnaly
 			  ,T_IPC_FT_INT2_ARRAY,&pl_Etat,&vl_nbvoie
	                  ,NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage %s non lu", version );
      XZST_10ArchiverMsgSyst(0,"ed_Conf_Voie_DAI_cb : Message XDM_EInit_IHM_DAI recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Conf_Voie_DAI_cb ");
      return;
   }

   /* recuperation du tableau de config des voies */ 
#ifndef _TIBCO_EMS
   for ( vl_Index = 0;(vl_Index < vl_nbvoie);vl_config[vl_Index] = *pl_Etat, vl_Index++, pl_Etat++);
#else
   for ( vl_Index = 0;vl_Index < vl_nbvoie;vl_Index++)
   	vl_config[vl_Index] = pl_Etat[vl_Index];
#endif
            	        
   XZST_03EcritureTrace(XZSTC_INFO, "ed_Conf_Voie_DAI_cb DAI=%d ANA=%d NumAna=%d V0=%d V1=%d V2=%d V3=%d V4=%d",
   			      vl_NoEqtDAI, vl_NoEqtAnaly, vl_NoAnaly, vl_config[0], vl_config[1], vl_config[2], vl_config[3], vl_config[4] );

   for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
   {
     if ( pl_listeDAI->Config.Numero == vl_NoEqtDAI) break;
   }
   if (pl_listeDAI == NULL) return;
   
   /*A Pour 1 analyseur */	    
   for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
      if ( /*!! (pl_listeAnaly->EtatConf != EDAIC_CONF_FIN) &&  */
           !(pl_listeAnaly->Etat_Service & XDC_EQT_HORS_SRV) && 
           !(pl_listeAnaly->Etat_Service & XDC_EQT_INACTIF) ) 
         break;
/*!!   if ( pl_listeAnaly == NULL )
   { 
*/      sprintf(pl_mlcr,"ST ID=%s", vg_idpassw);
      /*A Envoi de la trame */
      /*
      if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)) {
	      if ( EnvoyerTrame(pl_mlcr,
	                XZEXC_TRAME_CONF_DAI,
	                pl_listeDAI->Config.AdresseRGS,
	                ++vg_NumConf[vl_NoEqtDAI],
		        XZEXC_PRIORITEFAIBLE,
	                pl_listeDAI->SocketMaitre) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING, "ed_Conf_Voie_DAI_cb : Impossible d'emettre la trame <%s>", pl_mlcr);
	      }
        }
	else {
	   if (EnvoyerTrameIP2(    pl_mlcr,
                        XZEXC_TRAME_CONF_DAI,
                        pl_listeDAI->Config.AdresseIP,
                        pl_listeDAI->Config.Port,
                        pl_listeDAI->Config.Adresse2IP,
                        pl_listeDAI->Config.Port2,
                        pl_listeDAI->Config.DAIBis,
                        pl_listeDAI->Config.AdresseRGS,
                        0,
                        XZEXC_PRIORITEFAIBLE,
                        pl_listeDAI->SocketMaitre) == XDC_NOK )
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande DAI" );
	      return;
	   }
	}
	*/
/*!!   } 
*/ 
    if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)) {
                 if ( EnvoyerTrame(pl_mlcr,
	                          XZEXC_TRAME_CONF_DAI,
                          pl_listeDAI->Config.AdresseRGS,
                          vg_NumConf[vl_NoEqtDAI],
             XZEXC_PRIORITEFAIBLE,
                      pl_listeDAI->SocketMaitre) != XDC_OK )
              {
		           XZST_03EcritureTrace(XZSTC_WARNING, "ed_Conf_Voie_DAI_cb : Impossible d'emettre la trame <%s>", pl_mlcr);
             }
	            XZST_03EcritureTrace(XZSTC_WARNING, "ed_Conf_Voie_DAI_cb : emettre la trame <%s>", pl_mlcr);
	}

       /*
       else {
           sleep(0.3);
           if (EnvoyerTrameIP2(    pl_mlcr,
                        XZEXC_TRAME_CONF_DAI,
                        pl_listeDAI->Config.AdresseIP,
                        pl_listeDAI->Config.Port,
                        pl_listeDAI->Config.Adresse2IP,
                        pl_listeDAI->Config.Port2,
                        pl_listeDAI->Config.DAIBis,
                        pl_listeDAI->Config.AdresseRGS,
                        0,
                        XZEXC_PRIORITEFAIBLE,
                        pl_listeDAI->SocketMaitre) == XDC_NOK )
           {
              XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande DAI" );
              return;
           }
                 XZST_03EcritureTrace(XZSTC_WARNING, "ed_Conf_Voie_DAI_cb : 2 emettre la trame <%s>", pl_mlcr);
        }
	*/	





for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
   {   
      if ( pl_listeAnaly->Config.Numero == vl_NoEqtAnaly )
      {
         /*A envoi de la config a l'equipement si celui-ci est en service */
         if ( !(pl_listeAnaly->Etat_Service & XDC_EQT_HORS_SRV) && 
              !(pl_listeAnaly->Etat_Service & XDC_EQT_INACTIF) )
         {
            /*A mise en memoire de la config des voies ds la structure analyseur */
            vl_Cpt = sprintf(pl_mlcr,"Eqt:%d Analyseur:%d ", vl_NoEqtAnaly, vl_NoAnaly );
            for ( vl_Index = 0; vl_Index < vl_nbvoie; vl_Index++ )
            {
               if ( pl_listeAnaly->EtatVoie[vl_Index] != vl_config[vl_Index] )
               { 
                  pl_listeAnaly->EtatVoie[vl_Index] = vl_config[vl_Index];
                  pl_listeAnaly->EtatConfVoie[vl_Index] = EDAIC_CONF_DEBUT;
                  vl_Cpt += sprintf(pl_mlcr+vl_Cpt,"[Voie%d: %s]", vl_Index,\
                       (vl_config[vl_Index] == XDC_VOIE_INCONNUE) ? "inconnue" :\
                       (vl_config[vl_Index] == XDC_VOIE_SENS_NORMAL) ? "normale" :\
                       (vl_config[vl_Index] == XDC_VOIE_SENS_INVERSE) ? "inverse" :\
                       (vl_config[vl_Index] == XDC_VOIE_BLOQUEE) ? "bloquee" :\
                       (vl_config[vl_Index] == XDC_VOIE_INEXISTANTE) ? "inexistante" :\
                       (vl_config[vl_Index] == XDC_VOIE_NEUTRALISEE) ? "neutralisee" :\
                       (vl_config[vl_Index] == XDC_VOIE_BLOQUEE_NEUTRALISEE) ? "bloquee_neutralisee" :\
                       (vl_config[vl_Index] == XDC_VOIE_BAU_NORMALE) ? "BAU_normale" : "BAU_etroite" );
               }
            }
               
            pl_listeAnaly->EtatConf = EDAIC_CONF_DEBUT;      
         
	         XZST_03EcritureTrace(XZSTC_WARNING, "ed_Conf_Voie_DAI_cb : EDAIC_CONF_DEBUT");
	 /*CONFIG*/
	 if (strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)) {
                ex_ecriture_trace_cmd (  XDC_EQT_DAI,
                                  pl_listeDAI->Config.Numero, 
                                  pl_mlcr,
                                  NULL,
                                  0 );
	       conf_voie_IP(pl_listeDAI, pl_listeAnaly);
	       sleep(3);
	        return;
	  }
}									
	/* PNI pl_listeAnaly->EtatConf = EDAIC_CONF_FIN;*/

         ex_ecriture_trace_cmd (  XDC_EQT_DAI,
                                  pl_listeDAI->Config.Numero, 
                                  pl_mlcr,
                                  NULL,
                                  0 );
         
        	
         break;
      }
   }  
                
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Conf_Voie_DAI_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	ed_CMD_LCR ( T_IPC_CONN			va_Cnx,
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
*  Fonction appele lors de la rception du message
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
static char *version = "$Id: edai_cmd.c,v 1.25 2021/06/21 06:52:15 pc2dpdy Exp $      : ed_CMD_LCR";

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
   XDY_NomMachine              vl_NomMachine;
#endif

   
   EDAI_DONNEES_DAI		*pl_ListeDAI;
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
           XZST_03EcritureTrace(XZSTC_WARNING
 	   ,"messsage XDM_ELCR non lu \n" );
         				
           XZST_10ArchiverMsgSyst(0,"ep_CMD_LCR : Message XDM_ELCR(DAI) recu avec parametre(s) invalide(s)", NULL);
      
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_CMD_LCR");
           return;
	}
	      
        /* Trace du message en entree de la fonction */
        /*XZST_03EcritureTrace(XZSTC_WARNING
 	     ,"IN ed_CMD_LCR, msg= %d %d %s %s %d %d %d %s",
   				vl_TypeEqt,
   				vl_NoEqt,
   				vl_FicCmdLcr,
   				vl_FicResLcr,
   				vl_Periode,
   				vl_HrDebut,
   				vl_HrFin,
   				vl_NomMachine);*/
        XZST_03EcritureTrace(XZSTC_WARNING
 	     ,"IN ed_CMD_LCR");
	
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
	/*
        if ( (vl_TypeEqt == XDC_EQT_DAI) &&
             (ex_test_autorisation_eqt ( &pg_DebutListeDAIFLcr, 
        				 &pl_eqt,
        				 XDC_EQT_DAI) == XDC_NOK) )
        {
           return;
        }*/


	   if (vl_TypeEqt == XDC_EQT_DAI)
	   {
	      for ( pl_ListeDAI = pg_debutDAI; pl_ListeDAI != NULL ; 
	            pl_ListeDAI = pl_ListeDAI->Suivant )
	      {
	         if ( (pl_ListeDAI->Config.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	         {
		    if (strcmp(pl_ListeDAI->Config.AdresseIP, XZECC_IP_VIDE))
        	    ex_init_lcr_ip ( 	&pg_DebutListeDAIFLcr,
        	    			pl_eqt,
        	    			pl_ListeDAI->SocketMaitre,
        	    			pl_ListeDAI->Config.AdresseRGS,
        	    			pl_ListeDAI->Config.Numero,
        	    			"DAI_FLcr_NoEqt:",
        	    			vg_SockTIM,
					pl_ListeDAI->Config.AdresseIP,
					pl_ListeDAI->Config.Port);
		    else
        	    ex_init_lcr ( 	&pg_DebutListeDAIFLcr,
        	    			pl_eqt,
        	    			pl_ListeDAI->SocketMaitre,
        	    			pl_ListeDAI->Config.AdresseRGS,
        	    			pl_ListeDAI->Config.Numero,
        	    			"DAI_FLcr_NoEqt:",
        	    			vg_SockTIM );
        	    			
                 }
                 if (pl_ListeDAI->Config.Numero == vl_NoEqt)
                   break;
	      }
              if ( (pl_ListeDAI == NULL) && (vl_NoEqt!=XDC_EQTALL) )
              {
                 XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
              }
	   }
	
   	/* Fin du callback */
        XZST_03EcritureTrace(XZSTC_FONCTION
 		,"OUT Callback ed_CMD_LCR" );
 	
}

void conf_voie_IP(EDAI_DONNEES_DAI *pa_listeDAI,EDAI_DONNEES_ANALYSEUR *pa_listeAnaly)
{
EDAI_DONNEES_DAI             *pl_listeDAI;
EDAI_DONNEES_ANALYSEUR       *pl_listeAnaly;
int                          vl_Index = 0;
XZEXT_Trame                  pl_mlcr = "";
static char *version = "edai_tra.c : conf_voie_IP";

pl_listeDAI=pa_listeDAI;
pl_listeAnaly=pa_listeAnaly;

/*B Composition des trames LCR de config voie DAI pour ttes les voies */
for (vl_Index = 0; vl_Index < pl_listeAnaly->Config.NbVoiesGerees; vl_Index ++)
{
  XZST_03EcritureTrace(XZSTC_FONCTION,
        "Composition des trames LCR de config voie DAI pour ttes les voies %d %d ",
  vl_Index,pl_listeAnaly->EtatConfVoie[vl_Index]);
  if ( (pl_listeAnaly->EtatConfVoie[vl_Index] != EDAIC_CONF_FIN) &&
       (pl_listeAnaly->EtatVoie[vl_Index] != XDC_VOIE_INEXISTANTE) )
  {
    /* si la voie etait neutralisee : deneutralisation */
    if ( (pl_listeAnaly->EtatConfVoie[vl_Index] == EDAIC_CONF_DEBUT) &&
         (pl_listeAnaly->EtatVoie[vl_Index] != XDC_VOIE_NEUTRALISEE) )
    {
      sprintf(pl_mlcr,"CFSV ID=%s A%02d V%d=%s",
                                       vg_idpassw,
                                       pl_listeAnaly->Config.NoAnaly,
                                       vl_Index,
                                       (pl_listeAnaly->EtatVoie[vl_Index]==XDC_VOIE_SENS_INVERSE) ? "I" : "N" );

      XZST_03EcritureTrace(XZSTC_WARNING,"%s : trame = <%s>", version, pl_mlcr);

      /*A Envoyer trame a la tache TELMi */
      if (EnvoyerTrameIP2(pl_mlcr,
                                                          XZEXC_TRAME_CONF_DAI_IP,
                                                          pl_listeDAI->Config.AdresseIP,
                                                          pl_listeDAI->Config.Port,
                                                       pl_listeDAI->Config.Adresse2IP,
                                                       pl_listeDAI->Config.Port2,
                                                       pl_listeDAI->Config.DAIBis,
                                                          pl_listeDAI->Config.AdresseRGS,
                                                          XZEXC_NOCMD,
                                                          XZEXC_PRIORITEFAIBLE,
                                                          pl_listeDAI->SocketMaitre) != XDC_OK )
      {
         XZST_03EcritureTrace(XZSTC_WARNING,
           "%s : Impossible d'envoyer la trame IP = <%s> vers TELMI" , version, pl_mlcr);
         return ;
      }
    }
        sleep(0.5);
      sprintf( pl_mlcr,"CFIV ID=%s A%02d V%d=%s",
                                      vg_idpassw,
                                      pl_listeAnaly->Config.NoAnaly,
                                      vl_Index,
                                      (pl_listeAnaly->EtatVoie[vl_Index] == XDC_VOIE_NEUTRALISEE ) ? "F" : "O" );

      /*B Envoyer trame a la tache TELMi */
      XZST_03EcritureTrace(XZSTC_INFO,"%s : Envoi trame = <%s>", version, pl_mlcr );
      if ( EnvoyerTrameIP2(pl_mlcr,
                                                       XZEXC_TRAME_CONF_DAI_IP,
                                                       pl_listeDAI->Config.AdresseIP,
                                                       pl_listeDAI->Config.Port,
                                                       pl_listeDAI->Config.Adresse2IP,
                                                       pl_listeDAI->Config.Port2,
                                                       pl_listeDAI->Config.DAIBis,
                                                       pl_listeDAI->Config.AdresseRGS,
                                                       XZEXC_NOCMD,
                                                       XZEXC_PRIORITEFAIBLE,
                                                       pl_listeDAI->SocketMaitre) != XDC_OK )
      {
          XZST_03EcritureTrace(XZSTC_WARNING,"%s : Impossible d'envoyer la trame IP = <%s> vers TELMI" , version, pl_mlcr);
          return ;
      }
        sleep(0.5);
    
  }
  sleep (0.5);
}
  pl_listeAnaly->EtatConfVoie[vl_Index] = EDAIC_CONF_FIN;
}

void	ed_Enregistrement_DAI_cb (	T_IPC_CONN					va_Cnx,
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
*  Fonction appele lors de la reception du message
*  XDM_ERECORD_DAI
*
* FONCTION 
*  Pour 1 ou tous les Analyseurs , envoyer la demande de demarrage d'arret d'enregistrement
*
------------------------------------------------------*/
{

#ifndef _TIBCO_EMS   
	T_INT2			vl_NumEqt;
	T_STR			vl_cmd;
	T_INT2			vl_Duree;
#else
	XDY_Eqt			vl_NumEqt;
	XDY_Nom			vl_cmd;
	XDY_LongEntier	vl_Duree;
#endif

	XZEXT_Trame			pl_mlcr = "";
	EDAI_DONNEES_DAI	*pl_ListeDAI;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_Enregistrement_DAI_cb ");

	/* Recuperation des parametres du message XDM_RECORD_DAI */
	if(!TipcMsgRead(pa_Data -> msg,
					T_IPC_FT_INT2,	vl_NumEqt,
					T_IPC_FT_STR,	vl_cmd,
					T_IPC_FT_INT2,	vl_Duree,
					NULL))
	{       
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING," messsage XDM_RECORD_DAI non lu" );
		XZST_10ArchiverMsgSyst(0,"ed_Enregistrement_DAI_cb : Message XDM_RECORD_DAI recu avec parametre(s) invalide(s)", NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Enregistrement_DAI_cb ");
		return;
	}

	for ( pl_ListeDAI = pg_debutDAI; pl_ListeDAI != NULL; pl_ListeDAI = pl_ListeDAI->Suivant )
	{

		sprintf(pl_mlcr, "CFEV ID=%s A%d %s %d",
					vg_idpassw,
					vl_NumEqt,
					vl_cmd,
					vl_Duree);

		XZST_03EcritureTrace(XZSTC_WARNING,"ed_Enregistrement_DAI_cb : trame = <%s>", pl_mlcr);

		/*A Envoyer trame a la tache TELMi */
		if (EnvoyerTrameIP2(	pl_mlcr,
								XZEXC_TRAME_CONF_DAI_IP,
								pl_ListeDAI->Config.AdresseIP,
								pl_ListeDAI->Config.Port,
								pl_ListeDAI->Config.Adresse2IP,
								pl_ListeDAI->Config.Port2,
								pl_ListeDAI->Config.DAIBis,
								pl_ListeDAI->Config.AdresseRGS,
								XZEXC_NOCMD,
								XZEXC_PRIORITEFAIBLE,
								pl_ListeDAI->SocketMaitre) != XDC_OK )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "ed_Enregistrement_DAI_cb : Impossible d'envoyer la trame IP = <%s> vers TELMI", pl_mlcr);
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Enregistrement_DAI_cb ");       
			return ;
		}
	
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_Enregistrement_DAI_cb ");       
}
