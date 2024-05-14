/*E Fichier : $Id: asrv_prp.h,v 1.1 1994/09/26 14:43:16 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/26 14:43:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE MRP * FICHIER MRP.h
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

#ifndef PRP
#define PRP

/* fichiers inclus */
        #include        <stdio.h>
        #include        "ospublic.h"
        #include        "oserror.h"
        #include 	"xdc.h"
        #include 	"xdy.h"
        #include 	"xzen.h"
        #include 	"xzep.h"
        #include 	"xzet.h"
        #include 	"asrv_rpcom.h"
	#include 	"asql_com.h"
	
/* definitions de constantes exportees */
#define ca_aprp22_nb_param 	5

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern CS_RETCODE aprp22 (SRV_PROC        *);

#endif
