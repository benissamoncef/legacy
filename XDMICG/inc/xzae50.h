/*E*/
/*Fichier : $Id: xzae50.h,v 1.1 2008/11/17 10:52:18 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:52:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE50 * FICHIER XZAE50.h
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
------------------------------------------------------ */

#ifndef XZAE50
#define XZAE50

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
#define XZAE50C_RPC_NAME "XZAE;50"

#define XZAE50C_NB_PARM_RETURN 4


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE50_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE50_Lire_Fiche_SousCnc (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Nom		va_nom_out,
					XDY_Booleen	*va_ferme_out,
					XDY_Booleen	*va_penurie_out,
					XDY_Octet	*va_carburant_out);

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
