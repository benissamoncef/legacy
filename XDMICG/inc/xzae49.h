/*E*/
/*Fichier : $Id: xzae49.h,v 1.4 2020/06/02 21:19:32 devgfi Exp $      $Revision: 1.4 $        $Date: 2020/06/02 21:19:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE49 * FICHIER XZAE49.h
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
* VR	07/10/11	Ajout vitesse		(DEM/1015)
* JMG	08/03/18	ajout personnel 1.3
* JPL	06/09/19	Ajout bouchon (mobile pour Basculement pour travaux) (DEM 1346)  1.4
------------------------------------------------------ */

#ifndef XZAE49
#define XZAE49

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
#define XZAE49C_RPC_NAME "XZAE49"

#define XZAE49C_NB_PARM_RETURN 67


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE49_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE49_Lire_Fiche_Travaux (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Booleen	*va_nature_out,
					XDY_Octet      *va_type_out,
					XDY_Booleen	*va_continu_out,
					XDY_Horodate	*va_horodebc_out,
					XDY_Horodate	*va_horofinc_out,
					XDY_Booleen       *va_mobile_out,
					XDY_Autoroute       *va_autoroute_out,
					XDY_PR       *va_prtetem_out,
					XDY_PR       *va_prqueuem_out,
					XDY_Sens       *va_sensm_out,
					XDY_PR       *va_prtete_out,
					XDY_Booleen       *va_voiesret_out,
					XDY_Entier       *va_longueur_out,
					XDY_Entier       *va_longueurm_out,
					XDY_Octet       *va_VR_out,
					XDY_Octet       *va_VM2_out,
					XDY_Octet       *va_VM1_out,
					XDY_Octet       *va_VL_out,
					XDY_Octet       *va_BAU_out,
					XDY_Octet       *va_VR_I_out,
					XDY_Octet       *va_VM2_I_out,
					XDY_Octet       *va_VM1_I_out,
					XDY_Octet       *va_VL_I_out,
					XDY_Octet       *va_BAU_I_out,
					XDY_PR		*va_prdebut_vr_out,
					XDY_PR		*va_prfin_vr_out,
					XDY_Horodate	*va_debut_vr_out,
					XDY_Horodate	*va_fin_vr_out,
					XDY_PR		*va_prdebut_vm2_out,
					XDY_PR		*va_prfin_vm2_out,
					XDY_Horodate	*va_debut_vm2_out,
					XDY_Horodate	*va_fin_vm2_out,
					XDY_PR		*va_prdebut_vm1_out,
					XDY_PR		*va_prfin_vm1_out,
					XDY_Horodate	*va_debut_vm1_out,
					XDY_Horodate	*va_fin_vm1_out,
					XDY_PR		*va_prdebut_vl_out,
					XDY_PR		*va_prfin_vl_out,
					XDY_Horodate	*va_debut_vl_out,
					XDY_Horodate	*va_fin_vl_out,
					XDY_PR		*va_prdebut_bau_out,
					XDY_PR		*va_prfin_bau_out,
					XDY_Horodate	*va_debut_bau_out,
					XDY_Horodate	*va_fin_bau_out,
					XDY_PR		*va_prdebut_vr_i_out,
					XDY_PR		*va_prfin_vr_i_out,
					XDY_Horodate	*va_debut_vr_i_out,
					XDY_Horodate	*va_fin_vr_i_out,
					XDY_PR		*va_prdebut_vm2_i_out,
					XDY_PR		*va_prfin_vm2_i_out,
					XDY_Horodate	*va_debut_vm2_i_out,
					XDY_Horodate	*va_fin_vm2_i_out,
					XDY_PR		*va_prdebut_vm1_i_out,
					XDY_PR		*va_prfin_vm1_i_out,
					XDY_Horodate	*va_debut_vm1_i_out,
					XDY_Horodate	*va_fin_vm1_i_out,
					XDY_PR		*va_prdebut_vl_i_out,
					XDY_PR		*va_prfin_vl_i_out,
					XDY_Horodate	*va_debut_vl_i_out,
					XDY_Horodate	*va_fin_vl_i_out,
					XDY_PR		*va_prdebut_bau_i_out,
					XDY_PR		*va_prfin_bau_i_out,
					XDY_Horodate	*va_debut_bau_i_out,
					XDY_Horodate	*va_fin_bau_i_out,
					XDY_Entier	*va_vitesse_out,
					XDY_Octet	*va_personnel_out,
					XDY_Octet	*va_bouchon_out);
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
