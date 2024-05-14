/*E*/
/* Fichier : $Id: xzaf15.c,v 1.4 1995/05/29 18:57:14 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/29 18:57:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaf15.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       16/01/95     : Creation
* C.T.       29/05/95     : Ajout fonction sur echangeur
*                           en argument en entree (V 1.3)
* C.T.       29/05/95     : Modif interface XZAF06 (V 1.4)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf.h"

static char *version = "$Id: xzaf15.c,v 1.4 1995/05/29 18:57:14 gaborit Exp $ XZAF15_Animer_Formation";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Animer les synoptiques pour la formation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAF15_Animer_Formation (	XDY_Horodate	va_Horodate,
				XDY_FonctionInt	va_FonctionObjetMobile,
				XDY_FonctionInt	va_FonctionObjetMobileSurEch,
				XDY_FonctionInt va_FonctionBouchon,
				XDY_FonctionInt	va_FonctionRadtInverse,
				XDY_FonctionInt	va_FonctionBouchInverse,
				XDY_FonctionInt	va_FonctionVoiesNeut,
				XDY_FonctionInt	va_FonctionAffectVoies)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate		va_Horodate		: date de la formation
* XDY_FonctionInt	va_FonctionObjetMobile	: pointeur pour retourner objet mobile
* XDY_FonctionInt	va_FonctionObjetMobileSurEch	: pointeur pour retourner objet mobile sur echangeur
* XDY_FonctionInt	va_FonctionBouchon	: pointeur pour retourner bouchon
* XDY_FonctionInt	va_FonctionRadtInverse	: pointeur pour retourner radt inverse
* XDY_FonctionInt	va_FonctionBouchInverse	: pointeur pour retourner bouchon avec basculement
* XDY_FonctionInt	va_FonctionVoiesNeut	: pointeur pour retourner voies neutralisees
* XDY_FonctionInt	va_FonctionAffectVoies	: pointeur pour retourner affectation des voies
*
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK	: sous fonctions retournent XDC_NOK
*  XDC_ARG_INV	: fonctions invalides
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* Appelle XZAF02, XZAF03, XZAF05, XZAF06, XZAF08, XZA10, XZAF11, XZAF14
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "$Id: xzaf15.c,v 1.4 1995/05/29 18:57:14 gaborit Exp $ XZAF15_Animer_Formation";
	
	XDY_Entier	vl_Status;
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAF15_Animer_Formation : Horodate %lf",
			va_Horodate);

	if ((va_FonctionObjetMobile == NULL) || (va_FonctionObjetMobileSurEch == NULL) ||
	   (va_FonctionBouchon == NULL) ||
	   (va_FonctionRadtInverse == NULL) || (va_FonctionBouchInverse == NULL) ||
	   (va_FonctionVoiesNeut == NULL) || (va_FonctionAffectVoies == NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: interface incorrecte ");
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_ARG_INV);
		return XDC_ARG_INV;
	}
	
	/*
	 *A Rechercher les evenements et actions devant etre animes en formation
	 */
	vl_Status = XZAF10_Rechercher_Evts_Actions(va_Horodate);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF10_Rechercher_Evts_Actions retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Animer les objets mobiles
	 */
	vl_Status = XZAF02_Animer_Objet_Mobile(va_Horodate, va_FonctionObjetMobile, va_FonctionObjetMobileSurEch);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF02_Animer_Objet_Mobile retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Animer les feux d'affectation des voies
	 */
	vl_Status = XZAF03_Animer_Neut_Voies(va_FonctionAffectVoies);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF03_Animer_Neut_Voies retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Animer les bouchons
	 */
	vl_Status = XZAF05_Animer_Bouchon(va_FonctionBouchon);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF05_Animer_Bouchon retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Animer les voies neutralisees
	 */
	vl_Status = XZAF06_Animer_Voies_Neutralisees(va_Horodate, va_FonctionVoiesNeut);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF06_Animer_Voies_Neutralisees retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Animer les voies inversees
	 */
	vl_Status = XZAF08_Animer_Voies_RADT_Inversees(va_FonctionRadtInverse);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF08_Animer_Voies_RADT_Inversees retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Animer les bouchons avec basculement
	 */
	vl_Status = XZAF11_Animer_Bouchons_Inverses(va_FonctionBouchInverse);

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF11_Animer_Bouchons_Inverses retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);

		vl_Status = XZAF14_Supprimer_Evts_Actions();
		return XDC_NOK;
	}

	/*
	 *A Supprimer les evenements et actions selectionnees pour la formation
	 */
	vl_Status = XZAF14_Supprimer_Evts_Actions();

	if (vl_Status != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"xzaf15: XZAF14_Supprimer_Evts_Actions retourne %d", 
				vl_Status);
		XZST_03EcritureTrace(XZSTC_INTERFACE,
				"OUT : XZAF15_Animer_Formation retourne %d", 
				XDC_NOK);
		return XDC_NOK;
	}

	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAF15_Animer_Formation retourne %d \n", 
			XDC_OK);
			
 	
	return(XDC_OK);
}
