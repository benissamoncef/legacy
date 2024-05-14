use PRC
go

drop proc altdaitoulon
go

create proc altdaitoulon
as

select convert(varchar,ALT.numero)+","+convert(varchar,rtrim( A.nom))+","+convert(varchar,PR)+","+convert(varchar,sens)+","+convert(char(10),horodate,103)+" "+convert(char(8),horodate,8)+","+ rtrim(T.nom)+","+convert(varchar, qualification )
from EXP..ALT_EXP ALT, CFG..RES_AUT A, CFG..TYP_ALT T, CFG..EQT_GEN E
where horodate > dateadd(day,-1,getdate()) and type_equipement=63 and equipement <> 170 and equipement=E.numero and E.type=63 and A.numero=E.autoroute and 
ALT.type=T.numero and T.numero not in (94,99,14)
order by ALT.numero
go
