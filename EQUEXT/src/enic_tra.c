/*E*/
/*  Fichier : $Id: enic_tra.c,v 1.19 2007/11/20 08:35:07 pc2dpdy Exp $      Release : $Revision: 1.19 $        Date : $Date: 2007/11/20 08:35:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des sockets provenant de TEXAO
* pour la tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	14 Nov 1994	: Creation
* Mismer.D	version 1.2	25 Jan 1995	:
* Mismer.D	version 1.3	28 Jan 1995	:
* Mismer.D	version 1.4	28 Jan 1995	:
* Mismer.D	version 1.5	31 Jan 1995	:
* Mismer.D	version 1.6	31 Jan 1995	:
* Mismer.D	version 1.7	09 Fev 1995	:
* Mismer.D	version 1.8	21 Fev 1995	:
* Mismer.D	version 1.9	17 Mar 1995	:
* Mismer.D	version 1.10	09 Mar 1995	:
* Mismer.D	version 1.11	18 Mai 1995	: Ajout traitement demande relecture des flags cycliquement, Reprise entete, historique
* Mismer.D	version 1.12	12 Oct 1995	: Correction probleme horodate compte rendu action
* Mismer.D	version 1.13	02 Oct 1995	: Ajout ecriture fichier de memorisation des sequences actuelles
* Mismer.D      Version 1.14    15 Nov 1996     : Modif Alarmes (DEM/1232)
* Mismer.D      version 1.15    24 Avr 1997     : Ajout alerte monaco (DEM/1421)
* Mismer.D      version 1.16    23 Jun 1997     : Ajout alerte monaco suite (DEM/1421)
* PN pour Mismer.D 	1.17	18 Nov 1997	: Modif pour pilotage panneau GTC Nice
* PNI 		Version 1.18	12 Jan 2007	: Pas d'envoi de status NOK pour une discordance panneau sur Tunnel DEM609
* JMG   13/11/07        : ajout site de gestion DEM715
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "enic_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la socket de TEXAO
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_traitement ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket	: socket de lecture
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
   XDY_Horodate			vl_Horodate; 
   XDY_Texte			vl_TexteAlarme;
   int				vl_typetrame;
   int				vl_Etat_Service = 0;
   XZEXT_MSG_SOCKET		pl_Msg;
   char	*			vl_Mot;
   short			vl_Etat  [XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN],
   				vl_Change[XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN];
   int				vl_TypeEvt,
   				vl_Adresse, vl_TypeEqt, vl_NumEqt,
   				vl_Index,
   				vl_NumBit1, vl_NumBit2, vl_NumMot1, vl_NumMot2,
   				vl_EtatPan1, vl_EtatPan2,
   				vl_OP, vl_CO;
   ENIC_DONNEES_Panneau	*	pl_Panneau;
   ENIC_DONNEES_Sequence *	pl_Sequence;
   ENIC_DONNEES_Capteur	*	pl_Capt;
   ENIC_DONNEES_Tube *		pl_Tube;
   ENIC_DONNEES_Echangeur *	pl_Ech;
   div_t			vl_div;
   XDY_Datagroup		pl_NomDG;
   XDY_NomSite      	  	pl_NomSite;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      return ( XDC_NOK );
   }

   /*A Si il s'agit d'une deconnexion */
   if ( (vl_IndCnx == XZEXC_DECONNEC) || (vl_LgMsg == 0) )
   {
      /* Ecriture trace . */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
      return ( XDC_NOK );
   }
   
   /*A Scan du message provenant de la socket */
   if ( sscanf ( pl_Msg, 	"%d %d %d %d %lf ", 
   				&vl_TypeEvt, 
   				&vl_Adresse,
   				&vl_TypeEqt,
   				&vl_NumEqt,
   				&vl_Horodate ) != 5 )
   {
      /* Ecriture trace Erreur l */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur syntaxe message TEXA0. " );
      return ( XDC_NOK );
   }
   
   /* Lecture des etats panneaux */
   vl_Mot = strtok ( pl_Msg, " ");
   vl_Mot = strtok ( NULL, " " );
   vl_Mot = strtok ( NULL, " " );
   vl_Mot = strtok ( NULL, " " );
   vl_Mot = strtok ( NULL, " " );
   for ( vl_Index = 0; 
         (vl_Index < XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN) && ( vl_Mot != NULL );
         vl_Index++ )
   {
      if ( ( vl_Mot = strtok ( NULL, " " ) ) != NULL )
      {
         vl_Mot += 2;
         vl_Etat[vl_Index] = 	((*(vl_Mot+0)<'A') ? (*(vl_Mot+0)-'0') : (*(vl_Mot+0)-'A'+10)) * 0x1000 +
         			((*(vl_Mot+1)<'A') ? (*(vl_Mot+1)-'0') : (*(vl_Mot+1)-'A'+10)) * 0x0100 +
         			((*(vl_Mot+2)<'A') ? (*(vl_Mot+2)-'0') : (*(vl_Mot+2)-'A'+10)) * 0x0010 +
         			((*(vl_Mot+3)<'A') ? (*(vl_Mot+3)-'0') : (*(vl_Mot+3)-'A'+10)) * 0x0001 ;
      }
      if ( ( vl_Mot = strtok ( NULL, " " ) ) != NULL )
      {
         vl_Mot += 2;
         vl_Change[vl_Index] = 	((*(vl_Mot+0)<'A') ? (*(vl_Mot+0)-'0') : (*(vl_Mot+0)-'A'+10)) * 0x1000 +
         			((*(vl_Mot+1)<'A') ? (*(vl_Mot+1)-'0') : (*(vl_Mot+1)-'A'+10)) * 0x0100 +
         			((*(vl_Mot+2)<'A') ? (*(vl_Mot+2)-'0') : (*(vl_Mot+2)-'A'+10)) * 0x0010 +
         			((*(vl_Mot+3)<'A') ? (*(vl_Mot+3)-'0') : (*(vl_Mot+3)-'A'+10)) * 0x0001 ;
      }
   }
	 
      
    /*A Recherche dans la liste des tubes si type evenement tube*/
    for ( pl_Tube = pg_debutTube; 
          (vl_TypeEqt == XDC_EQT_TUBE) && (pl_Tube != NULL); 
          pl_Tube = pl_Tube->Suivant )
    {
      if ( pl_Tube->Numero == vl_NumEqt )
      {
         vl_Etat_Service = pl_Tube->Etat_Service;
         break;
      }
    }
       
    /*A Recherche dans la liste des echangeurs si type evenement echangeur */
    for ( pl_Ech = pg_debutEchangeur; 
          (vl_TypeEqt == XDC_EQT_ECH) && (pl_Ech != NULL);
          pl_Ech = pl_Ech->Suivant )
    {
      if ( pl_Ech->NumeroDemiEch == vl_NumEqt )
      {
         vl_Etat_Service = pl_Ech->Etat_Service;
         break;
      }
    }
            
    /* Retour a l'appelant si echangeur ou tube non trouve */      
    if ( (pl_Tube == NULL) && (pl_Ech == NULL) ) return;
      					 
    /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
    {
       /* Ecriture Trace */
       XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
    } 

    /*A Recherche du type d'evenement */
    switch( vl_TypeEvt )
    {
         /*A C'est une Telealarame */
         case XZETC_TELEALARME :
            /* Memorisation etat des telealarmes */
	    if ( vl_TypeEqt == XDC_EQT_TUBE )
	    {
               pl_Tube->EtatTelealarme = vl_Etat[0];
               if ( pl_Tube->Etat_Service != 0 )
                  break;
	    }
	    else
	    {
               pl_Ech->EtatTelealarme = vl_Etat[0];
               if ( pl_Ech->Etat_Service != 0 )
                  break;
	    }
               
            /*A Envoie Alerte ou alarme selon type de telealarme */
            /*A Si alarme energie */
            if ( vl_Change[0] & XZETC_BIT_0 )
            {
               /* Envoie Alarme Energie Tube */
  	        ex_env_alarme (	vg_NomMachine,
				vl_Horodate,
				vl_TypeEqt,
				(vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->Numero : pl_Ech->NumeroDemiEch,
				XZAMC_ENERGIE,
				(vl_Etat[0] & XZETC_BIT_0) ? XDC_VRAI : XDC_FAUX,
				NULL,
				XZSC_22NumeroSiteLocal());
            }
                  
            /*A Si alerte signalisation en local */ 
            if ( (vl_Change[0] & XZETC_BIT_1) && (vl_Etat[0] & XZETC_BIT_1) )
            {
               /* Envoie Alerte Tube en local */
               ex_env_alerte  ( vl_Horodate,
                                vl_TypeEqt,
                                vl_NumEqt,
                                XZAAC_SIG_LOCAL,
                                vg_Mode_Fonct,
				XZSC_22NumeroSiteLocal(),
                                NULL ); 
            }                

            /*A Si alarme defaut lampe */ 
            if ( vl_Change[0] & XZETC_BIT_2 )
            {
               /* Envoie Alarme lampe */
  	       ex_env_alarme (	vg_NomMachine,
				vl_Horodate,
				vl_TypeEqt,
				(vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->Numero : pl_Ech->NumeroDemiEch,
				XZAMC_LAMPE,
				(vl_Etat[0] & XZETC_BIT_2) ? XDC_VRAI : XDC_FAUX,
				NULL ,
				XZSC_22NumeroSiteLocal());
            }                
            
            /* Envoi etat tube dans XDM_ETAT_TELEALARME sur XDG_ETAT_TELEALARME */
	    sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_TELEALARME, pl_NomSite );
            if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_TELEALARME),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, vl_TypeEqt,
                          	    	T_IPC_FT_INT4, vl_NumEqt,
                          	    	T_IPC_FT_INT4, (vl_Etat[0] & XZETC_BIT_0) ? 1 : 0,
                          	    	T_IPC_FT_INT4, (vl_Etat[0] & XZETC_BIT_1) ? 1 : 0,
                          	    	T_IPC_FT_INT4, (vl_Etat[0] & XZETC_BIT_2) ? 1 : 0,
                          	    	NULL ) )
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_TELEALARME non effectue." );
            }
            break;
            
         /*A C'est une ETOR */
         case XZETC_ETOR :
            /* Recherche bit evenement */
            for ( vl_Index = 0, vl_NumBit1 = 1; vl_Index < 16; vl_Index++, vl_NumBit1 = vl_NumBit1 << 1 )
            {
               if ( vl_Change[0] & vl_NumBit1 )
               {
                  /* Recherche capteur concernee */
                  for ( pl_Capt = pl_Tube->ListeCapteur; pl_Capt != NULL; pl_Capt = pl_Capt->Suivant )
                  {
                      if ( (pl_Capt->NumBit == vl_Index+1) && ((pl_Capt->Type == XDC_EQT_EXT) || (pl_Capt->Type == XDC_EQT_LIT)) )
                      {
                         /* Memorisation etat Capteur */
                         pl_Capt->EtatCapteur = ( vl_Etat[0] & vl_NumBit1 ) ? 1 : 0;
                         /* Envoi alerte Capteur */
                         if ( (pl_Capt->EtatCapteur == 1) &&
                              !pl_Capt->Etat_Service &&
                              !pl_Tube->Etat_Service ) 
                         {
                            if ( (vl_Adresse == pl_Tube->AdresseExtincteur) && (pl_Capt->Type == XDC_EQT_EXT) )
                            {
                                ex_env_alerte  ( vl_Horodate,
                                             	 pl_Capt->Type,
                                             	 pl_Capt->NumeroCapteur,
                                             	 pl_Capt->NumAlerte,
                                             	 vg_Mode_Fonct,
						 XZSC_22NumeroSiteLocal(),
                                             	 NULL );
                            }

                            if ( (vl_Adresse == pl_Tube->AdresseLit ) && (pl_Capt->Type == XDC_EQT_LIT) )
                            {
                                ex_env_alerte  ( vl_Horodate,
                                             	 pl_Capt->Type,
                                             	 pl_Capt->NumeroCapteur,
                                             	 pl_Capt->NumAlerte,
                                             	 vg_Mode_Fonct,
						 XZSC_22NumeroSiteLocal(),
                                             	 NULL );
                            }
                         }
                         /* Envoi etat Capteur dans XDM_ETAT_CAPTEUR sur XDG_ETAT_CAPTEUR */
                         if ( !(pl_Tube->Etat_Service & XDC_EQT_HORS_SRV) )
                         {
	                    sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_CAPTEUR, pl_NomSite );
                            if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	        	XDM_IdentMsg(XDM_ETAT_CAPTEUR),
                          	    	        	XDM_FLG_SRVMSGWRITE,
                          	    	        	T_IPC_FT_INT4, pl_Tube->Numero,
                          	    	        	T_IPC_FT_INT4, pl_Capt->NumeroCapteur,
                          	    	        	T_IPC_FT_INT4, pl_Capt->EtatCapteur,
                          	    	        	NULL ) )
                            {
                                XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_CAPTEUR non effectue." );
                            }
                         }
                      }
                  }
               }
            }
            break;
               
         /*A C'est une discordance panneau  */
         case XZETC_DISCORDANCE :
            /* Si discordance panneau */
            vl_NumMot1 = 0;
            vl_NumMot2 = ( (vl_TypeEqt == XDC_EQT_TUBE) ? ((pl_Tube->NbrPanFAV <= 16) ? 0 : 1) : ((pl_Tube->NbrPanFAV <= 16) ? 0 : 1) );
            if ( (vl_Change[vl_NumMot1] != 0) || (vl_Change[vl_NumMot2] != 0) )
            {
               /* Recherche panneau concernee */
               for ( pl_Panneau = (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->ListePanneau : pl_Ech->ListePanneau;
                     pl_Panneau != NULL; 
                     pl_Panneau = pl_Panneau->Suivant )
               {
                  vl_NumBit1 = ( pl_Panneau->BitDiscordance  > 16 ) ? pl_Panneau->BitDiscordance-17 : pl_Panneau->BitDiscordance-1;
                  vl_NumMot1 = ( pl_Panneau->BitDiscordance  > 16 ) ? 1 : 0;
                  vl_NumBit2 = ( pl_Panneau->BitDiscordance2 > 16 ) ? pl_Panneau->BitDiscordance2-17 : pl_Panneau->BitDiscordance2-1;
                  vl_NumMot2 = ( pl_Panneau->BitDiscordance2 > 16 ) ? 1 : 0;
                  if ( ( (vl_Change[vl_NumMot1] & (1 << vl_NumBit1)) ) || 
                       ( (pl_Panneau->BitDiscordance2 != 0) && (vl_Change[vl_NumMot2] & (1 << vl_NumBit2)) ) )
                  {
                     /* Memorisation Etat panneau */
                     pl_Panneau->EtatDisc = (vl_Etat[vl_NumMot1] & (1 << vl_NumBit1)) || ((vl_NumBit2 != 0) && (vl_Change[vl_NumMot2] & (1 << vl_NumBit2)));
                     /* Envoyer Alarme discordance panneau */
                     if ( !pl_Panneau->Etat_Service && !vl_Etat_Service )
                        ex_env_alarme (	vg_NomMachine,
					vl_Horodate,
					XDC_EQT_PAN,
					pl_Panneau->NumeroPanneau,
					XZAMC_DISC_PANNEAU,
					(pl_Panneau->EtatDisc) ? XDC_VRAI : XDC_FAUX,
					NULL,
					XZSC_22NumeroSiteLocal());

                     /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur XDG_ETAT_PANNEAU */
	             sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PANNEAU, pl_NomSite );
                     if ( !(pl_Panneau->Etat_Service & XDC_EQT_HORS_SRV) && !(vl_Etat_Service & XDC_EQT_HORS_SRV) )
                        if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	        XDM_IdentMsg(XDM_ETAT_PANNEAU),
                          	    	        XDM_FLG_SRVMSGWRITE,
                          	    	        T_IPC_FT_INT4, vl_TypeEqt,
                          	    	        T_IPC_FT_INT4, vl_NumEqt,
                          	    	        T_IPC_FT_INT4, pl_Panneau->NumeroPanneau,
                           	    	        T_IPC_FT_INT4, pl_Panneau->EtatPanneau,
                           	    	        T_IPC_FT_INT4, pl_Panneau->EtatDisc,
                          	    	        NULL ) )
                        {
                           XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PANNEAU non effectue." );
                        }
                  }
               }
            }
            
            /* Si discordance sequence */
            vl_NumMot1 = ( (vl_TypeEqt == XDC_EQT_TUBE) ? ((pl_Tube->NbrPanFAV <= 16) ? 1 : 2) : ((pl_Ech->NbrPanFAV <= 16) ? 1 : 2) );
            if ( vl_Change[vl_NumMot1] != 0 )
            {
               /* Recherche si sequence concernee */
               for ( pl_Sequence = (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->ListeSequence : pl_Ech->ListeSequence;
                     pl_Sequence != NULL; 
                     pl_Sequence = pl_Sequence->Suivant )
               {
                  vl_NumBit1 = pl_Sequence->BitDiscordance - 1;
                  /* Memorisation etat sequence */
                  pl_Sequence->EtatDisc = vl_Etat[vl_NumMot1] & (1 << vl_NumBit1);
                  if ( vl_Change[vl_NumMot1] & (1 << vl_NumBit1) )  
                  {
                     /* Envoyer compte rendu action a BASEDD */
                     if ( (vl_TypeEqt == XDC_EQT_TUBE) && !pl_Tube->Etat_Service )
                     {
                        if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction > 0) )
                        {
                           /* Appel de XZAT22 avec XDC_OK et NumAction */ /* DEM609 12/01/07 : Envoi Status OK en cas de discordance */
                           XZST_03EcritureTrace(XZSTC_INFO, "Discordance Envoi OK pour Tube:%d Cmd:%d Seq:%d Disc.:%d.",
                   		 	pl_Tube->Numero,
                   		 	pl_Sequence->NumAction,
                   		 	pl_Sequence->NumSequence ,pl_Sequence->EtatDisc);
	                   xzat22_CompteRendu_Tube ( pl_Sequence->NumAction, vl_Horodate, XDC_OK);
	                }
	                
                        if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction <= 0) )
                        {
                           /* Envoyer alerte discordance sequence */
                           ex_env_alerte  ( 	vl_Horodate,
                                      		XDC_EQT_TUB,
                                      		pl_Tube->Numero,
                                      		XZAAC_AFF_DIF,
                                      		vg_Mode_Fonct,
						XZSC_22NumeroSiteLocal(),
                                      		pl_Sequence->NumSequence,
                                      		NULL );
                        }
                     }
                     if ( (vl_TypeEqt == XDC_EQT_ECH) && !pl_Ech->Etat_Service )
                     {
                       /* Envoyer compte rendu action a BASEDD */
                        if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction > 0) )
                        {
                           /* Appel de XZAT23 avec XDC_NOK et NumAction */
                           XZST_03EcritureTrace(XZSTC_INFO, "Envoi NOK pour Echangeur:%d Cmd:%d Seq:%d.",
                   		 	pl_Ech->NumeroDemiEch,
                   		 	pl_Sequence->NumAction,
                   		 	pl_Sequence->NumSequence );
	                   xzat23_CompteRendu_DemiEch ( pl_Sequence->NumAction, vl_Horodate, XDC_NOK);
	                }
	                
                        if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction <= 0) )
                        {
                           /* Envoyer alerte discordance sequence */
                           ex_env_alerte  ( 	vl_Horodate,
                                      		XDC_EQT_ECH,
                                      		pl_Ech->NumeroDemiEch,
                                      		XZAAC_AFF_DIF,
                                      		vg_Mode_Fonct,
						XZSC_22NumeroSiteLocal(),
                                      		pl_Sequence->NumSequence,
                                      		NULL );
                                      		
                        }
                     }
                     /* Envoyer Alarme discordance sequence */
                     sprintf ( vl_TexteAlarme, "%d", pl_Sequence->NumSequence );
                     if ( !vl_Etat_Service )
                        ex_env_alarme (	vg_NomMachine,
					vl_Horodate,
					vl_TypeEqt,
					vl_NumEqt,
					XZAMC_DISC_SEQUENCE,
					(vl_Etat[vl_NumMot1] & (1 << vl_NumBit1)) ? XDC_VRAI : XDC_FAUX,
					vl_TexteAlarme,
					XZSC_22NumeroSiteLocal);
                     
                     /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
	             sprintf ( pl_NomDG, "%s_%s", (vl_TypeEqt == XDC_EQT_TUBE) ? XDG_ETAT_SEQ_TUBE : XDG_ETAT_SEQ_ECH, pl_NomSite );
                     if ( !(vl_Etat_Service & XDC_EQT_HORS_SRV) )
                        if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	        XDM_IdentMsg((vl_TypeEqt == XDC_EQT_TUBE) ? XDM_ETAT_SEQUENCE_TUBE : XDM_ETAT_SEQUENCE_ECH),
                          	    	        XDM_FLG_SRVMSGWRITE,
                          	    	        T_IPC_FT_INT4, vl_TypeEqt,
                          	    	        T_IPC_FT_INT4, vl_NumEqt,
                          	    	        T_IPC_FT_INT4, (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->DerniereSequence : pl_Ech->DerniereSequence,
                           	    	        T_IPC_FT_INT4, pl_Sequence->NumSequence,
                           	    	        T_IPC_FT_INT4, vl_Etat[vl_NumMot1] & (1 << vl_NumBit1),
                         	    	        NULL ) )
                        {
                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE non effectue." );
                        }
                     
                  }
               }
            }
            
            /*A Si evenement seuil CO ou opacite */
            vl_NumMot1 = (pl_Tube->NbrPanFAV <= 16) ? 2 : 3;
            if ( (vl_TypeEqt == XDC_EQT_TUBE) && ( vl_Change[vl_NumMot1] != 0) ) 
            {
               for ( vl_Index = 0, vl_NumBit1 = 1; vl_Index < 16; vl_Index++, vl_NumBit1 = vl_NumBit1 << 1 )
               {
                  if ( vl_Change[vl_NumMot1] & vl_NumBit1 )
                  {
                     /*A Recherche capteur concernee */
                     for ( pl_Capt = pl_Tube->ListeCapteur; pl_Capt != NULL; pl_Capt = pl_Capt->Suivant )
                     {
                         if ( (pl_Capt->NumBit == vl_Index+1) && ((pl_Capt->Type==XDC_EQT_CCO)||(pl_Capt->Type==XDC_EQT_OPA)) )
                         {
                            /*A Memorisation etat Capteur */
                            pl_Capt->EtatCapteur = ( vl_Etat[vl_NumMot1] & vl_NumBit1 ) ? 1 : 0;
                            /*A Envoi alerte Capteur */
                            if ( (pl_Capt->EtatCapteur == 1) &&
                                 !pl_Capt->Etat_Service &&
                                 vl_Etat_Service )
                            {
                               ex_env_alerte  ( vl_Horodate,
                                             pl_Capt->Type,
                                             pl_Capt->NumeroCapteur,
                                             pl_Capt->NumAlerte,
                                             vg_Mode_Fonct,
					     XZSC_22NumeroSiteLocal(),
                                             NULL );
                            }
                         }
                      }
                  }
               }
            }
                  
            break;

         /*A C'est une Alerte */
         case XZETC_ALERTE :
            /* Recherche capteur concernee */
           for ( pl_Capt = pl_Tube->ListeCapteur; pl_Capt != NULL; pl_Capt = pl_Capt->Suivant )
           {
              if ( ((pl_Capt->Type == XDC_EQT_GAB) && (vl_Adresse == pl_Tube->AdresseGabarit)) ||
                   ((pl_Capt->Type == XDC_EQT_VIT) && (vl_Adresse == pl_Tube->AdresseVitesse)) || 
                   ((pl_Capt->Type == XDC_EQT_NIC) && (vl_Adresse == pl_Tube->AdresseNiche)) )
              {
                 /* Recherche bit evenement */
                 vl_div = div ( (pl_Capt->NumBit-1), 16 );
                 vl_NumBit1 = vl_div.rem;
                 vl_NumMot1 = vl_div.quot;
                 if ( vl_Change[vl_NumMot1] & (1 << vl_NumBit1) )
                 {
                     /* Memorisation etat Capteur */
                     pl_Capt->EtatCapteur = ( vl_Etat[vl_NumMot1] & (1 << vl_NumBit1) ) ? 1 : 0;
                     /* Envoi alerte Capteur */
                     if ( (pl_Capt->EtatCapteur == 1) && !pl_Capt->Etat_Service && !pl_Tube->Etat_Service ) 
                     {
                         ex_env_alerte  (    vl_Horodate,
                                             pl_Capt->Type,
                                             pl_Capt->NumeroCapteur,
                                             pl_Capt->NumAlerte,
                                             vg_Mode_Fonct,
					     XZSC_22NumeroSiteLocal(),
                                             NULL );
                      }
                      /* Envoi etat Capteur dans XDM_ETAT_CAPTEUR sur XDG_ETAT_CAPTEUR */
                      if ( !(pl_Tube->Etat_Service & XDC_EQT_HORS_SRV) )
                      {
	                 sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_CAPTEUR, pl_NomSite );
                         if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	        	XDM_IdentMsg(XDM_ETAT_CAPTEUR),
                          	    	        	XDM_FLG_SRVMSGWRITE,
                          	    	        	T_IPC_FT_INT4, pl_Tube->Numero,
                          	    	        	T_IPC_FT_INT4, pl_Capt->NumeroCapteur,
                          	    	        	T_IPC_FT_INT4, pl_Capt->EtatCapteur,
                          	    	        	NULL ) )
                         {
                             XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_CAPTEUR non effectue." );
                         }
                      }
                  }
               }
            }
            break;
            
         /*A C'est un changement d'etat d'un panneau */
         case XZETC_PANNEAU :
            /* Recherche pour tout les panneaux si il y a un evenement */
            for ( pl_Panneau = (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->ListePanneau : pl_Ech->ListePanneau;
                  pl_Panneau != NULL; 
                  pl_Panneau = pl_Panneau->Suivant )
            { 
               vl_div = div ( (pl_Panneau->RangEtat-1), XZETC_NB_ETAT_PAN );
               vl_NumBit1 = vl_div.rem;
               vl_NumMot1 = vl_div.quot;
               vl_div = div ( (pl_Panneau->RangEtat2-1), XZETC_NB_ETAT_PAN );
               vl_NumBit2 = vl_div.rem;
               vl_NumMot2 = vl_div.quot;
               
               if ( ( (vl_Change[vl_NumMot1] & (XZETC_MASQUE_PAN << (vl_NumBit1*XZETC_MAX_BIT_PAN))) ) || 
                    ( (pl_Panneau->RangEtat2 != 0) && (vl_Change[vl_NumMot2] & (XZETC_MASQUE_PAN << (vl_NumBit2*XZETC_MAX_BIT_PAN))) ) )
               {
                  /* Calcul de l'etat du panneau */
                  pl_Panneau->EtatPanneau = 0;
                  vl_EtatPan1 = vl_Etat[vl_NumMot1] & (XZETC_MASQUE_PAN << (vl_NumBit1*XZETC_MAX_BIT_PAN));
                  vl_EtatPan2 = vl_Etat[vl_NumMot2] & (XZETC_MASQUE_PAN << (vl_NumBit2*XZETC_MAX_BIT_PAN));
                  for ( vl_Index = 0; vl_Index < XZETC_MAX_BIT_PAN; vl_Index++ )
                  {
                     if ( vl_EtatPan1 & (1 << ((vl_NumBit1*XZETC_MAX_BIT_PAN) + vl_Index)) )
                     {
                        if ( pl_Panneau->EtatPanneau == 0 )
                        {
                            pl_Panneau->EtatPanneau = vl_Index+1;
                        }
                        else
                        {
                            pl_Panneau->EtatPanneau = 0;
                            break;
                        }
                     }
                     if ( (vl_Index < XZETC_MAX_BIT_PAN) &&
                          (pl_Panneau->RangEtat2 != 0) &&
                          (vl_EtatPan2 & (1 << ((vl_NumBit2*XZETC_MAX_BIT_PAN) + vl_Index ))) )
                     {
                        if ( pl_Panneau->EtatPanneau == 0 )
                        {
                            pl_Panneau->EtatPanneau = vl_Index+1+XZETC_MAX_BIT_PAN;
                        }
                        else
                        {
                            pl_Panneau->EtatPanneau = 0;
                            break;
                        }
                     }
                            
                  }
                  
                  /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur XDG_ETAT_PANNEAU */
	          sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PANNEAU, pl_NomSite );
                  if ( !(pl_Panneau->Etat_Service & XDC_EQT_HORS_SRV) && !(vl_Etat_Service & XDC_EQT_HORS_SRV) )
                     if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	        XDM_IdentMsg(XDM_ETAT_PANNEAU),
                          	    	        XDM_FLG_SRVMSGWRITE,
                          	    	        T_IPC_FT_INT4, vl_TypeEqt,
                          	    	        T_IPC_FT_INT4, vl_NumEqt,
                          	    	        T_IPC_FT_INT4, pl_Panneau->NumeroPanneau,
                           	    	        T_IPC_FT_INT4, pl_Panneau->EtatPanneau,
                           	    	        T_IPC_FT_INT4, pl_Panneau->EtatDisc,
                         	    	        NULL ) )
                     {
                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_PANNEAU non effectue." );
                     }
                 
               }
            }
            break;
   }

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des receptions de fin de time-out discordance sequence
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_traitement_timers ( )

/*
* ARGUMENTS EN ENTREE :
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
*  
------------------------------------------------------*/
{
   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   XDY_Horodate			vl_Horodate;   
   XZSMT_Horodate          	pl_StrHorodate;
   XZEXT_MSG_SOCKET		pl_Msg;
   XDY_Mot			vl_NumSeq, vl_TypeEqt, vl_NumEqt;
   ENIC_DONNEES_Sequence *	pl_Sequence;
   ENIC_DONNEES_Tube *		pl_Tube;
   ENIC_DONNEES_Echangeur *	pl_Ech;
   int				vl_Delai;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( vg_SockTETIM, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket timer. " );
      return ( XDC_NOK );
   }

   /* Si le message est correct et si ce n'est pas une deconnexion */
   if ( (vl_LgMsg == 0) || (vl_IndCnx == XZEXC_DECONNEC) || (strstr(pl_Msg, XZEXC_MSG_REVEIL) == NULL) )
   {
      /* Ecriture trace message socket timer incorrect. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket TETIM incorrect. " );
      return ( XDC_NOK );
   }
   
   sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d Nice%hd_%hd_%hd ",
  	  					&vl_Horodate,
  	  					&vl_Delai,
  	     					&vl_TypeEqt,
  	     					&vl_NumEqt,
						&vl_NumSeq );
  
   if ( vl_Delai == ENICC_REVEIL_15MN )
   {

         /* Construction du message de demande de relecture */
         sprintf ( pl_Msg, "%s %s %s", XZEXC_DEMANDE_INIT, vg_SockTEXA0, XZEXC_FIN_MSG_SOCK );
         /* Envoie de la demande d'init */
         sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTEXA0, pl_Msg, strlen (pl_Msg) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init." );
         }
         sem_post(&vg_semaphore); 
         return (XDC_OK);

   
   }  
  	  					         
   if ( vl_TypeEqt == XDC_EQT_TUBE )
   {
      /*A Recherche dans la liste des tubes */
      for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
      {
         if ( pl_Tube->Numero ==  vl_NumEqt )
         {
             /* Recherche de la sequence concernee */
             for ( pl_Sequence = pl_Tube->ListeSequence; pl_Sequence != NULL; pl_Sequence = pl_Sequence->Suivant )
             {
                if ( (pl_Sequence->NumSequence == vl_NumSeq) && (pl_Sequence->NumAction != 0) && (pl_Sequence->EtatDisc == 0) )
                {
                   /* Appel de XZAT22 avec XDC_OK et NumAction */
	           xzat22_CompteRendu_Tube ( pl_Sequence->NumAction, vl_Horodate, XDC_OK);
                   XZSM_11ConversionHorodate ( vl_Horodate, &pl_StrHorodate );
                   XZST_03EcritureTrace(XZSTC_WARNING, "Envoi CR OK pour Tube:%d Cmd:%d Seq:%d H:%s.",
                   		 pl_Tube->Numero,
                   		 pl_Sequence->NumAction,
                   		 pl_Sequence->NumSequence,
                   		 pl_StrHorodate );
                   et_wfic_derniere_seq ( XDC_EQT_TUBE );
                   break;
                }
             }
          }
       }
    }
    else
    {
       /*A Recherche dans la liste des echangeurs */
       for ( pl_Ech = pg_debutEchangeur; pl_Ech != NULL; pl_Ech = pl_Ech->Suivant )
       {
          if ( pl_Ech->NumeroDemiEch ==  vl_NumEqt )
          {
             /* Recherche de la sequence concernee */
             for ( pl_Sequence = pl_Ech->ListeSequence; pl_Sequence != NULL; pl_Sequence = pl_Sequence->Suivant )
             {
                if ( (pl_Sequence->NumSequence == vl_NumSeq) && (pl_Sequence->NumAction != 0) && (pl_Sequence->EtatDisc == 0) )
                {
                   /* Appel de XZAT23 avec XDC_OK et NumAction */
	           xzat23_CompteRendu_DemiEch ( pl_Sequence->NumAction, vl_Horodate, XDC_OK);
                   XZSM_11ConversionHorodate ( vl_Horodate, &pl_StrHorodate );
                   XZST_03EcritureTrace(XZSTC_WARNING, "Envoi CR OK pour Echangeur:%d Cmd:%d Seq:%d H:%s.",
                   		 pl_Ech->NumeroDemiEch,
                   		 pl_Sequence->NumAction,
                   		 pl_Sequence->NumSequence,
                   		 pl_StrHorodate );
                   et_wfic_derniere_seq ( XDC_EQT_ECH );
                   break;
                }
             }
          }
       }   
    }
    
    return ( XDC_OK );
	        
}






	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
