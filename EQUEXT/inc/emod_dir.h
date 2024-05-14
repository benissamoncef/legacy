/*E*/
/* Fichier : $Id: emod_dir.h,v 1.8 1995/11/27 11:03:52 volcic Exp $      Release : $Revision: 1.8 $        Date : $Date: 1995/11/27 11:03:52 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEMOD * FICHIER emod_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la 
*  tache TEMOD.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	20 Oct 1994	: Creation
* Mismer.D	version 1.2 	25 Jan 1995	:
* Mismer.D	version 1.3	09 Fev 1995	:
* Volcic.F	version 1.4	27 Avr 1995	: Suppr include xzao.h 
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	23 Jun 1995	: Ajout include etim_don.h
* Mismer.D	version 1.7	03 Oct 1995	: Modif constante EMOD_ARRET_RTSERVER
* Mismer.D	version 1.8	23 Nov 1995	: Suppression constante EMOD_
*
------------------------------------------------------ */

#ifndef emod_dir
#define emod_dir

/* fichiers inclus */

#include <xdc.h>
#include <xdy.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/fcntl.h>
#include <sys/types.h>

#include <xzst.h>
#include <xzsm.h>
#include <xzsa.h>
#include <xzex.h>
#include <xzec.h>
#include <ex_mdon.h>
#include "etim_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XZEXT_MASK *		vg_SockMask = NULL;

/* declarations des fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction effectuant les op≈rations de terminaison de la tache.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	em_arreter ( void );

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*  Fonction d≈clench≈e par SUPERV lors de l'arret de la tache
*  TEMOD
* 
* FONCTION 
*  Fermeture des fichier de configuration
*
------------------------------------------------------*/


#endif
