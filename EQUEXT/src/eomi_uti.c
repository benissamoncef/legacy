/*E*/
/*Fichier : $Id: eomi_uti.c,v 1.6 1995/12/13 15:02:45 volcic Exp $ 	Release : $Revision: 1.6 $ 		Date : $Date: 1995/12/13 15:02:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TEOMi  * FICHIER eomi_uti.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonctions communes modules TEOMi
* 
*
------------------------------------------------------
* HISTORIQUE :
*
*  Nouaille M.	Version 1.1	02 Dec 1994		: Creation
*  Nouaille M.  Version 1.2     24 Jan 1995             : RAS
*  Fontaine C.  Version 1.3     04 Mai 1995             : Reprise entete et commentaire
*  Fontaine C.  Version 1.4     17 Mai 1995             : Reprise historique
*  Mismer   D.  Version 1.5     27 Oct 1995             : Correction fonction lecture trame
*  Fontaine C.  Version 1.4     17 Mai 1995             : Suppression fonction ecom_init_OPE
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "eomi_uti.h"       

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eomi_uti.c,v 1.6 1995/12/13 15:02:45 volcic Exp $ eomi_uti" ;

/* declaration de fonctions externes */

int ecom_extraire_cmd_OPE( XZEXT_Msg_Socket *, XZEXT_Cmde_LCR *, int);
int ecom_lire_msg_OPE ( int, char *,int *,XZEXT_MASK **,int *, double ) ;
int ecom_flush_buff( int, double ) ;

/* definition de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Extraction d'une demande OPERATOR d'un msg. socket ds. une structure cmde. OPERATOR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_OPE( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande.
*
* ARGUMENTS EN SORTIE :
*   
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande depouillee.
*
* CODE RETOUR : 
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*   
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande.
*
* FONCTION 
*   depouillement de la demande et stokage ds. la structure cmde.
*   
------------------------------------------------------*/
int	ecom_extraire_cmd_OPE( 	XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR, 
				int va_DesSockClnt )
{
    static char *version = "$Id: eomi_uti.c,v 1.6 1995/12/13 15:02:45 volcic Exp $ : ecom_extraire_cmd_OPE" ;
    
    char		*pl_DebTrameLCR;	/* pointe sur le debut de la trame */
    XDY_Texte		vl_Texte;	/* juste pour rec. le separateur de trame */
    

    /*A SI depouillement message socket echoue */
    if( sscanf( &(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_OPE)]), "%d %s %d %d %s",
           			&(pa_CmdeLCR->type_cmd),
           			pa_CmdeLCR->rgs,
 		    		&(pa_CmdeLCR->no_cmd),
		    		&(pa_CmdeLCR->prior),
          			vl_Texte ) != 5 
          	|| (pl_DebTrameLCR = strstr( pa_MsgSock->msg, XZEXC_SEP_TRAME )) == NULL )
    {
    	/*A ALORS trace " Demande incorrecte <message> */
        XZST_03EcritureTrace( XZSTC_WARNING, "ecom_extraire_cmd_OPE: Demande incorrecte : <%s>", pa_MsgSock->msg );
        /*A retour XDC_NOK */
        return( XDC_NOK );
    }
            
    /*B recuperation trame */
    pl_DebTrameLCR += sizeof(XZEXC_SEP_TRAME);
    pa_MsgSock->msg[pa_MsgSock->len-(strstr(pa_MsgSock->msg, XZEXC_FIN_MSG_SOCK) == NULL ? 0 : sizeof(XZEXC_FIN_MSG_SOCK))] = '\0';
    strcpy( pa_CmdeLCR->cmd_LCR, pl_DebTrameLCR );
 
    XZST_03EcritureTrace( XZSTC_INFO, "ecom_extraire_cmd_OPE: %d %d trame <%s>", 
            		sizeof(XZEXC_SEP_TRAME), sizeof(XZEXC_FIN_MSG_SOCK), pa_CmdeLCR->cmd_LCR );
            								 
    /*B attachement desc. socket client a sa demande */
    pa_CmdeLCR->sock_clnt = va_DesSockClnt;
            
    /*B le type du message n'est pas utilise par TEOPE */
    pa_CmdeLCR->type_msg = 0;
            
    /*B le champs ack n'est pas utilise par TEOPE */
    pa_CmdeLCR->ack[0] = XZEXC_NULC;
    
    /*B si la cmde va generer un echange en blocs, le 1er a recevoir a le no. 0 */
    pa_CmdeLCR->no_blc_suivant = '0';

    /*A retour XDC_OK */  
    return( XDC_OK );

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet de lire un message dans 
*   la socket specifiee. 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ecom_lire_msg_OPE (
 		int		va_sd_sock,
		char 		*pa_message,
 		int		*pa_lg_msg,
 		XZEXT_MASK *  	*pa_masque,
		int		*pa_deconnex,
		double          va_attente )

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donnees.
*   pa_message	:	pointeur de reception du message.
*   pa_masque	:	masque de lecture.
*   va_sleep    :       temps d'attente nouveaux caracteres
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_message	:	pointeur de reception du message.
*   pa_lg_msg	:	pointeur de reception de la longueur du message.
*   pa_deconnex	:	indicateur de deconnexion.
*
* CODE RETOUR : 
*
*   XDC_OK		succes
*   XDC_NOK		lecture message socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Extraire un message d'une socket de donnees.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/
{
   static char *version = "$Id: eomi_uti.c,v 1.6 1995/12/13 15:02:45 volcic Exp $ : ecom_lire_msg_OPE ";
   register int			vl_valeurRetour = XDC_OK,
   				vl_NbCar        = 0,
  				vl_cr           = 0;
   XZEXT_MSG_SOCKET		vl_message      = ""; 
   int				vl_index	= 0;
   
   /*A Lecture de la taille du message de la socket specifiee. */
   fcntl ( va_sd_sock, F_SETFL, O_NDELAY | fcntl ( va_sd_sock, F_GETFL, 0 ) );
   sem_wait(&vg_semaphore); 
   if ( ( vl_cr = recv ( va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET), MSG_PEEK ) ) == -1 ) 				  				
   {   
      sem_post(&vg_semaphore); 
      /*B Ecriture dans le fichier trace en cas d'echec */
      XZST_03EcritureTrace( XZSTC_WARNING, "Erreur de lecture de message sur une socket." );
      vl_valeurRetour = XDC_NOK; 
   }
   else
   {      
      sem_post(&vg_semaphore); 
      /*A Si la longueur du message est nulle */
      if ( vl_cr == 0)
      {
         /*A Fermeture de la socket */
        
         sem_wait(&vg_semaphore); 
         read ( va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) );
         sem_post(&vg_semaphore); 
         ex_fin_cnx ( va_sd_sock, pa_masque );
         
       	 /*A Positionnement a VRAI de l'indicateur de deconnexion socket */
         *pa_deconnex = XZEXC_DECONNEC;
      }
      else
      {
      	 /*A Positionnement a FAUX de l'indicateur de deconnexion socket */
         *pa_deconnex = XZEXC_CONNEC;
         
         /*A Recherche si le message a ete recu en entier */
         while ( (ex_select(va_sd_sock, va_attente ) == XDC_OK) && (vl_NbCar < sizeof(XZEXT_MSG_SOCKET)) ) 
         {
            sem_wait(&vg_semaphore); 
            if ( ( vl_cr = read ( va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET)-2 ) ) == -1 )
            {
               sem_post(&vg_semaphore); 
               XZST_03EcritureTrace( XZSTC_WARNING, "Erreur de lecture de message socket:%d .", va_sd_sock );
               return ( XDC_NOK );
            }
            sem_post(&vg_semaphore); 
            for (vl_index = 0 ; vl_index <= vl_cr; vl_index++ ) 
               if ( (vl_message[vl_index] >= ' ') && (vl_message[vl_index] <= 'z') ) pa_message[vl_NbCar++] = vl_message[vl_index];
         }
         
         /*A Recopie du message pour l'appelant avec suppression des craracteres non affichables */
         pa_message[vl_NbCar] = '\0';
         *pa_lg_msg = vl_NbCar;
       }      
   }

   return ( vl_valeurRetour );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet de effacer le buffer de comm. 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ecom_flush_buff ( int		va_sd_sock, double	va_attente )

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donnees.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*   XDC_OK		succes
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Extraire un message d'une socket de donnees.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/
{
   static char *version = "$Id: eomi_uti.c,v 1.6 1995/12/13 15:02:45 volcic Exp $ : ecom_flush_buff ";
   
   XZEXT_MSG_SOCKET		vl_vidange      = ""; 
   	   

   /*A Vidange du buffer */
   sem_wait(&vg_semaphore); 
   while ( ex_select(va_sd_sock, va_attente ) == XDC_OK )
   {
       /*A Si un �venement est survenu sur une socket */
       if ( read ( va_sd_sock, vl_vidange, 1 ) == -1 ) break;
   }
   sem_post(&vg_semaphore); 
   return ( XDC_OK );   

}
