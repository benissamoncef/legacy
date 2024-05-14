/*X  Fichier : @(#)RS_function_string_HIS_DA.bas	1.2      Release : 1.2        Date : 01/20/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR                           
------------------------------------------------------
* SOUS-SYSTEME  BASEDD                                
------------------------------------------------------
* FICHIER RS_function_string_HIS_DA.bas               
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :                       
*                                                     
*   installation de la replication : function string  
*                                                     
------------------------------------------------------
* HISTORIQUE :                                        
*                                                     
* JMG	08/01/98	: creation 1.1
* C.T.	16/01/98	: ajout ACT_PAL 1.2
------------------------------------------------------
*/

/* DA pour base historique CI */
/* FMC_NAT */

create function string DA_FMC_NAT.rs_insert for HIS_class
go

create function string DA_FMC_NAT.rs_select for HIS_class
go

create function string DA_FMC_NAT.rs_select_with_lock for HIS_class
go

create function string DA_FMC_NAT.rs_update for HIS_class
go

create function string DA_FMC_NAT.rs_delete for HIS_class
output language ' '
go
