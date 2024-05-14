/*E*/
/* Fichier : $Id: ebad_tra.c,v 1.4 2020/11/03 18:09:28 pc2dpdy Exp $     Release : $Revision: 1.4 $       Date : $Date: 2020/11/03 18:09:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAD * FICHIER ebad_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
*
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	13/02/18	: corrcetion du test cmd ok vl_tabstatus[10][0]=='0' v1.2
* JMG	14/05/20 : EMS 1.3
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------ */

/* fichiers inclus */

#include "ebad_tra.h"

/* definitions de constantes */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)ebad_tra.c	1.27 01/30/98 : ebad_tra.c" ;

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
   static char *version = "@(#)ebad_tra.c	1.27 01/30/98 : em_traitement_trame" ;

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
   XDY_NomPicto			vl_etatBAD[XZEXC_nb_module_max_BAD];
   char				vl_poub[16] = "", 
   				vl_pos[16]  = "";
   EBAD_DONNEES_BAD		*pl_ListeBAD;
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
   static char *		pl_Tab[XZEXC_nb_module_max_BAD];
#else
   XDY_Nom 	pl_Tab[XZEXC_nb_module_max_BAD];
#endif

   static XZEXT_TEXTE_STATUS	vl_tabstatus[24];
   char   			vl_TexteAlerte[10] = "";
   XZEXT_Trame			vl_TexteMatrice = "";
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
   XDY_District			vl_Site;
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
      for ( pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL ; pl_ListeBAD = pl_ListeBAD->Suivant )
      {
         if (!strcmp(pl_ListeBAD->Config.AdresseRGS,vl_adrrgs))
         {
	    vl_noeqt = pl_ListeBAD->Config.Numero;
	    vl_Site = pl_ListeBAD->Config.SiteGestion;
	    if ( vl_typ_rep != XZEXC_REP_BLC_INT )
	       pl_ListeBAD->EtatTrans = EBADC_EQT_LIBRE;
	    if ( !pl_ListeBAD->Etat_Service )
	       vl_EtatEqt = XDC_OK;
	    if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max_BAD) &&
	         (!pl_ListeBAD->Config.Liste_Module[vl_nocmd-1].Etat_Service) )
	       vl_EtatModule = XDC_OK;
            break;
         }
      }

      /* eqt non trouve */
      if ( pl_ListeBAD == NULL ) 
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " BAD Non trouve en memoire mes=%s",vl_Msg);
         return;
      }

      /*A Traitement des alarmes transmission avec l'equipement */
      if ( !(pl_ListeBAD->Etat_Service & XDC_EQT_HORS_SRV) )
           ex_traite_alarme_com ( vg_NomMachine,
				  XDC_EQT_BAD,
				  vl_noeqt,
      				  vl_typ_rep,
      				  vl_nbnonrep,
      				  vl_nberrcrc,
      				  vl_typetrame,
      				  vg_Mode_Fonct ,
				  vl_Site);
      				  
      if ( (pl_ListeBAD->Etat_Service & XDC_EQT_HORS_SRV) && (vl_typetrame != XZEXC_TRAME_FEN_LCR) )
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
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeBADFLcr, vl_typ_rep );
            break;
         } 
         /*A C'est une trame de reponse status temps reel */
         case XZEXC_TRAME_STAT_TR:
         {
            /*A Si le status TR est ko -> demande du status complet */
            if ( ( ((*pl_trame_ptr != '@')&&(*(pl_trame_ptr+1) != '@')) && (pl_ListeBAD->StatusTR == XDC_OK) ) ||
                 ( ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1) == '@')) && (pl_ListeBAD->StatusTR != XDC_OK) ) ) 
            {
	       
               /*A Si le status TR est ko -> demande du status complet */
	       if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
	       {
	          pl_ListeBAD->EtatTrans = EBADC_EQT_PANNE;
	          return;
	       }
	       
	       /*A Envoyer demande Status Complet */
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListeBAD->EtatTrans = EBADC_EQT_OCCUP;
            }
	    pl_ListeBAD->StatusTR = ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1)) == '@') ? XDC_OK : XDC_NOK;
            break;
         }
         /*A C'est une trame de reponse etat de positionnement nav */
         case XZEXC_TRAME_ETAT_POS_BAD:
         {   
            /*A Suivant le type de bloc -> on concatene et on analyse   ou  on shunte si probleme */
            
            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
               strcpy(pl_ListeBAD->TramePS,"");
	       pl_ListeBAD->EtatTrans = EBADC_EQT_PANNE;
               if ( (vl_EtatModule == XDC_OK) && (vl_EtatEqt == XDC_OK) )
               {
      	          /* Envoi d'une alerte si etat different  */
                   ex_env_alerte  (   	vl_horodate,
                                   	XDC_EQT_MODBAD,
                                   	pl_ListeBAD->Config.Liste_Module[vl_nocmd-1].NoModule,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeBAD->Config.SiteGestion,
                                    	"Module",
                                    	NULL );
               }

               /*A Mise a jour memoire Cmd Module */
               if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max_BAD) ) 
                   pl_ListeBAD->EtatCmd.Module[vl_nocmd-1][0] = '\0';
               break;
            }

            if ((vl_typ_rep==XZEXC_REP_BLC_INT) || (vl_typ_rep==XZEXC_REP_BLC_FIN))
            {
               strcat(pl_ListeBAD->TramePS,pl_trame);
               
               if (vl_typ_rep==XZEXC_REP_BLC_INT) break;
            }
                        
            /*A Si le status TR est ko -> demande du status complet */
            if ( strchr ( pl_ListeBAD->TramePS, '@' ) == NULL ) 
            {	       
	       /*A Envoyer demande Status Complet */
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
                            (*pl_ListeBAD->Etat.Scenario != '\0') ? XZEXC_CMD_HORS_BASE : XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListeBAD->EtatTrans = EBADC_EQT_OCCUP;
            }
            else
            {	       
	       /*A Envoyer demande Status Complet */
	       EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
                            (*pl_ListeBAD->Etat.Scenario != '\0') ? XZEXC_CMD_HORS_BASE : XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListeBAD->EtatTrans = EBADC_EQT_OCCUP;
            }
            

            if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max_BAD) ) 
	        ex_ecriture_trace_cmd ( XDC_EQT_BAD,
                                        pl_ListeBAD->Config.Numero, 
                                        NULL,
                                        pl_ListeBAD->TramePS,
                                        vl_nocmd );

	    /* Recherche des modules et de leur position */
            for( i=0 ; i < XZEXC_nb_module_max_BAD; i++ )
            {  
       	       strcpy(vl_etatBAD[i],"0");
       	       
               /* Recherche ds la trame du module */
               pl_trame_ps_ptr = strstr ( pl_ListeBAD->TramePS, pl_ListeBAD->Config.Liste_Module[i].Module );
               
               /* Module dans la trame -> MAJ etat */
      	       if ( pl_trame_ps_ptr != NULL )
      	       {
      	          /* Extraction de l'etat */
      	          vl_param = sscanf (pl_trame_ps_ptr,"%s AF=%s",vl_poub,vl_pos);
      	          if ( vl_param == 2 ) 
      	          {
      	             /* Suppression des guillemets dans etat du module */
      	             if ( vl_pos[0] == '"' )
      	             {
      	                strcpy ( vl_pos, "1" );
      	             }
      	             
      	             if (!strncmp(pl_trame_ps_ptr-6,"N",1))
      	             {
			strcpy(vl_etatBAD[i], (vl_nocmd == XZEXC_NOCMD) ? "2" : vl_pos);
     	             }
      	             else
      	             {
      	                strcpy(vl_etatBAD[i],vl_pos);
     	             }
      	          }
               }
            }
                        
            /*A Comparaison etat reel et etat commande dans le cas de commande manuelle */
            if ( (vl_EtatModule == XDC_OK) && (vl_EtatEqt == XDC_OK) &&
		 (strcmp(pl_ListeBAD->EtatCmd.Module[vl_nocmd-1],vl_etatBAD[vl_nocmd-1])) )
            {
      	        /* Envoi d'une alerte si etat different  */
                 ex_env_alerte  (   	vl_horodate,
                                   	XDC_EQT_MODBAD,
                                   	pl_ListeBAD->Config.Liste_Module[vl_nocmd-1].NoModule,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeBAD->Config.SiteGestion,
                                    	"Module",
                                    	NULL );
            }

            /*A Mise a jour memoire Cmd Module */
            if ( (vl_nocmd != XZEXC_NOCMD) && (vl_nocmd <= XZEXC_nb_module_max_BAD) ) 
                pl_ListeBAD->EtatCmd.Module[vl_nocmd-1][0] = '\0';
                
            /*A Comparaison etat reel et etat commande dans le cas de commande contraste */
            if ( (pl_PtrContraste = strstr(  pl_TrameContraste, "EC=" )) != NULL )
            {
	       if ( *(pl_PtrContraste+3) != pl_ListeBAD->ContrasteEtat ) 
	       {
	          /* Envoi XDM_Etat_Contraste  */
	          pl_ListeBAD->ContrasteEtat = *(pl_PtrContraste+3);
	          /* Construction du datagroup */
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite) ;			
                  if (!TipcSrvMsgWrite (  	pl_DG
      			  			,XDM_IdentMsg(XDM_ETAT_Contraste)
  			 			,XDM_FLG_SRVMSGWRITE
                          			,T_IPC_FT_INT2, XDC_EQT_BAD
                          			,T_IPC_FT_INT2, pl_ListeBAD->Config.Numero
                          			,T_IPC_FT_INT2, (pl_ListeBAD->ContrasteEtat == 'S') ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL
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
               if ( (*pl_ListeBAD->Etat.Scenario == '\0') &&
                    (((pl_ListeBAD->Contraste == XDC_CONTRASTEBRILLANT) && (pl_ListeBAD->ContrasteEtat != 'S')) ||
                     ((pl_ListeBAD->Contraste == XDC_CONTRASTENORMAL) && (pl_ListeBAD->ContrasteEtat != 'J') && (vg_Contexte == XDC_JOUR)) ||
                     ((pl_ListeBAD->Contraste == XDC_CONTRASTENORMAL) && (pl_ListeBAD->ContrasteEtat != 'N') && (vg_Contexte != XDC_JOUR))) )
                  em_envoyer_contraste_BAD ( pl_ListeBAD, pl_ListeBAD->Contraste, XZEXC_NOCMD );
            }
           
              
           if ( (pl_PtrContraste != NULL) &&
                (vl_nocmd == XZEXC_CMD_HORS_BASE) &&
                (vl_EtatEqt == XDC_OK) && 
                (pl_ListeBAD->ContrasteCmd != pl_ListeBAD->ContrasteEtat) &&
                (pl_ListeBAD->ContrasteCmd != XZEXC_NOCMD) )
            {
               ex_env_alerte  ( vl_horodate,
                                XDC_EQT_BAD,
                                pl_ListeBAD->Config.Numero,
                                XZAAC_AFF_DIF,
                                vg_Mode_Fonct,
				pl_ListeBAD->Config.SiteGestion,
                                "Contraste",
                                NULL );
            }
            if (vl_nocmd  == XZEXC_CMD_HORS_BASE)
               pl_ListeBAD->ContrasteCmd = XZEXC_NOCMD;          
               

            /*A Comparaison etat precedent et etat reel */
	    for( i = 0; i < sizeof(vl_SyntModule)/sizeof(int); i++ )
	    {
	       vl_SyntModule[i] = XDC_FAUX;
	       vl_ChgtModule[i] = 0;
	    }
            for( i = 0, vl_Changement = 0; i < XZEXC_nb_module_max_BAD; i++ )
            {        	             	       
      	       vl_Module = atoi ( pl_ListeBAD->Config.Liste_Module[i].Module ) - 1;
      	       if ( strcmp(pl_ListeBAD->Etat.Module[i],vl_etatBAD[i]) )
      	       {
      	          /* L'etat a chang� */
      	          vl_Changement = 1;
      	          if ( vl_Module >= 0 ) vl_ChgtModule[vl_Module] = i;
      	       }
      	       if ( (vl_Module >= 0) && (vl_etatBAD[i][0] != '0') ) vl_SyntModule[vl_Module] = XDC_VRAI;
            }
            
            /* etat precedent <> etat reel */
            if ( vl_Changement )
            {
	       /*A Memorisation du nouvel etat courant et copie ds un tableau pour envoi du msg */               
		strcpy(vl_str,"");
               for( i=0; i < XZEXC_nb_module_max_BAD; i++ )
               {        
                  strcpy( pl_ListeBAD->Etat.Module[i], vl_etatBAD[i] );
                if (strcmp(vl_str,""))
                        sprintf(vl_str,"%s|%s",vl_str, pl_ListeBAD->Etat.Module[i]);
                else
                        sprintf(vl_str,"%s",pl_ListeBAD->Etat.Module[i]);

#ifndef _TIBCO_EMS
                  pl_Tab[i] = vl_etatBAD[i];
#else
                  strcpy(pl_Tab[i] , vl_etatBAD[i]);
#endif
               }
               
	       /* Construction du datagroup */
	       XDG_EncodeDG2 ( pl_DG, XDG_ETAT_BAD, XZSC23_Nom_From_Numero(pl_ListeBAD->Config.SiteGestion));
	       			
	       /*A Envoi du message XDM_ETAT_BAD */
     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_BAD),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeBAD->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeBAD->Config.Type,
                          	T_IPC_FT_STR_ARRAY ,  pl_Tab , XZEXC_nb_module_max_BAD,
#ifdef _TIBCO_EMS
				XDC_Lon_Nom,
#endif
                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_BAD non effectue." );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_BAD effectue." );
	       }

     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_BAD_EMS),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeBAD->Config.Numero,
                          	T_IPC_FT_INT2, pl_ListeBAD->Config.Type,
                          	T_IPC_FT_STR, vl_str,
                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_BAD_EMS non effectue. vl_str=[%s]", vl_str );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_BAD_EMS effectue. vl_str=[%s]", vl_str );
	       }
	       XZSC_06NomTache( vl_NomTache );
               XZSC_07NomMachine( vl_NomMachine );
	       for( i = 0; i < sizeof(vl_SyntModule)/sizeof(int); i++ )
	       {
	          if (	vl_ChgtModule[i] && pl_ListeBAD->Config.Liste_Module[vl_ChgtModule[i]].NoModule &&
	                (vl_SyntModule[i] != vl_MemoModule[i]) )
	              if ( atoi(pl_ListeBAD->Config.Liste_Module[vl_ChgtModule[i]].Module) < 5 )
	                 XZMA01_Maj_duree_marche ( 	 pl_ListeBAD->Config.Liste_Module[vl_ChgtModule[i]].NoModule, 
	              					 XDC_EQT_MODBAD,     
	              					 vl_SyntModule[i],
	              					 vl_NomTache,
	              					 vl_NomMachine );
	              else
	                 if ( vl_SyntModule[i] == XDC_VRAI )
	                    XZMA02_Maj_nb_utilisations ( pl_ListeBAD->Config.Liste_Module[vl_ChgtModule[i]].NoModule, 
	              					 XDC_EQT_MODBAD,     
	              					 vl_NomTache,
	              					 vl_NomMachine );
	          vl_MemoModule[i] = vl_SyntModule[i];
	       }
            }
            else
            {
               XZST_03EcritureTrace( XZSTC_DEBUG1, "etat precedent = etat reel");
            }
            /* Raz de la memoire etat BAD */
            pl_ListeBAD->TramePS[0] = '\0';
            break;
	 }
	 /* c'est une trame de status complet */
	 case XZEXC_TRAME_TEST_COM:
	 case XZEXC_TRAME_STAT_COMP:
	 {
                        
            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
               strcpy(pl_ListeBAD->TrameST,"");
	       pl_ListeBAD->EtatTrans = EBADC_EQT_PANNE;
	       if ( *pl_ListeBAD->Etat.Scenario != '\0' )
	       {
                  XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  NOK", pl_ListeBAD->NumCmd);
                  if ( ( pl_ListeBAD->NumCmd != XZEXC_NOCMD ) && ( pl_ListeBAD->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat31_CompteRendu_BAD(pl_ListeBAD->NumCmd,vl_horodate,XDC_NOK,pl_ListeBAD->Config.SiteGestion);
                  else
                  {
                     if ( vl_EtatEqt == XDC_OK )
                        ex_env_alerte  (      vl_horodate,
                                   	      XZECC_TYPE_EQT_BAD,
                                   	      pl_ListeBAD->Config.Numero,
                                   	      XZAAC_AFF_DIF,
                                    	      vg_Mode_Fonct,
					      pl_ListeBAD->Config.SiteGestion,
                                    	      vl_TexteAlerte,
                                    	      NULL );
                  }
               }
               *pl_ListeBAD->Etat.Scenario = '\0';
               break;
            }

            if ((vl_typ_rep==XZEXC_REP_BLC_INT) || (vl_typ_rep==XZEXC_REP_BLC_FIN))
            {
               strcat(pl_ListeBAD->TrameST,pl_trame);
               
               if (vl_typ_rep==XZEXC_REP_BLC_INT) break;
            }
            
            /* Decodage de la trame de status */
            ex_decode_status ( pl_ListeBAD->TrameST,
                               vl_tabstatus,
                               24,
                               vl_adrrgs,
                               vl_horodate,
                               XZECC_TYPE_EQT_BAD,
                               pl_ListeBAD->Config.Numero,
                               vg_NomMachine,
			       pl_ListeBAD->Config.SiteGestion);
            
            /*ex_traitERI_BAD(vl_tabstatus[7],vl_horodate,XZECC_TYPE_EQT_BAD,pl_ListeBAD->Config.Numero,vg_NomMachine);*/

            /*A Traitement des alerte connexion locale */
            if ( (vl_tabstatus[3][0]  != '0') && (vl_tabstatus[3][0]  != ' ') && (vl_tabstatus[3][0]  != '\0') ) vl_Alerte = XZAAC_CLE;
            if ( (vl_Alerte != 0) && (vl_EtatEqt == XDC_OK) && (pl_ListeBAD->Mode[0] == XZEXC_MODE_NORMAL) )
               ex_env_alerte  (    vl_horodate,
                                   XZECC_TYPE_EQT_BAD,
                                   pl_ListeBAD->Config.Numero,
                                   vl_Alerte,
                                   vg_Mode_Fonct,
				   pl_ListeBAD->Config.SiteGestion,
                                   vl_Alerte,
                                   NULL );
            pl_ListeBAD->Mode[0] = ( vl_Alerte == 0 ) ? XZEXC_MODE_NORMAL : XZEXC_MODE_CONNECTION_LOCALE;

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
               
               if ( (vl_Alerte != 0) && (vl_EtatEqt == XDC_OK) && (pl_ListeBAD->Mode[vl_index] == XZEXC_MODE_NORMAL) )
                  ex_env_alerte  ( vl_horodate,
                                   XZECC_TYPE_EQT_BAD,
                                   pl_ListeBAD->Config.Numero,
                                   vl_Alerte,
                                   vg_Mode_Fonct,
				   pl_ListeBAD->Config.SiteGestion,
                                   (vl_Alerte == XZAAC_CLL) ? atoi(vl_tabstatus[13]) : (vl_Alerte == XZAAC_CLE) ? atoi(vl_tabstatus[14]) : atoi(vl_tabstatus[15]),
                                   NULL );
               pl_ListeBAD->Mode[vl_index] = ( vl_Alerte == 0 ) ? XZEXC_MODE_NORMAL : XZEXC_MODE_CONNECTION_LOCALE;
            }
            
            /*A Traitement  alerte colision */
            if( (vl_EtatEqt == XDC_OK) && 
                (vl_tabstatus[16][0] != '0') && (strlen(vl_tabstatus[16]) > 0) && 
                (pl_ListeBAD->Colision == XDC_FAUX) )
               ex_env_alerte  (    vl_horodate,
                                   XZECC_TYPE_EQT_BAD,
                                   pl_ListeBAD->Config.Numero,
                                   XZAAC_COL,
                                   vg_Mode_Fonct,
				   pl_ListeBAD->Config.SiteGestion,
                                   vl_tabstatus[16],
                                   NULL );
            if ( (vl_tabstatus[16][0] != '0') && (strlen(vl_tabstatus[16]) > 0) )
               pl_ListeBAD->Colision = XDC_VRAI; 
            else
               pl_ListeBAD->Colision = XDC_FAUX;

	    /* gestion du compte-rendu */
	    
	    XZST_03EcritureTrace(XZSTC_WARNING,"JMG : nocmd = %d , tabstatus = %s %s, scen = %s testOK=%d",
							vl_nocmd,
							vl_tabstatus[10],
							vl_tabstatus[11],
							pl_ListeBAD->Etat.Scenario,vl_tabstatus[10][0]=='0');
            if (vl_tabstatus[10][0]=='0')
	       {
                  XZST_03EcritureTrace(XZSTC_WARNING,"Compte Rendu Commande :%d  OK", pl_ListeBAD->NumCmd);
                  if ( ( pl_ListeBAD->NumCmd != XZEXC_NOCMD ) && ( pl_ListeBAD->NumCmd != XZEXC_CMD_HORS_BASE ) 
			  && (pl_ListeBAD->NumCmd !=0) ) {
                     xzat31_CompteRendu_BAD(pl_ListeBAD->NumCmd,vl_horodate,XDC_OK,pl_ListeBAD->Config.SiteGestion);
		     pl_ListeBAD->NumCmd = XZEXC_NOCMD;
                     EnvoyerTrame( "PS",
                              XZEXC_TRAME_ETAT_POS_BAD,
                              pl_ListeBAD->Config.AdresseRGS,
                              XZEXC_CMD_HORS_BASE,
                              XZEXC_PRIORITEFAIBLE,
                              pl_ListeBAD->Socket);
		  }
	    }

	    if ( (vl_nocmd == XZEXC_CMD_HORS_BASE) &&
	         (vl_tabstatus[19][0]=='0') && (*pl_ListeBAD->Etat.Scenario != '\0') )
	    {
               EnvoyerTrame( "PS",
                              XZEXC_TRAME_ETAT_POS_BAD,
                              pl_ListeBAD->Config.AdresseRGS,
                              XZEXC_CMD_HORS_BASE,
                              XZEXC_PRIORITEFAIBLE,
                              pl_ListeBAD->Socket);
                pl_ListeBAD->EtatTrans = EBADC_EQT_OCCUP;
                
	        ex_ecriture_trace_cmd ( XDC_EQT_BAD,
                                        pl_ListeBAD->Config.Numero, 
                                        NULL,
                                        pl_ListeBAD->TrameST,
                                        0 );
                                        
	       /* acquisition etat scenario */
               sprintf ( vl_TexteAlerte, "Scenario %s\0", pl_ListeBAD->Etat.Scenario );
	       *pl_ListeBAD->Etat.Scenario = '\0';
	       
	       /* envoi cr nav */
	       if ((strstr(vl_tabstatus[20],"NOK")!=NULL) ||
	           (strstr(vl_tabstatus[21],"NOK")!=NULL))
	       {
                  XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  NOK", pl_ListeBAD->NumCmd);
                  if ( ( pl_ListeBAD->NumCmd != XZEXC_NOCMD ) && ( pl_ListeBAD->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat31_CompteRendu_BAD(pl_ListeBAD->NumCmd,vl_horodate,XDC_NOK,pl_ListeBAD->Config.SiteGestion);
                  else
                     ex_env_alerte  (   vl_horodate,
                                   	XZECC_TYPE_EQT_BAD,
                                   	pl_ListeBAD->Config.Numero,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeBAD->Config.SiteGestion,
                                    	vl_TexteAlerte,
                                    	NULL );
	       }
	       else
	       {
                  XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  OK", pl_ListeBAD->NumCmd);
                  if ( ( pl_ListeBAD->NumCmd != XZEXC_NOCMD ) && ( pl_ListeBAD->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat31_CompteRendu_BAD(pl_ListeBAD->NumCmd,vl_horodate,XDC_OK,pl_ListeBAD->Config.SiteGestion);
	       }
	    }
	    
	    /* Traitement du compte rendu d'arret d'urgence */
	    if ( vl_nocmd == XZEXC_CMD_AU_NAV )
	    {
	       ex_ecriture_trace_cmd ( XDC_EQT_BAD,
                                       pl_ListeBAD->Config.Numero, 
                                       NULL,
                                       pl_ListeBAD->TrameST,
                                       XZEXC_CMD_AU_NAV );
                                        
	       pl_ListeBAD->ArretUrg = XZEXC_NOCMD;
	       if ( vl_tabstatus[19][0] != '0' )
	       {
                  ex_env_alerte  (   	vl_horodate,
                                   	XZECC_TYPE_EQT_BAD,
                                   	pl_ListeBAD->Config.Numero,
                                   	XZAAC_AFF_DIF,
                                    	vg_Mode_Fonct,
					pl_ListeBAD->Config.SiteGestion,
                                    	"Arret d'urgence",
                                    	NULL );
	       }
	       else
	       {
                  if ( ( pl_ListeBAD->NumCmd != XZEXC_NOCMD ) && ( pl_ListeBAD->NumCmd != XZEXC_CMD_HORS_BASE ) )
                     xzat31_CompteRendu_BAD(pl_ListeBAD->NumCmd,vl_horodate,XDC_NOK,pl_ListeBAD->Config.SiteGestion);
	       }
	    }
            /* Raz de la memoire etat BAD */
            pl_ListeBAD->TrameST[0] = '\0';
            break;
	 }
      }
   }
}
