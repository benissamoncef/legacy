/*E*/
/*  Fichier : $Id: xzae157sp.prc,v 1.2 2019/01/17 18:15:02 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/01/17 18:15:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae157.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* recuperation du numero de queue de bouchon
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	17/0616 : creation SAGA
* JMG	14/06/18 : cumul meme si pas saga 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* recuperation du numero de queue de bouchon 
* 
* Sequence d'appel
* SP	XZAE157_Recuperer_Numero_Queue
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK : l'evenement en entree n'etait pas une tete de bouchon
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
-----------------------------------------------------*/

use PRC
go

drop procedure XZAE157
go

create procedure XZAE157
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Type_in		tinyint,
	@va_ComFMCInit_in	char(50),
	@va_Longueur_out	int = null	output
as
	declare @vl_Horodate		datetime,
		@vl_Classe		tinyint,
		@vl_NumEvtQueue		int,
		@vl_NumEvtTete		int, @vl_PR int, @vl_PR1 int,
		@vl_numero_cause	int, @vl_autoroute tinyint, @vl_autoroute1 tinyint,
		@vl_type 		tinyint,
		@vl_prq			int,
		@vl_cle_cause		int,
		@vl_numero_cause2	int,
		@vl_longueur_reelle	int,
		@vl_cle_cause2		tinyint,
		@vl_CleEvtQueue		tinyint,
		@vl_CleEvtTete		tinyint,
		@vl_Status		int,
		@vl_comfmc_init		char(100)

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
	begin
		select @va_Longueur_out=0
		return XDC_ARG_INV
	end

	/* si la FMC n est pas une tete ou une queue de bouchon*/
	/*on sort*/
	if @va_Type_in <> XZAEC_FMC_QueueBouchon and @va_Type_in <> XZAEC_FMC_TeteBouchon 
	begin
		select @va_Longueur_out=0
		return XDC_ARG_INV
	end

	/*si la FMC est une tete de bouchon*/
	if @va_Type_in = XZAEC_FMC_TeteBouchon
	begin
		select @vl_numero_cause = cause, @vl_cle_cause = cle_cause
		from EXP..FMC_GEN
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		/*si pas de cause*/
		if (@vl_numero_cause is null)
		begin
			select @va_Longueur_out=0
			return XDC_OK
		end
	end
	else
	begin
		/*on est la, donc c'est une queue de bouchon*/
		/*il faut aller chercher la cause de la tete de la FMC SAE*/
		/*d abord je recupere la tete*/
		select @vl_numero_cause = cause, @vl_cle_cause = cle_cause
		from EXP..FMC_GEN
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		/*puis je cherche la cause de la tete*/
		select @vl_numero_cause = cause, @vl_cle_cause = cle_cause
		from EXP..FMC_GEN
		where numero = @vl_numero_cause and cle = @vl_cle_cause

		/*si pas de cause*/
		if (@vl_numero_cause is null)
		begin
			select @va_Longueur_out=0
			return XDC_OK
		end
	end

	/*recupere les infos de la FMC SAGA*/
	select @vl_comfmc_init = com_fmc_init, @vl_type = type, @vl_numero_cause2=cause,
		@vl_cle_cause2 = cle_cause
	from EXP..FMC_GEN
	where numero = @vl_numero_cause and cle =  @vl_cle_cause

	/*si FMC SAGA n est pas un bouchon*/
	if @vl_type <> XZAEC_FMC_QueueBouchon
	begin
		select @va_Longueur_out=0
		return XDC_OK
	end

	select @vl_numero_cause, @vl_cle_cause
	exec PRC..XZAE;51 null,@vl_numero_cause, @vl_cle_cause,
			@vl_prq output, @vl_type output,  @va_Longueur_out output, @vl_numero_cause2 output, @vl_cle_cause2 output

	/*la FMC SAGA cause est un bouchon, je calcule sa longueur*/
	/*recherche de la localisation de la tete de bouchon*/
	/*select @va_Longueur_out = longueur
	from EXP..FMC_HIS_DER where numero= @vl_numero_cause and cle = @vl_cle_cause*/

	return XDC_OK
go
