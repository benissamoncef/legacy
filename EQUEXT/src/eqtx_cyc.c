/*E*/
/*  Fichier : $Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2011/02/25 16:37:46 $
-------------------------------------------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TEQTX * FICHIER eqtx_cyc.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module permetant les declenchements cycliques
* de la tache TEQTX
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	29 Oct 1996	: Creation
* Orengo.A	version 1.2	05 Nov 1996	: Changement du nom des equipements
* Mismer.D	version 1.4	13 Jan 1997	: Modification protocole (DEM/1393)
* Mismer.D      version 1.5     25 Mar 1997     : Modif date pour TDP (DEM/1396)
* JMG		version 1.6	22/11/07	: ajout site de gestion DEM/715
* P.NIEPCERON	version 1.5	08 Jul 2009	: Ajout mise à l'heure v1.7 DEM898
* JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.8
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include <unistd.h>
/*
#include	<rtworks/common.h>
#include	<rtworks/ipc.h>
*/

#include 	"etim_don.h"
#include 	"ex_mala.h"
#include 	"ex_mlcr.h"

#include	"eqtx_don.h"
#include	"eqtx_cfg.h"
#include	"eqtx_cmd.h"
#include	"eqtx_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $ : eqtx_cyc" ;

/* Declaration de fonctions internes */
/* --------------------------------- */

static int ey_traitement_cyclique_etat_ligne_maitre( XZEXT_CLE_PROG );
static int ey_traitement_cyclique_etat_ligne_esclave( XZEXT_CLE_PROG, XDY_Horodate );
static int ey_traitement_cyclique_synchro_date( int, XZEXT_CLE_PROG, XDY_Horodate );
static int ey_traitement_cyclique_demande_donnees ( int, XZEXT_CLE_PROG, XDY_Horodate );

extern char                   vg_idpassw[18];

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux TIMERS
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*
*	va_NumSock : Numero de la socket
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement des evenements suivants:
*	   - L'envoi de l'heure a l'equipement 1 fois par jour
*	   - Une demande periodique declenchee suite a une demande de la fenetre LCR
*	   - Un reveil periodique pour connaitre l'etat de la liaison avec l'equipement
*	   - Un reveil periodique pour demander les donnees a l'equipement
*	   - Un reveil periodique pour une alarme liaison avec l'equipement Esclave
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $ : ey_traitement_cyclique" ;
   int vl_IndCnx = 0;
   int vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET pl_Msg;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   XDY_Horodate vl_Horodate;
   int vl_Delai;
   XZEXT_CLE_PROG pl_CleProg;
   int vl_noeqt;
   char vl_Cas = 0;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /* Si le message est correct et si ce n'est pas une deconnexion */
      
      if( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
      {
         /*A Si il s'agit d'un reveil cyclique */
         
  	 if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	 {
  	    sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					&vl_Horodate,&vl_Delai, pl_CleProg );
  	  		
            /* Demande periodique de la fenetre LCR */
            
  	    if (strstr(pl_CleProg,CLE_CYCLE_TIMER_QTX_FENETRE_LCR)!=NULL)
  	    { 
  	       sscanf(&pl_CleProg[strlen(CLE_CYCLE_TIMER_QTX_FENETRE_LCR)],
  	        					"%c%d",&vl_Cas,&vl_noeqt);
               ex_relance_lcr(vl_noeqt,vl_Cas,&pg_DebutListeQTXFLcr,
			 			vl_Horodate,pl_CleProg,va_NumSock );
               return(XDC_OK);
            }
            /* Demande periodique de l'etat de la liaison avec l'equipement */
            
            if( ey_traitement_cyclique_etat_ligne_maitre( pl_CleProg )==XDC_OK)
            {
               return(XDC_OK);
            }
	  
	    /*A Si il s'agit d'un reveil pour une alarme liaison equipement Esclave */
           
            if( ey_traitement_cyclique_etat_ligne_esclave(
            						pl_CleProg, vl_Horodate)==XDC_OK)
            {
               return(XDC_OK);
            }
	  
            
            /*A Si il s'agit d'un reveil pour une demande d'envoi de donnees */
            
            if( ey_traitement_cyclique_demande_donnees(
            				va_NumSock, pl_CleProg, vl_Horodate)==XDC_OK)
            {
               return(XDC_OK);
            }
	  
            
	    /*A Si il s'agit d'un reveil pour synchro de la date */
           
            if( ey_traitement_cyclique_synchro_date(
            				va_NumSock, pl_CleProg, vl_Horodate)==XDC_OK)
            {
               return(XDC_OK);
            }            
         }
      }
   }
   return(XDC_NOK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux TIMERS horaire (Toutes les heures)
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ey_traitement_cyclique_etat_ligne_maitre( XZEXT_CLE_PROG pa_CleProg )

/*
* ARGUMENTS EN ENTREE :
*
*	pa_CleProg : Cle d'appel timer demande donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*	XDC_OK : Si la fonction est consernee
*	XDC_NOK : Si la fonction n'est pas consernee
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement de l' evenements suivant:
*	   - Un reveil periodique pour tester la ligne maitre avec les equipements
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $ : ey_traitement_cyclique_etat_ligne_maitre" ;
   int i;
    
   /* Demande periodique de l'etat de la liaison avec l'equipement */
            
   if (strstr(pa_CleProg,CLE_CYCLE_TIMER_QTX_1H)!=NULL)
   { 
      /* Trace du reveil */
               
      XZST_03EcritureTrace( XZSTC_DEBUG1, "%s", pa_CleProg );
  	       
      for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
      {
         if( !(vg_donnees_systeme_qtx.DonneesEqt[i].Etat_Service & XDC_EQT_HORS_SRV)
  	          	  && vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide==Valide
  	          	   && vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide==Valide)
  	 {
            if (EnvoyerTrame("DT",XZEXC_TRAME_STAT_COMP,
                   vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS,XZEXC_NOCMD,
	               XZEXC_PRIORITEFAIBLE,
                           vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre) == XDC_NOK )
            {
               /* *** return */;
            }
         }
      }
      return( XDC_OK);
   }
   return( XDC_NOK);
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux TIMERS horaire (Toutes les heures)
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ey_traitement_cyclique_etat_ligne_esclave( XZEXT_CLE_PROG pa_CleProg,
								 XDY_Horodate va_Horodate)


/*
* ARGUMENTS EN ENTREE :
*
*	pa_CleProg : Cle d'appel timer demande donnees
*	va_Horodate: Horodate du message recu
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*	XDC_OK : Si la fonction est consernee
*	XDC_NOK : Si la fonction n'est pas consernee
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement de l' evenements suivant:
*	   - Un reveil periodique pour tester la ligne esclave avec les equipements
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $ : ey_traitement_cyclique_etat_ligne_esclave" ;
   XZSMT_Horodate vl_Horodatedecompo;
   char vl_format[256];
   int vl_indice_table_eqt;
   int i;
   
   /*A Si il s'agit d'un reveil pour une alarme liaison equipement Esclave */

   if( strstr( pa_CleProg, CLE_CYCLE_TIMER_QTX_LIGNE_ESCLAVE) != NULL)
   {
      /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
               
      XZSM_11ConversionHorodate( va_Horodate, &vl_Horodatedecompo );
                          
      XZST_03EcritureTrace( XZSTC_DEBUG1,
  	       	   "Rupture de liaison avec l'equipement Esclave :%s",vl_Horodatedecompo );
  	       
      /* Recuperation de l'indice de la liaison */
  	       	   
      sprintf( vl_format, "%s%%02d", CLE_CYCLE_TIMER_QTX_LIGNE_ESCLAVE);
      sscanf( pa_CleProg, vl_format, &vl_indice_table_eqt);
               
      /* Si l'equipement n'est pas declare HS */
               
      if(( vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Etat_Service &
								XDC_EQT_HS) != XDC_EQT_HS )
      {
         /* Alors */
         /* Generation de l'alarme */
               
         ex_env_alarme( vg_donnees_systeme_qtx.NomMachine,
          va_Horodate, vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].TypeEqt,
                     vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].Numero,
				vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt]
				      .ptr_alarmes_alertes->Numero_Alarme, XDC_VRAI, NULL,
				      vg_donnees_systeme_qtx.DonneesEqt[vl_indice_table_eqt].SiteGestion);
      }
      /* Finsi */
      /* Relancement du timer */

      lancement_timer_eqtx( EQTXC_REVEIL_LIGNE_ESCLAVE, pa_CleProg);
      return( XDC_OK);
   }
   return( XDC_NOK);
}
   
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux TIMERS horaire (Toutes les heures)
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ey_traitement_cyclique_synchro_date(  int va_NumSock,
				      XZEXT_CLE_PROG pa_CleProg, XDY_Horodate va_Horodate)


/*
* ARGUMENTS EN ENTREE :
*
*	va_NumSock : Numero de la socket
*	pa_CleProg : Cle d'appel timer demande donnees
*	va_Horodate: Horodate du message recu
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*	XDC_OK : Si la fonction est consernee
*	XDC_NOK : Si la fonction n'est pas consernee
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement de l' evenements suivant:
*	   - Un reveil periodique pour tester la ligne esclave avec les equipements
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $ : ey_traitement_cyclique_synchro_date" ;
   XZSMT_Horodate vl_Horodatedecompo;
   XZEXT_Trame		vl_tlcr;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   int i;
   
   /*A Si il s'agit d'un reveil pour synchro de la date */
           
   if( strstr( pa_CleProg, CLE_SYNCHRO_HEURE_QTX ) != NULL)
   {
      /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
               
      XZSM_11ConversionHorodate ( va_Horodate, &vl_Horodatedecompo );
                          
      /* passage de l'annee sur 2 digit */
      memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
      /*B Composition de la trame mise a la date et a l'heure */
      sprintf ( vl_tlcr, "DATE ID=%s %s", vg_idpassw, vl_Horodatedecompo);
             
      XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	       
      for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
      {
         if( !(vg_donnees_systeme_qtx.DonneesEqt[i].Etat_Service & XDC_EQT_HORS_SRV)
  	          	     && vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide==Valide
  	          	      && vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide==Valide)
  	 {
            /*B Envoi trame  date et heure a l'equipement */
             
            if (EnvoyerTrame(vl_tlcr,XZEXC_TRAME_DATE,
                       vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS,XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
	                    vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre) == XDC_NOK )
            {
	       /* *** return */;
	    }
XZST_03EcritureTrace( XZSTC_WARNING, "Mise à l'heure : %s %s",vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS,vl_tlcr);
         }
      }

      /* Rearmement du reveil synchro date */
  	      
      ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, CLE_SYNCHRO_HEURE_QTX,
             							va_NumSock, pl_MsgTimer )
      return( XDC_OK);
   }
   return( XDC_NOK);
}
      
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux TIMERS horaire (Toutes les heures)
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ey_traitement_cyclique_demande_donnees ( int va_NumSock,
				       XZEXT_CLE_PROG pa_CleProg, XDY_Horodate va_Horodate)

/*
* ARGUMENTS EN ENTREE :
*
*	va_NumSock : Numero de la socket
*	pa_CleProg : Cle d'appel timer demande donnees
*	va_Horodate: Horodate du message recu
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*	XDC_OK : Si la fonction est consernee
*	XDC_NOK : Si la fonction n'est pas consernee
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement des evenements suivants:
*	   - Un reveil periodique pour demander les donnees a l'equipement
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cyc.c,v 1.8 2011/02/25 16:37:46 gesconf Exp $ : ey_traitement_cyclique_demande_donnees" ;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   XDY_Horodate vl_Horodate;
   XZSMT_Horodate vl_Horodatedecompo;
   int vl_heure;
   char	vl_chaine_travail[100];
   char *pl_nom_fichier_envoi;
   int i;

   /*A Si il s'agit d'un reveil pour une demande d'envoi de donnees */
            
   if( strstr( pa_CleProg, CLE_DEMANDE_HEURE_X_DONNEES_QTX ) != NULL)
   {
      /* Rearmement du reveil demande donnees */
  	      
      sprintf( vl_chaine_travail, "%s%%02d", CLE_DEMANDE_HEURE_X_DONNEES_QTX);
      sscanf( pa_CleProg , vl_chaine_travail, &vl_heure);
      
      /* Si l'heure recuperee est egale a 99 */
      
      if( vl_heure==99)
      {
         /* Alors */
         /* C'est la premiere demande au lancement de la tache ne plus relancer de timer */
         
      }
      else
      {
         /* Sinon */
         /* C'est le cas normal relancer le timer pour 24 heures */
         
         sprintf( vl_chaine_travail, "%02d%s",
					vl_heure,EQTXC_DEMANDE_HEURE_X_DONNEES_QTX );

         ETIM_SYNCHRO_DATE(  vl_chaine_travail, pa_CleProg, va_NumSock, pl_MsgTimer)
      }
      /* Finsi */

      /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
               
      XZSM_11ConversionHorodate( va_Horodate, &vl_Horodatedecompo );
                          
      XZST_03EcritureTrace( XZSTC_DEBUG1,
  	       			"Envoi demande trame donnees :%s",vl_Horodatedecompo );
  	     
      for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
      {
         if( vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide==Valide &&
		vg_donnees_systeme_qtx.DonneesEqt[i].TypeEqt== XDC_EQT_MET &&
		  !(vg_donnees_systeme_qtx.DonneesEqt[i].Etat_Service & XDC_EQT_HORS_SRV))
  	 {
            /*B Envoi trame demande donnees a l'equipement */
            /* Si il est entre 00:00:00 et 00:59:59 ou si la tache vient d'etre lancee */ 
             
            pl_nom_fichier_envoi=em_retourne_nom_fichier_envoi_qtx(vg_donnees_systeme_qtx.DonneesEqt[i].Numero);
      
            if( ((vl_heure == 0) || (vl_heure==99)) && !access( pl_nom_fichier_envoi, F_OK))
            {
               unlink(pl_nom_fichier_envoi);
            }
            if( access( pl_nom_fichier_envoi, F_OK) )
            {               
               if( EnvoyerTrame( "MJ", XZEXC_TRAME_DONNEES_METEO,
                    vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS,
                       XZEXC_NOCMD, XZEXC_PRIORITEFAIBLE,
	               	 vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre) == XDC_NOK )
               {
                  /* *** return */;
               }
               em_creation_fichier_donnees_meteo_invalides(vg_donnees_systeme_qtx.DonneesEqt[i].Numero);
            }
         }
      }
      em_lecture_repertoire(XZEXC_PATH_FIC_DYN, em_envoi_fichier_vers_Base_Donnees);
      return(XDC_OK);
   }
   return(XDC_NOK);
}
