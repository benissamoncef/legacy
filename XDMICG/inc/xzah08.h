/*E Fichier : $Id: xzah08.h,v 1.3 1996/09/06 11:23:06 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/09/06 11:23:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER XZAH08.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	27 Oct 1994	: Creation
* torregrossa	30 mar 1995	: Suppression du fichier xzah_utils.h
*                                 (V 1.2)
* niepceron	27 Aou 1996	: Ajout du site en arg  v1.3
------------------------------------------------------ */

#ifndef XZAH08
#define XZAH08

/* fichiers inclus */
	/* Include systeme */
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
	#include "xzag.h"
	#include "asql_com.h"
	#include "xzis.h"
	#include "xzahc.h"

	
/* definitions de constantes exportees */

#define XZAH08C_XZAH08_RPC_NAME "XZAH;08"
#define XZAH08C_XZAH08_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*------------------------------------------------------
* SERVICE RENDU : 
*	XZAH08 : Envoie a l IHM les modifications des variables
*  d animation synoptiques
*
*  Appelle XZAH;08 : Lecture et ecriture des synoptiques pour animation
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* PARAMETRES EN ENTREE :
*
* XDY_Booleen	va_Init_in		
* XDY_Nom	va_NomSite		
*
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
*
* Retourne la liste des types de PMV
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* INTERFACE :
*
* Pour chaque ligne du select retournee par la SP, on constitue une chaine,
* que l on envoie a l IHM grace a XZAH08_Envoyer_Objet_Mobile_Aff_Voies.
*
* MECANISMES :
*
------------------------------------------------------*/
extern int XZAH08_Envoyer_Objet_Mobile_Aff_Voies(XDY_Booleen,XDY_Nom);

#endif
