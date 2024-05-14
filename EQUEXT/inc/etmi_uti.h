/*E Fichier : $Id: etmi_uti.h,v 1.3 1995/11/27 11:07:53 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/11/27 11:07:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_uti.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusions des fichiers .h necessaires ds. les modules etmi_xxx 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     03 Mai 1995     : RAS
* Mismer D.     Version 1.3     15 Nov 1995     : Modif declaration fonction ecom_lire_msg_TEL
------------------------------------------------------ */

#ifndef etmi_uti
#define etmi_uti

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

/* fichiers inclus */
#include "etmi_common.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

int ecom_init_TEL( XZEXT_ST_Config *pa_Config, XZEXT_MASK **, XZEXT_ST_Sockets * );

int ecom_extraire_cmd_TEL( XZEXT_Msg_Socket *, XZEXT_Cmde_LCR *, int);

int ecom_lire_msg_TEL ( int, char *, int *, XZEXT_MASK **, int *, double );


#endif
