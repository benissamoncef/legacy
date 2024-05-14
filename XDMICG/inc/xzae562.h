/*E*/
/* Fichier : $Id: xzae562.h,v 1.1 2012/07/04 10:55:54 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:55:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE562 * FICHIER XZAE562.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZAE562
#define XZAE562

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
#define XZAE562C_RPC_NAME "XZAE562"

#define XZAOC_XZAE562_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE562_Bouchon_Conseq : nb de bouchons consequence
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
					
extern int XZAE562_Evts_impactants_En_Cours( XDY_Horodate 	va_Horodate_in, 
									 XDY_Autoroute 	va_Autoroute_in,
									 XDY_Sens 		va_Sens_in, 
									 XDY_PR 		va_PR_Debut_in,
									 XDY_PR 		va_PR_Fin_in,
									 XDY_FonctionInt pa_FonctionUtilisateur_in
									);

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
