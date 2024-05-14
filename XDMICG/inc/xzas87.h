/*E*/
/*Fichier : $Id: xzas87.h,v 1.1 2008/11/17 10:42:28 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:42:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS87 * FICHIER XZAS87.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	08/10/08	creation DEM/836
------------------------------------------------------ */

#ifndef XZAS87
#define XZAS87

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
#define XZAS87C_RPC_NAME "XZAS87"

#define XZAS87C_NB_PARM_RETURN 1
/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS87_Donnees_RADT : recupere les donnees RADT
*  pour la station et le sens donnes a l'heure courante
*  Appelle XZAS87SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS87_Niveau_RADT (	XDY_Eqt		va_station_in ,
					XDY_Sens	va_sens_in,
					XDY_Octet	*va_niveau_out);

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
