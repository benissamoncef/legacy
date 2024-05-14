/*E*/
/*Fichier : $Id: xzaz01_test.c,v 1.1 1995/01/18 11:39:34 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/18 11:39:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzaz01_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzaz01.c
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	17/01/1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaz.h"
#include "xzae.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaz01_test.c,v 1.1 1995/01/18 11:39:34 torregrossa Exp $ XZAZ01_Ecrire_Travaux_Satir";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAZ01_Ecrire_Travaux_Satir
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* CONDITION D'UTILISATION
*	On teste :
*	XZAZ01_Ecrire_Travaux_Satir() 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	int		vl_Status;
	XDY_Horodate	vl_NbSecondes;
	XDY_Horodate	vl_DebutPrevu;
	XDY_Horodate	vl_Debut;
	XDY_Horodate	vl_FinPrevu;
	XZAZT_Travaux	vl_Travaux;
	

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	vl_Status = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n", vl_Status);


	printf("\n\n**** Test 1 ****\n\n");

	/*A
	 * Rechercher le nombre de secondes
	 */
	vl_Status = XZSM_13ConversionHorodateSec("19/02/1995 18:00:00", 
						 (double*)&vl_DebutPrevu);
	vl_Status = XZSM_13ConversionHorodateSec("20/01/1995 18:00:00", 
						 (double*)&vl_Debut);
	vl_Status = XZSM_13ConversionHorodateSec("19/02/1995 19:00:00", 
						 (double*)&vl_FinPrevu);

	/*
	 *A initialisation des travaux en entree
	 */ 
	vl_Travaux.DebutPrevu	= vl_DebutPrevu;
	vl_Travaux.Debut	= 0;
	vl_Travaux.FinPrevue	= vl_FinPrevu;
	vl_Travaux.Autoroute	= 1;
	vl_Travaux.PR		= 65000;
	vl_Travaux.Sens		= XDC_SENS_SUD;
	vl_Travaux.VR		= 1;
	vl_Travaux.VM2		= 4;
	vl_Travaux.VM1		= 4;
	vl_Travaux.VL		= 1;
	vl_Travaux.BAU		= 7;
	vl_Travaux.VR_I		= 1;
	vl_Travaux.VM2_I	= 4;
	vl_Travaux.VM1_I	= 4;
	vl_Travaux.VL_I		= 1;
	vl_Travaux.BAU_I	= 7;
	strcpy(vl_Travaux.Texte, "commentaire") ;
	vl_Travaux.Nature	= XZAEC_TRV_Entretien;
	vl_Travaux.Type		= XZAEC_TRV_Vegetation;
	vl_Travaux.Continu	= XDC_NON;
	vl_Travaux.HoroDebC	= 0;
	vl_Travaux.HoroFinC	= 0;
	vl_Travaux.Mobile	= XDC_NON;
	vl_Travaux.PRqueueM	= 0;
	vl_Travaux.PRteteM	= 0;
	vl_Travaux.SensM	= XDC_SENS_SUD;
	vl_Travaux.PRtete	= 67000;
	vl_Travaux.VoiesRet	= XDC_NON;

	/*A
	** Tester avec des travaux prevus valides
	*/
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"xzaz01 : XZAZ01_Ecrire_Travaux_Satir : DebutPrevu=%lf Debut=%lf \n FinPrevue=%lf \n Autoroute=%d \n PR=%ld Sens=%d \n VR=%d VM2=%d VM1=%d \n VL=%d BAU=%d VR_I=%d \n VM2_I=%d VM1_I=%d VL_I=%d BAU_I=%d Texte=%s \n Nature=%d Type%d  Continu=%d \n HoroDebC=%lf HoroFinC=%lf Mobile=%d \n PRqueueM=%ld PRteteM=%ld \n SensM=%d PRtete=%ld VoiesRet=%d\n",
			vl_Travaux.DebutPrevu, vl_Travaux.Debut, 
			vl_Travaux.FinPrevue, vl_Travaux.Autoroute,
			vl_Travaux.PR, vl_Travaux.Sens, vl_Travaux.VR,
			vl_Travaux.VM2, vl_Travaux.VM1, vl_Travaux.VL, 
			vl_Travaux.BAU, vl_Travaux.VR_I, vl_Travaux.VM2_I,
			vl_Travaux.VM1_I, vl_Travaux.VL_I, vl_Travaux.BAU_I,
			vl_Travaux.Texte, vl_Travaux.Nature, vl_Travaux.Type,
			vl_Travaux.Continu, vl_Travaux.HoroDebC, vl_Travaux.HoroFinC,
			vl_Travaux.Mobile, vl_Travaux.PRqueueM,
			vl_Travaux.PRteteM, vl_Travaux.SensM, vl_Travaux.PRtete,
			vl_Travaux.VoiesRet);

	vl_Status = XZAZ01_Ecrire_Travaux_Satir(vl_Travaux);
	printf("XZAZ01_Ecrire_Travaux_Satir() : code retourne %d \n", 
		vl_Status);
	

	/*A
	** Tester avec des travaux prevus invalides
	*/
	vl_Status = XZSM_13ConversionHorodateSec("17/01/1995 18:00:00", 
						 (double*)&vl_DebutPrevu);
	vl_Travaux.DebutPrevu	= vl_DebutPrevu;

	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"xzaz01 : XZAZ01_Ecrire_Travaux_Satir : DebutPrevu=%lf Debut=%lf \n FinPrevue=%lf \n Autoroute=%d \n PR=%ld Sens=%d \n VR=%d VM2=%d VM1=%d \n VL=%d BAU=%d VR_I=%d \n VM2_I=%d VM1_I=%d VL_I=%d BAU_I=%d Texte=%s \n Nature=%d Type%d  Continu=%d \n HoroDebC=%lf HoroFinC=%lf Mobile=%d \n PRqueueM=%ld PRteteM=%ld \n SensM=%d PRtete=%ld VoiesRet=%d\n",
			vl_Travaux.DebutPrevu, vl_Travaux.Debut, 
			vl_Travaux.FinPrevue, vl_Travaux.Autoroute,
			vl_Travaux.PR, vl_Travaux.Sens, vl_Travaux.VR,
			vl_Travaux.VM2, vl_Travaux.VM1, vl_Travaux.VL, 
			vl_Travaux.BAU, vl_Travaux.VR_I, vl_Travaux.VM2_I,
			vl_Travaux.VM1_I, vl_Travaux.VL_I, vl_Travaux.BAU_I,
			vl_Travaux.Texte, vl_Travaux.Nature, vl_Travaux.Type,
			vl_Travaux.Continu, vl_Travaux.HoroDebC, vl_Travaux.HoroFinC,
			vl_Travaux.Mobile, vl_Travaux.PRqueueM,
			vl_Travaux.PRteteM, vl_Travaux.SensM, vl_Travaux.PRtete,
			vl_Travaux.VoiesRet);

	vl_Status = XZAZ01_Ecrire_Travaux_Satir(vl_Travaux);
	printf("XZAZ01_Ecrire_Travaux_Satir() : code retourne %d \n", 
		vl_Status);
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}
