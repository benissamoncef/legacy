/*E*/
/*Fichier : $Id: xzae220.h,v 1.1 2008/11/17 10:47:30 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:47:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE220 * FICHIER XZAE220.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	11/10/08	creation DEM/835
------------------------------------------------------ */

#ifndef XZAE220
#define XZAE220

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
#define XZAE220C_RPC_NAME "XZAE220"

#define XZAE220C_NB_PARM_RETURN 16


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE220_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE220_Lire_Config_Voies (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
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
					XDY_Octet	*va_point_car_out,
					XDY_Entier	*va_num_point_car_out,
					XDY_Octet	*va_nb_entrees_out,
					XDY_Octet	*va_nb_sorties_out);
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
