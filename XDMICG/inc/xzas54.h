/*E*/
/*Fichier : $Id: xzas54.h,v 1.3 1997/12/03 16:04:25 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1997/12/03 16:04:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS54 * FICHIER XZAS54.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Vathelot	02 Mai 1997	: Creation (dem/strada) 1.1
* Penot		07 Oct 1997	: correction du nom de la proc XZAS;54 1.2
------------------------------------------------------ */

#ifndef XZAS54
#define XZAS54

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
#define XZAS54C_RPC_NAME "XZAS54"

#define XZAS54C_NB_PARM_RETURN 3
/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS54_Donnees_RADT : recupere les donnees RADT
*  pour la station et le sens donnes a l'heure courante
*  Appelle XZAS54SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS54_Donnees_RADT (	XDY_Eqt		va_station_in ,
					XDY_Sens	va_sens_in,
					XDY_Entier	*va_Q_out,
					XDY_Entier	*va_V_out,
					XDY_Entier	*va_TO_out);

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
