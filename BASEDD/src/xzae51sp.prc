/*E*/
/*  Fichier : $Id: xzae51sp.prc,v 1.12 2019/04/23 11:07:30 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2019/04/23 11:07:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae51.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree et
*                         des test sur constantes
* C.T.	13/10/94	: Correction erreur de codage	V 1.4
* C.T.	10/11/94	: Modif bugs applix (V 1.5) 
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.6)
* C.T.	01/12/94	: correction recherche des infos au CI (V 1.7)
* B.G.	21/12/94	: Modif retourne pr tete (1.8)
* B.G.	17/01/95	: Modif date nulle (1.9)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.11)
* JMG	08/04/19 	: correction recup infos tete si FMC multi autoroutes 1.12
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante bouchon a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE51_Lire_Fiche_Bouchon
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR		va_PRQueue_out
* XDY_Octet		va_Type_out
* XDY_Entier		va_Longueur_out
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* Penser a lire les 2 fiches generiques tete et queue 
* de bouchon avant de lire le complement.
* Si l'evenement en entree correspond a une tete, le numero 
* d'evenement en sortie est ceui de la queue et inversement
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
* 
* Fonction
* Select dans la table QUEUE_DE_BOUCHON
* where numero and horodate
* Calcul de la distance entre PR tete et PR queue.
-----------------------------------------------------*/

use PRC
go

create procedure XZAE;51
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PRQueue_out		T_PR = null	output,
	@va_Type_out		tinyint = null	output,
	@va_Longueur_out	int = null	output,
	@va_NumEvt_out		int = null	output,
	@va_CleEvt_out		tinyint = null	output
as
	declare @vl_Horodate		datetime,
		@vl_AutorouteDebut	T_AUTOROUTE,
		@vl_AutorouteFin	T_AUTOROUTE,
		@vl_PRDebut		T_PR,
		@vl_PRFin		T_PR,
		@vl_Classe		tinyint,
		@vl_NumEvtQueue		int,
		@vl_NumEvtTete		int,
		@vl_CleEvtQueue		tinyint,
		@vl_CleEvtTete		tinyint,
		@vl_Status		int,
               @vl_num1 int, @vl_cle1 tinyint


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
begin
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
		else
		begin
			/* l'evenement en entree est la queue */
			select	@vl_NumEvtQueue = @va_NumEvt_in, 
				@vl_CleEvtQueue = @va_CleEvt_in
	
			/*B recherche du numero de la fiche cause de la queue */
			/*B de bouchon                                        */
			select
				@vl_NumEvtTete = cause,
				@vl_CleEvtTete = cle_cause
			from HIS..FMC_GEN
			where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue 
	
			if @@rowcount = 0
				return XDC_NOK
	
			/* test si l'evenement cause est de classe generique */
			if not exists(	select * from HIS..FMC_GEN, CFG..TYP_FMC
					where	HIS..FMC_GEN.numero = @vl_NumEvtTete and
						HIS..FMC_GEN.cle = @vl_CleEvtTete and
						CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
						CFG..TYP_FMC.classe = XZAEC_ClasseGenerique)
				return XDC_NOK
	
			/*B valorisation des variables de sortie */
			select @va_NumEvt_out = @vl_NumEvtTete,
				@va_CleEvt_out = @vl_CleEvtTete
		end
	
		/*A recherche la derniere date correspondante a l'heure donnee  */
		/*A pour la queue de bouchon 					*/
		select @vl_Horodate = max(horodate) from HIS..FMC_BOU
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and
			horodate <= @va_Horodate_in
	
		if @@rowcount = 0
			return XDC_NOK
	
		/*A lecture des infos sur la queue de bouchon */
		select
			@va_Type_out		= type
		from HIS..FMC_BOU
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and 
			horodate = @vl_Horodate
	
		/*A test si les infos ont ete trouvees */
		if @@rowcount != 1
			return XDC_NOK
	
		/*A recherche l'autoroute et le PR de tete de bouchon */
		select
			@vl_AutorouteDebut	= autoroute,
			@vl_PRDebut		= PR
		from HIS..FMC_HIS
		where	numero = @vl_NumEvtTete and 
			cle = @vl_CleEvtTete and 
			horodate_validation in 
				(select max(horodate_validation) 
			 	from HIS..FMC_HIS
			 	where	numero = @vl_NumEvtTete and 
					cle = @vl_CleEvtTete and
					horodate_validation <= 
					@va_Horodate_in )
	
                if @@rowcount = 0
                begin
                        /*recherche cause de la T si cause = bouchon*/
                        select @vl_num1=cause, @vl_cle1=cle_cause
                        from HIS..FMC_GEN
                        where numero = @vl_NumEvtTete and cle = @vl_CleEvtTete

                        if @@rowcount = 0
                        return XDC_NOK

                        select @vl_NumEvtTete = cause, @vl_CleEvtTete = cle_cause
                        from HIS..FMC_GEN
                        where numero = @vl_num1 and cle = @vl_cle1

                        if @@rowcount = 0
                        return XDC_NOK

                        select
                        @vl_AutorouteDebut      = autoroute,
                        @vl_PRDebut             = PR
                        from HIS..FMC_HIS
                        where   numero = @vl_NumEvtTete and
                        cle = @vl_CleEvtTete and
                        horodate_validation in
                                (select max(horodate_validation)
                                from HIS..FMC_HIS
                                where   numero = @vl_NumEvtTete and
                                        cle = @vl_CleEvtTete and
                                        horodate_validation <=
                                        dateadd (minute, 1, @va_Horodate_in) )

                        if @@rowcount = 0
                        return XDC_NOK
                end    

		/*A recherche l'autoroute et le PR de queue de bouchon */
		select
			@vl_AutorouteFin	= autoroute,
			@vl_PRFin		= PR
		from HIS..FMC_HIS
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and 
			horodate_validation = @vl_Horodate
	end
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE;51
			@va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in, @va_PRQueue_out output, @va_Type_out output,
			@va_Longueur_out output, @va_NumEvt_out output, @va_CleEvt_out output
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
		else
		begin
			/* l'evenement en entree est la queue */
			select	@vl_NumEvtQueue = @va_NumEvt_in, 
				@vl_CleEvtQueue = @va_CleEvt_in
	
			/*B recherche du numero de la fiche cause de la queue */
			/*B de bouchon                                        */
			select
				@vl_NumEvtTete = cause,
				@vl_CleEvtTete = cle_cause
			from EXP..FMC_GEN
			where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue 
	
			if @@rowcount = 0
				return XDC_NOK
	
			/* test si l'evenement cause est de classe generique */
			if not exists(	select * from EXP..FMC_GEN, CFG..TYP_FMC
					where	EXP..FMC_GEN.numero = @vl_NumEvtTete and
						EXP..FMC_GEN.cle = @vl_CleEvtTete and
						CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
						CFG..TYP_FMC.classe = XZAEC_ClasseGenerique)
				return XDC_NOK
	
			/*B valorisation des variables de sortie */
			select @va_NumEvt_out = @vl_NumEvtTete,
				@va_CleEvt_out = @vl_CleEvtTete
		end
	
		/*A recherche la derniere date correspondante a l'heure donnee  */
		/*A pour la queue de bouchon 					*/
		select @vl_Horodate = max(horodate) from EXP..FMC_BOU
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and
			horodate <= @va_Horodate_in
	
		if @@rowcount = 0
			return XDC_NOK
	
		/*A lecture des infos sur la queue de bouchon */
		select
			@va_Type_out		= type
		from EXP..FMC_BOU
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and 
			horodate = @vl_Horodate
	
		/*A test si les infos ont ete trouvees */
		if @@rowcount != 1
			return XDC_NOK
	
		/*A recherche l'autoroute et le PR de tete de bouchon */
		select
			@vl_AutorouteDebut	= autoroute,
			@vl_PRDebut		= PR
		from EXP..FMC_HIS
		where	numero = @vl_NumEvtTete and 
			cle = @vl_CleEvtTete and 
			horodate_validation in 
				(select max(horodate_validation) 
			 	from EXP..FMC_HIS
			 	where	numero = @vl_NumEvtTete and 
					cle = @vl_CleEvtTete and
					horodate_validation <= 
					@va_Horodate_in )
	
                if @@rowcount = 0
                begin
                        /*recherche cause de la T si cause = bouchon*/
                        select @vl_num1=cause, @vl_cle1=cle_cause
                        from EXP..FMC_GEN
                        where numero = @vl_NumEvtTete and cle = @vl_CleEvtTete

                        if @@rowcount = 0
                        return XDC_NOK

                        select @vl_NumEvtTete = cause, @vl_CleEvtTete = cle_cause
                        from EXP..FMC_GEN
                        where numero = @vl_num1 and cle = @vl_cle1

                        if @@rowcount = 0
                        return XDC_NOK

                        select
                        @vl_AutorouteDebut      = autoroute,
                        @vl_PRDebut             = PR
                        from EXP..FMC_HIS
                        where   numero = @vl_NumEvtTete and
                        cle = @vl_CleEvtTete and
                        horodate_validation in
                                (select max(horodate_validation)
                                from EXP..FMC_HIS
                                where   numero = @vl_NumEvtTete and
                                        cle = @vl_CleEvtTete and
                                        horodate_validation <=
                                        dateadd (minute, 1, @va_Horodate_in) )

                        if @@rowcount = 0
                        return XDC_NOK
                end    

		/*A recherche l'autoroute et le PR de queue de bouchon */
		select
			@vl_AutorouteFin	= autoroute,
			@vl_PRFin		= PR
		from EXP..FMC_HIS
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and 
			horodate_validation = @vl_Horodate
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
	else
	begin
		/* l'evenement en entree est la queue */
		select	@vl_NumEvtQueue = @va_NumEvt_in, 
			@vl_CleEvtQueue = @va_CleEvt_in

		/*B recherche du numero de la fiche cause de la queue */
		/*B de bouchon                                        */
		select
			@vl_NumEvtTete = cause,
			@vl_CleEvtTete = cle_cause
		from EXP..FMC_GEN
		where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue 

		if @@rowcount = 0
			return XDC_NOK

		/* test si l'evenement cause est de classe generique */
		if not exists(	select * from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.numero = @vl_NumEvtTete and
					EXP..FMC_GEN.cle = @vl_CleEvtTete and
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
					CFG..TYP_FMC.classe = XZAEC_ClasseGenerique)
			return XDC_NOK

		/*B valorisation des variables de sortie */
		select @va_NumEvt_out = @vl_NumEvtTete,
			@va_CleEvt_out = @vl_CleEvtTete
	end

	/*A recherche la derniere date correspondante a l'heure donnee  */
	/*A pour la queue de bouchon 					*/
	select @vl_Horodate = max(horodate) from EXP..FMC_BOU
	where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and
		horodate <= @va_Horodate_in

	if @@rowcount = 0
		return XDC_NOK

	/*A lecture des infos sur la queue de bouchon */
	select
		@va_Type_out		= type
	from EXP..FMC_BOU
	where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and 
		horodate = @vl_Horodate

	/*A test si les infos ont ete trouvees */
	if @@rowcount != 1
		return XDC_NOK

	/*A recherche l'autoroute et le PR de tete de bouchon */
	select
		@vl_AutorouteDebut	= autoroute,
		@vl_PRDebut		= PR
	from EXP..FMC_HIS
	where	numero = @vl_NumEvtTete and 
		cle = @vl_CleEvtTete and 
		horodate_validation in 
			(select max(horodate_validation) 
		 	from EXP..FMC_HIS
		 	where	numero = @vl_NumEvtTete and 
				cle = @vl_CleEvtTete and
				horodate_validation <= 
				@va_Horodate_in )

	if @@rowcount = 0
		return XDC_NOK

	/*A recherche l'autoroute et le PR de queue de bouchon */
	select
		@vl_AutorouteFin	= autoroute,
		@vl_PRFin		= PR
	from EXP..FMC_HIS
	where	numero = @vl_NumEvtQueue and cle = @vl_CleEvtQueue and 
		horodate_validation = @vl_Horodate
#endif
#endif

	if @@rowcount = 0
		return XDC_NOK

	/*A calcul de la longueur du bouchon */
	/*! la perturbation est sur une meme autoroute */
	exec @vl_Status = XZAO;22 @vl_AutorouteDebut, @vl_PRDebut, 
				  @vl_AutorouteFin, @vl_PRFin, 
				  @va_Longueur_out output

	/*A test si la procedure s'est correctement deroulee */
	if @vl_Status != XDC_OK
		select  @va_Longueur_out = 0

	/*A mettre a jour les parametres en sortie */
	/*! pr tete et non queue */
	select @va_PRQueue_out = @vl_PRDebut

	return XDC_OK

go
