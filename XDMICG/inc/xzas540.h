/*E*/
/* Fichier : $Id: xzas540.h,v 1.1 2012/07/04 11:01:22 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:01:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS54 * FICHIER XZAS540.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	16 Mai 2012	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZAS540
#define XZAS540

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
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAS540C_RPC_NAME "XZAS540"

#define XZAS540C_NB_PARM_RETURN 6
/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*  XZAS540_Liste_Donnees_Trafic : recupere les donnees trafic
*  pour la station et le sens donnes a l'heure courante
*  Appelle XZAS540SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS540_Liste_Donnees_Trafic (	XDY_Eqt			va_station_in ,
											XDY_Sens		va_sens_in,
											XDY_Horodate	va_horodate_in,
											XDY_Entier		*va_Q_out,
											XDY_Entier		*va_V_out,
											XDY_Entier		*va_TO_out,
											XDY_Octet		*va_validQ_out,
											XDY_Octet		*va_validV_out,
											XDY_Octet		*va_validTO_out);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_station_in
* XDY_Sens		va_sens_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
#endif
