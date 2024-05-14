/*X  Fichier :  $Id: RS_function_string_HIS_DS.bas,v 1.10 2018/09/04 13:47:03 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2018/09/04 13:47:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR                           
------------------------------------------------------
* SOUS-SYSTEME  BASEDD                                
------------------------------------------------------
* FICHIER RS_function_string_HIS_DS.bas               
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :                       
*                                                     
*   installation de la replication : function string  
*                                                     
------------------------------------------------------
* HISTORIQUE :                                        
*                                                     
* JMG	08/01/98 : DS devient un PC niveau 2 (dem/1536) 1.4
* C.T.	16/01/98 : ajout ACT_PAL 1.2
* F.C.	20/03/07 : ajout ACT_BAF, ACT_BAD, ACT_PMA, FMC_NAT
* JBL   Jan 08 :  DEM 663
* PNI   04/09/18        : ajout FMC_TRA
------------------------------------------------------
*/

/* DS pour base historique CI */
/* FMC_GEN */

create function string DS_FMC_GEN.rs_insert for HIS_class
go

create function string DS_FMC_GEN.rs_select for HIS_class
go

create function string DS_FMC_GEN.rs_select_with_lock for HIS_class
go

create function string DS_FMC_GEN.rs_update for HIS_class
go

create function string DS_FMC_GEN.rs_delete for HIS_class
output language ' '
go

/* FMC_HIS */

create function string DS_FMC_HIS.rs_insert for HIS_class
go

create function string DS_FMC_HIS.rs_select for HIS_class
go

create function string DS_FMC_HIS.rs_select_with_lock for HIS_class
go

create function string DS_FMC_HIS.rs_update for HIS_class
go

create function string DS_FMC_HIS.rs_delete for HIS_class
output language ' '
go

/* FMC_ACC */

create function string DS_FMC_ACC.rs_insert for HIS_class
go

create function string DS_FMC_ACC.rs_select for HIS_class
go

create function string DS_FMC_ACC.rs_select_with_lock for HIS_class
go

create function string DS_FMC_ACC.rs_update for HIS_class
go

create function string DS_FMC_ACC.rs_delete for HIS_class
output language ' '
go

/* FMC_BAS */

create function string DS_FMC_BAS.rs_insert for HIS_class
go

create function string DS_FMC_BAS.rs_select for HIS_class
go

create function string DS_FMC_BAS.rs_select_with_lock for HIS_class
go

create function string DS_FMC_BAS.rs_update for HIS_class
go

create function string DS_FMC_BAS.rs_delete for HIS_class
output language ' '
go

/* FMC_BOU */

create function string DS_FMC_BOU.rs_insert for HIS_class
go

create function string DS_FMC_BOU.rs_select for HIS_class
go

create function string DS_FMC_BOU.rs_select_with_lock for HIS_class
go

create function string DS_FMC_BOU.rs_update for HIS_class
go

create function string DS_FMC_BOU.rs_delete for HIS_class
output language ' '
go

/* FMC_CNC */

create function string DS_FMC_CNC.rs_insert for HIS_class
go

create function string DS_FMC_CNC.rs_select for HIS_class
go

create function string DS_FMC_CNC.rs_select_with_lock for HIS_class
go

create function string DS_FMC_CNC.rs_update for HIS_class
go

create function string DS_FMC_CNC.rs_delete for HIS_class
output language ' '
go

/* FMC_COM */

create function string DS_FMC_COM.rs_insert for HIS_class
go

create function string DS_FMC_COM.rs_select for HIS_class
go

create function string DS_FMC_COM.rs_select_with_lock for HIS_class
go

create function string DS_FMC_COM.rs_update for HIS_class
go

create function string DS_FMC_COM.rs_delete for HIS_class
output language ' '
go

/* FMC_DEL */

create function string DS_FMC_DEL.rs_insert for HIS_class
go

create function string DS_FMC_DEL.rs_select for HIS_class
go

create function string DS_FMC_DEL.rs_select_with_lock for HIS_class
go

create function string DS_FMC_DEL.rs_update for HIS_class
go

create function string DS_FMC_DEL.rs_delete for HIS_class
output language ' '
go

/* FMC_ECH */

create function string DS_FMC_ECH.rs_insert for HIS_class
go

create function string DS_FMC_ECH.rs_select for HIS_class
go

create function string DS_FMC_ECH.rs_select_with_lock for HIS_class
go

create function string DS_FMC_ECH.rs_update for HIS_class
go

create function string DS_FMC_ECH.rs_delete for HIS_class
output language ' '
go

/* FMC_MET */

create function string DS_FMC_MET.rs_insert for HIS_class
go

create function string DS_FMC_MET.rs_select for HIS_class
go

create function string DS_FMC_MET.rs_select_with_lock for HIS_class
go

create function string DS_FMC_MET.rs_update for HIS_class
go

create function string DS_FMC_MET.rs_delete for HIS_class
output language ' '
go

/* FMC_MNF */

create function string DS_FMC_MNF.rs_insert for HIS_class
go

create function string DS_FMC_MNF.rs_select for HIS_class
go

create function string DS_FMC_MNF.rs_select_with_lock for HIS_class
go

create function string DS_FMC_MNF.rs_update for HIS_class
go

create function string DS_FMC_MNF.rs_delete for HIS_class
output language ' '
go

/* FMC_NAT */

create function string DS_FMC_NAT.rs_insert for HIS_class
go

create function string DS_FMC_NAT.rs_select for HIS_class
go

create function string DS_FMC_NAT.rs_select_with_lock for HIS_class
go

create function string DS_FMC_NAT.rs_update for HIS_class
go

create function string DS_FMC_NAT.rs_delete for HIS_class
output language ' '
go

/* FMC_OPR */

create function string DS_FMC_OPR.rs_insert for HIS_class
go

create function string DS_FMC_OPR.rs_select for HIS_class
go

create function string DS_FMC_OPR.rs_select_with_lock for HIS_class
go

create function string DS_FMC_OPR.rs_update for HIS_class
go

create function string DS_FMC_OPR.rs_delete for HIS_class
output language ' '
go

/* FMC_TRF */

create function string DS_FMC_TRF.rs_insert for HIS_class
go

create function string DS_FMC_TRF.rs_select for HIS_class
go

create function string DS_FMC_TRF.rs_select_with_lock for HIS_class
go

create function string DS_FMC_TRF.rs_update for HIS_class
go

create function string DS_FMC_TRF.rs_delete for HIS_class
output language ' '
go

/* FMC_TRH */

create function string DS_FMC_TRH.rs_insert for HIS_class
go

create function string DS_FMC_TRH.rs_select for HIS_class
go

create function string DS_FMC_TRH.rs_select_with_lock for HIS_class
go

create function string DS_FMC_TRH.rs_update for HIS_class
go

create function string DS_FMC_TRH.rs_delete for HIS_class
output language ' '
go

/* FMC_VEH */

create function string DS_FMC_VEH.rs_insert for HIS_class
go

create function string DS_FMC_VEH.rs_select for HIS_class
go

create function string DS_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string DS_FMC_VEH.rs_update for HIS_class
go

create function string DS_FMC_VEH.rs_delete for HIS_class
output language ' '
go

/* FMC_VLN */

create function string DS_FMC_VLN.rs_insert for HIS_class
go

create function string DS_FMC_VLN.rs_select for HIS_class
go

create function string DS_FMC_VLN.rs_select_with_lock for HIS_class
go

create function string DS_FMC_VLN.rs_update for HIS_class
go

create function string DS_FMC_VLN.rs_delete for HIS_class
output language ' '
go

/* FMC_INC */

create function string DS_FMC_INC.rs_insert for HIS_class
go

create function string DS_FMC_INC.rs_select for HIS_class
go

create function string DS_FMC_INC.rs_select_with_lock for HIS_class
go

create function string DS_FMC_INC.rs_update for HIS_class
go

create function string DS_FMC_INC.rs_delete for HIS_class
output language ' '
go

/* ACT_GEN */

create function string DS_ACT_GEN.rs_insert for HIS_class
go

create function string DS_ACT_GEN.rs_select for HIS_class
go

create function string DS_ACT_GEN.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GEN.rs_update for HIS_class
go

create function string DS_ACT_GEN.rs_delete for HIS_class
output language ' '
go

/* ACT_APL */

create function string DS_ACT_APL.rs_insert for HIS_class
go

create function string DS_ACT_APL.rs_select for HIS_class
go

create function string DS_ACT_APL.rs_select_with_lock for HIS_class
go

create function string DS_ACT_APL.rs_update for HIS_class
go

create function string DS_ACT_APL.rs_delete for HIS_class
output language ' '
go

/* ACT_BAD */

create function string DS_ACT_BAD.rs_insert for HIS_class
go

create function string DS_ACT_BAD.rs_select for HIS_class
go

create function string DS_ACT_BAD.rs_select_with_lock for HIS_class
go

create function string DS_ACT_BAD.rs_update for HIS_class
go

create function string DS_ACT_BAD.rs_delete for HIS_class
output language ' '
go

/* ACT_BAF */

create function string DS_ACT_BAF.rs_insert for HIS_class
go

create function string DS_ACT_BAF.rs_select for HIS_class
go

create function string DS_ACT_BAF.rs_select_with_lock for HIS_class
go

create function string DS_ACT_BAF.rs_update for HIS_class
go

create function string DS_ACT_BAF.rs_delete for HIS_class
output language ' '
go

/* ACT_DEP */

create function string DS_ACT_DEP.rs_insert for HIS_class
go

create function string DS_ACT_DEP.rs_select for HIS_class
go

create function string DS_ACT_DEP.rs_select_with_lock for HIS_class
go

create function string DS_ACT_DEP.rs_update for HIS_class
go

create function string DS_ACT_DEP.rs_delete for HIS_class
output language ' '
go

/* ACT_ECH */

create function string DS_ACT_ECH.rs_insert for HIS_class
go

create function string DS_ACT_ECH.rs_select for HIS_class
go

create function string DS_ACT_ECH.rs_select_with_lock for HIS_class
go

create function string DS_ACT_ECH.rs_update for HIS_class
go

create function string DS_ACT_ECH.rs_delete for HIS_class
output language ' '
go

/* ACT_FAX */

create function string DS_ACT_FAX.rs_insert for HIS_class
go

create function string DS_ACT_FAX.rs_select for HIS_class
go

create function string DS_ACT_FAX.rs_select_with_lock for HIS_class
go

create function string DS_ACT_FAX.rs_update for HIS_class
go

create function string DS_ACT_FAX.rs_delete for HIS_class
output language ' '
go

/* ACT_FRG */

create function string DS_ACT_FRG.rs_insert for HIS_class
go

create function string DS_ACT_FRG.rs_select for HIS_class
go

create function string DS_ACT_FRG.rs_select_with_lock for HIS_class
go

create function string DS_ACT_FRG.rs_update for HIS_class
go

create function string DS_ACT_FRG.rs_delete for HIS_class
output language ' '
go

/* ACT_GAR */

create function string DS_ACT_GAR.rs_insert for HIS_class
go

create function string DS_ACT_GAR.rs_select for HIS_class
go

create function string DS_ACT_GAR.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GAR.rs_update for HIS_class
go

create function string DS_ACT_GAR.rs_delete for HIS_class
output language ' '
go

/* ACT_INT */

create function string DS_ACT_INT.rs_insert for HIS_class
go

create function string DS_ACT_INT.rs_select for HIS_class
go

create function string DS_ACT_INT.rs_select_with_lock for HIS_class
go

create function string DS_ACT_INT.rs_update for HIS_class
go

create function string DS_ACT_INT.rs_delete for HIS_class
output language ' '
go

/* ACT_NAV */

create function string DS_ACT_NAV.rs_insert for HIS_class
go

create function string DS_ACT_NAV.rs_select for HIS_class
go

create function string DS_ACT_NAV.rs_select_with_lock for HIS_class
go

create function string DS_ACT_NAV.rs_update for HIS_class
go

create function string DS_ACT_NAV.rs_delete for HIS_class
output language ' '
go

/* ACT_PAL */

create function string DS_ACT_PAL.rs_insert for HIS_class
go

create function string DS_ACT_PAL.rs_select for HIS_class
go

create function string DS_ACT_PAL.rs_select_with_lock for HIS_class
go

create function string DS_ACT_PAL.rs_update for HIS_class
go

create function string DS_ACT_PAL.rs_delete for HIS_class
output language ' '
go

/* ACT_PMA */

create function string DS_ACT_PMA.rs_insert for HIS_class
go

create function string DS_ACT_PMA.rs_select for HIS_class
go

create function string DS_ACT_PMA.rs_select_with_lock for HIS_class
go

create function string DS_ACT_PMA.rs_update for HIS_class
go

create function string DS_ACT_PMA.rs_delete for HIS_class
output language ' '
go

/* ACT_PMV */

create function string DS_ACT_PMV.rs_insert for HIS_class
go

create function string DS_ACT_PMV.rs_select for HIS_class
go

create function string DS_ACT_PMV.rs_select_with_lock for HIS_class
go

create function string DS_ACT_PMV.rs_update for HIS_class
go

create function string DS_ACT_PMV.rs_delete for HIS_class
output language ' '
go

/* ACT_RMQ */

create function string DS_ACT_RMQ.rs_insert for HIS_class
go

create function string DS_ACT_RMQ.rs_select for HIS_class
go

create function string DS_ACT_RMQ.rs_select_with_lock for HIS_class
go

create function string DS_ACT_RMQ.rs_update for HIS_class
go

create function string DS_ACT_RMQ.rs_delete for HIS_class
output language ' '
go

/* ACT_SIS */

create function string DS_ACT_SIS.rs_insert for HIS_class
go

create function string DS_ACT_SIS.rs_select for HIS_class
go

create function string DS_ACT_SIS.rs_select_with_lock for HIS_class
go

create function string DS_ACT_SIS.rs_update for HIS_class
go

create function string DS_ACT_SIS.rs_delete for HIS_class
output language ' '
go

/* ACT_TFM */

create function string DS_ACT_TFM.rs_insert for HIS_class
go

create function string DS_ACT_TFM.rs_select for HIS_class
go

create function string DS_ACT_TFM.rs_select_with_lock for HIS_class
go

create function string DS_ACT_TFM.rs_update for HIS_class
go

create function string DS_ACT_TFM.rs_delete for HIS_class
output language ' '
go

/* ACT_TUB */

create function string DS_ACT_TUB.rs_insert for HIS_class
go

create function string DS_ACT_TUB.rs_select for HIS_class
go

create function string DS_ACT_TUB.rs_select_with_lock for HIS_class
go

create function string DS_ACT_TUB.rs_update for HIS_class
go

create function string DS_ACT_TUB.rs_delete for HIS_class
output language ' '
go

/* GTC renovee */
/* ACT_GTC_SIG */

create function string DS_ACT_GTC_SIG.rs_insert for HIS_class
go

create function string DS_ACT_GTC_SIG.rs_select for HIS_class
go

create function string DS_ACT_GTC_SIG.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GTC_SIG.rs_update for HIS_class
go

create function string DS_ACT_GTC_SIG.rs_delete for HIS_class
output language ' '
go


/* ACT_GTC_VEN */

create function string DS_ACT_GTC_VEN.rs_insert for HIS_class
go

create function string DS_ACT_GTC_VEN.rs_select for HIS_class
go

create function string DS_ACT_GTC_VEN.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GTC_VEN.rs_update for HIS_class
go

create function string DS_ACT_GTC_VEN.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_ECL */

create function string DS_ACT_GTC_ECL.rs_insert for HIS_class
go

create function string DS_ACT_GTC_ECL.rs_select for HIS_class
go

create function string DS_ACT_GTC_ECL.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GTC_ECL.rs_update for HIS_class
go

create function string DS_ACT_GTC_ECL.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_EXP */

create function string DS_ACT_GTC_EXP.rs_insert for HIS_class
go

create function string DS_ACT_GTC_EXP.rs_select for HIS_class
go

create function string DS_ACT_GTC_EXP.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GTC_EXP.rs_update for HIS_class
go

create function string DS_ACT_GTC_EXP.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_ENR */

create function string DS_ACT_GTC_ENR.rs_insert for HIS_class
go

create function string DS_ACT_GTC_ENR.rs_select for HIS_class
go

create function string DS_ACT_GTC_ENR.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GTC_ENR.rs_update for HIS_class
go

create function string DS_ACT_GTC_ENR.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_INC */

create function string DS_ACT_GTC_INC.rs_insert for HIS_class
go

create function string DS_ACT_GTC_INC.rs_select for HIS_class
go

create function string DS_ACT_GTC_INC.rs_select_with_lock for HIS_class
go

create function string DS_ACT_GTC_INC.rs_update for HIS_class
go

create function string DS_ACT_GTC_INC.rs_delete for HIS_class
output language ' '
go

/* Fin GTC renovee */

/* ACT_TXT_TFM */

create function string DS_ACT_TXT_TFM.rs_insert for HIS_class
go

create function string DS_ACT_TXT_TFM.rs_select for HIS_class
go

create function string DS_ACT_TXT_TFM.rs_select_with_lock for HIS_class
go

create function string DS_ACT_TXT_TFM.rs_update for HIS_class
go

create function string DS_ACT_TXT_TFM.rs_delete for HIS_class
output language ' '
go

/* ALT_EXP */

create function string DS_ALT_EXP.rs_insert for HIS_class
go

create function string DS_ALT_EXP.rs_select for HIS_class
go

create function string DS_ALT_EXP.rs_select_with_lock for HIS_class
go

create function string DS_ALT_EXP.rs_update for HIS_class
go

create function string DS_ALT_EXP.rs_delete for HIS_class
output language ' '
go

/* EQT_DSP */

create function string DS_EQT_DSP.rs_insert for HIS_class
go

create function string DS_EQT_DSP.rs_select for HIS_class
go

create function string DS_EQT_DSP.rs_select_with_lock for HIS_class
go

create function string DS_EQT_DSP.rs_update for HIS_class
go

create function string DS_EQT_DSP.rs_delete for HIS_class
output language ' '
go

/* MES_POI */

create function string DS_MES_POI.rs_insert for HIS_class
go

create function string DS_MES_POI.rs_select for HIS_class
go

create function string DS_MES_POI.rs_select_with_lock for HIS_class
go

create function string DS_MES_POI.rs_update for HIS_class
go

create function string DS_MES_POI.rs_delete for HIS_class
output language ' '
go

/* MES_TRA */

create function string DS_MES_TRA.rs_insert for HIS_class
go

create function string DS_MES_TRA.rs_select for HIS_class
go

create function string DS_MES_TRA.rs_select_with_lock for HIS_class
go

create function string DS_MES_TRA.rs_update for HIS_class
go

create function string DS_MES_TRA.rs_delete for HIS_class
output language ' '
go

/* MES_KCS */

create function string DS_MES_KCS.rs_insert for HIS_class
go

create function string DS_MES_KCS.rs_select for HIS_class
go

create function string DS_MES_KCS.rs_select_with_lock for HIS_class
go

create function string DS_MES_KCS.rs_update for HIS_class
go

create function string DS_MES_KCS.rs_delete for HIS_class
output language ' '
go

/* MES_NEC */

create function string DS_MES_NEC.rs_insert for HIS_class
go

create function string DS_MES_NEC.rs_select for HIS_class
go

create function string DS_MES_NEC.rs_select_with_lock for HIS_class
go

create function string DS_MES_NEC.rs_update for HIS_class
go

create function string DS_MES_NEC.rs_delete for HIS_class
output language ' '
go

/* MES_QPC */

create function string DS_MES_QPC.rs_insert for HIS_class
go

create function string DS_MES_QPC.rs_select for HIS_class
go

create function string DS_MES_QPC.rs_select_with_lock for HIS_class
go

create function string DS_MES_QPC.rs_update for HIS_class
go

create function string DS_MES_QPC.rs_delete for HIS_class
output language ' '
go
/* PMV_NTR */

create function string DS_PMV_NTR.rs_insert for HIS_class
go

create function string DS_PMV_NTR.rs_select for HIS_class
go

create function string DS_PMV_NTR.rs_select_with_lock for HIS_class
go

create function string DS_PMV_NTR.rs_update for HIS_class
go

create function string DS_PMV_NTR.rs_delete for HIS_class
output language ' '
go


/* FMC_TRA */
create function string DS_FMC_TRA.rs_insert for HIS_class
go

create function string DS_FMC_TRA.rs_select for HIS_class
go

create function string DS_FMC_TRA.rs_select_with_lock for HIS_class
go

create function string DS_FMC_TRA.rs_update for HIS_class
go

create function string DS_FMC_TRA.rs_delete for HIS_class
output language ' '
go

