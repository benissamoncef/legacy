/*E*/
/*Fichier : $Id: elei_cfg.c,v 1.4 1995/05/23 09:09:20 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/23 09:09:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELEi * FICHIER elei_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fonctions specifique de controle et config. tache TELEi
* 
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	07 nov 1994	: Creation
* Flaidar.A	version 1.2	02 Dec 1994	:
* Mismer.D	version 1.3	26 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "elei_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: elei_cfg.c,v 1.4 1995/05/23 09:09:20 cornu Exp $ : elei_cfg" ;

/* declaration de fonctions externes */

/* declaration de fonctions internes */

void 	ecfg_sortir( void );


/* definition de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sortie de la tache telei.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*  void 	ecfg_sortir( void )
*
*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  Liberation ressources memoire.
*  Fermeture sockets.
*
------------------------------------------------------*/
void 	ecfg_sortir( void )
{
    static char *version = "$Id: elei_cfg.c,v 1.4 1995/05/23 09:09:20 cornu Exp $ : ecfg_sortir" ;
    
    /*A liberation memoire occuppee par la liste de sockets 
     *	appel ecom_detruire_liste_sock() */
    ecom_detruire_liste_sock( &vg_SockList );
    
    /*A sortie XDC_OK */
    exit( XDC_OK );
    
}
