/*E*/
/*Fichier : $Id: elmi_cfg.c,v 1.3 1995/05/23 09:09:26 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/23 09:09:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER elmi_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
* 
* Fonctions d'initialisation et de configuration de la tache TELMi
* 
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	25 Oct 1994	: Creation
* Mismer.D	version 1.2	26 Jan 1995	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "elmi_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: elmi_cfg.c,v 1.3 1995/05/23 09:09:26 cornu Exp $ : elmi_cfg" ;

/* declaration de fonctions externes */

extern void	ecom_detruire_liste_sock( XZEXT_MASK ** );
extern void	ecom_destroy_list_cmd_LCR( XZEXT_Liste_Cmdes_LCR ** );


/* declaration de fonctions internes */

void 	ecfg_sortir( void );

/* definition de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Sortie de la tache TELMi
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecfg_sortir( void )
*
*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*   
*   NEANT
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Liberation ressources memoire.
*   Fermeture sockets.
*   
------------------------------------------------------*/
void ecfg_sortir( void )
{
    static char *version = "$Id: elmi_cfg.c,v 1.3 1995/05/23 09:09:26 cornu Exp $ : ecfg_sortir" ;
    XZEXT_MASK 	*pl_Socket;
    
    /*A liberation memoire occuppee par la liste de cmdes. LCR non traitees 
     *	appel ecom_destroy_list_cmd_LCR() */
    ecom_destroy_list_cmd_LCR( &pg_ListeCmdesLCR );
    
    /*A liberation memoire occuppee par la liste de sockets 
     *	appel ecom_detruire_liste_sock() */
    ecom_detruire_liste_sock( &vg_SockList );
    
    /*A EXIT XDC_OK */
    exit( XDC_OK );
}
