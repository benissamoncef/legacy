/*E*/
/*Fichier : $Id: xzae48.h,v 1.2 2011/12/06 15:41:41 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/12/06 15:41:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE48 * FICHIER XZAE48.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
* VR	07/10/11	Ajout Vitesse et Vitesse_opposee (DEM/1015)
------------------------------------------------------ */

#ifndef XZAE48
#define XZAE48

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
#define XZAE48C_RPC_NAME "XZAE48"

#define XZAE48C_NB_PARM_RETURN 17


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE48_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE48_Lire_Fiche_Basculement (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_PR  *va_PR_out,
					XDY_Entier      *va_longueur_out,
					XDY_Octet       *va_VR_out,
					XDY_Octet       *va_VM2_out,
					XDY_Octet       *va_VM1_out,
					XDY_Octet       *va_VL_out,
					XDY_Octet       *va_BAU_out,
					XDY_Octet       *va_VR_I_out,
					XDY_Octet       *va_VM2_I_out,
					XDY_Octet       *va_VM1_I_out,
					XDY_Octet       *va_VL_I_out,
					XDY_Octet       *va_BAU_I_out,
					XDY_Entier	*vitesse,
					XDY_Entier	*vitesse_opposee);
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
