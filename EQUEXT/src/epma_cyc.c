/*E*/
/*Fichier : $Id: epma_cyc.c,v 1.7 2020/11/03 18:12:18 pc2dpdy Exp $  Release : $Revision: 1.7 $      Date : $Date: 2020/11/03 18:12:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEPMVA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05 : creation  1.1
* ???	xx/03/07 : Ajout envoi trame ...  1.2
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : retour de C.R. OK ou NOK (ey_traitement_cyclique)  1.3
* JMG	11/11/12 : TDP
* JMG	03/10/18 : IP suppression SAGA DEM1306 1.5
* JMG	15/03/19 : gestion ST 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "epma_cfg.h"
#include "epma_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "%W $Revision: 1.7 $ $Date: 2020/11/03 18:12:18 $ : epma_cyc" ;

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
   static char *version = "@(#)epma_cyc.c	1.11 03/10/98 : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EPMVA_DONNEES_PMVA 	*pl_listepma;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   char			vl_Cas;
   int			vl_cr = XDC_OK;
  char         vl_pwd[50];

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
      if ( vg_Simulation )
	return(XDC_OK);

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
  	  					         

  	     if (strstr(pl_CleProg,"PMVA_FLcr_NoEqt:")!=NULL)
  	     {
  	        sscanf(pl_CleProg,"PMVA_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt, 
                			vl_Cas,
			 		&pg_DebutListePMVAFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return (XDC_OK);
  	        
  	     }

  	     /*A Traitement du reveil 10S */
             if ( strstr ( pl_CleProg, EPMVAC_TIMER_10S ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pmv et les picto -> demande de status TR */
  	            
                    for ( pl_listepma = pg_debutPMVA; pl_listepma != NULL; pl_listepma = pl_listepma->Suivant )
                    {
                        if ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) &&
                              (pl_listepma->EtatTrans == EPMVAC_EQT_LIBRE)  && 
			       (!strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)))
                        {
                            EnvoyerTrame(  pl_listepma->Config.Type==EPMVAC_TYPE_ST?"ST\0":"P\0",
                                         	XZEXC_TRAME_STAT_TR,
                                         	pl_listepma->Config.AdresseRGS,
                                        	XZEXC_NOCMD,
	                                	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepma->Socket);
                           pl_listepma->EtatTrans = EPMVAC_EQT_OCCUP;
                        }
			else if  ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) &&
				 (strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)))
			{
			   XZST_03EcritureTrace( XZSTC_FONCTION,"cas -1");
                            	EnvoyerTrameIP(  pl_listepma->Config.Type==EPMVAC_TYPE_ST?"ST\0":"P\0",
					XZEXC_TRAME_STAT_TR,
					pl_listepma->Config.AdresseIP,
					pl_listepma->Config.Port,
					pl_listepma->Config.AdresseRGS,
					XZEXC_NOCMD,
					XZEXC_PRIORITEFAIBLE,
					pl_listepma->Socket);
			}	
                     }  
  	     }
  	     
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EPMVAC_TIMER_1MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	                  
  	            /*A Pour tous les pmv et les picto -> demande de status complet si status temps reel NOK */
  	            /*A Pour tous les pmv et les picto -> demande d'etat d'affichage (PS) */
  	            
                    for ( pl_listepma = pg_debutPMVA; pl_listepma != NULL; pl_listepma = pl_listepma->Suivant )
                    {
                        /* si il y a une connection locale -> demande d'etat d'affichage */
                        if ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) && 
                              (pl_listepma->EtatTrans!=EPMVAC_EQT_PANNE)  &&
			       (!strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)))
                        {
                           if ( EnvoyerTrame(	"PS\0",
                                         	XZEXC_TRAME_ETAT_AFF_PMVA,
                                         	pl_listepma->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepma->Socket) == XDC_OK )
                           {
                               pl_listepma->EtatTrans = EPMVAC_EQT_OCCUP;
                           }
                           else
                           {
                               pl_listepma->EtatTrans = EPMVAC_EQT_PANNE;
                           }
                        }
                        else if ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) && 
                              (pl_listepma->EtatTrans!=EPMVAC_EQT_PANNE)  &&
			       (strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)))
                        {
			   XZST_03EcritureTrace( XZSTC_FONCTION,"cas 0");
                           if ( EnvoyerTrameIP(	"PS\0",
                                         	XZEXC_TRAME_ETAT_AFF_PMVA,
						pl_listepma->Config.AdresseIP,
						pl_listepma->Config.Port,
                                         	pl_listepma->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepma->Socket) == XDC_OK )
                           {
                               pl_listepma->EtatTrans = EPMVAC_EQT_OCCUP;
                           }
                           else
                           {
                               pl_listepma->EtatTrans = EPMVAC_EQT_PANNE;
                           }
                        }
                        if ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) &&
                              ((pl_listepma->StatusTR==XDC_NOK) || (pl_listepma->EtatTrans==EPMVAC_EQT_PANNE)) &&
			       (!strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)))
                        {
	                   if ( EnvoyerTrame(	"ST\0",
	                                 	(pl_listepma->EtatTrans==EPMVAC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listepma->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepma->Socket) == XDC_OK )
                           {
                               pl_listepma->EtatTrans = EPMVAC_EQT_OCCUP;
                           }
                           else
                           {
			   XZST_03EcritureTrace( XZSTC_FONCTION,"cas 2");
                               ec_init_cnx ();
                           }
                        }
			
                        if ( !(pl_listepma->Etat_Service & XDC_EQT_HORS_SRV) &&
                              ((pl_listepma->StatusTR==XDC_NOK) || (pl_listepma->EtatTrans==EPMVAC_EQT_PANNE)) &&
			       (strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)))
                        {
			   XZST_03EcritureTrace( XZSTC_FONCTION,"cas 1");
	                   if ( EnvoyerTrameIP(	"ST\0",
	                                 	(pl_listepma->EtatTrans==EPMVAC_EQT_PANNE) ? XZEXC_TRAME_TEST_COM : XZEXC_TRAME_STAT_COMP,
                                         	pl_listepma->Config.AdresseIP,
                                         	pl_listepma->Config.Port,
                                         	pl_listepma->Config.AdresseRGS,
                                         	XZEXC_NOCMD,
	                                 	XZEXC_PRIORITEFAIBLE,
                                         	pl_listepma->Socket) == XDC_OK )
                           {
                               pl_listepma->EtatTrans = EPMVAC_EQT_OCCUP;
                           }
                           else
                           {
                               ec_init_cnx ();
                           }
                        }
                     }  
  	       }
  	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "PMVA_Timer_Synchro" ) != NULL )
	  {
             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
  	     /*B Pour tous les pmv et les picto -> demande de mise a l'heure */
  	            
             for ( pl_listepma = pg_debutPMVA; pl_listepma != NULL; pl_listepma = pl_listepma->Suivant )
             {
				if (strcmp(pl_listepma->Config.AdresseIP, XZECC_IP_VIDE)) {
  	     				sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             				EnvoyerTrame( vl_tlcr,
                           				XZEXC_TRAME_DATE,
                           				pl_listepma->Config.AdresseRGS,
                           				XZEXC_NOCMD,
	                   				XZEXC_PRIORITEFAIBLE,
                           				pl_listepma->Socket);
				}
	     			else {
  	     				sprintf ( vl_tlcr, "DATE %s %s",vl_Horodatedecompo,
								pl_listepma->Config.IdLCR);
             				EnvoyerTrameIP( vl_tlcr,
                           				XZEXC_TRAME_DATE,
							pl_listepma->Config.AdresseIP,
							pl_listepma->Config.Port,
                           				pl_listepma->Config.AdresseRGS,
                           				XZEXC_NOCMD,
	                   				XZEXC_PRIORITEFAIBLE,
                           				pl_listepma->Socket);
	     			}
  	     XZST_03EcritureTrace( XZSTC_WARNING, "trame de date:%s a %s", vl_tlcr ,pl_listepma->Config.AdresseRGS);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
		XZST_03EcritureTrace( XZSTC_WARNING, " Rearmement du reveil synchro date " );
		ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "PMVA_Timer_Synchro", va_NumSock, pl_MsgTimer )

	  }
       }
    }

    return (vl_cr);
}
