/*E*/
/*Fichier :  $Id: edai_cyc.c,v 1.21 2021/05/04 13:25:02 pc2dpdy Exp $      Release : $Revision: 1.21 $        Date : $Date: 2021/05/04 13:25:02 $ ------------------------------------------------------
-----------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des traitements cycliques de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	20 Oct 1994	: Creation
* Nagiel.E	version 1.2	07 Dec 1994	:
* Nagiel.E	version 1.3	22 Dec 1994	:
* Nagiel.E	version 1.4	05 Jan 1995	:
* Mismer.D	version 1.5	11 Jan 1995	:
* Nagiel.E	version 1.6	16 Jan 1995	:
* Nagiel.E	version 1.7	16 Jan 1995	:
* Mismer.D	version 1.8	17 Mar 1995	:
* Volcic.F	version 1.9	04 Avr 1995	:
* Fontaine.C	version 1.10	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.11	20 Jui 1995	: Ajout traitement eqt occupe
* Volcic.F	version 1.12	31 Oct 1995	: Modif trace 
* Volcic.F	version 1.13	15 Nov 1995	: Ajout mot de passe dans trame mise a l'heure 
* Mismer.D	version 1.14	15 Nov 1995	: Envoi d'un ST en cas de panne liaison serie
* Mismer.D	version 1.15	02 Fev 1996	: Correction trame mise a l'heure 
* Mismer.D	version 1.16	15 Fev 1996	: Modif config voie DAI (DEM/751) 
* Mismer.D	version 1.17 	07 Jan 1997	: Ajout trace config voie sur travaux (DEM/1155)
* Mismer.D	version 1.18 	30 Mai 2000	: Modif pour config avec 2 DAI sur un LT (DEM/102)
* Mismer.D	version 1.19 	30 Mai 2000	: Modif pour config avec 2 DAI sur un LT (DEM/102)
* LCL		version 1.20	14 Nov 2019	: Modif pour envoi IP (DEM1333)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "edai_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: edai_cyc.c,v 1.21 2021/05/04 13:25:02 pc2dpdy Exp $      : edai_cyc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Gestion cyclique 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSock )

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
   static char *version = "$Id: edai_cyc.c,v 1.21 2021/05/04 13:25:02 pc2dpdy Exp $      : ey_traitement_cyclique" ;

   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   EDAI_DONNEES_DAI 	*pl_listeDAI;
   EDAI_DONNEES_ANALYSEUR *pl_listeAnaly;
   XZEXT_Trame		vl_tlcr;
   INFO_EQT_FEN_LCR	*pl_ListeEqt;
   int			vl_noeqt;
   char			vl_Cas;
   XZEXT_Trame          pl_mlcr;
   static int		vl_CptConf = 0;
   
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
  	  					         
             /*A Si c'est un timer FLCR : gestion de celui-ci */
  	     if (strstr(pl_CleProg,"DAI_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"DAI_FLcr_NoEqt:%d",&vl_noeqt);
  	        
  	        sscanf(pl_CleProg,"DAI_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListeDAIFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
  	     }
  	     
  	     switch ( vl_Delai )
  	     {
  	         /*A Si il s'agit d'un reveil cyclique 1Mn */
  	         case EDAIC_REVEIL_1MN :
  	         {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s", pl_CleProg );
  	            if ( vl_CptConf++ > 2 ) vl_CptConf = 0;
  	            /*B Pour tous les dai -> demande de status   */
  	            
                    for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
                    {
         		XZST_03EcritureTrace ( XZSTC_WARNING, "REVEIL Config DAI: %d %s",
				pl_listeDAI->Config.Numero,
				pl_listeDAI->Config.AdresseRGS);
              
                       if ( !(pl_listeDAI->Etat_Service & XDC_EQT_HORS_SRV)/* &&
                             (pl_listeDAI->EtatTrans == EDAIC_EQT_LIBRE)*/ )
                       {
                          sprintf(pl_mlcr," ST ID=%s\0", vg_idpassw);
                          if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
                       {
			  if (pl_listeDAI->EtatTrans == EDAIC_EQT_LIBRE)
                          	EnvoyerTrame(
                                   pl_mlcr+1,
                                   (vl_CptConf < 2) ? XZEXC_TRAME_STAT_COMP : XZEXC_TRAME_CONF_DAI,
                                   pl_listeDAI->Config.AdresseRGS,
                                   (vl_CptConf < 2) ? XZEXC_NOCMD : ++vg_NumConf[pl_listeDAI->Config.Numero],
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeDAI->SocketMaitre);
                             }
                         else
                                EnvoyerTrameIP2(
                                   pl_mlcr,
                                   (vl_CptConf < 2) ? XZEXC_TRAME_STAT_COMP : XZEXC_TRAME_CONF_DAI,
                                   pl_listeDAI->Config.AdresseIP,
                                   pl_listeDAI->Config.Port,
                                   pl_listeDAI->Config.Adresse2IP,
                                   pl_listeDAI->Config.Port2,
                                   pl_listeDAI->Config.DAIBis,
                                   pl_listeDAI->Config.AdresseRGS,
                                   (vl_CptConf < 2) ? XZEXC_NOCMD : ++vg_NumConf[pl_listeDAI->Config.Numero],
	                           XZEXC_PRIORITEFAIBLE,
                                   pl_listeDAI->SocketMaitre);
                                   
                          /*B Pour tous les analyseurs de ce DAI -> demende de mesures */
			  /*
                          for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
                          {
                             if ( !(pl_listeDAI->Etat_Service & XDC_EQT_HORS_SRV) &&
                                  (pl_listeDAI->EtatTrans == EDAIC_EQT_LIBRE ) &&
                                  (pl_listeDAI->Config.Numero == pl_listeAnaly->Config.NumeroDAI) )
                             {
  	                        sprintf ( vl_tlcr, " MIA%02d",pl_listeAnaly->Config.NoAnaly);
                          if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
                                EnvoyerTrame(
                                         vl_tlcr+1,
                                         XZEXC_TRAME_INFO_TRAFIC_DAI,
                                         pl_listeDAI->Config.AdresseRGS,
                                         XZEXC_NOCMD,
	                                 XZEXC_PRIORITEFAIBLE,
                                         pl_listeDAI->SocketMaitre);
			  else
                                EnvoyerTrameIP2(
                                         vl_tlcr,
                                         XZEXC_TRAME_INFO_TRAFIC_DAI,
                                   	 pl_listeDAI->Config.AdresseIP,
	                                 pl_listeDAI->Config.Port,
                                   	 pl_listeDAI->Config.Adresse2IP,
	                                 pl_listeDAI->Config.Port2,
	                                 pl_listeDAI->Config.DAIBis,
                                         pl_listeDAI->Config.AdresseRGS,
                                         XZEXC_NOCMD,
	                                 XZEXC_PRIORITEFAIBLE,
                                         pl_listeDAI->SocketMaitre);

                             }
                          }*/
                          /*pl_listeDAI->EtatTrans = EDAIC_EQT_OCCUP;*/
                       }

                       if ( !(pl_listeDAI->Etat_Service & XDC_EQT_HORS_SRV) &&
                             (pl_listeDAI->EtatTrans == EDAIC_EQT_PANNE)  &&
			     (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)))
                       {
                          sprintf(pl_mlcr," ST ID=%s\0", vg_idpassw);
                          if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	                          EnvoyerTrame(
	                                   pl_mlcr+1,
	                                   XZEXC_TRAME_TEST_COM,
	                                   pl_listeDAI->Config.AdresseRGS,
	                                   XZEXC_NOCMD,
		                           XZEXC_PRIORITEFAIBLE,
	                                   pl_listeDAI->SocketMaitre);
			  else
        	                  EnvoyerTrameIP2(
	                                   pl_mlcr,
	                                   XZEXC_TRAME_TEST_COM,
                                   	   pl_listeDAI->Config.AdresseIP,
  	                                   pl_listeDAI->Config.Port,
					   pl_listeDAI->Config.Adresse2IP,
					   pl_listeDAI->Config.Port2,
					   pl_listeDAI->Config.DAIBis,
	                                   pl_listeDAI->Config.AdresseRGS,
	                                   XZEXC_NOCMD,
		                           XZEXC_PRIORITEFAIBLE,
	                                   pl_listeDAI->SocketMaitre);

                       }
                    }  
  	         }
  	     }
	  }
	  
	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "DAI_Timer_Synchro" ) != NULL )
	  {
             /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
             /* passage de l'annee sur 2 digit */
             memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
             /*B Composition de la trame mise a la date et a l'heure */
  	     sprintf ( vl_tlcr, " DATE ID=%s %s", vg_idpassw, vl_Horodatedecompo);
             
  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
             /*B Envoi trame  date et heure a ts les DAI*/
             for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
             {
                if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
        	        EnvoyerTrame( vl_tlcr+1,
	                              XZEXC_TRAME_DATE,
	                              pl_listeDAI->Config.AdresseRGS,
	                              XZEXC_NOCMD,
		                      XZEXC_PRIORITEFAIBLE,
	                              pl_listeDAI->SocketMaitre ) ;
		else
        	        EnvoyerTrameIP2( vl_tlcr,
					XZEXC_TRAME_DATE,
					pl_listeDAI->Config.AdresseIP,
					pl_listeDAI->Config.Port,
					pl_listeDAI->Config.Adresse2IP,
					pl_listeDAI->Config.Port2,
					pl_listeDAI->Config.DAIBis,
					pl_listeDAI->Config.AdresseRGS,
					XZEXC_NOCMD,
					XZEXC_PRIORITEFAIBLE,
					pl_listeDAI->SocketMaitre ) ;

                 
	     }
  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "DAI_Timer_Synchro", va_NumSock, pl_MsgTimer )
	  }
       }
    }
}


