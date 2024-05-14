/*E*/
/*  Fichier : $Id: xzae97sp.prc,v 1.2 1998/09/22 13:28:36 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/09/22 13:28:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae97.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* recuperation du numero de queue de bouchon
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	22/09/96	: Creation
* PN	29/06/98	: separation EXP/HIS 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* recuperation du numero de queue de bouchon 
* 
* Sequence d'appel
* SP	XZAE97_Recuperer_Numero_Queue
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
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

create procedure XZAE;97
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_NumEvt_out		int = null	output,
	@va_CleEvt_out		tinyint = null	output
as
	declare @vl_Horodate		datetime,
		@vl_Classe		tinyint,
		@vl_NumEvtQueue		int,
		@vl_NumEvtTete		int,
		@vl_CleEvtQueue		tinyint,
		@vl_CleEvtTete		tinyint,
		@vl_Status		int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A prendre l'heure courante dans le cas ou elle est nulle */
	if @va_Horodate_in = null
		select @va_Horodate_in = XDC_DATE_INFINIE

#ifdef HIST
		/*A recherche le type de l'evenement */
		select
			@vl_Classe = CFG..TYP_FMC.classe
		from HIS..FMC_GEN, CFG..TYP_FMC
		where HIS..FMC_GEN.numero = @va_NumEvt_in and HIS..FMC_GEN.cle = @va_CleEvt_in and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	
		if @@rowcount = 0 or (@vl_Classe != XZAEC_ClasseGenerique and
		   @vl_Classe != XZAEC_ClasseBouchon)
			return XDC_NOK
	
		if @vl_Classe = XZAEC_ClasseGenerique
		begin
			/* l'evenement en entree est la tete */
			select	@vl_NumEvtTete = @va_NumEvt_in, 
				@vl_CleEvtTete = @va_CleEvt_in
	
			/*B recherche du numero de la fiche conseq de la tete */
			/*B de bouchon */
			select
				@vl_NumEvtQueue = HIS..FMC_GEN.numero,
				@vl_CleEvtQueue = HIS..FMC_GEN.cle
			from HIS..FMC_GEN, CFG..TYP_FMC
			where 	HIS..FMC_GEN.cause = @vl_NumEvtTete and 
				HIS..FMC_GEN.cle_cause = @vl_CleEvtTete and
				CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClasseBouchon
	
			if @@rowcount = 0
				return XDC_NOK
	
			/*B valorisation des variables de sortie */
			select @va_NumEvt_out = @vl_NumEvtQueue,
				@va_CleEvt_out = @vl_CleEvtQueue
		end
		/* l'evenement en entree est la queue*/
		else
		begin
			return XDC_NOK
		end
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE;97
			@va_Horodate_in	,
			@va_NumEvt_in	,
			@va_CleEvt_in	,
			@va_NumEvt_out	output,
			@va_CleEvt_out	output

		if @vl_Status = null
                       	return XDC_PRC_INC
               	else return @vl_Status
	end
	else
	begin
		/*A recherche le type de l'evenement */
		select
			@vl_Classe = CFG..TYP_FMC.classe
		from EXP..FMC_GEN, CFG..TYP_FMC
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and 
			EXP..FMC_GEN.cle = @va_CleEvt_in and
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type
	
		if @@rowcount = 0 or (@vl_Classe != XZAEC_ClasseGenerique and
		   @vl_Classe != XZAEC_ClasseBouchon)
			return XDC_NOK
	
		if @vl_Classe = XZAEC_ClasseGenerique
		begin
			/* l'evenement en entree est la tete */
			select	@vl_NumEvtTete = @va_NumEvt_in, 
				@vl_CleEvtTete = @va_CleEvt_in
	
			/*B recherche du numero de la fiche conseq de la tete */
			/*B de bouchon */
			select
				@vl_NumEvtQueue = EXP..FMC_GEN.numero,
				@vl_CleEvtQueue = EXP..FMC_GEN.cle
			from EXP..FMC_GEN, CFG..TYP_FMC
			where 	EXP..FMC_GEN.cause = @vl_NumEvtTete and 
				EXP..FMC_GEN.cle_cause = @vl_CleEvtTete and
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClasseBouchon
	
			if @@rowcount = 0
				return XDC_NOK
	
			/*B valorisation des variables de sortie */
			select @va_NumEvt_out = @vl_NumEvtQueue,
				@va_CleEvt_out = @vl_CleEvtQueue
		end
		/* l'evenement en entree est la queue */
		else
		begin
			return XDC_NOK
		end	
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A recherche le type de l'evenement */
	select
		@vl_Classe = CFG..TYP_FMC.classe
	from EXP..FMC_GEN, CFG..TYP_FMC
	where EXP..FMC_GEN.numero = @va_NumEvt_in and EXP..FMC_GEN.cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	if @@rowcount = 0 or (@vl_Classe != XZAEC_ClasseGenerique and
	   @vl_Classe != XZAEC_ClasseBouchon)
		return XDC_NOK

	if @vl_Classe = XZAEC_ClasseGenerique
	begin
		select	@vl_NumEvtTete = @va_NumEvt_in, 
			@vl_CleEvtTete = @va_CleEvt_in

		/*B recherche du numero de la fiche conseq de la tete */
		/*B de bouchon */
		select
			@vl_NumEvtQueue = EXP..FMC_GEN.numero,
			@vl_CleEvtQueue = EXP..FMC_GEN.cle
		from EXP..FMC_GEN, CFG..TYP_FMC
		where 	EXP..FMC_GEN.cause = @vl_NumEvtTete and 
			EXP..FMC_GEN.cle_cause = @vl_CleEvtTete and
			CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
			CFG..TYP_FMC.classe = XZAEC_ClasseBouchon

		if @@rowcount = 0
			return XDC_NOK

		/*B valorisation des variables de sortie */
		select @va_NumEvt_out = @vl_NumEvtQueue,
			@va_CleEvt_out = @vl_CleEvtQueue
	end
	/* l'evenement en entree est la queue */
	else
	begin
		return XDC_NOK
	end

#endif
#endif

	return XDC_OK

go
