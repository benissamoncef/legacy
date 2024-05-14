/*E*/
/*  Fichier : $Id: xzac840.h,v 1.2 2020/06/15 08:43:15 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/06/15 08:43:15 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAC * FICHIER xzac840.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module Interface Actions.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	23/02/13 : Creation (DEM 1061)  1.1
* JMG	16/01/20 : ajout va_Attente_EQT_FIL 1.2 DEM1347
-----------------------------------------------------------------------------------------*/

#ifndef xzac840_H
#define xzac840_H


/* Definitions de constantes exportees */
/* ----------------------------------- */

#define	XZACC_XZAC840_RPC_NAME	"XZAC840"

#define	XZACC_XZAC840_NB_PARM_RETURN	2



/* Definitions de types exportes */
/* ----------------------------- */



/* Declarations de fonctions externes */
/* ---------------------------------- */

extern int XZAC840_Mise_Au_Neutre_PMVA (	XDY_Eqt		va_NumEqt_in,
						XDY_District	va_Site_in,
						XDY_Octet	va_Fin_Action_in,
						XDY_Ligne_PMV	va_Ligne1_TDP_in,
						XDY_Ligne_PMV	va_Ligne2_TDP_in,
						XDY_Ligne_PMV	va_Ligne3_TDP_in,
						XDY_Ligne_PMV	va_Ligne4_TDP_in,
						XDY_Ligne_PMV	va_Ligne5_TDP_in,
						XDY_Mot		va_Dest1_Proche_in,
						XDY_Mot		va_Dest1_Loin_in,
						XDY_Mot		va_Dest2_Proche_in,
						XDY_Mot		va_Dest2_Loin_in,
						XDY_Entier	*va_Num_Action_out,
						XDY_Octet	*va_Type_Neutre_out,
						XDY_Octet	va_Attente_EQT_FIL);

#endif
