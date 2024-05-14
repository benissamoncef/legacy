/*E*/
/* Fichier : $Id: etdp_cmd.c,v 1.13 2021/06/14 17:22:57 devgfi Exp $       Release : $Revision: 1.13 $        Date :  $Date: 2021/06/14 17:22:57 $
-------------------------------------------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_cmd.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le fichier des callbacks de la tache TETDP
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.3	06 Mai 1997	: Affichage TDP sur PMV  suite (DEM/1417)
* Mismer.D      version 1.4     13 Oct 1997     : Modif pour affichage minutes pour TDP (DEM/1505)
* Mismer.D      version 1.5     16 Oct 1997     : Modif pour affichage minutes pour TDP suite (DEM/1505)
* Niepceron.P	version 1.2 ?	29 avr 2005	: Modif et_demande_TDP_cb et et_TDP_voisin_cb pour prise en cpte TDP non valide DEM464
* Niepceron.P	version 1.3	08 juin 2005	: Modif et_TDP_voisin_cb : recup vl_TDP6mn si zoe virtuelle DEM464
* Niepceron.P	version 1.4	20 juin 2006	: Modif pour reprise rafraichissement apres perte de données tdp
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.5
* LCL		22/06/11 : TDP 18 1.6
* PNI		19/09/11 : TDP suppression de la sortie de liste des PMV en défaut de pilotage v1.7
* VR		21/11/11 : Ajout Destination Noeud 1.8 (DEM/1014)
* MG		29/05/12 : Ajout TDP PMVA (DEM/1014 PMA)
* JMG		15/11/14 : linux 
* GR            09/07/2020 : EMS 1.12
* JPL		07/06/21 : Traces des messages de commandes d'affichage Temps de parcours  1.13
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include	<xdc.h>
#include	<xdm.h>
#include	<xzst.h>
#include	<xzao.h>
#include	<xzez.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"

#include	"etdp_don.h"
#include	"etdp_cfg.h"
#include	"etdp_dir.h"
#include	"etdp_pmv.h"
#include	"etdp_pma.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
/* -------------------------------- */

INFO_EQT_FEN_LCR	*pg_DebutListeTDPFLcr  = NULL;

/* declaration de variables locales */
/* -------------------------------- */

static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      : etdp_cmd" ;

/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi de la mesure temps de parcours pour une zone donnÅe.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_Init_IHM_Mes_TDP_cb ( 
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_EINIT_ABT
*
* FONCTION 
*  Suivant la commande -> envoi du msg XDM_EMES_TDP_6MN_PAR_ZONE
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      : :et_Init_IHM_Mes_TDP_cb ";
   ;
   #ifndef _TIBCO_EMS
   T_STR		va_NomDGDesti;
   T_INT4		va_TypeEqt;
   T_INT4		va_NumEqt;
   #else
   XDY_Datagroup        va_NomDGDesti;
   XDY_Entier		va_TypeEqt;
   XDY_Entier		va_NumEqt;
   #endif
   

   int			vl_IndexUGTP = 0,
   			vl_IndexZone = 0;
   XDY_Datagroup	pl_NomDG;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: et_Init_IHM_Mes_TDP_cb ");
		  
   /*A Recuperation des parametres du message XDM_EINIT_ABT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_STR, &va_NomDGDesti
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: et_Init_IHM_Mes_TDP_cb Messsage XDM_EINIT_ABT non lu." );
      return;
   }

   
   /*A Suivant le type d'eqt -> envoi du msg correspondant */
   switch(va_TypeEqt)
   {
      /*B Traitement du systeme rau */
      case XDC_EQT_ZDP :
      {
         /*  Pour 1 ou tous les zones envoi du message XDM_EMES_TDP_6MN_PAR_ZONE */
         for ( vl_IndexUGTP = 0; vl_IndexUGTP <= ETDPC_NB_MAX_UGTP; vl_IndexUGTP++ )
         {
            if ( !(vg_DonneesUGTP[vl_IndexUGTP].Etat_Service) && (vg_DonneesUGTP[vl_IndexUGTP].ChampValide == Valide) )
            {
               for ( vl_IndexZone = 0; vl_IndexZone <= ETDPC_NB_MAX_ZONE; vl_IndexZone++ )
               {
                  if ( (vg_DonneesUGTP[vl_IndexUGTP].Zone[vl_IndexZone].ChampValide == Valide) &&
                       !(vg_DonneesUGTP[vl_IndexUGTP].Zone[vl_IndexZone].Etat_Service) &&
                       ((vg_Mesure[vl_IndexUGTP].Numero[vl_IndexZone] == va_NumEqt) || (va_NumEqt==XDC_EQTALL)) )
                  {
     	             if (!TipcSrvMsgWrite ( 	va_NomDGDesti,
      			              		XDM_IdentMsg(XDM_EMES_TDP_6MN_PAR_ZONE),
                          	      		XDM_FLG_SRVMSGWRITE,
               	     				T_IPC_FT_REAL8,	vg_DonneesUGTP[vl_IndexUGTP].Horodate6mn,
               					T_IPC_FT_INT2, 	vg_Mesure[vl_IndexUGTP].Numero[vl_IndexZone],
               					T_IPC_FT_INT2, 	vg_Mesure[vl_IndexUGTP].TDP6mn[vl_IndexZone],
               					T_IPC_FT_INT2, 	vg_Mesure[vl_IndexUGTP].Validite6mn[vl_IndexZone],	 
                          	      		NULL))
      	       	    {
         	       XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EMES_TDP_6MN_PAR_ZONE non effectue." );
	            }
	            else
      	            {
         	       XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_EMES_TDP_6MN_PAR_ZONE effectue sur %s" ,va_NomDGDesti);
	            }
                 }
               }
            }
         }
         break;
      }       
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_Init_IHM_Mes_TDP_cb ");
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Interrogation depuis la fenetre LCR.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_CMD_LCR( T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_data, T_CB_ARG pa_arg)
		     
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      : et_CMD_LCR";

#ifndef _TIBCO_EMS   
   T_INT2 vl_TypeEqt;
   T_INT2 vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2 vl_Periode;
   T_REAL8 vl_HrDebut;
   T_REAL8 vl_HrFin;
#else
   XDY_Mot vl_TypeEqt;
   XDY_Mot vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot vl_Periode;
   XDY_Horodate vl_HrDebut;
   XDY_Horodate vl_HrFin;
#endif   

    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    #else
    XDY_NomMachine		vl_NomMachine;
    #endif   

   INFO_EQT_FEN_LCR pl_eqt;
   int i;
   int vl_indice_table_eqt=-1;
   
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN et_CMD_LCR");
	       
   /*A Lecture du message */
   if( !TipcMsgRead(pa_data->msg,
   	T_IPC_FT_INT2,	&vl_TypeEqt,	T_IPC_FT_INT2,	&vl_NoEqt,
   	T_IPC_FT_STR,	&vl_FicCmdLcr,	T_IPC_FT_STR,	&vl_FicResLcr,
   	T_IPC_FT_INT2,	&vl_Periode,	T_IPC_FT_REAL8,	&vl_HrDebut,
   	T_IPC_FT_REAL8,	&vl_HrFin,	T_IPC_FT_STR,	&vl_NomMachine,	NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ELCR non lu." );
      XZST_10ArchiverMsgSyst(0,
           "et_CMD_LCR : Message XDM_ELCR(TDP) recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_CMD_LCR");

      return;
   }
   /* Si l'equipement n'est pas un equipement generique (Meteo, GTC sismique ... */
   
   if( vl_TypeEqt != XDC_EQT_TDP )
   {
      /* Quitter le callback */
      
      return;
   }
   /* Finsi */
   
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
   
   /* Si la commande LCR ne s'adresse pas a l'ensemble des equipements temps de parcours */
   
   if( vl_NoEqt!=XDC_EQTALL)
   {
      /* Alors */
      
      pl_eqt.NoEqt=vl_NoEqt;
      
      /* Boucle de recherche sur les equipements geres par la tache */
      
      for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
      {
         /* Si c'est l'equipement recherche */
      
         if( (vg_DonneesUGTP[i].Numero == vl_NoEqt) &&
	     (vg_DonneesUGTP[i].ChampValide == Valide) )
         {
	    /* Si l'UGTP est reelle */
	if (vg_DonneesUGTP[i].NumLS > 0)
	    {
            /* Alors */
            /* Recuperer l'indice de la table des equipements */
            vl_indice_table_eqt=i;
            }
	    else
            {
                /* Alors */
                /* UGTP virtuelle, on trace en INFO */
                XZST_03EcritureTrace(XZSTC_INFO," Demande LCR sur UGTP virtuelle ");
            }
            break;
         }
         /* Finsi */
      }
   
      /* Si l'indice n'a pas ete trouve */
   
      if( vl_indice_table_eqt==-1)
      {
         /* Alors */
         /* Trace : Impossible de reconnaitre le type d'equipement commande */
         XZST_03EcritureTrace(XZSTC_WARNING, 
      			" Impossible de reconnaitre le type d'equipement commande " );
         return;
      }
      /* Finsi */
      
      /* Si l'equipement n'a pas de liaison Maitre */
   
      if( vg_DonneesUGTP[vl_indice_table_eqt].SocketLS == Non_valide)
      {
         /* Alors */
         /* Trace : Impossible de transmettre la trame */
      
         XZST_03EcritureTrace(XZSTC_WARNING, " Impossible de tramsmettre la commande LCR : pas de liaison " );
         XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
         return;
      }
      /* Finsi */
      
      /* Recherche si pas de commande LCR en cours sur equipement concerne */
        
      if( ex_test_autorisation_eqt(&pg_DebutListeTDPFLcr, &pl_eqt, XDC_EQT_TDP) == XDC_NOK)
      {
         XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
      }
      else
      {
         ex_init_lcr( 	&pg_DebutListeTDPFLcr,
         		pl_eqt,
                	vg_DonneesUGTP[vl_indice_table_eqt].SocketLS,
                      	vg_DonneesUGTP[vl_indice_table_eqt].AdresseRGS,
        	        vg_DonneesUGTP[vl_indice_table_eqt].Numero,
			ETDPC_TIMER_FEN_LCR,
			vg_SockTIM) ;
      }
   }
   else
   {
      /* Sinon */
      /* Boucle sur l'ensemble des equipements de la tache */
      
      for(i=0; i!=ETDPC_NB_MAX_UGTP; i++)
      {
	/* Si l'UGTP est reelle */
         if ( vg_DonneesUGTP[i].NumLS > 0 )
         {
         /* Si l'equipement est valide */
         
         if( vg_DonneesUGTP[i].ChampValide == Valide )
         {
            /* Alors */

            pl_eqt.NoEqt=vg_DonneesUGTP[i].Numero;
            
            /* Recherche si pas de commande LCR en cours sur equipement concerne */
            if( ex_test_autorisation_eqt(&pg_DebutListeTDPFLcr, &pl_eqt, XDC_EQT_TDP) == XDC_NOK )
            {
               XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
            }
            else
            {
               ex_init_lcr( 	&pg_DebutListeTDPFLcr,
               			pl_eqt,
                 		vg_DonneesUGTP[i].SocketLS,
                      		vg_DonneesUGTP[i].AdresseRGS,
        	              	vg_DonneesUGTP[i].Numero,
				ETDPC_TIMER_FEN_LCR,
				vg_SockTIM );
            }
         }
         /* Finsi */
	}
      }
   }
   /* Finsi */
	   
   /* Fin du callback */
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback et_CMD_LCR" );

   return;
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Message de contrÂle.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_controle_cb( T_IPC_CONN va_Cnx,T_IPC_CONN_PROCESS_CB_DATA pa_Data,T_CB_ARG pa_arg)
 
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      : et_controle_cb";
   T_INT4 va_Etat;

    #ifndef _TIBCO_EMS
    T_STR			va_NomMachine;
   T_INT4 va_TypeEqt;
   T_INT4 va_NumEqt;
    #else
    XDY_NomMachine		va_NomMachine;
   XDY_Entier va_TypeEqt;
   XDY_Entier va_NumEqt;
    #endif   

   int i, j;   

		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   
   if( !TipcMsgRead( pa_Data -> msg,
	T_IPC_FT_INT4,	&va_Etat,	T_IPC_FT_STR,	&va_NomMachine,
	T_IPC_FT_INT4,	&va_TypeEqt,	T_IPC_FT_INT4,	&va_NumEqt,	NULL))
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu ." );
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: et_controle_cb etat:%d type:%d num:%d", va_Etat, va_TypeEqt, va_NumEqt );
   
   switch ( va_TypeEqt )
   {
      case XDC_EQT_TDP :
         for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
         {
            /* Si c'est l'equipement recherche */
            if ( (vg_DonneesUGTP[i].Numero == va_NumEqt) &&
		 (vg_DonneesUGTP[i].ChampValide == Valide) )
            {
               if( va_Etat > XZEXC_LIMITE_ETAT_EQT )
               {
                  vg_DonneesUGTP[i].Etat_Service &= va_Etat;
               }
               else
               {
                  vg_DonneesUGTP[i].Etat_Service |= va_Etat;
               }
               XZST_03EcritureTrace ( XZSTC_WARNING, "UGTP %d Etat de service: %d",
                  			va_NumEqt, vg_DonneesUGTP[i].Etat_Service );
               break;
            }
         }
         break;
      
      case XDC_EQT_ZDP :
         for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
         {
            for( j=0; ((vg_DonneesUGTP[i].ChampValide == Valide) && (j<=ETDPC_NB_MAX_ZONE)); j++)
            {
               /* Si c'est l'equipement recherche */
               if ( (vg_Mesure[i].Numero[j] == va_NumEqt) &&
                    (vg_DonneesUGTP[i].Zone[j].ChampValide == Valide) )
               {
                  if( va_Etat > XZEXC_LIMITE_ETAT_EQT )
                  {
                     vg_DonneesUGTP[i].Zone[j].Etat_Service &= va_Etat;
                  }
                  else
                  {
                     vg_DonneesUGTP[i].Zone[j].Etat_Service |= va_Etat;
                  }
                  XZST_03EcritureTrace ( XZSTC_WARNING, "Zone %d Etat de service: %d",
                     			va_NumEqt, vg_DonneesUGTP[i].Zone[j].Etat_Service );
                  break;
               }
            }
         }
         break;
   }
         
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_controle_cb ");
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Acquite la reception des donnees temps de parcours
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_ACQ_donnees_TDP_cb( T_IPC_CONN va_Cnx,
				T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG)
 
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
*
* FONCTION
*
*  Suivant la commande -> envoi du msg XDM_ACQ_DONNEES_TDP
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      : et_ACQ_donnees_TDP_cb " ;

#ifdef _TIBCO_EMS
   XDY_Mot	va_CleAcquittement;
   XDY_Mot	va_NumUGTP;
#else
   T_INT2	va_CleAcquittement;
   T_INT2	va_NumUGTP;
#endif
   int		i = 0;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: et_ACQ_donnees_TDP_cb ");
		  
   /*A Recuperation des parametres du message XDM_ACQ_DONNEES_TDP */
   /* Si probleme de recuperation */
   
   if( !TipcMsgRead( 	pa_Data -> msg, 
   			T_IPC_FT_INT2, &va_NumUGTP, 
   			T_IPC_FT_INT2, &va_CleAcquittement, 
   			NULL))
   {
      /* Alors */     
      /* Trace : Impossibilite de lire le message XDM_ACQ_DONNEES_TDP */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ACQ_DONNEES_TDP non lu." );
   }
   else
   {
      for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
      {
         /* Si c'est l'equipement recherche */
         if ( vg_DonneesUGTP[i].Numero == va_NumUGTP ) 
         {
            /* Sinon */
            switch ( va_CleAcquittement )
            {
               case ETDPC_REVEIL_6MN :
                  /* envoi mesure 6mn suivante */
                  et_Envoi_Mesure_6mn ( i, XDC_VRAI );
                  break;
               case ETDPC_REVEIL_1H :
                  /* envoi mesure horaire suivante */
                  et_Envoi_Mesure_TDP_Horaire ( i, XDC_VRAI );
                  break;
            }
            break;
         }
      }
   }
   /* Finsi */
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_ACQ_donnees_TDP_cb ");
   return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande affichage TDP sur PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	et_cmd_PMV_cb ( T_IPC_CONN			va_Cnx,
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_ETDP_PMV
*
* FONCTION 
*  Demande d'affichage TDP sur PMV
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      :  et_cmd_PMV_cb " ;

   ETDP_CONFIG_PMV *		pl_PMV;
   
#ifndef _TIBCO_EMS   
   T_STR			vl_Texte1;
   T_STR			vl_Texte2;
   T_STR			vl_Texte3;
   T_STR			vl_Texte4;
   T_INT2			vl_Numero;
   T_INT2			vl_Affichage1;
   T_CHAR			vl_Clign1;
   T_INT2			vl_Affichage2;
   T_CHAR			vl_Clign2;
   T_INT2			vl_Affichage3;
   T_CHAR			vl_Clign3;
   T_INT2			vl_Affichage4;
   T_CHAR			vl_Clign4;
   T_CHAR			vl_Flash;
   T_INT4			vl_NoCMD;
   T_INT4			vl_Dest;
   T_INT4			vl_DestAlt;
   T_INT4			vl_DestNoeud;
#else
   XDY_Ligne_PMV	       	vl_Texte1;
   XDY_Ligne_PMV       		vl_Texte2;
   XDY_Ligne_PMV		vl_Texte3;
   XDY_Ligne_PMV		vl_Texte4;
   XDY_Mot			vl_Numero;
   XDY_Mot			vl_Affichage1;
   XDY_Octet			vl_Clign1;
   XDY_Mot			vl_Affichage2;
   XDY_Octet			vl_Clign2;
   XDY_Mot			vl_Affichage3;
   XDY_Octet			vl_Clign3;
   XDY_Mot			vl_Affichage4;
   XDY_Octet			vl_Clign4;
   XDY_Octet			vl_Flash;
   XDY_Entier			vl_NoCMD;
   XDY_Entier			vl_Dest;
   XDY_Entier			vl_DestAlt;
   XDY_Entier			vl_DestNoeud;
#endif

#ifndef _TIBCO_EMS
   T_STR			vl_Alternat1;
   T_STR			vl_Alternat2;
   T_STR			vl_Alternat3;
   T_STR			vl_Alternat4;
#else
   XDY_Ligne_PMV	       	vl_Alternat1;
   XDY_Ligne_PMV       		vl_Alternat2;
   XDY_Ligne_PMV		vl_Alternat3;
   XDY_Ligne_PMV		vl_Alternat4;
#endif
        
        /*A Lecture du message */

   	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_STR,  &vl_Texte1
                          ,T_IPC_FT_INT2, &vl_Affichage1
                          ,T_IPC_FT_STR,  &vl_Alternat1
                          ,T_IPC_FT_CHAR, &vl_Clign1
                          ,T_IPC_FT_STR,  &vl_Texte2
                          ,T_IPC_FT_INT2, &vl_Affichage2
                          ,T_IPC_FT_STR,  &vl_Alternat2
                          ,T_IPC_FT_CHAR, &vl_Clign2
                          ,T_IPC_FT_STR,  &vl_Texte3
                          ,T_IPC_FT_INT2, &vl_Affichage3
                          ,T_IPC_FT_STR,  &vl_Alternat3
                          ,T_IPC_FT_CHAR, &vl_Clign3
                          ,T_IPC_FT_STR,  &vl_Texte4
                          ,T_IPC_FT_INT2, &vl_Affichage4
                          ,T_IPC_FT_STR,  &vl_Alternat4
                          ,T_IPC_FT_CHAR, &vl_Clign4
                          ,T_IPC_FT_CHAR, &vl_Flash
                          ,T_IPC_FT_INT4, &vl_NoCMD
                          ,T_IPC_FT_INT4, &vl_Dest
                          ,T_IPC_FT_INT4, &vl_DestAlt
                          ,T_IPC_FT_INT4, &vl_DestNoeud
                          ,NULL))
	          
      	{ 
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ETDP_PMV non lu \n" );
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_cmd_PMV_cb");
           return;
	}
        /* Recherche du PMV dans la liste */
        for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant )
          if (( pl_PMV->PMV.Numero == vl_Numero ) &&
		/* si le pmv affiche l'heure et qu'il a des destination tdp alors on ne memorise pas pour ne pas écraser les lib des dest° */
		!( (vl_Dest || vl_DestAlt || vl_DestNoeud) && (vl_Affichage2==XDC_PMV_HEURE)) )
          {
             /* Mise a jour memoire etat commande */
             pl_PMV->EtatPMV.Numero 		 = vl_Numero;
             pl_PMV->NumAction         		 = /* vl_NoCMD */ 0; 
             pl_PMV->EtatPMV.Destination         = vl_Dest;
             pl_PMV->EtatPMV.DestinationAlternat = vl_DestAlt;
			 pl_PMV->EtatPMV.DestinationNoeud = vl_DestNoeud;
       
             strcpy(pl_PMV->EtatPMV.Texte1,vl_Texte1);
             pl_PMV->EtatPMV.Affichage1=XDC_PMV_TEXTE;
             strcpy(pl_PMV->EtatPMV.Alternat1,vl_Alternat1);
             pl_PMV->EtatPMV.Clign1=vl_Clign1;

             strcpy(pl_PMV->EtatPMV.Texte2,vl_Texte2);
             pl_PMV->EtatPMV.Affichage2=XDC_PMV_TEXTE;
             strcpy(pl_PMV->EtatPMV.Alternat2,vl_Alternat2);
             pl_PMV->EtatPMV.Clign2=vl_Clign2;

             strcpy(pl_PMV->EtatPMV.Texte3,vl_Texte3);
             pl_PMV->EtatPMV.Affichage3=XDC_PMV_TEXTE;
             strcpy(pl_PMV->EtatPMV.Alternat3,vl_Alternat3);
             pl_PMV->EtatPMV.Clign3=vl_Clign3;
             pl_PMV->EtatPMV.Flash=vl_Flash;

             strcpy(pl_PMV->EtatPMV.Texte4,vl_Texte4);
             pl_PMV->EtatPMV.Affichage4=XDC_PMV_TEXTE;
             strcpy(pl_PMV->EtatPMV.Alternat4,vl_Alternat4);
             pl_PMV->EtatPMV.Clign4=XDC_PMV_PASCLIGNO;

             /* Trace du message en entree de la fonction */
             XZST_03EcritureTrace(XZSTC_WARNING
 	      ,"IN Callback et_cmd_PMV_cb, msg= %d T1\"%s\" %d A1\"%s\" C%d T2\"%s\" %d A2\"%s\" C%d T3\"%s\" %d A3\"%s\" C%d T4\"%s\" %d A4\"%s\" C%d F%d %d Dest:%d DestAlt:%d DestNoeud:%d",
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
                           		vl_NoCMD,
                           		vl_Dest,
                           		vl_DestAlt,
								vl_DestNoeud );
            break;
          }

      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_cmd_PMV_cb");
 		                            
  
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande calcul TDP pour IHM PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	et_demande_TDP_cb ( T_IPC_CONN			va_Cnx,
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_ETDP_IHM
*
* FONCTION 
*  Demande calcul TDP pour IHM PMV.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      :  et_demande_TDP_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			vl_DgRep;
   T_INT2			vl_Numero;
   T_INT2			vl_Dest;
   T_INT2			vl_DestAlt;
   #else
   XDY_Datagroup                vl_DgRep;
   XDY_Mot			vl_Numero;
   XDY_Mot			vl_Dest;
   XDY_Mot			vl_DestAlt;
   #endif   

   T_INT4			vl_DestNoeud;
   ETDP_CONFIG_PMV *		pl_PMV;
   XDY_TDP			vl_TDP = 0;
   XDY_Ligne_PMV		vl_LigneTDP = "";
   XDY_Ligne_PMV		vl_LigneAltTDP = "";
   XDY_Ligne_PMV		vl_LigneNoeudTDP = "";
   XDY_Ligne_PMV		vl_nom = "";
   
   
        
        /*A Lecture du message */

   	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_INT2, &vl_Dest
                          ,T_IPC_FT_INT2, &vl_DestAlt
                          ,T_IPC_FT_STR,  &vl_DgRep
						  ,T_IPC_FT_INT4, &vl_DestNoeud
                          ,NULL))
	          
      	{ 
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ETDP_IHM non lu \n" );
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_demande_TDP_cb");
           return;
	}
	
        /* Trace du message en entree de la fonction */
        XZST_03EcritureTrace(XZSTC_FONCTION, "IN Callback et_demande_TDP_cb, msg= %d  Dest:%d DestAlt:%d noeud=%d",
                           		vl_Numero, vl_Dest, vl_DestAlt, vl_DestNoeud );
                           		
        if ( et_calcul_temps_de_parcours(vl_Numero, vl_Dest, &vl_TDP,vl_nom) == XDC_OK )
        {
           /*si TDP non valide , on renvoie chaine vide */
	   if ( vl_TDP <= 0 )
		sprintf( vl_LigneTDP, "");
	   else {
		/* La duree est superieure a 10 heures ? */
                if ( vl_TDP >= 600 )
                {
                     sprintf( vl_LigneTDP, ETDPC_TEXTE_TDP_HOR, 9, 59);
                }
		/* entre 1 heure et 10 heures */
                else if ( vl_TDP > 59 )
                {
                     sprintf( vl_LigneTDP, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
                }
		else
	   	  sprintf ( vl_LigneTDP, ETDPC_TEXTE_TDP, vl_TDP );
	   }
	   if ( vl_DestAlt != 0 )
	   {
              et_calcul_temps_de_parcours ( vl_Numero, vl_DestAlt, &vl_TDP,vl_nom );
           /*si TDP non valide , on renvoie chaine vide */
	   if ( vl_TDP <= 0 )
		sprintf( vl_LigneAltTDP, "");
	   else  {
		/* La duree est superieure a 10 heures ? */
                if ( vl_TDP >= 600 )
                {
                     sprintf( vl_LigneAltTDP, ETDPC_TEXTE_TDP_HOR, 9, 59);
                }
                /* entre 1 heure et 10 heures */
                else if ( vl_TDP > 59 )
                {
                     sprintf( vl_LigneAltTDP, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
                }
                else
                  sprintf (vl_LigneAltTDP, ETDPC_TEXTE_TDP, vl_TDP );

  		}
	   }
   
/* Calcul du temps de parcours entre le PMV et cette destination */
	   if ( vl_DestNoeud != 0 )
	   {
			et_calcul_temps_de_parcours ( vl_Numero, vl_DestNoeud, &vl_TDP,vl_nom );
           /*si TDP non valide , on renvoie chaine vide */
		   if ( vl_TDP <= 0 )
			sprintf( vl_LigneNoeudTDP, "");
		   else  {
		/* La duree est superieure a 10 heures ? */
                if ( vl_TDP >= 600 )
                {
                     sprintf( vl_LigneNoeudTDP, ETDPC_TEXTE_TDP_HOR, 9, 59);
                }
                /* entre 1 heure et 10 heures */
                else if ( vl_TDP > 59 )
                {
                     sprintf( vl_LigneNoeudTDP, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
                }
                else
                  sprintf (vl_LigneNoeudTDP, ETDPC_TEXTE_TDP, vl_TDP );

				}
	   }
	
     	   if (!TipcSrvMsgWrite ( 	vl_DgRep,
      			        	XDM_IdentMsg(XDM_VAL_TDP),
                          		XDM_FLG_SRVMSGWRITE,
               				T_IPC_FT_INT2, 	vl_Numero,
               				T_IPC_FT_STR, 	vl_LigneTDP,
               				T_IPC_FT_STR, 	vl_LigneAltTDP,
							T_IPC_FT_STR,	vl_LigneNoeudTDP,
                          		NULL))
      	   {
              XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_VAL_TDP non effectue." );
	   }
	   else
           {
              XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_VAL_TDP effectue sur %s" ,vl_DgRep );
	   }
	}
	
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_demande_TDP_cb");
 		                            
  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception mesure zone UGTP voisin.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	et_TDP_voisin_cb ( T_IPC_CONN			va_Cnx,
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_EMES_TDP_6MN_PAR_ZONE
*
* FONCTION 
*  Demande calcul TDP pour IHM PMV.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      :  et_TDP_voisin_cb " ;

   XZEZT_Liste_Zdp *	pl_Zone;
#ifdef _TIBCO_EMS
   T_REAL8		vl_Horodate;
   T_INT2		vl_Numero;
   T_INT2		vl_TDP6mn;
   T_INT2		vl_Validite6mn;
#else
   XDY_Horodate		vl_Horodate;
   XDY_Mot		vl_Numero;
   XDY_Mot		vl_TDP6mn;
   XDY_Mot		vl_Validite6mn;
#endif
   int			vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_EMES_TDP_6MN_PAR_ZONE */
   if ( !TipcMsgRead(data -> msg,
               	     	T_IPC_FT_REAL8,	&vl_Horodate,
               		T_IPC_FT_INT2, 	&vl_Numero,
               		T_IPC_FT_INT2, 	&vl_TDP6mn,
               		T_IPC_FT_INT2, 	&vl_Validite6mn,	 
	       		NULL) )
   { 
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EMES_TDP_6MN_PAR_ZONE non lu \n" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_TDP_voisin_cb");
      return;
   }
	
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN Callback et_TDP_voisin_cb, Zone= %d  TDP:%d Dispo:%d",
                           		vl_Numero, vl_TDP6mn, vl_Validite6mn );

   if ( !vl_Horodate )
   {
      /* Appel fonction rafraichissement PMV */
/*!!      et_Rafraichissment_PMV ( ); */
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_TDP_voisin_cb apres et_Rafraichissment_PMV");
      return; 
   }
   /* Recherche de la zone dans la liste et mise a jour du TDP courant */
   for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant ) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "zone numero %d, code %d",
    			pl_Zone->zdp.Numero, pl_Zone->zdp.Code_Zone);
    if ( pl_Zone->zdp.Numero == vl_Numero )
    {
	XZST_03EcritureTrace(XZSTC_FONCTION, "maj de TDP");
        if ( vl_Validite6mn == XDC_VRAI ) 
		pl_Zone->zdp.TDP_Courant = vl_TDP6mn;
	else
		{
		XZST_03EcritureTrace(XZSTC_WARNING,"et_TDP_voisin_cb Donnée invalide, Zone= %d ,Code=%d TDP:%d Dispo:%d",
                           		vl_Numero, pl_Zone->zdp.Code_Zone,vl_TDP6mn, vl_Validite6mn );

		if ( pl_Zone->zdp.Code_Zone == 0) /* PNI: Si le code est nul (zone virtuelle, alors on prend le tdp habituel */
			pl_Zone->zdp.TDP_Courant = vl_TDP6mn;
		else
			pl_Zone->zdp.TDP_Courant = 0;        
		}
	break;
    }
    }
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_TDP_voisin_cb");
 		                            
  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception compte rendu affichage PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	et_CompteRendu_cb ( T_IPC_CONN			va_Cnx,
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
*  Fonction appelÅe lors de la rÅception du message XDM_CR_TDP
*  
*
* FONCTION 
*  Mise a jour compte rendu action.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.6 06/23/11      :  et_CompteRendu_cb " ;

   ETDP_CONFIG_PMV *	pl_PMV;
#ifdef _TIBCO_EMS
   T_INT4		vl_NumAction;
   T_INT2		vl_CR;
   T_INT2		vl_NumPMV;
#else
   XDY_Entier		vl_NumAction;
   XDY_Mot		vl_CR;
   XDY_Mot		vl_NumPMV;
#endif
   int			vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_CR_TDP */
   if ( !TipcMsgRead(data -> msg,
               		T_IPC_FT_INT4, 	&vl_NumAction,
               		T_IPC_FT_INT2, 	&vl_CR,
               		T_IPC_FT_INT2, 	&vl_NumPMV,	 
	       		NULL) )
   { 
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_CR_TDP non lu \n" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_CompteRendu_cb");
      return;
   }
	
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING, "IN Callback et_CompteRendu_cb, PMV= %d  Action:%d CR:%d",
                           		vl_NumPMV, vl_NumAction, vl_CR );

   /* Recherche du PMV dans la liste et mise a jour du Compte rendu */
   for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant )
      if ( pl_PMV->PMV.Numero == vl_NumPMV )
      {
/*         if ( vl_CR == XDC_NOK )
         {
            pl_PMV->EtatPMV.Destination = 0;
            pl_PMV->EtatPMV.DestinationAlternat = 0;
			pl_PMV->EtatPMV.DestinationNoeud = 0;
            pl_PMV->NumAction = 0;
         }
         else*/
         {
            pl_PMV->NumAction = vl_NumAction;
         }
         break;
      }
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_CompteRendu_cb ");
 		                            
  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande affichage TDP sur PMVA.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void	et_cmd_PMVA_cb ( T_IPC_CONN			va_Cnx,
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
*  Fonction appelée lors de la réception du message
*  XDM_ETDP_PMVA
*
* FONCTION
*  Demande d'affichage TDP sur PMVA
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.8 29/05/12      :  et_cmd_PMVA_cb " ;

#ifndef _TIBCO_EMS   
   T_INT2			vl_Numero;
   T_INT2			vl_Affichage1;
   T_CHAR			vl_Clign1;
   T_INT2			vl_Affichage2;
   T_CHAR			vl_Clign2;
   T_INT2			vl_Affichage3;
   T_CHAR			vl_Clign3;
   T_INT2			vl_Affichage4;
   T_CHAR			vl_Clign4;
   T_CHAR			vl_Flash;
   T_INT4			vl_NoCMD;
   T_STR			vl_Texte1;
   T_STR			vl_Texte2;
   T_STR			vl_Texte3;
   T_STR			vl_Texte4;
   T_CHAR			vl_Clign5;
   T_INT2			vl_Dest1_proche;
   T_INT2			vl_Dest1_loin;
   T_INT2			vl_Dest2_proche;
   T_INT2			vl_Dest2_loin;
   T_INT2			vl_Affichage5;
#else
   XDY_Mot			vl_Numero;
   XDY_Mot			vl_Affichage1;
   XDY_Octet			vl_Clign1;
   XDY_Mot			vl_Affichage2;
   XDY_Octet			vl_Clign2;
   XDY_Mot			vl_Affichage3;
   XDY_Octet			vl_Clign3;
   XDY_Mot			vl_Affichage4;
   XDY_Octet			vl_Clign4;
   XDY_Octet			vl_Flash;
   XDY_Entier			vl_NoCMD;
   XDY_Ligne_PMVA	       	vl_Texte1;
   XDY_Ligne_PMVA              	vl_Texte2;
   XDY_Ligne_PMVA		vl_Texte3;
   XDY_Ligne_PMVA		vl_Texte4;
   XDY_Octet			vl_Clign5;
   XDY_Mot			vl_Dest1_proche;
   XDY_Mot			vl_Dest1_loin;
   XDY_Mot			vl_Dest2_proche;
   XDY_Mot			vl_Dest2_loin;
   XDY_Mot			vl_Affichage5;
#endif

#ifndef _TIBCO_EMS
   T_STR			vl_Alternat1;
   T_STR			vl_Alternat2;
   T_STR			vl_Alternat3;
   T_STR			vl_Alternat4;
#else
   XDY_Ligne_PMVA	       	vl_Alternat1;
   XDY_Ligne_PMVA              	vl_Alternat2;
   XDY_Ligne_PMVA		vl_Alternat3;
   XDY_Ligne_PMVA		vl_Alternat4;
#endif   


   /* ** */
#ifndef _TIBCO_EMS
   T_STR			vl_Texte5;
#else
   XDY_Ligne_PMVA		vl_Texte5;
#endif 

#ifndef _TIBCO_EMS
   T_STR			vl_Alternat5;
#else
   XDY_Ligne_PMVA		vl_Alternat5;
#endif

   /* ** */

   ETDP_CONFIG_PMVA *pl_PMVA;

        /*A Lecture du message */
      	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_STR,  &vl_Texte1
                          ,T_IPC_FT_INT2, &vl_Affichage1
                          ,T_IPC_FT_STR,  &vl_Alternat1
                          ,T_IPC_FT_CHAR, &vl_Clign1
                          ,T_IPC_FT_STR,  &vl_Texte2
                          ,T_IPC_FT_INT2, &vl_Affichage2
                          ,T_IPC_FT_STR,  &vl_Alternat2
                          ,T_IPC_FT_CHAR, &vl_Clign2
                          ,T_IPC_FT_STR,  &vl_Texte3
                          ,T_IPC_FT_INT2, &vl_Affichage3
                          ,T_IPC_FT_STR,  &vl_Alternat3
                          ,T_IPC_FT_CHAR, &vl_Clign3
                          ,T_IPC_FT_STR,  &vl_Texte4
                          ,T_IPC_FT_INT2, &vl_Affichage4
                          ,T_IPC_FT_STR,  &vl_Alternat4
                          ,T_IPC_FT_CHAR, &vl_Clign4
                          ,T_IPC_FT_STR,  &vl_Texte5
                          ,T_IPC_FT_INT2, &vl_Affichage5
                          ,T_IPC_FT_STR,  &vl_Alternat5
                          ,T_IPC_FT_CHAR, &vl_Clign5
                          ,T_IPC_FT_CHAR, &vl_Flash
                          ,T_IPC_FT_INT4, &vl_NoCMD
                          ,T_IPC_FT_INT2, &vl_Dest1_proche
                          ,T_IPC_FT_INT2, &vl_Dest1_loin
                          ,T_IPC_FT_INT2, &vl_Dest2_proche
                          ,T_IPC_FT_INT2, &vl_Dest2_loin
                          ,NULL))

      	{
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ETDP_PMVA non lu \n" );
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_cmd_PMVA_cb");
           return;
      	}

        /* Recherche du PMVA dans la liste */
        for ( pl_PMVA = pg_ListePMVA; pl_PMVA != NULL; pl_PMVA = pl_PMVA->suivant )
          if (( pl_PMVA->PMVA.Numero == vl_Numero ) &&
		/* si le pmva affiche l'heure et qu'il a des destination tdp alors on ne memorise pas pour ne pas écraser les lib des dest� */
		!( (vl_Dest1_proche || vl_Dest1_loin || vl_Dest2_proche || vl_Dest2_loin) && (vl_Affichage2==XDC_PMVA_HEURE)) )
          {
             /* Mise a jour memoire etat commande */
             pl_PMVA->EtatPMVA.Numero 		 = vl_Numero;
             pl_PMVA->NumAction         		 = /* vl_NoCMD */ 0;
             pl_PMVA->EtatPMVA.Dest1_proche = vl_Dest1_proche;
             pl_PMVA->EtatPMVA.Dest1_loin = vl_Dest1_loin;
			 pl_PMVA->EtatPMVA.Dest2_proche = vl_Dest2_proche;
			 pl_PMVA->EtatPMVA.Dest2_loin = vl_Dest2_loin;

             strcpy(pl_PMVA->EtatPMVA.Texte1,vl_Texte1);
             pl_PMVA->EtatPMVA.Affichage1=XDC_PMVA_TEXTE;
             strcpy(pl_PMVA->EtatPMVA.Alternat1,vl_Alternat1);
             pl_PMVA->EtatPMVA.Clign1=vl_Clign1;

             strcpy(pl_PMVA->EtatPMVA.Texte2,vl_Texte2);
             pl_PMVA->EtatPMVA.Affichage2=XDC_PMVA_TEXTE;
             strcpy(pl_PMVA->EtatPMVA.Alternat2,vl_Alternat2);
             pl_PMVA->EtatPMVA.Clign2=vl_Clign2;

             strcpy(pl_PMVA->EtatPMVA.Texte3,vl_Texte3);
             pl_PMVA->EtatPMVA.Affichage3=XDC_PMVA_TEXTE;
             strcpy(pl_PMVA->EtatPMVA.Alternat3,vl_Alternat3);
             pl_PMVA->EtatPMVA.Clign3=vl_Clign3;

             strcpy(pl_PMVA->EtatPMVA.Texte4,vl_Texte4);
             pl_PMVA->EtatPMVA.Affichage4=XDC_PMVA_TEXTE;
             strcpy(pl_PMVA->EtatPMVA.Alternat4,vl_Alternat4);
             pl_PMVA->EtatPMVA.Clign4=vl_Clign4;

             strcpy(pl_PMVA->EtatPMVA.Texte5,vl_Texte5);
             pl_PMVA->EtatPMVA.Affichage5=XDC_PMVA_TEXTE;
             strcpy(pl_PMVA->EtatPMVA.Alternat5,vl_Alternat5);
             pl_PMVA->EtatPMVA.Clign5=vl_Clign5;

             /* Trace du message en entree de la fonction */
             XZST_03EcritureTrace(XZSTC_WARNING
 	      ,"IN Callback et_cmd_PMVA_cb, msg= %d T1\"%s\" %d A1\"%s\" C%d T2\"%s\" %d A2\"%s\" C%d T3\"%s\" %d A3\"%s\" C%d T4\"%s\" %d A4\"%s\" C%d T5\"%s\" %d A5\"%s\" C%d F%d %d Dest1_proche:%d Dest1_loin:%d Dest2_proche:%d Dest2_loin:%d",
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
                           		vl_Texte5,
								vl_Affichage5,
								vl_Alternat5,
								vl_Clign5,
                           		vl_Flash,
                           		vl_NoCMD,
                           		vl_Dest1_proche,
                           		vl_Dest1_loin,
                           		vl_Dest2_proche,
                           		vl_Dest2_loin);
            break;
          }

      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_cmd_PMVA_cb");
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande calcul TDP pour IHM PMVA.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void	et_demande_TDP_PMVA_cb( T_IPC_CONN			va_Cnx,
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
*  Fonction appelée lors de la réception du message
*  XDM_ETDP_IHM_PMVA
*
* FONCTION
*  Demande calcul TDP pour IHM PMVA.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.8 29/05/12      :  et_demande_TDP_cb " ;

   #ifndef _TIBCO_EMS
   T_INT2			vl_Numero;
   T_INT2			vl_Dest1_proche;
   T_INT2			vl_Dest1_loin;
   T_INT2			vl_Dest2_proche;
   T_INT2			vl_Dest2_loin;
   T_STR			vl_DgRep;
   #else
   XDY_Mot			vl_Numero;
   XDY_Mot			vl_Dest1_proche;
   XDY_Mot			vl_Dest1_loin;
   XDY_Mot			vl_Dest2_proche;
   XDY_Mot			vl_Dest2_loin;
   XDY_Datagroup                vl_DgRep;
   #endif

   ETDP_CONFIG_PMVA *		pl_PMVA;

   XDY_TDP				vl_TDP = 0;
   XDY_Ligne_PMVA		vl_LigneTDP1 = "";
   XDY_Ligne_PMVA		vl_LigneTDP2 = "";
   XDY_Ligne_PMVA		vl_LigneTDP3 = "";
   XDY_Ligne_PMVA		vl_LigneTDP4 = "";
   XDY_Datagroup     vl_dg="";
   char vl_nom[12]="";

        /*A Lecture du message */
   	if(!TipcMsgRead(data->msg
                          ,T_IPC_FT_INT2, &vl_Numero
                          ,T_IPC_FT_INT2, &vl_Dest1_proche
                          ,T_IPC_FT_INT2, &vl_Dest1_loin
                          ,T_IPC_FT_INT2, &vl_Dest2_proche
                          ,T_IPC_FT_INT2, &vl_Dest2_loin
                          ,T_IPC_FT_STR,  &vl_DgRep
                          ,NULL))
      	{
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ETDP_IHM_PMVA non lu \n" );
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_demande_TDP_PMVA_cb");
           return;
      	}

        /* Trace du message en entree de la fonction */
	     strcpy(vl_dg,vl_DgRep);

        XZST_03EcritureTrace(XZSTC_FONCTION, "IN Callback et_demande_TDP_PMVA_cb, msg= %d  Dest1_proche:%d Dest1_loin:%d Dest2_proche:%d Dest2_loin:%d a destination de %s/%s",
                           		vl_Numero, vl_Dest1_proche, vl_Dest1_loin, vl_Dest2_proche, vl_Dest2_loin, vl_dg, vl_DgRep );

        if ( et_calcul_temps_de_parcours_PMVA(vl_Numero, vl_Dest1_proche, &vl_TDP,vl_nom) == XDC_OK )
        {
            /*si TDP non valide , on renvoie chaine vide */
        	if ( vl_TDP <= 0 )
        		sprintf( vl_LigneTDP1, "");
        	else {
        		/* La duree est superieure a 10 heures ? */
                if ( vl_TDP >= 600 )
                {
                     sprintf( vl_LigneTDP1, ETDPC_TEXTE_TDP_HOR, 9, 59);
                }
                /* entre 1 heure et 10 heures */
                else if ( vl_TDP > 59 )
                {
                     sprintf( vl_LigneTDP1, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
                }
                else
                	sprintf ( vl_LigneTDP1, ETDPC_TEXTE_TDP, vl_TDP );
        	}
        	if ( vl_Dest1_loin != 0 )
        	{
        		et_calcul_temps_de_parcours_PMVA ( vl_Numero, vl_Dest1_loin, &vl_TDP, vl_nom );
        		/*si TDP non valide , on renvoie chaine vide */
        		if ( vl_TDP <= 0 )
        			sprintf( vl_LigneTDP2, "");
        		else  {
        			/* La duree est superieure a 10 heures ? */
					if ( vl_TDP >= 600 )
					{
						 sprintf( vl_LigneTDP2, ETDPC_TEXTE_TDP_HOR, 9, 59);
					}
					/* entre 1 heure et 10 heures */
					else if ( vl_TDP > 59 )
					{
						 sprintf( vl_LigneTDP2, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
					}
					else
					  sprintf (vl_LigneTDP2, ETDPC_TEXTE_TDP, vl_TDP );

        		}
        	}
        	if ( vl_Dest2_proche != 0 )
			{
				et_calcul_temps_de_parcours_PMVA ( vl_Numero, vl_Dest2_proche, &vl_TDP , vl_nom);
				/*si TDP non valide , on renvoie chaine vide */
				if ( vl_TDP <= 0 )
					sprintf( vl_LigneTDP3, "");
				else  {
					/* La duree est superieure a 10 heures ? */
					if ( vl_TDP >= 600 )
					{
						 sprintf( vl_LigneTDP3, ETDPC_TEXTE_TDP_HOR, 9, 59);
					}
					/* entre 1 heure et 10 heures */
					else if ( vl_TDP > 59 )
					{
						 sprintf( vl_LigneTDP3, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
					}
					else
					  sprintf (vl_LigneTDP3, ETDPC_TEXTE_TDP, vl_TDP );

				}
			}
        	if ( vl_Dest2_loin != 0 )
			{
				et_calcul_temps_de_parcours_PMVA ( vl_Numero, vl_Dest2_loin, &vl_TDP,vl_nom );
				/*si TDP non valide , on renvoie chaine vide */
				if ( vl_TDP <= 0 )
					sprintf( vl_LigneTDP4, "");
				else  {
					/* La duree est superieure a 10 heures ? */
					if ( vl_TDP >= 600 )
					{
						 sprintf( vl_LigneTDP4, ETDPC_TEXTE_TDP_HOR, 9, 59);
					}
					/* entre 1 heure et 10 heures */
					else if ( vl_TDP > 59 )
					{
						 sprintf( vl_LigneTDP4, ETDPC_TEXTE_TDP_HOR, vl_TDP/60, vl_TDP%60 );
					}
					else
					  sprintf (vl_LigneTDP4, ETDPC_TEXTE_TDP, vl_TDP );

				}
			}

        	if (!TipcSrvMsgWrite ( 	vl_DgRep,
      			        	XDM_IdentMsg(XDM_VAL_TDP_PMVA),
                          		XDM_FLG_SRVMSGWRITE,
               				T_IPC_FT_INT2, 	vl_Numero,
               				T_IPC_FT_STR, 	vl_LigneTDP1,
               				T_IPC_FT_STR, 	vl_LigneTDP2,
               				T_IPC_FT_STR, 	vl_LigneTDP3,
               				T_IPC_FT_STR, 	vl_LigneTDP4,	NULL))
      	   {
              XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_VAL_TDP_PMVA non effectue." );
	   }
	   else
           {
              XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_VAL_TDP_PMVA effectue sur %s" ,vl_dg );
	   }
	}

        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_demande_TDP_PMVA_cb");


}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception mesure zone UGTP voisin.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void	et_TDP_voisin_PMVA_cb ( T_IPC_CONN			va_Cnx,
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
*  XDM_EMES_TDP_6MN_PAR_ZONE
*
* FONCTION
*  Demande calcul TDP pour IHM PMVA.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.8 29/05/12      :  et_TDP_voisin_cb " ;

   XZEZT_Liste_Zdp *	pl_Zone;
#ifdef _TIBCO_EMS
   T_REAL8		vl_Horodate;
   T_INT2		vl_Numero;
   T_INT2		vl_TDP6mn;
   T_INT2		vl_Validite6mn;
#else
   XDY_Horodate		vl_Horodate;
   XDY_Mot		vl_Numero;
   XDY_Mot		vl_TDP6mn;
   XDY_Mot		vl_Validite6mn;
#endif
   int			vl_Resultat = XDC_OK;

   /* Recuperation des parametres du message XDM_EMES_TDP_6MN_PAR_ZONE */
   if ( !TipcMsgRead(data -> msg,
               	    T_IPC_FT_REAL8,	&vl_Horodate,
               		T_IPC_FT_INT2, 	&vl_Numero,
               		T_IPC_FT_INT2, 	&vl_TDP6mn,
               		T_IPC_FT_INT2, 	&vl_Validite6mn,
	       		NULL) )
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EMES_TDP_6MN_PAR_ZONE non lu \n" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_TDP_voisin_cb");
      return;
   }

   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN Callback et_TDP_voisin_cb, Zone= %d  TDP:%d Dispo:%d",
                           		vl_Numero, vl_TDP6mn, vl_Validite6mn );

   if ( !vl_Horodate )
   {
      /* Appel fonction rafraichissement PMVA */
/*!!      et_Rafraichissment_PMVA ( ); */
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_TDP_voisin_PMVA_cb apres et_Rafraichissment_PMVA");
      return;
   }
   /* Recherche de la zone dans la liste et mise a jour du TDP courant */
   for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
    if ( pl_Zone->zdp.Numero == vl_Numero )
    {
        if ( vl_Validite6mn == XDC_VRAI )
		pl_Zone->zdp.TDP_Courant = vl_TDP6mn;
	else
		{
		XZST_03EcritureTrace(XZSTC_WARNING,"et_TDP_voisin_cb Donn�e invalide, Zone= %d ,Code=%d TDP:%d Dispo:%d",
                           		vl_Numero, pl_Zone->zdp.Code_Zone,vl_TDP6mn, vl_Validite6mn );

		if ( pl_Zone->zdp.Code_Zone == 0) /* PNI: Si le code est nul (zone virtuelle, alors on prend le tdp habituel */
			pl_Zone->zdp.TDP_Courant = vl_TDP6mn;
		else
			pl_Zone->zdp.TDP_Courant = 0;
		}
	break;
    }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_TDP_voisin_cb");


}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception compte rendu affichage PMVA.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	et_CompteRendu_PMVA_cb ( T_IPC_CONN			va_Cnx,
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
*  Fonction appelée lors de la réception du message XDM_CR_TDP
*
*
* FONCTION
*  Mise a jour compte rendu action.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etdp_cmd.c	1.8 29/05/12      :  et_CompteRendu_cb " ;

   ETDP_CONFIG_PMVA *	pl_PMVA;
#ifdef _TIBCO_EMS
   T_INT4		vl_NumAction;
   T_INT2		vl_CR;
   T_INT2		vl_NumPMVA;
#else
   XDY_Entier		vl_NumAction;
   XDY_Mot		vl_CR;
   XDY_Mot		vl_NumPMVA;
#endif
   int			vl_Resultat = XDC_OK;


   /* Recuperation des parametres du message XDM_CR_TDP */
   if ( !TipcMsgRead(data -> msg,
               		T_IPC_FT_INT4, 	&vl_NumAction,
               		T_IPC_FT_INT2, 	&vl_CR,
               		T_IPC_FT_INT2, 	&vl_NumPMVA,
	       		NULL) )
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_CR_TDP_PMVA non lu \n" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_CompteRendu_cb");
      return;
   }

   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING, "IN Callback et_CompteRendu_PMVA_cb, PMVA= %d  Action:%d CR:%d",
                           		vl_NumPMVA, vl_NumAction, vl_CR );

   /* Recherche du PMV dans la liste et mise a jour du Compte rendu */
   for ( pl_PMVA = pg_ListePMVA; pl_PMVA != NULL; pl_PMVA = pl_PMVA->suivant )
      if ( pl_PMVA->PMVA.Numero == vl_NumPMVA )
      {
         if ( vl_CR == XDC_NOK )
         {
            pl_PMVA->EtatPMVA.Dest1_proche = 0;
            pl_PMVA->EtatPMVA.Dest1_loin = 0;
			pl_PMVA->EtatPMVA.Dest2_proche = 0;
			pl_PMVA->EtatPMVA.Dest2_loin = 0;
            pl_PMVA->NumAction = 0;
         }
         else
         {
            pl_PMVA->NumAction = vl_NumAction;
         }
         break;
      }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_CompteRendu_cb ");
}
