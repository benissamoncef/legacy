/*E*/
/*  Fichier : $Id: xzas62sp.prc,v 1.1 1999/02/26 14:26:37 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:26:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas62sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* PN 	06/01/99	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage
* 
* Sequence d'appel
* SP	XZAS62_Ajout_Mesure_DATEX
* 
* Arguments en entree
* XDY_Mot		va_Nom_in	
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_Q_in
* XDY_Booleen		va_ValiditeQ_in
* XDY_Octet		va_V_in
* XDY_Booleen		va_ValiditeV_in
* XDY_Octet		va_TO_in
* XDY_Booleen		va_ValiditeTO_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres d'entree null, ou valeur non autorisee ou
*                 station inexistante
* 
* Conditions d'utilisation
* - va_Nom_in, va_Sens_in, va_Voie_in, va_Horodate_in sont obligatoires
* - si va_Q_in est non value, utiliser XDC_RADT_Q_INCONNU
* de meme pour va_V_in (XDC_RADT_Q_INCONNU), va_TO_in (XDC_RADT_TO_INCONNU)
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MESURE_DATEX
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS62' and type = 'P')

    drop procedure XZAS62

go

create procedure XZAS62
	@va_Nom_in		char(35)	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		T_VOIE		= null,
	@va_Horodate_in		datetime	= null,
	@va_Q_in		smallint	= null,
	@va_ValiditeQ_in	bit,
	@va_V_in		tinyint		= null,
	@va_ValiditeV_in	bit,
	@va_TO_in		tinyint		= null,
	@va_ValiditeTO_in	bit
as
	declare @vl_Q smallint, @vl_ValiditeQ bit,
		@vl_V tinyint, @vl_ValiditeV bit, @vl_TO tinyint,
		@vl_ValiditeTO bit, 
		@vl_Minutes int, @vl_Retour int, @vl_Horodate smalldatetime

	/*A controle parametres en entree */
	if @va_Nom_in = null or @va_Sens_in = null or
	   @va_Voie_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE or
	   @va_Q_in = null or @va_V_in = null or @va_TO_in = null 
		return XDC_ARG_INV

	/*A si toutes les mesures sont inconnues : aucun traitement */
	if @va_Q_in = XDC_RADT_Q_INCONNU and @va_V_in = XDC_RADT_V_INCONNU and 
	   @va_TO_in = XDC_RADT_TO_INCONNU 
		return XDC_OK



	/*A arrondi horodate */
	
	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))
	
	
	/*A on jette les mesures vieilles de plus d'un mois */
	
	if @va_Horodate_in<dateadd(month,-1,getdate())
		return XDC_ARG_INV

	/*A recherche si la mesure existe en base */
	select 
		@vl_Q		= Q,
		@vl_ValiditeQ	= validite_Q,
		@vl_V		= V,
		@vl_ValiditeV	= validite_V,
		@vl_TO		= T_O,
		@vl_ValiditeTO	= validite_TO
	from EXP..MES_ADA
	where	nom = @va_Nom_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_Q_in = XDC_RADT_Q_INCONNU
			select @va_Q_in = null
		if @va_TO_in = XDC_RADT_TO_INCONNU
			select @va_TO_in = null
		if @va_V_in = XDC_RADT_V_INCONNU
			select @va_V_in = null

		insert EXP..MES_ADA (nom,sens, voie, horodate, Q, validite_Q,
		V, validite_V, T_O, validite_TO)
		values( @va_Nom_in, @va_Sens_in, @va_Voie_in, @va_Horodate_in,
		@va_Q_in, @va_ValiditeQ_in,  @va_V_in, @va_ValiditeV_in, 
		@va_TO_in, @va_ValiditeTO_in)
	end
	else
	begin
		/* prendre la nouvelle valeur de Q, V, T_O si elles sont :                 */
		/* - correcte et valuee                                                        */
		/* - reconstituee et la valeur actuellement en base est null                   */
		/* - correcte et null avec la valeur actuellement en base reconstituee et null */
		/* - valuee et conserve la meme validite par rapport a celle en base           */
		if (@va_Q_in != XDC_RADT_Q_INCONNU and @va_ValiditeQ_in = XDC_RADT_CORRECT) or
		   (@va_Q_in != XDC_RADT_Q_INCONNU and @vl_Q = null) or
		   (@va_Q_in = XDC_RADT_Q_INCONNU and @va_ValiditeQ_in = XDC_RADT_CORRECT and
		    @vl_Q = null and @vl_ValiditeQ = XDC_RADT_RECONST) or
		   (@va_Q_in != XDC_RADT_Q_INCONNU and @va_ValiditeQ_in = @vl_ValiditeQ)
		begin
			if @va_Q_in = XDC_RADT_Q_INCONNU
				select	@vl_ValiditeQ	= @va_ValiditeQ_in,
					@vl_Q 		= null
			else 	select	@vl_ValiditeQ	= @va_ValiditeQ_in,
					@vl_Q		= @va_Q_in
		end
		if (@va_V_in != XDC_RADT_V_INCONNU and @va_ValiditeV_in = XDC_RADT_CORRECT) or
		   (@va_V_in != XDC_RADT_V_INCONNU and @vl_V = null) or
		   (@va_V_in = XDC_RADT_V_INCONNU and @va_ValiditeV_in = XDC_RADT_CORRECT and
		    @vl_V = null and @vl_ValiditeV = XDC_RADT_RECONST) or
		   (@va_V_in != XDC_RADT_V_INCONNU and @va_ValiditeV_in = @vl_ValiditeV)
		begin
			if @va_V_in = XDC_RADT_V_INCONNU
				select	@vl_ValiditeV	= @va_ValiditeV_in,
					@vl_V 		= null
			else 	select	@vl_ValiditeV	= @va_ValiditeV_in,
					@vl_V		= @va_V_in
		end
		if (@va_TO_in != XDC_RADT_TO_INCONNU and @va_ValiditeTO_in = XDC_RADT_CORRECT) or
		   (@va_TO_in != XDC_RADT_TO_INCONNU and @vl_TO = null) or
		   (@va_TO_in = XDC_RADT_TO_INCONNU and @va_ValiditeTO_in = XDC_RADT_CORRECT and
		    @vl_TO = null and @vl_ValiditeTO = XDC_RADT_RECONST) or
		   (@va_TO_in != XDC_RADT_TO_INCONNU and @va_ValiditeTO_in = @vl_ValiditeTO)
		begin
			if @va_TO_in = XDC_RADT_TO_INCONNU
				select	@vl_ValiditeTO	= @va_ValiditeTO_in,
					@vl_TO 		= null
			else 	select	@vl_ValiditeTO	= @va_ValiditeTO_in,
					@vl_TO		= @va_TO_in
		end

		/* modification de cette mesure */
		update EXP..MES_ADA set
			Q		= @vl_Q,
			validite_Q	= @vl_ValiditeQ,
			V		= @vl_V,
			validite_V	= @vl_ValiditeV,
			T_O		= @vl_TO,
			validite_TO	= @vl_ValiditeTO
		where   nom = @va_Nom_in and
			sens = @va_Sens_in and
			voie = @va_Voie_in and
			horodate = @va_Horodate_in
	end

	if @@rowcount != 1
		select @vl_Retour = XDC_NOK
	else	select @vl_Retour = XDC_OK

	/*A purge d'une mesure 32 jours avant la date courante */
	select @vl_Horodate=getdate()
	select @vl_Minutes=datepart(minute,@vl_Horodate)%6
	if @vl_Minutes<>0
		select @vl_Horodate=dateadd(minute,-@vl_Minutes,@vl_Horodate)

	select @vl_Horodate=dateadd(day,-XDC_OFFSET_EN_JOURS_PURGE_BASE,@vl_Horodate)
	delete EXP..MES_ADA
	where	nom = @va_Nom_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @vl_Horodate

	return @vl_Retour
go
