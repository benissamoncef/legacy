/*E*/
/*Fichier : $Id: xzae62.h,v 1.2 2017/03/31 17:24:26 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2017/03/31 17:24:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE62 * FICHIER XZAE62.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   08/02/16 creation SAGA 1.0
------------------------------------------------------ */

#ifndef XZAE62
#define XZAE62

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
#define XZAE62C_RPC_NAME "XZAE62"

#define XZAE62C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE62_Ecrire_Fiche_Basculement (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_PR	va_PRfin_in,
					XDY_Octet		va_VR_in,
					XDY_Octet	va_VM2_in,
					XDY_Octet	va_VM1_in,
					XDY_Octet	va_VL_in,
					XDY_Octet	va_BAU_in,
					XDY_Octet	va_VR_I_in,
					XDY_Octet	va_VM2_I_in,
					XDY_Octet	va_VM1_I_in,
					XDY_Octet	va_VL_I_in,
					XDY_Octet	va_BAU_I_in,
					XDY_Octet	va_PosteOperateur_in,
					XDY_Octet	vitesse,
					XDY_Octet	vitesse_opposee);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
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
