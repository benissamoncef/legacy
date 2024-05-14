/*E*/
/*Fichier : $Id: xzae99.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae99.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	20/01/22	: Creation SAE-DEM367
------------------------------------------------------ */

#ifndef xzae99
#define xzae99

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

/* Include Sybase */
#include <ctpublic.h>
#include <cstypes.h>

/* Include applicatif Migrazur */
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAE99C_RPC_NAME "XZAE99"
#define XZAE99C_NB_PARM_RETURN 5

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int xzae99_Rechercher_NumFMC_NumAction_par_idAVA (XDY_Texte	va_id_ava_in,
                                            XDY_Entier	*va_numEvt_out,
                                            XDY_Octet	*va_cleEvt_out,
											XDY_Entier	*va_numEvtCause_out,
                                            XDY_Octet	*va_cleEvtCause_out,
                                            XDY_Entier *va_numAction_out);

#endif
