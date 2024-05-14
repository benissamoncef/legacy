/*E*/
/*Fichier : $Id: xzae185.h,v 1.1 2008/11/17 10:46:28 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date : $Date: 2008/11/17 10:46:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae185.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la liste des dernieres FMC modifiees
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou       07 05 1997     : Creation DEM / 835
------------------------------------------------------*/

/* fichiers inclus */

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
#define XZAE185C_XZAE185_RPC_NAME "XZAE185"

#define XZAE185C_NB_PARM_RETURN 0




	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE185SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE185_Recherche_Dernieres_FMC ( XDY_FonctionInt pa_FonctionUtilisateur_in,
				      XDY_Entier 	*va_Resultat_out );

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Fonction  pa_FonctionUtilisateur_in
*				
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* retourne la liste des fmc a exporter vers strada
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Pour chaque zone de parcours appel de la fonction utilisateur avec XDY_Destination
*
* MECANISMES :
*
------------------------------------------------------*/
