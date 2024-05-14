/*E*/
/*Fichier : $Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/05/10 12:08:07 $
------------------------------------------------------
* GIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TEOMi * FICHIER eomi_pro.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion du protocole de la tache TEOMi.
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1 	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     04 Mai 1995     : Reprise entete et commentaire
* D.Mismer      Version 1.3     22 Mai 1995     : Modif gestion deconnexion ST
* Mismer.D      version 1.4     13 Jun 1995     : Modif reconnexion RTserver
* Mismer.D  	version 1.5	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.6	15 Nov 1995	: Modif timeout echange modem
* Mismer.D  	version 1.7	07 Dec 1995	: Modif ouverture connexion transerver
* Mismer.D  	version 1.8 	30 Avr 1996	: Modif pour installation au CI (DEM/1098)
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "eomi_pro.h"

/* definitions de constantes */
#define CM_TIMEOUT_CMD   		10.0	
#define CM_TIMEOUT_NUMEROTATION 	30.0
#define CM_TIMEOUT_REP_MODEM		1.0
#define CM_TIMEOUT_REP_SER_OPE		3.0
#define CM_TIMEOUT_FLUSH		5.0
#define CM_ENV_OPERATOR_0              "ATE0\r"
#define CM_ENV_OPERATOR_1              "AT&K0;&D0;&O1;B2\r"   /* Force DSR; Force DTR; Appel uniquement; Scan automatique (pour recherche V23) */
#define CM_ENV_OPERATOR_2              "ATDT0,36090940\r"
#define CM_ENV_OPERATOR_3              "AUTOMA\r"
#define CM_REP_OPERATOR_0              "OK"
#define CM_REP_OPERATOR_1              "CONNECT"
#define CM_REP_OPERATOR_2              "veuillez saisir votre message:> "

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $ : eomi_pro" ;

/* declaration des variables externes.

/* declaration de fonctions externes */

/*extern int	ecom_init_OPE( XZEXT_ST_Config *, XZEXT_MASK **, XZEXT_ST_Sockets *  );*/
extern int	ecom_extraire_cmd_OPE( XZEXT_Msg_Socket *, XZEXT_Cmde_LCR *, int );

/* definition de fonctions internes */
 
void epro_envoi_trame_OPE   ( XZEXT_Cmde_LCR * );
void epro_cr_tache_teope    (  XZEXT_Cmde_LCR *, EPROT_Conditions *, int );
int epro_gestion_trame_OPE  (  EPROT_Conditions *, XZEXT_Cmde_LCR *, char *, int );
int  epro_traitement_msg_lser( XZEXT_Cmde_LCR *, XZEXT_Msg_Socket *, EPROT_Conditions *, int);
void epro_traitement_msg_donn( XZEXT_Msg_Socket * );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi d'une trame suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_OPE(  XZEXT_Cmde_LCR *pa_CmdeLCR )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. OPERATOR de structure LCR a envoyer.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde OPERATOR format LCR a envoyer lors de la phase 3.
*
* FONCTION 
*
* - envoi trames pour serveur OPERATOR et attente reponse (en 3 phases)
* - empilement des eventuelles demandes des clients TEOPE ds. une liste lors
*   de l'attente de la reponse au mesage 
*
------------------------------------------------------*/
void epro_envoi_trame_OPE(  XZEXT_Cmde_LCR *pa_CmdeLCR )
{
    static char *version = "$Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $ : epro_envoi_trame_OPE " ;
    
    XDY_Entier		  vl_TypeCmde;	/* juste pour recuperer le type de commande */
    EPROT_Conditions	  vl_Cond;	/* conditions d'envoi trame : 	nbre. essais,
    									nbre. erreurs,
    									nbre. non reponses */
    XZEXT_Msg_Socket	vl_MsgErr;	/* message d'erreur renvoye a TEOPE */
    int			vl_AttenteRep;	/* XDC_VRAI s'il faut attendre la rep. de l'equip. */	
    XZEXT_Trame		vl_deb_MsgOPE;	/* debut trame OPE a envoyer */
    int			vl_envoiOK ;    /* envoi message au serveur OPERATOR effectue correctement */
    XZEXT_Trame		vl_MsgOPE;	/* trame OPE */    	

    /*A Recuperation du type de message */
    vl_TypeCmde = pa_CmdeLCR->type_cmd;	
    
    /*A mise a jour conds. essais envoi trame */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_Cond.nb_err_crc 	= 0;
    vl_Cond.nb_non_rep 	= 0;

    /*A SI liaison ST HS envoi Non reponse */
    if ( vg_EtatST != XDC_OK )
    {
        /* ENVOI MESSAGE INTERMEDIAIRE NORESPONS */
        epro_cr_tache_teope ( pa_CmdeLCR, &vl_Cond, XDC_NORESPONS );
        return;
    }


    /*A mise a jour flag attente de reponse selon  */
    /*A CAS type de message */
    switch( vl_TypeCmde )
    {
    /*A cas XDC_APPEL_NUM ou XDC_APPEL_ALPHA */
    case XDC_APPEL_NUM :
    case XDC_APPEL_ALPHA :
    	/*B mise a jour flag a vrai */
    	vl_AttenteRep = XDC_VRAI;
    	break;	
 
    /*A autres cas : aucun traitement */	
    default : return;
    }
  
    /*A
    * sequence envoi message TEOPE sur la ligne serie geree :
    *
    * Operation effectuee en 3 phases :
    * 	phase 1 : demande connection -> "ATDP36090940;"
    *             retour (provenant du serveur OPERATOR) code "CONNECT"
    *	phase 2 : envoi mot de passe -> "AUTOMA"
    *		  retour message "veuillez saisir votre message"
    *   phase 3 : envoi du message -> "P< N ou A >,< no operateur >,< no tel ou message >"
    *             retour "<code retour>:<explication code retour>" puis deconnexion du serveur
    */
    
    /*A TANT QUE essais encore a faire */
    
    vl_envoiOK = XDC_FAUX ;
    while ((vl_envoiOK == XDC_FAUX) && ( vl_Cond.nb_essais > 0 ))
    {
        /*B decrementer nbre. essais */
        vl_Cond.nb_essais--;

        /*B Phase 0 : mode sans echo  */
    	vl_Cond.val_timer = CM_TIMEOUT_CMD;
        if (epro_gestion_trame_OPE( &vl_Cond, pa_CmdeLCR , CM_ENV_OPERATOR_0, XDC_PHASE_1 ) != XDC_OK ) continue ;
    	vl_Cond.val_timer = CM_TIMEOUT_CMD;
        if (epro_gestion_trame_OPE( &vl_Cond, pa_CmdeLCR , CM_ENV_OPERATOR_1, XDC_PHASE_1 ) != XDC_OK ) continue ;
       
        /*A ENVOI MESSAGE INTERMEDIAIRE */
        epro_cr_tache_teope ( pa_CmdeLCR, &vl_Cond, XDC_PHASE_1 );
  
        /*B Phase 1 : envoi demande de connexion avec serveur OPERATOR */
    	vl_Cond.val_timer = CM_TIMEOUT_NUMEROTATION;
        if (epro_gestion_trame_OPE( &vl_Cond, pa_CmdeLCR , CM_ENV_OPERATOR_2, XDC_PHASE_1 ) != XDC_OK ) continue ;
        
        /* ENVOI MESSAGE INTERMEDIAIRE */
        epro_cr_tache_teope ( pa_CmdeLCR, &vl_Cond, XDC_PHASE_2 );
        

        /*A Phase 2 : envoi mot de passe au serveur OPERATOR */
        
        ecom_flush_buff ( vg_DesSocks.curr, CM_TIMEOUT_FLUSH );
    	vl_Cond.val_timer = CM_TIMEOUT_CMD;
        if (epro_gestion_trame_OPE( &vl_Cond, pa_CmdeLCR ,CM_ENV_OPERATOR_3,  XDC_PHASE_2 ) != XDC_OK ) continue ;
        
        /*A ENVOI MESSAGE INTERMEDIAIRE */
        epro_cr_tache_teope ( pa_CmdeLCR, &vl_Cond, XDC_PHASE_2 );

        
        /*A Phase 3 : envoi message au serveur OPERATOR */
    	vl_Cond.val_timer = CM_TIMEOUT_CMD;
        if (epro_gestion_trame_OPE( &vl_Cond, pa_CmdeLCR , pa_CmdeLCR->cmd_LCR, XDC_PHASE_3 ) != XDC_OK ) continue ;
        ecom_flush_buff ( vg_DesSocks.curr, CM_TIMEOUT_FLUSH );

        
        /*A Toutes les phases se sont correctement deroulees */  		
    	vl_envoiOK = XDC_VRAI ;
    }
    /*A FIN TANT QUE NbEssais > 0 */
    
    
    /*A SUR DEFAUT ENVOI MESSAGE */
    if (vl_envoiOK != XDC_VRAI) 
    {
      /*A Nbre. d'essai epuise => envoi reponse CMDE. OPERATOR NON TRANSMISE a la tache cliente TEOPE */
       epro_cr_tache_teope ( pa_CmdeLCR, &vl_Cond, XDC_NORESPONS );
    }
    
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi d'un compte rendu a teope 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_cr_tache_teope(  XZEXT_Cmde_LCR *pa_CmdeLCR, EPROT_Conditions *pa_Cond, int va_phase )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. OPERATOR de structure LCR a envoyer.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*
------------------------------------------------------*/
void epro_cr_tache_teope(  XZEXT_Cmde_LCR *pa_CmdeLCR, EPROT_Conditions *pa_Cond, int va_phase )
{
    static char *version = "$Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $ : epro_cr_tache_teope " ;
    XZEXT_Msg_Socket	vl_MsgInter ;	/* message renvoye a TEOPE */
    XZSMT_Horodate	vl_Horodate;	/* horodate en format lisible */
    
      /* recuperation no liaison serie pour TEOPE */
      pa_CmdeLCR->rgs[0] = vg_Config.no_ligne ;
      pa_CmdeLCR->rgs[1] = '\0' ;
    
      sprintf( vl_MsgInter.msg, "rep. teomi %s %d %d %d %s %d %d %s %s %s",
			        ecom_lire_horodate( &vl_Horodate ),
				pa_Cond->nb_non_rep,
    	    			pa_Cond->nb_err_crc,	
    	    			pa_CmdeLCR->no_cmd,
    	    			pa_CmdeLCR->rgs,
    	    			pa_CmdeLCR->type_cmd,
    	    			va_phase,
    	    			XZEXC_SEP_TRAME,
    	    			pa_CmdeLCR->cmd_LCR,
    	    			XZEXC_FIN_MSG_SOCK );
      vl_MsgInter.len = strlen( vl_MsgInter.msg );

      /*B emission reponse vers la tache TEOPE client */
      /*B SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEOPE impossible. " */
      if (pa_CmdeLCR->sock_clnt > vg_sock_TEOPE) vg_sock_TEOPE = pa_CmdeLCR->sock_clnt ;
      sem_wait(&vg_semaphore); 
      if( write( vg_sock_TEOPE, vl_MsgInter.msg, vl_MsgInter.len ) != vl_MsgInter.len )
      {
 	XZST_03EcritureTrace( XZSTC_WARNING, "Emission message vers client TEOPE impossible." );
      }
      sem_post(&vg_semaphore); 

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi trame de connexion avec le serveur OPERATOR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_gestion_trame_OPE( EPROT_Conditions *pa_Cond,  
*			       XZEXT_Cmde_LCR   *pa_CmdeLCR,
*                              char 		*pa_MsgOPE,
*			       int		va_phase  )
*
* ARGUMENTS EN ENTREE :
* pa_Cond	: pointeur sur la structure des conds. pour gerer le protocole.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR : 
* XDC_OK	: si pas d'erreur detectee.
* XDC_NOK	: sinon.
*
* CONDITION D'UTILISATION
* pa_Cond	: pointeur sur la structure des conds. pour gerer le protocole.
*
* FONCTION 
*
* - envoi trame suppression mode echo et vidange buffer de reception
* - empilement des eventuelles demandes des clients TEOPE ds. une liste pendant
*   l'attente de la reponse au mesage TEDI/LCR
*
------------------------------------------------------*/
int epro_gestion_trame_OPE(  EPROT_Conditions *pa_Cond, 
			     XZEXT_Cmde_LCR   *pa_CmdeLCR,
                             char 	      *pa_MsgOPE,
			     int	      va_phase  )
{

    static char *version = "$Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $ : epro_gestion_trame_OPE " ;
    
    XZEXT_MASK		vl_ElmSockList;	/* pour l'insertion sock. RTServer ds. la liste */
    XZEXT_Msg_Socket	vl_MsgSock;	/* message lu localement (venant de L.SERIE ou TEOPE)  */		
    int			vl_EtatCnx;	/* etat conexion socket active apres lecture */
    int			vl_result_lect; /* resultat lecture message */
    
    
    /*A mode suppression echo sur socket connex. serveur terminaux - ligne serie geree */
    /*A SI emission echoue ALORS ecriture trace : "Emission message vers serv. terms. impossible "*/
    
    sem_wait(&vg_semaphore); 
    if( write( vg_DesSocks.lser, pa_MsgOPE, strlen(pa_MsgOPE) ) != strlen(pa_MsgOPE) )
    {
      sem_post(&vg_semaphore); 
      XZST_03EcritureTrace( XZSTC_WARNING, " Emission message supp. echo vers serv. OPERATOR impossible " );
      
      /*B incrementer nbr. non reponse */
      pa_Cond->nb_non_rep++;
      
      /*B retour a la fonction appelante pour relancer un nouvel essai */
      return(XDC_NOK) ;
    }
    sem_post(&vg_semaphore); 
    XZST_03EcritureTrace( XZSTC_INFO, "epro_gestion_trame_OPE phase:%d envoi <%s>", 
            		va_phase, pa_MsgOPE );

    /*A TANT QUE timer non ecoule */
    while( pa_Cond->val_timer > XZEXC_TIMER_ECOULE )
    {
 
	/*B attente avec TIMER d'un evnmt. sur la liste des sockets */
	/*B SI attente echoue retour vers fonction appelante */
	if( ex_attendre_msg_ST(  XZEXC_SOCKET_NULL, &vg_SockList, 
    			       pa_Cond->val_timer, &vg_DesSocks.curr, 
		               &pa_Cond->val_timer ) != XDC_OK )
	{
        /*B ALORS ecriture trace :  " Probleme attente message avec timer sur socket(s)." */
        ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
        vg_DesSocks.curr = XZEXC_NO_EVT;
        }
        else
        {
        ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );
	}    
	
	if( vg_DesSocks.curr == XZEXC_NO_EVT )
	{
        /*B ALORS incrementer nbre; non reponses */
	  pa_Cond->nb_non_rep++;
	}
		
        /*B SINON O.K., socket active soit lser, soit donnees */
	else
	{
      	  /*B detection adresse socket active */
	  	ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );
	  
	  /*B SI lecture sur socket active echoue */
	    	if (edir_detecte_type_sock() == XZEXC_SOCK_ACT_LSER) {
	    	 vl_result_lect =
	    	 ecom_lire_msg_OPE( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len, 
	    		     &vg_SockList, &vl_EtatCnx, (va_phase == 1) ? CM_TIMEOUT_REP_MODEM : CM_TIMEOUT_REP_SER_OPE );
	    	}
	    	else
	    	{
	    	 vl_result_lect =
	    	 ex_lire_msg( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len, 
	    			     &vg_SockList, &vl_EtatCnx ) ;
	    	}
	    		    	
      	  /*B SI lecture sur socket active echoue */
	  	if( vl_result_lect != XDC_OK )
	  	{
	  /*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
	    	  XZST_03EcritureTrace( XZSTC_WARNING, " Probleme lecture socket adresse %s...",
			  	  vg_DesSocks.curr_addr );
	  	}
	  /*B SINON SI deconnexion d'un client ALORS appel edir_traitement_dcnx() */
		else if( vl_EtatCnx == XZEXC_DECONNEC )
    	    	{
		  edir_traitement_dcnx();
		}
	  /*B SINON detection type socket active et traitement message recu */
	     	else
	    	{
		/*B CAS type de socket active */
		  switch( edir_detecte_type_sock() )
	    	  { 
	    	/*B cas : XZEXC_SOCK_ACT_LSER : sock. ligne serie active */
		    case XZEXC_SOCK_ACT_LSER :	
		      if( epro_traitement_msg_lser( pa_CmdeLCR, &vl_MsgSock, pa_Cond, va_phase ) == XDC_OK  )
			 return(XDC_OK);		/* SORTIE DE LA FONCTION */
		      else
		         pa_Cond->val_timer = XZEXC_TIMER_ECOULE;
		      break;
	    	
		/*B cas : XZEXC_SOCK_ACT_DONN : sock. donnees client active */
		    case XZEXC_SOCK_ACT_DONN : 	
		/*B appel epro_traitement_msg_donn() */
		      epro_traitement_msg_donn( &vl_MsgSock );
	    	      break;
	    	    	    
	    	/*B cas defaut : XZEXC_SOCK_ACT_UNDEF : sock. ecoute active = cnx. nouveau clnt. */
	    	    default :	
	    	      break;
	    	   }
	    	   /*B FIN CAS */
	    	}
	  /*B FIN SI lecture SINON ... */
		
	}
	/*B FIN SI socket RTServer SINON SI ...*/

	if ( vg_EtatST != XDC_OK )
	{
	    pa_Cond->val_timer = XZEXC_TIMER_ECOULE;
	    pa_Cond->nb_essais = 0;
	}
 
      
    }
    /*B FIN TANT QUE timer non ecoule */	

    return (XDC_NOK) ;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement d'un message recu sur la socket comms. avec le serveur de terminaux.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  int epro_traitement_msg_lser(  XZEXT_Cmde_LCR *pa_CmdeLCR , 
*				XZEXT_Msg_Socket *pa_MsgSock,
*				EPROT_Conditions *pa_Cond,
*				int		 va_phase )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. dont on recoit la reponse sur la ligne serie.
* pa_MsgSock	: pointeur sur le message recu sur la socket comms; avec la ligne serie geree.
* pa_Cond	: pointeur sur la structure des conds. pour gerer le protocole.
* 
*
* ARGUMENTS EN SORTIE :
* 
* pa_Cond	: les conditions mises a jour en fonction de la trame recue.
*
* CODE RETOUR : 
*
* XDC_OK	: si pas d'erreur CRC detectee.
* XDC_NOK	: sinon.
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. dont on recoit la reponse.
* pa_MsgSock doit contenir la reponse a pa_CmdeLCR.
*
* FONCTION 
*
* - detection type reponse ( message bloc unique, bloc intermediaire, aquittement ).
* - si c'est le cas, envoi compte rendu a la atche TEzzz ayant envoye pa_CmdeLCR.
*
------------------------------------------------------*/
int epro_traitement_msg_lser(   XZEXT_Cmde_LCR  *pa_CmdeLCR, 
				XZEXT_Msg_Socket *pa_MsgSock,
				EPROT_Conditions *pa_Cond,
				int		 va_phase )
{
    static char *version = "$Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $ : epro_traitement_msg_lser";

    XZEXT_Trame		vl_Trame;	     /* trame OPERATOR a decortiquer */
    char *		pl_retour;
    XDY_Entier		vl_CodeResult = -1 ; /* code resultat sous forme numerique */
    int			vl_ValRet     = XDC_OK;	
    int			vl_LgEcho     = 0;
    
   /*A Recuperation trame provenant du serveur de terminaux */
   strcpy(vl_Trame, pa_MsgSock->msg);
   
   switch ( va_phase )
   {
      case XDC_PHASE_3 :
         vl_LgEcho = strlen(pa_CmdeLCR->cmd_LCR) -1;
         vl_LgEcho = (vl_LgEcho >= strlen(vl_Trame)) ? 0 : vl_LgEcho;
         if ( (pl_retour = strtok (vl_Trame+vl_LgEcho, ":")) != NULL )
         {
    	    pl_retour = pl_retour + strlen (pl_retour) - 2;
    	    XZST_03EcritureTrace( XZSTC_INFO, "Reponse phase 3 :<%s>", pl_retour );
            /*A Recuperation code erreur de la trame */ 
            vl_CodeResult = atoi(pl_retour);
            /*B message bien recu, mise a zero du nbre. d'essais */
            pa_Cond->nb_essais = 0;
    		
            /*A ENVOI MESSAGE INTERMEDIAIRE */
            epro_cr_tache_teope ( pa_CmdeLCR, pa_Cond, vl_CodeResult );
    	
         }
      break;

      case XDC_PHASE_2 :
    	 XZST_03EcritureTrace( XZSTC_INFO, "Reponse phase 2 :<OK> <%s>", vl_Trame );
      break;
      
      case XDC_PHASE_1 :
    	 XZST_03EcritureTrace( XZSTC_INFO, "Reponse phase 1 :<%s>", vl_Trame );
         if ( (strstr (vl_Trame, CM_REP_OPERATOR_0) == NULL) && (strstr (vl_Trame, CM_REP_OPERATOR_1) == NULL) ) vl_ValRet = XDC_NOK;
      break;
      
   }
   
   return( vl_ValRet );
 
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement d'un message recu sur une sock. comms. avec un client TEOPE.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_traitement_msg_donn( XZEXT_Msg_Socket *pa_MsgSock )
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: pointeur sur le message recu sur la socket donnees active.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR : 
*
*   NEANT.
*
* CONDITION D'UTILISATION
*
* pa_MsgSock doit contenir un message de la part d'un client TEOPE.
*
* FONCTION 
* - si demande arret appel ecfg_sortir().
* - si demande init appel ecom_init().
* - si demande LCR :
*  	depouillement du message recu du client TEOPE ds. une struct type XZEXT_Cmde_LCR.
*  	empilement de cette meme demande ds. la liste des demandes a traiter.
*
------------------------------------------------------*/
void epro_traitement_msg_donn( XZEXT_Msg_Socket *pa_MsgSock )
{
    static char *version = "$Id: eomi_pro.c,v 1.8 1996/05/10 12:08:07 volcic Exp $ : epro_traitement_msg_donn";

  
    XZEXT_Cmde_LCR	vl_CmdeLCR;	/* cmde a empiler ds. la liste */
    XDY_Texte		vl_Texte;
    int			vl_ValRet;

    /*A SI demande d'arret ALORS appel ecfg_sortir() */
    if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
    {
    	XZST_03EcritureTrace( XZSTC_INFO, "epro_traitement_msg_donn : demande  arret TEOMi %d",
    				vg_Config.no_ligne );
    	ecfg_sortir();
    }
    /*A SINON SI demande init. ALORS appel ecom_init() */
    else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_INIT ) != NULL )
    {
    	XZST_03EcritureTrace( XZSTC_INFO, "epro_traitement_msg_donn : demande  reinitialisation TEOMi %d",
    				vg_Config.no_ligne );
	
	/*A SI init echoue pour cause de pb. ST  */
	if( (vg_EtatST = ecom_init( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
	{
    	    XZST_03EcritureTrace( XZSTC_WARNING, "epro_traitement_msg_donn : Echec relance initialisation TEOMi %d",
    	    				vg_Config.no_ligne );
    	}
    }
    /*A SINON SI demande envoi trame OPERATOR sur ligne serie geree ALORS */
    else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_OPE ) != NULL )
    {
    	/*B SI extraction cmde. OPERATOR ecom_extraire_cmd_OPE() OK */
	if( ecom_extraire_cmd_OPE( pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr ) == XDC_OK )
	{
	    /*B ALORS insertion cmde LCR en liste d'attente ecom_insert_cmd_LCR() */
	    ecom_insert_cmd_LCR( &vl_CmdeLCR, &pg_ListeCmdesLCR );
	}
    }
    /*A FIN SI */
    	 
}
