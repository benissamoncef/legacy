/*E*/
/*Fichier : $Id: eprv_cyc.c,v 1.1 2012/07/04 14:20:37 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:20:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEPRV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "eprv_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "%W 1.2 03/26/07 : eprv_cyc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*  Numero de la socket appelante 
*
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
*
* CODE RETOUR : 
*  Aucun 
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
   static char *version = "@(#)eprv_cyc.c	1.11 03/10/98 : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EPRV_DONNEES_PRV 	*pl_listeprv;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   char			vl_Cas;

   if (vg_Simulation)
	   	   return;

   XZST_03EcritureTrace( XZSTC_FONCTION, "ey_traitement_cyclique IN : " );
   /*A
    *  Lecture du message recu dans la socket appelante
    */

   XZST_03EcritureTrace( XZSTC_INFO, "Numero Sock : %d", va_NumSock );
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      XZST_03EcritureTrace( XZSTC_INFO, "Lecture message socket OK ");
      /* Si le message est correct et si ce n'est pas une deconnexion */
      if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
      {
    	  XZST_03EcritureTrace( XZSTC_INFO, "message correct et pas de deconnexion ");
  	  /*A Si il s'agit d'un reveil cyclique */
  	  if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	  {
  		XZST_03EcritureTrace( XZSTC_INFO, "Reveil cyclique ");
  	     sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					         &vl_Horodate,
  	  					         &vl_Delai,
  	  					         pl_CleProg );
  	  					         

  	     if (strstr(pl_CleProg,"PRV_FLcr_NoEqt:")!=NULL)
  	     {
  	        sscanf(pl_CleProg,"PRV_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt, 
                			vl_Cas,
			 		&pg_DebutListePRVFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return;
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, EPRVC_TIMER_10S ) != NULL )
	     {
            	 XZST_03EcritureTrace( XZSTC_INFO, "traitement du reveil 10s ");

  	            /* Trace du reveil */
            	XZST_03EcritureTrace( XZSTC_WARNING, "%s : %d", pl_CleProg, vl_Delai );
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pmv et les picto -> demande de status TR */
  	            
                   XZST_03EcritureTrace( XZSTC_INFO, "pour tous les picto et prv");
                    for ( pl_listeprv = pg_debutPRV; pl_listeprv != NULL; pl_listeprv = pl_listeprv->Suivant )
                    {
                       	    XZST_03EcritureTrace( XZSTC_INFO, "demande de status RGS %s",pl_listeprv->Config.AdresseRGS);
                        if ( !(pl_listeprv->Etat_Service & XDC_EQT_HORS_SRV) &&
                              (pl_listeprv->EtatTrans == EPRVC_EQT_LIBRE) )
                        {
                       	    XZST_03EcritureTrace( XZSTC_INFO, "demande de status RGS %s",pl_listeprv->Config.AdresseRGS);
                            EnvoyerTrame(  	"P\0",
                                         	XZEXC_TRAME_STAT_TR,
                                         	pl_listeprv->Config.AdresseRGS,
                                        	XZEXC_NOCMD,
	                                	XZEXC_PRIORITEFAIBLE,
                                         	pl_listeprv->Socket);
                           pl_listeprv->EtatTrans = EPRVC_EQT_OCCUP;
                        }
                     }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EPRVC_TIMER_1MN ) != NULL )
	     {
            	 XZST_03EcritureTrace( XZSTC_INFO, "traitement reveil 1mn");
  	            /* Trace du reveil */
            	 XZST_03EcritureTrace( XZSTC_INFO, "%s : %d", pl_CleProg, vl_Delai );
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	                  
  	            /*A Pour tous les pmv et les picto -> demande de status complet si status temps reel NOK */
  	            /*A Pour tous les pmv et les picto -> demande d'etat d'affichage (PS) */
  	            
                    for ( pl_listeprv = pg_debutPRV; pl_listeprv != NULL; pl_listeprv = pl_listeprv->Suivant )
                    {
                    	XZST_03EcritureTrace( XZSTC_INFO, "Pour tous les prv et les picto");
                        /* si il y a une connection locale -> demande d'etat d'affichage */
                        if ( !(pl_listeprv->Etat_Service & XDC_EQT_HORS_SRV) && 
                              (pl_listeprv->EtatTrans!=EPRVC_EQT_PANNE) )
                        {
                        	XZST_03EcritureTrace( XZSTC_INFO, "il y a une connection local");
                           if ( EnvoyerTrame(	"PS\0",
                                         	XZEXC_TRAME_ETAT_AFF_PRV,
                                         	pl_listeprv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listeprv->Socket) == XDC_OK )
                           {
                               pl_listeprv->EtatTrans = EPRVC_EQT_OCCUP;
                               XZST_03EcritureTrace( XZSTC_INFO, "demande etat affichage");
                           }
                           else
                           {
                               pl_listeprv->EtatTrans = EPRVC_EQT_PANNE;
                           }
                        }
                        if ( !(pl_listeprv->Etat_Service & XDC_EQT_HORS_SRV) &&
                              ((pl_listeprv->StatusTR==XDC_NOK) || (pl_listeprv->EtatTrans==EPRVC_EQT_PANNE)) )
                        {
                        	XZST_03EcritureTrace( XZSTC_INFO, "statusTR = NOK || etatTrans = EQT en Panne");
	                   if ( EnvoyerTrame(	"ST\0",
	                                 	(pl_listeprv->EtatTrans==EPRVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listeprv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listeprv->Socket) == XDC_OK )
                           {
                               pl_listeprv->EtatTrans = EPRVC_EQT_OCCUP;
                               XZST_03EcritureTrace( XZSTC_INFO, "envoyer trame");
                           }
                           else
                           {
                        	   XZST_03EcritureTrace( XZSTC_INFO, "trame non envoyée");
                               ec_init_cnx ();
                           }
                        }
			else
				EnvoyerTrame(	"ST\0",
	                                 	(pl_listeprv->EtatTrans==EPRVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listeprv->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listeprv->Socket);
                     }  
  	       }
  	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "PRV_Timer_Synchro" ) != NULL )
	  {
		  XZST_03EcritureTrace( XZSTC_INFO, "reveil pour synchro de la date" );
             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	   XZST_03EcritureTrace( XZSTC_INFO, "trame de date:%s", vl_tlcr );
  	     
  	     /*B Pour tous les pmv et les picto -> demande de mise a l'heure */
  	            
             for ( pl_listeprv = pg_debutPRV; pl_listeprv != NULL; pl_listeprv = pl_listeprv->Suivant )
             {
            	 XZST_03EcritureTrace( XZSTC_INFO, "Pour tous les prv et les picto");
             EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listeprv->Config.AdresseRGS,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listeprv->Socket);
  	     XZST_03EcritureTrace( XZSTC_WARNING, "trame de date:%s a %s", vl_tlcr ,pl_listeprv->Config.AdresseRGS);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
		XZST_03EcritureTrace( XZSTC_WARNING, " Rearmement du reveil synchro date " );
		ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "PRV_Timer_Synchro", va_NumSock, pl_MsgTimer )

	  }
       }
    }
   XZST_03EcritureTrace( XZSTC_FONCTION, "ey_traitement_cyclique OUT : " );
}


