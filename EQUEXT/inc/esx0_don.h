/*E*/
/* Fichier : $Id: esx0_don.h,v 1.2 1995/05/23 11:42:37 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 11:42:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier de donnees de la tache
*   TESX0, protocole de la tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	21 Dec 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef esx0_don
#define esx0_don

/* fichiers inclus */


#include "ex_mdon.h"
#include "etim_don.h"
#include "esx0_cmd.h"
#include "esx0_cfg.h"
#include "xzec.h"
#include "xzamc.h"

/* definitions de constantes exportees */

#define ESXC_REVEIL_30S		30
#define ESXC_REVEIL_10S		10



#define XZEXC_PATH_SATIR_TEMP_EMISSION	"/usr/tmp/satir/tmp/emission"
#define XZEXC_PATH_SATIR_TEMP_RECEPTION	"/usr/tmp/satir/tmp/reception"
#define XZEXC_PATH_SATIR_EMISSION	"/usr/tmp/satir/emission"
#define XZEXC_PATH_SATIR_RECEPTION	"/usr/tmp/satir/reception"

#endif
