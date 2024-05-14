/*E*/
/* Fichier : $Id: epal_tra.c,v 1.6 2020/11/03 18:12:16 pc2dpdy Exp $     Release : $Revision: 1.6 $      Date :  $Date: 2020/11/03 18:12:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant de l'eqt
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	14 Jan 1998	: Creation
* Mismer.D	version 1.2	29 avr 2004	: Ajout pilotage par scenario
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	10/07/09	: Correction du test de changement d'�tat d'affichage apres PS v1.4
* JMG	14/06/19	: ajout IP 1.5
* JMG	11/05/20 	: EMS 1.6
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* GGY    25/05/23 :  Suppression " /NOIR et /NOIR dans le message recu pour prendre en compte les nouveaux PAL (DEM-471)
------------------------------------------------------*/

/* fichiers inclus */

#include "epal_tra.h"

/* definitions de constantes */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: epal_tra.c,v 1.6 2020/11/03 18:12:16 pc2dpdy Exp $ : epal_tra.c" ;

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
   static char *version = "$Id: epal_tra.c,v 1.6 2020/11/03 18:12:16 pc2dpdy Exp $ : em_traitement_trame" ;

   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame_ptr,*pl_trame_ps_ptr,*pl_PtrContraste;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   int				vl_typetrame;
   char   			vl_TexteAlerte[10] = "";
   int 				vl_typ_rep;
   XZEXT_MSG_SOCKET		vl_Msg, pl_trame, pl_TrameContraste;
   int				vl_param,i,j;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   XDY_Etat_Module_PAL		vl_etatPAL;
   int				vl_etat;
   char				vl_rang[16] = "", 
   				vl_pos[16]  = "";
   EPAL_DONNEES_PAL		*pl_ListePAL;
   XDY_Datagroup		pl_DG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   char * 			vl_status[32];
   XDY_Mot	                vl_noeqt;
   int 				vl_valret;
   static XZEXT_TEXTE_STATUS	vl_tabstatus[12];
   char *			vl_Mot;
   XDY_Mot	    		vl_Changement = XDC_FAUX,
   				vl_PAL_allume = XDC_FAUX,
   				vl_PAL_anc_allume = XDC_FAUX,
   				vl_EchecCmd = XDC_FAUX;
   XZSCT_NomTache          	vl_NomTache   = "";
   XDY_NomMachine          	vl_NomMachine = "";
   char vl_suppr [10] = "";
   XZEXT_MSG_SOCKET vl_TmpMsg;

#ifndef _TIBCO_EMS
   static char *                pl_Tab[XDC_Lon_Nb_Mod_Pal];
#else
   XDY_Nom                 pl_Tab[XDC_Lon_Nb_Mod_Pal];
#endif
   char vl_str[1024];
   
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

	        /* Suppression des " si présent dans la chaine vl_Msg */
      int i=0, j=0, found=0, k, n=0, copy_loop=0;
      strcpy (vl_suppr,"\"");
      while (strstr(vl_Msg, vl_suppr) != NULL)
      {
         i=0; j=0; found=0; n=0; copy_loop=0;
         while(vl_Msg[i] !='\0') 
         { 
            j=0, found= 0, k=i; 
            while (vl_Msg[k]==vl_suppr [j] && vl_suppr [j]!='\0') 
            { 
               k++; 
               j++; 
            } 
            if (vl_suppr [j]=='\0') 
            { 
               copy_loop=k; 
            } 
            vl_TmpMsg[n] = vl_Msg[copy_loop]; 
            i++; 
            copy_loop++; 
            n++; 
         } 
         strcpy (vl_Msg,vl_TmpMsg);
      }

      /* Suppression de /NOIR si présent dans la chaine vl_Msg */
      i=0; j=0; found=0; n=0; copy_loop=0;
      strcpy (vl_suppr,"/NOIR");
      while (strstr(vl_Msg, vl_suppr) != NULL)
      {      
         i=0; j=0; found=0; n=0; copy_loop=0;
         while(vl_Msg[i] !='\0') 
         { 
            j=0, found= 0, k=i; 
            while (vl_Msg[k]==vl_suppr [j] && vl_suppr [j]!='\0') 
            { 
               k++; 
               j++; 
            } 
            if (vl_suppr [j]=='\0') 
            { 
               copy_loop=k; 
            } 
            vl_TmpMsg[n] = vl_Msg[copy_loop]; 
            i++; 
            copy_loop++; 
            n++;
         }
         strcpy (vl_Msg,vl_TmpMsg);
      } 
      
      /* Suppression de NOIR/ si présent dans la chaine vl_Msg */
      strcpy (vl_suppr,"NOIR/");
      while (strstr(vl_Msg, vl_suppr) != NULL)
      { 
         i=0; j=0; found=0; n=0; copy_loop=0;
         while(vl_Msg[i] !='\0') 
         { 
            j=0, found= 0, k=i; 
            while (vl_Msg[k]==vl_suppr [j] && vl_suppr [j]!='\0') 
            { 
               k++; 
               j++; 
            } 
            if (vl_suppr [j]=='\0') 
            { 
               copy_loop=k; 
            } 
            vl_TmpMsg[n] = vl_Msg[copy_loop]; 
            i++; 
            copy_loop++; 
            n++; 
         } 
         strcpy (vl_Msg,vl_TmpMsg);
      }

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
      
      /*A Recherche de la pal concern� */
      for ( pl_ListePAL = pg_debutPAL; pl_ListePAL != NULL ; pl_ListePAL = pl_ListePAL->Suivant )
      {
	 if (
	    ( (!strcmp(pl_ListePAL->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePAL->Config.AdresseRGS,vl_adrrgs)))
	      ||
	   ((strcmp(pl_ListePAL->Config.AdresseIP, XZECC_IP_VIDE)) && (!strcmp(pl_ListePAL->Config.AdresseIP, vl_adrrgs)))
						             )
         {
	    vl_noeqt = pl_ListePAL->Config.Numero;
	    strcpy(vl_adrrgs, pl_ListePAL->Config.AdresseRGS);
	    if ( vl_typ_rep != XZEXC_REP_BLC_INT )
	       pl_ListePAL->EtatTrans = EPALC_EQT_LIBRE;
            break;
         }
      }

      /* eqt non trouve */
      if ( pl_ListePAL == NULL ) 
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " PAL Non trouve en memoire mes=%s",vl_Msg);
         return;
      }

      /*A Traitement des alarmes transmission avec l'equipement */
      if ( !(pl_ListePAL->Etat_Service & XDC_EQT_HORS_SRV) )
           ex_traite_alarme_com ( vg_NomMachine,
				  XDC_EQT_PAL,
				  vl_noeqt,
      				  vl_typ_rep,
      				  vl_nbnonrep,
      				  vl_nberrcrc,
      				  vl_typetrame,
      				  vg_Mode_Fonct,
				  pl_ListePAL->Config.SiteGestion);
      				  
      if ( (pl_ListePAL->Etat_Service & XDC_EQT_HORS_SRV) && (vl_typetrame != XZEXC_TRAME_FEN_LCR) )
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
	       if (!strcmp(pl_ListePAL->Config.AdresseIP, XZECC_IP_VIDE))
            ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListePALFLcr, vl_typ_rep );
          else
            ex_ret_LCR_IP( pl_trame, vl_adrrgs, &pg_DebutListePALFLcr, vl_typ_rep );

            break;
         } 
         /*A C'est une trame de reponse status temps reel */
         case XZEXC_TRAME_STAT_TR:
         {
            /*A Si le status TR est ko -> demande du status complet */
            if ( ( ((*pl_trame_ptr != '@')&&(*(pl_trame_ptr+1) != '@')) && (pl_ListePAL->StatusTR == XDC_OK) ) ||
                 ( ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1) == '@')) && (pl_ListePAL->StatusTR != XDC_OK) ) ) 
            {
	       
               /*A Si le status TR est ko -> demande du status complet */
	       if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
	       {
	          pl_ListePAL->EtatTrans = EPALC_EQT_PANNE;
	          return;
	       }
	       
	       /*A Envoyer demande Status Complet */
	       if (!strcmp(pl_ListePAL->Config.AdresseIP, XZECC_IP_VIDE))
	       		EnvoyerTrame("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    vl_adrrgs,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       else
	       		EnvoyerTrameIP("ST\0",
	                    XZEXC_TRAME_STAT_COMP,
	                    pl_ListePAL->Config.AdresseIP,
	                    pl_ListePAL->Config.Port,
	                    vl_adrrgs,
	                    XZEXC_NOCMD,
	                    XZEXC_PRIORITEFAIBLE,
	                    va_socket);
	       pl_ListePAL->EtatTrans = EPALC_EQT_OCCUP;
            }
	    pl_ListePAL->StatusTR = ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1)) == '@') ? XDC_OK : XDC_NOK;
            break;
         }
         /*A C'est une trame de reponse etat de positionnement pal */
         case XZEXC_TRAME_ETAT_POS_PAL:
         {   
            /*A Suivant le type de bloc -> on concatene et on analyse   ou  on shunte si probleme */
            
            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
               strcpy(pl_ListePAL->TramePS,"");
	       pl_ListePAL->EtatTrans = EPALC_EQT_PANNE;

               if ( (pl_ListePAL->NumCmd == vl_nocmd) &&  (pl_ListePAL->NumCmd != XZEXC_NOCMD) )
               {
         	      XZST_03EcritureTrace(XZSTC_WARNING,"Echec commande du PAL %d cmd=%d",
         	  			pl_ListePAL->Config.Numero,
         	  			pl_ListePAL->NumCmd);
                      xzat28_CompteRendu_Pal(pl_ListePAL->NumCmd,vl_horodate,XDC_NOK,
						pl_ListePAL->Config.SiteGestion);
               }
               /*A Mise a jour memoire Cmd Module */
               if ( vl_nocmd != XZEXC_NOCMD ) 
                   pl_ListePAL->EtatCmd[0] = '\0';
               break;
            }

            if ((vl_typ_rep==XZEXC_REP_BLC_INT) || (vl_typ_rep==XZEXC_REP_BLC_FIN))
            {
               strcat(pl_ListePAL->TramePS,pl_trame);
               
               if (vl_typ_rep==XZEXC_REP_BLC_INT) break;
            }
                        

            if ( vl_nocmd != XZEXC_NOCMD ) 
	        ex_ecriture_trace_cmd ( XDC_EQT_PAL,
                                        pl_ListePAL->Config.Numero, 
                                        NULL,
                                        pl_ListePAL->TramePS,
                                        vl_nocmd );

	    pl_ListePAL->StatusTR = (strstr ( pl_ListePAL->TramePS,"@") != NULL) ? XDC_OK : XDC_NOK;
 

	    /* Recherche des modules et de leur position */
	    vl_Changement = vl_PAL_allume = vl_PAL_anc_allume = vl_EchecCmd = XDC_FAUX;
	    strcpy(vl_str,"");
            for( i=0 ; i < XDC_Lon_Nb_Mod_Pal; i++ )
            {  
       	       vl_etatPAL[0] = '\0';
      	       vl_etat = 0;
       	       
                  /* Recherche ds la trame du module */
                  pl_trame_ps_ptr = strstr ( pl_ListePAL->TramePS, pl_ListePAL->Config.Module[i] );
               
                  /* Module dans la trame -> MAJ etat */
      	          if ( pl_trame_ps_ptr != NULL )
      	          {
      	             /* Extraction de l'etat */
      	             vl_param = sscanf (pl_trame_ps_ptr,"%s AF=%s",vl_rang,vl_pos);
      	             if ( vl_param == 2 )
      	                   strcpy ( vl_etatPAL, vl_pos );
                     else
      	                   strcpy ( vl_etatPAL, "0" );
                   }
                XZST_03EcritureTrace(XZSTC_DEBUG1,"i=%d (%s)(%s)(%s) / (%s)",i,vl_rang,vl_etatPAL,pl_trame_ps_ptr,pl_ListePAL->Etat[i]    );

      	       /* test si L'etat a chang� */
      	       vl_Changement = ( strcmp(pl_ListePAL->Etat[i],vl_etatPAL) ) ? XDC_VRAI : vl_Changement;
               
      	       /* test si le PAL est allume */
      	       vl_PAL_allume = (strcmp(vl_etatPAL,"0")) ? XDC_VRAI : vl_PAL_allume;

      	       /* test si Le panneau etait allume */
      	       vl_PAL_anc_allume = (strcmp(pl_ListePAL->Etat[i],"0")) ? XDC_VRAI : vl_PAL_anc_allume;
      	             
      	            
	        /*A Memorisation du nouvel etat courant et copie ds un tableau pour envoi du msg */               
                strcpy (pl_ListePAL->Etat[i], vl_etatPAL);
                if (strcmp(vl_str,""))
                        sprintf(vl_str,"%s|%s",vl_str, pl_ListePAL->Etat[i]);
                else
                        sprintf(vl_str,"%s",pl_ListePAL->Etat[i]);
#ifndef _TIBCO_EMS
		pl_Tab[i] = pl_ListePAL->Etat[i];
#else
		strcpy(pl_Tab[i] , pl_ListePAL->Etat[i]);
#endif
            }
                        
            
            /* etat precedent <> etat reel */
            if ( vl_Changement == XDC_VRAI )
            {
	       /* Construction du datagroup */
	       XDG_EncodeDG2 ( pl_DG, XDG_ETAT_PAL, XZSC23_Nom_From_Numero(pl_ListePAL->Config.SiteGestion));
	       			
	       /*A Envoi du message XDM_ETAT_PAL */
     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PAL),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListePAL->Config.Numero,
                          	T_IPC_FT_INT2, vl_PAL_allume,
                          	T_IPC_FT_STR_ARRAY ,  pl_Tab, XDC_Lon_Nb_Mod_Pal,
#ifdef _TIBCO_EMS
				XDC_Lon_Nom,
#endif
                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_PAL non effectue." );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_PAL effectue." );
	       }
     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_PAL_EMS),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListePAL->Config.Numero,
                          	T_IPC_FT_INT2, vl_PAL_allume,
                          	T_IPC_FT_STR,	vl_str,
                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_PAL_EMS non effectue. vl_str=[%s]",vl_str );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_PAL_EMS effectue. vl_str=[%s]", vl_str );
	       }
	       
	       /* Gestion compteur de marche */
	       XZSC_06NomTache( vl_NomTache );
               XZSC_07NomMachine( vl_NomMachine );
	       if ( vl_PAL_allume !=  vl_PAL_anc_allume )
	          XZMA01_Maj_duree_marche ( 	 pl_ListePAL->Config.Numero, 
	              					 XDC_EQT_PAL,     
	              					 vl_PAL_allume,
	              					 vl_NomTache,
	              					 vl_NomMachine );
            }
            
            
            if ( vl_nocmd == XZEXC_CMD_LT_PAL )
               pl_ListePAL->NumCmd =  XZEXC_NOCMD;

                
            /*A Comparaison etat reel et etat commande dans le cas de commande contraste */
            if ( (pl_PtrContraste = strstr(  pl_TrameContraste, "EC=" )) != NULL )
            {
	       if ( *(pl_PtrContraste+3) != pl_ListePAL->ContrasteEtat ) 
	       {
	          /* Envoi XDM_Etat_Contraste  */
	          pl_ListePAL->ContrasteEtat = *(pl_PtrContraste+3);
	          /* Construction du datagroup */
	          XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CONTRASTE, vg_NomSite) ;			
                  if (!TipcSrvMsgWrite (  	pl_DG
      			  			,XDM_IdentMsg(XDM_ETAT_Contraste)
  			 			,XDM_FLG_SRVMSGWRITE
                          			,T_IPC_FT_INT2, XDC_EQT_PAL
                          			,T_IPC_FT_INT2, pl_ListePAL->Config.Numero
                          			,T_IPC_FT_INT2, (pl_ListePAL->ContrasteEtat == 'S') ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL
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
               if ( ((pl_ListePAL->Contraste == XDC_CONTRASTEBRILLANT) && (pl_ListePAL->ContrasteEtat != 'S')) ||
                     ((pl_ListePAL->Contraste == XDC_CONTRASTENORMAL) && (pl_ListePAL->ContrasteEtat != 'J') && (vg_Contexte == XDC_JOUR)) ||
                     ((pl_ListePAL->Contraste == XDC_CONTRASTENORMAL) && (pl_ListePAL->ContrasteEtat != 'N') && (vg_Contexte != XDC_JOUR)) )
                  em_envoyer_contraste_PAL ( pl_ListePAL, pl_ListePAL->Contraste, XZEXC_NOCMD );
            }
           
              
           if ( (pl_PtrContraste != NULL) &&
                (vl_nocmd == XZEXC_CMD_HORS_BASE) &&
                (!pl_ListePAL->Etat_Service) && 
                (pl_ListePAL->ContrasteCmd != pl_ListePAL->ContrasteEtat) &&
                (pl_ListePAL->ContrasteCmd != XZEXC_NOCMD) )
            {
               ex_env_alerte  ( vl_horodate,
                                XDC_EQT_PAL,
                                pl_ListePAL->Config.Numero,
                                XZAAC_AFF_DIF,
                                vg_Mode_Fonct,
				pl_ListePAL->Config.SiteGestion,
                                "Contraste",
                                NULL );
            }
            if (vl_nocmd  == XZEXC_CMD_HORS_BASE)
               pl_ListePAL->ContrasteCmd = XZEXC_NOCMD;          
               

            /* Raz de la memoire etat PAL */
            pl_ListePAL->TramePS[0] = '\0';
            break;
	 }
	 /* c'est une trame de status complet */
	 case XZEXC_TRAME_TEST_COM:
	 case XZEXC_TRAME_STAT_COMP:
	 {
                        
            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
               strcpy(pl_ListePAL->TrameST,"");
	       pl_ListePAL->EtatTrans = EPALC_EQT_PANNE;
               if ( *pl_ListePAL->EtatCmd != '\0' )
	      {
		XZST_03EcritureTrace(XZSTC_INFO,"Compte Rendu Commande :%d  NOK", pl_ListePAL->NumCmd);
	        if ( ( pl_ListePAL->NumCmd != XZEXC_NOCMD ) && ( pl_ListePAL->NumCmd != XZEXC_CMD_HORS_BASE ) )
	            xzat28_CompteRendu_Pal(pl_ListePAL->NumCmd,vl_horodate,XDC_NOK,
						pl_ListePAL->Config.SiteGestion);
	        else
                {
        	      if ( !pl_ListePAL->Etat_Service )
	        	      ex_env_alerte  (      vl_horodate,
				          XZECC_TYPE_EQT_PAL,
		                     pl_ListePAL->Config.Numero,
		                     XZAAC_AFF_DIF,
	                     	vg_Mode_Fonct,
				   pl_ListePAL->Config.SiteGestion,
		                     "affichage Panneau d'ALerte",
	                           NULL );
                 }
              }
	      *pl_ListePAL->EtatCmd = '\0';

	       break;
            }

            if ((vl_typ_rep==XZEXC_REP_BLC_INT) || (vl_typ_rep==XZEXC_REP_BLC_FIN))
            {
               strcat(pl_ListePAL->TrameST,pl_trame);
               
               if (vl_typ_rep==XZEXC_REP_BLC_INT) break;
            }
            
            /* Decodage de la trame de status */
            ex_decode_status ( pl_ListePAL->TrameST,
                               vl_tabstatus,
                               12,
                               vl_adrrgs,
                               vl_horodate,
                               XDC_EQT_PAL,
                               pl_ListePAL->Config.Numero,
                               vg_NomMachine,
			       pl_ListePAL->Config.SiteGestion);
            

	    /* gestion du compte-rendu */
	    
	    if ( (vl_tabstatus[11][0]=='0') && (*pl_ListePAL->EtatCmd != '\0') )
	    {
                
	        ex_ecriture_trace_cmd ( XDC_EQT_PAL,
                                        pl_ListePAL->Config.Numero, 
                                        NULL,
                                        pl_ListePAL->TrameST,
                                        0 );
                                        
	       /* acquisition etat scenario */
               sprintf ( vl_TexteAlerte, "Scenario %s\0", pl_ListePAL->EtatCmd );
	       *pl_ListePAL->EtatCmd = '\0';
	       
               XZST_03EcritureTrace(XZSTC_WARNING,"Compte Rendu Commande :%d  OK", pl_ListePAL->NumCmd);
               xzat28_CompteRendu_Pal(pl_ListePAL->NumCmd,vl_horodate,XDC_OK,
					pl_ListePAL->Config.SiteGestion);
	    }








	    
            /* Raz de la memoire etat PAL */
            pl_ListePAL->TrameST[0] = '\0';
            break;
	 }
  
      }
   }
}
