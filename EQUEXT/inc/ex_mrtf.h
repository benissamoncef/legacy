/*E*/
/*Fichier : $Id: ex_mrtf.h,v 1.5 2011/02/24 19:17:19 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2011/02/24 19:17:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mrtf.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees pour le fichier des messages
*  Rtworks communs au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Oct 1994	: Creation
* Mismer.D	version 1.2	09 Dec 1994	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.4	02 Jun 1995	: Ajout fonction reconnexion auto rtserver
*
* JPL		24/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket arg. de 'ex_ConnexionSocketRTserver'  1.5
------------------------------------------------------ */

#ifndef EX_MRTF
#define EX_MRTF

/* fichiers inclus */

#include <stdio.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/time.h>

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzss.h"
#include "xzex.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sauvegarde d'un message RTworks dans un fichier ASCII.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ex_OuvertureFichierMsg (	XDY_NomMachine	pa_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*
*  XDY_NomMachine	pa_NomMachine : Nom Machine
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Construction du message et envoi du message
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sauvegarde d'un message RTworks dans un fichier ASCII.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ex_RecopieFichierMsg (	XDY_NomMachine	pa_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*
*  XDY_NomMachine	pa_NomMachine : Nom Machine
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Recopie du fichier de message RTworks dans le repertoire de lecture pour TEMOD
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sauvegarde d'un message RTworks dans un fichier ASCII.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ex_TipcMsgFileWrite (	T_STR 		va_Dest,
					T_IPC_MT 	va_MT,
					T_BOOL 		va_CheckMsg,
					... );

/*
* ARGUMENTS EN ENTREE :
*
*  T_STR 		va_Dest		: Datagroup de destination du message
*  T_IPC_MT 		va_MT		: Identifiant du message
*  T_BOOL 		va_CheckMsg	: Mode d'emission du message
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Construction du message et envoi du message
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Connexion Socket RTserver.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ex_ConnexionSocketRTserver ( 	T_SOCK 		*va_SockRT,
						int		va_SocketHS,
 						XZEXT_MASK *    *pa_masque );

/*
* ARGUMENTS EN ENTREE :
* XZEXT_MASK *    *pa_masque 	: liste des sockets tache EQUEXT
*
* ARGUMENTS EN SORTIE :
* int 		  *va_SockRT	: descripteur de la socket RTserver
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  
*
------------------------------------------------------*/

#endif
