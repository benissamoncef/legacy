/*E*/
/*  Fichier : $Id: xzaf01sp.prc,v 1.5 2000/07/21 17:08:03 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2000/07/21 17:08:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/01/95	: Creation (V 1.1)
*                         test pas complet
* C.T.	31/01/95	: Modif filtre evenement (V 1.2)
* C.T.	30/05/95	: Modif filtre evenement (V 1.3)
* C.T.	31/05/95	: Modif date retournee par la 
*                         date de 1ere validation  (V 1.4)
* C.T.	09/10/95	: Prendre les evenements qui sont
*                         en cours ou clotures depuis moins 
*                         de 15 minutes (V 1.5)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des fiches main 
* courante à traiter à l'heure precisée
-------------------------------------------------------
* Sequence d'appel
* SP	XZAF01_Liste_Evt_Traiter
* 
* Arguments en entree
* XDY_Machine	va_PosteOperateur_in
* XDY_Horodate	va_Horodate_in
* 
* Arguments en sortie
* XDY_Octet	va_CleEvt_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_Type_out
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Horodate	va_Horodate_out
* XDY_Booleen	va_Enrichir_out
* XDY_Booleen	va_Reveil_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* va_PosteOperateur_in n'est pas utilise
* 
* Fonction
*
----------------------------------------------------*/

use PRC
go

create procedure XZAF;01
	@va_PosteOperateur_in	smallint= null,
	@va_Horodate_in		datetime= null
as
	declare @vl_Site tinyint,
		@vl_Num int,
		@vl_Cle tinyint,
		@vl_Type tinyint,
		@vl_Horodate datetime,
		@vl_HorodateFin datetime,
		@vl_Enrichir tinyint,
		@vl_Horo datetime,
		@vl_PR int,
		@vl_Autoroute tinyint,
		@vl_Sens tinyint,
		@vl_Bidon int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	
	/*A controle des arguments obligatoires */
	
	if @va_Horodate_in = null
		return XDC_NOK

	/*A creation table temporaire */
	
	create table #LISTE (
		cle		tinyint not null,
		numero		int not null,
		type		tinyint not null,
		autoroute 	tinyint null,
		PR 		int null,
		sens 		tinyint null,
		horodate 	datetime null,
		enrichir 	tinyint,
		reveil 		bit
		)
		
	/*A prendre les evenements qui sont clotures depuis moins de 15 minutes */
	/*A par rapport a la date donnee                                        */
	select @vl_HorodateFin = dateadd(minute, -15, @va_Horodate_in)

	/*A creation curseur */
	
	declare TRAITER cursor
	for
	select 
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		HIS..FMC_GEN.type,
		isnull(HIS..FMC_GEN.debut, HIS..FMC_GEN.debut_prevu)
		
	from HIS..FMC_GEN
	where	HIS..FMC_GEN.type != XZAEC_FMC_PosteOperateur and
		HIS..FMC_GEN.cloture > @vl_HorodateFin and
		HIS..FMC_GEN.fin > @vl_HorodateFin and
		(HIS..FMC_GEN.debut <= @va_Horodate_in or
		 HIS..FMC_GEN.debut_prevu <= @va_Horodate_in)


	open TRAITER
	
	fetch TRAITER into @vl_Num,@vl_Cle,@vl_Type,@vl_Horo
	
	while (@@sqlstatus=0)
	begin
		
		/*B 	pas le droit d'enrichir */
		select @vl_Enrichir=XDC_NON
		
		/*B 	si tete de bouchon, on met la queue */
		
		if @vl_Type=XZAEC_FMC_TeteBouchon
		begin
			select 	@vl_Num=numero,
				@vl_Cle=cle,
				@vl_Type=XZAEC_FMC_QueueBouchon
			from HIS..FMC_GEN
			where	cause = @vl_Num and
				cle_cause = @vl_Cle 
		end

		/*B 	recherche de la premiere validation */
			
		select @vl_Horodate=min(HIS..FMC_HIS.horodate_validation)
		from HIS..FMC_HIS
		where	numero=@vl_Num and
			cle=@vl_Cle
			
		/*	l'evenement est conserve s'il a ete valide avant la date precisee */
		if @vl_Horodate <= @va_Horodate_in
		begin
			/*B 	recherche des infos de FMC_HIS */
			
			select	@vl_Autoroute=autoroute,
				@vl_PR=PR,
				@vl_Sens=sens
			from	HIS..FMC_HIS
			where	numero=@vl_Num and
				cle=@vl_Cle and
				horodate_validation=@vl_Horodate
				
			/*B 	ajoute dans la liste si on a trouve une fiche */
			
			insert into #LISTE
			values (
				@vl_Cle,
				@vl_Num,
				@vl_Type,
				@vl_Autoroute,
				@vl_PR,
				@vl_Sens,
				@vl_Horodate,
				@vl_Enrichir,
				XDC_NON )
		end

		fetch TRAITER into @vl_Num,@vl_Cle,@vl_Type,@vl_Horo
	end

	/*A retourne les valeurs trouvÅes */

	select distinct cle,
			numero,
			type,
			autoroute,
			PR,
			sens,
			horodate,
			enrichir,
			reveil
	from #LISTE
	order by horodate desc

	return XDC_OK
go
