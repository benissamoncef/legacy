/*E*/
/*Fichier : $Id: enav_tra.c,v 1.34 2020/11/03 18:09:41 pc2dpdy Exp $      Release : $Revision: 1.34 $        Date : $Date: 2020/11/03 18:09:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant de l'eqt
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	24 Nov 1994	: Creation
* Nagiel.E	version 1.2	01 Dec 1994	: Suppression des init des tests unitaires
* Nagiel.E	version 1.3	21 Dec 1994	:
* Nagiel.E	version 1.4	21 Dec 1994	: RAS
* Mismer.D	version 1.5	16 Jan 1995	:
* Mismer.D	version 1.6	16 Jan 1995	: RAS
* Mismer.D	version 1.7	23 Jan 1995	:
* Mismer.D 	version 1.8	28 Jan 1995	:
* Mismer.D	version 1.9	09 Fev 1995	:
* Mismer.D	version 1.10	08 Mar 1995	:
* Mismer.D	version 1.11	09 Mar 1995	:
* Mismer.D	version 1.12	09 Mar 1995	: RAS
* Mismer.D	version 1.13	17 Mar 1995	:
* Mismer.D	version 1.14	09 Avr 1995	:
* Mismer.D	version 1.15	02 Mai 1995	:
* Volcic.F	version 1.16	10 Mai 1995	: Correction Compte Rendu Action Fin 
* Volcic.F	version 1.17	15 Mai 1995	: Correction retour commande manuelle,
*						  Reprise historique
* Mismer.D	version 1.18	07 Jun 1995	: Correction traitement status
* Mismer.D	version 1.19	22 Sep 1995	: Ajout envoi etat matrice NAV
* Volcic.F	version 1.20	?? ??? 1995	:
* Volcic.F	version 1.21	?? ??? 1995	:
* Mismer.D	version 1.22	02 Oct 1995	: Correction traitement etat matrice
* Mismer.D	version 1.23	27 Nov 1995	: Suppression trame de test matrice 
* Mismer.D	version 1.24	19 Fev 1996	: Ajout un PS dans fichier trace des commandes DEM/1036
* Mismer.D	version 1.25	30 Mai 1996	: Ajout appel fonction compteur de marche XZMA01 (DEM/643)
*						: Envoi cmd EC= si pas scenario et discordance (DEM/1136)
* Mismer.D	version 1.26	10 Jan 1997	: Modif pour matrice BRA (DEM/1419)
* Mismer.D	version 1.27	10 Jan 1998	: Modif pour FAU et FAC (DEM/1544)
* Cluchague     version 1.28	12 Oct 2007     : On n'autorise le retour que de 5 caracteres sur AF="..." (dem 713) 
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI		version 1.30	07/07/2008	: Envoi d'une alerte en cas d'alarme EDF v1.31 DEM788
* PNI		version 1.32	11/12/2009	: Supression de l'envoi d'alerte sur contraste v1.32 DEM922
* PNI		version 1.33	07/10/2019	: Ajou traces 1.33
* JMG		version 1.34	15/05/20 : EMS 1.34
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "enav_tra.h"

/* definitions de constantes */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: enav_tra.c,v 1.34 2020/11/03 18:09:41 pc2dpdy Exp $ : enav_tra.c" ;

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
   static char *version = "$Id: enav_tra.c,v 1.34 2020/11/03 18:09:41 pc2dpdy Exp $ : em_traitement_trame" ;

   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame_ptr,*pl_trame_ps_ptr,*pl_PtrContraste;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   int				vl_typetrame;
   int 				vl_typ_rep;
   XZEXT_MSG_SOCKET		vl_Msg, pl_trame, pl_TrameContraste;
   int				vl_param,i,j;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   XDY_NomPicto			vl_etatNAV[XZEXC_nb_module_max];
   char				vl_poub[16] = "", 
   				vl_pos[16]  = "";
   ENAV_DONNEES_NAV		*pl_ListeNAV;
   XDY_Datagroup		pl_DG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   char * 			vl_status[32];
   XDY_Mot	                vl_noeqt;
   XDY_Mot	                vl_EtatEqt    = XDC_NOK;
   XDY_Mot	                vl_EtatModule = XDC_NOK;
   int 				vl_valret;
   int				vl_Alerte = 0;
#ifndef _TIBCO_EMS
   static char *		pl_Tab[XZEXC_nb_module_max];
#else
   XDY_Nom 		pl_Tab[XZEXC_nb_module_max];
#endif
   static XZEXT_TEXTE_STATUS	vl_tabstatus[24];
   char   			vl_TexteAlerte[10] = "";
   XZEXT_Trame			vl_TexteMatrice = "";
   XZEXT_Trame			vl_MAT[ENAVC_NB_MATRICE] = {"", ""};
   char *			vl_Mot;
   int				vl_indexParam = 0,
   				vl_index      = 0;
   int				vl_SyntModule [10];
/*!!static   */int			vl_MemoModule [10] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
   int				vl_ChgtModule [10];
   int				vl_Module     = 0;
   int				vl_Changement = 0;
   XZSCT_NomTache          	vl_NomTache   = "";
   XDY_NomMachine          	vl_NomMachine = "";
   char				vl_str[1024];   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /*  Ecriture trace Erreur lecture message socket.  */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {
      XZST_03EcritureTrace( XZSTC_DEBUG1, " Msg Telmi :%s. ", vl_Msg );
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
         return;
      }
      					 
      /*A Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      /*A Recherche de la nav concern� */
      for ( pl_ListeNAV = pg_debutNAV; pl_ListeNAV != NULL ; pl_ListeNAV = pl_ListeNAV->Suivant )
      {
         if (!strcmp(pl_ListeNAV->Config.AdresseRGS,vl_adrrgs))
         {
	    vl_noeqt = pl_ListeNAV->Config.Numero;
	    if ( vl_typ_rep != XZEXC_REP_BLC_INT )
	       pl_ListeNAV->EtatTrans = ENAVC_EQT_LIBRE;
	    if ( !pl_ListeNAV->Etat_Service )
	       vl_EtatEqt = XDC_OK;
	    if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max) &&
	         (!pl_ListeNAV->Config.Liste_Module[vl_nocmd-1].Etat_Service) )
	       vl_EtatModule = XDC_OK;
            break;
         }
      }

      /* eqt non trouve */
      if ( pl_ListeNAV == NULL ) 
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " NAV Non trouve en memoire mes=%s",vl_Msg);
         return;
      }

      /*A Traitement des alarmes transmission avec l'equipement */
      if ( !(pl_ListeNAV->Etat_Service & XDC_EQT_HORS_SRV) )
           ex_traite_alarme_com ( vg_NomMachine,
				  XDC_EQT_NAV,
				  vl_noeqt,
      				  vl_typ_rep,
      				  vl_nbnonrep,
      				  vl_nberrcrc,
      				  vl_typetrame,
      				  vg_Mode_Fonct,
				  pl_ListeNAV->Config.SiteGestion);
      				  
      if ( (pl_ListeNAV->Etat_Service & XDC_EQT_HORS_SRV) && (vl_typetrame != XZEXC_TRAME_FEN_LCR) )
          return;
      

      /*A On se positionne sur le debut de la trame */
      pl_trame_ptr=strstr(vl_Msg,XZEXC_SEP_TRAME)+4;
      strcpy ( pl_trame, strtok(pl_trame_ptr,XZEXC_FIN_MSG_SOCK) );
      pl_trame[strlen(pl_trame)-1] = '\0';
      strcpy ( pl_TrameContraste, pl_trame );
      if (pl_trame == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return;
      }

      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {
         /*A C'est une trame de reponse pour la fenetre LCR */
         case XZEXC_TRAME_FEN_LCR:
         {
            /* Appel de ex_mrcr */
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeNAVFLcr, vl_typ_rep );
            break;
         } 
         /*A C'est une trame de reponse status temps reel */
         case XZEXC_TRAME_STAT_TR:
         {
            /*A Si le status TR est ko -> demande du status complet */
            if ( ( ((*pl_trame_ptr != '@')&&(*(pl_trame_ptr+1) != '@')) && (pl_ListeNAV->StatusTR == XDC_OK) ) ||
                 ( ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1) == '@')) && (pl_ListeNAV->StatusTR != XDC_OK) ) ) 
            {
	       
               /*A Si le status TR est ko -> demande du status complet */
	       if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
	       {
	          pl_ListeNAV->EtatTrans = ENAVC_EQT_PANNE;
	          return;
	       }
	       
	       /*A Envoyer demande Status Complet */
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListeNAV->EtatTrans = ENAVC_EQT_OCCUP;
            }
	    pl_ListeNAV->StatusTR = ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1)) == '@') ? XDC_OK : XDC_NOK;
            break;
         }
         /*A C'est une trame de reponse etat de positionnement nav */
         case XZEXC_TRAME_ETAT_POS_NAV:
         {   
            /*A Suivant le type de bloc -> on concatene et on analyse   ou  on shunte si probleme */
            
            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
               strcpy(pl_ListeNAV->TramePS,"");
	       pl_ListeNAV->EtatTrans = ENAVC_EQT_PANNE;
               if ( (vl_EtatModule == XDC_OK) && (vl_EtatEqt == XDC_OK) )
               {
      	          /* Envoi d'une alerte si etat different  */
                   ex_env_alerte  (   	vl_horodate,
                                   	XDC_EQT_MOD,
                                   	pl_ListeNAV->Config.Liste_Module[vl_nocmd-1].NoModule,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeNAV->Config.SiteGestion,
                                    	"Module",
                                    	NULL );
               }

               /*A Mise a jour memoire Cmd Module */
               if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max) ) 
                   pl_ListeNAV->EtatCmd.Module[vl_nocmd-1][0] = '\0';
               break;
            }

            if ((vl_typ_rep==XZEXC_REP_BLC_INT) || (vl_typ_rep==XZEXC_REP_BLC_FIN))
            {
               strcat(pl_ListeNAV->TramePS,pl_trame);
               
               if (vl_typ_rep==XZEXC_REP_BLC_INT) break;
            }
                        
            /*A Si le status TR est ko -> demande du status complet */
            if ( strchr ( pl_ListeNAV->TramePS, '@' ) == NULL ) 
            {	       
	       /*A Envoyer demande Status Complet */
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
                            (*pl_ListeNAV->Etat.Scenario != '\0') ? XZEXC_CMD_HORS_BASE : XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListeNAV->EtatTrans = ENAVC_EQT_OCCUP;
            }
            else
            {	       
	       /*A Envoyer demande Status Complet */
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
                            (*pl_ListeNAV->Etat.Scenario != '\0') ? XZEXC_CMD_HORS_BASE : XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListeNAV->EtatTrans = ENAVC_EQT_OCCUP;
            }
            

            if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max) ) 
	        ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                                        pl_ListeNAV->Config.Numero, 
                                        NULL,
                                        pl_ListeNAV->TramePS,
                                        vl_nocmd );

	    /* Recherche des modules et de leur position */
            for( i=0 ; i < XZEXC_nb_module_max; i++ )
            {  
       	       strcpy(vl_etatNAV[i],"0");
       	       
               /* Recherche ds la trame du module */
               pl_trame_ps_ptr = strstr ( pl_ListeNAV->TramePS, pl_ListeNAV->Config.Liste_Module[i].Module );
               
               /* Module dans la trame -> MAJ etat */
      	       if ( pl_trame_ps_ptr != NULL )
      	       {
      	          /* Extraction de l'etat */
      	          vl_param = sscanf (pl_trame_ps_ptr,"%s AF=%s",vl_poub,vl_pos);
		  vl_pos[XDC_Lon_Scenario-1 ]='\0'; /* DEM713 */
      	          if ( vl_param == 2 ) 
      	          {
      	             /* Suppression des guillemets dans etat du module */
      	             if ( vl_pos[0] == '"' )
      	             {
      	                strcpy ( vl_pos, "1" );
      	             }
      	             
      	             if (!strncmp(pl_trame_ps_ptr-6,"N",1))
      	             {
			strcpy(vl_etatNAV[i], (vl_nocmd == XZEXC_NOCMD) ? "2" : vl_pos);
     	             }
      	             else
      	             {
      	                strcpy(vl_etatNAV[i],vl_pos);
     	             }
      	          }
               }
            }
                        
            /*A Comparaison etat reel et etat commande dans le cas de commande manuelle */
            if ( (vl_EtatModule == XDC_OK) && (vl_EtatEqt == XDC_OK) &&
		 (strcmp(pl_ListeNAV->EtatCmd.Module[vl_nocmd-1],vl_etatNAV[vl_nocmd-1])) )
            {
      	        /* Envoi d'une alerte si etat different  */
                 ex_env_alerte  (   	vl_horodate,
                                   	XDC_EQT_MOD,
                                   	pl_ListeNAV->Config.Liste_Module[vl_nocmd-1].NoModule,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeNAV->Config.SiteGestion,
                                    	"Module",
                                    	NULL );
            }

            /*A Mise a jour memoire Cmd Module */
            if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max) ) 
                pl_ListeNAV->EtatCmd.Module[vl_nocmd-1][0] = '\0';
                
            /*A Comparaison etat reel et etat commande dans le cas de commande contraste */
            if ( (pl_PtrContraste = strstr(  pl_TrameContraste, "EC=" )) != NULL )
            {
	       if ( *(pl_PtrContraste+3) != pl_ListeNAV->ContrasteEtat ) 
	       {
	          /* Envoi XDM_Etat_Contraste  */
	          pl_ListeNAV->ContrasteEtat = *(pl_PtrContraste+3);
	          /* Construction du datagroup */
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, XZSC23_Nom_From_Numero(pl_ListeNAV->Config.SiteGestion)) ;			
                  if (!TipcSrvMsgWrite (  	pl_DG
      			  			,XDM_IdentMsg(XDM_ETAT_Contraste)
  			 			,XDM_FLG_SRVMSGWRITE
                          			,T_IPC_FT_INT2, XDC_EQT_NAV
                          			,T_IPC_FT_INT2, pl_ListeNAV->Config.Numero
                          			,T_IPC_FT_INT2, (pl_ListeNAV->ContrasteEtat == 'S') ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL
                          			,NULL))
      	          {
  		     /* Trace envoi message XDM_ETAT_Contraste non effectue */    	
         	     XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_Contraste non effectue.");
      	          }   
      	          else
      	          {
  		     /* Trace envoi message XDM_ETAT_Contraste effectue */    	
         	     XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_Contraste effectue.");
      	          }
      	       }
               if ( (*pl_ListeNAV->Etat.Scenario == '\0') &&
                    (((pl_ListeNAV->Contraste == XDC_CONTRASTEBRILLANT) && (pl_ListeNAV->ContrasteEtat != 'S')) ||
                     ((pl_ListeNAV->Contraste == XDC_CONTRASTENORMAL) && (pl_ListeNAV->ContrasteEtat != 'J') && (vg_Contexte == XDC_JOUR)) ||
                     ((pl_ListeNAV->Contraste == XDC_CONTRASTENORMAL) && (pl_ListeNAV->ContrasteEtat != 'N') && (vg_Contexte != XDC_JOUR))) )
                  em_envoyer_contraste_NAV ( pl_ListeNAV, pl_ListeNAV->Contraste, XZEXC_NOCMD );
            }
           
              
/**************************************************
DEM 922           if ( (pl_PtrContraste != NULL) &&
                (vl_nocmd == XZEXC_CMD_HORS_BASE) &&
                (vl_EtatEqt == XDC_OK) && 
                (pl_ListeNAV->ContrasteCmd != pl_ListeNAV->ContrasteEtat) &&
                (pl_ListeNAV->ContrasteCmd != XZEXC_NOCMD) )
            {
               ex_env_alerte  ( vl_horodate,
                                XDC_EQT_NAV,
                                pl_ListeNAV->Config.Numero,
                                XZAAC_AFF_DIF,
                                vg_Mode_Fonct,
				pl_ListeNAV->Config.SiteGestion,
                                "Contraste",
                                NULL );
            }
***********************************************************/
            if (vl_nocmd  == XZEXC_CMD_HORS_BASE)
               pl_ListeNAV->ContrasteCmd = XZEXC_NOCMD;          
               

            /*A Comparaison etat precedent et etat reel */
	    for( i = 0; i < sizeof(vl_SyntModule)/sizeof(int); i++ )
	    {
	       vl_SyntModule[i] = XDC_FAUX;
	       vl_ChgtModule[i] = 0;
	    }
            for( i = 0, vl_Changement = 0; i < XZEXC_nb_module_max; i++ )
            {        	             	       
      	       vl_Module = atoi ( pl_ListeNAV->Config.Liste_Module[i].Module ) - 1;
      	       if ( strcmp(pl_ListeNAV->Etat.Module[i],vl_etatNAV[i]) )
      	       {
      	          /* L'etat a chang� */
      	          vl_Changement = 1;
      	          if ( vl_Module >= 0 ) vl_ChgtModule[vl_Module] = i;
      	       }
      	       if ( (vl_Module >= 0) && (vl_etatNAV[i][0] != '0') ) vl_SyntModule[vl_Module] = XDC_VRAI;
            }
            
            /* etat precedent <> etat reel */
            if ( vl_Changement )
            {
		         strcpy(vl_str,"");
	       /*A Memorisation du nouvel etat courant et copie ds un tableau pour envoi du msg */               
               for( i=0; i < XZEXC_nb_module_max; i++ )
               {        
                  strcpy( pl_ListeNAV->Etat.Module[i], vl_etatNAV[i] );
                if (strcmp(vl_str,""))
                        sprintf(vl_str,"%s|%s",vl_str, pl_ListeNAV->Etat.Module[i]);
                else
                        sprintf(vl_str,"%s",pl_ListeNAV->Etat.Module[i]);

#ifndef _TIBCO_EMS
                  pl_Tab[i] = vl_etatNAV[i];
#else
                  strcpy(pl_Tab[i] , vl_etatNAV[i]);
#endif
               }
               
	       /* Construction du datagroup */
	       XDG_EncodeDG2 ( pl_DG, XDG_ETAT_NAV, XZSC23_Nom_From_Numero(pl_ListeNAV->Config.SiteGestion));
	       			
	       /*A Envoi du message XDM_ETAT_NAV */
     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_NAV),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Type,
                          	T_IPC_FT_STR_ARRAY ,  pl_Tab , XZEXC_nb_module_max,
#ifdef _TIBCO_EMS
			XDC_Lon_Nom,
#endif

                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_NAV non effectue." );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_NAV effectue." );
	       }
	       /*A Envoi du message XDM_ETAT_NAV */
     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_NAV_EMS),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeNAV->Config.Type,
                          	T_IPC_FT_STR, vl_str,
                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_NAV non effectue.vl_str=[%s]", vl_str );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_NAV effectue. vl_str=[%s]", vl_str );
	       }
	       XZSC_06NomTache( vl_NomTache );
               XZSC_07NomMachine( vl_NomMachine );
	       for( i = 0; i < sizeof(vl_SyntModule)/sizeof(int); i++ )
	       {
	          if (	vl_ChgtModule[i] && pl_ListeNAV->Config.Liste_Module[vl_ChgtModule[i]].NoModule &&
	                (vl_SyntModule[i] != vl_MemoModule[i]) )
	              if ( atoi(pl_ListeNAV->Config.Liste_Module[vl_ChgtModule[i]].Module) < 5 )
	                 XZMA01_Maj_duree_marche ( 	 pl_ListeNAV->Config.Liste_Module[vl_ChgtModule[i]].NoModule, 
	              					 XDC_EQT_MOD,     
	              					 vl_SyntModule[i],
	              					 vl_NomTache,
	              					 vl_NomMachine );
	              else
	                 if ( vl_SyntModule[i] == XDC_VRAI )
	                    XZMA02_Maj_nb_utilisations ( pl_ListeNAV->Config.Liste_Module[vl_ChgtModule[i]].NoModule, 
	              					 XDC_EQT_MOD,     
	              					 vl_NomTache,
	              					 vl_NomMachine );
	          vl_MemoModule[i] = vl_SyntModule[i];
	       }
            }
            else
            {
               XZST_03EcritureTrace( XZSTC_DEBUG1, "etat precedent = etat reel");
            }
            /* Raz de la memoire etat NAV */
            pl_ListeNAV->TramePS[0] = '\0';
            break;
	 }
	 /* c'est une trame de status complet */
	 case XZEXC_TRAME_TEST_COM:
	 case XZEXC_TRAME_STAT_COMP:
	 {
                        
            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
               strcpy(pl_ListeNAV->TrameST,"");
	       pl_ListeNAV->EtatTrans = ENAVC_EQT_PANNE;
	       if ( *pl_ListeNAV->Etat.Scenario != '\0' )
	       {
                  XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  NOK", pl_ListeNAV->NumCmd);
                  if ( ( pl_ListeNAV->NumCmd != XZEXC_NOCMD ) && ( pl_ListeNAV->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat24_CompteRendu_Nav(pl_ListeNAV->NumCmd,vl_horodate,XDC_NOK,
					pl_ListeNAV->Config.SiteGestion);
                  else
                  {
                     if ( vl_EtatEqt == XDC_OK )
                        ex_env_alerte  (      vl_horodate,
                                   	      XZECC_TYPE_EQT_NAV,
                                   	      pl_ListeNAV->Config.Numero,
                                   	      XZAAC_AFF_DIF,
                                    	      vg_Mode_Fonct,
					      pl_ListeNAV->Config.SiteGestion,
                                    	      vl_TexteAlerte,
                                    	      NULL );
                  }
               }
               *pl_ListeNAV->Etat.Scenario = '\0';
               break;
            }

            if ((vl_typ_rep==XZEXC_REP_BLC_INT) || (vl_typ_rep==XZEXC_REP_BLC_FIN))
            {
               strcat(pl_ListeNAV->TrameST,pl_trame);
               
               if (vl_typ_rep==XZEXC_REP_BLC_INT) break;
            }
            
            /* Decodage de la trame de status */
            ex_decode_status ( pl_ListeNAV->TrameST,
                               vl_tabstatus,
                               24,
                               vl_adrrgs,
                               vl_horodate,
                               XZECC_TYPE_EQT_NAV,
                               pl_ListeNAV->Config.Numero,
                               vg_NomMachine,
			       pl_ListeNAV->Config.SiteGestion);
            
            ex_traitERI_NAV(vl_tabstatus[7],vl_horodate,XZECC_TYPE_EQT_NAV,pl_ListeNAV->Config.Numero,vg_NomMachine,
	    			pl_ListeNAV->Config.SiteGestion);

            /*A Traitement des alerte connexion locale */
            if ( (vl_tabstatus[3][0]  != '0') && (vl_tabstatus[3][0]  != ' ') && (vl_tabstatus[3][0]  != '\0') ) vl_Alerte = XZAAC_CLE;
            if ( (vl_Alerte != 0) && (vl_EtatEqt == XDC_OK) && (pl_ListeNAV->Mode[0] == XZEXC_MODE_NORMAL) )
               ex_env_alerte  (    vl_horodate,
                                   XZECC_TYPE_EQT_NAV,
                                   pl_ListeNAV->Config.Numero,
                                   vl_Alerte,
                                   vg_Mode_Fonct,
				   pl_ListeNAV->Config.SiteGestion,
                                   vl_Alerte,
                                   NULL );
            pl_ListeNAV->Mode[0] = ( vl_Alerte == 0 ) ? XZEXC_MODE_NORMAL : XZEXC_MODE_CONNECTION_LOCALE;

            for ( vl_index = 1; vl_index < 4; vl_index++ )
            {
               if ( (vl_tabstatus[12+vl_index][0] != '0') && 
                    (vl_tabstatus[12+vl_index][0] != ' ') && 
                    (vl_tabstatus[12+vl_index][0] != '\0') )
               { 
                  switch ( vl_index )
                  {
                     case 1 : vl_Alerte = XZAAC_CLL; break;
                     case 2 : vl_Alerte = XZAAC_CLE; break;
                     case 3 : vl_Alerte = XZAAC_CLI; break;
                  }
               }
               else
                  vl_Alerte = 0;
               
               if ( (vl_Alerte != 0) && (vl_EtatEqt == XDC_OK) && (pl_ListeNAV->Mode[vl_index] == XZEXC_MODE_NORMAL) )
                  ex_env_alerte  ( vl_horodate,
                                   XZECC_TYPE_EQT_NAV,
                                   pl_ListeNAV->Config.Numero,
                                   vl_Alerte,
                                   vg_Mode_Fonct,
				   pl_ListeNAV->Config.SiteGestion,
                                   (vl_Alerte == XZAAC_CLL) ? atoi(vl_tabstatus[13]) : (vl_Alerte == XZAAC_CLE) ? atoi(vl_tabstatus[14]) : atoi(vl_tabstatus[15]),
                                   NULL );
               pl_ListeNAV->Mode[vl_index] = ( vl_Alerte == 0 ) ? XZEXC_MODE_NORMAL : XZEXC_MODE_CONNECTION_LOCALE;
            }
            
            /*A Traitement  alerte colision */
            if( (vl_EtatEqt == XDC_OK) && 
                (vl_tabstatus[16][0] != '0') && (strlen(vl_tabstatus[16]) > 0) && 
                (pl_ListeNAV->Colision == XDC_FAUX) )
               ex_env_alerte  (    vl_horodate,
                                   XZECC_TYPE_EQT_NAV,
                                   pl_ListeNAV->Config.Numero,
                                   XZAAC_COL,
                                   vg_Mode_Fonct,
				   pl_ListeNAV->Config.SiteGestion,
                                   vl_tabstatus[16],
                                   NULL );
            if ( (vl_tabstatus[16][0] != '0') && (strlen(vl_tabstatus[16]) > 0) )
               pl_ListeNAV->Colision = XDC_VRAI; 
            else
               pl_ListeNAV->Colision = XDC_FAUX;

            /*A Traitement  alerte EDF */
            if( (vl_EtatEqt == XDC_OK) && 
                (vl_tabstatus[2][1] == '.') && (strlen(vl_tabstatus[2]) > 2) && 
                (pl_ListeNAV->EDF == XDC_FAUX) ) 
	    {
               XZST_03EcritureTrace(XZSTC_WARNING,"ex_env_alerte EDF eqt:%d,etat prec EDF:%d,vl_tabstatus[2]=%s",pl_ListeNAV->Config.Numero,pl_ListeNAV->EDF,vl_tabstatus[2]);
		ex_env_alerte  (    vl_horodate,
                                   XZECC_TYPE_EQT_NAV,
                                   pl_ListeNAV->Config.Numero,
                                   XZAAC_EDF,
                                   vg_Mode_Fonct,
				   pl_ListeNAV->Config.SiteGestion,
                                   vl_tabstatus[2],
                                   NULL );
            }
	    if ( (vl_tabstatus[2][1] == '.') && (strlen(vl_tabstatus[2]) > 2) )
               pl_ListeNAV->EDF = XDC_VRAI; 
            else
               pl_ListeNAV->EDF = XDC_FAUX;

	    /* gestion du compte-rendu */

               XZST_03EcritureTrace(XZSTC_WARNING,"STAT COMP eqt:%d,nocmd=%d,etat prec scena:%s,vl_tabstatus[19]=%s",pl_ListeNAV->Config.Numero,vl_nocmd,pl_ListeNAV->Etat.Scenario,vl_tabstatus[19]);
	    if ( ((vl_nocmd == XZEXC_CMD_HORS_BASE) || (vl_nocmd == XZEXC_NOCMD)) &&
	         (vl_tabstatus[19][0]=='0') && (*pl_ListeNAV->Etat.Scenario != '\0') )
	    {
               EnvoyerTrame( "PS",
                              XZEXC_TRAME_ETAT_POS_NAV,
                              pl_ListeNAV->Config.AdresseRGS,
                              XZEXC_CMD_HORS_BASE,
                              XZEXC_PRIORITEFAIBLE,
                              pl_ListeNAV->Socket);
                pl_ListeNAV->EtatTrans = ENAVC_EQT_OCCUP;
                
	        ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                                        pl_ListeNAV->Config.Numero, 
                                        NULL,
                                        pl_ListeNAV->TrameST,
                                        0 );
                                        
	       /* acquisition etat scenario */
               sprintf ( vl_TexteAlerte, "Scenario %s\0", pl_ListeNAV->Etat.Scenario );
	       *pl_ListeNAV->Etat.Scenario = '\0';
	       
               XZST_03EcritureTrace(XZSTC_WARNING,"STAT COMP Mise � zero scenar eqt:%d,nocmd=%d,etat scena:%s,vl_tabstatus[19]=%s",pl_ListeNAV->Config.Numero,vl_nocmd,pl_ListeNAV->Etat.Scenario,vl_tabstatus[19]);
	       /* envoi cr nav */
	       if ((strstr(vl_tabstatus[20],"NOK")!=NULL) ||
	           (strstr(vl_tabstatus[21],"NOK")!=NULL))
	       {
                  XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  NOK", pl_ListeNAV->NumCmd);
                  if ( ( pl_ListeNAV->NumCmd != XZEXC_NOCMD ) && ( pl_ListeNAV->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat24_CompteRendu_Nav(pl_ListeNAV->NumCmd,vl_horodate,XDC_NOK,
					pl_ListeNAV->Config.SiteGestion);
                  else
                     ex_env_alerte  (   vl_horodate,
                                   	XZECC_TYPE_EQT_NAV,
                                   	pl_ListeNAV->Config.Numero,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeNAV->Config.SiteGestion,
                                    	vl_TexteAlerte,
                                    	NULL );
	       }
	       else
	       {
                  XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  OK", pl_ListeNAV->NumCmd);
                  if ( ( pl_ListeNAV->NumCmd != XZEXC_NOCMD ) && ( pl_ListeNAV->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat24_CompteRendu_Nav(pl_ListeNAV->NumCmd,vl_horodate,XDC_OK,
					pl_ListeNAV->Config.SiteGestion);
	       }
	    }
	    
	    /* Traitement du compte rendu d'arret d'urgence */
	    if ( vl_nocmd == XZEXC_CMD_AU_NAV )
	    {
	       ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                                       pl_ListeNAV->Config.Numero, 
                                       NULL,
                                       pl_ListeNAV->TrameST,
                                       XZEXC_CMD_AU_NAV );
                                        
	       pl_ListeNAV->ArretUrg = XZEXC_NOCMD;
	       if ( vl_tabstatus[19][0] != '0' )
	       {
                  ex_env_alerte  (   	vl_horodate,
                                   	XZECC_TYPE_EQT_NAV,
                                   	pl_ListeNAV->Config.Numero,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeNAV->Config.SiteGestion,
                                    	"Arret d'urgence",
                                    	NULL );
	       }
	       else
	       {
                  if ( ( pl_ListeNAV->NumCmd != XZEXC_NOCMD ) && ( pl_ListeNAV->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat24_CompteRendu_Nav(pl_ListeNAV->NumCmd,vl_horodate,XDC_NOK,
					pl_ListeNAV->Config.SiteGestion);
	       }
	    }
            /* Raz de la memoire etat NAV */
            pl_ListeNAV->TrameST[0] = '\0';
            break;
	 }
  
	 /* c'est une trame d'etat matrice */
	 case XZEXC_TRAME_MAT_NAV :
	 {

            if ( (vl_typ_rep != XZEXC_REP_NAK) && 
                 (vl_typ_rep != XZEXC_REP_ACK) && 
                 (vl_typ_rep != XZEXC_TRAME_NON_TRANSM) )
            {
               /* Analyse etat matrice NAV */
               strcpy ( vl_MAT[0], pl_trame );
               strcpy ( vl_MAT[1], pl_trame );
               strcpy ( vl_MAT[0], strtok( strstr(vl_MAT[0],ENAVC_MATRICE_PRES), " \n") );
               strcpy ( vl_MAT[1], strtok( strstr(vl_MAT[1],ENAVC_MATRICE_NEUT), " \n") );
               XZST_03EcritureTrace(XZSTC_WARNING,"BRA:%d MAT1:<%s>  MAT2:<%s> Cmd:<%s>", 
               			pl_ListeNAV->Config.Numero, vl_MAT[0],vl_MAT[1], pl_ListeNAV->Matrice.Param );
            
               if ( strcmp ( pl_ListeNAV->Matrice.Etat[0], vl_MAT[0] ) ||
                    strcmp ( pl_ListeNAV->Matrice.Etat[1], vl_MAT[1] ) )
               {
                  for ( vl_index = 0; vl_index < ENAVC_NB_MATRICE; vl_index++ )
                  {
                     strcpy ( pl_ListeNAV->Matrice.Etat[vl_index], vl_MAT[vl_index] );
                     for ( vl_indexParam = 0; vl_indexParam < XDC_Lon_Params_Matrice; vl_indexParam++ )
                        pl_ListeNAV->Matrice.Conf[vl_index][vl_indexParam] = 0;
                     strcpy ( vl_TexteMatrice, vl_MAT[vl_index] );
                     if ( vl_Mot = strtok ( vl_TexteMatrice, ENAVC_CAR_SEP_MATRICE ) )
                     {
                        strcpy ( pl_ListeNAV->Matrice.Nom[vl_index], vl_Mot );
                        vl_indexParam = 0;
                        while ( (vl_Mot = strtok ( NULL, ENAVC_CAR_SEP_MATRICE )) &&  (vl_indexParam < XDC_Lon_Params_Matrice) )
                        {
                           pl_ListeNAV->Matrice.Conf[vl_index][vl_indexParam++] = atoi ( vl_Mot );
                           XZST_03EcritureTrace (XZSTC_DEBUG1,"Mat%d Parametre Mat%d:%d",vl_index+1,vl_indexParam, pl_ListeNAV->Matrice.Conf[vl_index][vl_indexParam-1]);
                        }
                     }
                  }   
               }
            
               en_Emission_MAT_NAV ( pl_ListeNAV, XDG_ETAT_MAT_NAV );
            
               /* Envoi du cr suivant la comparairon de la matrice command� et de la matrice reelle */
	       if ( vl_nocmd != XZEXC_NOCMD )
	       {
	          ex_ecriture_trace_cmd ( XDC_EQT_NAV,
                                          pl_ListeNAV->Config.Numero, 
                                          NULL,
                                          pl_trame,
                                          0 );
                                       
	          if ( (vl_nocmd == 1) ? strcmp ( vl_MAT[0], pl_ListeNAV->Matrice.Param ) :\
	          			 strcmp ( vl_MAT[1], pl_ListeNAV->Matrice.Param ) )
                     ex_env_alerte  (   vl_horodate,
                                  	XZECC_TYPE_EQT_NAV,
                                   	pl_ListeNAV->Config.Numero,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeNAV->Config.SiteGestion,
                                    	(vl_nocmd == 1) ? "Matrice PRES" : "Matrice NEUT",
                                    	NULL );
                     
	       }    
	    }    
            break;
	 }
      }
   }
}
