/*E Fichier : $Id: asrv_rrp.h,v 1.3 1994/12/16 10:00:46 mercier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/12/16 10:00:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE RRP * FICHIER RRP.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef RRP
#define RRP

/* fichiers inclus */
        #include        <stdio.h>
        #include        "ospublic.h"
        #include        "oserror.h"
        #include 	"xdc.h"
        #include 	"xdy.h"
	#include 	"xzit.h"
        #include 	"asrv_rpcom.h"

/* definitions de constantes exportees */
#define ca_arrp03_nb_param	5

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern CS_RETCODE arrp03(SRV_PROC        *); /* Fait appel a XZIT08() */

#endif
