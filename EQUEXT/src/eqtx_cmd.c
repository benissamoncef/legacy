/*E*/
/*  Fichier : $Id: eqtx_cmd.c,v 1.4 2020/11/03 18:12:21 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 18:12:21 $
-------------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TEQTX * FICHIER eqtx_cmd.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le fichier des callbacks de la tache TEQTX
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	29 Oct 1996	: Creation
* Orengo.A	version 1.2	07 Nov 1996	: Modification du nom des equipements
* GR            version 1.3     08 Juil 2020    : EMS
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include	<xdc.h>
#include	<xdm.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"

#include	"eqtx_don.h"
#include	"eqtx_cfg.h"
#include	"eqtx_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
/* -------------------------------- */

INFO_EQT_FEN_LCR	*pg_DebutListeQTXFLcr  = NULL;

/* declaration de variables locales */
/* -------------------------------- */

static char *version = "$Id: eqtx_cmd.c,v 1.4 2020/11/03 18:12:21 pc2dpdy Exp $ : eqtx_cmd" ;

/* declaration de fonctions internes */

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Interrogation depuis la fenetre LCR.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_CMD_LCR( T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_data, T_CB_ARG pa_arg)
		     
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
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cmd.c,v 1.4 2020/11/03 18:12:21 pc2dpdy Exp $ : er_CMD_LCR";

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
    T_STR			  vl_NomMachine;
    #else
    XDY_NomMachine                vl_NomMachine;
    #endif

   INFO_EQT_FEN_LCR pl_eqt;
   int i;
   int vl_indice_table_eqt=-1;
   
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN er_CMD_LCR");
	       
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
           "er_CMD_LCR : Message XDM_ELCR(QTX) recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_CMD_LCR");

      return;
   }
   /* Si l'equipement n'est pas un equipement generique (Meteo, GTC sismique ... */
   
   if( vl_TypeEqt != XDC_EQT_GEN_LCR )
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
   
   /* Si la commonde LCR ne s'adresse pas a l'ensemble des equipements generiques */
   
   if( vl_NoEqt!=XDC_EQTALL)
   {
      /* Alors */
      
      pl_eqt.NoEqt=vl_NoEqt;
      
      /* Boucle de recherche sur les equipements geres par la tache */
      
      for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
      {
         /* Si c'est l'equipement recherche */
      
         if( vg_donnees_systeme_qtx.DonneesEqt[i].Numero== vl_NoEqt &&
			       vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide== Valide )
         {
            /* Alors */
            /* Recuperer l'indice de la table des equipements */
         
            vl_indice_table_eqt=i;
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
   
      if( vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].SocketMaitre == Non_valide)
      {
         /* Alors */
         /* Trace : Impossible de transmettre la trame */
      
         XZST_03EcritureTrace(XZSTC_WARNING, 
      		" Impossible de tramsmettre la commande LCR : pas de liaison Maitre" );
         XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
         return;
      }
      /* Finsi */
      
      /* Recherche si pas de commande LCR en cours sur equipement concerne */
        
      if(ex_test_autorisation_eqt( &pg_DebutListeQTXFLcr, &pl_eqt, XDC_EQT_GEN_LCR)
      									       == XDC_NOK)
      {
         XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
      }
      else
      {
         ex_init_lcr( &pg_DebutListeQTXFLcr,pl_eqt,
                vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].SocketMaitre,
                      vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].AdresseRGS,
        	           vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Numero,
				   	      CLE_CYCLE_TIMER_QTX_FENETRE_LCR,vg_SockTIM);
      }
   }
   else
   {
      /* Sinon */
      /* Boucle sur l'ensemble des equipements de la tache */
      
      for(i=0; i!=NB_EQT_MAX_TEQTX; i++)
      {
         /* Si l'equipement est valide */
         
         if( vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide== Valide &&
		vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide== Valide )
         {
            /* Alors */

            pl_eqt.NoEqt=vg_donnees_systeme_qtx.DonneesEqt[i].Numero;
            
            /* Recherche si pas de commande LCR en cours sur equipement concerne */
        
            if(ex_test_autorisation_eqt( &pg_DebutListeQTXFLcr, &pl_eqt, XDC_EQT_GEN_LCR)
            								       == XDC_NOK)
            {
               XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
            }
            else
            {
               ex_init_lcr( &pg_DebutListeQTXFLcr,pl_eqt,
                 vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre,
                      vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS,
        	              vg_donnees_systeme_qtx.DonneesEqt[i].Numero,
				   	      CLE_CYCLE_TIMER_QTX_FENETRE_LCR,vg_SockTIM);
            }
         }
         /* Finsi */
      }
   }
   /* Finsi */
	   
   /* Fin du callback */
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback er_CMD_LCR" );

   return;
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_controle_cb( T_IPC_CONN va_Cnx,T_IPC_CONN_PROCESS_CB_DATA pa_Data,T_CB_ARG pa_arg)
 
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
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cmd.c,v 1.4 2020/11/03 18:12:21 pc2dpdy Exp $ : er_controle_cb";

    #ifndef _TIBCO_EMS
   T_INT4 va_Etat;
    T_STR		    va_NomMachine;
   T_INT4 va_TypeEqt;
   T_INT4 va_NumEqt;
    #else
   XDY_Entier va_Etat;
    XDY_NomMachine          va_NomMachine;
   XDY_Entier va_TypeEqt;
   XDY_Entier va_NumEqt;
    #endif
   
   int vl_indice_table_eqt=-1;
   int i;   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   
   if( !TipcMsgRead( pa_Data -> msg,
	T_IPC_FT_INT4,	&va_Etat,	T_IPC_FT_STR,	&va_NomMachine,
	T_IPC_FT_INT4,	&va_TypeEqt,	T_IPC_FT_INT4,	&va_NumEqt,	NULL))
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu ." );
   }
   
   for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
   {
      /* Si c'est l'equipement recherche */
      
      if( vg_donnees_systeme_qtx.DonneesEqt[i].Numero== va_NumEqt &&
				vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide== Valide )
      {
         /* Alors */
         /* Recuperer l'indice de la table des equipements */
         
         vl_indice_table_eqt=i;
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
      			" Impossible de reconnaitre le type d'equipement commande ");
      return;
   }
   /* Finsi */
	         
   if( va_Etat > XZEXC_LIMITE_ETAT_EQT )
   {
      vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Etat_Service &= va_Etat;
   }
   else
   {
      vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Etat_Service |= va_Etat;
   }
   XZST_03EcritureTrace ( XZSTC_WARNING, "Equipement QTX %d Etat de service: %d",
       va_NumEqt, vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Etat_Service );
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_controle_cb ");
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Acquite la reception des donnees meteo
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_ACQ_donnees_meteo_QTX_cb( T_IPC_CONN va_Cnx,
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
*  Suivant la commande -> envoi du msg XDM_ACQ_DONNEES_METEO_QTX
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cmd.c,v 1.4 2020/11/03 18:12:21 pc2dpdy Exp $ : er_ACQ_donnees_meteo_QTX_cb " ;

    #ifndef _TIBCO_EMS
   T_STR va_Nom_fichier_donnees_acquite;
   #else
   XDY_FichEquext  va_Nom_fichier_donnees_acquite;
#endif

   
   char vl_ArchiverMsgSyst[200];
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_ACQ_donnees_meteo_QTX_cb ");
		  
   /*A Recuperation des parametres du message XDM_ACQ_DONNEES_METEO_QTX */
   /* Si probleme de recuperation */
   
   if( !TipcMsgRead( pa_Data -> msg, T_IPC_FT_STR, &va_Nom_fichier_donnees_acquite,NULL))
   {
      /* Alors */     
      /* Trace : Impossibilite de lire le message XDM_ACQ_DONNEES_METEO_QTX */
      
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ACQ_DONNEES_METEO_QTX non lu." );
      sprintf( vl_ArchiverMsgSyst, "%s %s"," er_ACQ_donnees_meteo_QTX_cb :",
		"Message XDM_ACQ_DONNEES_METEO_QTX recu avec parametre(s) invalide(s)");
      XZST_10ArchiverMsgSyst(0,vl_ArchiverMsgSyst, NULL);
   }
   else
   {
      /* Sinon */
      /* Si le fichier existe */
      
      if(access( va_Nom_fichier_donnees_acquite, W_OK)==0)
      {
         /* Alors */
         /* Effacer le fichier historique donnees meteo */
	    
	 unlink(va_Nom_fichier_donnees_acquite);
	    
         XZST_03EcritureTrace( XZSTC_DEBUG1, "Destruction du fichier '%s'",
            						va_Nom_fichier_donnees_acquite);
      }
      else
      {
         /* Sinon */
         /* Le fichier historique donnees meteo n'existe pas ou n'a pas les bons attribus */
         
          XZST_03EcritureTrace( XZSTC_DEBUG1,
		"Destruction du fichier '%s' impossible ce fichier n'existe pas %s",
			va_Nom_fichier_donnees_acquite, "ou n'a pas les bons attribus");
      }
      /* Finsi */
   }
   /* Finsi */
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_ACQ_donnees_meteo_QTX_cb ");
   return;
}


