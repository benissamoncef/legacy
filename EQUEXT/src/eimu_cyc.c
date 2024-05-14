/*E*/
/* Fichier : $Id: eimu_cyc.c,v 1.2 2021/05/03 13:42:39 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2021/05/03 13:42:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEIMU
------------------------------------------------------
* HISTORIQUE :
*
* JMG		04/11/2020	: Creation 1.1
* ABE		16/02/2021	: Modification Envoyer trame OPC DEM-SAE155 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "eimu_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eimu_cyc.c,v 1.2 2021/05/03 13:42:39 pc2dpdy Exp $ : eimu_cyc" ;

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
   static char *version = "$Id: eimu_cyc.c,v 1.2 2021/05/03 13:42:39 pc2dpdy Exp $ : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EIMU_DONNEES_IMU 	*pl_listeIMU;
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
  	  					         
  	     /*A Traitement du reveil 1MN */
             if ( strstr ( pl_CleProg, EIMUC_TIMER_1MN ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pal -> demande de status complet si status temps reel incorrect */
                    for ( pl_listeIMU = pg_debutIMU; pl_listeIMU != NULL; pl_listeIMU = pl_listeIMU->Suivant )
                    {
                       if ( !(pl_listeIMU->Etat_Service & XDC_EQT_HORS_SRV))
                       {
                          EnvoyerTrameOPC(
				   EIMUC_TAG_MESSAGE_EN_COURS,
				   XZEXC_TRAME_DEMANDE_ETAT_IMU,
                                   pl_listeIMU->Config.Numero,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeIMU->Socket);
                       }
                    }  
  	     }
  	     
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "IMU_Timer_Synchro" ) != NULL )
	  {
             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
  	     /*B Pour tous les pal -> demande de status TR */
  	            
             for ( pl_listeIMU = pg_debutIMU; pl_listeIMU != NULL; pl_listeIMU = pl_listeIMU->Suivant )
             {
                if ( !(pl_listeIMU->Etat_Service & XDC_EQT_HORS_SRV) )
                   EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listeIMU->Config.EndPointOPCUA,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listeIMU->Socket);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "IMU_Timer_Synchro", va_NumSock, pl_MsgTimer )
  	     
	  }
       }
    }
 
}


