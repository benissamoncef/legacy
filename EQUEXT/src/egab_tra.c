/*E*/
/*Fichier :  $Id: egab_tra.c,v 1.6 2021/02/15 10:14:24 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2021/02/15 10:14:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des trames provenant des eqts
*  pour la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* JM	16/08/18	creation 1.1
* JMG	26/10/18	decodage trame systeme 1.2
* JMG	31/10/18	cmd LCR 1.3
* JMG	13/05/19	envoi alerte si panne DEM1328 1.4
* GR    09/07/2020      EMS 1.5
* jmg	15/02/21	plus d envoi de commandes vide 1.6
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "egab_tra.h"
#include "egab_cfg.h"
#include "egab_trace.h"
#include "xzsm.h"
#include "xzaac.h"

/* definitions de constantes */

#define CM_EDAI_PASDALERTE	0
#define CM_EDAI_ALERTE 		1
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern char *vg_idpassw;
extern XDY_NomMachine           vg_NomMachine;

extern INFO_EQT_FEN_LCR     *pg_DebutListeCABFLcr  ;
int vm_alarme=0	;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame et traitement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket:socket de lecture
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
------------------------------------------------------*/
{
   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   XDY_AdresseIP		vl_adrip;
   int				vl_typetrame;
   XZEXT_MSG_SOCKET		vl_Msg;
   int				vl_param;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   T_Liste_CAB		*pl_liste;
   XDY_Datagroup		pl_DG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   int                          vl_Voie,
      	                        vl_coefconf;
   char				vl_chaine[4];
   char                         vl_horod1[20],
                                vl_horod2[20],
                                vl_codesite[10],
                                vl_novoie[9],
                                vl_typeal[9],
                                vl_alerte[9];
   int                          vl_tpsarrveh;
   char				vl_autoroute[8];
   char				vl_erreur[20];
   int                          vl_pr;
   int                          vl_sens;
   XZSMT_Horodate       	pl_Horodatedecompo;
   char 			vl_annee[5];
   char				vl_vid[53]="\0",
                                vl_trm[53]="\0",
                                vl_ana[53]="\0";                           
   char                         vl_debfin[2];
   static XZEXT_TEXTE_STATUS vl_tabstatus[12];
   int                          vl_noerr;
   int                          vl_nbana,vl_nbana2;
   int                          vl_trouve;
   char				*pl_deb;
   int				vl_indtaberr;
   int 				vl_typ_rep;
   int 				vl_al;
   char 			vl_arg[6];
   XDY_Datagroup		pl_NomDG;
   int 				vl_Index = 0;
   int 				vl_RetCmd = 0;
   int 				vl_fin = XDC_VRAI;
   XZEXT_Trame			pl_mlcr = "";
   ldiv_t               	vl_Result_Div;
   XZSMT_Horodate       	vl_heure_string;
   int                 		vl_Minutes      = 0;
   int                 		vl_Heures      = 0;
   FILE                         *pl_Fd;
   char                         pl_PathEtFic[50] = "";
   char                         vl_str[50] = "";
   XDY_Texte			pl_Ligne = "";
   int				vl_ValidTrace = XDC_NOK;
   int				vl_NumAlerte, vl_TypeAlerte;
  	XDY_Octet		vl_detection;
 
   static char *version = "$Id: egab_tra.c,v 1.6 2021/02/15 10:14:24 pc2dpdy Exp $      : em_traitement_trame" ;
   
   /*A  Lecture du message recu dans la socket appelante */
   XZST_03EcritureTrace( XZSTC_FONCTION, " IN : %s", version );
   
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {
      /*A Si deconnexion tache esclave  alors fin de la tache */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
               XZST_03EcritureTrace( XZSTC_WARNING, "Perte connexion avec tache esclave alors : fin de la tache. " );
               dir_sortie();
      }

      XZST_03EcritureTrace( XZSTC_DEBUG2, "Message recu <%s>",vl_Msg);
      
      /*A Scan du message provenant de la socket */
      vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
      					   	    vl_entete2,
      					   	    vl_horod1,
      					   	    vl_horod2,
      					   	    &vl_nbnonrep,
      					  	    &vl_nberrcrc,
      					  	    &vl_nocmd,
      					   	    vl_adrip,
      					  	    &vl_typetrame,
      					  	    &vl_typ_rep);
      					 
      if (vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "Message mal forme (champ %d)", vl_param + 1);
         return (XDC_NOK);
      }
      					 
      /*A Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      /*A Gestion des trames provenant de telei */

      if (vl_typetrame == XZEXC_TRAME_TELEI_KO)
      {
         /*B Recherche du No DAI concern� en cas de reponse telei */
         for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
         {
            if ( !strcmp(pl_liste->cab.config.AdresseIP ,vl_adrip )) break;
         }
         if (pl_liste == NULL)
         {
            XZST_03EcritureTrace( XZSTC_WARNING, " Dai Non trouve en memoire mes=%s",vl_Msg);
            return (XDC_NOK);
         }
         
         /* Envoi de l'alarme */
         
         sprintf(vl_arg,"%d",vl_nberrcrc);
         
	 /*
         if ( !pl_liste->cab.Etat_Service )
            ex_env_alarme  ( 	vg_NomMachine,
				vl_horodate,
				XZECC_TYPE_EQT_CAP,
				pl_liste->cab.config.NumeroCAP,
				XZAMC_ERR_CRC_TELEI,
				XDC_VRAI,
				vl_arg,
				pl_liste->cab.config.SiteGestion);
				
	 */
         XZST_03EcritureTrace( XZSTC_WARNING, " Err crc provenant de la dai");

         return (XDC_NOK);
      }
      			
      if (vl_typetrame == XZEXC_TRAME_TELEI_OK)
      {
         /*B Recherche du No DAI concern� en cas de reponse telei */
         for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
         {
	    if ( !strcmp(pl_liste->cab.config.AdresseIP ,vl_adrip ))
               break;
         }
         if (pl_liste == NULL)
         {
            XZST_03EcritureTrace( XZSTC_WARNING, " Dai Non trouve en memoire mes=%s",vl_Msg);
            return (XDC_NOK);
         }
         
         sprintf(vl_arg,"%d",vl_nberrcrc);
         
	 /*
         if ( !pl_liste->cab.Etat_Service )
            ex_env_alarme  ( 	vg_NomMachine,
				vl_horodate,
				XZECC_TYPE_EQT_CAP,
				pl_liste->cab.config.NumeroCAP,
				XZAMC_ERR_CRC_TELEI,
				XDC_FAUX,
				vl_arg,
				pl_liste->cab.config.SiteGestion);
	*/

         if (strstr(vl_Msg,"ID") != NULL)
         {
            /* Envoi ACK a la DAI pour obtenir la suite de la trame alarme */   
            EnvoyerTrameIP("\0",
        		 XZEXC_TRAME_TELEI_ACK,
			 pl_liste->cab.config.AdresseIP,
		    pl_liste->cab.config.PortEsclave,
		       pl_liste->cab.config.AdresseRgs,
		 	 XZEXC_NOCMD,
		 	 XZEXC_PRIORITEFAIBLE,
			 pl_liste->cab.Socket);
            return (XDC_OK);
         }
         else
         {   
            if (strstr(vl_Msg,"MESU") != NULL)
            {
               vl_typetrame = XZEXC_TRAME_ALERTE_DAI;
            }
            else
            {
               if (strstr(vl_Msg,"SYST") != NULL)
               {   
                  vl_typetrame = XZEXC_TRAME_STAT_SYS;
               }
               else
               {
                  XZST_03EcritureTrace( XZSTC_DEBUG1, " Trame dai incomprehensible mes=%s",vl_Msg);
                  /* Acquitement telei */   
		  /*
                  EnvoyerTrameIP("\0",
        		       XZEXC_TRAME_TELEI_NREP,
			       pl_liste->cab.config.AdresseIP,
			       pl_liste->cab.config.PortEsclave,
			       pl_liste->cab.config.AdresseRgs,
		 	       XZEXC_NOCMD,
		 	       XZEXC_PRIORITEFAIBLE,
        		       pl_liste->cab.Socket);
		  */
                  return (XDC_NOK);
               }
            }
         }
      }
       
      /*B Recherche du No DAI concern� */
      for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
      {
         if (!strcmp(pl_liste->cab.config.AdresseIP,vl_adrip )) break;
      }
      if (pl_liste == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Dai Non trouve en memoire mes=%s",vl_Msg);
         return (XDC_NOK);
      }
      
      /* Trace de la commande de la date */
      if ( vl_typetrame == XZEXC_TRAME_DATE )
               ex_ecriture_trace_cmd (  XDC_EQT_CAP,
                                        pl_liste->cab.config.NumeroCAP, 
                                	strtok(strstr(vl_Msg,XZEXC_SEP_TRAME)+4,"~"),
                                	strtok(NULL,"~"),
                                	vl_nocmd );
      
      /*A Traitement des alarmes transmission avec l'equipement */
           ex_traite_alarme_com ( 	vg_NomMachine,
					XDC_EQT_CAP,
					pl_liste->cab.config.NumeroCAP,
      					vl_typ_rep,
      					vl_nbnonrep,
      					vl_nberrcrc,
      					vl_typetrame,
      					XZEXC_MODE_NORMAL,
					pl_liste->cab.config.SiteGestion);

      if (vl_typetrame != XZEXC_TRAME_FEN_LCR) {
	if (vl_typ_rep == XZEXC_TRAME_NON_TRANSM) {
 	  pl_liste->cab.detection1 = EGABC_DETECTION_HS ;
 	  pl_liste->cab.detection2 = EGABC_DETECTION_HS ;
 	  pl_liste->cab.detection3 = EGABC_DETECTION_HS ;
 	  pl_liste->cab.detection4 = EGABC_DETECTION_HS ;
	}
      }
      
      pl_liste->cab.etat= XDC_VRAI;
      
      /*A On se positionne sur le debut de la trame */
      pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;
      
      while (pl_trame[0]==' ') pl_trame++;


      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {
         /*A C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
            /* Appel de ex_mrcr */
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeCABFLcr, vl_typ_rep );
            break;
	 
         /*A C'est une trame status complet */
	 case XZEXC_TRAME_CONF_DAI:
            
            /*B Recherche du parametre Cmd */
            if ( (pl_deb=strstr(pl_trame,"Cmd=")) == NULL)
               pl_deb=strstr(pl_trame,"cmd=");
            XZST_03EcritureTrace(XZSTC_INFO,"trame = <%s>", pl_deb);
            /*B Recherche du No DAI concern� */
            for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
            {
               if (!strcmp(pl_liste->cab.config.AdresseIP,vl_adrip ))
               { 
                  break;
               }                     
            }
            if (pl_liste == NULL) return (XDC_OK);

            /*B Si reponse ACK ou absence du parametre Cmd alors commande en cours */
            if ( pl_deb == NULL )
            { 
               /* Ecriture dans fichier Trace_CMD_DAI de la config voie */    
   	       if ( vl_typ_rep == XZEXC_REP_NAK ) sprintf ( pl_mlcr, "%s> <Reponse NAK", strtok(pl_trame, "~\n") ); 
               if ( vl_typ_rep == XZEXC_REP_ACK ) sprintf ( pl_mlcr, "%s> <Reponse ACK", strtok(pl_trame, "~\n") ); 
               if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )  sprintf ( pl_mlcr, "Pas de reponse pour \"%s\"", strtok(pl_trame, "~\n")); 
               ex_ecriture_trace_cmd (  XDC_EQT_CAP,
                                        pl_liste->cab.config.NumeroCAP, 
                                	NULL,
                                	pl_mlcr,
                                	vl_nocmd );
               if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
                  break; 
            }
                                	            
           break;
            
         
         /*A C'est une trame status complet */
	 case XZEXC_TRAME_TEST_COM :
	 case XZEXC_TRAME_STAT_COMP:
            /*B Recherche du No DAI concern� */
            for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
            {
               if (!strcmp(pl_liste->cab.config.AdresseIP,vl_adrip ))
               { 
                  break;
               }                     
            }
           ex_decode_status ( pl_trame,
                               vl_tabstatus,
                               12,
                               pl_liste->cab.config.AdresseRgs,
                               vl_horodate,
                               XDC_EQT_CAP,
                               pl_liste->cab.config.NumeroCAP,
                               vg_NomMachine,
                               pl_liste->cab.config.SiteGestion);
 
            break;
	 
	 /*A c'est une trame d'alerte */
	 case XZEXC_TRAME_ALERTE_DAI:
	 {
	    strcpy (vl_autoroute, "XXXX");
	    vl_pr = -1;
	    vl_sens = -1;

	       /*B Acquisition des donnes de la trame */
               vl_param=sscanf(pl_trame,"TC E %s %s %s MESU %s %s %s %d %d AUT=%s PR=%d SENS=%d"
                                    ,vl_horod1
                                    ,vl_horod2
                                    ,vl_codesite
                                    ,vl_typeal
                                    ,vl_novoie
                                    ,vl_alerte
                                    ,&vl_tpsarrveh
                                    ,&vl_coefconf
                                    ,vl_autoroute
                                    ,&vl_pr
                                    ,&vl_sens);

               if (vl_param < 8)
               {
                  XZST_03EcritureTrace( XZSTC_WARNING, " Trame dai incomprehensible mes=%s",vl_Msg);
               
                  /* Acquitement telei */   
                  EnvoyerTrameIP("\0",
        		    XZEXC_TRAME_TELEI_NREP,
			    pl_liste->cab.config.AdresseIP,
			    pl_liste->cab.config.PortEsclave,
			    pl_liste->cab.config.AdresseRgs,
		 	    XZEXC_NOCMD,
		 	    XZEXC_PRIORITEFAIBLE,
        		    pl_liste->cab.Socket);
                  return (XDC_NOK);
               }

               strcat(vl_typeal,vl_novoie);
               strcpy(vl_novoie,vl_typeal);

	    XZST_03EcritureTrace( XZSTC_FONCTION, "Donnees trame: D:%s %s Site:%s Type:%s V:%s N:%s T:%d CC:%d LOC:%s-%d-%d"
                                    ,vl_horod1
                                    ,vl_horod2
                                    ,vl_codesite
                                    ,vl_typeal
                                    ,vl_novoie
                                    ,vl_alerte
                                    ,vl_tpsarrveh
                                    ,vl_coefconf
                                    ,vl_autoroute
                                    ,vl_sens
                                    ,vl_pr);

	    /*B Passage de l'annee de 2 a 4 caracteres */
	    /* Lecture vl_horodate */
	    XZSM_07LireHorodate( XZSMC_TYPE_RTWORKS , &vl_js , &vl_horodate );
	
            /* Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
            XZSM_11ConversionHorodate ( vl_horodate, &pl_Horodatedecompo );
            
            strncpy(vl_annee,&pl_Horodatedecompo[6],4);
            vl_annee[4]='\0';
            
            strcpy(&vl_horod1[6],vl_annee);
            
            sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
            XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
            
            /*B Recherche du No DAI concern� */
            
            for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
            {
               if (!strcmp(pl_liste->cab.config.AdresseIP,vl_adrip )) break;
            }
            
      	    strncpy(vl_chaine,&vl_codesite[7],2);
      	    vl_chaine[2]='\0';
      	    
      	    strncpy(vl_chaine,&vl_novoie[0],1);
      	    vl_chaine[1] = '\0';
            vl_Voie = atoi(&vl_novoie[1]);
            vl_NumAlerte = 0;
            vl_TypeAlerte = 0;
	    ex_decode_alerte_dai  ( vl_alerte, &vl_NumAlerte, &vl_TypeAlerte );
            if ( vl_NumAlerte == 0 )
            {
               	XZST_03EcritureTrace( XZSTC_WARNING, " Type alerte incorrect mes=%s",vl_Msg);
                /* Acquitement telei */   
                EnvoyerTrameIP("\0",
        		    XZEXC_TRAME_TELEI_NAK,
			    pl_liste->cab.config.AdresseIP,
			    pl_liste->cab.config.PortEsclave,
			    pl_liste->cab.config.AdresseRgs,
		 	    XZEXC_NOCMD,
		 	    XZEXC_PRIORITEFAIBLE,
        		    pl_liste->cab.Socket);
                return (XDC_NOK);
            }
	    
	    if (vl_NumAlerte == XZAAC_ALERTE_GABEB) 
	    {
		/*detection bouchon*/
		/*recup voie*/
		/*memo etat detection*/
		XZST_03EcritureTrace( XZSTC_FONCTION,"voie %d, chaine %c", vl_Voie, vl_chaine[0] );
		switch (vl_Voie) {
			case 1 : pl_liste->cab.detection1 = (vl_chaine[0] == 'O') ? EGABC_DETECTION : EGABC_NO_DETECTION;
				vl_detection = pl_liste->cab.detection1;
					break;
			case 2 : pl_liste->cab.detection2 = (vl_chaine[0] == 'O') ? EGABC_DETECTION : EGABC_NO_DETECTION;
				vl_detection = pl_liste->cab.detection2;
					break;
			case 3 : pl_liste->cab.detection3 = (vl_chaine[0] == 'O') ? EGABC_DETECTION : EGABC_NO_DETECTION;
				vl_detection = pl_liste->cab.detection3;
					break;
			case 4 : pl_liste->cab.detection4= (vl_chaine[0] == 'O') ? EGABC_DETECTION : EGABC_NO_DETECTION;
				vl_detection = pl_liste->cab.detection4;
					break;
		}
		 sprintf(vl_str, "Detection voie %d etat %c",
		 			vl_Voie,
					vl_chaine[0]);

		ex_ecriture_trace_alerte(XDC_EQT_CAP, pl_liste->cab.config.NumeroCAP, pl_trame);
		trace_process(EGAB_TRACE_TYPE_DETECTION,
				pl_liste->cab.config.PointCaracteristique,
				0,0,
				pl_liste->cab.config.NumeroCAP,
				vl_detection,
				0,0,
				vl_str);
                 XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CAP, XZSC23_Nom_From_Numero(pl_liste->cab.config.SiteGestion));
                 if (!TipcSrvMsgWrite ( pl_DG,
                                XDM_IdentMsg(XDM_ETAT_CAP),
                                XDM_FLG_SRVMSGWRITE,
                                T_IPC_FT_INT2,pl_liste->cab.config.NumeroCAP,
                                T_IPC_FT_INT2, vl_Voie,
                                T_IPC_FT_INT2 , vl_detection,
                                NULL))
                        XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_CAP non effectue." );
                else
                        XZST_03EcritureTrace(XZSTC_INFO,"Envoi message XDM_ETAT_CAP effectue, %d %d %d",
                                        pl_liste->cab.config.NumeroCAP, vl_Voie, vl_detection);

	    }
            /*B Si c'est une debut d'alerte -> envoi d'alerte */
	   
            else if ( (vl_chaine[0] == 'O') && (vl_NumAlerte > 0) && (pl_liste->cab.DernAlerte != vl_NumAlerte)  )
            {
            	pl_liste->cab.DernAlerte = vl_NumAlerte;
                if ( !pl_liste->cab.Etat_Service  )
                {
                    if ( vl_sens > 0 )
                    { 
			ex_env_alerte_localisee  ( vl_horodate,
                                   XDC_EQT_CAP,
                                   pl_liste->cab.config.NumeroCAP,
                                   vl_NumAlerte,
                                   XZEXC_MODE_NORMAL,
                                   pl_liste->cab.config.SiteGestion,
                                   vl_autoroute,
                                   (XDY_PR) vl_pr,
                                   (XDY_Sens) vl_sens,
                                   vl_Voie,
                                   vl_tpsarrveh,
                                   vl_coefconf,
                                   NULL );
                    } 
                    else
                    { 
			ex_env_alerte  ( vl_horodate,
                                   XDC_EQT_CAP,
                                   pl_liste->cab.config.NumeroCAP,
                                   vl_NumAlerte,
                                   XZEXC_MODE_NORMAL,
                                   pl_liste->cab.config.SiteGestion,
                                   vl_Voie,
                                   vl_tpsarrveh,
                                   vl_coefconf,
                                   NULL );
                    } 
		}

            }
        
            /*B Memorisation de fin d'alerte */
            if ( vl_chaine[0] == 'F' )
            {
               pl_liste->cab.DernAlerte = 0;
            }
               
            /* Acquitement telei */   
            EnvoyerTrameIP("\0",
        		 XZEXC_TRAME_TELEI_ACK,
			 pl_liste->cab.config.AdresseIP,
			 pl_liste->cab.config.PortEsclave,
			 pl_liste->cab.config.AdresseRgs,
		 	 XZEXC_NOCMD,
		 	 XZEXC_PRIORITEFAIBLE,
        		 pl_liste->cab.Socket);
            break;
	 }
         /*A c'est une trame de status */
         case XZEXC_TRAME_STAT_SYS:
         {
            /*B Acquisition des donnees de la trame */
            vl_param=sscanf(pl_trame,"TC E %s %s %s SYST %s %d %s"
                                    ,vl_horod1
                                    ,vl_horod2
                                    ,vl_codesite
                                    ,vl_debfin
                                    ,&vl_Voie
				    ,vl_erreur);
            if (vl_param != 6)
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Trame dai incomprehensible mes=%s",vl_Msg);
               
               /* Acquitement telei */   
               EnvoyerTrameIP("\0",
        		    XZEXC_TRAME_TELEI_NREP,
			    pl_liste->cab.config.AdresseIP,
			    pl_liste->cab.config.PortEsclave,
        		    pl_liste->cab.config.AdresseRgs,
		 	    XZEXC_NOCMD,
		 	    XZEXC_PRIORITEFAIBLE,
        		    pl_liste->cab.Socket);
               return (XDC_NOK);
            }
	    
            /*B Recherche du No DAI concerne */
            
            for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
            {
               if (!strcmp(pl_liste->cab.config.AdresseIP,vl_adrip )) break;
            }
            
	    XZST_03EcritureTrace( XZSTC_WARNING, "Trame SYST : %s %d",
				vl_debfin, vl_Voie);

		switch (vl_Voie) {
			case 1 : pl_liste->cab.detection1 = (vl_debfin[0] == 'O') ? EGABC_DETECTION_HS : EGABC_NO_DETECTION;
					break;
			case 2 : pl_liste->cab.detection2 = (vl_debfin[0] == 'O') ? EGABC_DETECTION_HS : EGABC_NO_DETECTION;
					break;
			case 3 : pl_liste->cab.detection3 = (vl_debfin[0] == 'O') ? EGABC_DETECTION_HS : EGABC_NO_DETECTION;
					break;
			case 4 : pl_liste->cab.detection4= (vl_debfin[0] == 'O') ? EGABC_DETECTION_HS : EGABC_NO_DETECTION;
					break;
		}

		/*si alarme presente*/
		if (vl_debfin[0] == 'O') {
	    XZST_03EcritureTrace( XZSTC_WARNING, "vm_alarme %d",vm_alarme);
			if (vm_alarme == 0) {
				vm_alarme=1;
				/*appel XZAA01*/
	    XZST_03EcritureTrace( XZSTC_WARNING, "XZAA");
				xzaa01_Creer_Alerte(vl_horodate,XDC_EQT_CAP,pl_liste->cab.config.NumeroCAP,
							XZAAC_ALERTE_PANNE_GABEB,
							XZAAC_ALERTE_PANNE_GABEB_NOM,
							pl_liste->cab.config.SiteGestion);
			}
		}
		else
			vm_alarme=0;

		if (!strcmp(vl_erreur,"COM"))
		  ex_env_alarme  (   vg_NomMachine,
			vl_horodate,
			XDC_EQT_CAP,
			pl_liste->cab.config.NumeroCAP,
			XZAMC_NON_REP,
			(vl_debfin[0] == 'O') ? XDC_VRAI : XDC_FAUX,
			NULL,
			pl_liste->cab.config.SiteGestion);
		else
		  ex_env_alarme  (   vg_NomMachine,
			vl_horodate,
			XDC_EQT_CAP,
			pl_liste->cab.config.NumeroCAP,
			XZAMC_DEF_VIDEO,
			(vl_debfin[0] == 'O') ? XDC_VRAI : XDC_FAUX,
			NULL,
			pl_liste->cab.config.SiteGestion);
			
            /* Acquitement telei */   
            EnvoyerTrameIP("\0",
        		 XZEXC_TRAME_TELEI_ACK,
			 pl_liste->cab.config.AdresseIP,
			 pl_liste->cab.config.PortEsclave,
			 pl_liste->cab.config.AdresseRgs,
		 	 XZEXC_NOCMD,
		 	 XZEXC_PRIORITEFAIBLE,
        		 pl_liste->cab.Socket);
         } 
      }
   }  
   XZST_03EcritureTrace( XZSTC_FONCTION, " OUT : %s", version );

   return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  commabde fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void    ep_CMD_LCR ( T_IPC_CONN                 va_Cnx,
                     T_IPC_CONN_PROCESS_CB_DATA         data,
                     T_CB_ARG                           arg)

/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
*
* CONDITION D'UTILISATION
*  Fonction appelo?=e lors de la ro?=ception du message
*  XDM_ELCR
*
* FONCTION
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS   
   T_INT2                       vl_TypeEqt;
   T_INT2                       vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2                       vl_Periode;
   T_REAL8                      vl_HrDebut;
   T_REAL8                      vl_HrFin;
   T_STR			vl_NomMachine;
#else
   XDY_Mot                   vl_TypeEqt;
   XDY_Mot                   vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Entier                   vl_Periode;
   XDY_Horodate                 vl_HrDebut;
   XDY_Horodate                 vl_HrFin;
   XDY_NomMachine		vl_NomMachine;
#endif   

   XZEXT_MSG_SOCKET             pl_MsgTimer;
   INFO_EQT_FEN_LCR_IP             pl_eqt, *pl_ListeEqt;
   XZEXT_CLE_PROG               vl_clef;
   char vl_texte_cmd[50]="";
   T_Liste_CAB	*pl_liste=NULL;

        /*A Lecture du message */
   XZST_03EcritureTrace(XZSTC_WARNING ,"IN :ep_CMD_LCR"  );
        if(!TipcMsgRead(data->msg,
                        T_IPC_FT_INT2,  &vl_TypeEqt,
                        T_IPC_FT_INT2,  &vl_NoEqt,
                        T_IPC_FT_STR,   &vl_FicCmdLcr,
                        T_IPC_FT_STR,   &vl_FicResLcr,
                        T_IPC_FT_INT2,  &vl_Periode,
                        T_IPC_FT_REAL8, &vl_HrDebut,
                        T_IPC_FT_REAL8, &vl_HrFin,
                        T_IPC_FT_STR,   &vl_NomMachine,
                        NULL))

       {
           /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ELCR non lu \n" );
           XZST_10ArchiverMsgSyst(0,"ep_CMD_LCR : Message XDM_ELCR(Pmv Picto) recu avec parametre(s) invalide(s)",NULL);
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_CMD_LCR");
           return;
        }

        /* Trace du message en entree de la fonction */
        XZST_03EcritureTrace(XZSTC_WARNING,"IN ep_CMD_LCR");
      for ( pl_liste = pg_CAB; pl_liste != NULL; pl_liste = pl_liste->suivant )
      {
         if (pl_liste->cab.config.NumeroCAP==vl_NoEqt ) break;
      }
      if (pl_liste == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " CAP Non trouve en memoire mes=%d",vl_NoEqt);
         return ;
      }
     
	pl_eqt.TypeEqt=XDC_EQT_CAP;
        strcpy(pl_eqt.NomFichCmdesLCR,vl_FicCmdLcr);
        strcpy(pl_eqt.NomFichResLCR,vl_FicResLcr);
        pl_eqt.Periode=vl_Periode;
        pl_eqt.HrDebut=vl_HrDebut;
        pl_eqt.HrFin=vl_HrFin;
        strcpy(pl_eqt.NomMachine,vl_NomMachine);
        pl_eqt.NoEqt=vl_NoEqt;
        pl_eqt.Position=0;
        pl_eqt.Suivant=NULL;
	strcpy(pl_eqt.AdresseIP , pl_liste->cab.config.AdresseIP);
	pl_eqt.Port = pl_liste->cab.config.PortMaitre; 
	pl_eqt.Socket = pl_liste->cab.Socket;
	ex_init_lcr_ip (       &pg_DebutListeCABFLcr,
				pl_eqt,
				pl_liste->cab.Socket,
				pl_liste->cab.config.AdresseRgs,
				pl_liste->cab.config.NumeroCAP,
				"CAB_FLCR_NoEqt",
				pl_liste->cab.config.AdresseIP,
				pl_liste->cab.config.PortMaitre);

        /* Fin du callback */
        XZST_03EcritureTrace(XZSTC_WARNING, "OUT Callback ep_CMD_LCR" );

}

