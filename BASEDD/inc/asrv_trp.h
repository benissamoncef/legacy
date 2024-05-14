/*E Fichier : $Id: asrv_trp.h,v 1.2 2009/12/07 14:49:25 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/12/07 14:49:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE TRP * FICHIER TRP.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	26 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef TRP
#define TRP

/* fichiers inclus */
	 #include        <stdio.h>
	 #include        "ospublic.h"
	 #include        "oserror.h"
	 #include        "xdc.h"
	 #include        "xdy.h"
	 #include	 "xzec.h"
	 #include        "xzir.h"
	 #include        "asrv_rpcom.h"

/* definitions de constantes exportees */

#define ca_atrp18_nb_param 5
#define ca_atrp19_nb_param 6

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern CS_RETCODE atrp18 (SRV_PROC        *);
extern CS_RETCODE atrp19 (SRV_PROC        *);

#endif
