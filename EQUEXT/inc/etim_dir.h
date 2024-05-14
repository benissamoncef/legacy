/*E*/
/* Fichier : $Id: etim_dir.h,v 1.2 1995/05/23 12:49:53 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 12:49:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache
*  TETIM
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef etim_dir
#define etim_dir

/* fichiers inclus */
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "etim_don.h"
#include "etim_trt.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction effectuant les operation de terminaison de la tache TETIM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ed_arreter ( void );

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   Fonction declenchee par la supervision lors de l'arret de la tache ou apres
*   reception du message d'arret de la part de TESRV
*
* FONCTION 
*   Inhibition du signal d'alarme
*   Annulation de l'alarme en cours
*   Liberation des ressources memoires
*   Fermeture de toutes les sockets
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction traite les message recu de TESRV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ed_cmd ( int va_NumSock );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket de TESRV
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK		: Message de TESRV correct
*   XDC_NOK		: Message de TESRV incorrect
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture du message recu dans la socket avec TESRV
*   Examen du message
*
------------------------------------------------------*/

#endif
