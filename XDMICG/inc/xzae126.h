/*E*/
/*Fichier : $Id: xzae126.h,v 1.3 1997/11/12 15:27:27 verdier Exp $     Release : $Revision: 1.3 $      Date : $Date: 1997/11/12 15:27:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae126.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la liste des dernieres FMC modifiees
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou       07 05 1997     : Creation
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
#define XZAE126C_XZAE126_RPC_NAME "XZAE126"

#define XZAE126C_NB_PARM_RETURN 0




	
	
/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAE126SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE126_Recherche_Dernieres_FMC ( XDY_FonctionInt pa_FonctionUtilisateur_in,
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
