/*E*/
/* Fichier : $Id: eopc_pro.c,v 1.1 2021/05/04 13:45:33 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2021/05/04 13:45:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER eimi_pro.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion du protocole LCR maitre de la tache TELMi.
* 
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* JMG	13/05/19 : delai timer 1.2
* NB    28/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.7
* jmg 	05/01/21 : bloc inter DEM-SAE215 1.8
* jmg  15/01/21 : gestion ddm esclave 1.9
* jmg 15/01/21 : purge forcee de la commande sur retour IP LCR 1.10
* jmg 03/02/21 : gestion deco / reco esclave 1.11
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "ex_mrtf.h"

#include "eimi_cfg.h"
#include "eimi_pro.h"
#include "eimi_mdon.h"
#include "ex_mdia.h"


/* definitions de constantes */

#define CM_VAL_DEFAUT_TIMER          4.0

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
int vg_RepTEzzz;
static char *version = "$Id: eopc_pro.c,v 1.1 2021/05/04 13:45:33 pc2dpdy Exp $      : eimi_pro" ;

/* declaration des variables externes */
extern int vg_presence_esclave;
extern XZEXT_IP_Config vg_ConfigE;

/* definition de fonctions internes */

void epro_envoi_trame_LCR_IP( XZEXT_Cmde_LCR_IP * , XDY_Booleen );
void epro_envoi_trame_DIASER( XZEXT_Cmde_DIASER * );
int  epro_traitement_msg_lser( XZEXT_Cmde_LCR *, XZEXT_Msg_Socket *, 
				EPROT_Conditions *, XZEXT_Msg_TEDI * );
void epro_traitement_msg_donn( XZEXT_Msg_Socket * );
void epro_traitement_msg_ip_lcr_e( XZEXT_Msg_Socket *, XDY_Booleen );
void epro_armer_timeout(char * );
/* declaration de fonctions externes */
extern void edir_traitement_dcnx ();
extern int edir_detecte_type_sock( void );
char vm_tampon[1000]="";
extern T_Tampon        tg_tampons[C_MAX_TAMPON];

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi d'une trame TEDI/LCR suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_LCR_IP(  XZEXT_Cmde_LCR_IP *pa_CmdeLCR , XDY_Booleen va_insert
)
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a envoyer.
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
* - envoi trame LCR et attente reponse
* - empilement des eventuelles demandes des clients TEzzz ds. une liste lors
*   de l'attente de la reponse au mesage TEDI/LCR
*
------------------------------------------------------*/
void epro_envoi_trame_LCR_IP(  XZEXT_Cmde_LCR_IP *pa_CmdeLCR , XDY_Booleen va_insert)
{
    static char *version = "$Id: eopc_pro.c,v 1.1 2021/05/04 13:45:33 pc2dpdy Exp $      : epro_envoi_trame_LCR " ;
    
    XZEXT_Cmde_LCR 	  vl_CmdeLCR;	/* structure depouillement messages a empiler */
    XZEXT_Comp_Trame_TEDI vl_SkelTEDI;	/* composants trame TEDI pour EX_encapsuler_LCR */
    XZEXT_Msg_TEDI	  vl_MsgTEDI;	/* contient le msg. TEDI a envoyer ET sa long. */
    XDY_Texte		  vl_TypeMsg;	/* juste pour recuperer le type de message */
    EPROT_Conditions	  vl_Cond;	/* conditions d'envoi trame : 	nbre. essais,
    									nbre. erreurs,
    									nbre. non reponses,
    					*/
    XZEXT_Msg_Socket	vl_MsgSock;	/* message lu localement */
    int			vl_EtatCnx;	/* etat conexion socket active apres lecture */
    int			vl_AttenteRep;	/* XDC_VRAI s'il faut attendre la rep. de l'equip. */	
    XZEXT_MASK		vl_ElmSockList;	/* pour l'insertion sock. RTServer ds. la liste */
    XZEXT_MASK		*pl_ListSock = NULL;
    char		*pl_TrameLisible;	/* pointeur sur trame format lisible */
    XZSMT_Horodate	vl_Horodate;	/* horodate en format lisible */
    XDY_Horodate	vl_HorodateSec;
    XZSMT_Horodate      vl_Horodatedecompo;
    int			vl_JourSem;	/* jour de la semaine */
    int 		vl_retour=0;
	XZEXT_IP_Config vl_config;

    vl_AttenteRep = XDC_VRAI;
     
    /*A mise a jour composants trame TEDI */
    /*A mise a jour type message TEDI */
    vl_SkelTEDI.type_msg = pa_CmdeLCR->type_msg;	/* type msg. TEDI associe a la cmde. LCR */
   
 /*recherche de la bonne socket avec la bonne adresse IP*/
        for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
        {
          if ((!strcmp(pl_ListSock->adresse, pa_CmdeLCR->adresseIP)) &&
	     (pl_ListSock->Port == pa_CmdeLCR->port)){
                XZST_03EcritureTrace( XZSTC_DEBUG1,"socket trouvee %s %d",pa_CmdeLCR->adresseIP,
						pa_CmdeLCR->port);
                break;
          }
        }

    if (pl_ListSock==NULL) {
      XZST_03EcritureTrace( XZSTC_WARNING,"pas de socket ouverte sur cette adresse %s",
                                pa_CmdeLCR->adresseIP);
      /*on essaie d ouvrir a nouveau cette socket*/
        /*cas par exemple ou l eqt redevient operationnel*/
        vl_config.No_port = pa_CmdeLCR->port;
        strcpy(vl_config.adresseIP, pa_CmdeLCR->adresseIP);
	strcpy(vl_config.Nom_machine, vg_Config.Nom_machine);
        vl_config.protocole = XZECC_PROT_LCR;

	if (vl_config.No_port==vg_ConfigE.No_port)
	                vl_config.protocole = XZECC_PROT_LCR_E;

        if (ecom_init_IP(&vl_config,
                                &vg_SockList,
                                &vg_DesSocks) != XDC_OK)
        {
	  sprintf( vl_MsgSock.msg, "rep. teimi %s %d %d %d %s %d %d %s %s %s",
                                ecom_lire_horodate( &vl_Horodate),
                                vl_Cond.nb_non_rep,
                                vl_Cond.nb_err_crc,
                                pa_CmdeLCR->no_cmd,
                                pa_CmdeLCR->adresseIP,
                                pa_CmdeLCR->type_cmd,
                                XZEXC_TRAME_NON_TRANSM,
                                XZEXC_SEP_TRAME,
                                pa_CmdeLCR->cmd_LCR,
                                XZEXC_FIN_MSG_SOCK
                );
    vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );

    /*A emission reponse vers la tache TEzzz client */
    /*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
    sem_wait(&vg_semaphore); 
    if( write( pa_CmdeLCR->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "epro_envoi_trame_LCR : Emission message <%s> vers client TEzzz impossible.",
                                 vl_MsgSock.msg );
    }
    sem_post(&vg_semaphore); 
    return;
       } 

    /* on force la reconnexion esclave*/
    if ((vg_presence_esclave==XDC_VRAI) && (vl_config.No_port!=vg_ConfigE.No_port))
       {
        XZST_03EcritureTrace( XZSTC_WARNING, "reco esclave");
        ecom_init_IP(&vg_ConfigE, &vg_SockList, &vg_DesSocks);
        }
    }

    /*recherche de la bonne socket avec la bonne adresse IP*/
    for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
    {
          if ((!strcmp(pl_ListSock->adresse, pa_CmdeLCR->adresseIP)) &&
	     (pl_ListSock->Port == pa_CmdeLCR->port)){
                  XZST_03EcritureTrace( XZSTC_DEBUG1,"socket trouvee %s",pa_CmdeLCR->adresseIP);
           break;
        }
    }
    if (pl_ListSock==NULL) {
	  sprintf( vl_MsgSock.msg, "rep. teimi %s %d %d %d %s %d %d %s %s %s",
                                ecom_lire_horodate( &vl_Horodate),
                                vl_Cond.nb_non_rep,
                                vl_Cond.nb_err_crc,
                                pa_CmdeLCR->no_cmd,
                                pa_CmdeLCR->adresseIP,
                                pa_CmdeLCR->type_cmd,
                                XZEXC_TRAME_NON_TRANSM,
                                XZEXC_SEP_TRAME,
                                pa_CmdeLCR->cmd_LCR,
                                XZEXC_FIN_MSG_SOCK
                );
    vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );

    /*A emission reponse vers la tache TEzzz client */
    /*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
    sem_wait(&vg_semaphore); 
    if( write( pa_CmdeLCR->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "epro_envoi_trame_LCR : Emission message <%s> vers client TEzzz impossible.",
                                 vl_MsgSock.msg );
    }
    sem_post(&vg_semaphore); 
    return;
    }

    /*A mise a jour adresse rgsb du message en fonction de son type */
    /*A CAS type message */
    XZST_03EcritureTrace( XZSTC_WARNING,"vl_SkelTEDI.type_msg %d",vl_SkelTEDI.type_msg);
    switch( vl_SkelTEDI.type_msg )
    {
    /*A cas XZEXC_MSG_REQ */
    case XZEXC_MSG_REQ :
    	/* Cas particulier de la date */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "XZEXC_MSG_REQ"  );
        /* Envoi de la date sous format normal DT ID= JJ/MM/AA HH:MM:SS */
    	if ( pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE )
    	{
            XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
            /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
            XZSM_11ConversionHorodate ( vl_HorodateSec, &vl_Horodatedecompo );
            /* passage de l'annee sur 2 digit */
            memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
              
    	    /*B Composition de la trame mise a la date et a l'heure */
  	    /*sprintf ( pa_CmdeLCR->cmd_LCR, "DT ID=%s %s", vg_idpassw, vl_Horodatedecompo );*/
  	    sprintf ( pa_CmdeLCR->cmd_LCR, "DT %s", vl_Horodatedecompo );
  	    XZST_03EcritureTrace( XZSTC_DEBUG2, "trame de date:<%s0 %s>", pa_CmdeLCR->rgs, pa_CmdeLCR->cmd_LCR  );
    	}

        /* Envoi de la date sous format  DT ou DATE JJ/MM/AA HH:MM:SS */
    	if ( pa_CmdeLCR->type_cmd == XZEXC_TRAME_DATE_ID_SUITE )
  	{
            XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
            /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
            XZSM_11ConversionHorodate ( vl_HorodateSec, &vl_Horodatedecompo );
            /* passage de l'annee sur 2 digit */
            memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
    	    /*B Composition de la trame mise a la date et a l'heure */
  	    sprintf ( pa_CmdeLCR->cmd_LCR, "%s %s", pa_CmdeLCR->cmd_LCR, vl_Horodatedecompo );
  	    XZST_03EcritureTrace( XZSTC_DEBUG2, "trame de date:<%s0 %s>", pa_CmdeLCR->rgs, pa_CmdeLCR->cmd_LCR  );
     	}
    	
    	/*B mise a jour adresse rgsb et trame_LCR ds. vl_SkelTEDI */
    	sprintf( vl_SkelTEDI.adr_RGSB, "%s%d", pa_CmdeLCR->rgs, XZEXC_NOBL_REQ_TEDI );
    	strcpy( vl_SkelTEDI.trame_LCR, pa_CmdeLCR->cmd_LCR );
    	break;
    
    /*A cas XZEXC_MSG_INFO ou XZEXC_BLC_INFO */
    case XZEXC_MSG_INFO :
    case XZEXC_BLC_INFO :
        XZST_03EcritureTrace( XZSTC_DEBUG1, "XZEXC_MSG_INFO"  );
    	/*B mise a jour adresse rgsb et trame_LCR ds. vl_SkelTEDI */
    	strcpy( vl_SkelTEDI.trame_LCR, pa_CmdeLCR->cmd_LCR );
    	sprintf( vl_SkelTEDI.adr_RGSB, "%s%s", pa_CmdeLCR->rgs, pa_CmdeLCR->ack );
    	break;
    
    /*A cas XZEXC_MSG_ACQUIT */
    default :
     	/*B mise a jour adresse rgsb ds. vl_SkelTEDI */
        XZST_03EcritureTrace( XZSTC_FONCTION, "XZEXC_MSG_ACQUIT (%s)" , pa_CmdeLCR->ack );
   	sprintf( vl_SkelTEDI.adr_RGSB, "%s", pa_CmdeLCR->ack );
    	break;
    }
     /* FIN CAS */

    /*A mise a jour nbre. cars. pref. & suff. */    
    vl_SkelTEDI.long_pref = vg_Config.ligne.NbCarPref;
    vl_SkelTEDI.long_suff = vg_Config.ligne.NbCarSuff;
    
    /*A mise a jour conds. essais envoi trame TEDI/LCR */
    if ( (pa_CmdeLCR->type_cmd == XZEXC_TRAME_TEST_COM) || (pa_CmdeLCR->type_cmd == XZEXC_TRAME_FEN_LCR) )
       vl_Cond.nb_essais = 1;
    else
       vl_Cond.nb_essais = vg_Config.ligne.NbEssais;
    vl_Cond.nb_err_crc 	= 0;
    vl_Cond.nb_non_rep 	= 0;
    
    /*A encapsulation msg. TEDI/LCR -> appel EX_encapsuler_LCR() */
    /*if ( pa_CmdeLCR->type_cmd == XZEXC_TRAME_FEN_LCR )
    {
       for ( pl_ListSock = vg_SockList; pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
    	  if ( pl_ListSock->desc_sock == pa_CmdeLCR->sock_clnt ) 
    	  {
    	     if ( strncmp ( pl_ListSock->adresse, XZEXC_ADR_SOCK_GTCTELM, strlen(XZEXC_ADR_SOCK_GTCTELM)) )
                vl_SkelTEDI.trame_LCR[strlen(vl_SkelTEDI.trame_LCR)-1] = '\0';
    	     break;
    	  }
    }*/
    /*A SI encapsulation echoue ALORS ecriture trace :"Probleme encapsulation msg. TEDI..." et RETOUR */
    if( EX_encapsuler_LCR_IP( &vl_SkelTEDI, &vl_MsgTEDI ) != XDC_OK )
    {
    	XZST_03EcritureTrace( XZSTC_WARNING, "Probleme encapsulation msg. %s...", vl_SkelTEDI.trame_LCR );
    	return;
    }
    
    /*A
    * sequence envoi message TEDI/LCR sur la ligne serie geree 
    */
        /*B SI emission echoue ALORS ecriture trace : "Emission message TEDI/LCR vers serv. terms. impossible "*/
        sem_wait(&vg_semaphore); 
        if( write( pl_ListSock->desc_sock, vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len ) != vl_MsgTEDI.len )
        {
            if( (pl_TrameLisible = malloc( 4 * (size_t) vl_MsgTEDI.len + 1 )) )
            {
 	    	XZST_03EcritureTrace( XZSTC_INFO, "epro_envoi_trame_LCR : Emission message IP TEDI/LCR <%s> vers eqt impossible ",
 	    		ecom_rendre_trame_lisible( vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible ) );
 	    	free( pl_TrameLisible );
 	    }
         sem_post(&vg_semaphore); 
	}
	else
	{
            if( (pl_TrameLisible = malloc( 4 * (size_t) vl_MsgTEDI.len + 1 )) )
            {
 	    	XZST_03EcritureTrace( XZSTC_INFO, "Emission message IP %s (%d) TEDI/LCR <%s> vers eqt.",pa_CmdeLCR->rgs,vl_Cond.nb_essais,
 	    		ecom_rendre_trame_lisible( vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible ) );
 	    	free( pl_TrameLisible );
 	    }
		if (va_insert)
		ecom_insert_cmd_LCR_IP(pa_CmdeLCR,&pg_ListeCmdesLCR);
		/*epro_armer_timeout(pa_CmdeLCR->adresseIP);*/
		XZST_03EcritureTrace( XZSTC_WARNING, "avant return");
		return;
	}
    	
    /*A Nbre. d'essai epuise => envoi reponse CMDE. LCR NON TRANSMISE a la tache cliente TEzzz */
    /*A formatage reponse */
    sprintf( vl_MsgSock.msg, "rep. teimi %s %d %d %d %s %d %d %s %s %s",
				ecom_lire_horodate( &vl_Horodate),
				vl_Cond.nb_non_rep,
    	    			vl_Cond.nb_err_crc,
    	    			pa_CmdeLCR->no_cmd,
    	    			pa_CmdeLCR->rgs,
    	    			pa_CmdeLCR->type_cmd,
    	    			XZEXC_TRAME_NON_TRANSM,
    	    			XZEXC_SEP_TRAME,
    	    			pa_CmdeLCR->cmd_LCR,
    	    			XZEXC_FIN_MSG_SOCK
    	    	);
    vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );

    /*A emission reponse vers la tache TEzzz client */
    /*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
    sem_wait(&vg_semaphore); 
    if( write( pa_CmdeLCR->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
 	XZST_03EcritureTrace( XZSTC_WARNING, "epro_envoi_trame_LCR : Emission message <%s> vers client TEzzz impossible.",
 				 vl_MsgSock.msg );
    }
    sem_post(&vg_semaphore); 
    /*B FIN SI */
    
}


int epro_traitement_msg_ip_diaser(XZEXT_Msg_Socket *pa_MsgSock)
{
int vl_TypeRep;
XZEXT_MASK          *pl_ListSock = NULL;
XZEXT_Msg_Socket vl_MsgSock;
XZSMT_Horodate      vl_Horodate;    /* horodate en format converti */

	/*recuperer l eqt pour lequel on a passe la commande*/
	for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
	{
		if (pl_ListSock->desc_sock==vg_DesSocks.curr)
		  break;
	}

	/*suivant la trame recue depuis un equipement*/
  	if (strstr(pa_MsgSock->msg, "D?") ){
		XZST_03EcritureTrace( XZSTC_FONCTION,"commande en echec");
		
		/*preparer la reponse a TExxx*/
      		sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
    				XZEXC_REP_TELMi,
				ecom_lire_horodate( &vl_Horodate ),
				0 /*pa_Cond->nb_non_rep*/,
    	    			0 /*pa_Cond->nb_err_crc*/,
    	    			1 /*pa_CmdeLCR->no_cmd*/,
    	    			pl_ListSock->adresse,
    	    			XZEXC_TRAME_ETAT_POS_CFE /*XZEXC_TRAME_STAT_TR*/,
    	    			XZEXC_TRAME_NON_TRANSM,
    	    			XZEXC_SEP_TRAME,
    	    			pa_MsgSock->msg,
    	    			XZEXC_FIN_MSG_SOCK );
	 }
	else if (strstr(pa_MsgSock->msg, "DIA")) {
		/*reponse a une demande DI*/
		XZST_03EcritureTrace( XZSTC_FONCTION,"commande en echec");
		
		/*preparer la reponse a TExxx*/
      		sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
    				XZEXC_REP_TELMi,
				ecom_lire_horodate( &vl_Horodate ),
				0 /*pa_Cond->nb_non_rep*/,
    	    			0 /*pa_Cond->nb_err_crc*/,
    	    			1 /*pa_CmdeLCR->no_cmd*/,
    	    			pl_ListSock->adresse,
    	    			XZEXC_TRAME_ETAT_POS_CFE,
    	    			XZEXC_TRAME_NON_TRANSM,
    	    			XZEXC_SEP_TRAME,
    	    			pa_MsgSock->msg,
				XZEXC_FIN_MSG_SOCK );
	}
	else if (strstr(pa_MsgSock->msg, "DI")) {
		/*preparer la reponse a TExxx*/
		XZST_03EcritureTrace( XZSTC_FONCTION,"commande ok");
      		sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
    				XZEXC_REP_TELMi,
				ecom_lire_horodate( &vl_Horodate ),
				0 /*pa_Cond->nb_non_rep*/,
    	    			0 /*pa_Cond->nb_err_crc*/,
    	    			1 /*pa_CmdeLCR->no_cmd*/,
    	    			pl_ListSock->adresse,
    	    			XZEXC_TRAME_ETAT_POS_CFE,
    	    			XZEXC_REP_BLC_FIN,
    	    			XZEXC_SEP_TRAME,
    	    			pa_MsgSock->msg,
				XZEXC_FIN_MSG_SOCK );
	}
	else if (strstr(pa_MsgSock->msg, "*!")) {
		/*reponse a une demande DATE ok$/
		XZST_03EcritureTrace( XZSTC_FONCTION,"reponse date ok");
		return (XDC_OK);
	}
	else if (strstr(pa_MsgSock->msg, "$?")) {
		/*reponse a une demande DATE non comprise*/

		XZST_03EcritureTrace( XZSTC_FONCTION,"reponse date non comprise");
		return (XDC_OK);
	}
    vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );
    	    
    /*B emission reponse vers la tache TEzzz client */
    /*B SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
    
    XZST_03EcritureTrace( XZSTC_FONCTION, "Emission message <%s> \n lg:%d.",vl_MsgSock.msg,vl_MsgSock.len);
	for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
    {
		XZST_03EcritureTrace( XZSTC_DEBUG1,"socket %d %s",pl_ListSock->desc_sock, pl_ListSock->adresse);
    }
    
	for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
    {
	if ((pl_ListSock->Protocole == XZECC_PROT_MIGRAZUR) && (pl_ListSock->desc_sock!=vg_DesSocks.rtsrv)) {
		XZST_03EcritureTrace( XZSTC_DEBUG1,"socket %d %s",pl_ListSock->desc_sock, pl_ListSock->adresse);
		break;
	}
    }
    sem_wait(&vg_semaphore); 
    if( write( pl_ListSock->desc_sock, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
 	XZST_03EcritureTrace( XZSTC_WARNING, "Emission message <%s> vers client TEzzz impossible.",
 		 vl_MsgSock.msg );
    }
    sem_post(&vg_semaphore); 
 	return( XDC_OK );

}



#if 0

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement d'un message recu sur une sock. comms. avec un client TEzzz.
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
* pa_MsgSock doit contenir un message de la part d'un client TEzzz.
*
* FONCTION 
* - si demande arret appel ecfg_sortir().
* - si demande init appel ecom_init().
* - si demande LCR :
*  	depouillement du message recu du client TEzzz ds. une struct type XZEXT_Cmde_LCR.
*  	empilement de cette meme demande ds. la liste des demandes a traiter.
*
------------------------------------------------------*/
void epro_traitement_msg_donn( XZEXT_Msg_Socket *pa_MsgSock )
{
    static char *version = "$Id: eopc_pro.c,v 1.1 2021/05/04 13:45:33 pc2dpdy Exp $      : epro_traitement_msg_donn";

    XDY_Texte		vl_TypeMsg;	/* juste pour recuperer le type de message */
    XZEXT_Cmde_LCR	vl_CmdeLCR;	/* cmde LCR a empiler ds. la liste */
    XDY_Texte		vl_Texte;
    int			vl_JourSem;	/* jour de la semaine */
    int			vl_ValRet;

    /*A SI demande d'arret ALORS appel ecfg_sortir() */
    if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
    {
    	XZST_03EcritureTrace( XZSTC_WARNING, "epro_traitement_msg_donn : demande  arret TELMi %d",
    				vg_Config.no_ligne );
    	ecfg_sortir();
    }
    /*A SINON SI demande init. ALORS appel ecom_init() */
    else 
    {
       if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_INIT ) != NULL )
       {
    	  XZST_03EcritureTrace( XZSTC_WARNING, "epro_traitement_msg_donn : demande  reinitialisation TELMi %d",
    				vg_Config.no_ligne );

	  /*A SI init echoue pour cause de pb. ST ET alarme non envoyee */
	  if( (vg_EtatST = ecom_init_IP( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
	  {
    	      XZST_03EcritureTrace( XZSTC_WARNING, "epro_traitement_msg_donn : Echec relance initialisation TELMi %d",
    	    				vg_Config.no_ligne );
    	  }
       }
       /*A SINON SI demande envoi trame LCR sur ligne serie geree ALORS */
       else 
       {
          if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_LCR ) != NULL )
          {
    	     /*B SI extraction cmde. LCR ecom_extraire_cmd_LCR() OK */
	     if( ecom_extraire_cmd_LCR( pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr ) == XDC_OK )
	     {
	        /*B ALORS insertion cmde LCR en liste d'attente ecom_insert_cmd_LCR() */
	        ecom_insert_cmd_LCR( &vl_CmdeLCR, &pg_ListeCmdesLCR );
	     }
          }
       }
    }
    	 
}

#endif

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi d'une trame TEDI/LCR suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_DIASER(  XZEXT_Cmde_DIASER *pa_Cmde )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a envoyer.
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
* - envoi trame LCR et attente reponse
* - empilement des eventuelles demandes des clients TEzzz ds. une liste lors
*   de l'attente de la reponse au mesage TEDI/LCR
*
------------------------------------------------------*/
void epro_envoi_trame_DIASER(  XZEXT_Cmde_DIASER *pa_Cmde )
{
    static char *version = "$Id: eopc_pro.c,v 1.1 2021/05/04 13:45:33 pc2dpdy Exp $      : epro_envoi_trame_LCR " ;
    
    XZEXT_Cmde_LCR 	  vl_CmdeLCR;	/* structure depouillement messages a empiler */
    XZEXT_Comp_Trame_TEDI vl_SkelTEDI;	/* composants trame TEDI pour EX_encapsuler_LCR */
    XZEXT_Msg_TEDI	  vl_MsgTEDI;	/* contient le msg. TEDI a envoyer ET sa long. */
    XDY_Texte		  vl_TypeMsg;	/* juste pour recuperer le type de message */
    EPROT_Conditions	  vl_Cond;	/* conditions d'envoi trame : 	nbre. essais,
    									nbre. erreurs,
    									nbre. non reponses,
    					*/
    XZEXT_Msg_Socket	vl_MsgSock;	/* message lu localement */
    int			vl_EtatCnx;	/* etat conexion socket active apres lecture */
    int			vl_AttenteRep;	/* XDC_VRAI s'il faut attendre la rep. de l'equip. */	
    XZEXT_MASK		vl_ElmSockList;	/* pour l'insertion sock. RTServer ds. la liste */
    XZEXT_MASK		*pl_ListSock = NULL;
    char		*pl_TrameLisible;	/* pointeur sur trame format lisible */
    XZSMT_Horodate	vl_Horodate;	/* horodate en format lisible */
    XDY_Horodate	vl_HorodateSec;
    XZSMT_Horodate      vl_Horodatedecompo;
    int			vl_JourSem;	/* jour de la semaine */
    int 		vl_retour=0;
	XZEXT_IP_Config	vl_config;

    vl_AttenteRep = XDC_VRAI;
     
    /*A mise a jour composants trame TEDI */
    /*A mise a jour type message TEDI */
    vl_SkelTEDI.type_msg = pa_Cmde->type_cmd;	/* type msg. TEDI associe a la cmde. LCR */
   strcpy( vl_SkelTEDI.trame_LCR, pa_Cmde->cmd );

    
    sprintf(vl_MsgSock.msg,"%c%s%c",
			XZEXC_CAR_STX, pa_Cmde->cmd, XZEXC_CAR_ETX);
    strcpy(vl_MsgTEDI.trame_TEDI,vl_MsgSock.msg);
    /*sprintf(vl_MsgTEDI.trame_TEDI,"%s%c",vl_MsgSock.msg,EX_calcul_CRC(vl_MsgSock.msg));*/
    vl_MsgTEDI.len = strlen(vl_MsgTEDI.trame_TEDI);
    vl_MsgTEDI.trame_TEDI[vl_MsgTEDI.len] = EX_calcul_CRC_DIASER( vl_MsgSock.msg );

    XZST_03EcritureTrace( XZSTC_FONCTION,"CRC calcule %d",EX_calcul_CRC(vl_MsgSock.msg));
    vl_MsgTEDI.len++;

    /*A
    * sequence envoi message TEDI/LCR sur la ligne serie geree 
    */
    
	/*recherche de la bonne socket avec la bonne adresse IP*/
	for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant ) 
	{
	  if (!strcmp(pl_ListSock->adresse, pa_Cmde->adresseIP)) {
		XZST_03EcritureTrace( XZSTC_DEBUG1,"socket trouvee %s",pa_Cmde->adresseIP);
		break;
	  }
	}
	if (pl_ListSock!=NULL) 
	{
        sem_wait(&vg_semaphore); 
        if( write( pl_ListSock->desc_sock, vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len ) != vl_MsgTEDI.len )
        {
            sem_post(&vg_semaphore); 
            if( (pl_TrameLisible = malloc( 4 * (size_t) vl_MsgTEDI.len + 1 )) )
            {
                XZST_03EcritureTrace( XZSTC_INFO, "epro_envoi_trame_LCR : Emission message IP <%s> vers eqt impossible ",
                    ecom_rendre_trame_lisible( vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible ) );
                free( pl_TrameLisible );
 	        }
	    }
	    else
	    {
            sem_post(&vg_semaphore); 
            if( (pl_TrameLisible = malloc( 4 * (size_t) vl_MsgTEDI.len + 1 )) )
            {
                XZST_03EcritureTrace( XZSTC_INFO, "Emission message  IP <%s> vers eqt ",
                    ecom_rendre_trame_lisible( vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible ) );
                free( pl_TrameLisible );
    	    }
    		return;
    	}
    }
    else {
	XZST_03EcritureTrace( XZSTC_WARNING,"pas de socket ouverte sur cette adresse %s",
				pa_Cmde->adresseIP);

	/*on essaie d ouvrir a nouveau cette socket*/
	/*cas par exemple ou l eqt redevient operationnel*/
	vl_config.No_port = pa_Cmde->port;
	strcpy(vl_config.adresseIP, pa_Cmde->adresseIP);
	vl_config.protocole = XZECC_PROT_DIASER;
	if (ecom_init_IP(&vl_config,
				&vg_SockList,
				&vg_DesSocks) == XDC_OK) 
    	{
		/*si ok*/
		/*on envoie la commande*/
		/*recherche de la bonne socket avec la bonne adresse IP*/
		for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant ) 
		{
	  		if (!strcmp(pl_ListSock->adresse, pa_Cmde->adresseIP)) {
				XZST_03EcritureTrace( XZSTC_DEBUG1,"socket trouvee %s",pa_Cmde->adresseIP);
				break;
	  		}
		}
		if (pl_ListSock!=NULL) 
		{
            sem_wait(&vg_semaphore); 
            if( write( pl_ListSock->desc_sock, vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len ) != vl_MsgTEDI.len )
            {
                sem_post(&vg_semaphore); 
                if( (pl_TrameLisible = malloc( 4 * (size_t) vl_MsgTEDI.len + 1 )) )
                {
                    XZST_03EcritureTrace( XZSTC_INFO, "epro_envoi_trame_LCR : Emission message IP <%s> vers eqt impossible ",
                        ecom_rendre_trame_lisible( vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible ) );
                    free( pl_TrameLisible );
                }
           }
            else
            {
                sem_post(&vg_semaphore); 
                if( (pl_TrameLisible = malloc( 4 * (size_t) vl_MsgTEDI.len + 1 )) )
                {
                    XZST_03EcritureTrace( XZSTC_INFO, "Emission message  IP <%s> vers eqt ",
                        ecom_rendre_trame_lisible( vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible ) );
                    free( pl_TrameLisible );
                }
 				return;
			}
    		}
    	}

   }

    /*A formatage reponse */
    sprintf( vl_MsgSock.msg, "rep. teimi %s %d %d %d %s %d %d %s %s %s",
				ecom_lire_horodate( &vl_Horodate),
				vl_Cond.nb_non_rep,
    	    			vl_Cond.nb_err_crc,
    	    			pa_Cmde->no_cmd,
    	    			pa_Cmde->adresseIP,
    	    			pa_Cmde->type_cmd,
    	    			XZEXC_TRAME_NON_TRANSM,
    	    			XZEXC_SEP_TRAME,
    	    			pa_Cmde->cmd,
    	    			XZEXC_FIN_MSG_SOCK
    	    	);
    vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );

    /*A emission reponse vers la tache TEzzz client */
    /*A SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
    sem_wait(&vg_semaphore); 
    if( write( pa_Cmde->sock_clnt, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
 	XZST_03EcritureTrace( XZSTC_WARNING, "epro_envoi_trame_LCR : Emission message <%s> vers client TEzzz impossible.",
 				 vl_MsgSock.msg );
    }
    sem_post(&vg_semaphore); 
    /*B FIN SI */
    
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* Traitement des messages equipements maitres.
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  void epro_traitement_msg_ip_lcr_e( XZEXT_Msg_Socket *pa_MsgSock , XDY_Booleen va_esclave)
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock    : message recu sur la socket a verifier.
*
* ARGUMENTS EN SORTIE :
*
* pa_MsgSock    : pa_MsgSock->msg contient le trame LCR si le message est correcte TEDI.
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* pa_MsgSock    : doit contenir le message recu sur la socket ligne serie.
*
* FONCTION
*
* - verification du message contnu ds. pa_MsgSock->msg.
* - seule une REQUETTE ( [ENQ]rgs0 ... [ETX][BCC] ) est reconnue comme message correcte
*   d'un point de vue TEDI.
* - toute autre trame est une erreur.
*
------------------------------------------------------*/
void epro_traitement_msg_ip_lcr_e( XZEXT_Msg_Socket *pa_MsgSock , XDY_Booleen va_esclave)
{
    int         vl_ResVerif;            /* resultat verif. de la trame TEDI/LCR envoyee par l'equip. */
    char        vl_DDM[5];              /* DDM a verifier en debut du message recu */
    char        vl_NoBloc;              /* no. bloc du message recu */
    char        *pl_TrameLisible;       /* pointeur sur la trame en format lisible */
    XZEXT_MASK  *pl_ElmSockList;        /* pointeur sur un elm. de la liste des sockets */
    XDY_Texte   vl_Texte;               /* texte trace */
    XDY_Booleen vl_EnvoiMsg = (XDY_Booleen)XDC_FAUX;    /* teste pour l'envoi d'un message vers tache traitement */
    XZEXT_Cmde_LCR_IP vl_CmdeLCR;
	int vl_TypeRep;	
    XDY_AdresseIP	vl_adresse;
    XDY_PortIP	vl_port;
    XZEXT_Msg_Socket    vl_MsgSock;     /* message a envoyer a la tache TEzzz */
    XZSMT_Horodate      vl_Horodate;    /* horodate en format converti */
    static int  vl_CmptErrCRC = 0;      /* compteur erreurs CRC */
 XZEXT_MASK          *pl_ListSock = NULL;
	XZSMT_CleProg vl_cle;
  	XDY_Octet vl_type_trame;
	int		vl_no_cmd;
	int vl_indice;
    vl_Texte[0] = '\0';

   /*recuperer l eqt pour lequel on a passe la commande*/
	   for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
	   {
		 if (pl_ListSock->desc_sock==vg_DesSocks.curr)
	     break;
	    }

	strcpy(vl_adresse,pl_ListSock->adresse);
	vl_port = pl_ListSock->Port;
	vl_no_cmd= XZEXC_NOCMD;
	if (va_esclave==XDC_FAUX) {
		vl_type_trame = XZEXC_TRAME_STAT_COMP;
		if (ecom_existe_cmd_LCR_IP(pl_ListSock->adresse, pl_ListSock->Port, pg_ListeCmdesLCR, &vl_CmdeLCR)==XDC_OK) {
			XZST_03EcritureTrace( XZSTC_INFO,"type trame recuperee %d cmd %d",vl_CmdeLCR.type_cmd,
						vl_CmdeLCR.no_cmd);
			vl_type_trame = vl_CmdeLCR.type_cmd;
			vl_no_cmd = vl_CmdeLCR.no_cmd;
    			/*sprintf(vl_cle, "TEMIP_%s",pl_ListSock->adresse);
    			XZSM_02AnnulerProgrammation(vl_cle, XDG_ETEMIP,1);*/
		}
	}
    /*A mise a jour du DDM a verifier avec l'adresse rgs de SAE et le car. debut message */
      strcpy(vl_DDM,"");
      if (va_esclave==XDC_VRAI) {
      /*sprintf( vl_DDM, "%c000", XZEXC_CAR_ENQ );*/
      sprintf( vl_DDM, "%c%sx", XZEXC_CAR_ENQ, XZEXC_RGS_SAE );
      sprintf( vl_DDM, "%c%c%c%c", XZEXC_CAR_ENQ,pa_MsgSock->msg[1],
      		pa_MsgSock->msg[2], pa_MsgSock->msg[3]);
	XZST_03EcritureTrace( XZSTC_INFO,"DDM esclave %s",vl_DDM);	
      }
      else {
      sprintf( vl_DDM, "%c%c%c%c", XZEXC_CAR_STX,pa_MsgSock->msg[1],
      		pa_MsgSock->msg[2], pa_MsgSock->msg[3]);
	XZST_03EcritureTrace( XZSTC_INFO,"DDM maitre %s",vl_DDM);	
      }

    /*A on s'attend aussi qu'on acquitte avec no. bloc =  XZEXC_CAR_NO_BLC_REQ */
    vl_NoBloc = XZEXC_CAR_NO_BLC_REQ;

    /*A CAS resultat de la verification de la trame recue */
    vl_ResVerif = ex_verif_TEDI_CRC( pa_MsgSock, vl_DDM, &vl_NoBloc ) ;
    XZST_03EcritureTrace( XZSTC_INFO,"vl_ResVerif %d",vl_ResVerif);
    switch( vl_ResVerif  )
    {
    /*A cas acquittement positif OK */
    case XZEXC_ACK_OK :
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "acquit positif OK" );
    /*A cas message bloc unique OK */
    case XZEXC_MSG_OK :
        /*B le cas correcte - TELEi recoit seulement des DEMANDES TEDI/LCR */

		for (vl_indice=0;vl_indice<C_MAX_TAMPON;vl_indice++) {
			if ( (tg_tampons[vl_indice].port==vl_port) &&
			  !strcmp(tg_tampons[vl_indice].adresse,vl_adresse))
				break;
      		}
	strcat(tg_tampons[vl_indice].tampon,pa_MsgSock->msg);
	strcat(vm_tampon,pa_MsgSock->msg);
	strcpy(vm_tampon,tg_tampons[vl_indice].tampon);
        /*B formatage message reponse pour la tache TEzzz client */
        sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
                                XZEXC_REP_TELEi,
                                ecom_lire_horodate( &vl_Horodate ),
                                0,
                                0,
                                vl_no_cmd,
                                pl_ListSock->adresse,
                                va_esclave ? XZEXC_TRAME_TELEI_OK : vl_type_trame ,
                                XZEXC_REP_BLC_FIN,
                                XZEXC_SEP_TRAME,
                                vm_tampon /*pa_MsgSock->msg*/,
                                XZEXC_FIN_MSG_SOCK
                        );
	strcpy(vm_tampon,"");
	strcpy(tg_tampons[vl_indice].tampon,"");
        vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );
        /*B OK pour l'envoi du message */
        vl_EnvoiMsg = (XDY_Booleen)XDC_VRAI;
        /*B OK pour attente reponse tache traitement */
        vg_RepTEzzz = XDC_NOK;
        /* trace message envoye */
        XZST_03EcritureTrace( XZSTC_INFO, "telei ligne %d : msg. recu sur ligne serie\nenvoi msg vers TEzzz %s",
                vg_Config.no_ligne, vl_MsgSock.msg );

        break;

    /*A TOUS LES AUTRES CAS SONT INCORRECTES, traitement commun des cas */
    /*A cas bloc. interm. OK */
    case XZEXC_BLC_OK :
	if (va_esclave==XDC_VRAI) {
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "bloc intermed OK" );
      }
	else {
		sprintf( vl_Texte, "detection bloc intermed. %s %c OK, bloc attendu %c", vl_CmdeLCR.rgs,vl_NoBloc, vl_CmdeLCR.no_blc_suivant );
		        XZST_03EcritureTrace( XZSTC_INFO, "epro_traitement_msg_lser : %s %s\n",
			                                vl_Texte, pa_MsgSock->msg );
		for (vl_indice=0;vl_indice<C_MAX_TAMPON;vl_indice++) {
			if ( (tg_tampons[vl_indice].port==vl_port) &&
			  !strcmp(tg_tampons[vl_indice].adresse,vl_adresse))
				break;
      		}
		sprintf(tg_tampons[vl_indice].tampon, pa_MsgSock->msg);
		sprintf(vm_tampon, pa_MsgSock->msg);
		vl_CmdeLCR.no_blc_suivant = ( vl_CmdeLCR.no_blc_suivant == '9' ? '0' : vl_CmdeLCR.no_blc_suivant + (char)1 );
		sprintf( vl_CmdeLCR.ack, "%c%c", XZEXC_CAR_ACK, vl_NoBloc );
		        vl_CmdeLCR.type_msg = XZEXC_MSG_ACQUIT;
			epro_envoi_trame_LCR_IP( &vl_CmdeLCR, TRUE );
	return;		
      }

    /*A cas acquittement positif OK */
    /*case XZEXC_ACK_OK :
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "acquit positif OK" );
*/
     /*A cas acquittement negatif OK */
    case XZEXC_NAK_OK :
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "acquit negatif OK" );

    /*A cas err. CRC bloc intermediaire */
    case XZEXC_ERR_CRC_BLC :
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "err. CRC bloc interm" );

    /*A cas. err. CRC bloc unique */
    case XZEXC_ERR_CRC_MSG :
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "err. CRC bloc unique" );

    /*A cas. msg. sans structure TEDI */
    case XZEXC_TEDI_NOK :
        if( vl_Texte[0] == '\0' )
            strcpy( vl_Texte, "d'info ou sans structure TEDI" );

        if( (pl_TrameLisible = malloc( 4 * (size_t) pa_MsgSock->len + 1 )) )
        {
            XZST_03EcritureTrace( XZSTC_INFO,
                                "epro_traitement_msg_equip_maitre : message %s <%s>\n",
                                vl_Texte,
                                ecom_rendre_trame_lisible( pa_MsgSock->msg, pa_MsgSock->len, pl_TrameLisible ) );
	    if (vg_Config.no_ligne==XDC_EQT_CAP)
	    	ex_ecriture_trace_alerte(XDC_EQT_CAP, 1, pl_TrameLisible);
            free( pl_TrameLisible );
        }
        /* incrementer compteur erreur CRC */
        vl_CmptErrCRC++;

        /*B SI seuil envoi alarme CRC atteint */
        if( vl_CmptErrCRC == vg_Config.ligne.NbEssais )
        {
            /*B ALORS formatage message alarme */
            sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
                                XZEXC_REP_TELEi,
                                ecom_lire_horodate( &vl_Horodate ),
                                vl_CmptErrCRC,
                                0,
                                XZEXC_NOCMD,
                                pl_ListSock->adresse,
                                va_esclave ? XZEXC_TRAME_TELEI_KO : vl_type_trame /*LCL XZEXC_TRAME_STAT_COMP*/,
                                XZEXC_REP_NAK /*XZEXC_REP_ALARME_TELEi*/,
                                XZEXC_SEP_TRAME,
                                "ALM. TEMIP",
                                XZEXC_FIN_MSG_SOCK
                        );
            vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );

            /* reinit compteur erreurs CRC */
            vl_CmptErrCRC = 0;
            /*B OK pour l'envoi du message */
            vl_EnvoiMsg = (XDY_Booleen)XDC_VRAI;
            /*B il faut pas attendre une reponse tache traitement */
            vg_RepTEzzz = XDC_OK;
            /* trace seuil alarme atteint */
            XZST_03EcritureTrace( XZSTC_INFO, "telei ligne %d : seuil alarme %d atteint\nenvoi msg vers TEzzz %s",
                vg_Config.no_ligne, vg_Config.ligne.NbEssais, vl_MsgSock.msg );
        }
        /*B FIN SI */
        break;
    }
    /*A FIN CAS */
    /*A SI message a envoyer */
    if( vl_EnvoiMsg == (XDY_Booleen)XDC_VRAI )
    {
        /*B ALORS emission reponse vers la ( ou les !!! ) tache(s) TEzzz client */
    XZST_03EcritureTrace( XZSTC_FONCTION, "Emission message <%s> \n lg:%d.",vl_MsgSock.msg,vl_MsgSock.len);
    
	for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
    {
	XZST_03EcritureTrace( XZSTC_FONCTION,"socket %d adresse %s prot %c",
					pl_ListSock->desc_sock,
					pl_ListSock->adresse,
					pl_ListSock->Protocole);
	if ((pl_ListSock->Protocole == XZECC_PROT_MIGRAZUR) && (pl_ListSock->desc_sock!=vg_DesSocks.rtsrv)) {
		XZST_03EcritureTrace( XZSTC_FONCTION,"socket %d %s",pl_ListSock->desc_sock, pl_ListSock->adresse);
		break;
	}
    }
    sem_wait(&vg_semaphore); 
    if( write( pl_ListSock->desc_sock, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
 	XZST_03EcritureTrace( XZSTC_WARNING, "Emission message <%s> vers client TEzzz impossible.",
 		 vl_MsgSock.msg );
	    if (vg_Config.no_ligne==XDC_EQT_CAP) {
	    ex_ecriture_trace_alerte(XDC_EQT_CAP, 1, "Emission trame impossible");
	    ex_ecriture_trace_alerte(XDC_EQT_CAP, 1, vl_MsgSock.msg);
    	}
    }
    sem_post(&vg_semaphore); 
    }

    /*envoi commande suivante */
	ecom_delete_cmd_LCR_IP(pl_ListSock->adresse, pl_ListSock->Port, &pg_ListeCmdesLCR);
    if (ecom_getfirst_cmd_LCR_IP(&vl_CmdeLCR, &pg_ListeCmdesLCR, vl_adresse, vl_port)== XDC_OK){
    XZST_03EcritureTrace( XZSTC_FONCTION,"envoi cmd suivante");
    epro_envoi_trame_LCR_IP( &vl_CmdeLCR , FALSE);
	}
    /*A FIN SI */

}

void epro_armer_timeout(char * va_rgs) {
  XDY_Datagroup           pl_dg_timer;
  XZSMT_CleProg           pl_cle_timer;
  int                     vl_JourSemaine;
  XDY_Horodate            vl_DateEnSeconde;

   return;
   sprintf(pl_dg_timer,"%s_%d_%s",XDG_ETEMIP, vg_Config.no_ligne, vg_Config.Nom_machine);
   sprintf (pl_cle_timer, "TEMIP_%s", va_rgs);

   XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_DateEnSeconde);
   vl_DateEnSeconde += 8.0;
   XZSM_01DemanderProgrammation (vl_DateEnSeconde, 0, pl_dg_timer, XDM_ANEUTR_TIMER,
   					pl_cle_timer, 0);
}

/*Signal a la tache TExxx que le serveur DAI est tomb� */
void envoyer_trame_dai_ko(int socket, char addr[]) {
  XZEXT_Msg_Socket	vl_MsgSock;
  XZSMT_Horodate	vl_Horodate;
  
  sprintf( vl_MsgSock.msg, "rep. teimi %s %d %d %d %s %d %d %s %s %s",
	   ecom_lire_horodate( &vl_Horodate),
	   0,/*vl_Cond.nb_non_rep,*/
	   0,/*vl_Cond.nb_err_crc,*/
	   "-1", /*pa_CmdeLCR->no_cmd,*/
	   addr, /*pa_CmdeLCR->adresseIP,*/
	   XZEXC_TRAME_CONF_DAI, /*pa_CmdeLCR->type_cmd, -> vl_typetrame*/
	   XZEXC_PERTE_LIAISON_DAI, /* ->vl_typ_rep*/
	   XZEXC_SEP_TRAME,
	   "?", /*pa_CmdeLCR->cmd_LCR,*/
	   XZEXC_FIN_MSG_SOCK
	   );
    vl_MsgSock.len = (int) strlen( vl_MsgSock.msg );

    sem_wait(&vg_semaphore); 
    if( write(socket, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "envoyer_trame_dai_ko : Emission message <%s> vers client TEzzz impossible.",
			    vl_MsgSock.msg );
    }
    sem_post(&vg_semaphore); 
}
