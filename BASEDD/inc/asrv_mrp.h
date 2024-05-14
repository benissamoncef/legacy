/*E Fichier : $Id: asrv_mrp.h,v 1.1 1994/09/26 14:43:15 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/26 14:43:15 $
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

#ifndef MRP
#define MRP

/* fichiers inclus */
        #include        <stdio.h>
        #include        "ospublic.h"
        #include        "oserror.h"
        #include "xdc.h"
        #include "xdy.h"
	#include "xzis.h"
	#include "xzit.h"
        #include "asrv_rpcom.h"

/* definitions de constantes exportees */
#define ca_amrp05_nb_param	0
#define ca_amrp07_nb_param	3

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern CS_RETCODE amrp05(SRV_PROC        *);
extern CS_RETCODE amrp07(SRV_PROC        *);

#endif
