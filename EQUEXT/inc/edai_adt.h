/*E*/
/* Fichier : $Id: edai_adt.h,v 1.2 2011/11/30 19:14:59 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/11/30 19:14:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_adt.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier gestion alerte DAI Tunnels de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Nov 1996	: Creation pour alerte DAI tunnel (DEM/1309)
* JPL		version 1.2	30/11/2011	: Declaration fonction "ec_lire_config_Alerte_Tunnel"
*
------------------------------------------------------ */

#ifndef edai_adt
#define edai_adt

/* fichiers inclus */

#include "edai_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int ec_lire_config_Alerte_Tunnel ( XDY_NomMachine pa_NomMachine );



void ec_detect_Alerte_DAI_Tunnel( EDAI_DONNEES_ANALYSEUR *	pa_DonneeAnalyseur,
				  XDY_Horodate			va_Horodate  );

#endif
