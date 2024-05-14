/*E*/
/*  Fichier : $Id: xzae126sp.prc,v 1.10 2021/05/20 06:48:52 gesconf Exp $      Release : $Revision: 1.10 $        Date :  $Date: 2021/05/20 06:48:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae126sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture des dernieres fmc modifiees a exporter vers
* le PC strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou  08/05/97        : Creation
* JMG	17/06/04 : ne pas emettre les FMC ESSAI (SAGA)
* JPL	30/10/07 : Filtrer les FMC ayant pour origine 'Exercice' (DEM 719) 1.5
* JMG	15/09/08 : gestion diffusion_datex pour passerelle generique 1.6 DEM 835
* PNI	12/09/11 : On ne transmet plus les FMC en prévision (DATEX I) 1.7
* PNI	05/03/15 : Suppression des envoi des FMC sans date de debut 1.8 DEM1119
 PNI	16/11/16 : Suppression des envoi des FMC sans date de debut NI de fin 1.9 DEM1199
* PNI	17/05/21 : update à 1 des FMC non traitées (type non present dans CFG..ADA_EVT)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE126_Recherche_Dernieres_FMC
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE126' and type = 'P')
	drop procedure XZAE126
go


create procedure XZAE126
as
	declare @vl_evt int, @vl_cle tinyint, @vl_diff int

	create table #EXPORT (evenement int, cle tinyint, type smallint, diffusion_datex int)

	/*recherche FMC a exporter*/
	insert into #EXPORT
	select G.numero, G.cle, G.type, D.diffusion_datex
	from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..ADA_EVT E
	where G.numero = D.numero and G.cle = D.cle
	and D.autoroute = E.autoroute and G.type = E.type
	and D.PR <= E.fin and D.PR>=E.debut and (G.debut !=null or G.fin != null) 
	and (D.diffusion_datex=0 or D.diffusion_datex=null or D.diffusion_datex=2)
	and G.origine_creation not like '%ESSAI%'
	and G.origine_creation not like '%EXERCICE%'

	/*on passe diffusion_datex a 1 pour toutes les FMC trouvees*/
	declare exp_c cursor
	for select evenement, cle, diffusion_datex from #EXPORT

	open exp_c
	fetch exp_c into @vl_evt, @vl_cle, @vl_diff

	while (@@sqlstatus=0)
	begin
		if @vl_diff =0 or @vl_diff is null
			begin
			update EXP..FMC_HIS_DER set diffusion_datex = 1
			where numero = @vl_evt and cle = @vl_cle
		end
		else 
			begin
			update EXP..FMC_HIS_DER set diffusion_datex = 3
			where numero = @vl_evt and cle = @vl_cle
		end
		fetch exp_c into @vl_evt, @vl_cle, @vl_diff
	end
	close exp_c

        declare exp_d cursor
        for select distinct G.numero, G.cle, D.diffusion_datex
                from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..ADA_EVT E
                where G.numero = D.numero and G.cle = D.cle
                        and D.autoroute = E.autoroute and G.type != E.type
                        and D.PR <= E.fin and D.PR>=E.debut and (G.debut !=null or G.fin != null)
                        and (D.diffusion_datex=0 or D.diffusion_datex=null )

        open  exp_d
        fetch exp_d into @vl_evt, @vl_cle, @vl_diff

        while (@@sqlstatus=0)
        begin
                update EXP..FMC_HIS_DER set diffusion_datex = 1
                where numero = @vl_evt and cle = @vl_cle
                fetch exp_d into @vl_evt, @vl_cle, @vl_diff
        end
	close exp_d
	select evenement, cle, type from #EXPORT

	return XDC_OK

go

