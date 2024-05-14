/*E*/
/*Fichier : $Id: eimi_cfg.c,v 1.1 2019/01/29 11:08:01 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/01/29 11:08:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER eimi_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
* 
* Fonctions d'initialisation et de configuration de la tache TELMi
* 
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------*/

/* fichiers inclus */

#include "eimi_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eimi_cfg.c,v 1.1 2019/01/29 11:08:01 pc2dpdy Exp $ : eimi_cfg" ;

/* declaration de fonctions externes */

extern void	ecom_detruire_liste_sock( XZEXT_MASK ** );
extern void	ecom_destroy_list_cmd_LCR_IP( XZEXT_Liste_Cmdes_LCR_IP ** );


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
    static char *version = "$Id: eimi_cfg.c,v 1.1 2019/01/29 11:08:01 pc2dpdy Exp $ : ecfg_sortir" ;
    XZEXT_MASK 	*pl_Socket;
    
    /*A liberation memoire occuppee par la liste de cmdes. LCR non traitees 
     *	appel ecom_destroy_list_cmd_LCR() */
    ecom_destroy_list_cmd_LCR_IP( &pg_ListeCmdesLCR );
    
    /*A liberation memoire occuppee par la liste de sockets 
     *	appel ecom_detruire_liste_sock() */
    ecom_detruire_liste_sock( &vg_SockList );
    
    /*A EXIT XDC_OK */
    exit( XDC_OK );
}
