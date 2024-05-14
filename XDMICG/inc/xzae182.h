/*E*/
/*Fichier : $Id: xzae182.h,v 1.1 2008/11/17 10:46:03 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:46:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE182 * FICHIER XZAE182.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	11/10/08	creation DEM 835
------------------------------------------------------ */

#ifndef XZAE182
#define XZAE182

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
#define XZAE182C_RPC_NAME "XZAE182"

#define XZAE182C_NB_PARM_RETURN 6


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

extern int XZAE182_Restrictions_Travaux (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet       va_VR_in,
					XDY_Octet       va_VM2_in,
					XDY_Octet       va_VM1_in,
					XDY_Octet       va_VL_in,
					XDY_Octet       va_BAU_in,
					XDY_Octet       va_VR_I_in,
					XDY_Octet       va_VM2_I_in,
					XDY_Octet       va_VM1_I_in,
					XDY_Octet       va_VL_I_in,
					XDY_Octet       va_BAU_I_in,
					XDY_PR		va_PR_debut_VR_in,
					XDY_PR		va_PR_fin_VR_in,
					XDY_PR		va_PR_debut_VM2_in,
					XDY_PR		va_PR_fin_VM2_in,
					XDY_PR		va_PR_debut_VM1_in,
					XDY_PR		va_PR_fin_VM1_in,
					XDY_PR		va_PR_debut_VL_in,
					XDY_PR		va_PR_fin_VL_in,
					XDY_PR		va_PR_debut_BAU_in,
					XDY_PR		va_PR_fin_BAU_in,
					XDY_PR		va_PR_debut_VR_I_in,
					XDY_PR		va_PR_fin_VR_I_in,
					XDY_PR		va_PR_debut_VM2_I_in,
					XDY_PR		va_PR_fin_VM2_I_in,
					XDY_PR		va_PR_debut_VM1_I_in,
					XDY_PR		va_PR_fin_VM1_I_in,
					XDY_PR		va_PR_debut_VL_I_in,
					XDY_PR		va_PR_fin_VL_I_in,
					XDY_PR		va_PR_debut_BAU_I_in,
					XDY_PR		va_PR_fin_BAU_I_in,
					char            *va_restrictions1_out,
					char      *va_restrictions2_out,
					XDY_Octet 	*va_type_loc_out       ,
					char		*va_nom_loc_out,
					XDY_Entier	*va_num_loc_out,
					char		*va_libelle_type_out);
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
