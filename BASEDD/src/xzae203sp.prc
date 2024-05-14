/*E*/
/*  Fichier : @(#)xzae203sp.prc	     Release :       Date :
------------------------------------------------------
*  ESCOTA *  PROJET PASTRE 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae203sp.prc
------------------------------------------------------
* DESCRIPTION:
*
* cherche toutes les FMC causes/consequences de la FMC passee en arg entree
* et renvoie pour chacune trouvee son site, num, type, aut, PR et sens
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher 07/03/05 : Creation
* JPL	22/08/12 : Simple renommage de constantes  1.2
* PNI	01/08/2014	: Limiter la profondeur de recherche d'évènements DEM1093 v1.3
*
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE203
* 
* Arguments en entree
* 
* 1. num_fmc: numero de la FMC
* 2. cle_fmc: cle de la FMC
*
* Arguments en sortie
*
* liste de toutes les FMC causes/consequences de la FMC passee en arg entree
* et renvoie pour chacune trouvee son site, num, type, aut, PR et sens         
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction



-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE203' and type = 'P')
	drop procedure XZAE203
go


create procedure XZAE203
		 @va_num_fmc int =null,
                 @va_cle_fmc tinyint =null
as

declare 
	@vl_numero_fmc     int          ,
	@vl_cle            tinyint      ,
	@vl_type           smallint     ,
	@vl_autoroute      tinyint      ,
	@vl_PR             int          ,
	@vl_sens           tinyint      ,
	@vl_horodate       datetime     ,
    	@vl_cause          int          ,
	@vl_cle_cause      tinyint      ,
	@vl_compte         int		


/* Transfo en parametres nulls bug applix */
if @va_num_fmc = 0 select @va_num_fmc = null
if @va_cle_fmc = 0 select @va_cle_fmc = null

/*analyse des parametres d'entree */
if (@va_num_fmc = null or @va_cle_fmc =null)
	return XDC_ARG_INV

/*recherche des FMC causes/consequences */

create table #TMP_LISTE_FMC (	numero int null , 
				cle tinyint null , 
				type smallint null , 
				cause int null ,
				cle_cause tinyint null )

begin
  insert into #TMP_LISTE_FMC 
    select distinct G.numero,G.cle,G.type,G.cause,G.cle_cause
      from
#ifdef HIST
      HIS..FMC_GEN G
#else
      EXP..FMC_GEN G
#endif
      where G.numero = @va_num_fmc and G.cle = @va_cle_fmc

    /*select distinct numero,cle,type,cause,cle_cause from #TMP_LISTE_FMC*/

  /* on recherche les causes de facon iterative */   
  declare recherche_cause cursor for
    select distinct 
       T.numero, T.cle, T.type, T.cause , T.cle_cause
    from #TMP_LISTE_FMC T
    where T.cause is not null

continue_a_rechercher_causes:
  /* ouverture curseur */
  open recherche_cause
  fetch recherche_cause into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause

  /* boucle curseur */
  while (@@sqlstatus = 0 )
  begin
    insert into #TMP_LISTE_FMC 
    select 
      G.numero,G.cle,G.type,G.cause,G.cle_cause
    from 
#ifdef HIST
      HIS..FMC_GEN G
#else 
      EXP..FMC_GEN G
#endif
    where G.numero = @vl_cause and G.cle=@vl_cle_cause

    update #TMP_LISTE_FMC
    set cause=null where numero=@vl_numero_fmc and cle=@vl_cle
    fetch recherche_cause into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause
  end

  /* fermeture curseur */
  close recherche_cause

  /* recherche d'autres causes eventuelles */
  select @vl_compte=count(*) from #TMP_LISTE_FMC where cause is not null
  if @vl_compte != 0 goto continue_a_rechercher_causes

  /* filtre on enleve les evenements fils */
  delete #TMP_LISTE_FMC where numero is null or (cle_cause is not null and cause is null)

  /* trace clu de tous les evenements racine */ 
  /*if @va_trace_in is not null
    select distinct numero,cle,type,cause,cle_cause from #TMP_LISTE_FMC*/

  /* a ce stade on a toutes les causes premieres. on va rechercher toutes les FMC filles */
  declare recherche_consequences cursor for
    select distinct 
       T.numero, T.cle, T.type, T.cause , T.cle_cause
    from #TMP_LISTE_FMC T
    where T.cause is null
continue_a_rechercher_conseq:
  /* ouverture curseur */
  open recherche_consequences
  fetch recherche_consequences into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause

  /* boucle curseur */
  while (@@sqlstatus = 0 )
  begin
    insert into #TMP_LISTE_FMC 
    select 
      G.numero,G.cle,G.type,null, null           
    from 
#ifdef HIST 
      HIS..FMC_GEN G
#else
      EXP..FMC_GEN G
#endif
    where G.cause = @vl_numero_fmc and G.cle_cause=@vl_cle
    update #TMP_LISTE_FMC set cause=1 where numero=@vl_numero_fmc and cle= @vl_cle
    fetch recherche_consequences into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause
  end

  /* fermeture curseur */
  close recherche_consequences

  /* recherche d'autres consequences eventuelles */
  select @vl_compte=count(*) from #TMP_LISTE_FMC where cause is null
  if @vl_compte != 0 goto continue_a_rechercher_conseq
end

   /*filtre: on enleve la FMC bouchon(tete) qd FMC bouchon(queue)*/
  delete #TMP_LISTE_FMC where type = XZAEC_FMC_TeteBouchon
  
  select distinct F.numero, F.cle, Y.nom, A.nom, H.PR, P.libelle, convert(char(10),F.debut,103)+' '+convert(char(8),F.debut,8),convert(char(10),F.fin,103)+' '+convert(char(8),F.fin,8)
	 from
#ifdef HIST
	HIS..FMC_GEN F, HIS..FMC_HIS H, #TMP_LISTE_FMC T, CFG..RES_AUT A, CFG..TYP_FMC Y, CFG..LIB_PRE P 
#else
	EXP..FMC_GEN F, EXP..FMC_HIS_DER H, #TMP_LISTE_FMC T, CFG..RES_AUT A, CFG..TYP_FMC Y, CFG..LIB_PRE P
#endif
	where H.numero = F.numero and 
	H.cle = F.cle and 
	F.numero = T.numero and 
	F.cle = T.cle and
	H.autoroute = A.numero and
	F.type = Y.numero and
	H.sens = P.code and 
	P.notyp = XDC_LIB_SENS_LOC


return (XDC_OK)

go
