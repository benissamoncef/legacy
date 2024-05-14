/* ======================================================== */
/* VERIF_TRF_DY_DP_ACTION                            				*/
/* ESCOTA : PC N3 N2                             						*/
/* Vérification par des select qu'il n'y a plus de					*/ 
/* référence à DY dans les données ACTION base HIS 					*/
/* Création : 06/2007  																			*/
/* ======================================================== */
use HIS
go

/* Table ACT_GEN */
select * from ACT_GEN
where cle = 23

/* Tables liées */
select * from ACT_APL
where actio in (select numero from ACT_GEN where cle = 23)

select * from ACT_BAD
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_BAF
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_DEP
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_ECH
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_EXP_ADA
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_FAX
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_FRG
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_GAR
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_INT
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_NAV
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_PMA
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_PMV
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_PMV_ADA
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_RMQ
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_SIS
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_TFM
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_TFM_EXP
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_TUB
where actio in (select numero from ACT_GEN where cle = 23)
		
select * from ACT_TXT_TFM
where actio in (select numero from ACT_GEN where cle = 23)
		
/*
select * from ADA_ACT
where actio in (select numero from ACT_GEN where cle = 23)
*/		
/* N'EXISTE PAS ENCORE */
/*		
select * from ACT_EXP
where actio in (select numero from ACT_GEN where cle = 23)
*/				
go
