/*E*/
/*Fichier : $Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 18:09:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des fonction de commande ( Callback RTworks )
* pour la tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Mismer.D	version 1.2	31 Jan 1995	:
* Mismer.D	version 1.3	09 Fev 1995	:
* Mismer.D	version 1.4	21 Fev 1995	:
* Mismer.D	version 1.5	09 Mar 1995	:
* Mismer.D	version 1.6	17 Mar 1995	:
* Mismer.D	version 1.7	09 Avr 1995	:
* Mismer.D	version 1.8	25 Avr 1995	:
* Volcic.F	version 1.9	04 Mai 1995	:
* Fontaine.C	version 1.10	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.11	16 Nov 1995	: Correction init Abt IHM des etat sequence
* PN pour Mismer.D	1.12	18 Nov 1997	: Modif pour pilotage Panneau GTC Nice
* JMG		1.13 	15/01/20 : EMS 
* GR            1.14    09/07/2020 : EMS 
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "enic_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ : enic_cmd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM GTCNice
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_Init_IHM_TNIC_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_EInit_IHM_TNIC
*
* FONCTION 
*  Renvoyer l'etat  demande
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ : et_Init_IHM_TNIC_cb " ;

 
   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   T_INT4			vl_NumTE,
   				vl_TypeEqt,
   				vl_TypeInit;
   #else
   XDY_Datagroup                pl_DGInit;
   XDY_Entier			vl_NumTE,
   				vl_TypeEqt,
   				vl_TypeInit;
   #endif   

   ENIC_DONNEES_Panneau	*	pl_Panneau;
   ENIC_DONNEES_Capteur	*	pl_Capteur;
   ENIC_DONNEES_Sequence *	pl_Sequence;
   ENIC_DONNEES_Tube *		pl_Tube;
   ENIC_DONNEES_Echangeur *	pl_Ech;

		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_TNIC */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
               T_IPC_FT_INT4, &vl_TypeEqt,
               T_IPC_FT_INT4, &vl_NumTE,
               T_IPC_FT_INT4, &vl_TypeInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "Messsage XDM_EInit_IHM_TNIC non lu ." );
      return;
   }
   
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_FONCTION ," IN  : et_Init_IHM_TNIC_cb DGInit: %s Eqt: %d",
                          pl_DGInit,
                          vl_NumTE );
	        
	        
   for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
   {
      switch ( vl_TypeInit )
      {
         case XZETC_TELEALARME :
            /* Envoi etat telealarme dans XDM_ETAT_TELEALARME sur pl_DGInit */
            if ( (vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Tube->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_TELEALARME),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, XDC_EQT_TUBE,
                          	    	T_IPC_FT_INT4, pl_Tube->Numero,
                          	    	T_IPC_FT_INT4, (pl_Tube->EtatTelealarme & XZETC_BIT_0) ? 1 : 0,
                          	    	T_IPC_FT_INT4, (pl_Tube->EtatTelealarme & XZETC_BIT_1) ? 1 : 0,
                          	    	T_IPC_FT_INT4, (pl_Tube->EtatTelealarme & XZETC_BIT_2) ? 1 : 0,
                          	    	NULL ) )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_TELEALARME non effectue." );
               }
            }
            break;
            
         case XZETC_CAPTEUR :
            /* Envoi etat capteur dans XDM_ETAT_CAPTEUR sur pl_DGInit */
            if ( (vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Tube->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               for ( pl_Capteur = pl_Tube->ListeCapteur;
                     pl_Capteur != NULL; 
                     pl_Capteur = pl_Capteur->Suivant )
               {
                  if ( !(pl_Capteur->Etat_Service & XDC_EQT_HORS_SRV) )
                  {
                     if (!TipcSrvMsgWrite (	pl_DGInit,
      	      	      	                 	XDM_IdentMsg(XDM_ETAT_CAPTEUR),
                          	         	XDM_FLG_SRVMSGWRITE,
                          	          	T_IPC_FT_INT4, pl_Tube->Numero,
                          	          	T_IPC_FT_INT4, pl_Capteur->NumeroCapteur,
                          	    		T_IPC_FT_INT4, pl_Capteur->EtatCapteur,
                         	    		NULL ) )
                     {
                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_CAPTEUR non effectue." );
                     }
                  }
               }
            }
            break;
            
         case XZETC_SEQUENCE :
            /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE_TUBE sur pl_DGInit */
            if ( (vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Tube->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               if (!TipcSrvMsgWrite (	pl_DGInit,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_SEQUENCE_TUBE),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, XDC_EQT_TUBE,
                          	    	T_IPC_FT_INT4, pl_Tube->Numero,
                           	    	T_IPC_FT_INT4, pl_Tube->DerniereSequence,
                           	    	T_IPC_FT_INT4, 0,
                           	    	T_IPC_FT_INT4, 0,
                         	    	NULL ) )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE_TUBE non effectue." );
               }
            }
            break;

         case XZETC_PANNEAU :
            if ( (vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Tube->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               for ( pl_Panneau = pl_Tube->ListePanneau;
                     pl_Panneau != NULL; 
                     pl_Panneau = pl_Panneau->Suivant )
               {
                  /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur pl_DGInit */
                  if ( !(pl_Panneau->Etat_Service & XDC_EQT_HORS_SRV) )
                  {
                     if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	        XDM_IdentMsg(XDM_ETAT_PANNEAU),
                          	    	        XDM_FLG_SRVMSGWRITE,
                          	    	        T_IPC_FT_INT4, XDC_EQT_TUBE,
                          	    	        T_IPC_FT_INT4, pl_Tube->Numero,
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
            break;
      }
   }
         
       
  for ( pl_Ech = pg_debutEchangeur; pl_Ech != NULL; pl_Ech = pl_Ech->Suivant )
   {
      switch ( vl_TypeInit )
      {
         case XZETC_TELEALARME :
            /* Envoi etat tube dans XDM_ETAT_TELEALARME sur pl_DGInit */
            if ( (vl_TypeEqt == XDC_EQT_ECH) && 
                 ((pl_Ech->NumeroDemiEch == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Ech->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_TELEALARME),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, XDC_EQT_ECH,
                          	    	T_IPC_FT_INT4, pl_Ech->NumeroDemiEch,
                          	    	T_IPC_FT_INT4, (pl_Ech->EtatTelealarme & XZETC_BIT_0) ? 1 : 0,
                          	    	T_IPC_FT_INT4, (pl_Ech->EtatTelealarme & XZETC_BIT_1) ? 1 : 0,
                          	    	T_IPC_FT_INT4, (pl_Ech->EtatTelealarme & XZETC_BIT_2) ? 1 : 0,
                          	    	NULL ) )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_TELEALARME non effectue." );
               }
            }
            break;
            
         case XZETC_SEQUENCE :
            /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE_ECH sur pl_DGInit */
            if ( (vl_TypeEqt == XDC_EQT_ECH) && 
                 ((pl_Ech->NumeroDemiEch == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Ech->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_SEQUENCE_ECH),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, XDC_EQT_ECH,
                          	    	T_IPC_FT_INT4, pl_Ech->NumeroDemiEch,
                           	    	T_IPC_FT_INT4, pl_Ech->DerniereSequence,
                           	    	T_IPC_FT_INT4, 0,
                           	    	T_IPC_FT_INT4, 0,
                         	    	NULL ) )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE_ECH non effectue." );
               }
            }
            break;

         case XZETC_PANNEAU :
            if ( (vl_TypeEqt == XDC_EQT_ECH) && 
                 ((pl_Ech->NumeroDemiEch == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) &&
                 !(pl_Ech->Etat_Service & XDC_EQT_HORS_SRV) )
            {
               for ( pl_Panneau = pl_Ech->ListePanneau;
                     pl_Panneau != NULL; 
                     pl_Panneau = pl_Panneau->Suivant )
               {
                  /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur pl_DGInit */
                  if ( !(pl_Panneau->Etat_Service & XDC_EQT_HORS_SRV) )
                  {
                     if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	        XDM_IdentMsg(XDM_ETAT_PANNEAU),
                          	    	        XDM_FLG_SRVMSGWRITE,
                          	    	        T_IPC_FT_INT4, XDC_EQT_ECH,
                          	    	        T_IPC_FT_INT4, pl_Ech->NumeroDemiEch,
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
            break;
      }
   }
  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_Init_IHM_TNIC_cb ");
	       
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi un message de commande panneau a TEXA0.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int      et_envoi_cmd_panneau ( XDY_Mot		va_Adresse,
			    	XDY_Mot *	pa_Rang1,		
			    	XDY_Mot	*	pa_Rang2,		
			    	XDY_Mot	*	pa_NumCmd,
			    	XDY_Mot		va_TypeEqt,
			    	XDY_Eqt		va_NumEqt,
			    	XDY_Mot		va_NumSeq )
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK  : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{		     
static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ : et_envoi_cmd_panneau " ;
   
XZEXT_MSG_SOCKET	vl_Msg; 
short			vl_PosMsg = 0,
			vl_Index,
			vl_NbrPan;		    
div_t			vl_Position;
short			vl_Cmd;
short			vl_TabCmd[XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN];
XDY_Datagroup		pl_NomDG;
XDY_NomSite        	pl_NomSite;



	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
	} 
 
        for ( vl_Index = 0; vl_Index < XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN; vl_TabCmd[vl_Index++] = 0);
        
	for ( vl_Index = 0, vl_NbrPan = 0; 
	      (vl_Index < XZETC_MAX_PANNEAU) && ( *pa_Rang1 > 0);
	      vl_Index++, pa_Rang1++, pa_Rang2++, pa_NumCmd++ )
	{
           XZST_03EcritureTrace( XZSTC_INFO, "Cmd:%d  pa_Rang1:%d  pa_Rang2 :%d pa_NumCmd:%d.", 
                                             vl_Index, *pa_Rang1, *pa_Rang2, *pa_NumCmd );
	   if ( (*pa_Rang1 < XZETC_MAX_PANNEAU) &&
	        (*pa_NumCmd > 0) && (*pa_NumCmd <= XZETC_MAX_BIT_PAN) )
	   {
	      vl_Position = div ( ((*pa_Rang1)-1), XZETC_NB_ETAT_PAN );
	      vl_Cmd = ( 1 << ((*pa_NumCmd)-1) ) << ( XZETC_MAX_BIT_PAN * vl_Position.rem );
	      vl_TabCmd[vl_Position.quot] = vl_TabCmd[vl_Position.quot] | vl_Cmd;
	      if ( vl_NbrPan < vl_Position.quot ) vl_NbrPan = vl_Position.quot;
	   }
	   if ( (*pa_Rang2 > 0) && (*pa_Rang2 < XZETC_MAX_PANNEAU) &&
	        (*pa_NumCmd > XZETC_MAX_BIT_PAN) )
	   {
	      vl_Position = div ( ((*pa_Rang2)-1), XZETC_NB_ETAT_PAN );
	      vl_Cmd = ( 1 << ((*pa_NumCmd)-6) ) << ( XZETC_MAX_BIT_PAN * vl_Position.rem );
	      vl_TabCmd[vl_Position.quot] = vl_TabCmd[vl_Position.quot] | vl_Cmd;
	      if ( vl_NbrPan < vl_Position.quot ) vl_NbrPan = vl_Position.quot;
	   }
	}
	      
        
        /*A Construction de la trame */
        vl_PosMsg = sprintf ( vl_Msg, 	"%d %d %d ", 
        				XZETC_CMD_PAN, 
        				va_Adresse,
        				vl_NbrPan+1 );
	for ( vl_Index = 0; 
	      vl_Index < sizeof(vl_TabCmd)/sizeof(vl_TabCmd[0]) ;
	      vl_Index++ )
	{
           /* Ajout dans la trame des commandes */
           if ( (vl_TabCmd[vl_Index] <= 0xFFFF) && (vl_TabCmd[vl_Index] >= 0) )
              vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"0x%04X ", vl_TabCmd[vl_Index] );
           else
              vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"0x0000 " );
           
        }
        vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%s", XZEXC_FIN_MSG_SOCK );
         			
        /*A Envoi de la trame */
        fcntl ( vg_SockTEXA0, F_SETFL, O_NDELAY | fcntl ( vg_SockTEXA0, F_GETFL, 0 ) );
        if ( send ( vg_SockTEXA0 , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
        {
           /* Ecriture dans le fichier trace en cas d'�chec */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'�mettre la trame <%s> vers TEXA0.", vl_Msg );
	   return(XDC_NOK);
        }
        else
        {   
           /*B Ecriture dans le fichier trace en cas de succes */
           XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame �mise vers TEXA0: %s.", vl_Msg );
           /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
	   sprintf ( pl_NomDG, "%s_%s", (va_TypeEqt == XDC_EQT_TUBE) ? XDG_ETAT_SEQ_TUBE : XDG_ETAT_SEQ_ECH, pl_NomSite );
           if (!TipcSrvMsgWrite (   pl_NomDG,
      	      	      	            XDM_IdentMsg((va_TypeEqt == XDC_EQT_TUBE) ? XDM_ETAT_SEQUENCE_TUBE : XDM_ETAT_SEQUENCE_ECH),
                          	    XDM_FLG_SRVMSGWRITE,
                          	    T_IPC_FT_INT4, va_TypeEqt,
                          	    T_IPC_FT_INT4, va_NumEqt,
                           	    T_IPC_FT_INT4, va_NumSeq,
                           	    T_IPC_FT_INT4, 0,
                           	    T_IPC_FT_INT4, 0,
                         	    NULL ) )
           {
               XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE non effectue." );
           }
	   return(XDC_OK);
        }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi un message de commande sequence a TEXA0.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int      et_envoi_cmd_sequence ( XDY_Mot	va_Adresse,
			    	 XDY_Mot 	va_NumCmd,
			    	 XDY_Mot	va_TypeEqt,
			    	 XDY_Eqt	va_NumEqt,
			    	 XDY_Mot	va_NumSeq )
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK  : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{		     
static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ : et_envoi_cmd_sequence " ;
   
XZEXT_MSG_SOCKET	vl_Msg; 
XZEXT_MSG_SOCKET	pl_MsgTimer;
short			vl_PosMsg = 0;		    
short			vl_Cmd;
double			vl_Horodate;
int           		vl_jourSemaine;
XDY_Datagroup		pl_NomDG;
XDY_NomSite        	pl_NomSite;



	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
	} 

	vl_Cmd = 1 << (va_NumCmd-1); 
	      
        /*A Construction de la trame */
        vl_PosMsg = sprintf ( vl_Msg, 	"%d %d %d 0x%04X %s", 
        				(va_TypeEqt == XDC_EQT_TUBE) ? XZETC_CMD_TUBE : XZETC_CMD_ECH,
        				va_Adresse,
        				1,
        				vl_Cmd,
        				XZEXC_FIN_MSG_SOCK );
         			
        /*A Envoi de la trame */
        fcntl ( vg_SockTEXA0, F_SETFL, O_NDELAY | fcntl ( vg_SockTEXA0, F_GETFL, 0 ) );
        if ( send ( vg_SockTEXA0 , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
        {
           /* Ecriture dans le fichier trace en cas d'�chec */
           XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame <%> vers TEXA0.", vl_Msg );
	   return(XDC_NOK);
        }
        else
        {   
           /*B Ecriture dans le fichier trace en cas de succes */
           XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame �mise vers TEXA0: <%s>.", vl_Msg );
           /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
	   sprintf ( pl_NomDG, "%s_%s", (va_TypeEqt == XDC_EQT_TUBE) ? XDG_ETAT_SEQ_TUBE : XDG_ETAT_SEQ_ECH, pl_NomSite );
           if (!TipcSrvMsgWrite (   pl_NomDG,
      	      	      	            XDM_IdentMsg((va_TypeEqt == XDC_EQT_TUBE) ? XDM_ETAT_SEQUENCE_TUBE : XDM_ETAT_SEQUENCE_ECH),
                          	    XDM_FLG_SRVMSGWRITE,
                          	    T_IPC_FT_INT4, va_TypeEqt,
                          	    T_IPC_FT_INT4, va_NumEqt,
                           	    T_IPC_FT_INT4, va_NumSeq,
                           	    T_IPC_FT_INT4, 0,
                           	    T_IPC_FT_INT4, 0,
                         	    NULL ) )
           {
               XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE non effectue." );
           }
        
           /*A Demande de reveil pour time_out discordance */
           /*B Lecture de l'horodate actuelle */
           XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
           sprintf ( pl_MsgTimer, "%s %lf %d Nice%hd_%hd_%hd %s",
                		  XZEXC_DEMANDE_REVEIL,
                		  vl_Horodate + (XZETC_TIMEOUT_DISC * 10),
                		  0,
                		  va_TypeEqt,
                		  va_NumEqt,
                		  va_NumSeq,
                		  XZEXC_FIN_MSG_SOCK );
         
           /* Envoie a la TETIM une demande reveil */      		
           fcntl ( vg_SockTETIM, F_SETFL, O_NDELAY | fcntl ( vg_SockTETIM, F_GETFL, 0 ) );
           if ( send ( vg_SockTETIM, pl_MsgTimer, strlen (pl_MsgTimer) , 0 ) <= 0 )
           {
              XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );
           }
           
	   return(XDC_OK);
        }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ : et_controle_cb" ;
   
    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
   T_INT4			vl_Etat;
   T_INT4			vl_TypeEqt;
   T_INT4			vl_NumEqt;
    #else
    XDY_NomMachine		vl_NomMachine;
   XDY_Entier			vl_Etat;
   XDY_Entier			vl_TypeEqt;
   XDY_Entier			vl_NumEqt;
    #endif   

   ENIC_DONNEES_Tube *		pl_Tube;
   ENIC_DONNEES_Echangeur *	pl_Ech;
   ENIC_DONNEES_Panneau *	pl_Panneau;
   ENIC_DONNEES_Capteur *	pl_Capt;
   int				vl_EqtTrouve = XDC_FAUX;
   

   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&vl_Etat
                          ,T_IPC_FT_STR, &vl_NomMachine
                          ,T_IPC_FT_INT4,&vl_TypeEqt
                          ,T_IPC_FT_INT4,&vl_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_EQPT non lu." );
      return;
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING ," IN  : et_controle_cb, Etat: %d Machine: %s Type: %d Eqt: %d",
                          vl_Etat,
                          vl_NomMachine,
                          vl_TypeEqt,
                          vl_NumEqt );
   /* Selon le type d'equipement */
   switch ( vl_TypeEqt )
   {
      /* Si type d'equipement est panneau */
      case XDC_EQT_PAN :
         /* Recherche dans la liste des tubes si le panneau existe */
         for ( pl_Tube = pg_debutTube;
               (pl_Tube != NULL) && (vl_EqtTrouve != XDC_VRAI); 
               pl_Tube = pl_Tube->Suivant )
         {
            for ( pl_Panneau = pl_Tube->ListePanneau;
                  (pl_Panneau != NULL) && (vl_EqtTrouve != XDC_VRAI);
                  pl_Panneau = pl_Panneau->Suivant )
            {
               /* Si panneau trouve */
               if ( pl_Panneau->NumeroPanneau == vl_NumEqt )
               {
                  /* Mise a jour de l'etat de service ou d'activite du panneau */
      	          if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
      	             pl_Panneau->Etat_Service &= vl_Etat;
      	          else
      	             pl_Panneau->Etat_Service += vl_Etat;
                  vl_EqtTrouve = XDC_VRAI;
                  break;
               }
            }
         }
         
         /* Recherche dans la liste des echangeurs si le panneau existe */
         for ( pl_Ech = pg_debutEchangeur;
               (pl_Ech != NULL) && (vl_EqtTrouve != XDC_VRAI); 
               pl_Ech = pl_Ech->Suivant )
         {
            for ( pl_Panneau = pl_Ech->ListePanneau;
                  (pl_Panneau != NULL) && (vl_EqtTrouve != XDC_VRAI);
                  pl_Panneau = pl_Panneau->Suivant )
            {
               /* Si panneau trouve */
               if ( pl_Panneau->NumeroPanneau == vl_NumEqt )
               {
                  /* Mise a jour de l'etat de service ou d'activite du panneau */
      	          if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
      	             pl_Panneau->Etat_Service &= vl_Etat;
      	          else
      	             pl_Panneau->Etat_Service += vl_Etat;
                  vl_EqtTrouve = XDC_VRAI;
                  break;
               }
            }
         }
         break;
         
            
      /* Si type d'equipement est Capteur */
      case XDC_EQT_CPT :
         /* Recherche dans la liste des tubes si le Capteur existe */
         for ( pl_Tube = pg_debutTube;
               pl_Tube != NULL; 
               pl_Tube = pl_Tube->Suivant )
         {
            for ( pl_Capt = pl_Tube->ListeCapteur;
                  (pl_Capt != NULL) && (vl_EqtTrouve != XDC_VRAI);
                  pl_Capt = pl_Capt->Suivant )
            {
               /* Si Capteur trouve */
               if ( pl_Capt->NumeroCapteur == vl_NumEqt )
               {
                  /* Mise a jour de l'etat de service ou d'activite du Capteur */
      	          if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
      	             pl_Capt->Etat_Service &= vl_Etat;
      	          else
      	             pl_Capt->Etat_Service += vl_Etat;
                  break;
               }
            }
         }
         break;
                  
      /* Si type d'equipement est Tube */
      case XDC_EQT_TUB :
         /* Recherche dans la liste des tubes si le Tube existe */
         for ( pl_Tube = pg_debutTube;
               pl_Tube != NULL; 
               pl_Tube = pl_Tube->Suivant )
         {
            /* Si Tube trouve */
            if ( pl_Tube->Numero == vl_NumEqt )
            {
               /* Mise a jour de l'etat de service ou d'activite du Tube */
      	       if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_Tube->Etat_Service &= vl_Etat;
      	       else
      	          pl_Tube->Etat_Service += vl_Etat;
               break;
            }
         }
         break;
	
      /* Si type d'equipement est Echangeur */
      case XDC_EQT_ECH :
         /* Recherche dans la liste des tubes si le Echangeur existe */
         for ( pl_Ech = pg_debutEchangeur;
               pl_Ech != NULL; 
               pl_Ech = pl_Ech->Suivant )
         {
            /* Si Echangeur trouve */
            if ( pl_Ech->NumeroDemiEch == vl_NumEqt )
            {
               /* Mise a jour de l'etat de service ou d'activite du Echangeur */
      	       if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_Ech->Etat_Service &= vl_Etat;
      	       else
      	          pl_Ech->Etat_Service += vl_Etat;
               break;
            }
         }
         break;
         
   }
   
   
   XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : et_controle_cb ");
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande signalisation tunnel
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_cmd_sigt_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_SIGT
*
* FONCTION 
*  Pour toutes la signalisations concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ :  et_cmd_sigt_cb " ;
   
   ENIC_DONNEES_Tube *		pl_Tube;
   ENIC_DONNEES_Panneau *	pl_Panneau;
#ifndef _TIBCO_EMS
   T_INT2			*pl_NumPan,
    				*pl_NumCmd;
   T_INT4			vl_NumTube,
   				vl_NumAct,
   				vl_NumSeq;
   T_INT4			vl_NbPan, 
   				vl_NbCmd;
#else
   T_INT2			pl_NumPan[XZETC_MAX_PANNEAU],
    				pl_NumCmd[XZETC_MAX_PANNEAU];
   T_INT4			vl_NumTube,
   				vl_NumAct,
   				vl_NumSeq;
   T_INT4			vl_NbPan, 
   				vl_NbCmd;
#endif
   short			vl_NumPan[XZETC_MAX_PANNEAU],
   				vl_NumCmd[XZETC_MAX_PANNEAU];
   short			vl_Rang1[XZETC_MAX_PANNEAU],
    				vl_Rang2[XZETC_MAX_PANNEAU],
    				vl_Cmd[XZETC_MAX_PANNEAU];
   int				vl_Index, vl_IndexCmd;

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_sigt_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_SIGT */
   if(!TipcMsgRead ( 	  pa_Data -> msg,
                          T_IPC_FT_INT4, &vl_NumTube,
 			  T_IPC_FT_INT2_ARRAY,&pl_NumPan,&vl_NbPan,
 			  T_IPC_FT_INT2_ARRAY,&pl_NumCmd,&vl_NbCmd,
                          T_IPC_FT_INT4, &vl_NumSeq,
                          T_IPC_FT_INT4, &vl_NumAct,
	       		  NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_SIGT non lu ." );
      return;
   }

#ifndef _TIBCO_EMS
   for ( vl_Index = 0;
         vl_Index < XZETC_MAX_PANNEAU;
         vl_NumPan[vl_Index] = *pl_NumPan, vl_NumCmd[vl_Index] = *pl_NumCmd , vl_Index++, pl_NumPan++, pl_NumCmd++);
#else
   for ( vl_Index = 0;
         vl_Index < XZETC_MAX_PANNEAU;
         vl_Index++)
   {       
         vl_NumPan[vl_Index] = pl_NumPan[vl_Index];
         vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index];
   }
#endif
   
   /* Recherche dans la liste le tube concerne */
   for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
   {
       /* Si tube trouve et pas signalisation en local */
       if ( (pl_Tube->Numero == vl_NumTube) && 
            !(pl_Tube->EtatTelealarme & XZETC_BIT_1) &&
            !pl_Tube->Etat_Service )
       {
          /* Recherche le rang de commande des panneau concernes */
          for ( pl_Panneau = pl_Tube->ListePanneau, vl_IndexCmd = 0;
                pl_Panneau != NULL;
                pl_Panneau = pl_Panneau->Suivant)
          {
             
             vl_Rang1[vl_IndexCmd] = 0;
             vl_Rang2[vl_IndexCmd] = 0;
             vl_Cmd[vl_IndexCmd] = 0;
             for ( vl_Index = 0; 
          	   (vl_Index < XZETC_MAX_PANNEAU) && (vl_NumPan[vl_Index] != 0); 
          	   vl_Index++ )
             {
                /* Si panneau trouve */
                 if ( (pl_Panneau->NumeroPanneau == vl_NumPan[vl_Index]) &&
                      !pl_Panneau->Etat_Service )
                 {
                    /* Stock le rang de commande du panneau */
                    vl_Rang1[vl_IndexCmd] = pl_Panneau->RangComm;
                    vl_Rang2[vl_IndexCmd] = pl_Panneau->RangComm2;
                    vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                    XZST_03EcritureTrace(XZSTC_INFO ,"NumPan:%d NumCmd:%d RangComm1:%d RangComm2:%d.",
                            vl_NumPan[vl_Index], vl_NumCmd[vl_Index], vl_Rang1[vl_IndexCmd], vl_Rang2[vl_IndexCmd] );
                    break;
                 }
              }
           }
           break; /* Fin recherche tube */
        }
    }

    if ( pl_Tube == NULL )
    {       
       /* Sortie : tube non trouve ou en local */
       XZST_03EcritureTrace(XZSTC_WARNING ,"Tube:%d non trouve ou en local ou inactif pour Action:%d.",
       				vl_NumTube, vl_NumAct );
    }
    else
    {
       /* Envoie des commandes a TEXA0 */
       if ( et_envoi_cmd_panneau ( pl_Tube->AdresseCommande,
    			        vl_Rang1,
			        vl_Rang2,    				 
			        vl_Cmd,
			        XDC_EQT_TUBE,
			        pl_Tube->Numero,
			        pl_Tube->DerniereSequence ) == XDC_OK )
       { 
          /*A Memorisation de la sequence en cours */
          pl_Tube->DerniereSequence = vl_NumSeq;
       }
    }

   XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_sigt_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande signalisation echangeur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_cmd_sige_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_SIGE
*
* FONCTION 
*  Pour toutes la signalisations concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ :  et_cmd_sige_cb " ;
   
   ENIC_DONNEES_Echangeur *	pl_Ech;
   ENIC_DONNEES_Panneau *	pl_Panneau;
#ifndef _TIBCO_EMS
   T_INT4			vl_NumEch,
   				vl_NumAct,
   				vl_NumSeq;
   T_INT2			*pl_NumPan,
    				*pl_NumCmd;
   T_INT4			vl_NbPan, 
   				vl_NbCmd;
#else
   XDY_Entier			vl_NumEch,
   				vl_NumAct,
   				vl_NumSeq;
   XDY_Mot			pl_NumPan[XZETC_MAX_PANNEAU],
    				pl_NumCmd[XZETC_MAX_PANNEAU];
   XDY_Entier			vl_NbPan, 
   				vl_NbCmd;
#endif
   short			vl_NumPan[XZETC_MAX_PANNEAU],
   				vl_NumCmd[XZETC_MAX_PANNEAU];
   short			vl_Rang1[XZETC_MAX_PANNEAU],
    				vl_Rang2[XZETC_MAX_PANNEAU],
    				vl_Cmd[XZETC_MAX_PANNEAU];
   int				vl_Index, vl_IndexCmd;

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_sige_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_SIGE */
   if(!TipcMsgRead ( 	  pa_Data -> msg,
                          T_IPC_FT_INT4, &vl_NumEch,
 			  T_IPC_FT_INT2_ARRAY,&pl_NumPan,&vl_NbPan,
 			  T_IPC_FT_INT2_ARRAY,&pl_NumCmd,&vl_NbCmd,
                          T_IPC_FT_INT4, &vl_NumSeq,
                          T_IPC_FT_INT4, &vl_NumAct,
	       		  NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_SIGE non lu ." );
      return;
   }
   
#ifndef _TIBCO_EMS
   for ( vl_Index = 0;
         vl_Index < XZETC_MAX_PANNEAU;
         vl_NumPan[vl_Index] = *pl_NumPan, vl_NumCmd[vl_Index] = *pl_NumCmd , vl_Index++, pl_NumPan++, pl_NumCmd++);
#else
   for ( vl_Index = 0;
         vl_Index < XZETC_MAX_PANNEAU;
         vl_Index++)
   {       
      /* Trace : Impossibilite de lire le message */
         vl_NumPan[vl_Index] = pl_NumPan[vl_Index];
	 vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index];
   }
#endif
   
   /* Recherche dans la liste le echangeur concerne */
   /* Recherche dans la liste le tube concerne */
   for ( pl_Ech = pg_debutEchangeur; 
         pl_Ech != NULL; 
         pl_Ech = pl_Ech->Suivant )
   {
       /* Si echangeur trouve et pas signalisation en local */
       if ( (pl_Ech->NumeroDemiEch == vl_NumEch) && 
            !(pl_Ech->EtatTelealarme & XZETC_BIT_1) &&
            !pl_Ech->Etat_Service )
       {
          /* Recherche le rang de commande des panneau concernes */
          for ( pl_Panneau = pl_Ech->ListePanneau, vl_IndexCmd = 0;
                pl_Panneau != NULL;
                pl_Panneau = pl_Panneau->Suivant )
          {
             
             vl_Rang1[vl_IndexCmd] = 0;
             vl_Rang2[vl_IndexCmd] = 0;
             vl_Cmd[vl_IndexCmd] = 0;
             for ( vl_Index = 0; 
          	   (vl_Index < XZETC_MAX_PANNEAU) && (vl_NumPan[vl_Index] != 0); 
          	   vl_Index++ )
             {
                /* Si panneau trouve */
                 if ( (pl_Panneau->NumeroPanneau == vl_NumPan[vl_Index]) &&
                      !pl_Panneau->Etat_Service )
                 {
                    /* Stock le rang de commande du panneau */
                    vl_Rang1[vl_IndexCmd] = pl_Panneau->RangComm;
                    vl_Rang2[vl_IndexCmd] = pl_Panneau->RangComm2;
                    vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                    break;
                 }
              }
           }
           break; /* Fin recherche tube */
        }
    }

    if ( pl_Ech == NULL )
    {       
       /* Sortie : Echangeur non trouve ou en local */
       XZST_03EcritureTrace(XZSTC_WARNING ,"Echangeur:%d non trouve ou en local ou inactif  pour Action:%d.",
       				vl_NumEch, vl_NumAct );
    }
    else
    {
       /* Envoie des commandes a TEXA0 */
       if ( et_envoi_cmd_panneau ( pl_Ech->AdresseCommande,
    			        vl_Rang1,
			        vl_Rang2,    				 
			        vl_Cmd,
			        XDC_EQT_ECH,
			        pl_Ech->NumeroDemiEch,
			        pl_Ech->DerniereSequence ) == XDC_OK )
       {
          /*A Memorisation de la sequence en cours */
          pl_Ech->DerniereSequence = vl_NumSeq;
       }
        
    }

   XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_sige_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence tunnel 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_cmd_seqt_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_SEQT
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ :  et_cmd_seqt_cb " ;
  
#ifdef _TIBCO_EMS
   XDY_Entier			vl_NumTube,
   				vl_NumAct,
   				vl_NumSeq;
#else
   T_INT4			vl_NumTube,
   				vl_NumAct,
   				vl_NumSeq;
#endif
   ENIC_DONNEES_Tube *		pl_Tube;
   ENIC_DONNEES_Sequence *	pl_Seq;
   int				vl_js;
   XDY_Horodate			vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_seqt_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_SEQT */
   if(!TipcMsgRead ( 	  pa_Data -> msg,
                          T_IPC_FT_INT4, &vl_NumTube,
                          T_IPC_FT_INT4, &vl_NumSeq,
                          T_IPC_FT_INT4, &vl_NumAct,
	       		  NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_SEQT non lu ." );
      return;
   }
   
   
   /* Recherche dans la liste le tube concerne */
   for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
   {
       /* Si tube trouve et pas signalisation en local */
       if ( (pl_Tube->Numero == vl_NumTube) && 
            !(pl_Tube->EtatTelealarme & XZETC_BIT_1) &&
            !pl_Tube->Etat_Service )
       {
          /* Recherche si la sequence existe */
          for ( pl_Seq = pl_Tube->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
          {
             /* Si sequence trouve */
             if ( pl_Seq->NumSequence == vl_NumSeq )
             {
                /* Envoi de la commande sequence a TEXA0 */
    		if ( (pl_Seq->EtatDisc == 0) && 
    		     (et_envoi_cmd_sequence ( pl_Seq->Adresse,
    			   		      pl_Seq->BitCommande,
    			   		      XDC_EQT_TUBE,
    			   		      pl_Tube->Numero,
    			   		      vl_NumSeq ) == XDC_OK) )
    		{
                   /*A Memorisation de la sequence en cours */
                   pl_Tube->DerniereSequence = vl_NumSeq;
                   pl_Seq->NumAction         = vl_NumAct;
                }
                else
                {
                   /*A Echec envoie commande */
                   pl_Tube = NULL;
                }
                break;
             }
          }
          break; /* Fin recherche tube */
       }
    }

    if ( (pl_Tube == NULL) || (pl_Seq == NULL) )
    {
       /* Envoyer echec commande sequence a BD */
       /* Appel de XZAT22 avec XDC_NOK et NumAction */
       XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );
       xzat22_CompteRendu_Tube ( vl_NumAct, vl_horodate, XDC_NOK);
       XZST_03EcritureTrace(XZSTC_WARNING, "Tube:%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
       				vl_NumTube, vl_NumSeq, vl_NumAct );
    }
    
   XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_seqt_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_cmd_seqe_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_SEQE
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_cmd.c,v 1.14 2020/11/03 18:09:42 pc2dpdy Exp $ :  et_cmd_seqt_cb " ;
  
#ifdef _TIBCO_EMS
   XDY_Entier			vl_NumEch,
   				vl_NumAct,
   				vl_NumSeq;
#else
   T_INT4			vl_NumEch,
   				vl_NumAct,
   				vl_NumSeq;
#endif
   ENIC_DONNEES_Echangeur *	pl_Ech;
   ENIC_DONNEES_Sequence *	pl_Seq;
   int				vl_js;
   XDY_Horodate			vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_seqe_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_SEQE */
   if(!TipcMsgRead ( 	  pa_Data -> msg,
                          T_IPC_FT_INT4, &vl_NumEch,
                          T_IPC_FT_INT4, &vl_NumSeq,
                          T_IPC_FT_INT4, &vl_NumAct,
	       		  NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_SEQE non lu ." );
      return;
   }
   
   
   /* Recherche dans la liste l'echangeur concerne */
   for ( pl_Ech = pg_debutEchangeur; pl_Ech != NULL; pl_Ech = pl_Ech->Suivant )
   {
       /* Si echangeur trouve et pas signalisation en local */
       if ( (pl_Ech->NumeroDemiEch == vl_NumEch) && 
            !(pl_Ech->EtatTelealarme & XZETC_BIT_1) &&
            !pl_Ech->Etat_Service )
       {
          /* Recherche si la sequence existe */
          for ( pl_Seq = pl_Ech->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
          {
             /* Si sequence trouve */
             if ( pl_Seq->NumSequence == vl_NumSeq )
             {
                /* Envoi de la commande sequence a TEXA0 */
    		if ( (pl_Seq->EtatDisc == 0) &&
    		     (et_envoi_cmd_sequence ( pl_Seq->Adresse,
    			   		      pl_Seq->BitCommande,
    			   		      XDC_EQT_ECH,
    			   		      pl_Ech->NumeroDemiEch,
    			   		      vl_NumSeq ) == XDC_OK) )
    		{
                   /*A Memorisation de la sequence en cours */
                   pl_Ech->DerniereSequence = vl_NumSeq;
                   pl_Seq->NumAction        = vl_NumAct;
                }
                else
                {
                   /*A Echec envoie commande */
                   pl_Ech = NULL;
                }
                break;
             }
          }
          break; /* Fin recherche tube */
       }
    }

    if ( (pl_Ech == NULL) || (pl_Seq == NULL) )
    {
       /* Envoyer echec commande sequence a BD */
       /* Appel de XZAT23 avec XDC_NOK et NumAction */
       XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );
       xzat23_CompteRendu_DemiEch ( vl_NumAct, vl_horodate, XDC_NOK);
       XZST_03EcritureTrace(XZSTC_WARNING, "Echangeur:%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
       				vl_NumEch, vl_NumSeq, vl_NumAct );
    }
    
   XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_seqe_cb ");
}
