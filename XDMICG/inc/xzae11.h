/*E*/
/* Fichier : $Id: xzae11.h,v 1.4 2019/05/13 10:38:06 devgfi Exp $        $Revision: 1.4 $        $Date: 2019/05/13 10:38:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE11 * FICHIER XZAE11.h
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
* JPL	08/04/19 Ajout de l'indicateur d'écoulement en paramètre (DEM 1326)  1.4
------------------------------------------------------ */

#ifndef XZAE11
#define XZAE11

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
#define XZAE11C_RPC_NAME "XZAE11"

#define XZAE11C_NB_PARM_RETURN 1


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

extern int XZAE11_Valider_Fiche_MC (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Mot		va_PosteOperateur_in,
					XDY_Octet	va_Prevision_Traiter_Clos_in,
					XDY_Mot		va_Type_in,
					XDY_Horodate	va_DebutPrevu_in,
					XDY_Horodate	va_Debut_in,
					XDY_Horodate	va_FinPrevu_in,
					XDY_Horodate	va_Fin_in,
					XDY_Horodate	va_Validation_in,
					XDY_Entier	va_NumCause_in,
					XDY_Octet	va_CleCause_in,
					XDY_Entier	va_NumAlerte_in,
					char		*va_Origine_in,
					XDY_Mot		va_PosteEnrich_in,
					XDY_Booleen	va_ConfirmeSignale_in,
					XDY_Autoroute	va_NumAutoroute_in,
					XDY_PR		va_PR_in,
					XDY_Sens	va_Sens_in,
					XDY_Octet	va_PtCaracteristique_in,
					XDY_Octet	va_TypePtCaracteristique_in,
					XDY_Octet	va_Position_in,
					XDY_Octet	va_VR_in,
					XDY_Octet	va_VM2_in,
					XDY_Octet	va_VM1_in,
					XDY_Octet	va_VL_in,
					XDY_Octet	va_BAU_in,
					XDY_Octet	va_VR_I_in,
					XDY_Octet	va_VM2_I_in,
					XDY_Octet	va_VM1_I_in,
					XDY_Octet	va_VL_I_in,
					XDY_Octet	va_BAU_I_in,
					XDY_Octet	va_BAU_Etroite_in,
					XDY_Octet	va_BAU_I_Etroite_in,
					XDY_Octet	va_Degats_in,
					XDY_Mot		va_TypeAnterieur_in,
					XDY_Octet	va_FausseAlerte_in,
					XDY_Entier	va_Longueur_in,
					char		*va_ComEvtInit_in,
					XDY_Octet	va_SiteAlerte_in,
					XDY_Octet	va_Majeur_in,
					XDY_Octet	va_Ecoulement_in);

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
