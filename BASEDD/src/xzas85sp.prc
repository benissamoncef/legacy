/*E*/
/*  Fichier : $Id: xzas85sp.prc,v 1.1 2008/11/17 11:26:38 pc2dpdy Exp $      Release : $Revision: 1.1 $      Date : $Date: 2008/11/17 11:26:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas82sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module aweb : Retourne la liste des Q, V, T_O ... pour TDP
* Remarque :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/836
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------

* Sequence d"appel
* SP    XZAS82
*
* Arguments en entree
*
* Arguments en sortie
*
* aucun :
*
* Service rendu
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d"entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d"utilisation
*
*
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS85' and type = 'P')

    drop procedure XZAS85

go

create procedure XZAS85 
	@va_numero_in	smallint,
	@va_sens_in	tinyint
as

declare
@vl_i smallint,
@vl_debut datetime,
@vl_fin   datetime,
@vl_horodate_recente char(12)

   create table #TMP_RADT
	 (
		identifiant  char(20) NULL ,
		horodate     char(50) NULL ,
		sens         int NULL ,
		validite     int NULL ,
                Q            int NULL ,
		QPL          int NULL ,
		V            int NULL ,
		TOC           int NULL,
         )

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/

/* calcul de la tranche de date a traiter */
select @vl_debut = dateadd(minute,-18,getdate())
select @vl_fin =   getdate()

/* insertion dans table temporaire des valeurs pour les stations */
insert into #TMP_RADT
select G.nom, convert(char(8),T.horodate,112)+ convert(char(2),(convert(char(2),T.horodate,8)))+ substring(convert(char(8),T.horodate,8),4,2),
T.sens,1,Q,convert(smallint,convert(float,Q)/100*PL),V,T_O
from EXP..MES_TRA T,CFG..EQT_GEN G,CFG..RES_AUT A
where G.numero=T.station and G.type=26 and G.autoroute=A.numero and T.station=@va_numero_in
and T.sens=@va_sens_in and T.voie=0 and T.horodate between @vl_debut and @vl_fin

/* on cherche l'horodate la plus recente pour masquer les lenteurs de mise en base */
select @vl_horodate_recente=max(horodate) from #TMP_RADT

/*valeurs a retourner*/

select @vl_i=count(*) from #TMP_RADT

if @vl_i=0
	begin
	select G.nom, convert(char(8),@vl_fin,112)+ convert(char(2),(convert(char(2),@vl_fin,8)))+ substring(convert(char(8),@vl_fin,8),4,2),@va_sens_in,0,0,0,0,0
	from CFG..EQT_GEN G
	where G.numero=@va_numero_in and G.type=26
end
else
begin
select  identifiant,@vl_horodate_recente,sens,validite,Q,QPL,V,TOC  
      from #TMP_RADT group by identifiant,sens having horodate = max(horodate) order by identifiant, sens
end

drop table #TMP_RADT

return (XDC_OK)

go

