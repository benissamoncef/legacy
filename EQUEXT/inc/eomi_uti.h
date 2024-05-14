/*E Fichier : $Id: eomi_uti.h,v 1.2 1995/05/04 20:14:11 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/04 20:14:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOMi * FICHIER eomi_uti.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusions des fichiers .h necessaires ds. les modules eomi_xxx 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     04 Mai 1995     : Reprise entete
------------------------------------------------------ */

#ifndef eomi_uti
#define eomi_uti

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

#include <xdy.h>
#include <xdc.h>
#include <xdf.h>
#include <xdl.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzex.h>
#include <xzec.h>
#include <xzss.h>
#include <xzsm.h>
#include <xzamc.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

/*#include "ex_mala.h"*/
/* fichiers inclus */
#include "eomi_common.h"

/* definitions de constantes exportees */

#define ELxiC_ST_NOK		9	/* retour fonc. init si comms. avec ST impossible */

/*! A remettre dans les fichiers .h */

#define XZAMC_SOCKET_ST_HS	3	
#define XZAMC_SOCKET_ST_HS_NOM	"DEFAUT COUPURE SOCKET"
#define XZAMC_SOCKET_ST_HS_URG	XZAMC_NON_URGENT


/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

/* definitions de constantes exportees */

/*! A remettre dans le fichier xdf.h */

/* definitions de types exportes */

/* type configuration tache TEOMi */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

#endif
