/*X  Fichier :  $Id: RS_function_string_HIS_HIS.bas,v 1.2 2012/09/17 10:19:46 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/09/17 10:19:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR                           
------------------------------------------------------
* SOUS-SYSTEME  BASEDD                                
------------------------------------------------------
* FICHIER RS_function_string_HIS_DC.bas               
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :                       
*                                                     
*   installation de la replication : function string  
*                                                     
------------------------------------------------------
* HISTORIQUE :                                        
*                                                     
* JMG	08/01/98 : DC devient un PC niveau 2 (dem/1536) 1.4
* C.T.	16/01/98 : ajout ACT_PAL 1.5
* JBL   Jan 08	 : DEM 663
------------------------------------------------------
*/

/* DC pour base historique CI */
/* FMC_GEN */

create function string HIS_FMC_GEN.rs_insert for HIS_class
go

create function string HIS_FMC_GEN.rs_select for HIS_class
go

create function string HIS_FMC_GEN.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_GEN.rs_update for HIS_class
go

create function string HIS_FMC_GEN.rs_delete for HIS_class
output language ' '
go

/* FMC_HIS */

create function string HIS_FMC_HIS.rs_insert for HIS_class
go

create function string HIS_FMC_HIS.rs_select for HIS_class
go

create function string HIS_FMC_HIS.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_HIS.rs_update for HIS_class
go

create function string HIS_FMC_HIS.rs_delete for HIS_class
output language ' '
go

/* FMC_ACC */

create function string HIS_FMC_ACC.rs_insert for HIS_class
go

create function string HIS_FMC_ACC.rs_select for HIS_class
go

create function string HIS_FMC_ACC.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_ACC.rs_update for HIS_class
go

create function string HIS_FMC_ACC.rs_delete for HIS_class
output language ' '
go

/* FMC_BAS */

create function string HIS_FMC_BAS.rs_insert for HIS_class
go

create function string HIS_FMC_BAS.rs_select for HIS_class
go

create function string HIS_FMC_BAS.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_BAS.rs_update for HIS_class
go

create function string HIS_FMC_BAS.rs_delete for HIS_class
output language ' '
go

/* FMC_BOU */

create function string HIS_FMC_BOU.rs_insert for HIS_class
go

create function string HIS_FMC_BOU.rs_select for HIS_class
go

create function string HIS_FMC_BOU.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_BOU.rs_update for HIS_class
go

create function string HIS_FMC_BOU.rs_delete for HIS_class
output language ' '
go

/* FMC_CNC */

create function string HIS_FMC_CNC.rs_insert for HIS_class
go

create function string HIS_FMC_CNC.rs_select for HIS_class
go

create function string HIS_FMC_CNC.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_CNC.rs_update for HIS_class
go

create function string HIS_FMC_CNC.rs_delete for HIS_class
output language ' '
go

/* FMC_COM */

create function string HIS_FMC_COM.rs_insert for HIS_class
go

create function string HIS_FMC_COM.rs_select for HIS_class
go

create function string HIS_FMC_COM.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_COM.rs_update for HIS_class
go

create function string HIS_FMC_COM.rs_delete for HIS_class
output language ' '
go

/* FMC_DEL */

create function string HIS_FMC_DEL.rs_insert for HIS_class
go

create function string HIS_FMC_DEL.rs_select for HIS_class
go

create function string HIS_FMC_DEL.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_DEL.rs_update for HIS_class
go

create function string HIS_FMC_DEL.rs_delete for HIS_class
output language ' '
go

/* FMC_ECH */

create function string HIS_FMC_ECH.rs_insert for HIS_class
go

create function string HIS_FMC_ECH.rs_select for HIS_class
go

create function string HIS_FMC_ECH.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_ECH.rs_update for HIS_class
go

create function string HIS_FMC_ECH.rs_delete for HIS_class
output language ' '
go

/* FMC_MET */

create function string HIS_FMC_MET.rs_insert for HIS_class
go

create function string HIS_FMC_MET.rs_select for HIS_class
go

create function string HIS_FMC_MET.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_MET.rs_update for HIS_class
go

create function string HIS_FMC_MET.rs_delete for HIS_class
output language ' '
go

/* FMC_MNF */

create function string HIS_FMC_MNF.rs_insert for HIS_class
go

create function string HIS_FMC_MNF.rs_select for HIS_class
go

create function string HIS_FMC_MNF.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_MNF.rs_update for HIS_class
go

create function string HIS_FMC_MNF.rs_delete for HIS_class
output language ' '
go

/* FMC_OPR */

create function string HIS_FMC_OPR.rs_insert for HIS_class
go

create function string HIS_FMC_OPR.rs_select for HIS_class
go

create function string HIS_FMC_OPR.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_OPR.rs_update for HIS_class
go

create function string HIS_FMC_OPR.rs_delete for HIS_class
output language ' '
go

/* FMC_TRF */

create function string HIS_FMC_TRF.rs_insert for HIS_class
go

create function string HIS_FMC_TRF.rs_select for HIS_class
go

create function string HIS_FMC_TRF.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_TRF.rs_update for HIS_class
go

create function string HIS_FMC_TRF.rs_delete for HIS_class
output language ' '
go

/* FMC_TRH */

create function string HIS_FMC_TRH.rs_insert for HIS_class
go

create function string HIS_FMC_TRH.rs_select for HIS_class
go

create function string HIS_FMC_TRH.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_TRH.rs_update for HIS_class
go

create function string HIS_FMC_TRH.rs_delete for HIS_class
output language ' '
go

/* FMC_VEH */

create function string HIS_FMC_VEH.rs_insert for HIS_class
go

create function string HIS_FMC_VEH.rs_select for HIS_class
go

create function string HIS_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_VEH.rs_update for HIS_class
go

create function string HIS_FMC_VEH.rs_delete for HIS_class
output language ' '
go

/* FMC_VLN */

create function string HIS_FMC_VLN.rs_insert for HIS_class
go

create function string HIS_FMC_VLN.rs_select for HIS_class
go

create function string HIS_FMC_VLN.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_VLN.rs_update for HIS_class
go

create function string HIS_FMC_VLN.rs_delete for HIS_class
output language ' '
go
/* FMC_DAT */
create function string HIS_FMC_DAT.rs_insert for HIS_class
go

create function string HIS_FMC_DAT.rs_select for HIS_class
go

create function string HIS_FMC_DAT.rs_select_with_lock for HIS_class
go

create function string HIS_FMC_DAT.rs_update for HIS_class
go

create function string HIS_FMC_DAT.rs_delete for HIS_class
output language ' '
go

/* ACT_GEN */

create function string HIS_ACT_GEN.rs_insert for HIS_class
go

create function string HIS_ACT_GEN.rs_select for HIS_class
go

create function string HIS_ACT_GEN.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GEN.rs_update for HIS_class
go

create function string HIS_ACT_GEN.rs_delete for HIS_class
output language ' '
go

/* ACT_APL */

create function string HIS_ACT_APL.rs_insert for HIS_class
go

create function string HIS_ACT_APL.rs_select for HIS_class
go

create function string HIS_ACT_APL.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_APL.rs_update for HIS_class
go

create function string HIS_ACT_APL.rs_delete for HIS_class
output language ' '
go

/* ACT_DEP */

create function string HIS_ACT_DEP.rs_insert for HIS_class
go

create function string HIS_ACT_DEP.rs_select for HIS_class
go

create function string HIS_ACT_DEP.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_DEP.rs_update for HIS_class
go

create function string HIS_ACT_DEP.rs_delete for HIS_class
output language ' '
go

/* ACT_ECH */

create function string HIS_ACT_ECH.rs_insert for HIS_class
go

create function string HIS_ACT_ECH.rs_select for HIS_class
go

create function string HIS_ACT_ECH.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_ECH.rs_update for HIS_class
go

create function string HIS_ACT_ECH.rs_delete for HIS_class
output language ' '
go

/* ACT_FAX */

create function string HIS_ACT_FAX.rs_insert for HIS_class
go

create function string HIS_ACT_FAX.rs_select for HIS_class
go

create function string HIS_ACT_FAX.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_FAX.rs_update for HIS_class
go

create function string HIS_ACT_FAX.rs_delete for HIS_class
output language ' '
go

/* ACT_FRG */

create function string HIS_ACT_FRG.rs_insert for HIS_class
go

create function string HIS_ACT_FRG.rs_select for HIS_class
go

create function string HIS_ACT_FRG.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_FRG.rs_update for HIS_class
go

create function string HIS_ACT_FRG.rs_delete for HIS_class
output language ' '
go

/* ACT_GAR */

create function string HIS_ACT_GAR.rs_insert for HIS_class
go

create function string HIS_ACT_GAR.rs_select for HIS_class
go

create function string HIS_ACT_GAR.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GAR.rs_update for HIS_class
go

create function string HIS_ACT_GAR.rs_delete for HIS_class
output language ' '
go

/* ACT_INT */

create function string HIS_ACT_INT.rs_insert for HIS_class
go

create function string HIS_ACT_INT.rs_select for HIS_class
go

create function string HIS_ACT_INT.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_INT.rs_update for HIS_class
go

create function string HIS_ACT_INT.rs_delete for HIS_class
output language ' '
go

/* ACT_NAV */

create function string HIS_ACT_NAV.rs_insert for HIS_class
go

create function string HIS_ACT_NAV.rs_select for HIS_class
go

create function string HIS_ACT_NAV.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_NAV.rs_update for HIS_class
go

create function string HIS_ACT_NAV.rs_delete for HIS_class
output language ' '
go

/* ACT_PAL */

create function string HIS_ACT_PAL.rs_insert for HIS_class
go

create function string HIS_ACT_PAL.rs_select for HIS_class
go

create function string HIS_ACT_PAL.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_PAL.rs_update for HIS_class
go

create function string HIS_ACT_PAL.rs_delete for HIS_class
output language ' '
go

/* ACT_PMV */

create function string HIS_ACT_PMV.rs_insert for HIS_class
go

create function string HIS_ACT_PMV.rs_select for HIS_class
go

create function string HIS_ACT_PMV.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_PMV.rs_update for HIS_class
go

create function string HIS_ACT_PMV.rs_delete for HIS_class
output language ' '
go

/* ACT_RMQ */

create function string HIS_ACT_RMQ.rs_insert for HIS_class
go

create function string HIS_ACT_RMQ.rs_select for HIS_class
go

create function string HIS_ACT_RMQ.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_RMQ.rs_update for HIS_class
go

create function string HIS_ACT_RMQ.rs_delete for HIS_class
output language ' '
go

/* ACT_SIS */

create function string HIS_ACT_SIS.rs_insert for HIS_class
go

create function string HIS_ACT_SIS.rs_select for HIS_class
go

create function string HIS_ACT_SIS.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_SIS.rs_update for HIS_class
go

create function string HIS_ACT_SIS.rs_delete for HIS_class
output language ' '
go

/* ACT_TFM */

create function string HIS_ACT_TFM.rs_insert for HIS_class
go

create function string HIS_ACT_TFM.rs_select for HIS_class
go

create function string HIS_ACT_TFM.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_TFM.rs_update for HIS_class
go

create function string HIS_ACT_TFM.rs_delete for HIS_class
output language ' '
go

/* ACT_TUB */
create function string HIS_ACT_TUB.rs_insert for HIS_class
go

create function string HIS_ACT_TUB.rs_select for HIS_class
go

create function string HIS_ACT_TUB.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_TUB.rs_update for HIS_class
go

create function string HIS_ACT_TUB.rs_delete for HIS_class
output language ' '
go

/* GTC Renovee */
/* ACT_GTC_SIG */
create function string HIS_ACT_GTC_SIG.rs_insert for HIS_class
go

create function string HIS_ACT_GTC_SIG.rs_select for HIS_class
go

create function string HIS_ACT_GTC_SIG.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GTC_SIG.rs_update for HIS_class
go

create function string HIS_ACT_GTC_SIG.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_VEN */
create function string HIS_ACT_GTC_VEN.rs_insert for HIS_class
go

create function string HIS_ACT_GTC_VEN.rs_select for HIS_class
go

create function string HIS_ACT_GTC_VEN.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GTC_VEN.rs_update for HIS_class
go

create function string HIS_ACT_GTC_VEN.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_ECL */
create function string HIS_ACT_GTC_ECL.rs_insert for HIS_class
go

create function string HIS_ACT_GTC_ECL.rs_select for HIS_class
go

create function string HIS_ACT_GTC_ECL.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GTC_ECL.rs_update for HIS_class
go

create function string HIS_ACT_GTC_ECL.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_EXP */
create function string HIS_ACT_GTC_EXP.rs_insert for HIS_class
go

create function string HIS_ACT_GTC_EXP.rs_select for HIS_class
go

create function string HIS_ACT_GTC_EXP.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GTC_EXP.rs_update for HIS_class
go

create function string HIS_ACT_GTC_EXP.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_ENR */
create function string HIS_ACT_GTC_ENR.rs_insert for HIS_class
go

create function string HIS_ACT_GTC_ENR.rs_select for HIS_class
go

create function string HIS_ACT_GTC_ENR.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GTC_ENR.rs_update for HIS_class
go

create function string HIS_ACT_GTC_ENR.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_INC */
create function string HIS_ACT_GTC_INC.rs_insert for HIS_class
go

create function string HIS_ACT_GTC_INC.rs_select for HIS_class
go

create function string HIS_ACT_GTC_INC.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_GTC_INC.rs_update for HIS_class
go

create function string HIS_ACT_GTC_INC.rs_delete for HIS_class
output language ' '
go
/* Fin GTC Renovee */


/* ACT_TXT_TFM */

create function string HIS_ACT_TXT_TFM.rs_insert for HIS_class
go

create function string HIS_ACT_TXT_TFM.rs_select for HIS_class
go

create function string HIS_ACT_TXT_TFM.rs_select_with_lock for HIS_class
go

create function string HIS_ACT_TXT_TFM.rs_update for HIS_class
go

create function string HIS_ACT_TXT_TFM.rs_delete for HIS_class
output language ' '
go

/* ALT_EXP */

create function string HIS_ALT_EXP.rs_insert for HIS_class
go

create function string HIS_ALT_EXP.rs_select for HIS_class
go

create function string HIS_ALT_EXP.rs_select_with_lock for HIS_class
go

create function string HIS_ALT_EXP.rs_update for HIS_class
go

create function string HIS_ALT_EXP.rs_delete for HIS_class
output language ' '
go

/* EQT_DSP */

create function string HIS_EQT_DSP.rs_insert for HIS_class
go

create function string HIS_EQT_DSP.rs_select for HIS_class
go

create function string HIS_EQT_DSP.rs_select_with_lock for HIS_class
go

create function string HIS_EQT_DSP.rs_update for HIS_class
go

create function string HIS_EQT_DSP.rs_delete for HIS_class
output language ' '
go

/* MES_POI */

create function string HIS_MES_POI.rs_insert for HIS_class
go

create function string HIS_MES_POI.rs_select for HIS_class
go

create function string HIS_MES_POI.rs_select_with_lock for HIS_class
go

create function string HIS_MES_POI.rs_update for HIS_class
go

create function string HIS_MES_POI.rs_delete for HIS_class
output language ' '
go

/* MES_TRA */

create function string HIS_MES_TRA.rs_insert for HIS_class
go

create function string HIS_MES_TRA.rs_select for HIS_class
go

create function string HIS_MES_TRA.rs_select_with_lock for HIS_class
go

create function string HIS_MES_TRA.rs_update for HIS_class
go

create function string HIS_MES_TRA.rs_delete for HIS_class
output language ' '
go

/* MES_KCS */

create function string HIS_MES_KCS.rs_insert for HIS_class
go

create function string HIS_MES_KCS.rs_select for HIS_class
go

create function string HIS_MES_KCS.rs_select_with_lock for HIS_class
go

create function string HIS_MES_KCS.rs_update for HIS_class
go

create function string HIS_MES_KCS.rs_delete for HIS_class
output language ' '
go

/* MES_NEC */

create function string HIS_MES_NEC.rs_insert for HIS_class
go

create function string HIS_MES_NEC.rs_select for HIS_class
go

create function string HIS_MES_NEC.rs_select_with_lock for HIS_class
go

create function string HIS_MES_NEC.rs_update for HIS_class
go

create function string HIS_MES_NEC.rs_delete for HIS_class
output language ' '
go

/* MES_QPC */

create function string HIS_MES_QPC.rs_insert for HIS_class
go

create function string HIS_MES_QPC.rs_select for HIS_class
go

create function string HIS_MES_QPC.rs_select_with_lock for HIS_class
go

create function string HIS_MES_QPC.rs_update for HIS_class
go

create function string HIS_MES_QPC.rs_delete for HIS_class
output language ' '
go
