/* ======================================================== */
/* VERIF_TRF_DY_DP_FMC                               				*/
/* ESCOTA : PC N3 N2                             						*/
/* Vérification par des select qu'il n'y a plus de					*/ 
/* référence à DY dans les données FMC base HIS 						*/
/* Création : 06/2007  																			*/
/* ======================================================== */
use HIS
go

/* Table FMC_GEN */
select * from FMC_GEN
where cle = 23

/* Tables liées */
select * from ACT_GEN
where cle = 23

select * from ALT_EXP
where cle = 23

select * from FMC_ACC
where cle = 23
/*
select * from FMC_ACT
where cle = 23

select * from FMC_ADA
where cle = 23
*/
select * from FMC_BAS
where cle = 23

select * from FMC_BOU
where cle = 23

select * from FMC_COM
where cle = 23

select * from FMC_CNC
where cle = 23

select * from FMC_DAT
where cle = 23

select * from FMC_DEL
where cle = 23

select * from FMC_ECH
where cle = 23
/*
select * from FMC_TFM
where cle = 23

select * from FMC_TRS
where cle = 23
*/
select * from FMC_HIS
where cle = 23

select * from FMC_LIS
where cle = 23

select * from FMC_MET
where cle = 23

select * from FMC_MNF
where cle = 23

select * from FMC_TRF
where cle = 23

select * from FMC_VEH
where cle = 23

select * from FMC_NAT
where cle = 23 

select * from FMC_OPR
where cle = 23
/*
select * from FMC_SAT
where cle = 23
*/
select * from FMC_TRH
where cle = 23

select * from FMC_VLN
where cle = 23

go
