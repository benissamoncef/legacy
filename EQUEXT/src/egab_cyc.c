/*E*/
/*Fichier :  $Id: egab_cyc.c,v 1.2 2018/10/26 15:53:23 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 15:53:23 $ ------------------------------------------------------
-----------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER egab_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des traitements cycliques de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/05/18 : creation 1.1
* JMG	18/09/18 : ST sans ID 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "egab_cyc.h"
#include "egab_cfg.h"
#include "ex_mlcr.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */
extern T_Liste_CAB *pg_CAB;

/* declaration de variables locales */

static char *version = "$Id: egab_cyc.c,v 1.2 2018/10/26 15:53:23 pc2dpdy Exp $      : egab_cyc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Gestion cyclique 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique_gab ( int va_NumSock )

/*
* ARGUMENTS EN ENTREE :
* va_NumSock : Soket de tetim
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* DeclenchÅe a l'arrive d'un timer
*
* FONCTION 
*
* gere la fenetre LCR et l'envoi de trame cyclique
------------------------------------------------------*/
{
   static char *version = "$Id: egab_cyc.c,v 1.2 2018/10/26 15:53:23 pc2dpdy Exp $      : ey_traitement_cyclique" ;

   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   T_Liste_CAB 	*pl_liste;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   char			vl_Cas;
   XZEXT_Trame          pl_mlcr;
   static int		vl_CptConf = 0;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */

XZST_03EcritureTrace( XZSTC_WARNING, "ey_traitement_cyclique");

   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket. " );

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
  	  					         
  	     switch ( vl_Delai )
  	     {
  	         /*A Si il s'agit d'un reveil cyclique 1Mn */
  	         case EGABC_REVEIL_1MN :
  	         {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "EGABC_REVEIL_1MN : pl_CleProg %s", pl_CleProg );
  	            if ( vl_CptConf++ > 2 ) vl_CptConf = 0;
  	            /*B Pour tous les cap -> demande de status   */
  	            
                    for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
                    {
                       XZST_03EcritureTrace( XZSTC_DEBUG1, "Parcours liste CAP : %s", pl_liste->cab.config.AdresseIP);
                       if (!(pl_liste->cab.Etat_Service & XDC_EQT_HORS_SRV) )
                       {
		          XZST_03EcritureTrace( XZSTC_DEBUG1, "Envoi ST sur ports maitre et esclave");
                          /*sprintf(pl_mlcr,"ST ID=%s\0", vg_idpassw);*/
                          sprintf(pl_mlcr,"ID\0");
                          EnvoyerTrameIP(
                                   pl_mlcr,
                                   XZEXC_TRAME_STAT_COMP,
				   pl_liste->cab.config.AdresseIP,
				   pl_liste->cab.config.PortEsclave,
                                   pl_liste->cab.config.AdresseRgs,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_liste->cab.Socket);
                          sprintf(pl_mlcr,"ST \0");
                          EnvoyerTrameIP(
                                   pl_mlcr,
                                   XZEXC_TRAME_STAT_COMP,
				   pl_liste->cab.config.AdresseIP,
				   pl_liste->cab.config.PortMaitre,
                                   pl_liste->cab.config.AdresseRgs,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_liste->cab.Socket);
                      }               
		      else
		          XZST_03EcritureTrace( XZSTC_DEBUG1, "Equipement indisponible, pas envoi ST");

                    }  
  	         }
  	     }
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "GAB_Timer_Synchro" ) != NULL )
	  {
             /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s", vg_idpassw, vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
             /*B Envoi trame  date et heure a ts les DAI*/
             for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
             {
                EnvoyerTrameIP( vl_tlcr,
                              XZEXC_TRAME_DATE,
			      pl_liste->cab.config.AdresseIP,
			      pl_liste->cab.config.PortMaitre,
                              pl_liste->cab.config.AdresseRgs,
                              XZEXC_NOCMD,
	                      XZEXC_PRIORITEFAIBLE,
                              pl_liste->cab.Socket ) ;
                 
	     }
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "GAB_Timer_Synchro", va_NumSock, pl_MsgTimer )
	  }
       }
    }
}


