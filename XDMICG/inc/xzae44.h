/*E*/
/*Fichier : @(#)xzae44.h	1.3      Release : 1.3        Date : 03/26/07
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE44 * FICHIER XZAE44.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------ */

#ifndef XZAE44
#define XZAE44

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
#define XZAE44C_RPC_NAME "XZAE;44"

#define XZAE44C_NB_PARM_RETURN 7


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE44_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE44_Lire_Fiche_Meteo (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_PR	*va_PRfin_out,
					XDY_Octet *va_perturbation_out,
					XDY_Mot *va_visibilite_out,
					XDY_Entier *va_longueur_out,
					XDY_Octet *va_nature_out,
					XDY_Nom	va_libelle_nature_out);

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
