/*E*/
/*Fichier : $Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $      Release : $Revision: 1.12 $        Date : $Date: 1995/11/27 11:51:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_pro.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion du protocole maitre de la tache TETMi.
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Mismer D.     Version 1.2     17 Mar 1995     : RAS
* Fontaine C.	Version 1.3	08 Mar 1995	: Ajout des fonctions :
*                                                 epro_envoi_trame_REP,
*                                                 epro_phase_trame_REP,
*                                                 pour prise en compte modif repondeur
* Fontaine C.   Version 1.4     24 Avr 1995     : Remplacement du mot "ALCATEL" par "***"
*						  dans epro_phase_REP
* Fontaine.C    Version 1.5     28 Avr 1995     : Modification sequence Repondeur apres mise en exploitation
* Fontaine.C    Version 1.6     03 Mai 1995     : Reprise des entetes, et commentaires
* Fontaine.C    Version 1.7     17 Mai 1995     : Reprise historique
* D.Mismer      Version 1.8     22 Mai 1995     : Modif gestion deconnexion ST
* Mismer.D      version 1.9     13 Jun 1995     : Modif reconnexion RTserver
* Mismer.D  	version 1.10	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.11	05 Oct 1995	: Modif time out repondeur et niveau trace
* Mismer.D  	version 1.12	20 Oct 1995	: Modif time out repondeur suite
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "etmi_pro.h"

/* definitions de constantes */
#define	CM_TIME_OUT_CNX_REPONDEUR 	10.0
#define	CM_TIME_OUT_CMD_REPONDEUR 	3.0
#define	CM_TIME_OUT_CMD_TELEPHONE 	2.0

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $ : etmi_pro" ;

/* declaration des variables externes.

/* declaration de fonctions externes */
extern int ecom_init_TEL( XZEXT_ST_Config *pa_Config, XZEXT_MASK **, XZEXT_ST_Sockets * );
extern int ecom_extraire_cmd_TEL( XZEXT_Msg_Socket *, XZEXT_Cmde_LCR *, int);

/* definition de fonctions internes */

void epro_envoi_trame_TEL( XZEXT_Cmde_LCR * );
void epro_envoi_trame_REP( XZEXT_Cmde_LCR * );
int  epro_phase_trame_REP(  EPROT_Conditions * , char * );

int  epro_traitement_msg_lser( XZEXT_Cmde_LCR *, XZEXT_Msg_Socket *, 
				EPROT_Conditions *);
void epro_traitement_msg_donn( XZEXT_Msg_Socket * );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi d'une trame type telephonique suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_TEL(  XZEXT_Cmde_LCR *pa_CmdeLCR )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. TEL de structure LCR a envoyer.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. a envoyer.
*
* FONCTION 
*
* - envoi trame TEL et attente reponse
* - empilement des eventuelles demandes des clients TETEL ds. une liste lors
*   de l'attente de la reponse au mesage precedent
*
------------------------------------------------------*/
void epro_envoi_trame_TEL(  XZEXT_Cmde_LCR *pa_CmdeLCR )
{
    static char *version = "$Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $ : epro_envoi_trame_TEL " ;
    
    XDY_Entier		  vl_TypeCmde;	/* juste pour recuperer le type de commande */
    EPROT_Conditions	  vl_Cond;	/* conditions d'envoi trame : 	nbre. essais,
    									nbre. erreurs,
    									nbre. non reponses,
    					*/
    XZEXT_Msg_Socket	vl_MsgSock;	/* message lu localement */
    int			vl_EtatCnx;	/* etat conexion socket active apres lecture */
    XZEXT_Trame		vl_MsgTEL;	/* trame TEL a envoyer */
    XZEXT_Trame		vl_deb_MsgTEL;	/* debut trame TEL a envoyer */
    XZSMT_Horodate	vl_Horodate;	/* horodate en format lisible */
    int			vl_result_lect; /* resultat lecture message */
         
    XZST_03EcritureTrace( XZSTC_FONCTION, "epro_envoi_trame_TEL : IN" );
  
    /*A Recuperation du type de message */
    vl_TypeCmde = pa_CmdeLCR->type_cmd;	
    
    /*A Si liaison serie HS renvoi Non Reponse */
    if ( vg_EtatST != XDC_OK )
    {
       /*A formatage reponse */
       sprintf( vl_MsgSock.msg, "rep. tetmi %s 0 0 0 %s %d %d %s %s %s",
				ecom_lire_horodate( &vl_Horodate ),
    	    			pa_CmdeLCR->rgs,
    	    			pa_CmdeLCR->type_cmd,
    	    			XZEXC_TRAME_NON_TRANSM,
    	    			XZEXC_SEP_TRAME,
    	    			pa_CmdeLCR->cmd_LCR,
    	    			XZEXC_FIN_MSG_SOCK );
    	    			
       vl_MsgSock.len = strlen( vl_MsgSock.msg );

       /*A emission reponse vers la tache TETEL client */
       /*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TETEL impossible. " */
       if (pa_CmdeLCR->sock_clnt > vg_sock_TETEL) vg_sock_TETEL = pa_CmdeLCR->sock_clnt ;
	   sem_wait(&vg_semaphore); 
       if( send( vg_sock_TETEL, vl_MsgSock.msg, vl_MsgSock.len, 0 ) <= 0 )
       {
 	   		XZST_03EcritureTrace( XZSTC_WARNING, "Emission message vers client TETEL impossible." );
       }
	   sem_post(&vg_semaphore); 
    }
    
    /*A mise a jour conds. essais envoi trame */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_Cond.nb_err_crc 	= 0;
    vl_Cond.nb_non_rep 	= 0;
    
    /*A Recuperation du message de TETEL a envoyer */
    sprintf( vl_MsgTEL, "%s\r\n", pa_CmdeLCR->cmd_LCR) ;
    XZST_03EcritureTrace( XZSTC_DEBUG1, "epro_envoi_trame_TEL: envoi %s Nbre d'essais : %d", pa_CmdeLCR->cmd_LCR,vl_Cond.nb_essais ); 
  
    /*A
    * sequence envoi message TETEL sur la ligne serie geree 
    */
    
    /*A TANT QUE essais encore a faire */
    while( vl_Cond.nb_essais > 0 )
    {
        /*B decrementer nbre. essais */
        vl_Cond.nb_essais--;

		/*B Recup. valeur timer attente reponse sur ligne serie */
		strcpy(vl_deb_MsgTEL, vl_MsgTEL) ;
		vl_deb_MsgTEL[4] = '\0' ;
		if (strstr(vl_deb_MsgTEL, "ATS7") != NULL) 
			vg_val_timer = atof(&vl_deb_MsgTEL[5]) + CM_TIME_OUT_CMD_TELEPHONE ;
		vl_Cond.val_timer = vg_val_timer; 
			
		/*B emission message sur socket connex. serveur terminaux - ligne serie geree */
		/*B SI emission echoue ALORS ecriture trace : "Emission message vers serv. terms. impossible "*/
		
		sem_wait(&vg_semaphore);
		if( send( vg_DesSocks.lser, vl_MsgTEL, strlen(vl_MsgTEL), 0 ) <= 0 )
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "epro_envoi_trame_TEL : Emission message <%s> vers serv. terms. impossible ", vl_MsgTEL );
			/*B mise timer a XZEXC_TIMER_ECOULE pour relancer un nouvel essai */
			vl_Cond.val_timer = XZEXC_TIMER_ECOULE;
			/*B incremernter nbr. non rep */
			vl_Cond.nb_non_rep++;
		}
		else
		{
			XZST_03EcritureTrace( XZSTC_INFO, "epro_envoi_trame_TEL : Emission message <%s> vers serv. terms ",vl_MsgTEL);
		}
		sem_post(&vg_semaphore);
    	

	/*B TANT QUE timer non ecoule */
	while( vl_Cond.val_timer > XZEXC_TIMER_ECOULE )
	{
	    /*B SI pas d'attente reponse ALORS return */
	    if ( vl_TypeCmde == XZEXC_MAA_SANS_REPONSE )
	    {
	    	vl_Cond.val_timer = CM_TIME_OUT_CMD_TELEPHONE;
	    }
	    
	    /*B attente avec TIMER d'un evnmt. sur la liste des sockets */
	    /*B SI attente echoue */
	    if ( ex_attendre_msg_ST( XZEXC_SOCKET_NULL, &vg_SockList, 
	    			     vl_Cond.val_timer, &vg_DesSocks.curr, 
	    			     &vl_Cond.val_timer ) != XDC_OK )
	    {
	    	/*B ALORS ecriture trace :  " Probleme attente message avec timer sur socket(s)." */
                ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
	        continue;
        }
		/*B SINON */
		else
		{
			ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );
			if ( vg_DesSocks.curr == vg_DesSocks.rtsrv ) continue;
			if ( vg_DesSocks.curr == XZEXC_NO_EVT )
			{
				/*B ALORS incrementer nbre; non reponses */
				if ( vl_TypeCmde == XZEXC_MAA_SANS_REPONSE ) return;
				vl_Cond.nb_non_rep++;
			}
			/*B SINON O.K., socket active soit lser, soit donnees */
			else
			{
				/*B detection adresse socket active */
				ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );

				/*B SI lecture sur socket active echoue */
				if (edir_detecte_type_sock() == XZEXC_SOCK_ACT_LSER) 
				{
					vl_result_lect = ecom_lire_msg_TEL( vg_DesSocks.curr, 
									vl_MsgSock.msg, 
									&vl_MsgSock.len, 
									&vg_SockList, 
									&vl_EtatCnx, 
									CM_TIME_OUT_CMD_TELEPHONE ) ;
				}
				else
				{
					vl_result_lect = ex_lire_msg(       vg_DesSocks.curr, 
								vl_MsgSock.msg, 
								&vl_MsgSock.len, 
									&vg_SockList, 
									&vl_EtatCnx ) ;
				}
				if ( vl_result_lect != XDC_OK ) 	     
				{
					/*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
					XZST_03EcritureTrace( XZSTC_WARNING, " Probleme lecture socket adresse %s...", vg_DesSocks.curr_addr );
				}
				/*B SINON SI deconnexion d'un client ALORS appel edir_traitement_dcnx() */
				else 
				{
					if( vl_EtatCnx == XZEXC_DECONNEC )
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
								/*B appel epro_traitement_msg_lser() */
							/*B SI traitement msg. recu sur ligne serie OK, ALORS return */
								if( epro_traitement_msg_lser( pa_CmdeLCR, &vl_MsgSock, 
													&vl_Cond ) == XDC_OK )
								{
									return;
								}
								/*B SINON ereur TEDI ou CRC detectee, mise timer a zero */
								if ( vl_TypeCmde == XZEXC_MAA_SANS_REPONSE ) return;
								vl_Cond.val_timer = XZEXC_TIMER_ECOULE;
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
				/*B FIN SINON deconnection */
			}
	    	/*B FIN SI attente SINON ... */
	    }
	    /*B FIN SI emission message SINON ... */

	    if ( vg_EtatST != XDC_OK )
	    {
	       vl_Cond.val_timer = XZEXC_TIMER_ECOULE;
	       vl_Cond.nb_essais = 0;
	    }
	}
	/*B FIN TANT QUE timer non ecoule */	
    }
    /*A FIN TANT QUE NbEssais > 0 */
    
    /*A Nbre. d'essai epuise => envoi reponse CMDE. TEL NON TRANSMISE a la tache cliente TETEL */
    /*A formatage reponse */
    sprintf( vl_MsgSock.msg, "rep. tetmi %s %d %d %d %s %d %d %s %s %s",
				ecom_lire_horodate( &vl_Horodate ),
				vl_Cond.nb_non_rep,
    	    			vl_Cond.nb_err_crc,	/* Non utilise */
    	    			pa_CmdeLCR->no_cmd,
    	    			pa_CmdeLCR->rgs,
    	    			pa_CmdeLCR->type_cmd,
    	    			XZEXC_TRAME_NON_TRANSM,
    	    			XZEXC_SEP_TRAME,
    	    			pa_CmdeLCR->cmd_LCR,
    	    			XZEXC_FIN_MSG_SOCK
    	    	);
    vl_MsgSock.len = strlen( vl_MsgSock.msg );

    /*A emission reponse vers la tache TETEL client */
    /*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TETEL impossible. " */
    if (pa_CmdeLCR->sock_clnt > vg_sock_TETEL) vg_sock_TETEL = pa_CmdeLCR->sock_clnt ;
	sem_wait(&vg_semaphore);
    if( send( vg_sock_TETEL, vl_MsgSock.msg, vl_MsgSock.len, 0 ) <= 0 )
    {
 	XZST_03EcritureTrace( XZSTC_WARNING, "Emission message vers client TETEL impossible." );
    }
	sem_post(&vg_semaphore);
    /*B FIN SI */
    XZST_03EcritureTrace( XZSTC_FONCTION, "epro_envoi_trame_TEL : OUT" );

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
*				EPROT_Conditions *pa_Cond )
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
* pa_MsgTEDI	: msg. TEDI (acquit. neg. a envoyer en cas de err. CRC blc. interm. 
*
* CODE RETOUR : 
*
* XDC_OK	: si pas d'erreur CRC ou TEDI detectee.
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
int epro_traitement_msg_lser(  XZEXT_Cmde_LCR *pa_CmdeLCR , 
				XZEXT_Msg_Socket *pa_MsgSock,
				EPROT_Conditions *pa_Cond )
{
    static char *version = "$Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $ : epro_traitement_msg_lser";

    XZEXT_Trame		vl_Trame;	/* trame TEL a decortiquer */
    XZEXT_Msg_Socket	vl_MsgSock;	/* message comms. sur socket */
    XDY_Entier		vl_CodeResult = -1 ; /* code resultat sous forme numerique */
    XZSMT_Horodate	vl_Horodate;	/* horodate en format lisible */	
 
   /*A Recuperation trame provenant du serveur de terminaux */
   strcpy(vl_Trame, pa_MsgSock->msg);
   XZST_03EcritureTrace( XZSTC_DEBUG1, "epro_traitement_msg_lser: recu %s." , vl_Trame );
     
   /*A Controle si debut trame est de type :
   	- ASCII : debut <CR><LF> 
   	- NUMERIQUE : debut different de <CR><LF> */
   	
   /*A Trame decortique selon type ASCII ou NUMERIQUE */
    if ( (vl_Trame[0] == '\r') && (vl_Trame[1] == '\n') )
    { 
      /* mode ASCII */
      switch (vl_Trame[2]) 
      {
   	case 'O' :	vl_CodeResult = XDC_OK ;
   			break;
   	case 'C' :	vl_CodeResult = XDC_CONNECT ;
   			break;
   	case 'R' :	vl_CodeResult = XDC_RING ;
   			break;
   	case 'N' :	if (strstr(vl_Trame, "\r\nNO CARRIER\r\n") != NULL) 
   			 vl_CodeResult = XDC_NOCARRIER ;
   			else
   			 vl_CodeResult = XDC_NOANSWER ;
   			break;
   	case 'E' :	vl_CodeResult = XDC_ERROR ;
   			break;
   	case 'L' :	vl_CodeResult = XDC_LIBRE ;
   			break;
   	case 'B' :	vl_CodeResult = XDC_BUSY ;
   			break;

      }
    }	
    else {
      /* mode NUMERIQUE */	
   	vl_CodeResult = atoi(vl_Trame) ;
    }	
   
    /*B message bien recu, mise a zero du nbre. d'essais */
    pa_Cond->nb_essais = 0;
    		
    /*B formatage message reponse pour la tache TEzzz client */
    sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
    				XZEXC_REP_TETMi,
				ecom_lire_horodate( &vl_Horodate ),
				pa_Cond->nb_non_rep,	/* Nb non reponses */
    	    			pa_Cond->nb_err_crc,	/* Non utilise */
    	    			XDC_APPEL,		/* No commande envoye par TETEL */
    	    			pa_CmdeLCR->rgs,	/* No liaison */
    	    			pa_CmdeLCR->type_cmd,	/* Non utilise */
    	    			vl_CodeResult,		/* resultat retour */
    	    			XZEXC_SEP_TRAME,
    	    			pa_MsgSock->msg,
    	    			XZEXC_FIN_MSG_SOCK );
    	    			
    vl_MsgSock.len = strlen( vl_MsgSock.msg );
    	    
    /*B emission reponse vers la tache TETEL client */
    /*B SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TETEL impossible. " */
    if (pa_CmdeLCR->sock_clnt > vg_sock_TETEL) vg_sock_TETEL = pa_CmdeLCR->sock_clnt ;
    if( send ( vg_sock_TETEL, vl_MsgSock.msg, vl_MsgSock.len , 0 ) <= 0 )
    {
 	XZST_03EcritureTrace( XZSTC_WARNING, "Emission message vers client TETEL impossible." );
    }
    else 
    {
       	XZST_03EcritureTrace( XZSTC_INFO, "epro_traitement_msg_lser : envoi vers TETEL reussi : %d - %s",
    				vg_sock_TETEL, pa_MsgSock->msg );
    }
    /*B FIN SI */
    	
    /*B O.K, retour XDC_OK */
    return( XDC_OK );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement d'un message recu sur une sock. comms. avec un client TETEL.
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
* pa_MsgSock doit contenir un message de la part d'un client TETEL.
*
* FONCTION 
* - si demande arret appel ecfg_sortir().
* - si demande init appel ecom_init_TEL().
* - si demande LCR :
*  	depouillement du message recu du client TETEL ds. une struct type XZEXT_Cmde_LCR.
*  	empilement de cette meme demande ds. la liste des demandes a traiter.
*
------------------------------------------------------*/
void epro_traitement_msg_donn( XZEXT_Msg_Socket *pa_MsgSock )
{
    static char *version = "$Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $ : epro_traitement_msg_donn";

    XDY_Texte		vl_TypeMsg;	/* juste pour recuperer le type de message */
    XZEXT_Cmde_LCR	vl_CmdeLCR;	/* cmde LCR a empiler ds. la liste */
    XDY_Texte		vl_Texte;
    int			vl_ValRet;

    XZST_03EcritureTrace( XZSTC_FONCTION, "epro_traitement_msg_donn : IN" );
    /*A SI demande d'arret ALORS appel ecfg_sortir() */
    if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
    {
    	XZST_03EcritureTrace( XZSTC_INFO, "epro_traitement_msg_donn : demande  arret TELMi %d", vg_Config.no_ligne );
        XZST_03EcritureTrace( XZSTC_FONCTION, "epro_traitement_msg_donn : OUT" );
    	ecfg_sortir();
    }
    /*A SINON SI demande init. ALORS appel ecom_init_TEL() */
    else 
    {
       if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_INIT ) != NULL )
       {
    	  XZST_03EcritureTrace( XZSTC_INFO, "epro_traitement_msg_donn : demande  reinitialisation TELMi %d", vg_Config.no_ligne );
	 /*A SI init echoue pour cause de pb. ST ET alarme non envoyee */
	 if( (vg_EtatST = ecom_init_TEL( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
    	    XZST_03EcritureTrace( XZSTC_WARNING, "epro_traitement_msg_donn : Echec relance initialisation TETMi %d", vg_Config.no_ligne );
       }
       /*A SINON SI demande envoi trame TEL sur ligne serie geree ALORS */
       else 
       {
          if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_TEL ) != NULL )
          {
    	     /*B SI extraction cmde. TEL ecom_extraire_cmd_TEL() OK */
	     if( ecom_extraire_cmd_TEL( pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr ) == XDC_OK )
	     {
	        /*B ALORS insertion cmde LCR en liste d'attente ecom_insert_cmd_LCR() */
	        ecom_insert_cmd_LCR( &vl_CmdeLCR, &pg_ListeCmdesLCR );
	     }
	  }
       }
    }
    /*A FIN SI */
    XZST_03EcritureTrace( XZSTC_FONCTION, "epro_traitement_msg_donn : OUT" );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Envoi d'une trame type repondeur suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_REP(  XZEXT_Cmde_LCR *pa_CmdeLCR )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR    : pointeur sur cmde. TEL de structure LCR a envoyer.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR :
*
* NEANT
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. LCR a envoyer.
*
* FONCTION
*
* - envoi trame TEL et attente reponse
* - empilement des eventuelles demandes des clients TETEL ds. une liste lors
*   de l'attente de la reponse au mesage TEDI/LCR
*
------------------------------------------------------*/
void epro_envoi_trame_REP(  XZEXT_Cmde_LCR *pa_CmdeLCR )
{
   static char *version = "$Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $ : epro_envoi_trame_REP " ;

    EPROT_Conditions    vl_Cond;          /* conditions d'envoi trame :   nbre. essais,
                                                                        nbre. erreurs,
                                                                        nbre. non reponses */
    XZEXT_Msg_Socket    vl_MsgErr;        /* message d'erreur renvoye a TETEL */
    XZEXT_Trame         vl_MsgREP;        /* trame REP a envoyer */
    int                 vl_envoiOK ;      /* envoi message au serveur REPONDEUR effectue correctement */
    XZSMT_Horodate      vl_Horodate;      /* horodate en format lisible */

    
    XZST_03EcritureTrace( XZSTC_FONCTION, "epro_envoi_trame_REP : IN" );
    strcpy(vl_MsgREP, pa_CmdeLCR->cmd_LCR) ;
    if ( strstr ( vl_MsgREP, "4" ) != NULL )
    {
       XZST_03EcritureTrace( XZSTC_WARNING, "Passage sur REPONDEUR en cours." );
    }
    else
    {
       XZST_03EcritureTrace( XZSTC_WARNING, "Annulation REPONDEUR en cours." );
    }
 
    /*A mise a jour conds. essais envoi trame TEDI/LCR */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_Cond.nb_err_crc  = 0;
    vl_Cond.nb_non_rep  = 0;

    /*A Recuperation du message de TETEL a envoyer */
    strcpy(vl_MsgREP, pa_CmdeLCR->cmd_LCR) ;

    /*A
    * sequence envoi message TETEL sur la ligne serie geree
    *
    * Operation en 5 phases :
    *   phase 1 : demande de connection -> "DHM"
    *             retour (provenant du serveur REPONDEUR) page ecran
    *   phase 2 : emission du login -> "I\r1064\r"
    *   phase 3 : emission du type de commande -> "34\r"
    *   phase 4 : envoi de la trame 
    *   phase 5 : emission du fin de connection -> "Q\r"
    */

    /*A TANT QUE essais encore a faire */
    /*B Emission du DHM */
    vl_envoiOK = XDC_FAUX ;
    while (( vl_envoiOK == XDC_FAUX ) && ( vl_Cond.nb_essais > 0 ))
    {
        vl_Cond.nb_essais--;
        vl_Cond.val_timer =  CM_TIME_OUT_CNX_REPONDEUR; 
        if (epro_phase_trame_REP( &vl_Cond , "DHM" ) == XDC_NOK )
	{
	  continue ;
        }
	vl_envoiOK = XDC_VRAI ;
    }

    /*B Emission du Login */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_envoiOK = XDC_FAUX ;
    while (( vl_envoiOK == XDC_FAUX ) && ( vl_Cond.nb_essais > 0 ))
    {
	vl_Cond.nb_essais--;
	vl_Cond.val_timer = CM_TIME_OUT_CNX_REPONDEUR;
	if (epro_phase_trame_REP( &vl_Cond , "I\r1064\r" ) == XDC_NOK )
	{
	  continue ;
        }
	vl_envoiOK = XDC_VRAI ;
    }

    /*B Emission du type de commande */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_envoiOK = XDC_FAUX ;
    while (( vl_envoiOK == XDC_FAUX ) && ( vl_Cond.nb_essais > 0 ))
    {
	vl_Cond.nb_essais--;
	vl_Cond.val_timer = CM_TIME_OUT_CNX_REPONDEUR;
	if (epro_phase_trame_REP( &vl_Cond , "34\r" ) == XDC_NOK )
	{
	  continue ;
        }
	vl_envoiOK = XDC_VRAI ;
    }

    /*B Emission de la trame de mise ou d'annulation de repondeur */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_envoiOK = XDC_FAUX ;
    while (( vl_envoiOK == XDC_FAUX ) && ( vl_Cond.nb_essais > 0 ))
    {
	/* Emission de la demande de rentree en saisie */
	vl_Cond.val_timer = CM_TIME_OUT_CNX_REPONDEUR;
	epro_phase_trame_REP( &vl_Cond , "S" );

	/* Emission de la trame */
        vl_Cond.nb_essais--;
	vl_Cond.val_timer = CM_TIME_OUT_CNX_REPONDEUR;
	if (epro_phase_trame_REP( &vl_Cond , pa_CmdeLCR->cmd_LCR ) == XDC_NOK )
	{
	  continue ;
        }
	vl_envoiOK = XDC_VRAI ;
    }

    /*B Emission du fin de connection */
    vl_Cond.nb_essais = vg_Config.ligne_serie.NbEssais;
    vl_envoiOK = XDC_FAUX ;
    while (( vl_envoiOK == XDC_FAUX ) && ( vl_Cond.nb_essais > 0 ))
    {
        vl_Cond.nb_essais--;
	vl_Cond.val_timer = CM_TIME_OUT_CNX_REPONDEUR;
        if (epro_phase_trame_REP( &vl_Cond , "Q\r" ) == XDC_NOK )
	{
	  continue ;
	}
        vl_envoiOK = XDC_VRAI ;
    }

    /*A ENVOI MESSAGE MSG COMPTE RENDU */
    /*B formatage reponse */

    sprintf( vl_MsgErr.msg, "rep. tetmi %s %d %d %d %s %d %d %s %s %s",
                                ecom_lire_horodate( &vl_Horodate ),
                                vl_Cond.nb_non_rep,
                                vl_Cond.nb_err_crc,     /* Non utilise */
                                pa_CmdeLCR->no_cmd,
                                pa_CmdeLCR->rgs,
                                pa_CmdeLCR->type_cmd,
                                (vl_envoiOK != XDC_VRAI) ? XDC_ERROR : XDC_OK,
                                XZEXC_SEP_TRAME,
                                pa_CmdeLCR->cmd_LCR,
                                XZEXC_FIN_MSG_SOCK
                );
    vl_MsgErr.len = strlen( vl_MsgErr.msg );

    /*B emission reponse vers la tache TETEL client */
    /*B SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TETEL impossible. " */
    if (pa_CmdeLCR->sock_clnt > vg_sock_TETEL) vg_sock_TETEL = pa_CmdeLCR->sock_clnt ;
	sem_wait(&vg_semaphore); 
    if( write( vg_sock_TETEL, vl_MsgErr.msg, vl_MsgErr.len ) != vl_MsgErr.len )
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "Emission message vers client TETEL impossible." );
    }
	sem_post(&vg_semaphore); 
   
   if ( strstr ( vl_MsgREP, "4" ) != NULL )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "%s passage sur REPONDEUR.\n", (vl_envoiOK == XDC_VRAI) ? "Succes" : "Echec" );
   }
   else
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "%s annulation REPONDEUR.\n", (vl_envoiOK == XDC_VRAI) ? "Succes" : "Echec" );
   }
   
   XZST_03EcritureTrace( XZSTC_FONCTION, "epro_envoi_trame_REP : OUT" );

}                            




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi trame au serveur REPONDEUR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_phase_trame_REP( EPROT_Conditions *pa_Cond , char* pa_MsgREP )
*
* ARGUMENTS EN ENTREE :
* pa_Cond	: pointeur sur la structure des conds. pour gerer le protocole.
* pa_MsgREP     : trame a emettre.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR : 
* XDC_OK	: si pas d'erreur detectee.
* XDC_NOK	: sinon.
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*
* - envoi trame au REPONDEUR et attente reponse
* - empilement des eventuelles demandes des clients TETEL ds. une liste pendant
*   l'attente de la reponse au mesage suivant 
*
------------------------------------------------------*/
int epro_phase_trame_REP(  EPROT_Conditions *pa_Cond , char* pa_MsgREP )
{

   static char *version = "$Id: etmi_pro.c,v 1.12 1995/11/27 11:51:12 volcic Exp $ : epro_phase_trame_REP " ;
    
    XZEXT_Msg_Socket	vl_MsgSock;	/* message lu localement (venant de L.SERIE ou TETEL)  */		
    int			vl_EtatCnx;	/* etat conexion socket active apres lecture */
    int			vl_result_lect; /* resultat lecture message */
    
    XZST_03EcritureTrace( XZSTC_FONCTION, "epro_phase_trame_REP : IN %s ", pa_MsgREP );
  
    /*A emission message sur socket connex. serveur terminaux - ligne serie geree */
    /*A SI emission echoue ALORS ecriture trace : "Emission message vers serv. terms. impossible "*/
	sem_wait(&vg_semaphore); 
    if( write( vg_DesSocks.lser, pa_MsgREP, strlen(pa_MsgREP) ) != strlen(pa_MsgREP) )
    {
	  sem_post(&vg_semaphore); 
      XZST_03EcritureTrace( XZSTC_WARNING, " Emission message vers serv. REPONDEUR impossible " );
      
      /*B incrementer nbr. non reponse */
      pa_Cond->nb_non_rep++;
      
      /*B retour a la fonction appelante pour relancer un nouvel essai */
      return(XDC_NOK) ;
    }
	sem_post(&vg_semaphore); 
    XZST_03EcritureTrace( XZSTC_INFO, "epro_phase_trame_REP: envoi %s.", pa_MsgREP );

    /*A TANT QUE timer non ecoule */
    while( pa_Cond->val_timer > XZEXC_TIMER_ECOULE )
    {
 
	/*B attente avec TIMER d'un evnmt. sur la liste des sockets */
	/*B SI attente echoue retour vers fonction appelante */
	if( ex_attendre_msg_ST(  XZEXC_SOCKET_NULL, &vg_SockList, 
    			         pa_Cond->val_timer, &vg_DesSocks.curr, 
		                 &pa_Cond->val_timer ) != XDC_OK )
	{
           ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
	   vg_DesSocks.curr = XZEXC_NO_EVT;
	}
	else
	{
           ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );
        }
	
        if ( vg_DesSocks.curr == vg_DesSocks.rtsrv )  continue;
	if ( vg_DesSocks.curr == XZEXC_NO_EVT )
	{
          /*B ALORS incrementer nbre; non reponses */
	  pa_Cond->nb_non_rep++;
	}
		
        /*B SINON O.K., socket active soit lser, soit donnees */
	else
	{
      	   /*B detection adresse socket active */
	   ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );
	  
	   /*B SI sock. ligne serie active */
	   if (edir_detecte_type_sock() == XZEXC_SOCK_ACT_LSER)
	   {
	      vl_result_lect = ecom_lire_msg_TEL ( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len, &vg_SockList, &vl_EtatCnx, CM_TIME_OUT_CMD_REPONDEUR ) ;
	   }
	   else
	   {
	      vl_result_lect = ex_lire_msg( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len, &vg_SockList, &vl_EtatCnx ) ;
	   }
	    	
      	   /*B SI lecture sur socket active echoue */
	   if( vl_result_lect != XDC_OK )
	   {
	      /*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
	      XZST_03EcritureTrace( XZSTC_WARNING, " Probleme lecture socket adresse %s...", vg_DesSocks.curr_addr );
	   }
	   else /*B SINON  */
	   {
	      /*B SI deconnexion d'un client ALORS appel edir_traitement_dcnx() */
	      if( vl_EtatCnx == XZEXC_DECONNEC )
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
		      /*B verification si reception code renvoi du serveur et SI msg. recu sur ligne serie OK, ALORS return */
		      if ( (strstr(vl_MsgSock.msg, "***") != NULL) || (strstr(vl_MsgSock.msg, "MENU") != NULL) ||
			   (strstr(vl_MsgSock.msg, "lecture") != NULL) || (strstr(vl_MsgSock.msg, "alidation") != NULL) ||
			   (*pa_MsgREP == 'S') )
       		      {
                         XZST_03EcritureTrace( XZSTC_INFO, "epro_phase_trame_REP : OUT sur OK : %s", pa_MsgREP );
		         return(XDC_OK);		/* SORTIE DE LA FONCTION */
		      }
		      else
		      {
		         /*B SINON erreur TEDI , mise timer a zero */
                         XZST_03EcritureTrace( XZSTC_WARNING, "epro_phase_trame_REP : Reponse NOK: %s", vl_MsgSock.msg );
		         pa_Cond->val_timer = XZEXC_TIMER_ECOULE;
		      }
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
	     /*B FIN SI  */
	  }
	  /*B FIN SI lecture SINON ... */
       }
       /*B FIN SI socket RTServer SINON SI ...*/
    }
    /*B FIN TANT QUE timer non ecoule */	
    XZST_03EcritureTrace( XZSTC_WARNING, "epro_phase_trame_REP : OUT sur NonOK %s", pa_MsgREP );

    return (XDC_NOK) ;
}



