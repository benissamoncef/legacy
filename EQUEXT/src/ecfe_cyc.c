/*E*/
/* Fichier : $Id: ecfe_cyc.c,v 1.1 2019/01/29 11:07:54 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2019/01/29 11:07:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TECFE
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
------------------------------------------------------*/

/* fichiers inclus */

#include "ecfe_cyc.h"
#include "ex_mdia.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ecfe_cyc.c,v 1.1 2019/01/29 11:07:54 pc2dpdy Exp $ : ecfe_cyc" ;

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
   static char *version = "$Id: ecfe_cyc.c,v 1.1 2019/01/29 11:07:54 pc2dpdy Exp $ : ey_traitement_cyclique" ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   ECFE_DONNEES_CFE 	*pl_listeCFE;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   int			vl_Index = 0;
   char			vl_Cas;
  	int 	vl_jour,vl_mois,vl_annee,vl_heure,vl_min,vl_sec; 
	int vl_JourSem;
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
  	  					         
  	     /*A Traitement du reveil 9 secondes */
             if ( strstr ( pl_CleProg, ECFEC_TIMER_10S ) != NULL )
	     {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG2, "%s : %d", pl_CleProg, vl_Delai );
  	            
  	            /*B Pour tous les pal -> demande de status complet si status temps reel incorrect */
                    for ( pl_listeCFE = pg_debutCFE; pl_listeCFE != NULL; pl_listeCFE = pl_listeCFE->Suivant )
                    {
		       /*
                       if ( !(pl_listeCFE->Etat_Service & XDC_EQT_HORS_SRV) &&
                            ((pl_listeCFE->StatusTR == XDC_NOK) || (pl_listeCFE->EtatTrans==ECFEC_EQT_PANNE)) )
                       {*/
                          EnvoyerTrameDiaser(
				   "DI",
                                   XZEXC_TRAME_STAT_COMP, 
                                   pl_listeCFE->Config.AdresseIP,
                                   pl_listeCFE->Config.Port,
                                   XZEXC_NOCMD,
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeCFE->Socket);
                          pl_listeCFE->EtatTrans = ECFEC_EQT_OCCUP;
                      /* }*/
                    }  
  	     }
  	     
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "CFE_Timer_Synchro" ) != NULL )
	  {
	     XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate );
	     XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
	     /* *yaammqqhhmmssj */
	     memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);

	     sscanf(vl_Horodatedecompo,"%2d/%2d/%2d %2d:%2ds:%2d",
	     		&vl_jour,&vl_mois,&vl_annee, &vl_heure,&vl_min, &vl_sec);
	     sprintf(vl_tlcr,"*y%02X%X%02X%02X%02X%02X%d",
	     		vl_annee,vl_mois,vl_jour,vl_heure,vl_min,vl_sec,vl_JourSem);
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date :%s",vl_tlcr );
  	     
             for ( pl_listeCFE = pg_debutCFE; pl_listeCFE != NULL; pl_listeCFE = pl_listeCFE->Suivant )
             {
                if ( !(pl_listeCFE->Etat_Service & XDC_EQT_HORS_SRV) )
                   EnvoyerTrameDiaser( vl_tlcr,
                           XZEXC_TRAME_DATE,
                           pl_listeCFE->Config.AdresseIP,
                           pl_listeCFE->Config.Port,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listeCFE->Socket);
             }  
  	            
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "CFE_Timer_Synchro", va_NumSock, pl_MsgTimer )
  	     
	  }

	  if (strstr(pl_CleProg, "CFE_reveil_FMC")  != NULL )
	  {
		/*appel proc reveil sur FMC avec action en cours*/
		XZST_03EcritureTrace( XZSTC_WARNING,"XZAC902");
		XZAC902_ReveilFMC();	    
	  }
       }
    }
 
}


