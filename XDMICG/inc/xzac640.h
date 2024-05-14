/*E*/
/* Fichier : $Id: xzac640.h,v 1.2 2018/05/29 08:30:21 devgfi Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/05/29 08:30:21 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAC * FICHIER xzac640.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module Interface Actions.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	30/03/17 : Creation (DEM 1221)  1.1
* JPL	09/04/18 : Ajout parametres: fin d'action en cours; en sortie Action et Type neutre  1.2
*
-----------------------------------------------------------------------------------------*/

#ifndef xzac640_H
#define xzac640_H


/* Definitions de constantes exportees */
/* ----------------------------------- */

#define	XZACC_XZAC640_RPC_NAME	"XZAC640"

#define	XZACC_XZAC640_NB_PARM_RETURN	2



/* Definitions de types exportes */
/* ----------------------------- */



/* Declarations de fonctions externes */
/* ---------------------------------- */

extern int XZAC640_Mise_Au_Neutre_PMVx_SAGA (	XDY_Eqt		va_NumEqt_in,
						XDY_District	va_Site_in,
						char		*va_Texte_Affiche_in,
						XDY_Octet	va_Fin_Action_in,
						char		*va_Ligne1_TDP_in,
						char		*va_Ligne2_TDP_in,
						char		*va_Ligne3_TDP_in,
						char		*va_Ligne4_TDP_in,
						char		*va_Ligne5_TDP_in,
						char		*va_Alternat1_in,
						char		*va_Alternat2_in,
						char		*va_Alternat3_in,
						char		*va_Alternat4_in,
						char		*va_Alternat5_in,
						XDY_Entier	*va_NumeroAction_out,
						XDY_Octet	*va_Type_Neutre_out );

#endif
