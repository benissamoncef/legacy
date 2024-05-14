/*E*/
/*Fichier : $Id: egtc_tra.c,v 1.19 2011/03/17 15:27:47 gesconf Exp $       Release : $Revision: 1.19 $     Date : $Date: 2011/03/17 15:27:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module traitant les trames en provenance de l'eqt GTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	19 Dec 1994	: Creation
* Nagiel.E	version 1.2	22 Dec 1994	:
* Nagiel.E	version 1.3	23 Dec 1994	:
* Nagiel.E	version 1.4	11 Jan 1995	:
* Mismer.D	version 1.5	23 Jan 1995	:
* Mismer.D	version 1.6	27 Jan 1995	:
* Mismer.D	version 1.7	27 Jan 1995	: RAS
* Mismer.D	version 1.8	09 Fev 1995	:
* Mismer.D	version 1.9	09 Avr 1995	: 
* Volcic.F	version 1.10	09 Avr 1995	: Modif XDC_JOUR XDC_NUIT 
* Fontaine.C	version 1.11	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.12	07 Aou 1995	: Correction paramentre env_alerte
* Mismer.D	version 1.13	08 Aou 1995	: Correction condition envoi alerte et alarme
* Mismer.D	version 1.14	10 Oct 1995	: Ajout etat service pour les ETOR
* Mismer.D	version 1.15	06 Fev 1996	: Correction datagroup emission etat GTC (XDG_ETAT_GTC_<NomMachine> au lieu de NomSite
* Mismer.D	version 1.16	15 Nov 1996	: Modif Alarme (DEM/1232)
* Mismer.D	version 1.17	10 Fev 1997	: Modif TDP (decodage status )(DEM/1395)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	17/03/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. pour fonction em_traitement_trame  1.19
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* JPL	15/11/22 : Plus d'envoi d'alerte de passage Jour / Nuit  SAE-429
------------------------------------------------------*/

/* fichiers inclus */

#include "egtc_tra.h"

#include "ex_mala.h"
#include "ex_msta.h"

/* definitions de constantes */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Analyse de la trame >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket : no socket sur laquelle le message est arriv�
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*  analyse la trame et execute les actions correspondantes
*
------------------------------------------------------*/
{
   static char *version = "$Id: egtc_tra.c,v 1.19 2011/03/17 15:27:47 gesconf Exp $ :em_traitement_trame " ;

   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame,*pl_trame2;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   int				vl_typetrame;
   int 				vl_typ_rep;
   XZEXT_MSG_SOCKET		vl_Msg;
   int				vl_param;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   XDY_Datagroup		pl_DG,pl_NomDG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   char * 			vl_status[32];
   XDY_Mot	                vl_noeqt;
   int 				vl_valret,vl_retour;
   int 				vl_typeeqt;
   XZEXT_TEXTE_STATUS		vl_tabstatus[11];
   int				vl_nomot,vl_mot,vl_ajout,vl_masque,vl_etat,i;
   char				vl_stat[2];
   char   			vl_NbErr[10] = "";
   int				vl_cr = XDC_OK;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {
      XZST_03EcritureTrace( XZSTC_INFO, " -------------tra mes=%s",vl_Msg);
      /*A Scan du message provenant de la socket */
      vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
      					   	    	     vl_entete2,
      					   	             vl_horod1,
      					   	             vl_horod2,
      					   	    	     &vl_nbnonrep,
      					  	    	     &vl_nberrcrc,
      					  	    	     &vl_nocmd,
      					   	    	     vl_adrrgs,
      					  	             &vl_typetrame,
      					  	             &vl_typ_rep);
      if (vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return (XDC_NOK);
      }
      					 
      /*A Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      if (!strcmp(vg_donneesGTC.AdresseRGS,vl_adrrgs))
      {
         /* Sauvegarde donnees pmv */
         vl_typeeqt = XDC_EQT_GTC;
         vl_noeqt   = vg_donneesGTC.Numero;
      }
      else
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Gtc Non trouve en memoire mes=%s",vl_Msg);
         return (XDC_NOK);
      }

      /*A On se positionne sur le debut de la trame */
      pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;
      if (pl_trame == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return (XDC_NOK);
      }
	 
      /*A Traitement des alarmes transmission avec l'equipement */
      if (ex_traite_alarme_com ( 	vg_NomMachine,
					vl_typeeqt,
					vl_noeqt,
      					vl_typ_rep,
      					vl_nbnonrep,
      					vl_nberrcrc,
      					vl_typetrame,
      					vg_Mode_Fonct,
					XZSC_22NumeroSiteLocal()) == XDC_NOK )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Erreur trame non transmise ou non comprise ");
         return (XDC_NOK);
      }

      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {
         /*A C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
         {
            /* Appel de ex_mrcr */
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeGTCFLcr, vl_typ_rep );
            break;
         } 
         /*A C'est une trame de reponse entree TOR */
         case XZEXC_TRAME_ENTREES_TOR:
         {  
            pl_trame2=strstr(pl_trame,"M AM=");
            if (pl_trame2 == NULL) 
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Trame entree TOR incomprise mes=%s",vl_Msg);
               return (XDC_NOK);
            }
            
            /* Scanning de la trame */
            vl_param = sscanf( pl_trame2, "M AM=%d AF=%04x%s", &vl_nomot, &vl_mot, vl_stat);
            
            if (vl_param != 3) 
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Trame entree TOR incomprise mes=%s",vl_Msg);
               return (XDC_NOK);
            }
            
            /* Si le status temps reel ko -> Denmande de status complet */
            if (strncmp(vl_stat,"@",1))
            {
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vg_donneesGTC.AdresseRGS,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    vg_donneesGTC.Socket);
            }
 
            /* On se positionne sur la deuxieme partie des TOR  (de 16 a 31) */
            vl_ajout = (vl_nomot == 2) ? XDC_NB_ETOR : 0;
            
            /* Boucle "d'evolution" du masque */
            for( i = 0, vl_masque = 1; i < XDC_NB_ETOR ; vl_masque = vl_masque << 1 , i++ )
            {
               /* Detection de l'etat */
               vl_etat = ( (vl_mot & vl_masque) == vl_masque ) ? 1 : 0;
               
               /* Si c'est la Cellule JourNuit -> Envoi de son etat */ 
               if ( (vg_donneesGTC.ETOR[i+vl_ajout].Alerte == XZAAC_TOR_0) &&
                    (vl_etat != vg_donneesGTC.ETOR[i+vl_ajout].Etat) &&
                    !(vg_donneesGTC.Etat_Service) &&
                    !(vg_donneesGTC.ETOR[i+vl_ajout].Etat_Service) )
               {
                  XDG_EncodeDG2 ( pl_NomDG, XDG_ETAT_JOURNUIT, vg_NomSite) ;			
     	          if (!TipcSrvMsgWrite ( pl_NomDG,
      			           XDM_IdentMsg(XDM_ETAT_JourNuit),
                          	   XDM_FLG_SRVMSGWRITE,
                          	   T_IPC_FT_INT2, (vl_etat==1) ? XDC_NUIT : XDC_JOUR,
                          	   NULL))
      	          {
         	     XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_JourNuit non effectue." );
	          }
	          else
      	          {
                     XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_JourNuit effectue." );
	          }
// SAE-429  Plus d'envoi d'alerte de passage Jour / Nuit
//                  ex_env_alerte  ( vl_horodate,
//                                   XDC_EQT_TOR,
//                                   vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor,
//                                   vg_donneesGTC.ETOR[i+vl_ajout].Alerte,
//                                   vg_Mode_Fonct,
//				   XZSC_22NumeroSiteLocal(),
//                                   (vl_etat==1) ? "NUIT" : "JOUR",
//                                   NULL);
               }
               else
               {
                  /* Si etat reel et etat precedent <> alors envoi du changement d'etat */
                  if ( (vl_etat != vg_donneesGTC.ETOR[i+vl_ajout].Etat) &&
                       (vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor != 0) &&
                       !(vg_donneesGTC.Etat_Service) &&
                       !(vg_donneesGTC.ETOR[i+vl_ajout].Etat_Service) )
                  {
                     XDG_EncodeDG2 ( pl_NomDG, XDG_ETAT_GTC, vg_NomMachine ) ;			

     	             if (!TipcSrvMsgWrite ( pl_NomDG,
      			              XDM_IdentMsg(XDM_ETAT_GTC),
                          	      XDM_FLG_SRVMSGWRITE,
                          	      T_IPC_FT_INT2, XDC_EQT_GTC,
                          	      T_IPC_FT_INT2, vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor,
                          	      T_IPC_FT_INT2, vl_etat,
                          	      NULL))
      	             {
         	        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_GTC non effectue." );
	             }
	             else
      	             {
         	        XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_GTC effectue." );
	             }
                  }
               
               
                  /* Si alarme ds la config : envoi de celle-ci */
                  if ( (vg_donneesGTC.ETOR[i+vl_ajout].Alarme != 0) &&
                       (vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor != 0) &&
                       !(vg_donneesGTC.Etat_Service) &&
                       !(vg_donneesGTC.ETOR[i+vl_ajout].Etat_Service) )
                  {
                     ex_env_alarme  ( 	vg_NomMachine,
				        vl_horodate,
				        XDC_EQT_TOR,
				        vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor,
				        vg_donneesGTC.ETOR[i+vl_ajout].Alarme,
				        (vl_etat == 0) ? XDC_VRAI : XDC_FAUX,
				        vg_donneesGTC.ETOR[i+vl_ajout].Nom,
					XZSC_22NumeroSiteLocal());
               
                  }
               
                  /* Si alerte ds la config */
                  if ( (vg_donneesGTC.ETOR[i+vl_ajout].Alerte != 0) &&
                       (vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor != 0) )
                  {
                     /* Si alerte passe de 1 a 0 -> envoi d'une alerte */
                     if (( vg_donneesGTC.ETOR[i+vl_ajout].Etat == 1) &&
                         ( vl_etat == 0) &&
                         !(vg_donneesGTC.Etat_Service) &&
                         !(vg_donneesGTC.ETOR[i+vl_ajout].Etat_Service) )
                     {
                        ex_env_alerte  ( vl_horodate,
                                         XDC_EQT_TOR,
                                         vg_donneesGTC.ETOR[i+vl_ajout].NoEqtETor,
                                         vg_donneesGTC.ETOR[i+vl_ajout].Alerte,
                                         vg_Mode_Fonct,
				    	XZSC_22NumeroSiteLocal(),
                                         NULL);
                     }
                  }
               }
               /* Memorisation de l'etat */
               vg_donneesGTC.ETOR[i+vl_ajout].Etat = vl_etat;
            }
            break;
         }
	 /*A c'est une trame de status complet */
	 case XZEXC_TRAME_STAT_COMP:
	 {
            /* Decodage de la trame de status */
            ex_decode_status ( pl_trame,
                               vl_tabstatus,
                               3,
                               vl_adrrgs,
                               vl_horodate,
                               vl_typeeqt,
                               vg_donneesGTC.Numero,
                               vg_NomMachine,
			       XZSC_22NumeroSiteLocal());
            break;
         }
      }
   }

   return (vl_cr);
}

