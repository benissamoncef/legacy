/*E*/
/*Fichier : $Id: xzae61.h,v 1.4 2020/06/02 21:18:53 devgfi Exp $     $Revision: 1.4 $        $Date: 2020/06/02 21:18:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE61 * FICHIER XZAE61.h
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
 JMG	08/0£3/18	ajout personnel 1.3
* JPL	09/09/19	Ajout indicateur de bouchon mobile pour Basculements pour travaux (DEM 1346)  1.4
------------------------------------------------------ */

#ifndef XZAE61
#define XZAE61

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
#define XZAE61C_RPC_NAME "XZAE61"

#define XZAE61C_NB_PARM_RETURN 1


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

extern int XZAE61_Ecrire_Fiche_Travaux (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Booleen	va_Nature_in,
					XDY_Octet		va_Type_in,
					XDY_Octet	va_Continu_in,
					XDY_Horodate	va_HoroDebC_in,
					XDY_Horodate	va_HoroFinC_in,
					XDY_Booleen	va_Mobile_in,
					XDY_Autoroute	va_Autoroute_in,
					XDY_PR		va_PRqueueM_in,
					XDY_PR		va_PRteteM_in,
					XDY_Sens	va_SensM_in,
					XDY_PR		va_PRtete_in,
					XDY_Booleen	va_VoiesRet_in,
					XDY_Horodate	va_Horodate_in,
					XDY_Mot	va_PosteOperateur_in,
					XDY_Octet	va_Prevision_Traiter_Clos_in,
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
					XDY_PR		va_PRdebut_VR_in,
					XDY_PR		va_PRfin_VR_in,
					XDY_Horodate	va_debut_VR_in,
					XDY_Horodate	va_fin_VR_in,
					XDY_PR		va_PRdebut_VM2_in,
					XDY_PR		va_PRfin_VM2_in,
					XDY_Horodate	va_debut_VM2_in,
					XDY_Horodate	va_fin_VM2_in,
					XDY_PR		va_PRdebut_VM1_in,
					XDY_PR		va_PRfin_VM1_in,
					XDY_Horodate	va_debut_VM1_in,
					XDY_Horodate	va_fin_VM1_in,
					XDY_PR		va_PRdebut_VL_in,
					XDY_PR		va_PRfin_VL_in,
					XDY_Horodate	va_debut_VL_in,
					XDY_Horodate	va_fin_VL_in,
					XDY_PR		va_PRdebut_BAU_in,
					XDY_PR		va_PRfin_BAU_in,
					XDY_Horodate	va_debut_BAU_in,
					XDY_Horodate	va_fin_BAU_in,
					XDY_PR		va_PRdebut_VR_I_in,
					XDY_PR		va_PRfin_VR_I_in,
					XDY_Horodate	va_debut_VR_I_in,
					XDY_Horodate	va_fin_VR_I_in,
					XDY_PR		va_PRdebut_VM2_I_in,
					XDY_PR		va_PRfin_VM2_I_in,
					XDY_Horodate	va_debut_VM2_I_in,
					XDY_Horodate	va_fin_VM2_I_in,
					XDY_PR		va_PRdebut_VM1_I_in,
					XDY_PR		va_PRfin_VM1_I_in,
					XDY_Horodate	va_debut_VM1_I_in,
					XDY_Horodate	va_fin_VM1_I_in,
					XDY_PR		va_PRdebut_VL_I_in,
					XDY_PR		va_PRfin_VL_I_in,
					XDY_Horodate	va_debut_VL_I_in,
					XDY_Horodate	va_fin_VL_I_in,
					XDY_PR		va_PRdebut_BAU_I_in,
					XDY_PR		va_PRfin_BAU_I_in,
					XDY_Horodate	va_debut_BAU_I_in,
					XDY_Horodate	va_fin_BAU_I_in,
					XDY_Octet	        vitesse ,
					XDY_Octet	personnel,
					XDY_Octet	va_bouchon_in);

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
