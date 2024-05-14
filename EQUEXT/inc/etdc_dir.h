/*E*/
/* Fichier : $Id: etdc_dir.h,v 1.6 2009/10/08 17:46:53 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2009/10/08 17:46:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETDC * FICHIER etdc_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier directeur de la
*   tache TERDT 
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	14 Jan 1995	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
* Volcic.F	version 1.3	20 Jun 1995	: Ajout definitions
* Volcic.F	version 1.4	27 Jun 1995	: Ajout definitions
* JPL		08/10/09 : Dimensions structures donnees par constantes ERDT (pour DEM 909) 1.6
------------------------------------------------------ */

#ifndef etdc_dir
#define etdc_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etdc_tra.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ETDCT_VAR	pg_variables[]=
                {
			/* Vitesse Sens 1 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"V_S1_V0_T1",0.0},
			{"V_S1_V0_T0",0.0},
			{"V_S1_V1_T1",0.0},
			{"V_S1_V1_T0",0.0},
			{"V_S1_V2_T1",0.0},
			{"V_S1_V2_T0",0.0},
			{"V_S1_V3_T1",0.0},
			{"V_S1_V3_T0",0.0},
			{"V_S1_V4_T1",0.0},
			{"V_S1_V4_T0",0.0},
			/* Vitesse Sens 2 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"V_S2_V0_T1",0.0},
			{"V_S2_V0_T0",0.0},
			{"V_S2_V1_T1",0.0},
			{"V_S2_V1_T0",0.0},
			{"V_S2_V2_T1",0.0},
			{"V_S2_V2_T0",0.0},
			{"V_S2_V3_T1",0.0},
			{"V_S2_V3_T0",0.0},
			{"V_S2_V4_T1",0.0},
			{"V_S2_V4_T0",0.0},
			/* Vitesse Sens 3 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"V_S3_V0_T1",0.0},
			{"V_S3_V0_T0",0.0},
			{"V_S3_V1_T1",0.0},
			{"V_S3_V1_T0",0.0},
			{"V_S3_V2_T1",0.0},
			{"V_S3_V2_T0",0.0},
			{"V_S3_V3_T1",0.0},
			{"V_S3_V3_T0",0.0},
			{"V_S3_V4_T1",0.0},
			{"V_S3_V4_T0",0.0},
			/* Vitesse Sens 4 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"V_S4_V0_T1",0.0},
			{"V_S4_V0_T0",0.0},
			{"V_S4_V1_T1",0.0},
			{"V_S4_V1_T0",0.0},
			{"V_S4_V2_T1",0.0},
			{"V_S4_V2_T0",0.0},
			{"V_S4_V3_T1",0.0},
			{"V_S4_V3_T0",0.0},
			{"V_S4_V4_T1",0.0},
			{"V_S4_V4_T0",0.0},
			/* Vitesse Sens 7 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"V_S7_V0_T1",0.0},
			{"V_S7_V0_T0",0.0},
			{"V_S7_V1_T1",0.0},
			{"V_S7_V1_T0",0.0},
			{"V_S7_V2_T1",0.0},
			{"V_S7_V2_T0",0.0},
			{"V_S7_V3_T1",0.0},
			{"V_S7_V3_T0",0.0},
			{"V_S7_V4_T1",0.0},
			{"V_S7_V4_T0",0.0},
			/* Vitesse Sens 8 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"V_S8_V0_T1",0.0},
			{"V_S8_V0_T0",0.0},
			{"V_S8_V1_T1",0.0},
			{"V_S8_V1_T0",0.0},
			{"V_S8_V2_T1",0.0},
			{"V_S8_V2_T0",0.0},
			{"V_S8_V3_T1",0.0},
			{"V_S8_V3_T0",0.0},
			{"V_S8_V4_T1",0.0},
			{"V_S8_V4_T0",0.0},
			/* Debit de vehicules Sens 1 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"Q_S1_V0_T1",0.0},
			{"Q_S1_V0_T0",0.0},
			{"Q_S1_V1_T1",0.0},
			{"Q_S1_V1_T0",0.0},
			{"Q_S1_V2_T1",0.0},
			{"Q_S1_V2_T0",0.0},
			{"Q_S1_V3_T1",0.0},
			{"Q_S1_V3_T0",0.0},
			{"Q_S1_V4_T1",0.0},
			{"Q_S1_V4_T0",0.0},
			/* Debit de vehicules Sens 2 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"Q_S2_V0_T1",0.0},
			{"Q_S2_V0_T0",0.0},
			{"Q_S2_V1_T1",0.0},
			{"Q_S2_V1_T0",0.0},
			{"Q_S2_V2_T1",0.0},
			{"Q_S2_V2_T0",0.0},
			{"Q_S2_V3_T1",0.0},
			{"Q_S2_V3_T0",0.0},
			{"Q_S2_V4_T1",0.0},
			{"Q_S2_V4_T0",0.0},
			/* Debit de vehicules Sens 3 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"Q_S3_V0_T1",0.0},
			{"Q_S3_V0_T0",0.0},
			{"Q_S3_V1_T1",0.0},
			{"Q_S3_V1_T0",0.0},
			{"Q_S3_V2_T1",0.0},
			{"Q_S3_V2_T0",0.0},
			{"Q_S3_V3_T1",0.0},
			{"Q_S3_V3_T0",0.0},
			{"Q_S3_V4_T1",0.0},
			{"Q_S3_V4_T0",0.0},
			/* Debit de vehicules Sens 4 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"Q_S4_V0_T1",0.0},
			{"Q_S4_V0_T0",0.0},
			{"Q_S4_V1_T1",0.0},
			{"Q_S4_V1_T0",0.0},
			{"Q_S4_V2_T1",0.0},
			{"Q_S4_V2_T0",0.0},
			{"Q_S4_V3_T1",0.0},
			{"Q_S4_V3_T0",0.0},
			{"Q_S4_V4_T1",0.0},
			{"Q_S4_V4_T0",0.0},
			/* Debit de vehicules Sens 7 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"Q_S7_V0_T1",0.0},
			{"Q_S7_V0_T0",0.0},
			{"Q_S7_V1_T1",0.0},
			{"Q_S7_V1_T0",0.0},
			{"Q_S7_V2_T1",0.0},
			{"Q_S7_V2_T0",0.0},
			{"Q_S7_V3_T1",0.0},
			{"Q_S7_V3_T0",0.0},
			{"Q_S7_V4_T1",0.0},
			{"Q_S7_V4_T0",0.0},
			/* Debit de vehicules Sens 8 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"Q_S8_V0_T1",0.0},
			{"Q_S8_V0_T0",0.0},
			{"Q_S8_V1_T1",0.0},
			{"Q_S8_V1_T0",0.0},
			{"Q_S8_V2_T1",0.0},
			{"Q_S8_V2_T0",0.0},
			{"Q_S8_V3_T1",0.0},
			{"Q_S8_V3_T0",0.0},
			{"Q_S8_V4_T1",0.0},
			{"Q_S8_V4_T0",0.0},
			/* Taux d'occupation Sens 1 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"TO_S1_V0_T1",0.0},
			{"TO_S1_V0_T0",0.0},
			{"TO_S1_V1_T1",0.0},
			{"TO_S1_V1_T0",0.0},
			{"TO_S1_V2_T1",0.0},
			{"TO_S1_V2_T0",0.0},
			{"TO_S1_V3_T1",0.0},
			{"TO_S1_V3_T0",0.0},
			{"TO_S1_V4_T1",0.0},
			{"TO_S1_V4_T0",0.0},
			/* Taux d'occupation Sens 2 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"TO_S2_V0_T1",0.0},
			{"TO_S2_V0_T0",0.0},
			{"TO_S2_V1_T1",0.0},
			{"TO_S2_V1_T0",0.0},
			{"TO_S2_V2_T1",0.0},
			{"TO_S2_V2_T0",0.0},
			{"TO_S2_V3_T1",0.0},
			{"TO_S2_V3_T0",0.0},
			{"TO_S2_V4_T1",0.0},
			{"TO_S2_V4_T0",0.0},
			/* Taux d'occupation Sens 3 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"TO_S3_V0_T1",0.0},
			{"TO_S3_V0_T0",0.0},
			{"TO_S3_V1_T1",0.0},
			{"TO_S3_V1_T0",0.0},
			{"TO_S3_V2_T1",0.0},
			{"TO_S3_V2_T0",0.0},
			{"TO_S3_V3_T1",0.0},
			{"TO_S3_V3_T0",0.0},
			{"TO_S3_V4_T1",0.0},
			{"TO_S3_V4_T0",0.0},
			/* Taux d'occupation Sens 4 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"TO_S4_V0_T1",0.0},
			{"TO_S4_V0_T0",0.0},
			{"TO_S4_V1_T1",0.0},
			{"TO_S4_V1_T0",0.0},
			{"TO_S4_V2_T1",0.0},
			{"TO_S4_V2_T0",0.0},
			{"TO_S4_V3_T1",0.0},
			{"TO_S4_V3_T0",0.0},
			{"TO_S4_V4_T1",0.0},
			{"TO_S4_V4_T0",0.0},
			/* Taux d'occupation Sens 7 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"TO_S7_V0_T1",0.0},
			{"TO_S7_V0_T0",0.0},
			{"TO_S7_V1_T1",0.0},
			{"TO_S7_V1_T0",0.0},
			{"TO_S7_V2_T1",0.0},
			{"TO_S7_V2_T0",0.0},
			{"TO_S7_V3_T1",0.0},
			{"TO_S7_V3_T0",0.0},
			{"TO_S7_V4_T1",0.0},
			{"TO_S7_V4_T0",0.0},
			/* Taux d'occupation Sens 8 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"TO_S8_V0_T1",0.0},
			{"TO_S8_V0_T0",0.0},
			{"TO_S8_V1_T1",0.0},
			{"TO_S8_V1_T0",0.0},
			{"TO_S8_V2_T1",0.0},
			{"TO_S8_V2_T0",0.0},
			{"TO_S8_V3_T1",0.0},
			{"TO_S8_V3_T0",0.0},
			{"TO_S8_V4_T1",0.0},
			{"TO_S8_V4_T0",0.0},
			/* %PL Sens 1 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"PL_S1_V0_T1",0.0},
			{"PL_S1_V0_T0",0.0},
			{"PL_S1_V1_T1",0.0},
			{"PL_S1_V1_T0",0.0},
			{"PL_S1_V2_T1",0.0},
			{"PL_S1_V2_T0",0.0},
			{"PL_S1_V3_T1",0.0},
			{"PL_S1_V3_T0",0.0},
			{"PL_S1_V4_T1",0.0},
			{"PL_S1_V4_T0",0.0},
			/* Taux d'occupation Sens 2 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"PL_S2_V0_T1",0.0},
			{"PL_S2_V0_T0",0.0},
			{"PL_S2_V1_T1",0.0},
			{"PL_S2_V1_T0",0.0},
			{"PL_S2_V2_T1",0.0},
			{"PL_S2_V2_T0",0.0},
			{"PL_S2_V3_T1",0.0},
			{"PL_S2_V3_T0",0.0},
			{"PL_S2_V4_T1",0.0},
			{"PL_S2_V4_T0",0.0},
			/* Taux d'occupation Sens 3 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"PL_S3_V0_T1",0.0},
			{"PL_S3_V0_T0",0.0},
			{"PL_S3_V1_T1",0.0},
			{"PL_S3_V1_T0",0.0},
			{"PL_S3_V2_T1",0.0},
			{"PL_S3_V2_T0",0.0},
			{"PL_S3_V3_T1",0.0},
			{"PL_S3_V3_T0",0.0},
			{"PL_S3_V4_T1",0.0},
			{"PL_S3_V4_T0",0.0},
			/* Taux d'occupation Sens 4 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"PL_S4_V0_T1",0.0},
			{"PL_S4_V0_T0",0.0},
			{"PL_S4_V1_T1",0.0},
			{"PL_S4_V1_T0",0.0},
			{"PL_S4_V2_T1",0.0},
			{"PL_S4_V2_T0",0.0},
			{"PL_S4_V3_T1",0.0},
			{"PL_S4_V3_T0",0.0},
			{"PL_S4_V4_T1",0.0},
			{"PL_S4_V4_T0",0.0},
			/* Taux d'occupation Sens 7 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"PL_S7_V0_T1",0.0},
			{"PL_S7_V0_T0",0.0},
			{"PL_S7_V1_T1",0.0},
			{"PL_S7_V1_T0",0.0},
			{"PL_S7_V2_T1",0.0},
			{"PL_S7_V2_T0",0.0},
			{"PL_S7_V3_T1",0.0},
			{"PL_S7_V3_T0",0.0},
			{"PL_S7_V4_T1",0.0},
			{"PL_S7_V4_T0",0.0},
			/* Taux d'occupation Sens 8 : Voie = 0, 1, 2, 3, 4 et t = 0, t = -1 */
			{"PL_S8_V0_T1",0.0},
			{"PL_S8_V0_T0",0.0},
			{"PL_S8_V1_T1",0.0},
			{"PL_S8_V1_T0",0.0},
			{"PL_S8_V2_T1",0.0},
			{"PL_S8_V2_T0",0.0},
			{"PL_S8_V3_T1",0.0},
			{"PL_S8_V3_T0",0.0},
			{"PL_S8_V4_T1",0.0},
			{"PL_S8_V4_T0",0.0},
			/* Contextes */
			{"CTXT_J",0.0},
			{"CTXT_N",0.0},
			{"CTXT_S",0.0},
			{"CTXT_W",0.0},
			{"CTXT_F",0.0},
			/* Distance Inter Vehiculaire Moyenne sens 1 et 2 */
			{"DIV_S1",0.0},
			{"DIV_S2",0.0},
			/* Cumul de Poids */
			{"PDS",0.0}
		};
		
XDY_NomMachine			vg_NomMachine;
XDY_NomSite        		vg_NomSite;
int    				vg_SockTIM = 0;
float				vg_DIV[2];
int				vg_Cumul_Poids = 0;
int				vg_Contexte = 0;
int                		vg_Mode_Fonct = 0;

XDY_EMes_RADT			pg_Mes_RADT[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_1MN];

/* delaration de fonctions externes */

XZEXT_MASK *			vg_SockMask = NULL;
T_INT4				vg_SockRT = NULL;

#endif
