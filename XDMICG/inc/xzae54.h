/*E*/
/*Fichier : $Id: xzae54.h,v 1.2 2017/03/31 17:23:13 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2017/03/31 17:23:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE54 * FICHIER XZAE54.h
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

#ifndef XZAE54
#define XZAE54

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
#define XZAE54C_RPC_NAME "XZAE54"

#define XZAE54C_NB_PARM_RETURN 1


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

extern int XZAE54_Ecrire_Fiche_Vehicule (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet	va_Type_in,
					XDY_Octet	va_Passagers_in,
					XDY_Booleen	va_Animaux_in,
					XDY_Booleen	va_VehEscota_in,
					XDY_Octet	va_Chargement_in,
					char		*va_CodeMat_in,
					char		*va_Texte_in,
					XDY_Mot		va_NumeroVehicule_in,
					XDY_Octet	va_site_creation_in,
					XDY_Mot		va_nb_bebes_in,
					XDY_Mot		va_nb_agees_in,
					XDY_Octet	va_feu_maitrise_in,
					XDY_Octet	va_type_rappel_in,
					char		*va_complement_rappel_in);

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
