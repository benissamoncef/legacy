/*E*/
/*Fichier : $Id: ex_mdia.c,v 1.1 2019/03/12 15:13:45 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/03/12 15:13:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mdia.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de traitement des demandes de la fenetre LCR.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "ex_mdia.h"
#include "etim_don.h"

/* definitions de constantes */

#define	ELCRC_TailleFic	300000
#define	ELCRC_DEBUT		1
#define	ELCRC_FIN		2


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ex_mdia.c,v 1.1 2019/03/12 15:13:45 pc2dpdy Exp $ : ex_mdia" ;

/* declaration de fonctions externes */

/* definition de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi une trame a TELMi.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int  EnvoyerTrameDiaser ( XZEXT_Trame	trame,
		    int		type_msg,
		    char*	adr,
		    int		port,
		    int 	nocmd,
		    int 	priorite,
		    int		socket)
/*
* ARGUMENTS EN ENTREE :
*  trame : trame a envoyer
*  type_msg : type du message
*  adr_rgs : adresse rgs 
*  nocmd : numero de commande
*  priorite : priorite de la trame
*  socket : socket d'emision
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   Fonction appele apres avoir construit la trame
*
* FONCTION 
*  Demande d'envoi de trame a telmi
*
------------------------------------------------------*/
{		     
static char *version = "$Id: ex_mdia.c,v 1.1 2019/03/12 15:13:45 pc2dpdy Exp $ : EnvoyerTrame" ;
   
XZEXT_MSG_SOCKET	   vl_message;
ssize_t			   vl_RetCode = 0; 
        
        /*A Construction de la trame */
        sprintf(vl_message,"%s%d %s %d %d %d %s%s%s",
           			XZEXC_DEMANDE_DIASER,
           			type_msg,
           			adr,
				port,
 		    		nocmd,
		    		priorite,
          			XZEXC_SEP_TRAME,
          			trame,
           			XZEXC_FIN_MSG_SOCK);
            			
        fcntl ( socket, F_SETFL, O_NDELAY | fcntl ( socket, F_GETFL, 0 ) );
           			
        //sem_wait(&vg_semaphore); 
        if ( (vl_RetCode = write ( socket , vl_message , strlen(vl_message) )) < 0 ) 				  				
        {
           //sem_post(&vg_semaphore); 
           /*B Ecriture dans le fichier trace en cas d'echec */
           XZST_03EcritureTrace( XZSTC_FATAL, "Impossible d'emettre la trame(%s) vers TEMIP.",
	   		vl_message);
	   return(XDC_NOK);
        }
        else
        {   
           //sem_post(&vg_semaphore); 
           /*B Ecriture dans le fichier trace en cas de succes */
           if ( vl_RetCode != strlen(vl_message) )
           {
              XZST_03EcritureTrace( XZSTC_WARNING, "Trame INCOMPLETE emise vers TELMi <%s>" ,vl_message );
	      return(XDC_OK);
           }
           else
           {
              XZST_03EcritureTrace( XZSTC_INFO, "Trame emise vers TELMi  <%s>" ,vl_message );
	      return(XDC_OK);
           }
        }
}

