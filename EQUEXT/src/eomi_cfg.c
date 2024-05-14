/*E*/
/*Fichier : $Id: eomi_cfg.c,v 1.4 1995/05/22 19:01:56 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/22 19:01:56 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TEOMi * FICHIER eomi_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
* 
* Fonctions de initialisation et configuration de la tache TEOMi
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Mismer D.     Version 1.2     26 Jan 1995     : R.A.S
* Fontaine C.   Version 1.3     04 Mai 1995     : Reprise entete et commentaire
* Fontaine C.   Version 1.4     17 Mai 1995     : Reprise Historique
------------------------------------------------------*/

/* fichiers inclus */

#include "eomi_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eomi_cfg.c,v 1.4 1995/05/22 19:01:56 cornu Exp $ : eomi_cfg" ;

/* declaration de fonctions externes */

extern void	ecom_detruire_liste_sock( XZEXT_MASK ** );
extern void	ecom_destroy_list_cmd_LCR( XZEXT_Liste_Cmdes_LCR ** );


/* declaration de fonctions internes */

void 	ecfg_sortir( void );

/* definition de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Sortie de la tache TEOMi
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
    static char *version = "$Id: eomi_cfg.c,v 1.4 1995/05/22 19:01:56 cornu Exp $ : ecfg_sortir" ;
    XZEXT_MASK 	*pl_Socket;
    
    /*A liberation memoire occuppee par la liste de cmdes non traitees 
     *	appel ecom_destroy_list_cmd_LCR() */
    ecom_destroy_list_cmd_LCR( &pg_ListeCmdesLCR );
    
    /*A liberation memoire occuppee par la liste de sockets 
     *	appel ecom_detruire_liste_sock() */
    ecom_detruire_liste_sock( &vg_SockList );
    
    /*A EXIT XDC_OK */
    exit( XDC_OK ) ;
}
