# Fichier : $Id: rs_install_parms.DM,v 1.1 1994/09/30 07:32:19 gaborit Exp $        Release : $Revision: 1.1 $        Date : $Date: 1994/09/30 07:32:19 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# FICHIER rs_install_parms.DM ($SYBASE/replication)
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.GABORIT	30/09/94	: Creation
##################################################################################
ID_Server_name          RS_CI
ID_Server_user          MIGRAZUR
ID_Server_passwd        MIGRAZUR
RS_name                 RS_DM
RSSD_requires_ltm       y
RSSD_SQL_Server         SQL_DM
Create_RSSD             n
RSSD_sa_passwd          
RS_diskp_name           /dev/vg01/lvol6
RS_diskp_lname          RS_QUEUE
RS_diskp_size           128
# 
# Following are the other parameters with the default values.
# 
RSSD_database        DM_RSSD
RSSD_prim_user       DM_PRIM
RSSD_prim_passwd     DM_PRIM_PS
RSSD_maint_user      DM_MAIN
RSSD_maint_passwd    DM_MAIN_PS
RS_RS_user           MIGRAZUR
RS_RS_passwd         MIGRAZUR
RSSD_LTM_name        DM_RSSD_LTM
LTM_RS_user          DM_LTM
LTM_RS_passwd        DM_LTM_PS
LTM_admin_user       sa
LTM_admin_passwd 
# RSSD_DB_device       default
# RSSD_DB_size         10
# RSSD_LOG_device      default
# RSSD_LOG_size        10
#
# Following are paramaters that are optional, and are not used if not
# provided, so they don't have default values.
# 
# RSSD_DBO_user
# RSSD_DBO_passwd
# START_rs_id
# START_db_id
