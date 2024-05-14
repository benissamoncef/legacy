/*E*/
/*  Fichier : @(#)xzae554sp.prc	1.3      Release : 1.3      Date : 02/21/05
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae554sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
* rechercher les evts a mettre sur le WEB
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/835
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE554
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

if exists (select * from sysobjects where name = 'XZAE554' and type = 'P')
	drop procedure XZAE554
go

create procedure XZAE554
as

/*premiere recherche des FMC a exporter (avec premier filtre)*/
create table #TMP_WEB_FMC (
	numero int null,
	cle tinyint null,
	type smallint null,
	horodate_validation datetime null,
	VR tinyint null,
	VM2 tinyint null,
	VM1 tinyint null,
	VL tinyint null,
	BAU tinyint null,
	VR_I tinyint null,
	VM2_I tinyint null,
	VM1_I tinyint null,
	VL_I tinyint null,
	BAU_I tinyint null,
	confirme tinyint null,
	autoroute tinyint null,
	PR int null,
	sens tinyint null,
	point_caracteristique tinyint null,
	numero_point_caracteristique tinyint null,
	longueur int null)

create unique index WEB_FMC_PK on #TMP_WEB_FMC (numero, cle)

/*premiere recherche des FMC a exporter (avec premier filtre)*/
insert into #TMP_WEB_FMC
select	G.numero,
	G.cle,
	G.type,
	H.horodate_validation,
	H.VR,
	H.VM2,
	H.VM1,
	H.VL,
	H.BAU,
	H.VR_I,
	H.VM2_I,
	H.VM1_I,
	H.VL_I,
	H.BAU_I,
	H.confirme_ou_signale,
	H.autoroute,
	H.PR,
	H.sens,
	H.point_caracteristique,
	H.numero_point_caracteristique,
	0

from EXP..FMC_GEN G, EXP..FMC_HIS_DER H

where  (G.type in (select type from CFG..WEB_FMC) or G.type=XZAEC_FMC_QueueBouchon or G.type=XZAEC_FMC_Deviation)
and    G.datex = XDC_FAUX
and    G.debut is not null
and    G.fin is NULL
and    G.numero = H.numero
and    G.cle = H.cle
and    H.confirme_ou_signale = 1
and    G.origine_creation not like "%ESSAI%"

select * from #TMP_WEB_FMC

drop table #TMP_WEB_FMC

return (XDC_OK)

go
