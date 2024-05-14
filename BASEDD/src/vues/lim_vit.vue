/* ========================================================== */
/* STERIA *  PROJET MIGRAZUR	*/
/* ========================================================== */
/* SOUS-SYSTEME  BASEDD */
/* ========================================================== */
/* FICHIER lim_vit.vue	*/
/* ========================================================== */
/* L.V.	22/08/96	Creation			*/
/* ========================================================== */

/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'LIM_VIT1' and type = 'V')
        begin
        drop view LIM_VIT1
        end
go

/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'LIM_VIT2' and type = 'V')
        begin
        drop view LIM_VIT2
        end
go


create view LIM_VIT1 as
select POR1.autoroute,POL1.portion,POL1.sens,POL1.PR,POL1.vitesse,PR_fin=
(
select min(POL2.PR)
from CFG..RES_POL POL2,CFG..RES_POR POR2
where
POL2.PR>POL1.PR
and POR2.numero=POL2.portion
and POR2.autoroute=POR1.autoroute
)
from CFG..RES_POL POL1,CFG..RES_POR POR1
where
POR1.numero=POL1.portion

go

create view LIM_VIT2 as
select POR3.autoroute,POL3.portion,POL3.sens,POL3.PR,POL3.vitesse,PR_fin=POR3.PR_fin
from CFG..RES_POL POL3,CFG..RES_POR POR3
where
POR3.numero=POL3.portion
group by POR3.autoroute,POL3.sens
having
POR3.numero=POL3.portion
and POL3.PR=max(POL3.PR)

go
