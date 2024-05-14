/*E*/
/* Fichier : $Id: elei_pro.c,v 1.6 2011/02/25 10:32:29 gesconf Exp $        Release : $Revision: 1.6 $        Date : $Date: 2011/02/25 10:32:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER elei_pro.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* Module de gestion du protocole LCR esclave de la tache TELEi.
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	07 Nov 1994	: Creation
* Mismer.D	version 1.2	13 Jan 1995	: 
* Mismer.D	version 1.3	11 Avr 1995	: 
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise historique
*
* JPL		25/02/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,*)  1.6
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "elei_pro.h"
#include "ex_mtcr.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: elei_pro.c,v 1.6 2011/02/25 10:32:29 gesconf Exp $ : elei_pro" ;

/* declaration de fonctions externes */

/* definition de fonctions internes */

void epro_traitement_msg_equip_maitre( XZEXT_Msg_Socket * );




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* Traitement des messages equipements maitres.
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  void epro_traitement_msg_equip_maitre( XZEXT_Msg_Socket *pa_MsgSock )
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: message recu sur la socket a verifier.
*
* ARGUMENTS EN SORTIE :
*
* pa_MsgSock	: pa_MsgSock->msg contient le trame LCR si le message est correcte TEDI.
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
* 
* pa_MsgSock	: doit contenir le message recu sur la socket ligne serie.
*
* FONCTION 
*
* - verification du message contnu ds. pa_MsgSock->msg.
* - seule une REQUETTE ( [ENQ]rgs0 ... [ETX][BCC] ) est reconnue comme message correcte 
*   d'un point de vue TEDI.
* - toute autre trame est une erreur.
*
------------------------------------------------------*/
void epro_traitement_msg_equip_maitre( XZEXT_Msg_Socket *pa_MsgSock )
{
    static char *version = "$Id: elei_pro.c,v 1.6 2011/02/25 10:32:29 gesconf Exp $ : epro_traitement_msg_equip_maitre" ;
    
    int		vl_ResVerif;		/* resultat verif. de la trame TEDI/LCR envoyee par l'equip. */
    char	vl_DDM[5];		/* DDM a verifier en debut du message recu */
    char	vl_NoBloc;		/* no. bloc du message recu */
    char	*pl_TrameLisible;	/* pointeur sur la trame en format lisible */
    XZEXT_MASK	*pl_ElmSockList;	/* pointeur sur un elm. de la liste des sockets */
    XDY_Texte	vl_Texte;		/* texte trace */
    XDY_Booleen	vl_EnvoiMsg = (XDY_Booleen)XDC_FAUX;	/* teste pour l'envoi d'un message vers tache traitement */
    XZEXT_Msg_Socket	vl_MsgSock; 	/* message a envoyer a la tache TEzzz */
    XZSMT_Horodate	vl_Horodate;	/* horodate en format converti */
    static int	vl_CmptErrCRC = 0;  	/* compteur erreurs CRC */
   
    
    vl_Texte[0] = '\0';
    
    /*A mise a jour du DDM a verifier avec l'adresse rgs de SAE et le car. debut message */
    sprintf( vl_DDM, "%c%s", XZEXC_CAR_ENQ, vg_AdrSAE );

    /*A on s'attend aussi qu'on acquitte avec no. bloc =  XZEXC_CAR_NO_BLC_REQ */
    vl_NoBloc = XZEXC_CAR_NO_BLC_REQ;

    /*A CAS resultat de la verification de la trame recue */
    switch( vl_ResVerif = ex_verif_TEDI_CRC( pa_MsgSock, vl_DDM, &vl_NoBloc ) )
    {
    /*A cas message bloc unique OK */
    case XZEXC_MSG_OK :
    	/*B le cas correcte - TELEi recoit seulement des DEMANDES TEDI/LCR */

    	/*B formatage message reponse pour la tache TEzzz client */
    	sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
    				XZEXC_REP_TELEi,
				ecom_lire_horodate( &vl_Horodate ),
				0,
    	    			0,
    	    			XZEXC_NOCMD,
    	    			vg_AdrSAE,
    	    			XZEXC_TRAME_TELEI_OK,
    	    			XZEXC_REP_BLC_FIN,
    	    			XZEXC_SEP_TRAME,
    	    			pa_MsgSock->msg,
    	    			XZEXC_FIN_MSG_SOCK
    	    		);
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
	if( vl_Texte[0] == '\0' )
	    strcpy( vl_Texte, "bloc intermed OK" );
	    
    /*A cas acquittement positif OK */	
    case XZEXC_ACK_OK :		
	if( vl_Texte[0] == '\0' )
	    strcpy( vl_Texte, "acquit positif OK" );
    
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
    	    free( pl_TrameLisible );
    	}
    	
    	/* incrementer compteur erreur CRC */
    	vl_CmptErrCRC++;
    	
    	/*B SI seuil envoi alarme CRC atteint */
    	if( vl_CmptErrCRC == vg_Config.ligne_serie.NbEssais )
    	{
    	    /*B ALORS formatage message alarme */
    	    sprintf( vl_MsgSock.msg, "%s %s %d %d %d %s %d %d %s %s %s",
    				XZEXC_REP_TELEi,
				ecom_lire_horodate( &vl_Horodate ),
				vl_CmptErrCRC,
    	    			0,
    	    			XZEXC_NOCMD,
    	    			vg_AdrSAE,
    	    			XZEXC_TRAME_TELEI_KO,
    	    			XZEXC_REP_ALARME_TELEi,
    	    			XZEXC_SEP_TRAME,
				EPROC_TXT_ALM_TELEi,
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
    	    	vg_Config.no_ligne, vg_Config.ligne_serie.NbEssais, vl_MsgSock.msg );
    	}
    	/*B FIN SI */
        break;
    }
    /*A FIN CAS */
    
    /*A SI message a envoyer */
    if( vl_EnvoiMsg == (XDY_Booleen)XDC_VRAI )
    {
    	/*B ALORS emission reponse vers la ( ou les !!! ) tache(s) TEzzz client */
    	/*B POUR tous les elems. de la liste de sockets */
    	for( 	pl_ElmSockList = vg_SockList; pl_ElmSockList != NULL; 
    		pl_ElmSockList = pl_ElmSockList->Suivant )
	{
	    /*B SI socket diff. de la socket ecoute et de la socket ligne serie ALORS */
	    if( pl_ElmSockList->desc_sock != vg_DesSocks.ect && 
	    	pl_ElmSockList->desc_sock != vg_DesSocks.rtsrv &&
	    	pl_ElmSockList->desc_sock != vg_DesSocks.lser )
	    {
    	    	/*B SI emission reponse echoue ALORS ecriture trace " Emission message vers clnt. TEzzz impossible. " */
				sem_wait(&vg_semaphore); 
            	if( write( pl_ElmSockList->desc_sock, vl_MsgSock.msg, vl_MsgSock.len ) != vl_MsgSock.len )
      	    	{
 	    	    XZST_03EcritureTrace( XZSTC_WARNING, "Emission message \n<%s>\nvers client TEzzz impossible.",
 	    	    	vl_MsgSock.msg );
    	    	}
				sem_post(&vg_semaphore); 
    	    }
    	}
    	/* FIN POUR */
    }
    /*A FIN SI */

}




