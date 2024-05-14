/*E*/
/*Fichier :  $Id: edai_tra.c,v 1.36 2021/05/04 13:25:02 pc2dpdy Exp $      Release : $Revision: 1.36 $        Date : $Date: 2021/05/04 13:25:02 $
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
* Nagiel.E	version 1.1	21 Oct 1994	: Creation
* Nagiel.E	version 1.2	22 Dec 1994	:
* Nagiel.E	version 1.3	22 Dec 1994	:
* Nagiel.E	version 1.4	05 Jan 1995	:
* Mismer.D	version 1.5	11 Jan 1995	:
* Nagiel.E	version 1.6	16 Jan 1995	:
* Nagiel.E	version 1.7	16 Jan 1995	:
* Mismer.D	version 1.8	16 Jan 1995	:
* Mismer.D	version 1.9	09 Fev 1995	:
* Mismer.D	version 1.10	17 Mar 1995	:
* Volcic.F	version 1.11	04 Avr 1995	:
* Volcic.F	version 1.12	19 Avr 1995	: Modif Libelle des alertes
* Fontaine.C	version 1.13	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.14	20 Jui 1995	: Ajout traitement eqt occupe
* Volcic.F	version 1.15	23 Oct 1995	: Ajout trace
* Mismer.D	version 1.16	15 Nov 1995	: Ajout sortie de la tache si deconnexion tache esclave
* Volcic.F	version 1.17	04 Dec 1995	: Modif return XDC_OK => 															continue apres reception XDC_VOIE_NEUTRALISEE
* Mismer.D	version 1.18	15 Fev 1996	: Correction memorisation alerte dai et config voie (DEM/751)
* Mismer.D	version 1.19	19 Avr 1996	: Ajout trace mesure en fichier (DEM/1120)
* Mismer.D	version 1.20 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
* Mismer.D	version 1.21 	07 Jan 1997	: Ajout trace config voie sur travaux (DEM/1155)
* Mismer.D	version 1.22 	07 Nov 1997	: Corection pour DAI inrets (STATUS Cmd=) (DEM/xxxx)
* Mismer.D      version 1.23    08 Jan 1998     : Modif new alerte DAI (DEM/APG17) fiche ANA74
* Mismer.D      version 1.24    29 Mai 2000     : Suppression de la relance de la config des voies sur controle "AUCUN VEHICULE" 
* Mismer.D      version 1.25    29 Mai 2000     : Suppression de la relance de la config des voies sur controle "AUCUN VEHICULE" 
* Guilhou	version 1.27	25 Mai 2001	: envoi vitesse meme si voie bloquee ou neutralisee
* Hilmarcher    version 1.27    13 Mai 2005     : rajout trace pour analyse pb envoi alerte dai
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG            10/02/2010      : portage Linux 
* PNI            30/08/2010      : correction portage Linux
* JPL		30/11/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. pour fonction em_traitement_trame
* JPL		30/11/11 : Migration architecture HP ia64 (DEM 975) : adequation format sprintf et type d'entier  1.31
* JPL		30/11/11 : ajout de la localisation a l'alerte si elle figure dans la trame (DEM 1013)  1.32
* JPL		xx/03/12 : Correction tailles de donnees (dates, code site)  1.33
* JPL		10/04/12 : Ajout trace d'une trame d'alerte DAI dans tous les cas  1.34
* JMG		06/06/19 : passage IP DEM1333 1.35
* NB            28/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.7
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "edai_tra.h"
#include "edai_adt.h"
#include "edai_bis.h"

/* definitions de constantes */

#define CM_EDAI_PASDALERTE	0
#define CM_EDAI_ALERTE 		1
#define CM_EDAI_PATH_TRA_MES	"/produits/migrazur/appliSD/fichiers/dyn/Mes_DAI"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
XDY_Horodate vg_last_switch;

/* declaration de fonctions internes */

/* definition de fonctions externes */


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
   				vl_adrrgs[20];
   int				vl_typetrame;
   XZEXT_MSG_SOCKET		vl_Msg;
   int				vl_param;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   XDY_EMes_DAI			vl_mes;
   EDAI_DONNEES_DAI		*pl_listeDAI;
   EDAI_DONNEES_ANALYSEUR	*pl_listeAnaly;
   XDY_Datagroup		pl_DG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   XDY_Horodate			vl_horodate2;
   int 				vl_nocmd;   
   XDY_Mot			vl_Analyseur;
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
   int                          vl_pr;
   int                          vl_sens;
   XZSMT_Horodate       	pl_Horodatedecompo;
   char 			vl_annee[5];
   char				vl_vid[53]="\0",
                                vl_trm[53]="\0",
                                vl_ana[53]="\0";                           
   char                         vl_debfin[2];
   int                          vl_noerr;
   int                          vl_nbana,vl_nbana2;
   int                          vl_trouve;
   char				*pl_deb;
   int				vl_indtaberr;
   int 				vl_typ_rep;
   int 				vl_al;
   char 			vl_arg[6];
   XDY_EMes_DAI			vl_infotrafic;
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
   XDY_Texte			pl_Ligne = "";
   int				vl_ValidTrace = XDC_NOK;
   int				vl_NumAlerte, vl_TypeAlerte;
   
   static char *version = "$Id: edai_tra.c,v 1.36 2021/05/04 13:25:02 pc2dpdy Exp $      : em_traitement_trame" ;
   
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
         for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
            if ( pl_listeDAI->SocketEsclave == va_socket )
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Perte connexion avec tache esclave alors : fin de la tache. " );
               ec_sortir();
            }
      }

      XZST_03EcritureTrace( XZSTC_WARNING, "Message recu <%s>",vl_Msg);
      
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
         XZST_03EcritureTrace( XZSTC_WARNING, "Message mal forme (champ %d)", vl_param + 1);
         return (XDC_NOK);
      }
      					 
      /*A Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      /*A Gestion des trames provenant de telei */

      if (vl_typetrame == XZEXC_TRAME_TELEI_KO)
      {
         /*B Recherche du No DAI concernÅ en cas de reponse telei */
         for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
         {
            if ( pl_listeDAI->SocketEsclave == va_socket ) break;
         }
         if (pl_listeDAI == NULL)
         {
            XZST_03EcritureTrace( XZSTC_WARNING, " Dai Non trouve1 en memoire mes=%s",vl_Msg);
            return (XDC_NOK);
         }
         
         /* Envoi de l'alarme */
         
         sprintf(vl_arg,"%d",vl_nberrcrc);
         
         if ( !pl_listeDAI->Etat_Service )
            ex_env_alarme  ( 	vg_NomMachine,
				vl_horodate,
				XZECC_TYPE_EQT_DAI,
				pl_listeDAI->Config.Numero,
				XZAMC_ERR_CRC_TELEI,
				XDC_VRAI,
				vl_arg,
				pl_listeDAI->Config.SiteGestion);
				
         XZST_03EcritureTrace( XZSTC_WARNING, " Err crc provenant de la dai");
         return (XDC_NOK);
      }
      			
      if (vl_typetrame == XZEXC_TRAME_TELEI_OK)
      {
         /*B Recherche du No DAI concernÅ en cas de reponse telei */
         for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
         {
            if ( pl_listeDAI->SocketEsclave == va_socket ) 
            {
               strcpy(vl_adrrgs,pl_listeDAI->Config.AdresseRGS);
               break;
            }
         }
         if (pl_listeDAI == NULL)
         {
            XZST_03EcritureTrace( XZSTC_WARNING, " Dai Non trouve2 en memoire mes=%s",vl_Msg);
            return (XDC_NOK);
         }
         
         sprintf(vl_arg,"%d",vl_nberrcrc);
         
         if ( !pl_listeDAI->Etat_Service )
            ex_env_alarme  ( 	vg_NomMachine,
				vl_horodate,
				XZECC_TYPE_EQT_DAI,
				pl_listeDAI->Config.Numero,
				XZAMC_ERR_CRC_TELEI,
				XDC_FAUX,
				vl_arg,
				pl_listeDAI->Config.SiteGestion);

         if (strstr(vl_Msg,"ID") != NULL)
         {
            /* Envoi ACK a la DAI pour obtenir la suite de la trame alarme */   
            if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	            EnvoyerTrame("\0",
	        		 XZEXC_TRAME_TELEI_ACK,
	        		 pl_listeDAI->Config.AdresseRGS,
			 	 XZEXC_NOCMD,
			 	 XZEXC_PRIORITEFAIBLE,
	        		 pl_listeDAI->SocketEsclave);
	    else
	            EnvoyerTrameIP2(" \0",
	        		 XZEXC_TRAME_TELEI_ACK,
                                 pl_listeDAI->Config.AdresseIP,
                                 pl_listeDAI->Config.Port,
				 pl_listeDAI->Config.Adresse2IP,
                                 pl_listeDAI->Config.Port2,
                                 pl_listeDAI->Config.DAIBis,
	        		 pl_listeDAI->Config.AdresseRGS,
			 	 XZEXC_NOCMD,
			 	 XZEXC_PRIORITEFAIBLE,
	        		 pl_listeDAI->SocketEsclave);

            return (XDC_OK);
         }
         else
         {   
	     XZST_03EcritureTrace( XZSTC_WARNING, "ICI");
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
                  XZST_03EcritureTrace( XZSTC_WARNING, " Trame dai incomprehensible mes=%s",vl_Msg);
                  /* Acquitement telei */   
            	if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
                	  EnvoyerTrame("\0",
        			       XZEXC_TRAME_TELEI_NREP,
	        		       pl_listeDAI->Config.AdresseRGS,
			 	       XZEXC_NOCMD,
			 	       XZEXC_PRIORITEFAIBLE,
	        		       pl_listeDAI->SocketEsclave);
		else
                	  EnvoyerTrameIP2(" \0",
					  XZEXC_TRAME_TELEI_NREP,
					  pl_listeDAI->Config.AdresseIP,
					  pl_listeDAI->Config.Port,
					  pl_listeDAI->Config.Adresse2IP,
					  pl_listeDAI->Config.Port2,
					  pl_listeDAI->Config.DAIBis,
					  pl_listeDAI->Config.AdresseRGS,
					  XZEXC_NOCMD,
					  XZEXC_PRIORITEFAIBLE,
					  pl_listeDAI->SocketEsclave);
		return (XDC_NOK);
               }
            }
         }
      }
       
      /*B Recherche du No DAI concernÅ */

/* 	XZST_03EcritureTrace( XZSTC_WARNING, " Dai [%s] recherche dans pl_liste_DAI",vl_adrrgs);
*/

      for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
      {
/* 	XZST_03EcritureTrace( XZSTC_WARNING, " Parcours pl_listeDAI RGS [%s] IP [%s]",pl_listeDAI->Config.AdresseRGS,pl_listeDAI->Config.AdresseIP);
*/

      	if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)){
        	if (!strcmp(pl_listeDAI->Config.AdresseRGS,vl_adrrgs )) 
		{
/* 			XZST_03EcritureTrace( XZSTC_WARNING, " Break RGS");
*/
			break;
		}
	}
	else {
		if (!strcmp(pl_listeDAI->Config.AdresseIP,vl_adrrgs ) || !strcmp(pl_listeDAI->Config.AdresseRGS,vl_adrrgs )  || !strcmp(pl_listeDAI->Config.Adresse2IP,vl_adrrgs )  )
		{
			strcpy(vl_adrrgs,pl_listeDAI->Config.AdresseRGS);
/* 			XZST_03EcritureTrace( XZSTC_WARNING, " Break IP");
*/
			break;
		}
	}
      }
      if (pl_listeDAI == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Dai [%s] Non trouve3 en memoire mes=%s",vl_adrrgs,vl_Msg);
         return (XDC_NOK);
      }
      else
      		XZST_03EcritureTrace( XZSTC_WARNING, " Dai [%s] trouve en memoire mes=[%s]",vl_adrrgs,vl_Msg);
      
      /* Trace de la commande de la date */
      if ( vl_typetrame == XZEXC_TRAME_DATE )
               ex_ecriture_trace_cmd (  XDC_EQT_DAI,
                                        pl_listeDAI->Config.Numero, 
                                	strtok(strstr(vl_Msg,XZEXC_SEP_TRAME)+4,"~"),
                                	strtok(NULL,"~"),
                                	vl_nocmd );
      
      /*A Traitement des alarmes transmission avec l'equipement */
      if ( (vl_typetrame != XZEXC_TRAME_CONF_DAI) &&
           (vl_typetrame != XZEXC_TRAME_FEN_LCR) &&
           (ex_traite_alarme_com ( 	vg_NomMachine,
					XDC_EQT_DAI,
					pl_listeDAI->Config.Numero,
      					vl_typ_rep,
      					vl_nbnonrep,
      					vl_nberrcrc,
      					vl_typetrame,
      					vg_Mode_Fonct,
					pl_listeDAI->Config.SiteGestion) == XDC_NOK) )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Erreur trame non transmise ou non comprise sur l'eqt no %d"
                                            ,pl_listeDAI->Config.Numero);
         if ( vl_typetrame == XZEXC_TRAME_STAT_COMP )
            pl_listeDAI->EtatTrans = EDAIC_EQT_PANNE;
         return (XDC_NOK);
      }
      pl_listeDAI->EtatTrans = EDAIC_EQT_LIBRE;
      
      /*A On se positionne sur le debut de la trame */
      pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;
      
      while (pl_trame[0]==' ') pl_trame++;

      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {
         /*A C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
         {
            /* Appel de ex_mrcr */
	    XZST_03EcritureTrace( XZSTC_WARNING, "retour FENETRE LCR");
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeDAIFLcr, vl_typ_rep );
            break;
         } 
         /*A C'est une trame d'informations DAI */
         case XZEXC_TRAME_INFO_TRAFIC_DAI:
         {   
            /*B Recherche du No DAI concernÅ */
            for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
            {
       		{ 
              		pl_listeDAI->EtatTrans = EDAIC_EQT_LIBRE;
               		break;
       		}
            }

            /*B Recherche du No d'analyseur concernÅ */
      	    strncpy(vl_chaine,pl_trame+7,2);
      	    vl_chaine[2]='\0';
      	    vl_Analyseur=atoi(vl_chaine);
      	    
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {
                if ((pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero) && 
                    (vl_Analyseur == pl_listeAnaly->Config.NoAnaly)) break;
            }
            if (pl_listeAnaly == NULL)
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Analyseur(%d) Non trouve en memoire mes=%s(1)",vl_Analyseur,vl_Msg);
               return (XDC_NOK);
            }
                  	    
            /*B Affectations des infos lues dans la structure */
            pl_listeAnaly->Mes.Horodate = 	vl_horodate;
            pl_listeAnaly->Mes.Numero = 	pl_listeDAI->Config.Numero;
            pl_listeAnaly->Mes.NumAnalyseur = 	vl_Analyseur;
            
      	    pl_listeAnaly->Mes.VitesseVoie[0] = EDAIC_MES_INCONNU;
            for (vl_Index = 0; vl_Index < EDAIC_NB_VOIE-1; vl_Index ++)
            {
      	       strncpy(vl_chaine,pl_trame+9+(vl_Index*3),3);
      	       vl_chaine[3]='\0';
	       
      	       if ( (pl_listeAnaly->EtatVoie[vl_Index+1] == XDC_VOIE_SENS_NORMAL)  || 
      	            (pl_listeAnaly->EtatVoie[vl_Index+1] == XDC_VOIE_SENS_INVERSE) ||
		    (pl_listeAnaly->EtatVoie[vl_Index+1] == XDC_VOIE_NEUTRALISEE)  ||
		    (pl_listeAnaly->EtatVoie[vl_Index+1] == XDC_VOIE_BLOQUEE_NEUTRALISEE) )
      	       {
      	          pl_listeAnaly->Mes.VitesseVoie[vl_Index] = atoi(vl_chaine);
      	          if ( pl_listeAnaly->Mes.VitesseVoie[vl_Index] == EDAIC_AUCUN_VEHICULE )
      	          {
      	             pl_listeAnaly->Mes.VitesseVoie[vl_Index] = 0;
  /*!! Suppression de la relance de la config si aucun vehicule 
                     pl_listeAnaly->EtatConfVoie[vl_Index+1] = EDAIC_CONF_DEBUT;
                     if ( pl_listeAnaly->EtatConf == EDAIC_CONF_FIN ) 
                         pl_listeAnaly->EtatConf = EDAIC_CONF_DEBUT; */
      	          }
      	       }
      	       else
      	       {
      	          pl_listeAnaly->Mes.VitesseVoie[vl_Index] = EDAIC_MES_INCONNU;
  /*!! Suppression de la relance de la config si aucun vehicule 
      	          if ( (atoi(vl_chaine) != EDAIC_AUCUN_VEHICULE) && (vl_Index+1 < pl_listeAnaly->Config.NbVoiesGerees) )
      	          {
                     pl_listeAnaly->EtatConfVoie[vl_Index+1] = EDAIC_CONF_DEBUT;
                     if ( pl_listeAnaly->EtatConf == EDAIC_CONF_FIN ) 
                         pl_listeAnaly->EtatConf = EDAIC_CONF_DEBUT;
      	          } */
      	       }
      	    }
      	    pl_listeAnaly->Mes.VitesseVoie[4] = EDAIC_MES_INCONNU;
            for (vl_Index = 0; vl_Index < EDAIC_NB_VOIE; vl_Index ++)
            {
      	       strncpy(vl_chaine,pl_trame+24+(vl_Index*3),3);
      	       vl_chaine[3]='\0';
      	       if ( pl_listeAnaly->Mes.VitesseVoie[vl_Index] != EDAIC_MES_INCONNU )
      	          pl_listeAnaly->Mes.LongueurVoie[vl_Index] = atoi(vl_chaine);
      	       else
      	          pl_listeAnaly->Mes.LongueurVoie[vl_Index] = EDAIC_MES_INCONNU;
      	    }
      	          	    
      	    strncpy(vl_chaine,pl_trame+39,2);
      	    vl_chaine[2]='\0';
      	    pl_listeAnaly->Mes.TO=atoi(vl_chaine);
      	    
      	    strncpy(vl_chaine,pl_trame+43,3);
      	    vl_chaine[3]='\0';
      	    pl_listeAnaly->Mes.VitesseMoyenne=atoi(vl_chaine);
      	    
                     
            vl_Heures  = atoi ( &pl_Horodatedecompo[11] );
            vl_Minutes = atoi ( &pl_Horodatedecompo[14] );
            vl_Result_Div = ldiv ( vl_Minutes, 6 );
            if ( pl_listeAnaly->MemTrace[vl_Result_Div.quot] != vl_Heures )
            {
               pl_listeAnaly->MemTrace[vl_Result_Div.quot] = vl_Heures;
               sprintf ( pl_PathEtFic, "%s/Enregistrement_DAI", XZEXC_PATH_CONFIG );
               if ( (pl_Fd = fopen ( pl_PathEtFic, "r" )) != NULL )
               {
                  while ( !(feof(pl_Fd)) ) /*LINUX*/
                  {
                     fgets ( pl_Ligne, sizeof(XDY_Texte), pl_Fd );
                     if ( (pl_Ligne[0] != XDF_COMMENTAIRE) && (vl_Analyseur == atoi(pl_Ligne)) )
                     {
                        vl_ValidTrace = XDC_OK;
                        break;
                     }
                  }
                  fclose (pl_Fd);
               }
            }
            
            
            if ( vl_ValidTrace == XDC_OK )
            {
               if ( access (CM_EDAI_PATH_TRA_MES, F_OK) )
                  mkdir ( CM_EDAI_PATH_TRA_MES, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
               sprintf ( pl_PathEtFic, "%s/%02d%02d%s.ASC", CM_EDAI_PATH_TRA_MES, atoi(pl_Horodatedecompo), vl_Analyseur, &vg_NomMachine[4] );
               if ( (vl_Heures == 0) && (vl_Result_Div.quot == 0) )
                   XZSS_06DetruireFichier ( pl_PathEtFic );
                   
               if ( (pl_Fd = fopen ( pl_PathEtFic, "a+" )) != NULL )
               {
                  sprintf ( &pl_Horodatedecompo[14], "%02ld:00", vl_Result_Div.quot * 6 );
                  fprintf (pl_Fd , "%s;", pl_Horodatedecompo );
                  for ( vl_Index = 0; vl_Index < 2*EDAIC_NB_VOIE; vl_Index ++)
                  {
      	             strncpy(vl_chaine,pl_trame+9+(vl_Index*3),3);
      	             vl_chaine[3]='\0';
                     fprintf (pl_Fd , "%s;", ((atoi(vl_chaine) == 999) || (atoi(vl_chaine) == 255)) ? "000" : vl_chaine );
                  }
      	          vl_chaine[0] = *(pl_trame+41);
      	          vl_chaine[1]='\0';
                  fprintf (pl_Fd , "%02d;%s;", pl_listeAnaly->Mes.TO, vl_chaine );
      	          vl_chaine[0] = *(pl_trame+42);
      	          vl_chaine[1]='\0';
                  fprintf (pl_Fd , "%s;", vl_chaine );
      	          strncpy(vl_chaine,pl_trame+46,2);
      	          vl_chaine[2]='\0';
                  fprintf (pl_Fd , "%03d;%s\n", pl_listeAnaly->Mes.VitesseMoyenne, vl_chaine );
                  
                  fclose (pl_Fd);
               }
             }
            }
            break;
	 
         /*A C'est une trame status complet */
	 case XZEXC_TRAME_CONF_DAI:
	    
            XZST_03EcritureTrace(XZSTC_WARNING,"XZEXC_TRAME_conf_dai %d",vl_typ_rep);
            /*B Perte de connection DAI */
	   if(vl_typ_rep == XZEXC_PERTE_LIAISON_DAI) {
	     /*connection to the other IP for the DAI*/
	      XZSM_07LireHorodate( XZSMC_TYPE_RTWORKS , &vl_js , &vl_horodate2 );
            XZST_03EcritureTrace(XZSTC_WARNING,"%lf-%lf=%lf",vl_horodate2, vg_last_switch,vl_horodate2-vg_last_switch);
	     if (vl_horodate2-vg_last_switch> (double)90)
            {
	     vg_last_switch = vl_horodate2;
            XZST_03EcritureTrace(XZSTC_WARNING,"SWITCH DAI");
	      switchDAI(vl_adrrgs);
	     }
	     break;
	   }
            /*B Recherche du parametre Cmd */
            if ( (pl_deb=strstr(pl_trame,"Cmd=")) == NULL)
               pl_deb=strstr(pl_trame,"cmd=");
            XZST_03EcritureTrace(XZSTC_INFO,"trame = <%s>", pl_deb);
            /*B Recherche du No DAI concernÅ */
            for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
            {
               if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	            if (!strcmp(pl_listeDAI->Config.AdresseRGS,vl_adrrgs ))
	                 {
	                    break;
	                   }
            }
            if (pl_listeDAI == NULL) return (XDC_OK);

            /*B Si reponse ACK ou absence du parametre Cmd alors commande en cours */
            if ( pl_deb == NULL )
            { 
               /* Ecriture dans fichier Trace_CMD_DAI de la config voie */    
   	       if ( vl_typ_rep == XZEXC_REP_NAK ) sprintf ( pl_mlcr, "%s> <Reponse NAK", strtok(pl_trame, "~\n") ); 
               if ( vl_typ_rep == XZEXC_REP_ACK ) sprintf ( pl_mlcr, "%s> <Reponse ACK", strtok(pl_trame, "~\n") ); 
               if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )  sprintf ( pl_mlcr, "Pas de reponse pour \"%s\"", strtok(pl_trame, "~\n")); 
               ex_ecriture_trace_cmd (  XDC_EQT_DAI,
                                        pl_listeDAI->Config.Numero, 
                                	NULL,
                                	pl_mlcr,
                                	vl_nocmd );
               if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
                  break; 
            }
            /*B Sinon recuperation du parametre Cmd */
            else
            {
               vl_RetCmd = atoi(pl_deb + 4);
               if ( vl_nocmd < vg_NumConf[pl_listeDAI->Config.Numero] )
                  break;
               else
                  if ( vg_NumConf[pl_listeDAI->Config.Numero] > 32000 ) vg_NumConf[pl_listeDAI->Config.Numero] = 0;
               pl_listeDAI->EtatTrans = ((vl_RetCmd == EDAIC_NOCMD_COURS_OK) || (vl_RetCmd == EDAIC_NOCMD_COURS_NOK)) ? EDAIC_EQT_LIBRE : EDAIC_EQT_OCCUP;
            }
                                	            
            if ( (pl_deb == NULL) || (pl_listeDAI->EtatTrans != EDAIC_EQT_LIBRE) )
            {
               /*B Envoyer trame a la tache TELMi */
               sleep(2);
               sprintf(pl_mlcr,"ST ID=%s", vg_idpassw);
               XZST_03EcritureTrace(XZSTC_DEBUG1,"%s : Envoi trame = <%s>", version, pl_mlcr );
	        XZST_03EcritureTrace(XZSTC_WARNING,"JMG 2");
               if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
             		EnvoyerTrame (   pl_mlcr,
	        		        XZEXC_TRAME_CONF_DAI,
	        		        pl_listeDAI->Config.AdresseRGS,
			 	        ++vg_NumConf[pl_listeDAI->Config.Numero],
			 	        XZEXC_PRIORITEFAIBLE,
	        		        pl_listeDAI->SocketMaitre );
		else
             		EnvoyerTrameIP2 (   pl_mlcr,
					   XZEXC_TRAME_CONF_DAI,
					   pl_listeDAI->Config.AdresseIP,
					   pl_listeDAI->Config.Port,
					   pl_listeDAI->Config.Adresse2IP,
					   pl_listeDAI->Config.Port2,
					   pl_listeDAI->Config.DAIBis,
					   pl_listeDAI->Config.AdresseRGS,
					   ++vg_NumConf[pl_listeDAI->Config.Numero],
					   XZEXC_PRIORITEFAIBLE,
					   pl_listeDAI->SocketMaitre );

               pl_listeDAI->EtatTrans = EDAIC_EQT_OCCUP;
               break;
            }
            

               if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
            {   
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {   
               if ((pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero) && 
                   (pl_listeAnaly->EtatConf != EDAIC_CONF_FIN) )
               {
                  /*A envoi de la config a l'equipement si celui-ci est en service */
                  if ( !(pl_listeAnaly->Etat_Service & XDC_EQT_HORS_SRV) && 
                       !(pl_listeAnaly->Etat_Service & XDC_EQT_INACTIF) &&
                       (pl_listeDAI->EtatTrans == EDAIC_EQT_LIBRE) )
                  {     
                     /*B Composition des trames LCR de config voie DAI pour ttes les voies */
                     for (vl_Index = 0; vl_Index < pl_listeAnaly->Config.NbVoiesGerees; vl_Index ++)
                     {
	                if ( (pl_listeAnaly->EtatConfVoie[vl_Index] != EDAIC_CONF_FIN) && 
	                     (pl_listeAnaly->EtatVoie[vl_Index] != XDC_VOIE_INEXISTANTE) )
                        {
 	                    /* si la voie etait neutralisee : deneutralisation */
 	                    if ( (pl_listeAnaly->EtatConfVoie[vl_Index] == EDAIC_CONF_DEBUT) &&
 	                         (pl_listeAnaly->EtatVoie[vl_Index] != XDC_VOIE_NEUTRALISEE) )
 	                    {
	                       sprintf(pl_mlcr,"CFSV ID=%s A%02d V%d=%s",
	                               vg_idpassw,
	                               pl_listeAnaly->Config.NoAnaly,
	   	                       vl_Index,
	   	                       (pl_listeAnaly->EtatVoie[vl_Index]==XDC_VOIE_SENS_INVERSE) ? "I" : "N" );
	   						
                               XZST_03EcritureTrace(XZSTC_INFO,"%s : trame = <%s>", version, pl_mlcr);

	                       /*A Envoyer trame a la tache TELMi */
                          	if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)){
	                               if (EnvoyerTrame(pl_mlcr,
	        		                          XZEXC_TRAME_CONF_DAI,
	        		                          pl_listeDAI->Config.AdresseRGS,
			 	                          XZEXC_NOCMD,
			 	                          XZEXC_PRIORITEFAIBLE,
	        		                          pl_listeDAI->SocketMaitre) != XDC_OK ) 
	        		       {
	                                  XZST_03EcritureTrace(XZSTC_WARNING,"%s : Impossible d'envoyer la trame = <%s> vers TELMI" , version, pl_mlcr);
	   		                  return (XDC_NOK);
				       }        
				}
				else
				{
	                               if (EnvoyerTrameIP2(pl_mlcr,
							   XZEXC_TRAME_CONF_DAI,
							   pl_listeDAI->Config.AdresseIP,
							   pl_listeDAI->Config.Port,
							   pl_listeDAI->Config.Adresse2IP,
							   pl_listeDAI->Config.Port2,
							   pl_listeDAI->Config.DAIBis,
							   pl_listeDAI->Config.AdresseRGS,
							   XZEXC_NOCMD,
							   XZEXC_PRIORITEFAIBLE,
							   pl_listeDAI->SocketMaitre) != XDC_OK ) 
	        		       {
	                                  XZST_03EcritureTrace(XZSTC_WARNING,"%s : Impossible d'envoyer la trame IP = <%s> vers TELMI" , version, pl_mlcr);
	   		                  return (XDC_NOK);
				       }        
				}
   		               pl_listeDAI->EtatTrans = EDAIC_EQT_OCCUP;
                               /* Ecriture dans fichier Trace_CMD_DAI de la config voie */    
               		       ex_ecriture_trace_cmd (  	XDC_EQT_DAI,
                                	 			pl_listeDAI->Config.Numero, 
                                				NULL,
                                				strtok (pl_trame, "~\n"),
                                				vl_nocmd );
                               ex_ecriture_trace_cmd ( 	XDC_EQT_DAI,
                                	                        pl_listeDAI->Config.Numero, 
                                	                        pl_mlcr,
                                	                        NULL,
                                	                        vl_RetCmd );
   		             
                                pl_listeAnaly->EtatConf = EDAIC_CONF_ATTENTE;
                                pl_listeAnaly->EtatConfVoie[vl_Index] = EDAIC_CONF_ATTENTE;
                                return (XDC_OK);
 	                     }
 	          
	                     sprintf( pl_mlcr,"CFIV ID=%s A%02d V%d=%s",
	                              vg_idpassw,
	                              pl_listeAnaly->Config.NoAnaly,
	   	                      vl_Index,
	   	                      (pl_listeAnaly->EtatVoie[vl_Index] == XDC_VOIE_NEUTRALISEE ) ? "F" : "O" );
 	    	
	                     /*B Envoyer trame a la tache TELMi */
                             XZST_03EcritureTrace(XZSTC_INFO,"%s : Envoi trame = <%s>", version, pl_mlcr );
                          	if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)){
	                             if ( EnvoyerTrame(pl_mlcr,
	        		                       XZEXC_TRAME_CONF_DAI,
	        		                       pl_listeDAI->Config.AdresseRGS,
			 	                       XZEXC_NOCMD,
			 	                       XZEXC_PRIORITEFAIBLE,
	        		                       pl_listeDAI->SocketMaitre) != XDC_OK )
	        		     {
	                                 XZST_03EcritureTrace(XZSTC_WARNING,"%s : Impossible d'envoyer la trame = <%s> vers TELMI" , version, pl_mlcr);
	   		                 return (XDC_NOK);
				     }
				}
				else
				{
	                             if ( EnvoyerTrameIP2(pl_mlcr,
							  XZEXC_TRAME_CONF_DAI,
							  pl_listeDAI->Config.AdresseIP,
							  pl_listeDAI->Config.Port,
							  pl_listeDAI->Config.Adresse2IP,
							  pl_listeDAI->Config.Port2,
							  pl_listeDAI->Config.DAIBis,
							  pl_listeDAI->Config.AdresseRGS,
							  XZEXC_NOCMD,
							  XZEXC_PRIORITEFAIBLE,
							  pl_listeDAI->SocketMaitre) != XDC_OK )
	        		     {
	                                 XZST_03EcritureTrace(XZSTC_WARNING,"%s : Impossible d'envoyer la trame IP = <%s> vers TELMI" , version, pl_mlcr);
	   		                 return (XDC_NOK);
				     }
				}


                             /* Ecriture dans fichier Trace_CMD_DAI de la config voie */    
               		     ex_ecriture_trace_cmd (  	XDC_EQT_DAI,
                                	 		pl_listeDAI->Config.Numero, 
                                			NULL,
                                			strtok (pl_trame, "~\n"),
                                			vl_nocmd );
                             ex_ecriture_trace_cmd ( 	XDC_EQT_DAI,
                                	                pl_listeDAI->Config.Numero, 
                                	                pl_mlcr,
                                	                NULL,
                                	                vl_RetCmd );
                                	                
                             pl_listeDAI->EtatTrans = EDAIC_EQT_OCCUP;                       
                             pl_listeAnaly->EtatConfVoie[vl_Index] = EDAIC_CONF_FIN;
                             for (vl_Index = 0; vl_Index < pl_listeAnaly->Config.NbVoiesGerees; vl_Index ++)
                                if ( pl_listeAnaly->EtatConfVoie[vl_Index] != EDAIC_CONF_FIN )
                                   vl_fin = XDC_FAUX;
                              if (vl_fin == XDC_VRAI) 
                                 pl_listeAnaly->EtatConf = EDAIC_CONF_FIN;
                              else
                                 pl_listeAnaly->EtatConf = EDAIC_CONF_ATTENTE;
                             return (XDC_OK);
                       }
                    }
                 }
              }
           }
           }
           break;
            
         
         /*A C'est une trame status complet */
	 case XZEXC_TRAME_TEST_COM :
	 case XZEXC_TRAME_STAT_COMP:
	 {
            
XZST_03EcritureTrace(XZSTC_INFO,"%s : ex_env_alarme  trame:%s",version,pl_trame);
            /*B Traitement eventuel d'un probleme video */
            if ( (pl_deb = strstr(pl_trame,"Vid=")) == NULL )
          	  pl_deb = strstr(pl_trame,"vid=");
	    /*LINUX*/
	    if (pl_deb!=NULL)  {
            sscanf(pl_deb,"%s",vl_vid);
            
            /* Recherche de l'analyseur concernÅ */
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {
               /* Pour ts les analyseur de la dai */
               if (pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero)
               {
                  vl_trouve = XDC_FAUX;
                     
                  if (pl_deb != NULL) 
                  {
                     for(vl_nbana=1;vl_nbana<=((strlen(&vl_vid[4])+1)/3);vl_nbana++)
                     {
      	                strncpy(vl_chaine,&vl_vid[(3*vl_nbana)+1],2);
      	                vl_chaine[2]='\0';
      	                vl_Analyseur=atoi(vl_chaine);
      	                
      	                if (vl_Analyseur==pl_listeAnaly->Config.NoAnaly)
      	                {
      	                   vl_trouve = XDC_VRAI;
      	                }
                     }
                  }
      	          if (vl_trouve == XDC_VRAI)
      	          {
      	             vl_al=XDC_VRAI;
      	          }
      	          else
      	          {
      	             vl_al=XDC_FAUX;
      	          }
                  /* Envoi de l'alarme */
                  /*if ( !pl_listeDAI->Etat_Service && !pl_listeAnaly->Etat_Service )*/
                  if ( !pl_listeDAI->Etat_Service && (!pl_listeAnaly->Etat_Service || !vl_al) )
                     ex_env_alarme  ( 	vg_NomMachine,
			                vl_horodate,
			                XZECC_TYPE_EQT_ANA,
					pl_listeAnaly->Config.Numero,
					XZAMC_DEF_VIDEO,
					vl_al,
					NULL,
					pl_listeDAI->Config.SiteGestion);
               }
             }
            }
            
            if ( (pl_deb = strstr(pl_trame,"Trm=")) == NULL )
            	pl_deb = strstr(pl_trame,"trm=");
	    if (pl_deb!=NULL) 
            sscanf(pl_deb,"%s",vl_trm);
            
            /* Recherche de l'analyseur concernÅ */
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {
               /* Pour ts les analyseur de la dai */
               if (pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero)
               {
                  vl_trouve = XDC_FAUX;
                     
                  if (pl_deb != NULL) 
                  {
                     for(vl_nbana=1;vl_nbana<=((strlen(&vl_trm[4])+1)/3);vl_nbana++)
                     {
      	                strncpy(vl_chaine,&vl_trm[(3*vl_nbana)+1],2);
      	                vl_chaine[2]='\0';
      	                vl_Analyseur=atoi(vl_chaine);
      	                
      	                if (vl_Analyseur==pl_listeAnaly->Config.NoAnaly)
      	                {
      	                   vl_trouve = XDC_VRAI;
      	                }
                     }
                  }
      	          if (vl_trouve == XDC_VRAI)
      	          {
      	             vl_al=XDC_VRAI;
      	          }
      	          else
      	          {
      	             vl_al=XDC_FAUX;
      	          }
                  /* Envoi de l'alarme */
                  if ( !pl_listeDAI->Etat_Service && (!pl_listeAnaly->Etat_Service || !vl_al) )
{XZST_03EcritureTrace(XZSTC_INFO,"%s : ex_env_alarme  ana:%d trm:%d",version,pl_listeAnaly->Config.Numero,vl_al);
                     ex_env_alarme  ( 	vg_NomMachine,
			                vl_horodate,
			                XZECC_TYPE_EQT_ANA,
					pl_listeAnaly->Config.Numero,
					XZAMC_DEF_TRANS,
					vl_al,
					NULL,
					pl_listeDAI->Config.SiteGestion);
               }
               }
            }
            
            if ( (pl_deb = strstr(pl_trame,"Ana=")) == NULL)
          	  pl_deb = strstr(pl_trame,"ana=");
	    if (pl_deb!=NULL) 
            sscanf(pl_deb,"%s",vl_ana);
            /* Recherche de l'analyseur concernÅ */
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {
               /* Pour ts les analyseur de la dai */
               if (pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero)
               {
                  vl_trouve = XDC_FAUX;
                     
                  if (pl_deb != NULL) 
                  {
                     for(vl_nbana=1;vl_nbana<=((strlen(&vl_ana[4])+1)/3);vl_nbana++)
                     {
      	                strncpy(vl_chaine,&vl_ana[(3*vl_nbana)+1],2);
      	                vl_chaine[2]='\0';
      	                vl_Analyseur=atoi(vl_chaine);
      	                
      	                if (vl_Analyseur==pl_listeAnaly->Config.NoAnaly)
      	                {
      	                   vl_trouve = XDC_VRAI;
      	                }
                     }
                  }
      	          if (vl_trouve == XDC_VRAI)
      	          {
      	             vl_al=XDC_VRAI;
      	          }
      	          else
      	          {
      	             vl_al=XDC_FAUX;
      	          }
                  /* Envoi de l'alarme */
                  /*if ( !pl_listeDAI->Etat_Service && !pl_listeAnaly->Etat_Service )*/
                  if ( !pl_listeDAI->Etat_Service && (!pl_listeAnaly->Etat_Service || !vl_al) )
                     ex_env_alarme  ( 	vg_NomMachine,
			                vl_horodate,
			                XZECC_TYPE_EQT_ANA,
					pl_listeAnaly->Config.Numero,
					XZAMC_DEF_ANA_ARR,
					vl_al,
					NULL,
					pl_listeDAI->Config.SiteGestion);
               }
            }
            break;
	 }
	 /*A c'est une trame d'alerte */
	 case XZEXC_TRAME_ALERTE_DAI:
	 {
	    strcpy (vl_autoroute, "XXXX");
	    vl_pr = -1;
	    vl_sens = -1;

	    /*B Acquisition des donnes de la trame */
                  XZST_03EcritureTrace( XZSTC_WARNING, " TRAME %s",pl_trame);
            vl_param=sscanf(pl_trame,"TC E %s %s %s MESU %s %s %d %d AUT=%s PR=%d SENS=%d"
                                    ,vl_horod1
                                    ,vl_horod2
                                    ,vl_codesite
                                    ,vl_novoie
                                    ,vl_alerte
                                    ,&vl_tpsarrveh
                                    ,&vl_coefconf
                                    ,vl_autoroute
                                    ,&vl_pr
                                    ,&vl_sens);
            
            if (vl_param < 7)
            {
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
                  if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	                  EnvoyerTrame(" \0",
	        		    XZEXC_TRAME_TELEI_NREP,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
		 else
	                  EnvoyerTrameIP2(" \0",
					 XZEXC_TRAME_TELEI_NREP,
					 pl_listeDAI->Config.AdresseIPE,
					 pl_listeDAI->Config.PortE,
					 pl_listeDAI->Config.Adresse2IPE,
					 pl_listeDAI->Config.Port2E,
					 pl_listeDAI->Config.DAIBis,
					 pl_listeDAI->Config.AdresseRGS,
					 XZEXC_NOCMD,
					 XZEXC_PRIORITEFAIBLE,
					 pl_listeDAI->SocketEsclave);
	         return (XDC_NOK);
	       }

               strcat(vl_typeal,vl_novoie);
               strcpy(vl_novoie,vl_typeal);
            }

	    XZST_03EcritureTrace( XZSTC_WARNING, "Donnees trame: D:%s %s Site:%s Type:%s V:%s N:%s T:%d CC:%d LOC:%s-%d-%d"
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
            
            /*B Recherche du No DAI concernÅ */
            
	    /*
            for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
            {
          if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)){
          	if (!strcmp(pl_listeDAI->Config.AdresseRGS,vl_adrrgs )) break;
	  }
	  else {
          	if (!strcmp(pl_listeDAI->Config.AdresseIP,vl_adrrgs )) break;
	  }
            }*/
            
      	    strncpy(vl_chaine,&vl_codesite[7],2);
      	    vl_chaine[2]='\0';
      	    vl_Analyseur=atoi(vl_chaine);

            /*B Recherche du No Analsyseur concernÅ */
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {
                if ((pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero) && 
                    (vl_Analyseur == pl_listeAnaly->Config.NoAnaly)) break;
            }
            if (pl_listeAnaly == NULL)
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Analyseur(%d) Non trouve en memoire mes=%s(2)",vl_Analyseur,vl_Msg);
               
               /* Acquitement telei */   
               if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	               EnvoyerTrame(" \0",
        			    XZEXC_TRAME_TELEI_NAK,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
		else
	               EnvoyerTrameIP2(" \0",
				       XZEXC_TRAME_TELEI_NAK,
				       pl_listeDAI->Config.AdresseIPE,
				       pl_listeDAI->Config.PortE,
				       pl_listeDAI->Config.Adresse2IPE,
				       pl_listeDAI->Config.Port2E,
				       pl_listeDAI->Config.DAIBis,
				       pl_listeDAI->Config.AdresseRGS,
				       XZEXC_NOCMD,
				       XZEXC_PRIORITEFAIBLE,
				       pl_listeDAI->SocketEsclave);
               return (XDC_NOK);
            }
            
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
               if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	               EnvoyerTrame(" \0",
        			    XZEXC_TRAME_TELEI_NAK,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
		else
	               EnvoyerTrameIP2(" \0",
				       XZEXC_TRAME_TELEI_NAK,
				       pl_listeDAI->Config.AdresseIPE,
				       pl_listeDAI->Config.PortE,
				       pl_listeDAI->Config.Adresse2IPE,
				       pl_listeDAI->Config.Port2E,
				       pl_listeDAI->Config.DAIBis,
				       pl_listeDAI->Config.AdresseRGS,
				       XZEXC_NOCMD,
				       XZEXC_PRIORITEFAIBLE,
				       pl_listeDAI->SocketEsclave);
                return (XDC_NOK);
            }
	    
            /*B Si c'est une debut d'alerte -> envoi d'alerte */
            if ( (vl_chaine[0] == 'O') && (vl_NumAlerte > 0) && (pl_listeAnaly->DernAlerte != vl_NumAlerte)  )
            {
            	pl_listeAnaly->DernAlerte = vl_NumAlerte;
                if ( !pl_listeDAI->Etat_Service && !pl_listeAnaly->Etat_Service )
                {
                    if ( vl_sens > 0 )
                    { 
			ex_env_alerte_localisee  ( vl_horodate,
                                   XDC_EQT_ANA,
                                   pl_listeAnaly->Config.Numero,
                                   vl_NumAlerte,
                                   XZEXC_MODE_NORMAL,
                                   pl_listeDAI->Config.SiteGestion,
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
                                   XDC_EQT_ANA,
                                   pl_listeAnaly->Config.Numero,
                                   vl_NumAlerte,
                                   XZEXC_MODE_NORMAL,
                                   pl_listeDAI->Config.SiteGestion,
                                   vl_Voie,
                                   vl_tpsarrveh,
                                   vl_coefconf,
                                   NULL );
                    } 
		}

		if ( (vl_TypeAlerte) && (pl_listeAnaly->Alerte == CM_EDAI_PASDALERTE) )
            	{
                  pl_listeAnaly->Alerte = CM_EDAI_ALERTE;
                  /* Appel recherche alerte DAI Tunnel (ec_detect_Alerte_DAI_Tunnel) */
                  ec_detect_Alerte_DAI_Tunnel ( pl_listeAnaly, vl_horodate );
                  /* Ecriture dans fichier Trace_CMD_DAI de l'info alerte */    
               	  ex_ecriture_trace_cmd (  	XDC_EQT_DAI,
                                		pl_listeDAI->Config.Numero, 
                                		(pl_listeAnaly->EtatVoie[vl_Voie] != XDC_VOIE_NEUTRALISEE) ? NULL : "Alerte sur voie neutralisee",
                                		strtok (pl_trame, "~\n"),
                                		vl_Analyseur );
               }
               else
               {
               	  ex_ecriture_trace_cmd (  	XDC_EQT_DAI,
                                		pl_listeDAI->Config.Numero, 
                                		NULL,
                                		strtok (pl_trame, "~\n"),
                                		vl_Analyseur );
               }
            }
         
            /*B Memorisation de fin d'alerte */
            if ( vl_chaine[0] == 'F' )
            {
               pl_listeAnaly->DernAlerte = 0;
               if ( vl_TypeAlerte )
               {
                  pl_listeAnaly->Alerte = CM_EDAI_PASDALERTE;
                  ec_detect_Alerte_DAI_Tunnel ( pl_listeAnaly, vl_horodate );
               }
            }
               
            /* Acquitement telei */   
            if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	               EnvoyerTrame(" \0",
        			    XZEXC_TRAME_TELEI_NAK,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
  	    else
	               EnvoyerTrameIP2(" \0",
				       XZEXC_TRAME_TELEI_NAK,
				       pl_listeDAI->Config.AdresseIPE,
				       pl_listeDAI->Config.PortE,
				       pl_listeDAI->Config.Adresse2IPE,
				       pl_listeDAI->Config.Port2E,
				       pl_listeDAI->Config.DAIBis,
				       pl_listeDAI->Config.AdresseRGS,
				       XZEXC_NOCMD,
				       XZEXC_PRIORITEFAIBLE,
				       pl_listeDAI->SocketEsclave);
            break;
	 }
         /*A c'est une trame de status */
         case XZEXC_TRAME_STAT_SYS:
         {
            /*B Acquisition des donnees de la trame */
            vl_param=sscanf(pl_trame,"TC E %s %s %s SYST %s ERR=%d"
                                    ,vl_horod1
                                    ,vl_horod2
                                    ,vl_codesite
                                    ,vl_debfin
                                    ,&vl_noerr);
            if (vl_param != 5)
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Trame dai incomprehensible mes=%s",vl_Msg);
               
               /* Acquitement telei */   
            if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	               EnvoyerTrame(" \0",
        			    XZEXC_TRAME_TELEI_NREP,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
  	    else
	               EnvoyerTrameIP2(" \0",
				       XZEXC_TRAME_TELEI_NREP,
				       pl_listeDAI->Config.AdresseIP,
				       pl_listeDAI->Config.Port,
				       pl_listeDAI->Config.Adresse2IP,
				       pl_listeDAI->Config.Port2,
				       pl_listeDAI->Config.DAIBis,
				       pl_listeDAI->Config.AdresseRGS,
				       XZEXC_NOCMD,
				       XZEXC_PRIORITEFAIBLE,
				       pl_listeDAI->SocketEsclave);
               return (XDC_NOK);
            }
	    
            /*B Recherche du No DAI concernÅ */
            
            for ( pl_listeDAI = pg_debutDAI; pl_listeDAI != NULL; pl_listeDAI = pl_listeDAI->Suivant )
            {
/*          if (!strcmp(pl_listeDAI->Config.AdresseRGS,vl_adrrgs )) break;
*/
          if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE)){
          	if (!strcmp(pl_listeDAI->Config.AdresseRGS,vl_adrrgs )) break;
	  }
	  else {
          	if (!strcmp(pl_listeDAI->Config.AdresseIP,vl_adrrgs )) break;
	  }
            }
            
      	    strncpy(vl_chaine,&vl_codesite[7],2);
      	    vl_chaine[2]='\0';
      	    vl_Analyseur=atoi(vl_chaine);
      	    
            /*B Recherche du No Analyseur concernÅ */
            for ( pl_listeAnaly = pg_debutAnaly; pl_listeAnaly != NULL; pl_listeAnaly = pl_listeAnaly->Suivant )
            {
                if ((pl_listeAnaly->Config.NumeroDAI == pl_listeDAI->Config.Numero) && 
                    (vl_Analyseur == pl_listeAnaly->Config.NoAnaly)) break;
            }
            if (pl_listeAnaly == NULL)
            {
               XZST_03EcritureTrace( XZSTC_WARNING, " Analyseur(%d) Non trouve en memoire mes=%s(3)",vl_Analyseur,vl_Msg);

            /* Acquitement telei */   
            if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	               EnvoyerTrame("\0",
        			    XZEXC_TRAME_TELEI_NAK,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
  	    else
	               EnvoyerTrameIP2(" \0",
				       XZEXC_TRAME_TELEI_NAK,
				       pl_listeDAI->Config.AdresseIP,
				       pl_listeDAI->Config.Port,
				       pl_listeDAI->Config.Adresse2IP,
				       pl_listeDAI->Config.Port2,
				       pl_listeDAI->Config.DAIBis,
				       pl_listeDAI->Config.AdresseRGS,
				       XZEXC_NOCMD,
				       XZEXC_PRIORITEFAIBLE,
				       pl_listeDAI->SocketEsclave);
               return (XDC_NOK);
            }
            
            /*B Si c'est un debut d'un pb systeme -> envoi d'une alarme */
            vl_trouve = XDC_FAUX;
            for ( vl_indtaberr=0;vl_indtaberr<NB_ERR_POSSI;vl_indtaberr++)
            {
               if (vl_noerr==pl_listeAnaly->TabErrSyst[vl_indtaberr])
               {
                  vl_trouve = XDC_VRAI;
               }
            }
            if ((vl_trouve == XDC_FAUX) && (!strcmp(vl_debfin,"O")))
            {
               /* envoyer alarme*/
               for ( vl_indtaberr=0;vl_indtaberr<NB_ERR_POSSI;vl_indtaberr++)
               {
                  if (pl_listeAnaly->TabErrSyst[vl_indtaberr]==0) 
                  {
                     pl_listeAnaly->TabErrSyst[vl_indtaberr]=vl_noerr;
                     break;
                  }
               }
               for ( vl_indtaberr=0;vl_indtaberr<NB_ERR_POSSI;vl_indtaberr++)
               {
                  XZST_03EcritureTrace( XZSTC_DEBUG1, "%d ",pl_listeAnaly->TabErrSyst[vl_indtaberr]);
                   
               }
            }
            
            /*B Memorisation d'une fin d'alarme et envoi de fin d'alarme */
            if (!strcmp(vl_debfin,"F"))
            {
               /* envoyer fin alarme */
               for ( vl_indtaberr=0;vl_indtaberr<NB_ERR_POSSI;vl_indtaberr++)
               {
                  pl_listeAnaly->TabErrSyst[vl_indtaberr]=0;
                   
               }
            }
            
            /* Acquitement telei */   
            if (!strcmp(pl_listeDAI->Config.AdresseIP, XZECC_IP_VIDE))
	               EnvoyerTrame("\0",
        			    XZEXC_TRAME_TELEI_ACK,
	        		    pl_listeDAI->Config.AdresseRGS,
			 	    XZEXC_NOCMD,
			 	    XZEXC_PRIORITEFAIBLE,
	        		    pl_listeDAI->SocketEsclave);
  	    else
	               EnvoyerTrameIP2(" \0",
				       XZEXC_TRAME_TELEI_ACK,
				       pl_listeDAI->Config.AdresseIP,
				       pl_listeDAI->Config.Port,
				       pl_listeDAI->Config.Adresse2IP,
				       pl_listeDAI->Config.Port2,
				       pl_listeDAI->Config.DAIBis,
				       pl_listeDAI->Config.AdresseRGS,
				       XZEXC_NOCMD,
				       XZEXC_PRIORITEFAIBLE,
				       pl_listeDAI->SocketEsclave);
         } 
      }
   }  
   XZST_03EcritureTrace( XZSTC_FONCTION, " OUT : %s", version );

   return (XDC_OK);
}
