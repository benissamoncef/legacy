/*E*/
/*Fichier : $Id: egtc_cyc.c,v 1.5 2011/03/17 15:08:17 gesconf Exp $       Release : $Revision: 1.5 $     Date : $Date: 2011/03/17 15:08:17 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module g≈rant la fenetre LCR et les demandes cyclique a l'eqt
* pour la tache TEGTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1 	16 Dec 1994	: Creation
* Nagiel.E	version 1.2	21 Dec 1994	:
* Mismer.D	version 1.3	23 Jan 1995	:
* Fontaine.C	version 1.4	19 Mai 1995	: Reprise entete, historique
*
* JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. pour fonction ey_traitement_cyclique  1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "egtc_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: egtc_cyc.c,v 1.5 2011/03/17 15:08:17 gesconf Exp $ : egtc_cyc";

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
static char *version = "$Id: egtc_cyc.c,v 1.5 2011/03/17 15:08:17 gesconf Exp $ : ey_traitement_cyclique";
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   char			vl_Cas = 0;
   int			vl_cr = XDC_OK;
   
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
  	  					         
	     /* Demande periodique de la fenetre LCR */
  	     if (strstr(pl_CleProg,"GTC_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"GTC_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListeGTCFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
		return (XDC_OK);
  	     }

  	           switch ( vl_Delai )
  	           {
  	               /*A Traitement du reveil 20S */
  	               case EGTCC_REVEIL_20S	 :
  	               {
  	                  /* Trace du reveil */
  	                  XZST_03EcritureTrace( XZSTC_DEBUG1, "%s", pl_CleProg );
  	            
  	                  /*B Pour la miniGTC -> Lecture des Entrees TOR */
  	            
                          if (EnvoyerTrame(
                                         "M AM=1\0",
                                         XZEXC_TRAME_ENTREES_TOR,
                                         vg_donneesGTC.AdresseRGS,
                                         XZEXC_NOCMD,
	                                 XZEXC_PRIORITEFAIBLE,
                                         vg_donneesGTC.Socket) == XDC_NOK )
                          {
                             /*return*/
                             vl_cr = XDC_NOK;
                          }
  	            
                          if (EnvoyerTrame(
                                         "M AM=2\0",
                                         XZEXC_TRAME_ENTREES_TOR,
                                         vg_donneesGTC.AdresseRGS,
                                         XZEXC_NOCMD,
	                                 XZEXC_PRIORITEFAIBLE,
                                         vg_donneesGTC.Socket) == XDC_NOK )
                          {
                             /*return*/
                             vl_cr = XDC_NOK;
                          }
  	            
  	                  break;
  	               }
	            }
	  }
       }
   }

   return (vl_cr);
}
