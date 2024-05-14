/*E*/
/*  Fichier : $Id: xzas40sp.prc,v 1.3 1998/09/22 13:32:04 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/09/22 13:32:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas40sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/07/95	: Creation	(V 1.1)
* B.G.	31/10/95	: gestion sens A57 (1.2)
* P.N.	29/06/98	: separation EXP/HIS (1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe bouchon
* 
* Sequence d'appel
* SP	XZAS40_Volume_Bouchon
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures multiple de 6mn
* 
* Arguments en sortie
* XDY_Entier	va_Volume_out		
*
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee 
* 
* Conditions d'utilisation
* Ecretage des PR de debut et de fin aux valeurs donnees en entree
* Attention aucun controle n'est effectue pour verifier que le PR
* de debut et de fin de bouchon correspondent au sens de l'autoroute
* au niveau des fiches evenements : pour le graphe c'est identique
*
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 jour)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
*
* Fonction
* selectionner dans la table FMC_GEN, FMC_HIS, FMC_BOU
-------------------------------------------------------*/

use PRC
go

create procedure XZAS40
	@va_Autoroute_in	tinyint		= null,
	@va_PRDebut_in		T_PR		= null,
	@va_PRFin_in		T_PR		= null,
	@va_HorodateDebut_in	datetime	= null,
	@va_Volume_out		int		= null output
as
	declare @vl_Type tinyint, @vl_Sens T_SENS, @vl_Sens_inv T_SENS,
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_Numero int, @vl_Cle tinyint,
		@vl_PRDebut T_PR, @vl_PRFin T_PR, @vl_HorodateDebut datetime,
		@vl_Status int,
		@vl_HorodateFin datetime, @vl_DateFin datetime

	/*A controle parametres en entree */
	if @va_Autoroute_in = null or @va_PRDebut_in = null or
	   @va_HorodateDebut_in = null or @va_PRFin_in = null or
	   @va_PRDebut_in = @va_PRFin_in
		return XDC_ARG_INV

	/* sens du trafic */
	if @va_PRDebut_in > @va_PRFin_in
		select @vl_Sens_inv = XDC_SENS_NORD
	else	select @vl_Sens_inv = XDC_SENS_SUD

	/* gestion sens inverse A57 */
	if exists (	select * from CFG..RES_AUT_INV
			where	numero=@va_Autoroute_in
			  and	inversee=XDC_PR_INV )
	begin
		if @vl_Sens_inv = XDC_SENS_NORD
			select @vl_Sens = XDC_SENS_SUD
		else
			select @vl_Sens = XDC_SENS_NORD
	end
	else
		select @vl_Sens = @vl_Sens_inv
	
	/*A calcul la date de fin pour la recherche des mesures */
	select @vl_DateFin = dateadd(month, 1, @va_HorodateDebut_in)

	/*! creation d'une table de stockage des PR de tete et */
	/*! queue de bouchon */
	create table #BOU_PR (numero int, cle tinyint, 
	horodate_debut datetime null, horodate_fin datetime null, 
	PR_debut int null, PR_fin int null, cause int null, 
	cle_cause tinyint null, type tinyint null)

#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un jour au moins */
 	/*A rechercher les mesures dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_HorodateDebut_in < (select dateadd(day, -1, getdate ()))
	begin

		exec @vl_Status = SQL_HIS.PRC..XZAS40
			@va_Autoroute_in	,
			@va_PRDebut_in		,
			@va_PRFin_in		,
			@va_HorodateDebut_in	,
			@va_Volume_out		output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		/*B lecture des PR de debut de bouchon */
		/*B pour ce morceau d'autoroute        */
		/*! en prenant en plus les derniers PR */
		/*! inferieurs a la date de debut et   */
		/*! et les premiers superieurs a la date */
		/*! de fin pour chaque bouchon         */
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR
		from EXP..FMC_GEN, EXP..FMC_HIS
		where EXP..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
			EXP..FMC_HIS.autoroute = @va_Autoroute_in and
			((EXP..FMC_HIS.sens % 2)= (@vl_Sens % 2)) and
			EXP..FMC_HIS.horodate_validation <= @vl_DateFin and
			EXP..FMC_HIS.horodate_validation >= @va_HorodateDebut_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 
		
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select distinct
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR
		from EXP..FMC_GEN, EXP..FMC_HIS
		where EXP..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
			EXP..FMC_HIS.autoroute = @va_Autoroute_in and
			((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
			EXP..FMC_HIS.horodate_validation > @vl_DateFin and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle
		group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
			EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
		having EXP..FMC_HIS.horodate_validation = min(EXP..FMC_HIS.horodate_validation)
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select distinct
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR
		from EXP..FMC_GEN, EXP..FMC_HIS
		where EXP..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
			EXP..FMC_HIS.autoroute = @va_Autoroute_in and
			((EXP..FMC_HIS.sens % 2)= (@vl_Sens % 2)) and
			EXP..FMC_HIS.horodate_validation < @va_HorodateDebut_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle
		group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
			EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
		having EXP..FMC_HIS.horodate_validation = max(EXP..FMC_HIS.horodate_validation)

		/*B lecture des PR de fin de bouchon */
		/*B pour ce morceau d'autoroute      */
		/*! en prenant en plus les derniers PR */
		/*! inferieurs a la date de debut et   */
		/*! et les premiers superieurs a la date */
		/*! de fin pour chaque bouchon         */
		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause,
			EXP..FMC_BOU.type
		from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
		where EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
			EXP..FMC_HIS.horodate_validation <= @vl_DateFin and
			EXP..FMC_HIS.horodate_validation >= @va_HorodateDebut_in and
			EXP..FMC_HIS.autoroute = @va_Autoroute_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
			EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation and
			EXP..FMC_GEN.cause is not null and
			EXP..FMC_GEN.cle_cause is not null

		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select distinct
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause,
			EXP..FMC_BOU.type
		from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
		where EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
			EXP..FMC_HIS.horodate_validation < @va_HorodateDebut_in and
			EXP..FMC_HIS.autoroute = @va_Autoroute_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
			EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation and
			EXP..FMC_GEN.cause is not null and
			EXP..FMC_GEN.cle_cause is not null
		group by EXP..FMC_BOU.numero, EXP..FMC_BOU.cle,
			EXP..FMC_BOU.horodate,
			EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
			EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
		having EXP..FMC_HIS.horodate_validation = max(EXP..FMC_HIS.horodate_validation)

		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select distinct
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause,
			EXP..FMC_BOU.type
		from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
		where EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
			EXP..FMC_HIS.horodate_validation > @vl_DateFin and
			EXP..FMC_HIS.autoroute = @va_Autoroute_in and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
			EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
			EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation and
			EXP..FMC_GEN.cause is not null and
			EXP..FMC_GEN.cle_cause is not null
		group by EXP..FMC_BOU.numero, EXP..FMC_BOU.cle, 
			EXP..FMC_BOU.horodate,
			EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
			EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
		having EXP..FMC_HIS.horodate_validation = min(EXP..FMC_HIS.horodate_validation)
	end


#else
#ifdef HIST
		/*B lecture des PR de debut de bouchon */
		/*B pour ce morceau d'autoroute        */
		/*! en prenant en plus les derniers PR */
		/*! inferieurs a la date de debut et   */
		/*! et les premiers superieurs a la date */
		/*! de fin pour chaque bouchon         */
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR
		from HIS..FMC_GEN, HIS..FMC_HIS
		where HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			((HIS..FMC_HIS.sens %2) = (@vl_Sens % 2)) and
			HIS..FMC_HIS.horodate_validation >= @va_HorodateDebut_in and
			HIS..FMC_HIS.horodate_validation <= @vl_DateFin and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle 
		
		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select distinct
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR
		from HIS..FMC_GEN, HIS..FMC_HIS
		where HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			((HIS..FMC_HIS.sens %2) = (@vl_Sens % 2)) and
			HIS..FMC_HIS.horodate_validation > @vl_DateFin and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
		group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, 
		HIS..FMC_GEN.numero, HIS..FMC_GEN.cle
		having HIS..FMC_HIS.horodate_validation = min(HIS..FMC_HIS.horodate_validation) 

		insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
		select distinct
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR
		from HIS..FMC_GEN, HIS..FMC_HIS
		where HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			((HIS..FMC_HIS.sens %2) = (@vl_Sens % 2)) and
			HIS..FMC_HIS.horodate_validation < @va_HorodateDebut_in and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
		group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, 
		HIS..FMC_GEN.numero, HIS..FMC_GEN.cle
		having HIS..FMC_HIS.horodate_validation = max(HIS..FMC_HIS.horodate_validation)

		/*B lecture des PR de fin de bouchon */
		/*B pour ce morceau d'autoroute      */
		/*! en prenant en plus les derniers PR */
		/*! inferieurs a la date de debut et   */
		/*! et les premiers superieurs a la date */
		/*! de fin pour chaque bouchon         */
		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_GEN.cause,
			HIS..FMC_GEN.cle_cause,
			HIS..FMC_BOU.type
		from HIS..FMC_GEN, HIS..FMC_HIS, HIS..FMC_BOU
		where HIS..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			HIS..FMC_HIS.horodate_validation >= @va_HorodateDebut_in and
			HIS..FMC_HIS.horodate_validation <= @vl_DateFin and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			((HIS..FMC_HIS.sens %2) = (@vl_Sens % 2))and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.numero = HIS..FMC_GEN.numero and
			HIS..FMC_BOU.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.horodate = HIS..FMC_HIS.horodate_validation and
			HIS..FMC_GEN.cause is not null and
			HIS..FMC_GEN.cle_cause is not null

		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select distinct
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_GEN.cause,
			HIS..FMC_GEN.cle_cause,
			HIS..FMC_BOU.type
		from HIS..FMC_GEN, HIS..FMC_HIS, HIS..FMC_BOU
		where HIS..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			HIS..FMC_HIS.horodate_validation < @va_HorodateDebut_in and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			((HIS..FMC_HIS.sens %2) = (@vl_Sens % 2)) and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.numero = HIS..FMC_GEN.numero and
			HIS..FMC_BOU.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.horodate = HIS..FMC_HIS.horodate_validation and
			HIS..FMC_GEN.cause is not null and
			HIS..FMC_GEN.cle_cause is not null
		group by HIS..FMC_BOU.numero, HIS..FMC_BOU.cle,
		HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, 
		HIS..FMC_GEN.numero, HIS..FMC_GEN.cle
		having HIS..FMC_HIS.horodate_validation = max(HIS..FMC_HIS.horodate_validation) and 
		HIS..FMC_BOU.horodate = HIS..FMC_HIS.horodate_validation

		insert #BOU_PR (numero, cle, horodate_fin, 
		PR_fin, cause, cle_cause, type)
		select distinct
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_GEN.cause,
			HIS..FMC_GEN.cle_cause,
			HIS..FMC_BOU.type
		from HIS..FMC_GEN, HIS..FMC_HIS, HIS..FMC_BOU
		where HIS..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
			HIS..FMC_HIS.horodate_validation > @vl_DateFin and
			HIS..FMC_HIS.autoroute = @va_Autoroute_in and
			((HIS..FMC_HIS.sens % 2)= (@vl_Sens % 2)) and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.numero = HIS..FMC_GEN.numero and
			HIS..FMC_BOU.cle = HIS..FMC_GEN.cle and
			HIS..FMC_BOU.horodate = HIS..FMC_HIS.horodate_validation and
			HIS..FMC_GEN.cause is not null and
			HIS..FMC_GEN.cle_cause is not null
		group by HIS..FMC_BOU.numero, HIS..FMC_BOU.cle,
			HIS..FMC_HIS.numero, HIS..FMC_HIS.cle, 
			HIS..FMC_GEN.numero, HIS..FMC_GEN.cle
		having HIS..FMC_HIS.horodate_validation = min(HIS..FMC_HIS.horodate_validation) and 
		HIS..FMC_BOU.horodate = HIS..FMC_HIS.horodate_validation
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des PR de debut de bouchon */
	/*A pour ce morceau d'autoroute        */
	/*! en prenant en plus les derniers PR */
	/*! inferieurs a la date de debut et   */
	/*! et les premiers superieurs a la date */
	/*! de fin pour chaque bouchon         */
	insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
	select 
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR
	from EXP..FMC_GEN, EXP..FMC_HIS
	where EXP..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
		EXP..FMC_HIS.autoroute = @va_Autoroute_in and
		((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
		EXP..FMC_HIS.horodate_validation <= @vl_DateFin and
		EXP..FMC_HIS.horodate_validation >= @va_HorodateDebut_in and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 

	insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
	select distinct
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR
	from EXP..FMC_GEN, EXP..FMC_HIS
	where EXP..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
		EXP..FMC_HIS.autoroute = @va_Autoroute_in and
		((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
		EXP..FMC_HIS.horodate_validation > @vl_DateFin and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle
	group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
		EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
	having EXP..FMC_HIS.horodate_validation = min(EXP..FMC_HIS.horodate_validation)
	insert #BOU_PR (numero, cle, horodate_debut, PR_debut)
	select distinct
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR
	from EXP..FMC_GEN, EXP..FMC_HIS
	where EXP..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
		EXP..FMC_HIS.autoroute = @va_Autoroute_in and
		((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
		EXP..FMC_HIS.horodate_validation < @va_HorodateDebut_in and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle
	group by EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
		EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
	having EXP..FMC_HIS.horodate_validation = max(EXP..FMC_HIS.horodate_validation)

	/*A lecture des PR de fin de bouchon */
	/*A pour ce morceau d'autoroute      */
	/*! en prenant en plus les derniers PR */
	/*! inferieurs a la date de debut et   */
	/*! et les premiers superieurs a la date */
	/*! de fin pour chaque bouchon         */
	insert #BOU_PR (numero, cle, horodate_fin, 
	PR_fin, cause, cle_cause, type)
	select 
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR,
		EXP..FMC_GEN.cause,
		EXP..FMC_GEN.cle_cause,
		EXP..FMC_BOU.type
	from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
	where EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
		EXP..FMC_HIS.horodate_validation <= @vl_DateFin and
		EXP..FMC_HIS.horodate_validation >= @va_HorodateDebut_in and
		EXP..FMC_HIS.autoroute = @va_Autoroute_in and
		((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
		EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
		EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
		EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation and
		EXP..FMC_GEN.cause is not null and
		EXP..FMC_GEN.cle_cause is not null

	insert #BOU_PR (numero, cle, horodate_fin, 
	PR_fin, cause, cle_cause, type)
	select distinct
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR,
		EXP..FMC_GEN.cause,
		EXP..FMC_GEN.cle_cause,
		EXP..FMC_BOU.type
	from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
	where EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
		EXP..FMC_HIS.horodate_validation < @va_HorodateDebut_in and
		EXP..FMC_HIS.autoroute = @va_Autoroute_in and
		((EXP..FMC_HIS.sens % 2) = (@vl_Sens % 2)) and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
		EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
		EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
		EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation and
		EXP..FMC_GEN.cause is not null and
		EXP..FMC_GEN.cle_cause is not null
	group by EXP..FMC_BOU.numero, EXP..FMC_BOU.cle, EXP..FMC_BOU.horodate,
		EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
		EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
	having EXP..FMC_HIS.horodate_validation = max(EXP..FMC_HIS.horodate_validation)

	insert #BOU_PR (numero, cle, horodate_fin, 
	PR_fin, cause, cle_cause, type)
	select distinct
		EXP..FMC_HIS.numero,
		EXP..FMC_HIS.cle,
		EXP..FMC_HIS.horodate_validation,
		EXP..FMC_HIS.PR,
		EXP..FMC_GEN.cause,
		EXP..FMC_GEN.cle_cause,
		EXP..FMC_BOU.type
	from EXP..FMC_GEN, EXP..FMC_HIS, EXP..FMC_BOU
	where EXP..FMC_GEN.type = XZAEC_FMC_QueueBouchon and
		EXP..FMC_HIS.horodate_validation > @vl_DateFin and
		EXP..FMC_HIS.autoroute = @va_Autoroute_in and
		((EXP..FMC_HIS.sens % 2) = @vl_Sens) and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
		EXP..FMC_BOU.numero = EXP..FMC_GEN.numero and
		EXP..FMC_BOU.cle = EXP..FMC_GEN.cle and
		EXP..FMC_BOU.horodate = EXP..FMC_HIS.horodate_validation and
		EXP..FMC_GEN.cause is not null and
		EXP..FMC_GEN.cle_cause is not null
	group by EXP..FMC_BOU.numero, EXP..FMC_BOU.cle, EXP..FMC_BOU.horodate,
		EXP..FMC_HIS.numero, EXP..FMC_HIS.cle, 
		EXP..FMC_GEN.numero, EXP..FMC_GEN.cle
	having EXP..FMC_HIS.horodate_validation = min(EXP..FMC_HIS.horodate_validation)
#endif
#endif

	/*! creation d'une table de travail de stockage du graphe de bouchon */
	create table #GRA_BOU (numero int, cle tinyint, 
	horodate datetime , horodate_fin datetime null, PR_debut int , 
	PR_fin int, type tinyint)

	/*! creation d'une table des tetes de bouchon n'etant pas */
	/*! dans la zone du PR mais sur l'autoroute specifiee     */
	create table #BOU_PAS_ZONE (numero int, cle tinyint)

	/*! creation d'une table des bouchon en sens inconnu */
	/*! a virer                                          */
	create table #BOU_INC (numero int, cle tinyint)

	/*! creation d'un curseur pour parcourir la table des */
	/*! PR de queue de bouchon */
	declare Pointeur_Pr_Queue cursor
	for select numero, cle, horodate_fin, PR_fin, cause, cle_cause, type
	from #BOU_PR
	where PR_debut is null and PR_fin is not null and 
		horodate_fin >= @va_HorodateDebut_in and
		horodate_fin <= @vl_DateFin 
	order by horodate_fin

	open Pointeur_Pr_Queue

	/*A construction du graphe en recherchant toutes  */
	/*A les queues de bouchon dont la date de         */
	/*A validation est comprise dans l'intervalle     */
	/*A de temps (date donnee , date donnee + 6h )    */
	fetch Pointeur_Pr_Queue into @vl_Numero, @vl_Cle, @vl_HorodateFin, 
	@vl_PRFin, @vl_Cause, @vl_CleCause, @vl_Type
	while @@sqlstatus = 0
	begin
		/*B rechercher le PR de debut de bouchon */
		/*B correspondant a la queue */
		select 
			@vl_PRDebut = PR_debut,
			@vl_HorodateDebut = horodate_debut
		from #BOU_PR
		where numero = @vl_Cause and cle = @vl_CleCause and
			horodate_debut in (select max(horodate_debut) from #BOU_PR
					where numero = @vl_Cause and cle = @vl_CleCause and
						horodate_debut <= @vl_HorodateFin)
		if @@rowcount = 0
		begin
			insert #BOU_INC (numero, cle)
			values(@vl_Numero, @vl_Cle)
			insert #BOU_INC (numero, cle)
			values(@vl_Cause, @vl_CleCause)
		end
		else
		begin

			/*B ecretage des PR */
			if (@vl_Sens_inv = XDC_SENS_SUD and 
			   @vl_PRFin < @va_PRDebut_in and
			   @vl_PRDebut > @va_PRDebut_in) or
			   (@vl_Sens_inv = XDC_SENS_NORD and
			   @vl_PRFin > @va_PRDebut_in and
			   @vl_PRDebut < @va_PRDebut_in)
				select @vl_PRFin = @va_PRDebut_in
			if (@vl_Sens_inv = XDC_SENS_SUD and
			   @vl_PRFin < @va_PRFin_in and
			   @vl_PRDebut > @va_PRFin_in) or
			   (@vl_Sens_inv = XDC_SENS_NORD and
			   @vl_PRFin > @va_PRFin_in and
			   @vl_PRDebut < @va_PRFin_in)
				select @vl_PRDebut = @va_PRFin_in
	
			/*B inserer la tete et la queue de bouchon  */
			/*B si le bouchon fait partie de cette zone */
			if (@vl_PRFin >= @va_PRDebut_in and
			   @vl_PRDebut <= @va_PRFin_in and
			   @vl_Sens_inv = XDC_SENS_SUD) or
			   (@vl_Sens_inv = XDC_SENS_NORD and
			   @vl_PRFin <= @va_PRDebut_in and
			   @vl_PRDebut >= @va_PRFin_in)
			begin
				insert #GRA_BOU
				values( @vl_Cause, @vl_CleCause, 
				@vl_HorodateFin, null, @vl_PRDebut, @vl_PRFin, @vl_Type)
			end
			else 
				/* memoriser que le bouchon n'est pas sur la zone  */
				/* au moins une fois pendant l'intervalle de temps */
				insert #BOU_PAS_ZONE
				values(@vl_Cause, @vl_CleCause)
		end

		/* lecture de la queue de bouchon suivante */
		fetch Pointeur_Pr_Queue into @vl_Numero, @vl_Cle, @vl_HorodateFin, 
		@vl_PRFin, @vl_Cause, @vl_CleCause, @vl_Type
	end
	close Pointeur_Pr_Queue

	delete #GRA_BOU
	from #GRA_BOU, #BOU_INC
	where #GRA_BOU.numero = #BOU_INC.numero and
		#GRA_BOU.cle = #BOU_INC.cle

	/* creation d'un curseur pour parcourir la table des */
	/* PR de tete de bouchon */
	declare Pointeur_Pr_Tete cursor
	for select numero, cle, horodate_debut, PR_debut
	from #BOU_PR
	where PR_fin is null and PR_debut is not null and 
		horodate_debut <= @vl_DateFin and
		horodate_debut >= @va_HorodateDebut_in 
	order by horodate_debut desc
	open Pointeur_Pr_Tete

	/*A construction du graphe en recherchant toutes    */
	/*A les tetes de bouchon dont le PR et la date      */
	/*A de validation sont compris dans les intervalles */
	/*A specifies en entree                             */
	fetch Pointeur_Pr_Tete into @vl_Numero, @vl_Cle, 
	@vl_HorodateDebut, @vl_PRDebut
	while @@sqlstatus = 0
	begin
		/*B rechercher le PR de fin de bouchon */
		/*B correspondant a la tete */
		select 
			@vl_Type = type,
			@vl_PRFin = PR_fin,
			@vl_HorodateFin = horodate_fin
		from #BOU_PR
		where cause = @vl_Numero and cle_cause = @vl_Cle and
			horodate_fin in (select max(horodate_fin)from #BOU_PR
					where cause = @vl_Numero and cle_cause = @vl_Cle and
					horodate_fin <= @vl_HorodateDebut)

		/*! si une queue est trouvee alors on insere la mesure */
		/*! correspondant a la validation de la tete sinon     */
		/*! pas d'insertion car le bouchon n'existe pas encore */
		if @@rowcount != 0
		begin
			/* ecretage des PR */
			if (@vl_Sens_inv = XDC_SENS_SUD and 
			   @vl_PRFin < @va_PRDebut_in and
			   @vl_PRDebut > @va_PRDebut_in) or
			   (@vl_Sens_inv = XDC_SENS_NORD and
			   @vl_PRFin > @va_PRDebut_in and
			   @vl_PRDebut < @va_PRDebut_in)
				select @vl_PRFin = @va_PRDebut_in
			if (@vl_Sens_inv = XDC_SENS_SUD and
			   @vl_PRFin < @va_PRFin_in and
			   @vl_PRDebut > @va_PRFin_in) or
			   (@vl_Sens_inv = XDC_SENS_NORD and
			   @vl_PRFin > @va_PRFin_in and
			   @vl_PRDebut < @va_PRFin_in)
				select @vl_PRDebut = @va_PRFin_in
	
			/*B inserer la tete et la queue de bouchon */
			/*B si le bouchon fait partie de cette zone */
			if (@vl_PRFin >= @va_PRDebut_in and
			   @vl_PRDebut <= @va_PRFin_in and
			   @vl_Sens_inv = XDC_SENS_SUD) or
			   (@vl_Sens_inv = XDC_SENS_NORD and
			   @vl_PRFin <= @va_PRDebut_in and
			   @vl_PRDebut >= @va_PRFin_in)
				insert #GRA_BOU
				values( @vl_Numero, @vl_Cle, 
				@vl_HorodateDebut, null, @vl_PRDebut, 
				@vl_PRFin, @vl_Type)
			else 
				/* memoriser que le bouchon n'est pas sur */
				/* la zone au moins une fois pendant      */
				/* l'intervalle de temps                  */
				insert #BOU_PAS_ZONE
				values(@vl_Numero, @vl_Cle)
		end

		/* lecture de la tete de bouchon suivante */
		fetch Pointeur_Pr_Tete into @vl_Numero, @vl_Cle, 
		@vl_HorodateDebut, @vl_PRDebut
	end
	close Pointeur_Pr_Tete

	/*! mesure a la date de debut pour chaque bouchon */
	/*! ayant apparu avant cette date                 */
	declare Pointeur_Date cursor
	for select
		g.numero,
		g.cle,
		p1.PR_debut,
		p2.PR_fin,
		p2.type
	from #GRA_BOU g, #BOU_PR p1, #BOU_PR p2
	where p1.numero = g.numero and p1.cle = g.cle and
		p2.cause = p1.numero and p2.cle_cause = p1.cle and
		p1.horodate_debut < @va_HorodateDebut_in and
		p2.horodate_fin < @va_HorodateDebut_in  and
		((@vl_Sens_inv = XDC_SENS_SUD and p2.PR_fin < @va_PRFin_in and
		p1.PR_debut > @va_PRDebut_in) or
		(@vl_Sens_inv = XDC_SENS_NORD and p2.PR_fin > @va_PRFin_in and
		p1.PR_debut < @va_PRDebut_in))

	open Pointeur_Date
	fetch Pointeur_Date into @vl_Numero, @vl_Cle, 
			@vl_PRDebut, @vl_PRFin, @vl_Type
	while @@sqlstatus = 0
	begin
		if not exists (select * from #GRA_BOU 
				where horodate = @va_HorodateDebut_in and
					numero = @vl_Numero and cle= @vl_Cle)
			insert #GRA_BOU
			values(@vl_Numero, @vl_Cle, @va_HorodateDebut_in, null, 
			@vl_PRDebut, @vl_PRFin, @vl_Type)

		fetch Pointeur_Date into @vl_Numero, @vl_Cle, 
			@vl_PRDebut, @vl_PRFin, @vl_Type
	end
	close Pointeur_Date

	/* creation d'une table de stockage du graphe de bouchon */
	create table #GRAPHE ( horodate datetime, horodate_fin datetime, 
	PR_debut int, PR_fin int, type tinyint, volume int null)

	/*! positionner la date de fin de chaque mesure */
	insert #GRAPHE (horodate, horodate_fin, PR_debut, PR_fin, type)
	select
		g2.horodate,
		g1.horodate,
		g2.PR_debut,
		g2.PR_fin,
		g2.type
	from #GRA_BOU g1, #GRA_BOU g2
	where g2.numero = g1.numero and g2.cle = g1.cle and
		g1.horodate > g2.horodate
	group by g1.numero, g1.cle, g2.numero, g2.cle, g2.horodate
	having g1.horodate = min(g1.horodate)

	/*! extension du dernier intervalle de mesure par */
	/*! bouchon jusqu'a date de fin si le bouchon a   */
	/*! d'autres validations apres cette date et pas  */
	/*! de validation a date de fin                   */
	insert #GRAPHE (horodate, horodate_fin, PR_debut, PR_fin, type)
	select
		g.horodate,
		@vl_DateFin,
		g.PR_debut,
		g.PR_fin,
		g.type
	from #GRA_BOU g, #BOU_PR p
	where  p.horodate_fin > @vl_DateFin and
		((p.cause = g.numero and
		p.cle_cause = g.cle) or
		(p.numero = g.numero and
		p.cle = g.cle))
	group by g.numero, g.cle, p.numero, p.cle
	having g.horodate = max(g.horodate) and g.horodate < @vl_DateFin

	/*A recherche des bouchons commencant avant la date de debut */
	/*A donnee et finissant apres la date de fin (+ 6h)      */
	/*A sans validation pendant l'intervalle de temps        */

	/* rechercher tous les bouchons  sans validation pendant */
	/* l'intervalle de temps                                 */
	select * into #BOU_NON_VAL
	from #BOU_PR

	delete #BOU_NON_VAL
	from #GRA_BOU, #BOU_NON_VAL
	where (#BOU_NON_VAL.numero = #GRA_BOU.numero and 
		#BOU_NON_VAL.cle = #GRA_BOU.cle) or
		(#BOU_NON_VAL.cause = #GRA_BOU.numero and
		#BOU_NON_VAL.cle_cause = #GRA_BOU.cle)

	delete #BOU_NON_VAL
	from #BOU_PAS_ZONE, #BOU_NON_VAL
	where (#BOU_NON_VAL.numero = #BOU_PAS_ZONE.numero and 
		#BOU_NON_VAL.cle = #BOU_PAS_ZONE.cle) or
		(#BOU_NON_VAL.cause = #BOU_PAS_ZONE.numero and
		#BOU_NON_VAL.cle_cause = #BOU_PAS_ZONE.cle)
	
	declare Pointeur_Bou_NonVal cursor
	for select numero, cle, cause, cle_cause, PR_fin, type
	from #BOU_NON_VAL
	where horodate_fin < @va_HorodateDebut_in 

	open Pointeur_Bou_NonVal

	/*A ajouter les bouchons qui ont une validation d'une tete et */
	/*A d'une queue de bouchon avant la date de debut et qui ont  */
	/*A ete valides apres la date de fin                          */
	fetch Pointeur_Bou_NonVal into @vl_Numero, @vl_Cle,
	@vl_Cause,  @vl_CleCause, @vl_PRFin, @vl_Type
	while @@sqlstatus = 0
	begin
		/* tester s'il existe une tete ou une queue de bouchon */
		/* apres la date de fin sinon le bouchon s'est fini    */
		/* avant la date de debut specifiee                    */
		if exists (select * from #BOU_NON_VAL
			where (numero = @vl_Numero and cle = @vl_Cle and
				horodate_fin > @vl_DateFin) or
				(numero = @vl_Cause and cle = @vl_CleCause and
				horodate_debut > @vl_DateFin) )
		begin
			/* rechercher la tete du bouchon avant la date de */
			/* debut                                          */
		 	select @vl_PRDebut = PR_debut from #BOU_NON_VAL
			where numero = @vl_Cause and cle = @vl_CleCause and
				horodate_debut < @va_HorodateDebut_in

			if @@rowcount = 1
			begin
				/*B ecretage des PR */
				if (@vl_Sens_inv = XDC_SENS_SUD and 
				   @vl_PRFin < @va_PRDebut_in and
				   @vl_PRDebut > @va_PRDebut_in) or
				   (@vl_Sens_inv = XDC_SENS_NORD and
				   @vl_PRFin > @va_PRDebut_in and
				   @vl_PRDebut < @va_PRDebut_in)
					select @vl_PRFin = @va_PRDebut_in
				if (@vl_Sens_inv = XDC_SENS_SUD and
				   @vl_PRFin < @va_PRFin_in and
				   @vl_PRDebut > @va_PRFin_in) or
				   (@vl_Sens_inv = XDC_SENS_NORD and
				   @vl_PRFin > @va_PRFin_in and
				   @vl_PRDebut < @va_PRFin_in)
					select @vl_PRDebut = @va_PRFin_in
		
				/*B Insertion du bouchon pour la duree totale de temps */
				if (@vl_PRFin >= @va_PRDebut_in and
				   @vl_PRDebut <= @va_PRFin_in and
				   @vl_Sens_inv = XDC_SENS_SUD) or
				   (@vl_Sens_inv = XDC_SENS_NORD and
				   @vl_PRFin <= @va_PRDebut_in and
				   @vl_PRDebut >= @va_PRFin_in)
					insert #GRAPHE (horodate, horodate_fin, PR_debut, PR_fin, type)
					values ( @va_HorodateDebut_in, @vl_DateFin,
						@vl_PRDebut, @vl_PRFin, @vl_Type)
			end
		end

		fetch Pointeur_Bou_NonVal into @vl_Numero, @vl_Cle,
		@vl_Cause,  @vl_CleCause, @vl_PRFin, @vl_Type
	end
	close Pointeur_Bou_NonVal

	/* ecretage des PR */
	if @vl_Sens_inv = XDC_SENS_SUD
	begin
		update #GRAPHE set PR_fin = @va_PRDebut_in
		where PR_fin < @va_PRDebut_in
		update #GRAPHE set PR_debut = @va_PRFin_in
		where PR_debut > @va_PRFin_in
	end
	else
	begin
		update #GRAPHE set PR_fin = @va_PRDebut_in
		where PR_fin > @va_PRDebut_in
		update #GRAPHE set PR_debut = @va_PRFin_in
		where PR_debut < @va_PRFin_in
	end

	/*A Calcul du volume d'encombrement par mois */
	update #GRAPHE set volume = datediff(minute, horodate, horodate_fin) * abs(PR_debut - PR_fin)

	select @va_Volume_out = sum(volume)
	from #GRAPHE

	return XDC_OK
go
