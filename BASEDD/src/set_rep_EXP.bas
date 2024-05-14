/*X  Fichier : $Id: set_rep_EXP.bas,v 1.7 2018/01/12 13:24:10 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/01/12 13:24:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER set_rep_EXP.bas
------------------------------------------------------
* DESCRIPTION :
* d≈finit les tables primaires r≈pliqu≈es de EXP
* (» ex≈cuter dans tous les sites)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	20/09/94	: Creation
* C.TORREGROSSA	09/09/96	: Ajout replication de fmc_pur au CI pour purge RADT (1.2)
* P.NIEPCERON	04/10/96	: Ajout replication de MES_QPC MES_KCS MES_NEC (DEM/1227) (1.3)
* P.NIEPCERON	21/02/97	: Ajout replication de ZDP_6MN et FRN_TDP (1.4)
* C.TORREGROSSA	16/01/98	: Ajout replication de ACT_PAL (1.5)
* C.TORREGROSSA	13/03/98	: Ajout replication de FMC_TRS (1.6)
* PNI		07/11/12	: Ajout ACT_CCH FMC_MET_CCH 
----------------------------------------------------*/

use EXP
go

exec sp_setreplicate ACT_CCH, true
go

exec sp_setreplicate ACT_APL, true
go

exec sp_setreplicate ACT_DEP, true
go

exec sp_setreplicate ACT_ECH, true
go

exec sp_setreplicate ACT_FAX, true
go

exec sp_setreplicate ACT_FRG, true
go

exec sp_setreplicate ACT_GAR, true
go

exec sp_setreplicate ACT_GEN, true
go

exec sp_setreplicate ACT_INT, true
go

exec sp_setreplicate ACT_NAV, true
go

exec sp_setreplicate ACT_PAL, true
go

exec sp_setreplicate ACT_PMV, true
go

exec sp_setreplicate ACT_RMQ, true
go

exec sp_setreplicate ACT_SIS, true
go

exec sp_setreplicate ACT_TFM, true
go

exec sp_setreplicate ACT_TUB, true
go

exec sp_setreplicate ACT_TXT_TFM, true
go

exec sp_setreplicate ALM_EQT, true
go

exec sp_setreplicate ALM_NBR, true
go

exec sp_setreplicate ALT_EXP, true
go

exec sp_setreplicate EQT_ACV, true
go

exec sp_setreplicate EQT_DSP, true
go

exec sp_setreplicate EQT_FIL, true
go

exec sp_setreplicate FMC_ACC, true
go

exec sp_setreplicate FMC_BAS, true
go

exec sp_setreplicate FMC_BOU, true
go

exec sp_setreplicate FMC_CNC, true
go

exec sp_setreplicate FMC_COM, true
go

exec sp_setreplicate FMC_DEL, true
go

exec sp_setreplicate FMC_ECH, true
go

exec sp_setreplicate FMC_GEN, true
go

exec sp_setreplicate FMC_HIS, true
go

exec sp_setreplicate FMC_MET, true
go

exec sp_setreplicate FMC_MET_CCH, true
go

exec sp_setreplicate FMC_MNF, true
go

exec sp_setreplicate FMC_OPR, true
go

if @@servername="SQL_CI"
	exec sp_setreplicate FMC_PUR, true
go

exec sp_setreplicate FMC_TRS, true
go

if @@servername="SQL_CI"
	exec sp_setreplicate FMC_PUR, true
go

exec sp_setreplicate FMC_TFM, true
go

exec sp_setreplicate FMC_TRF, true
go

exec sp_setreplicate FMC_TRH, true
go

exec sp_setreplicate FMC_TRT, true
go

exec sp_setreplicate FMC_REV, true
go

exec sp_setreplicate FMC_VEH, true
go

exec sp_setreplicate FMC_VLN, true
go

exec sp_setreplicate MES_NIV, true
go

exec sp_setreplicate MES_POI, true
go

exec sp_setreplicate MES_TRA, true
go

if @@servername!="SQL_CI"
	exec sp_setreplicate MES_KCS, true
go
if @@servername!="SQL_CI"
	exec sp_setreplicate MES_NEC, true
go
exec sp_setreplicate FMC_TFM, true
go

exec sp_setreplicate FMC_TRF, true
go

exec sp_setreplicate FMC_TRH, true
go

exec sp_setreplicate FMC_TRT, true
go

exec sp_setreplicate FMC_REV, true
go

exec sp_setreplicate FMC_VEH, true
go

exec sp_setreplicate FMC_VLN, true
go

exec sp_setreplicate MES_NIV, true
go

exec sp_setreplicate MES_POI, true
go

exec sp_setreplicate MES_TRA, true
go

if @@servername!="SQL_CI"
	exec sp_setreplicate MES_KCS, true
go
if @@servername!="SQL_CI"
	exec sp_setreplicate MES_NEC, true
go
if @@servername!="SQL_CI"
	exec sp_setreplicate MES_QPC, true
go
if @@servername!="SQL_CI"
	exec sp_setreplicate ZDP_6MN, true
go

exec sp_setreplicate FRN_TDP, true
go
