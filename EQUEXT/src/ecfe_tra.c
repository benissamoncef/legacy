/* Fichier : ecfe_tra.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant de l'eqt
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG 04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* JPL	02/05/23 : Gestion versions, traces, retour de fonction; identique precedente  1.3
* JPL	03/05/23 : Extraction et controle des modes de pilotage et fonctionnement (SAE-440)  1.4
* JPL	12/07/23 : Separation de l'acquisition et du traitement de la trame  1.5
* JPL	13/07/23 : Memorisation et ecriture en base de l'etat du controleur de feux (SAE-440)  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include <ctype.h>

#include "xzat81.h"

#include "ecfe_tra.h"

/* definitions de constantes */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "ecfe_tra.c 1.6" ;


/* declaration de fonctions internes */
int em_traitement_msg_trame ( char * );


/* definition de fonctions externes */



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
* va_socket : no socket sur laquelle le message est arrive
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
	XZEXT_MSG_SOCKET	vl_Msg;
	int			vl_IndCnx = 0,
				vl_LgMsg  = 0;

	/* Lecture du message recu dans la socket appelante */
	if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
		return (XDC_NOK);
	}

	return (em_traitement_msg_trame ( vl_Msg ));
}




/*------------------------------------------------------
* SERVICE RENDU : 
*  < Analyse de la trame >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_msg_trame ( char *pa_Msg )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket : no socket sur laquelle le message est arrive
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
   char				*pl_trame_ptr,*pl_trame_ps_ptr;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrip[30];
   int				vl_typetrame;
   int 				vl_typ_rep;
   XZEXT_MSG_SOCKET		pl_trame;
   int				vl_param;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   int		vl_etatCFE = XDC_CFE_PLN_NUL;
   XDY_Booleen	vl_etatCFE_Valide = XDC_VRAI;

   ECFE_DONNEES_CFE		*pl_ListeCFE;
   XDY_Datagroup		pl_DG;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd;   
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   XDY_Mot	                vl_noeqt;
   XZSCT_NomTache          	vl_NomTache   = "";
   XDY_NomMachine          	vl_NomMachine = "";

   char				vl_code_fonction, vl_mode;
   char				vl_fonctionnement = (char) XDC_CFE_FCT_NUL, vl_feu = (char) XDC_CFE_PLN_NUL;
   int				vl_fonction_maj = XDC_CFE_FCT_NUL;
   XDY_LibellePlanDeFeux	vl_libelle;


      XZST_03EcritureTrace( XZSTC_FONCTION, " Msg TEIMI :%s. ", pa_Msg );
      /* Scan du message provenant de la socket */
      vl_param=sscanf(pa_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
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
         XZST_03EcritureTrace( XZSTC_WARNING, " Message socket de format invalide mes=%s", pa_Msg);
         return (XDC_NOK);
      }
      					 
      /* Conversion horodate */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
            
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      
      /* Recherche du CFE concerne */
      for ( pl_ListeCFE = pg_debutCFE; pl_ListeCFE != NULL ; pl_ListeCFE = pl_ListeCFE->Suivant )
      {
         if (!strcmp(pl_ListeCFE->Config.AdresseIP,vl_adrip))
         {
	    vl_noeqt = pl_ListeCFE->Config.Numero;
	       pl_ListeCFE->EtatTrans = ECFEC_EQT_LIBRE;
            break;
         }
      }

      /* eqt non trouve */
      if ( pl_ListeCFE == NULL ) 
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " CFE Non trouve en memoire mes=%s", pa_Msg);
         return (XDC_NOK);
      }

      /* Traitement des alarmes transmission avec l'equipement */
      if ( !(pl_ListeCFE->Etat_Service & XDC_EQT_HORS_SRV) )
           ex_traite_alarme_com ( vg_NomMachine,
				  XDC_EQT_CFE,
				  vl_noeqt,
      				  vl_typ_rep,
      				  vl_nbnonrep,
      				  vl_nberrcrc,
      				  vl_typetrame,
      				  vg_Mode_Fonct,XZSC_22NumeroSiteLocal());
      				  
      if ( (pl_ListeCFE->Etat_Service & XDC_EQT_HORS_SRV) && (vl_typetrame != XZEXC_TRAME_FEN_LCR) )
          return (XDC_NOK);
      

      /* On se positionne sur le debut de la trame */
      pl_trame_ptr=strstr(pa_Msg,XZEXC_SEP_TRAME);
      if (pl_trame_ptr == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Message socket : debut de trame introuvable mes=%s", pa_Msg);
         return (XDC_NOK);
      }
      pl_trame_ptr=pl_trame_ptr+4;
      strcpy ( pl_trame, strtok(pl_trame_ptr,XZEXC_FIN_MSG_SOCK) );
      pl_trame[strlen(pl_trame)-1] = '\0';

      /* Recherche du type de trame */
      switch(vl_typetrame)
      {
         /* C'est une trame de reponse status temps reel */
         case XZEXC_TRAME_STAT_TR:
         {
	       XZST_03EcritureTrace( XZSTC_DEBUG1, "trame XZEXC_TRAME_STAT_TR");
               /* Si le status TR est ko -> demande du status complet */
	       if ( vl_typ_rep == XZEXC_TRAME_NON_TRANSM )
	       {
	          pl_ListeCFE->EtatTrans = ECFEC_EQT_PANNE;
	          return (XDC_NOK);
	       }
	       
	    pl_ListeCFE->StatusTR = ((*pl_trame_ptr == '@')||(*(pl_trame_ptr+1)) == '@') ? XDC_OK : XDC_NOK;
            break;
         }

         /* C'est une trame de reponse etat de positionnement CFE */
         case XZEXC_TRAME_ETAT_POS_CFE:
         {   
	       XZST_03EcritureTrace( XZSTC_DEBUG1, "trame XZEXC_TRAME_ETAT_POS_CFE");

            if (vl_typ_rep==XZEXC_TRAME_NON_TRANSM)
            {
	       pl_ListeCFE->EtatTrans = ECFEC_EQT_PANNE;

	       XZST_03EcritureTrace( XZSTC_WARNING, "XZEXC_TRAME_NON_TRANSM");
               /*if ( (pl_ListeCFE->NumCmd == vl_nocmd) &&  (pl_ListeCFE->NumCmd != XZEXC_NOCMD) )
               {
	       */
		      if (pl_ListeCFE->NumCmd != XZEXC_NOCMD) {
         	      XZST_03EcritureTrace(XZSTC_WARNING,"Echec commande du CFE %d cmd=%d",
         	  			pl_ListeCFE->Config.Numero,
         	  			pl_ListeCFE->NumCmd);
                      xzat521_CompteRendu_CFE(pl_ListeCFE->NumCmd,vl_horodate,XDC_NOK,
						pl_ListeCFE->Config.SiteGestion);
			pl_ListeCFE->NumCmd = XZEXC_NOCMD;
	       		}
               /*
	       }*/

               /* Mise a jour memoire Cmd Module */
               if ( vl_nocmd != XZEXC_NOCMD ) 
		   pl_ListeCFE->EtatCmd=0;

	       vl_etatCFE_Valide = XDC_FAUX;
               break;
            }

	    /* Recherche de la reponse du Controleur de feux (format Diaser) */
	    pl_trame_ps_ptr = strstr(pl_trame_ptr,"D");
	    if ( (pl_trame_ps_ptr == NULL)  ||  strlen (pl_trame_ps_ptr) < 8 ) {
		XZST_03EcritureTrace( XZSTC_WARNING, "Reponse etat de positionnement CFE invalide");
		vl_etatCFE_Valide = XDC_FAUX;
	    }

	    /* Controle du CODE_FONCTION_COMMANDE_CONTROLE */
	    if (vl_etatCFE_Valide == XDC_VRAI) {
		vl_param = sscanf(pl_trame_ps_ptr,"D%c%c%c", &vl_code_fonction, &vl_mode, &vl_fonctionnement);
		if ( (vl_code_fonction != 'I')  &&  (vl_code_fonction != 'i') ) {
		    XZST_03EcritureTrace( XZSTC_WARNING, "Code fonction invalide '%c'", vl_code_fonction);
		    vl_etatCFE_Valide = XDC_FAUX;
		}
	    }

	    /* Controle du Mode */
	    if (vl_etatCFE_Valide == XDC_VRAI) {
		if ( (vl_mode=='A') || (vl_mode=='a') ) {
		    ex_env_alarme ( vg_NomMachine,
		                       vl_horodate,
		                       XZECC_TYPE_EQT_CFE,
		                       pl_ListeCFE->Config.Numero,
		                       XZAMC_DEF_REP_TEL,
		                       "",
		                       "",
		                       XZSC_22NumeroSiteLocal() );
		} else if (strchr ("XABCDEFxabcdef", (int) vl_mode) == NULL) {
		    XZST_03EcritureTrace( XZSTC_WARNING, "Mode invalide '%c'", vl_mode);
		    vl_etatCFE_Valide = XDC_FAUX;
		}
	    }

	    /* Controle du Fonctionnement / Asservissement */
	    if (vl_etatCFE_Valide == XDC_VRAI) {
		vl_fonction_maj = toupper ((int) vl_fonctionnement);
		if (strchr ("ABCDFGIKabcdfgik", (int) vl_fonctionnement) == NULL) {
		    XZST_03EcritureTrace( XZSTC_WARNING, "Mode de fonctionnement invalide '%c'", vl_fonctionnement);
		    vl_etatCFE_Valide = XDC_FAUX;
		}
		else if ( vl_fonction_maj == XDC_CFE_FCT_INIT ) {
		    /* ignorer les phases d'initialisation */
		    break;
		}
	    }

	    /* Extraction du Plan de feux */
	    if (vl_etatCFE_Valide == XDC_VRAI) {
		if ( (vl_fonction_maj == XDC_CFE_FCT_ETEINT) || (vl_fonction_maj == XDC_CFE_FCT_CLIGNO) ) {
		    vl_feu = pl_trame_ps_ptr[4];
		} else {
		    vl_feu = pl_trame_ps_ptr[5];
		}
		XZST_03EcritureTrace( XZSTC_INFO, "vl_feu '%c' vl_mode '%c' vl_fonctionnement '%c'", vl_feu, vl_mode, vl_fonctionnement);

		/* Controle du Plan de feux */
		vl_etatCFE = XDC_CFE_PLN_NUL;
		if (vl_feu != '*') {
		    if ( (vl_feu >= '0')  &&  (vl_feu <= '9') ) {
			vl_etatCFE = vl_feu - '0';
		    }
		    else if ( (vl_feu >= 'A')  &&  (vl_feu <= 'F') ) {
			vl_etatCFE = 10 + (vl_feu - 'A');
		    }
		    else {
			XZST_03EcritureTrace( XZSTC_WARNING, "Plan de feux invalide '%c'", vl_feu);
			vl_etatCFE_Valide = XDC_FAUX;
		    }
		}
	    }

	    if (vl_etatCFE_Valide != XDC_VRAI) {
		vl_fonctionnement = XDC_CFE_FCT_NUL;
		vl_fonction_maj = XDC_CFE_FCT_NUL;
		vl_etatCFE = XDC_CFE_PLN_NUL;
	    }

	    XZST_03EcritureTrace( XZSTC_WARNING,"Etat CFE %d: %d, ancien %d", pl_ListeCFE->Config.Numero, vl_etatCFE, pl_ListeCFE->Etat);

	    /* Si l'etat DU PLAN DE FEUX est different de l'etat precedent alors */
	    if ( pl_ListeCFE->Etat != vl_etatCFE )
            {
	       /* envoyer le message XDM_ETAT_CFE */
	       XDG_EncodeDG2 ( pl_DG, XDG_ETAT_CFE, XZSC23_Nom_From_Numero(pl_ListeCFE->Config.SiteGestion));
	       strcpy(vl_libelle,"");
     	       if (!TipcSrvMsgWrite ( pl_DG,
      			        XDM_IdentMsg(XDM_ETAT_CFE),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeCFE->Config.Numero,
                          	T_IPC_FT_INT4, vl_etatCFE,
				T_IPC_FT_STR, vl_libelle,
                          	NULL))
      	       {
         	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_CFE non effectue." );
	       }
	       else
      	       {
         	  XZST_03EcritureTrace(XZSTC_INFO,"Envoi message XDM_ETAT_CFE effectue. %d %d",
		  		pl_ListeCFE->Config.Numero, vl_etatCFE);
	       }
            }


	    /* Si l'etat DU CONTROLEUR DE FEUX est different de l'etat precedent alors */
	    if ( ! ((vl_fonctionnement == pl_ListeCFE->Mode_Fct)  &&  (vl_etatCFE == pl_ListeCFE->Etat)) )
	    {
		/* ecrire l'etat du controleur en base */
		XZAT81_Ecrire_Etat_CFE (pl_ListeCFE->Config.Numero,
		                           vl_horodate, (XDY_Octet) vl_fonctionnement, (XDY_Octet) vl_etatCFE);
	    }


	    /* Memorisation du nouvel etat */ 
	    pl_ListeCFE->Mode_Fct = vl_fonctionnement;
	    pl_ListeCFE->Etat = vl_etatCFE;

	    /* Si une commande est en cours alors */ 
	    if (pl_ListeCFE->NumCmd != XZEXC_NOCMD) {
		XZST_03EcritureTrace(XZSTC_WARNING,"numcmd=%d, etat commande/reel %d/%d",
		                        pl_ListeCFE->NumCmd, pl_ListeCFE->EtatCmd, pl_ListeCFE->Etat);

		/* si l'etat reel est l'etat commande alors envoyer le compte-rendu */ 
		if (pl_ListeCFE->EtatCmd == pl_ListeCFE->Etat) {
			xzat521_CompteRendu_CFE(pl_ListeCFE->NumCmd,vl_horodate,XDC_OK,
			               pl_ListeCFE->Config.SiteGestion);
			pl_ListeCFE->NumCmd=XZEXC_NOCMD;
			pl_ListeCFE->EtatCmd = 0;
	   	}
	   }

            break;
	 }
      }
}
