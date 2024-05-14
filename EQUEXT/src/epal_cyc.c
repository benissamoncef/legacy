/*E*/
/* Fichier : $Id: epal_cyc.c,v 1.4 2009/10/05 15:05:35 gesconf Exp $     Release : $Revision: 1.4 $      Date :  $Date: 2009/10/05 15:05:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEPAL
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	14 Jan 1998	: Creation
* Mismer.D	version 1.2	28 avr 2004	: Ajout pilotage par scEnarios
* PNI		v1.4		09 jul 2009	: correction polling eqt HS DEM897
* GGY    15/06/23 :  Correction ex_mlcr pour pilotage eqt IP (DEM471)
------------------------------------------------------*/

/* fichiers inclus */

#include "epal_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: epal_cyc.c,v 1.4 2009/10/05 15:05:35 gesconf Exp $ : epal_cyc" ;

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
   static char *version = "$Id: epal_cyc.c,v 1.4 2009/10/05 15:05:35 gesconf Exp $ : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EPAL_DONNEES_PAL 	*pl_listePAL;
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
  	  					         
  	     if (strstr(pl_CleProg,"PAL_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"PAL_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListePALFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return;
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, EPALC_TIMER_10S ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pal -> demande de status TR */
                    for ( pl_listePAL = pg_debutPAL; pl_listePAL != NULL; pl_listePAL = pl_listePAL->Suivant )
                    {
                       if ( !(pl_listePAL->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listePAL->EtatTrans == EPALC_EQT_LIBRE) &&
			                   (!strcmp(pl_listePAL->Config.AdresseIP, XZECC_IP_VIDE)) )
                       {
                          EnvoyerTrame(
                                   "PS",
                                   XZEXC_TRAME_ETAT_POS_PAL,
                                   pl_listePAL->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listePAL->Socket);
                          pl_listePAL->EtatTrans = EPALC_EQT_OCCUP;
                       }
					   else if ( !(pl_listePAL->Etat_Service & XDC_EQT_HORS_SRV) &&
							(strcmp(pl_listePAL->Config.AdresseIP, XZECC_IP_VIDE))) {
								EnvoyerTrameIP(       "PS",
										XZEXC_TRAME_ETAT_POS_PAL,
										pl_listePAL->Config.AdresseIP,
										pl_listePAL->Config.Port,
										pl_listePAL->Config.AdresseRGS,
										XZEXC_NOCMD,
										XZEXC_PRIORITEFAIBLE,
										pl_listePAL->Socket);
			}
                    }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EPALC_TIMER_1MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pal -> demande de status complet si status temps reel incorrect */
                    for ( pl_listePAL = pg_debutPAL; pl_listePAL != NULL; pl_listePAL = pl_listePAL->Suivant )
                    {
                       if ( !(pl_listePAL->Etat_Service & XDC_EQT_HORS_SRV) &&
                            ((pl_listePAL->StatusTR == XDC_NOK) || (pl_listePAL->EtatTrans==EPALC_EQT_PANNE)) )
                       {
                          EnvoyerTrame(
				   "ST",
                                   (pl_listePAL->EtatTrans==EPALC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP, 
                                   pl_listePAL->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listePAL->Socket);
                          pl_listePAL->EtatTrans = EPALC_EQT_OCCUP;
                       }
    /*!!! Modif DM Trame PS toutes les 10secondes                   else
                       {
                          EnvoyerTrame(	"PS",
                                   	XZEXC_TRAME_ETAT_POS_PAL,
                                   	pl_listePAL->Config.AdresseRGS,
                                   	XZEXC_NOCMD,
	                           	XZEXC_PRIORITEFAIBLE,
                                   	pl_listePAL->Socket );
                          pl_listePAL->EtatTrans = EPALC_EQT_OCCUP;
                          break;
                       }  */
                    }  
  	     }
  	     
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "PAL_Timer_Synchro" ) != NULL )
	  {
             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
  	     /*B Pour tous les pal -> demande de status TR */
  	            
             for ( pl_listePAL = pg_debutPAL; pl_listePAL != NULL; pl_listePAL = pl_listePAL->Suivant )
             {
                if ( !(pl_listePAL->Etat_Service & XDC_EQT_HORS_SRV) )
                   EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listePAL->Config.AdresseRGS,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listePAL->Socket);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "PAL_Timer_Synchro", va_NumSock, pl_MsgTimer )
  	     
	  }
       }
    }
 
}


