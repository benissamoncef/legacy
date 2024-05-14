/*E*/
/*Fichier : $Id: xzae15.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae15.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	09/05/22	: Creation SAE-DEM367
------------------------------------------------------ */

#ifndef xzae15
#define xzae15

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

#define XZAE15C_RPC_NAME "XZAE15"
#define XZAE15C_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int xzae15_Transmettre_Droit_Enrichir (	 XDY_Entier	va_NumEvt_in,
                                         XDY_Octet	va_CleEvt_in,
                                         XDY_Octet	va_Site_in,
                                         XDY_Mot va_PosteOperateur_in,
                                         XDY_Entier	va_Sans_Rafraichir_in,
                                         XDY_Entier	va_Sans_Reveil_in,
										 XDY_NomSite va_NomSiteLocal_in,
                                         XDY_Entier	*va_Resultat_out);

#endif
