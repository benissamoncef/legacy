/*E*/
/*  Fichier : $Id: vuem02sp.prc,v 1.15 2017/03/09 15:32:12 pc2dpdy Exp $        Release : $Revision: 1.15 $        Date : $Date: 2017/03/09 15:32:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	18/07/95	: Creation
* C.T.	29/09/95	: Modif nom proc (V 1.3)
* C.T.	11/12/95	: pb de coherence inter vue (V1.4)
* C.T.	07/06/96	: supprimer les basculements lies aux travaux pour
*                         le comptage et la proc effectue uniquement le
*                         recapitulatif des actions d'exploitation/evenement par
*                         district (V1.5-1.6)
* C.T.	06/11/96	: Ajout nombre de fausses alertes en sortie (FMC18) (V1.7)
* C.T.	24/02/98	: Modif pour pb default suite a passage en SQL 11.0.3.2 (V1.8)
* P.N.	21/09/98	: suppression des consequence trvx uniqt si basculement dem/1716 v1.10
* P.N.	05/02/99	: Modif interg. dem/1724 v1.11
* P.N.	26/03/99	: Modif interg. dem/1724 v1.12
* P.N	19/04/99	: Correction dem/1759 v1.12
* JPL	14/09/10	: Adaptation version compilateur C ('#' en debut de ligne) 1.13
* JPL	08/07/16	: Support regions pour criteres geographiques; compatible precedente (DEM 1171)  1.14
* JMG   09/03/17 	: REGIONALISATION - DEM1220 1.15
* LCL	28/09/23 	: Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM02_Recapitulatif_exploitation
* 
* Arguments en entree
* XDY_Octet	va_Debut_in
* XDY_Octet	va_Fin_in
* XDY_Octet	va_ModeDistrict_in
* 
* Arguments en sortie
*
* liste retournee
* XDC_NOM	type
* XDC_Entier	nombre
* XDC_Octet	mois
* 
* Service rendu
* recapitulatif des actions d'exploitation/evenement pour un 
* district par type/par mois 
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'VUEM02' and type = 'P')
	drop procedure VUEM02
go


create procedure VUEM02
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_District_in		tinyint		= null
as
	declare @vl_Fin datetime, @vl_Debut datetime, 
		@vl_MoisFin tinyint, @vl_Mois tinyint, @vl_Type smallint,
		@vl_Site T_SITE, @vl_DebutSuivant datetime,
		@vl_NombreZero int, @vl_NomType T_NOM

	if @va_District_in = 0 select @va_District_in = null

	/*A controle des parametres d'entree */
	if @va_Debut_in is null or @va_Fin_in is null
		return XDC_ARG_INV

	select @vl_NombreZero = 0

	create table #LISTE_FMC (type char(25) default XDC_CHAINE_VIDE, 
				district tinyint null, nombre int, mois tinyint, nb_fausse_alerte int DEFAULT 0)

	create table #TEMPO_FMC (numero int,cle tinyint, cause int null, cle_cause tinyint null, district tinyint null,
				fausse_alerte tinyint null)

	/*A rechercher les evenements par type et par mois pour un district */
	create table #TEMPO_FMC2 (numero int,cle tinyint, autoroute tinyint null , PR int null, cause int null, cle_cause tinyint null,
				fausse_alerte tinyint null)


	/* creer et renseigner une table temporaires des sites (regions, secteurs, districts) */
	create table #LISTE_SITES ( numero tinyint null )

	insert #LISTE_SITES ( numero )  values ( @va_District_in )


	/*A Initialiser au premier type de fmc */
	select @vl_Type = XZAEC_FMC_Accident

	while @vl_Type <= XZAEC_FMC_NB_TYP
	begin
		if @vl_Type != XZAEC_FMC_PosteOperateur and
		   @vl_Type != XZAEC_FMC_Travaux and
		   @vl_Type != XZAEC_FMC_TeteBouchon and
		   @vl_Type != XZAEC_FMC_Regulation
		begin
			select @vl_Debut = @va_Debut_in
			select @vl_Fin = dateadd(month,1,@va_Debut_in),
				@vl_Mois = datepart(month, @vl_Debut),
				@vl_MoisFin = datepart(month, @va_Fin_in)
		
			while @vl_Mois <= @vl_MoisFin
			begin
				/*A rechercher les fmc d'un type par mois */
				insert #TEMPO_FMC2 (numero,cle, autoroute, PR, cause, cle_cause, fausse_alerte)
				select distinct
					HIS..FMC_GEN.numero,
					HIS..FMC_GEN.cle,
					HIS..FMC_HIS.autoroute,
					HIS..FMC_HIS.PR,
					HIS..FMC_GEN.cause,
					HIS..FMC_GEN.cle_cause,
					HIS..FMC_GEN.fausse_alerte
				from HIS..FMC_GEN, HIS..FMC_HIS
				where	type = @vl_Type and
					((datex<>XDC_VRAI and  @va_District_in <> 99) or
					 (datex=XDC_VRAI and  @va_District_in=99)) and
					debut >= @vl_Debut and debut < @vl_Fin and
					HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
					HIS..FMC_HIS.cle=HIS..FMC_GEN.cle
				group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, 
					HIS..FMC_GEN.numero, HIS..FMC_GEN.cle
				having horodate_validation = max(horodate_validation)

				if  @va_District_in <> 99
				begin
					insert #TEMPO_FMC (numero,cle, cause, cle_cause, district, fausse_alerte)
					select distinct #TEMPO_FMC2.numero, #TEMPO_FMC2.cle, #TEMPO_FMC2.cause, #TEMPO_FMC2.cle_cause,
					                CFG..RES_POR.district, #TEMPO_FMC2.fausse_alerte
					from #TEMPO_FMC2, CFG..RES_POR
					where	CFG..RES_POR.autoroute=#TEMPO_FMC2.autoroute and
						CFG..RES_POR.PR_debut <=#TEMPO_FMC2.PR and
						CFG..RES_POR.PR_fin > #TEMPO_FMC2.PR and
						(@va_District_in is null or CFG..RES_POR.district in (select numero from #LISTE_SITES) )
				end
				else
				begin
					insert #TEMPO_FMC (numero,cle, cause, cle_cause, district, fausse_alerte)
					select distinct #TEMPO_FMC2.numero, #TEMPO_FMC2.cle, #TEMPO_FMC2.cause, #TEMPO_FMC2.cle_cause,
					                99, #TEMPO_FMC2.fausse_alerte
					from #TEMPO_FMC2
				end
				
				/* rechercher les fmc en limites de concession */
				delete #TEMPO_FMC2
				from #TEMPO_FMC2, #TEMPO_FMC
				where #TEMPO_FMC2.numero = #TEMPO_FMC.numero and #TEMPO_FMC2.cle = #TEMPO_FMC.cle

				delete #TEMPO_FMC2
				from #TEMPO_FMC2, CFG..RES_POR
				where #TEMPO_FMC2.PR = CFG..RES_POR.PR_debut

				if  @va_District_in <> 99
				begin
					insert #TEMPO_FMC (numero,cle, cause, cle_cause, district, fausse_alerte)
					select distinct #TEMPO_FMC2.numero, #TEMPO_FMC2.cle, #TEMPO_FMC2.cause, #TEMPO_FMC2.cle_cause,
					                CFG..RES_POR.district, #TEMPO_FMC2.fausse_alerte
					from #TEMPO_FMC2, CFG..RES_POR
					where	CFG..RES_POR.autoroute=#TEMPO_FMC2.autoroute and
						CFG..RES_POR.PR_fin = #TEMPO_FMC2.PR and
						(@va_District_in is null or CFG..RES_POR.district in (select numero from #LISTE_SITES) )
				end
				else
				begin
					insert #TEMPO_FMC (numero,cle, cause, cle_cause, district, fausse_alerte)
					select distinct #TEMPO_FMC2.numero, #TEMPO_FMC2.cle, #TEMPO_FMC2.cause, #TEMPO_FMC2.cle_cause,
					                99, #TEMPO_FMC2.fausse_alerte
					from #TEMPO_FMC2 
				end
				
				/*A Supprimer les basculements dus a des travaux */
				if @vl_Type = XZAEC_FMC_Basculement
				begin
				delete #TEMPO_FMC
				from #TEMPO_FMC , HIS..FMC_GEN
				where #TEMPO_FMC.cause is not null and HIS..FMC_GEN.numero = #TEMPO_FMC.cause and
					HIS..FMC_GEN.cle = #TEMPO_FMC.cle_cause and 
					HIS..FMC_GEN.type = XZAEC_FMC_Travaux and
					((HIS..FMC_GEN.datex<>XDC_VRAI and @va_District_in <> 99)  or
					 (HIS..FMC_GEN.datex=XDC_VRAI and @va_District_in=99))
				end

				/*A recherche du nom du type */
				select	@vl_NomType = nom 
				from CFG..TYP_FMC
				where numero = @vl_Type

				/*A comptage des evenements de ce type */
				insert #LISTE_FMC (type, nombre, mois, district, nb_fausse_alerte)
				select
					@vl_NomType,
					count(*),
					@vl_Mois,
					district,
					isnull(sum(fausse_alerte),0)
				from #TEMPO_FMC
				group by district

				/*A ajout des compteurs a zero pour les districts n'ayant pas ce type d'evenment */
				if @va_District_in is null
					insert #LISTE_FMC (type, nombre, mois, district, nb_fausse_alerte)
					select
						@vl_NomType,
						@vl_NombreZero,
						@vl_Mois,
						numero,
						0
					from CFG..RES_DIS
					where numero != XDC_CI and type != 0 and
						numero not in (select distinct district from #TEMPO_FMC)
				else if not exists (select district from #TEMPO_FMC
						where district = @va_District_in )
					insert #LISTE_FMC (type, nombre, mois, district, nb_fausse_alerte)
					values ( @vl_NomType, @vl_NombreZero, @vl_Mois, @va_District_in, 0 )
		
				/* passage au mois suivant */
				select @vl_DebutSuivant = dateadd(month,1,@vl_Debut)
				select @vl_Fin = dateadd(month,1,@vl_DebutSuivant)
				select @vl_Mois = @vl_Mois + 1, 
					@vl_Debut = @vl_DebutSuivant
				delete #TEMPO_FMC
				delete #TEMPO_FMC2
			end
		end

		/* passage au type de fmc suivant */
		select @vl_Type = @vl_Type + 1
	end
	
	/*A retourner la liste des infos */
	select
		type, 
		nombre, 
		mois,
		district,
		nb_fausse_alerte
	from #LISTE_FMC
	order by district, mois, type

	return XDC_OK
go
