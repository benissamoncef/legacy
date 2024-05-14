/*E*/
/*  Fichier : $Id: xzac610.h,v 1.2 2020/06/15 08:42:44 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/06/15 08:42:44 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAC * FICHIER xzac610.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module Interface Actions.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/02/13 : Creation (DEM 1061)  1.1
* JMG	15/01/20 : ajout va_Attente_EQT_FIL DEM1347
-----------------------------------------------------------------------------------------*/

#ifndef xzac610_H
#define xzac610_H


/* Definitions de constantes exportees */
/* ----------------------------------- */

#define	XZACC_XZAC610_RPC_NAME	"XZAC610"

#define	XZACC_XZAC610_NB_PARM_RETURN	2



/* Definitions de types exportes */
/* ----------------------------- */



/* Declarations de fonctions externes */
/* ---------------------------------- */

extern int XZAC610_Mise_Au_Neutre_PMV (	XDY_Eqt		va_NumEqt_in,
					XDY_District	va_Site_in,
					XDY_Octet	va_Fin_Action_in,
					XDY_Ligne_PMV	va_Ligne1_TDP_in,
					XDY_Ligne_PMV	va_Ligne2_TDP_in,
					XDY_Ligne_PMV	va_Ligne3_TDP_in,
					XDY_Mot		va_Dest_Proche_in,
					XDY_Mot		va_Dest_Loin_in,
					XDY_Mot		va_Dest_Noeud_in,
					XDY_Entier	*va_Num_Action_out,
					XDY_Octet	*va_Type_Neutre_out ,
					XDY_Octet	va_Attente_EQT_FIL);

#endif
