/*E*/
/*Fichier : $Id: epic_cyc.c,v 1.1 2023/08/18 14:20:37 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2023/08/18 14:20:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epic_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEPICTO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK    	18/08/23		: Creation (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */

#include "epic_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "%W 1.2 03/26/07 : epic_cyc" ;

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
   static char *version = "@(#)epic_cyc.c	1.11 03/10/98 : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EPIC_DONNEES_PIC 	*pl_listepic;
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
  	  					         

  	     if (strstr(pl_CleProg,"PIC_FLcr_NoEqt:")!=NULL)
  	     {
  	        sscanf(pl_CleProg,"PIC_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt, 
                			vl_Cas,
			 		&pg_DebutListePICFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return;
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, EPICC_TIMER_10S ) != NULL )
	     {
            	 XZST_03EcritureTrace( XZSTC_INFO, "traitement du reveil 10s ");

  	            /* Trace du reveil */
            	XZST_03EcritureTrace( XZSTC_WARNING, "%s : %d", pl_CleProg, vl_Delai );
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pmv et les picto -> demande de status TR */
  	            
                   XZST_03EcritureTrace( XZSTC_INFO, "pour tous les picto et pic");
                    for ( pl_listepic = pg_debutPIC; pl_listepic != NULL; pl_listepic = pl_listepic->Suivant )
                    {
                       	    XZST_03EcritureTrace( XZSTC_INFO, "demande de status RGS %s",pl_listepic->Config.AdresseRGS);
                        if ( !(pl_listepic->Etat_Service & XDC_EQT_HORS_SRV) &&
                              (pl_listepic->EtatTrans == EPICC_EQT_LIBRE) )
                        {
                       	    XZST_03EcritureTrace( XZSTC_INFO, "demande de status RGS %s",pl_listepic->Config.AdresseRGS);
                            EnvoyerTrame(  	"P\0",
                                         	XZEXC_TRAME_STAT_TR,
                                         	pl_listepic->Config.AdresseRGS,
                                        	XZEXC_NOCMD,
	                                	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepic->Socket);
                           pl_listepic->EtatTrans = EPICC_EQT_OCCUP;
                        }
                     }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EPICC_TIMER_1MN ) != NULL )
	     {
            	 XZST_03EcritureTrace( XZSTC_INFO, "traitement reveil 1mn");
  	            /* Trace du reveil */
            	 XZST_03EcritureTrace( XZSTC_INFO, "%s : %d", pl_CleProg, vl_Delai );
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	                  
  	            /*A Pour tous les pmv et les picto -> demande de status complet si status temps reel NOK */
  	            /*A Pour tous les pmv et les picto -> demande d'etat d'affichage (PS) */
  	            
                    for ( pl_listepic = pg_debutPIC; pl_listepic != NULL; pl_listepic = pl_listepic->Suivant )
                    {
                    	XZST_03EcritureTrace( XZSTC_INFO, "Pour tous les pic et les picto");
                        /* si il y a une connection locale -> demande d'etat d'affichage */
                        if ( !(pl_listepic->Etat_Service & XDC_EQT_HORS_SRV) && 
                              (pl_listepic->EtatTrans!=EPICC_EQT_PANNE) )
                        {
                        	XZST_03EcritureTrace( XZSTC_INFO, "il y a une connection local");
                           if ( EnvoyerTrame(	"PS\0",
                                         	XZEXC_TRAME_ETAT_AFF_PIC,
                                         	pl_listepic->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepic->Socket) == XDC_OK )
                           {
                               pl_listepic->EtatTrans = EPICC_EQT_OCCUP;
                               XZST_03EcritureTrace( XZSTC_INFO, "demande etat affichage");
                           }
                           else
                           {
                               pl_listepic->EtatTrans = EPICC_EQT_PANNE;
                           }
                        }
                        if ( !(pl_listepic->Etat_Service & XDC_EQT_HORS_SRV) &&
                              ((pl_listepic->StatusTR==XDC_NOK) || (pl_listepic->EtatTrans==EPICC_EQT_PANNE)) )
                        {
                        	XZST_03EcritureTrace( XZSTC_INFO, "statusTR = NOK || etatTrans = EQT en Panne");
	                   if ( EnvoyerTrame(	"ST\0",
	                                 	(pl_listepic->EtatTrans==EPICC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listepic->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepic->Socket) == XDC_OK )
                           {
                               pl_listepic->EtatTrans = EPICC_EQT_OCCUP;
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
	                                 	(pl_listepic->EtatTrans==EPICC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listepic->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepic->Socket);
                     }  
  	       }
  	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "PIC_Timer_Synchro" ) != NULL )
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
  	            
             for ( pl_listepic = pg_debutPIC; pl_listepic != NULL; pl_listepic = pl_listepic->Suivant )
             {
            	 XZST_03EcritureTrace( XZSTC_INFO, "Pour tous les pic et les picto");
             EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listepic->Config.AdresseRGS,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listepic->Socket);
  	     XZST_03EcritureTrace( XZSTC_WARNING, "trame de date:%s a %s", vl_tlcr ,pl_listepic->Config.AdresseRGS);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
		XZST_03EcritureTrace( XZSTC_WARNING, " Rearmement du reveil synchro date " );
		ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "PIC_Timer_Synchro", va_NumSock, pl_MsgTimer )

	  }
       }
    }
   XZST_03EcritureTrace( XZSTC_FONCTION, "ey_traitement_cyclique OUT : " );
}


