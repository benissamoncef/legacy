/*  Fichier : 	@(#)xzat511.h
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat511.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/2023	:	Creation (DEM483)
------------------------------------------------------ */
#ifndef xzat511
#define xzat511

/* fichiers inclus */

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
#define XZATC_XZAT511_RPC_NAME "XZAT511"
#define XZATC_XZAT511_NB_PARM_RETURN 10

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

extern int XZAT511_Utilisation_PIC  ( 	XDY_Horodate			va_Horodate_in,
								XDY_Mot					va_NumEqt_in,
								XDY_Octet				*va_DispoPIC_out,
								XDY_Bandeau_PIC			*va_Bandeau_out,
								XDY_Booleen				*va_Flash_out,
								XDY_Picto_PIC			*va_Picto_out,								
								XDY_Entier				*va_NumEvt_out,
								XDY_Octet				*va_CleEvt_out,
								XDY_Octet				*va_Priorite_out,
								XDY_Entier				*va_DistanceEvt_out ,
								XDY_Entier				*va_Action_out,
								XDY_Octet				*va_SiteAction_out);

#endif
