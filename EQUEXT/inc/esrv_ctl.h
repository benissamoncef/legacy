/*X*/
/* Fichier : $Id: esrv_ctl.h,v 1.6 2011/02/24 19:04:59 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/02/24 19:04:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESRV * FICHIER esrv_ctl.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de controle de la tache TESRV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	29 Nov 1994	:
* Mismer.D	version 1.3	13 Dec 1994	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Fontaine.C	version 1.5	22 Mai 1995	: Reprise entete, historique
*
* JPL		24/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket ; declaration 'el_arret_tache'  1.6
------------------------------------------------------ */

#ifndef esrv_ctl
#define esrv_ctl

/* fichiers inclus */
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <termio.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/signal.h>
#include <sys/syscall.h>
#include <xdc.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzsm.h>

#include <rtworks/common.h>
#include <rtworks/ipc.h>

#include "ex_msok.h"
#include "xzec.h"
#include "xzex.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK *	vg_SockMask;
extern int		vg_SockEct;
extern int		vg_SockTETIM;
extern T_SOCK		vg_SockRT;

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer des tache temporaire du sous-systeme EQUEXT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int el_lancer_tache_protocole ( 	char 	va_TypeProtocole,
					char	va_Sens,
				       	char 	*pa_NumLiaison );

/*
* ARGUMENTS EN ENTREE :
*  va_TypeProtocole		Type de protocole a lancer
*  va_NumLiaison		Numero de la liaison
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  XDC_OK		succes
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Selon le type de protocole ( XZECC_PROT_LCR_MAITRE, XZECC_PROT_LCR_ESCLAVE, 
*   XZECC_PROT_APRILNET, XZECC_PROT_OPERATOR ) lancement de la tache temporaire
*   corespondante.
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer une tache equipement suivant
*  le type de l'equipement specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int el_lancer_tache_eqt ( char 	*pa_TypeEqt );

/*
* ARGUMENTS EN ENTREE :
*  pa_TypeEqt		Type d'equipement a lancer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  XDC_OK		succes
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Selon le type d'equipement ( XZECC_TYPE_EQT_xxx ) creation de la tache TEzzz
*   relative a l'equipement identifie par zzz.
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer une tache quelconque dont le nom est specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int el_lancer_tache ( char 	*pa_NomTache , char *	pa_NumLiaison );

/*
* ARGUMENTS EN ENTREE :
*  pa_NomTache		Nom de la tache a creer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  			succes PID de la tache
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Creation de la tache TEzzz specifie en entree
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d'arreter une tache.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int el_arreter_tache ( int	va_TypeTache );

/*
* ARGUMENTS EN ENTREE :
*  va_TypeTache		: Type de la tache a arreter
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK		: Succes
*   XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie du message de demande d'arret par socket 
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d'arret une tache quelconque dont le nom est specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void el_arret_tache ( 	char 	va_TypeProtocole,
			char	va_Sens,
			char 	*pa_NumLiaison );

/*
* ARGUMENTS EN ENTREE :
*  pa_NomTache		Nom de la tache a creer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  			succes PID de la tache
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Arret de la tache TEzzz specifie en entree
*
------------------------------------------------------*/


#endif
