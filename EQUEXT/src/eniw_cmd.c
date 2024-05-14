/*E*/
/*Fichier :  $Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      Release : $Revision: 1.6 $        Date : $Date: 2021/01/29 14:00:56 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des fonction de commande ( Callback RTworks )
* pour la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation PLate forme
* Moulin.D	version 1.3	23 Avr 2002	: Modif des etats de service
* Moulin.D	version 1.4	23 Oct 2002	: Modif de la synchro
* JMG		version 1.5	15/05/20 : EMS
* GR            version 1.6     09/07/2020 : EMS
* AMI 28/01/21 : SAE_191: chantier date: encapsulation localtime pour hiniber TZ car nbseconde deja en TZ
------------------------------------------------------*/
 
/* fichiers inclus */

#include "eniw_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : eniw_cmd" ;

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
*  Fonction appel≈e lors de la r≈ception du message
*  XDM_EInit_IHM_TNIC
*
* FONCTION 
*  Renvoyer l'etat  demande
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_Init_IHM_TNIC_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   T_INT4			vl_NumTE,
   				vl_TypeEqt,
   				vl_TypeInit;
   ENIW_DONNEES_Panneau	*	pl_Panneau;
   ENIW_DONNEES_Capteur	*	pl_Capteur;
   ENIW_DONNEES_Sequence *	pl_Sequence;
   ENIW_DONNEES_Tube *		pl_Tube;
   ENIW_DONNEES_Echangeur *	pl_Ech;

		  
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
            if  ((vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
            {
               if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_TELEALARME),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, XDC_EQT_TUBE,
                          	    	T_IPC_FT_INT4, pl_Tube->Numero,
                          	    	T_IPC_FT_INT4, pl_Tube->EtatEnergie,
                          	    	T_IPC_FT_INT4, pl_Tube->EtatLocal,
                          	    	T_IPC_FT_INT4, pl_Tube->EtatLampe,
                          	    	NULL ) )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_TELEALARME non effectue." );
               }
            }
            break;
            
         case XZETC_CAPTEUR :
            /* Envoi etat capteur dans XDM_ETAT_CAPTEUR sur pl_DGInit */
            if  ((vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
            {
               for ( pl_Capteur = pl_Tube->ListeCapteur;
                     pl_Capteur != NULL; 
                     pl_Capteur = pl_Capteur->Suivant )
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
            break;
            
         case XZETC_SEQUENCE :
            /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE_TUBE sur pl_DGInit */
            if  ((vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
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
            if  ((vl_TypeEqt == XDC_EQT_TUBE) && 
                 ((pl_Tube->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
            {
               for ( pl_Panneau = pl_Tube->ListePanneau;
                     pl_Panneau != NULL; 
                     pl_Panneau = pl_Panneau->Suivant )
               {
                  /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur pl_DGInit */
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
            break;
      }
   }
         
       
  for ( pl_Ech = pg_debutEchangeur; pl_Ech != NULL; pl_Ech = pl_Ech->Suivant )
   {
      switch ( vl_TypeInit )
      {
         case XZETC_TELEALARME :
            /* Envoi etat tube dans XDM_ETAT_TELEALARME sur pl_DGInit */
            if  ((vl_TypeEqt == XDC_EQT_ECH) && 
                 ((pl_Ech->NumeroDemiEch == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
            {
               if (!TipcSrvMsgWrite ( 	pl_DGInit,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_TELEALARME),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, XDC_EQT_ECH,
                          	    	T_IPC_FT_INT4, pl_Ech->NumeroDemiEch,
                          	    	T_IPC_FT_INT4, pl_Ech->EtatEnergie,
                          	    	T_IPC_FT_INT4, pl_Ech->EtatLocal,
                          	    	T_IPC_FT_INT4, pl_Ech->EtatLampe,
                          	    	NULL ) )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_TELEALARME non effectue." );
               }
            }
            break;
            
         case XZETC_SEQUENCE :
            /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE_ECH sur pl_DGInit */
            if  ((vl_TypeEqt == XDC_EQT_ECH) && 
                 ((pl_Ech->NumeroDemiEch == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
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
            if  ((vl_TypeEqt == XDC_EQT_ECH) && 
                 ((pl_Ech->NumeroDemiEch == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
            {
               for ( pl_Panneau = pl_Ech->ListePanneau;
                     pl_Panneau != NULL; 
                     pl_Panneau = pl_Panneau->Suivant )
               {
                  /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur pl_DGInit */
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
            break;
      }
   }
  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_Init_IHM_TNIC_cb ");
	       
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi un message de commande panneau a la tache maitre Wizcon ou esclave Wizcon.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int      et_envoi_cmd_panneau ( 
					int			va_socket,
			    	XDY_Mot		va_TypeEqt,
					XDY_Mot *	pa_Pan,			
			    	XDY_Mot	*	pa_NumCmd,
			    	int			va_NbCommande,
				XDY_Mot	va_NumEqt,
				XDY_Mot va_NumSeq)
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
static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_envoi_cmd_panneau " ;
   
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
 
        
    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg, 	"%d|%d|",ENIW_CMD_PANNEAU,va_TypeEqt);

	for ( vl_Index = 0;vl_Index < va_NbCommande-1;vl_Index++ )
	{
        /* Ajout dans la trame des commandes */
		vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d~", pa_Pan[vl_Index]);
           
    }

	/* le dernier */
	vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d|", pa_Pan[va_NbCommande-1]);
    
	for ( vl_Index = 0;vl_Index < va_NbCommande-1;vl_Index++ )
	{
        /* Ajout dans la trame des commandes */
		vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d~", pa_NumCmd[vl_Index]);
           
    }
  
	/* le dernier */
	vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d|", pa_NumCmd[va_NbCommande-1]);
        
	  
	/* Fin de trame */
	vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%s", XZEXC_FIN_MSG_SOCK_WIZCON);

    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );

    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
    {
		/* Ecriture dans le fichier trace en cas d'≈chec */
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'≈mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
		return(XDC_NOK);
    }
    else
    {   
		/*B Ecriture dans le fichier trace en cas de succes */
		XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame ≈mise vers maitre Wizcon ou esclave Wizcon: %s.", vl_Msg );
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
*  Envoi un message de commande sequence a maitre Wizcon ou esclave Wizcon.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int      et_envoi_cmd_sequence ( int va_socket,
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
static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_envoi_cmd_sequence " ;
   
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
	      
	/*A Construction de la trame */
	vl_PosMsg = sprintf ( vl_Msg, 	"%d|%d|%d|%d|%s",
					ENIW_CMD_SEQUENCE,
					va_TypeEqt,
        			va_NumEqt,
        			va_NumSeq,
        			XZEXC_FIN_MSG_SOCK_WIZCON );
         		
	/*A Envoi de la trame */
	fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );

	if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
	{
		/* Ecriture dans le fichier trace en cas d'≈chec */
		XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'≈mettre la trame <%> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
		return(XDC_NOK);
	}
	else
	{   
		/*B Ecriture dans le fichier trace en cas de succes */
		XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame ≈mise vers maitre Wizcon ou esclave Wizcon: <%s>.", vl_Msg );
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
*  Message de contr¬le.
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
*  Fonction appel≈e lors de la r≈ception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_controle_cb" ;
   
   T_INT4			vl_Etat;

    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    #else
    XDY_NomMachine		vl_NomMachine;
    #endif


   T_INT4			vl_TypeEqt;
   T_INT4			vl_NumEqt;
   ENIW_DONNEES_Tube *		pl_Tube;
   ENIW_DONNEES_Echangeur *	pl_Ech;
   ENIW_DONNEES_Panneau *	pl_Panneau;
   ENIW_DONNEES_Capteur *	pl_Capt;
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
*  Fonction appel≈e lors de la r≈ception du message
*  XDM_ECMD_SIGT
*
* FONCTION 
*  Pour toutes la signalisations concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_sigt_cb " ;
   
   ENIW_DONNEES_Tube *		pl_Tube;
   ENIW_DONNEES_Panneau *	pl_Panneau;
   T_INT4			vl_NumTube,
   				vl_NumAct,
   				vl_NumSeq;
   T_INT4			vl_NbPan, 
   				vl_NbCmd;
#ifndef _TIBCO_EMS
   T_INT2			*pl_NumPan,
    				*pl_NumCmd;
#else
   T_INT2			pl_NumPan[XZETC_MAX_PANNEAU],
    				pl_NumCmd[XZETC_MAX_PANNEAU];
#endif
   short			vl_NumPan[XZETC_MAX_PANNEAU],
	      			vl_Pan[XZETC_MAX_PANNEAU],
   				vl_NumCmd[XZETC_MAX_PANNEAU];
   short			vl_Rang1[XZETC_MAX_PANNEAU],
    				vl_Rang2[XZETC_MAX_PANNEAU],
    				vl_Cmd[XZETC_MAX_PANNEAU];
   int				vl_Index, vl_IndexCmd;
   int				vl_commande_TWWI0,vl_commande_TWWI1;

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
         vl_NumPan[vl_Index] = pl_NumPan[vl_Index];
   }
#endif
   
   /* Recherche dans la liste le tube concerne */
   for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
   {
      	XZST_03EcritureTrace(XZSTC_INFO ,"Panneau num:<%d>, etat local:<%d>, etat service : <%d>.",pl_Tube->Numero,pl_Tube->EtatLocal,pl_Tube->Etat_Service);
       /* Si tube trouve et pas signalisation en local */
       if  (pl_Tube->Numero == vl_NumTube)
       {
          /* Recherche le rang de commande des panneau concernes */
          for ( pl_Panneau = pl_Tube->ListePanneau, vl_IndexCmd = 0;
                pl_Panneau != NULL;
                pl_Panneau = pl_Panneau->Suivant)
          {
             vl_Cmd[vl_IndexCmd] = 0;
			 vl_Pan[vl_IndexCmd]=0;
             for ( vl_Index = 0; 
          	   (vl_Index < XZETC_MAX_PANNEAU) && (vl_NumPan[vl_Index] != 0); 
          	   vl_Index++ )
             {
                /* Si panneau trouve */
                 if (pl_Panneau->NumeroPanneau == vl_NumPan[vl_Index])
                 {
                    /* Stock la commande et le panneau */
					vl_Pan[vl_IndexCmd]= vl_NumPan[vl_Index];
                    vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                    XZST_03EcritureTrace(XZSTC_INFO ,"NumPan:%d NumCmd:%d.",
                            vl_NumPan[vl_Index], vl_NumCmd[vl_Index] );
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
       /* Envoie des commandes a maitre Wizcon */

       vl_commande_TWWI0= et_envoi_cmd_panneau (vg_SockTWWI0, 
			        XDC_EQT_TUBE,
					vl_Pan,				 
			        vl_Cmd,
					vl_IndexCmd,
					pl_Tube->Numero,
					pl_Tube->DerniereSequence);

       /* Envoie des commandes a esclave Wizcon */	   

       vl_commande_TWWI1= et_envoi_cmd_panneau (vg_SockTWWI1, 
			        XDC_EQT_TUBE,
					vl_Pan,				 
			        vl_Cmd,
					vl_IndexCmd,
					pl_Tube->Numero,
					pl_Tube->DerniereSequence);
		
		/* Si la maitre est la tache maitre Wizcon c'est son rÈsultat qui fait foi*/
       if (vg_maitre_actif == XDC_MAITRE_TWWI0)
       { 
		   if (vl_commande_TWWI0 == XDC_OK)
		   {
			  /*A Memorisation de la sequence en cours */
			  pl_Tube->DerniereSequence = vl_NumSeq;
		   }
       }
	   else
	   {
			if (vl_commande_TWWI1 == XDC_OK)
		   {
			  /*A Memorisation de la sequence en cours */
			  pl_Tube->DerniereSequence = vl_NumSeq;
		   }	
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
*  Fonction appel≈e lors de la r≈ception du message
*  XDM_ECMD_SIGE
*
* FONCTION 
*  Pour toutes la signalisations concernee
*  Envoyer aux taches maitre Wizcon et esclave Wizcon la commande
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_sige_cb " ;
   
   T_INT4			vl_NumEch,
   				vl_NumAct,
   				vl_NumSeq;
   ENIW_DONNEES_Echangeur *	pl_Ech;
   ENIW_DONNEES_Panneau *	pl_Panneau;
   T_INT4			vl_NbPan, 
   				vl_NbCmd;
#ifndef _TIBCO_EMS
   T_INT2			*pl_NumPan,
    				*pl_NumCmd;
#else
   T_INT2			pl_NumPan[XZETC_MAX_PANNEAU],
    				pl_NumCmd[XZETC_MAX_PANNEAU];
#endif
   short			vl_NumPan[XZETC_MAX_PANNEAU],
					vl_Pan[XZETC_MAX_PANNEAU],
   				vl_NumCmd[XZETC_MAX_PANNEAU];
   short			vl_Rang1[XZETC_MAX_PANNEAU],
    				vl_Rang2[XZETC_MAX_PANNEAU],
    				vl_Cmd[XZETC_MAX_PANNEAU];
   int				vl_Index, vl_IndexCmd;
   int				vl_commande_TWWI0,vl_commande_TWWI1;

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
         vl_NumPan[vl_Index] = pl_NumPan[vl_Index];
         vl_NumPan[vl_Index] = pl_NumPan[vl_Index];
   }
#endif
   
   /* Recherche dans la liste le echangeur concerne */
   /* Recherche dans la liste le tube concerne */
   for ( pl_Ech = pg_debutEchangeur; 
         pl_Ech != NULL; 
         pl_Ech = pl_Ech->Suivant )
   {
       /* Si echangeur trouve et pas signalisation en local */
       if  (pl_Ech->NumeroDemiEch == vl_NumEch)
       {
          /* Recherche le rang de commande des panneau concernes */
          for ( pl_Panneau = pl_Ech->ListePanneau, vl_IndexCmd = 0;
                pl_Panneau != NULL;
                pl_Panneau = pl_Panneau->Suivant )
          {
             vl_Cmd[vl_IndexCmd] = 0;
             for ( vl_Index = 0; 
          	   (vl_Index < XZETC_MAX_PANNEAU) && (vl_NumPan[vl_Index] != 0); 
          	   vl_Index++ )
             {
                /* Si panneau trouve */
                 if  (pl_Panneau->NumeroPanneau == vl_NumPan[vl_Index]) 
                 {
                    /* Stock la commande et le panneau */
					vl_Pan[vl_IndexCmd]= vl_NumPan[vl_Index];
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

       /* Envoie des commandes a maitre Wizcon */

       vl_commande_TWWI0= et_envoi_cmd_panneau (vg_SockTWWI0, 
			        XDC_EQT_ECH,
					vl_Pan,				 
			        vl_Cmd,
					vl_IndexCmd,
					pl_Ech->NumeroDemiEch,
					pl_Ech->DerniereSequence);


       /* Envoie des commandes a esclave Wizcon */	   

       vl_commande_TWWI1= et_envoi_cmd_panneau (vg_SockTWWI1, 
			        XDC_EQT_ECH,
					vl_Pan,				 
			        vl_Cmd,
					vl_IndexCmd,
					pl_Ech->NumeroDemiEch,
					pl_Ech->DerniereSequence);

		
		/* Si la maitre est la tache maitre Wizcon c'est son rÈsultat qui fait foi*/
       if (vg_maitre_actif == XDC_MAITRE_TWWI0)
       { 
		   if (vl_commande_TWWI0 == XDC_OK)
		   {
			  /*A Memorisation de la sequence en cours */
			   pl_Ech->DerniereSequence = vl_NumSeq;
		   }
       }
	   else
	   {
			if (vl_commande_TWWI1 == XDC_OK)
		   {
			  /*A Memorisation de la sequence en cours */
	          pl_Ech->DerniereSequence = vl_NumSeq;
		   }	
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
*  Fonction appel≈e lors de la r≈ception du message
*  XDM_ECMD_SEQT
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_seqt_cb " ;
   
   T_INT4			vl_NumTube,
   				vl_NumAct,
   				vl_NumSeq;
   ENIW_DONNEES_Tube *		pl_Tube;
   ENIW_DONNEES_Sequence *	pl_Seq;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int				vl_commande_TWWI0,vl_commande_TWWI1;

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
      	XZST_03EcritureTrace(XZSTC_INFO ,"Tube num:<%d>, etat local:<%d>, etat service : <%d>.",pl_Tube->Numero,pl_Tube->EtatLocal,pl_Tube->Etat_Service);
       /* Si tube trouve et pas signalisation en local */
       if  (pl_Tube->Numero == vl_NumTube)
       {
          /* Recherche si la sequence existe */
          for ( pl_Seq = pl_Tube->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
          {
      		XZST_03EcritureTrace(XZSTC_INFO ,"Sequence num:<%d>, etat disc:<%d>.",pl_Seq->NumSequence,pl_Seq->EtatDisc);
             /* Si sequence trouve */
             if ( pl_Seq->NumSequence == vl_NumSeq )
             {
                

    			if (pl_Seq->EtatDisc == 0)
				{
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
    			   											  XDC_EQT_TUBE,
    			   											  pl_Tube->Numero,
    			   											  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
    			   											  XDC_EQT_TUBE,
    			   											  pl_Tube->Numero,
    			   											  vl_NumSeq );


					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
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
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
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
					}
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
*  Fonction appel≈e lors de la r≈ception du message
*  XDM_ECMD_SEQE
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_seqt_cb " ;
   
   T_INT4			vl_NumEch,
   				vl_NumAct,
   				vl_NumSeq;
   ENIW_DONNEES_Echangeur *	pl_Ech;
   ENIW_DONNEES_Sequence *	pl_Seq;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int				vl_commande_TWWI0,vl_commande_TWWI1;

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
       if  (pl_Ech->NumeroDemiEch == vl_NumEch)
       {
          /* Recherche si la sequence existe */
          for ( pl_Seq = pl_Ech->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
          {
             /* Si sequence trouve */
             if ( pl_Seq->NumSequence == vl_NumSeq )
             {
				if (pl_Seq->EtatDisc == 0)
				{
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
																XDC_EQT_ECH,
																pl_Ech->NumeroDemiEch,
																vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI1, 
																XDC_EQT_ECH,
																pl_Ech->NumeroDemiEch,
																vl_NumSeq );
					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
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
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
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
					}
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

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	et_cmd_maitre ( 
        int 			va_socket)
 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel≈e lors de la mise a jour du maitre
*
* FONCTION 
* Envoyer a la tache maitre Wizcon ou esclave Wizcon la notion de maitre  
*
------------------------------------------------------*/
{
   	static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_maitre " ;
	XZEXT_MSG_SOCKET	vl_Msg; 
	short			vl_PosMsg = 0;
   

 	XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_maitre ");

        /*A Construction de la trame */
        vl_PosMsg = sprintf ( vl_Msg, "%d|1|%s", ENIW_CMD_MAITRE, XZEXC_FIN_MSG_SOCK_WIZCON );

        /*A Envoi de la trame */
        fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
        if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
        {

           /* Ecriture dans le fichier trace en cas d'≈chec */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'≈mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
	   return(XDC_NOK);

	}

	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT  : et_cmd_maitre ");
	return(XDC_OK);
		  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	et_cmd_alive ( 
        int 			va_socket)
 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel≈e pour verifier la communication 
*
* FONCTION 
* Envoyer a la tache maitre Wizcon ou esclave Wizcon la notion de maitre  
*
------------------------------------------------------*/
{
   	static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_alive " ;
	XZEXT_MSG_SOCKET	vl_Msg; 
	short			vl_PosMsg = 0;
   

 	XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_alive ");

        /*A Construction de la trame */
        vl_PosMsg = sprintf ( vl_Msg, "%d|1|%s", ENIW_CMD_IS_ALIVE, XZEXC_FIN_MSG_SOCK_WIZCON );

        /*A Envoi de la trame */
        fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
        if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
        {

           /* Ecriture dans le fichier trace en cas d'≈chec */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'≈mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
	   return(XDC_NOK);

	}

	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT  : et_cmd_alive ");
	return(XDC_OK);
		  
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	et_cmd_synchro ( 
        int 			va_socket)
 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel≈e pour verifier la communication 
*
* FONCTION 
* Envoyer a la tache maitre Wizcon ou esclave Wizcon la notion de maitre  
*
------------------------------------------------------*/
{
	static char *version = "$Id: eniw_cmd.c,v 1.6 2021/01/29 14:00:56 akkaakka Exp $      : et_cmd_synchro " ;
	XZEXT_MSG_SOCKET	vl_Msg; 
	short			vl_PosMsg = 0;
	double vl_HorodateSec;
	struct tm *		pl_InfoTemps ;
	time_t                vl_Heure;
   	int vl_JourSem;
     char *tz;


 	XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_synchro ");

	/* Utilisation de la fonctoin RTWorks : TutGetWallTime */
   // vvl_HorodateSec = TutGetWallTime() ;  Absurde !!!!
   
   // reccupere la date courrante en Nb de seconde "TZ"
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
   // transforme le nombre de seconde TZ   en structure tm (char) 
   // ici la fonction XZSM_11 ne peut pas etre utilisee car le formatage necessaire 
   // n'est pas celui retourne par celle-ci.
   vl_Heure = (const time_t) (vl_HorodateSec);
   tz = getenv("TZ");
	setenv("TZ", "", 1);
	tzset();
	pl_InfoTemps    = localtime(&vl_Heure) ;
	if (tz)
	    setenv("TZ", tz, 1);
	else
	    unsetenv("TZ");
	tzset();


	pl_InfoTemps->tm_mon = pl_InfoTemps->tm_mon + 1;
 	                        

        /*A Envoi de la trame */
       	 fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
       	if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 ) 				  				
       	{

 	        /* Ecriture dans le fichier trace en cas d'≈chec */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'≈mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
		return(XDC_NOK);

	}
	XZST_03EcritureTrace(XZSTC_FONCTION, " OUT  : et_cmd_synchro ");
	return(XDC_OK);
		  
}
