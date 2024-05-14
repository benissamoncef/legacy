/*E*/
/*Fichier :  $Id: enav_cyc.c,v 1.18 2000/05/31 09:29:07 gesconf Exp $      Release : $Revision: 1.18 $        Date : $Date: 2000/05/31 09:29:07 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TENAV
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	21 Nov 1994	: Creation
* Nagiel.E	version 1.2	21 Dec 1994	:
* Mismer.D	version 1.3	23 Jan 1995	:
* Mismer.D	version 1.4	09 Fev 1995	:
* Mismer.D	version 1.5	21 Fev 1995	:
* Mismer.D	version 1.6	08 Mar 1995	:
* Mismer.D	version 1.7	09 Mar 1995	:
* Mismer.D	version 1.8	09 Mar 1995	:
* Fontaine.C	version 1.9	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.10	02 Oct 1995	: Correction condition envoi ST pour eqpt HS
* Volcic.F	version 1.11	12 Oct 1995	: Ajout timer cyclique ST (5 min)
* Volcic.F	version 1.12	23 Nov 1995	: Modif timer cyclique ST (5 min)
* Volcic.F	version 1.13	19 Dec 1995	: Modif envoi ST (5 min)
* Mismer.D	version 1.14	06 Fev 1996	: Modif PS toutes les 15 minutes
* Mismer.D	version 1.15	08 Fev 1996	: Ajout PS toutes les minutes si au moins 1 module en NK
* Mismer.D	version 1.16	30 Mai 1996	: Polling PS et ST toutes le 10" sur cnx local (cad TRM!=0) (DEM/1136)
* Mismer.D	version 1.17	11 Sep 1996	: Ajout periode variable en fonction NB eqt (RADT_W)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "enav_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: enav_cyc.c,v 1.18 2000/05/31 09:29:07 gesconf Exp $      : enav_cyc" ;

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
   static char *version = "$Id: enav_cyc.c,v 1.18 2000/05/31 09:29:07 gesconf Exp $      : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   ENAV_DONNEES_NAV 	*pl_listeNAV;
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
  	  					         
  	     if (strstr(pl_CleProg,"NAV_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"NAV_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListeNAVFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return;
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, ENAVC_TIMER_10S ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les nav -> demande de status TR */
                    for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
                    {
                       if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listeNAV->EtatTrans == ENAVC_EQT_LIBRE) &&
                            ((pl_listeNAV->Mode[0]==XZEXC_MODE_CONNECTION_LOCALE) ||
                            (*pl_listeNAV->Etat.Scenario != '\0')) )
                       {
                          EnvoyerTrame(
                                   "PS",
                                   XZEXC_TRAME_ETAT_POS_NAV,
                                   pl_listeNAV->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeNAV->Socket);
                          pl_listeNAV->EtatTrans = ENAVC_EQT_OCCUP;
                       }
                       
                       if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listeNAV->EtatTrans == ENAVC_EQT_LIBRE) )
                       {
                          EnvoyerTrame(
                                   "P",
                                   XZEXC_TRAME_STAT_TR,
                                   pl_listeNAV->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeNAV->Socket);
                          pl_listeNAV->EtatTrans = ENAVC_EQT_OCCUP;
                       }
                    }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, ENAVC_TIMER_1MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les nav -> demande de status complet si status temps reel incorrect */
                    for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
                    {
                       if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) &&
                            ((pl_listeNAV->StatusTR == XDC_NOK) || (pl_listeNAV->EtatTrans==ENAVC_EQT_PANNE)) )
                       {
                          EnvoyerTrame(
				   "ST",
                                   (pl_listeNAV->EtatTrans==ENAVC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP, 
                                   pl_listeNAV->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeNAV->Socket);
                          pl_listeNAV->EtatTrans = ENAVC_EQT_OCCUP;
                       }
                       else
                       {
                          if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) &&
                                (*pl_listeNAV->Etat.Scenario == '\0') )
                          {
                             for ( vl_Index = 0;  vl_Index < XZEXC_nb_module_max; vl_Index++ )
                             {
                                if ( atoi(pl_listeNAV->Etat.Module[vl_Index]) == 2 )
                                {
                          		EnvoyerTrame(	"PS",
                                   			XZEXC_TRAME_ETAT_POS_NAV,
                                   			pl_listeNAV->Config.AdresseRGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeNAV->Socket );
                          		pl_listeNAV->EtatTrans = ENAVC_EQT_OCCUP;
                          		break;
                                }
                             }
                          }
                       }
                    }  
  	     }
  	     
  	     /*A Traitement du reveil 5MN */
             if ( strstr ( pl_CleProg, ENAVC_TIMER_5MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les nav -> demande de etat nav */
                    for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
                    {
                       if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) &&
                            (pl_listeNAV->EtatTrans != ENAVC_EQT_PANNE) &&
                            (*pl_listeNAV->Etat.Scenario == '\0') )
                       {
                          EnvoyerTrame(
                                   "PS",
                                   XZEXC_TRAME_ETAT_POS_NAV,
                                   pl_listeNAV->Config.AdresseRGS,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeNAV->Socket);
                          pl_listeNAV->EtatTrans = ENAVC_EQT_OCCUP;
                       }
                    }  
  	     }
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "NAV_Timer_Synchro" ) != NULL )
	  {
             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
  	     /*B Pour tous les nav -> demande de status TR */
  	            
             for ( pl_listeNAV = pg_debutNAV; pl_listeNAV != NULL; pl_listeNAV = pl_listeNAV->Suivant )
             {
                if ( !(pl_listeNAV->Etat_Service & XDC_EQT_HORS_SRV) )
                   EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listeNAV->Config.AdresseRGS,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listeNAV->Socket);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "NAV_Timer_Synchro", va_NumSock, pl_MsgTimer )
  	     
	  }
       }
    }
 
}


