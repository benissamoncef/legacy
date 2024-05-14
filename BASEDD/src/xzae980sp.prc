/*E*/
/*  Fichier : @(#)xzae980sp.prc	1.2      Release : 1.2        Date : 10/02/12
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae980sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* recherche d un evenements communautaire lie a une fiche main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU	10/02/12 : DEM 1015 - Creation	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* recherche de la FMC liee a l'evenement communaitaire
* 
* Sequence d'appel
* SP	XZAE980_Recherche_FMC_Liee
* 
* Arguments en entree
* XDY_Evt_Commu        va_Evt_Commu_in		: evenement communautaire
* 
* Arguments en sortie
* XDY_Entier          va_NumeroEvt_out : numero FMC
* XDY_Octet           va_CleEvt_out    : cle FMC
* XDY_Booleen         va_Encours_out   : indicateur en cours
* XDY_Tendance_Comm   va_Tendance_out  : tendance
* XDY_Synthese_Commu  va_Synthese_out  : synthese
* 
* Code retour
* XDC_OK
* XDC_NOK		: pas de FMC liee
* XDC_ARG_INV		: parametres d'entree null
* 
* Conditions d'utilisation
*
* Fonction
	@va_Evt_Commu_in	int	= null,
	@va_NumeroEvt_out   int = null output,
	@va_CleEvt_out	tinyint	= null output,
	@va_encours_out	tinyint	= null output,
	@va_tendance_out    int = null output,	
	@va_synthese_out    int = null output
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE980
go

create procedure XZAE980
	@va_Evt_Commu_in	int	= null,
	@va_synthese_in	int	= null,
	@va_tendance_in	int	= null,
	@va_NumeroEvt_out   int = null output,
	@va_CleEvt_out	tinyint	= null output,
	@va_encours_out	tinyint	= 0 output,
	@va_tendance_out    int = null output,	
	@va_synthese_out    int = null output
as
	declare @vl_cloture		datetime
	
	/*A controle parametres en entree */
	if @va_Evt_Commu_in = null
		return XDC_ARG_INV

	select @va_NumeroEvt_out = commu.evenement, 
		   @va_CleEvt_out = commu.cle, 
		   @va_tendance_out = commu.tendance, 
		   @va_synthese_out = commu.synthese,
		   @vl_cloture = fmc.fin
	from EXP..FMC_COMMU as commu, EXP..FMC_GEN as fmc
	where commu.id_commu = @va_Evt_Commu_in
	and fmc.numero = commu.evenement
	and fmc.cle = commu.cle

	if @@rowcount = 0
	begin
		select @va_NumeroEvt_out=-1, @va_CleEvt_out=0
		return XDC_OK
	end

	if @vl_cloture = null
		select @va_encours_out = 1
		
	if @va_encours_out = 1
	begin
	update EXP..FMC_COMMU 
	set tendance=@va_tendance_in, synthese=@va_synthese_in
	where id_commu = @va_Evt_Commu_in
	end
	return XDC_OK
go
