/*X  Fichier : $Id: set_rep_CFG.bas,v 1.14 2018/01/12 13:25:58 gesconf Exp $      Release : $Revision: 1.14 $        Date : $Date: 2018/01/12 13:25:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER set_rep_CFG.bas
------------------------------------------------------
* DESCRIPTION :
* d≈finit les tables primaires r≈pliqu≈es de CFG
* (» ex≈cuter au CI uniquement)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	11/10/94	: Creation
* C.TORREGROSSA	29/06/95	: Ajout de RES_NUM_ECH
* C.TORREGROSSA	10/08/95	: Ajout de EQT_ISI
* C.TORREGROSSA	18/10/95	: Ajout de RES_AUT_INV
* C.TORREGROSSA	04/12/95	: Ajout de EQT_FAX (V 1.7)
* P.VERDIER	18/06/96	: Ajout de LIB_TYP et LIB_PRE
* D.MISMER	05/11/96	: Ajout de EQT_LCR (DEM/1232) (V 1.9)
* D.MISMER      07/02/97	: Ajout de EQT_TDP, EQT_ZDP, RES_DTP (DEM/1395) (V1.9)
* GUILHOU	08/01/98	: ajout de ZON_PMV (dem/1534) 1.11
* C.TORREGROSSA	16/01/98	: Ajout de EQT_PAL (V 1.12)
* PNI		02/07/07	: Ajout de RES_CAN (DEM 657) 1.13
* PNI		07/11/12	: Ajout CCH_TRO 1.12
----------------------------------------------------*/

use CFG
go


exec sp_setreplicate ANN_AST_CAL, true
go
exec sp_setreplicate ANN_AST_CAT, true
go
exec sp_setreplicate ANN_AST_CMP, true
go
exec sp_setreplicate ANN_CMP, true
go
exec sp_setreplicate ANN_FIC, true
go
exec sp_setreplicate ANN_GEO, true
go
exec sp_setreplicate CCH_TRO, true
go
exec sp_setreplicate EQT_ALM, true
go
exec sp_setreplicate EQT_ANA, true
go
exec sp_setreplicate EQT_CAM, true
go
exec sp_setreplicate EQT_CAM_POS, true
go
exec sp_setreplicate EQT_CPT, true
go
exec sp_setreplicate EQT_DAI, true
go
exec sp_setreplicate EQT_ECH, true
go
exec sp_setreplicate EQT_FAX, true
go
exec sp_setreplicate EQT_FAV, true
go
exec sp_setreplicate EQT_GEN, true
go
exec sp_setreplicate EQT_GTC, true
go
exec sp_setreplicate EQT_ISI, true
go
exec sp_setreplicate EQT_LTV, true
go
exec sp_setreplicate EQT_MAA, true
go
exec sp_setreplicate EQT_MAC, true
go
exec sp_setreplicate EQT_MAG, true
go
exec sp_setreplicate EQT_MNT, true
go
exec sp_setreplicate EQT_MOD, true
go
exec sp_setreplicate EQT_NAV, true
go
exec sp_setreplicate EQT_NIC, true
go
exec sp_setreplicate EQT_PAL, true
go
exec sp_setreplicate EQT_PAN, true
go
exec sp_setreplicate EQT_PAU, true
go
exec sp_setreplicate EQT_PCT, true
go
exec sp_setreplicate EQT_PMV, true
go
exec sp_setreplicate EQT_PNT_MES, true
go
exec sp_setreplicate EQT_RAD, true
go
exec sp_setreplicate EQT_RAU, true
go
exec sp_setreplicate EQT_LCR, true
go
exec sp_setreplicate EQT_TDP, true
go
exec sp_setreplicate EQT_ZDP, true
go
exec sp_setreplicate EQT_SCN_CNN, true
go
exec sp_setreplicate EQT_SCN_NAV, true
go
exec sp_setreplicate EQT_SEQ_ECH, true
go
exec sp_setreplicate EQT_SEQ_TUB, true
go
exec sp_setreplicate EQT_SQA_ECH, true
go
exec sp_setreplicate EQT_SQA_TUB, true
go
exec sp_setreplicate EQT_SYV, true
go
exec sp_setreplicate EQT_TOR, true
go
exec sp_setreplicate EQT_TUB, true
go
exec sp_setreplicate LIB_TYP, true
go
exec sp_setreplicate LIB_PRE, true
go
exec sp_setreplicate OPR_FCT_MAC, true
go
exec sp_setreplicate OPR_FCT_NOM, true
go
exec sp_setreplicate OPR_LOG_MDP, true
go
exec sp_setreplicate OPR_NOM, true
go
exec sp_setreplicate OPR_PRF, true
go
exec sp_setreplicate OPR_PRF_FCT, true
go
exec sp_setreplicate PLN_SCR, true
go
exec sp_setreplicate RES_AIR, true
go
exec sp_setreplicate RES_AUT, true
go
exec sp_setreplicate RES_AUT_INV, true
go
exec sp_setreplicate RES_CAN, true
go
exec sp_setreplicate RES_CHA, true
go
exec sp_setreplicate RES_DIS, true
go
exec sp_setreplicate RES_ECH, true
go
exec sp_setreplicate RES_INF, true
go
exec sp_setreplicate RES_ITP, true
go
exec sp_setreplicate RES_NEU, true
go
exec sp_setreplicate RES_NUM_ECH, true
go
exec sp_setreplicate RES_PEA, true
go
exec sp_setreplicate RES_PNT, true
go
exec sp_setreplicate RES_POL, true
go
exec sp_setreplicate RES_POR, true
go
exec sp_setreplicate RES_PTL, true
go
exec sp_setreplicate RES_SEG, true
go
exec sp_setreplicate RES_SUP, true
go
exec sp_setreplicate RES_TRC, true
go
exec sp_setreplicate RES_TRN, true
go
exec sp_setreplicate RES_VIL, true
go
exec sp_setreplicate RES_DTP, true
go
exec sp_setreplicate TYP_ALM, true
go
exec sp_setreplicate TYP_ALT, true
go
exec sp_setreplicate TYP_AST, true
go
exec sp_setreplicate TYP_CRB, true
go
exec sp_setreplicate TYP_FMC, true
go
exec sp_setreplicate TYP_NAV, true
go
exec sp_setreplicate TYP_PCT, true
go
exec sp_setreplicate TYP_PMV, true
go
exec sp_setreplicate EQT_RAD_VTL, true
go
exec sp_setreplicate ZON_PMV, true
go

