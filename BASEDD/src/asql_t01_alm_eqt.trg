/*E*/
/*  Fichier : @(#)asql_t01_alm_eqt.trg	1.8      Release : 1.8        Date : 02/28/17
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t01_alm_eqt.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table ALM_EQT
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic  	04 Nov 94 : Creation
* gaborit 	11 nov 94 : ajout de drop trigger
* gaborit 	16 nov 94 : modif table
* torregrossa 	09 Sep 96 : Ajout cas sur DS et DC (RADT 1.4)
* torregrossa	03 Fev 98 : Re Correction gestion multi-sites dans les procs (1.5) (dem/1536)
* claudel       09 Jul 07 : Suppression DY (1.6)
* JMG           02/10/09        : secto DEM887 1/7
* LCL       26/09/2023  : CNA DEM-483
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* appel de AMRP07
*
* Code retour
* 
* n�ant
*
* Conditions d'utilisation
* base EXP
---------------------------------------------------*/

use EXP
go

drop trigger alm_eqt_iud_trg
go

create trigger alm_eqt_iud_trg
on ALM_EQT
for insert,update,delete
as
select 1
/*
   if @@servername = XDC_SQLCI
      exec TASRV_CI...AMRP07
   else
   if @@servername = XDC_SQLCA
      exec TASRV_CA...AMRP07
   else
   if @@servername = XDC_SQLVC
      exec TASRV_VC...AMRP07
   else
   if @@servername = XDC_SQLDP
      exec TASRV_DP...AMRP07
*/
go
