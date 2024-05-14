/*E*/
/* Fichier : $Id: ebad_cyc.c,v 1.1 2007/03/26 12:00:39 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 12:00:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAD * FICHIER ebad_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
*
------------------------------------------------------ */

/* fichiers inclus */

#include "ebad_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)ebad_cyc.c	1.18 05/31/00      : ebad_cyc" ;

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
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
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
   static char *version = "@(#)ebad_cyc.c	1.18 05/31/00      : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EBAD_DONNEES_BAD 	*pl_listeBAD;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   int			vl_Index = 0;
   char			vl_Cas;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /* Si le message est correct et si ce n'est pas une deconnexion */
      if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
      {
  	  /*A Si il s'agit d'un reveil cyclique */
  	  if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	  {
  	     sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					         &vl_Horodate,
  	  					         &vl_Delai,
  	  					         pl_CleProg );
  	  					         
  	     if (strstr(pl_CleProg,"BAD_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"BAD_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListeBADFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return;
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, EBADC_TIMER_10S ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les nav -> demande de status TR */
                    for ( pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant )
                    {
                       if ( !(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listeBAD->EtatTrans == EBADC_EQT_LIBRE) &&
                            ((pl_listeBAD->Mode[0]==XZEXC_MODE_CONNECTION_LOCALE) ||
                            (*pl_listeBAD->Etat.Scenario != '\0')) )
                       {
                          EnvoyerTrame(
                                   "PS",
                                   XZEXC_TRAME_ETAT_POS_BAD,
                                   pl_listeBAD->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeBAD->Socket);
                          pl_listeBAD->EtatTrans = EBADC_EQT_OCCUP;
                       }
                       
                       if ( !(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listeBAD->EtatTrans == EBADC_EQT_LIBRE) )
                       {
                          EnvoyerTrame(
                                   "P",
                                   XZEXC_TRAME_STAT_TR,
                                   pl_listeBAD->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeBAD->Socket);
                          pl_listeBAD->EtatTrans = EBADC_EQT_OCCUP;
                       }
                    }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EBADC_TIMER_1MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les nav -> demande de status complet si status temps reel incorrect */
                    for ( pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant )
                    {
                       if ( !(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) &&
                            ((pl_listeBAD->StatusTR == XDC_NOK) || (pl_listeBAD->EtatTrans==EBADC_EQT_PANNE)) )
                       {
                          EnvoyerTrame(
				   "ST",
                                   (pl_listeBAD->EtatTrans==EBADC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP, 
                                   pl_listeBAD->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeBAD->Socket);
                          pl_listeBAD->EtatTrans = EBADC_EQT_OCCUP;
                       }
                       else
                       {
                          if ( !(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) &&
                                (*pl_listeBAD->Etat.Scenario == '\0') )
                          {
                             for ( vl_Index = 0;  vl_Index < XZEXC_nb_module_max_BAD; vl_Index++ )
                             {
                                if ( atoi(pl_listeBAD->Etat.Module[vl_Index]) == 2 )
                                {
                          		EnvoyerTrame(	"PS",
                                   			XZEXC_TRAME_ETAT_POS_BAD,
                                   			pl_listeBAD->Config.AdresseRGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeBAD->Socket );
                          		pl_listeBAD->EtatTrans = EBADC_EQT_OCCUP;
                          		break;
                                }
                             }
                          }
                       }
                    }  
  	     }
  	     
  	     /*A Traitement du reveil 5MN */
             if ( strstr ( pl_CleProg, EBADC_TIMER_5MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les nav -> demande de etat nav */
                    for ( pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant )
                    {
                       if ( !(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listeBAD->EtatTrans != EBADC_EQT_PANNE) &&
                            (*pl_listeBAD->Etat.Scenario == '\0') )
                       {
                          EnvoyerTrame(
                                   "PS",
                                   XZEXC_TRAME_ETAT_POS_BAD,
                                   pl_listeBAD->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeBAD->Socket);
                          pl_listeBAD->EtatTrans = EBADC_EQT_OCCUP;
                       }
                    }  
  	     }
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "BAD_Timer_Synchro" ) != NULL )
	  {
             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
  	     /*B Pour tous les nav -> demande de status TR */
  	            
             for ( pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant )
             {
                if ( !(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) )
                   EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listeBAD->Config.AdresseRGS,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listeBAD->Socket);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "BAD_Timer_Synchro", va_NumSock, pl_MsgTimer )
  	     
	  }
       }
    }
 
}


