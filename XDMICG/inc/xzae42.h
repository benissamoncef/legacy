/*E*/
/*Fichier : $Id: xzae42.h,v 1.2 2009/05/25 17:52:19 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/05/25 17:52:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE42 * FICHIER XZAE42.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JMG   11/10/08        CREATION        DEM 835
* JMG 	02/12/08	ajout PR en sortie DEM 835 
------------------------------------------------------ */

#ifndef XZAE42
#define XZAE42

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
#define XZAE42C_RPC_NAME "XZAE;42"

#define XZAE42C_NB_PARM_RETURN 5


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE42_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE42_Lire_Fiche_Delestage (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					char       *va_nomPlan_out,
					XDY_Entier       *va_numPlan_out,
					char       *va_hierarchie_out,
					XDY_PR	*va_PR,
					XDY_PR	*va_PR2);

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
