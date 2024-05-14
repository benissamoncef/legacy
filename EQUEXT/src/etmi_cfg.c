/*E*/
/*Fichier : $Id: etmi_cfg.c,v 1.4 1995/05/22 19:02:24 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/22 19:02:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
* 
* Fonctions de initialisation et configuration de la tache TETMi
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Mismer D.     Version 1.2     26 Jan 1995     : RAS
* Fontaine C.   Version 1.3     03 Mai 1995     : Reprise des entetes et des commentaires
* Fontaine C.   Version 1.4     17 Mai 1995     : Reprise historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "etmi_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etmi_cfg.c,v 1.4 1995/05/22 19:02:24 cornu Exp $ : etmi_cfg" ;

/* declaration de fonctions externes */

extern void	ecom_detruire_liste_sock( XZEXT_MASK ** );
extern void	ecom_destroy_list_cmd_LCR( XZEXT_Liste_Cmdes_LCR ** );


/* declaration de fonctions internes */

void 	ecfg_sortir( void );

/* definition de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Sortie de la tache TETMi
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
    static char *version = "$Id: etmi_cfg.c,v 1.4 1995/05/22 19:02:24 cornu Exp $ : ecfg_sortir" ;
    XZEXT_MASK 	*pl_Socket;
    
    /*A liberation memoire occuppee par la liste de cmdes. LCR non traitees 
     *	appel ecom_destroy_list_cmd_LCR() */
    ecom_destroy_list_cmd_LCR( &pg_ListeCmdesLCR );
    
    /*A liberation memoire occuppee par la liste de sockets 
     *	appel ecom_detruire_liste_sock() */
    ecom_detruire_liste_sock( &vg_SockList );
    
    /*A EXIT XDC_OK */
    exit( XDC_OK ) ;
}
