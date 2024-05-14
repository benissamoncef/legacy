/*X  Fichier :  $Id: RS_function_string_HIS_DN.bas,v 1.8 2012/09/17 10:19:34 devgtie Exp $      Release : $Revision: 1.8 $        Date : $Date: 2012/09/17 10:19:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR                           
------------------------------------------------------
* SOUS-SYSTEME  BASEDD                                
------------------------------------------------------
* FICHIER RS_function_string_HIS_DN.bas               
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :                       
*                                                     
*   installation de la replication : function string  
*                                                     
------------------------------------------------------
* HISTORIQUE :                                        
*                                                     
* C.T.	09/08/94	: Creation              
* C.T.	20/03/95	: Modif presentation    
*                         entete pour serveur (1.2)  
* C.T.	03/05/96	: supprimer les fonctions sur MES_NIV (1.3)
* P.N.	04/10/96	: ajouter les fonctions sur MES_KCS MES_QPC MES_NEC (1.4 )
* C.T.	16/01/98	: ajout ACT_PAL 1.5
* F.C.	20/03/07 : ajout ACT_BAF, ACT_BAD, ACT_PMA, FMC_NAT
* JBL   Jan 08      : DEM 663
------------------------------------------------------
*/

/* DM pour base historique CI */
/* FMC_GEN */

create function string DN_FMC_GEN.rs_insert for HIS_class
go

create function string DN_FMC_GEN.rs_select for HIS_class
go

create function string DN_FMC_GEN.rs_select_with_lock for HIS_class
go

create function string DN_FMC_GEN.rs_update for HIS_class
go

create function string DN_FMC_GEN.rs_delete for HIS_class
output language ' '
go

/* FMC_HIS */

create function string DN_FMC_HIS.rs_insert for HIS_class
go

create function string DN_FMC_HIS.rs_select for HIS_class
go

create function string DN_FMC_HIS.rs_select_with_lock for HIS_class
go

create function string DN_FMC_HIS.rs_update for HIS_class
go

create function string DN_FMC_HIS.rs_delete for HIS_class
output language ' '
go

/* FMC_ACC */

create function string DN_FMC_ACC.rs_insert for HIS_class
go

create function string DN_FMC_ACC.rs_select for HIS_class
go

create function string DN_FMC_ACC.rs_select_with_lock for HIS_class
go

create function string DN_FMC_ACC.rs_update for HIS_class
go

create function string DN_FMC_ACC.rs_delete for HIS_class
output language ' '
go

/* FMC_BAS */

create function string DN_FMC_BAS.rs_insert for HIS_class
go

create function string DN_FMC_BAS.rs_select for HIS_class
go

create function string DN_FMC_BAS.rs_select_with_lock for HIS_class
go

create function string DN_FMC_BAS.rs_update for HIS_class
go

create function string DN_FMC_BAS.rs_delete for HIS_class
output language ' '
go

/* FMC_BOU */

create function string DN_FMC_BOU.rs_insert for HIS_class
go

create function string DN_FMC_BOU.rs_select for HIS_class
go

create function string DN_FMC_BOU.rs_select_with_lock for HIS_class
go

create function string DN_FMC_BOU.rs_update for HIS_class
go

create function string DN_FMC_BOU.rs_delete for HIS_class
output language ' '
go

/* FMC_CNC */

create function string DN_FMC_CNC.rs_insert for HIS_class
go

create function string DN_FMC_CNC.rs_select for HIS_class
go

create function string DN_FMC_CNC.rs_select_with_lock for HIS_class
go

create function string DN_FMC_CNC.rs_update for HIS_class
go

create function string DN_FMC_CNC.rs_delete for HIS_class
output language ' '
go

/* FMC_COM */

create function string DN_FMC_COM.rs_insert for HIS_class
go

create function string DN_FMC_COM.rs_select for HIS_class
go

create function string DN_FMC_COM.rs_select_with_lock for HIS_class
go

create function string DN_FMC_COM.rs_update for HIS_class
go

create function string DN_FMC_COM.rs_delete for HIS_class
output language ' '
go

/* FMC_DEL */

create function string DN_FMC_DEL.rs_insert for HIS_class
go

create function string DN_FMC_DEL.rs_select for HIS_class
go

create function string DN_FMC_DEL.rs_select_with_lock for HIS_class
go

create function string DN_FMC_DEL.rs_update for HIS_class
go

create function string DN_FMC_DEL.rs_delete for HIS_class
output language ' '
go

/* FMC_ECH */

create function string DN_FMC_ECH.rs_insert for HIS_class
go

create function string DN_FMC_ECH.rs_select for HIS_class
go

create function string DN_FMC_ECH.rs_select_with_lock for HIS_class
go

create function string DN_FMC_ECH.rs_update for HIS_class
go

create function string DN_FMC_ECH.rs_delete for HIS_class
output language ' '
go

/* FMC_MET */

create function string DN_FMC_MET.rs_insert for HIS_class
go

create function string DN_FMC_MET.rs_select for HIS_class
go

create function string DN_FMC_MET.rs_select_with_lock for HIS_class
go

create function string DN_FMC_MET.rs_update for HIS_class
go

create function string DN_FMC_MET.rs_delete for HIS_class
output language ' '
go

/* FMC_MNF */

create function string DN_FMC_MNF.rs_insert for HIS_class
go

create function string DN_FMC_MNF.rs_select for HIS_class
go

create function string DN_FMC_MNF.rs_select_with_lock for HIS_class
go

create function string DN_FMC_MNF.rs_update for HIS_class
go

create function string DN_FMC_MNF.rs_delete for HIS_class
output language ' '
go

/* FMC_NAT */

create function string DN_FMC_NAT.rs_insert for HIS_class
go

create function string DN_FMC_NAT.rs_select for HIS_class
go

create function string DN_FMC_NAT.rs_select_with_lock for HIS_class
go

create function string DN_FMC_NAT.rs_update for HIS_class
go

create function string DN_FMC_NAT.rs_delete for HIS_class
output language ' '
go

/* FMC_OPR */

create function string DN_FMC_OPR.rs_insert for HIS_class
go

create function string DN_FMC_OPR.rs_select for HIS_class
go

create function string DN_FMC_OPR.rs_select_with_lock for HIS_class
go

create function string DN_FMC_OPR.rs_update for HIS_class
go

create function string DN_FMC_OPR.rs_delete for HIS_class
output language ' '
go

/* FMC_TRF */

create function string DN_FMC_TRF.rs_insert for HIS_class
go

create function string DN_FMC_TRF.rs_select for HIS_class
go

create function string DN_FMC_TRF.rs_select_with_lock for HIS_class
go

create function string DN_FMC_TRF.rs_update for HIS_class
go

create function string DN_FMC_TRF.rs_delete for HIS_class
output language ' '
go

/* FMC_TRH */

create function string DN_FMC_TRH.rs_insert for HIS_class
go

create function string DN_FMC_TRH.rs_select for HIS_class
go

create function string DN_FMC_TRH.rs_select_with_lock for HIS_class
go

create function string DN_FMC_TRH.rs_update for HIS_class
go

create function string DN_FMC_TRH.rs_delete for HIS_class
output language ' '
go

/* FMC_VEH */

create function string DN_FMC_VEH.rs_insert for HIS_class
go

create function string DN_FMC_VEH.rs_select for HIS_class
go

create function string DN_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string DN_FMC_VEH.rs_update for HIS_class
go

create function string DN_FMC_VEH.rs_delete for HIS_class
output language ' '
go

/* FMC_VLN */

create function string DN_FMC_VLN.rs_insert for HIS_class
go

create function string DN_FMC_VLN.rs_select for HIS_class
go

create function string DN_FMC_VLN.rs_select_with_lock for HIS_class
go

create function string DN_FMC_VLN.rs_update for HIS_class
go

create function string DN_FMC_VLN.rs_delete for HIS_class
output language ' '
go

/* ACT_GEN */

create function string DN_ACT_GEN.rs_insert for HIS_class
go

create function string DN_ACT_GEN.rs_select for HIS_class
go

create function string DN_ACT_GEN.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GEN.rs_update for HIS_class
go

create function string DN_ACT_GEN.rs_delete for HIS_class
output language ' '
go

/* ACT_APL */

create function string DN_ACT_APL.rs_insert for HIS_class
go

create function string DN_ACT_APL.rs_select for HIS_class
go

create function string DN_ACT_APL.rs_select_with_lock for HIS_class
go

create function string DN_ACT_APL.rs_update for HIS_class
go

create function string DN_ACT_APL.rs_delete for HIS_class
output language ' '
go

/* ACT_BAD */

create function string DN_ACT_BAD.rs_insert for HIS_class
go

create function string DN_ACT_BAD.rs_select for HIS_class
go

create function string DN_ACT_BAD.rs_select_with_lock for HIS_class
go

create function string DN_ACT_BAD.rs_update for HIS_class
go

create function string DN_ACT_BAD.rs_delete for HIS_class
output language ' '
go

/* ACT_BAF */

create function string DN_ACT_BAF.rs_insert for HIS_class
go

create function string DN_ACT_BAF.rs_select for HIS_class
go

create function string DN_ACT_BAF.rs_select_with_lock for HIS_class
go

create function string DN_ACT_BAF.rs_update for HIS_class
go

create function string DN_ACT_BAF.rs_delete for HIS_class
output language ' '
go

/* ACT_DEP */

create function string DN_ACT_DEP.rs_insert for HIS_class
go

create function string DN_ACT_DEP.rs_select for HIS_class
go

create function string DN_ACT_DEP.rs_select_with_lock for HIS_class
go

create function string DN_ACT_DEP.rs_update for HIS_class
go

create function string DN_ACT_DEP.rs_delete for HIS_class
output language ' '
go

/* ACT_ECH */

create function string DN_ACT_ECH.rs_insert for HIS_class
go

create function string DN_ACT_ECH.rs_select for HIS_class
go

create function string DN_ACT_ECH.rs_select_with_lock for HIS_class
go

create function string DN_ACT_ECH.rs_update for HIS_class
go

create function string DN_ACT_ECH.rs_delete for HIS_class
output language ' '
go

/* ACT_FAX */

create function string DN_ACT_FAX.rs_insert for HIS_class
go

create function string DN_ACT_FAX.rs_select for HIS_class
go

create function string DN_ACT_FAX.rs_select_with_lock for HIS_class
go

create function string DN_ACT_FAX.rs_update for HIS_class
go

create function string DN_ACT_FAX.rs_delete for HIS_class
output language ' '
go

/* ACT_FRG */

create function string DN_ACT_FRG.rs_insert for HIS_class
go

create function string DN_ACT_FRG.rs_select for HIS_class
go

create function string DN_ACT_FRG.rs_select_with_lock for HIS_class
go

create function string DN_ACT_FRG.rs_update for HIS_class
go

create function string DN_ACT_FRG.rs_delete for HIS_class
output language ' '
go

/* ACT_GAR */

create function string DN_ACT_GAR.rs_insert for HIS_class
go

create function string DN_ACT_GAR.rs_select for HIS_class
go

create function string DN_ACT_GAR.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GAR.rs_update for HIS_class
go

create function string DN_ACT_GAR.rs_delete for HIS_class
output language ' '
go

/* ACT_INT */

create function string DN_ACT_INT.rs_insert for HIS_class
go

create function string DN_ACT_INT.rs_select for HIS_class
go

create function string DN_ACT_INT.rs_select_with_lock for HIS_class
go

create function string DN_ACT_INT.rs_update for HIS_class
go

create function string DN_ACT_INT.rs_delete for HIS_class
output language ' '
go

/* ACT_NAV */

create function string DN_ACT_NAV.rs_insert for HIS_class
go

create function string DN_ACT_NAV.rs_select for HIS_class
go

create function string DN_ACT_NAV.rs_select_with_lock for HIS_class
go

create function string DN_ACT_NAV.rs_update for HIS_class
go

create function string DN_ACT_NAV.rs_delete for HIS_class
output language ' '
go

/* ACT_PAL */

create function string DN_ACT_PAL.rs_insert for HIS_class
go

create function string DN_ACT_PAL.rs_select for HIS_class
go

create function string DN_ACT_PAL.rs_select_with_lock for HIS_class
go

create function string DN_ACT_PAL.rs_update for HIS_class
go

create function string DN_ACT_PAL.rs_delete for HIS_class
output language ' '
go

/* ACT_PMA */

create function string DN_ACT_PMA.rs_insert for HIS_class
go

create function string DN_ACT_PMA.rs_select for HIS_class
go

create function string DN_ACT_PMA.rs_select_with_lock for HIS_class
go

create function string DN_ACT_PMA.rs_update for HIS_class
go

create function string DN_ACT_PMA.rs_delete for HIS_class
output language ' '
go

/* ACT_PMV */

create function string DN_ACT_PMV.rs_insert for HIS_class
go

create function string DN_ACT_PMV.rs_select for HIS_class
go

create function string DN_ACT_PMV.rs_select_with_lock for HIS_class
go

create function string DN_ACT_PMV.rs_update for HIS_class
go

create function string DN_ACT_PMV.rs_delete for HIS_class
output language ' '
go

/* ACT_RMQ */

create function string DN_ACT_RMQ.rs_insert for HIS_class
go

create function string DN_ACT_RMQ.rs_select for HIS_class
go

create function string DN_ACT_RMQ.rs_select_with_lock for HIS_class
go

create function string DN_ACT_RMQ.rs_update for HIS_class
go

create function string DN_ACT_RMQ.rs_delete for HIS_class
output language ' '
go

/* ACT_SIS */

create function string DN_ACT_SIS.rs_insert for HIS_class
go

create function string DN_ACT_SIS.rs_select for HIS_class
go

create function string DN_ACT_SIS.rs_select_with_lock for HIS_class
go

create function string DN_ACT_SIS.rs_update for HIS_class
go

create function string DN_ACT_SIS.rs_delete for HIS_class
output language ' '
go

/* ACT_TFM */

create function string DN_ACT_TFM.rs_insert for HIS_class
go

create function string DN_ACT_TFM.rs_select for HIS_class
go

create function string DN_ACT_TFM.rs_select_with_lock for HIS_class
go

create function string DN_ACT_TFM.rs_update for HIS_class
go

create function string DN_ACT_TFM.rs_delete for HIS_class
output language ' '
go

/* ACT_TUB */

create function string DN_ACT_TUB.rs_insert for HIS_class
go

create function string DN_ACT_TUB.rs_select for HIS_class
go

create function string DN_ACT_TUB.rs_select_with_lock for HIS_class
go

create function string DN_ACT_TUB.rs_update for HIS_class
go

create function string DN_ACT_TUB.rs_delete for HIS_class
output language ' '
go

/* GTC renovee */
/* ACT_GTC_SIG */

create function string DN_ACT_GTC_SIG.rs_insert for HIS_class
go

create function string DN_ACT_GTC_SIG.rs_select for HIS_class
go

create function string DN_ACT_GTC_SIG.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GTC_SIG.rs_update for HIS_class
go

create function string DN_ACT_GTC_SIG.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_ECL */

create function string DN_ACT_GTC_ECL.rs_insert for HIS_class
go

create function string DN_ACT_GTC_ECL.rs_select for HIS_class
go

create function string DN_ACT_GTC_ECL.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GTC_ECL.rs_update for HIS_class
go

create function string DN_ACT_GTC_ECL.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_VEN */

create function string DN_ACT_GTC_VEN.rs_insert for HIS_class
go

create function string DN_ACT_GTC_VEN.rs_select for HIS_class
go

create function string DN_ACT_GTC_VEN.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GTC_VEN.rs_update for HIS_class
go

create function string DN_ACT_GTC_VEN.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_EXP */

create function string DN_ACT_GTC_EXP.rs_insert for HIS_class
go

create function string DN_ACT_GTC_EXP.rs_select for HIS_class
go

create function string DN_ACT_GTC_EXP.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GTC_EXP.rs_update for HIS_class
go

create function string DN_ACT_GTC_EXP.rs_delete for HIS_class
output language ' '
go

/* ACT_GTC_INC */

create function string DN_ACT_GTC_INC.rs_insert for HIS_class
go

create function string DN_ACT_GTC_INC.rs_select for HIS_class
go

create function string DN_ACT_GTC_INC.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GTC_INC.rs_update for HIS_class
go

create function string DN_ACT_GTC_INC.rs_delete for HIS_class
output language ' '
go


/* ACT_GTC_ENR */

create function string DN_ACT_GTC_ENR.rs_insert for HIS_class
go

create function string DN_ACT_GTC_ENR.rs_select for HIS_class
go

create function string DN_ACT_GTC_ENR.rs_select_with_lock for HIS_class
go

create function string DN_ACT_GTC_ENR.rs_update for HIS_class
go

create function string DN_ACT_GTC_ENR.rs_delete for HIS_class
output language ' '
go


/* Fin GTC renovee */

/* ACT_TXT_TFM */

create function string DN_ACT_TXT_TFM.rs_insert for HIS_class
go

create function string DN_ACT_TXT_TFM.rs_select for HIS_class
go

create function string DN_ACT_TXT_TFM.rs_select_with_lock for HIS_class
go

create function string DN_ACT_TXT_TFM.rs_update for HIS_class
go

create function string DN_ACT_TXT_TFM.rs_delete for HIS_class
output language ' '
go

/* ALT_EXP */

create function string DN_ALT_EXP.rs_insert for HIS_class
go

create function string DN_ALT_EXP.rs_select for HIS_class
go

create function string DN_ALT_EXP.rs_select_with_lock for HIS_class
go

create function string DN_ALT_EXP.rs_update for HIS_class
go

create function string DN_ALT_EXP.rs_delete for HIS_class
output language ' '
go

/* EQT_DSP */

create function string DN_EQT_DSP.rs_insert for HIS_class
go

create function string DN_EQT_DSP.rs_select for HIS_class
go

create function string DN_EQT_DSP.rs_select_with_lock for HIS_class
go

create function string DN_EQT_DSP.rs_update for HIS_class
go

create function string DN_EQT_DSP.rs_delete for HIS_class
output language ' '
go

/* MES_POI */

create function string DN_MES_POI.rs_insert for HIS_class
go

create function string DN_MES_POI.rs_select for HIS_class
go

create function string DN_MES_POI.rs_select_with_lock for HIS_class
go

create function string DN_MES_POI.rs_update for HIS_class
go

create function string DN_MES_POI.rs_delete for HIS_class
output language ' '
go

/* MES_TRA */

create function string DN_MES_TRA.rs_insert for HIS_class
go

create function string DN_MES_TRA.rs_select for HIS_class
go

create function string DN_MES_TRA.rs_select_with_lock for HIS_class
go

create function string DN_MES_TRA.rs_update for HIS_class
go

create function string DN_MES_TRA.rs_delete for HIS_class
output language ' '
go

/* MES_KCS */

create function string DN_MES_KCS.rs_insert for HIS_class
go

create function string DN_MES_KCS.rs_select for HIS_class
go

create function string DN_MES_KCS.rs_select_with_lock for HIS_class
go

create function string DN_MES_KCS.rs_update for HIS_class
go

create function string DN_MES_KCS.rs_delete for HIS_class
output language ' '
go

/* MES_NEC */

create function string DN_MES_NEC.rs_insert for HIS_class
go

create function string DN_MES_NEC.rs_select for HIS_class
go

create function string DN_MES_NEC.rs_select_with_lock for HIS_class
go

create function string DN_MES_NEC.rs_update for HIS_class
go

create function string DN_MES_NEC.rs_delete for HIS_class
output language ' '
go

/* MES_QPC */

create function string DN_MES_QPC.rs_insert for HIS_class
go

create function string DN_MES_QPC.rs_select for HIS_class
go

create function string DN_MES_QPC.rs_select_with_lock for HIS_class
go

create function string DN_MES_QPC.rs_update for HIS_class
go

create function string DN_MES_QPC.rs_delete for HIS_class
output language ' '
go

/* ZDP_6MN */
create function string DN_ZDP_6MN.rs_insert for HIS_class
go

create function string DN_ZDP_6MN.rs_select for HIS_class
go

create function string DN_ZDP_6MN.rs_select_with_lock for HIS_class
go

create function string DN_ZDP_6MN.rs_update for HIS_class
go

create function string DN_ZDP_6MN.rs_delete for HIS_class
output language ' '
go

