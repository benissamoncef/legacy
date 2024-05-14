/*E*/
/*  Fichier : $Id: xzas10sp.prc,v 1.13 2012/01/11 14:22:23 gesconf Exp $      Release : $Revision: 1.13 $        Date : $Date: 2012/01/11 14:22:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/10/94	: Creation	(V 1.1)
* B.G.	12/01/95	: arrondi horodate a 1'	(V 1.2)
* B.G.	19/01/95	: filtre les horodates	(V 1.3)
* C.T.	27/01/95	: si toutes les mesures sont XDC_RADT_Q_INCONNU
*                         pas de traitement (V 1.4)
* C.T.	27/01/95	: Changement du nom de la proc	(V 1.5)
* C.T.	28/01/95	: si toutes les mesures sont XDC_RADT_?_INCONNU
*                         pas de traitement (V 1.6)
* B.G.	19/01/95	: modif or -> and pour toutes les mesures sont XDC_RADT_?_INCONNU	(V 1.7)
* C.T.	09/08/95	: ajout de la suppreesion de la mesure a 
*                         meme date moins 32 jours (V 1.8)
* C.T.	21/08/95	: ajout d'un controle sur la coherence
*                         de la date (V 1.8)
* C.T.	16/09/96	: ajout cas sur DS et DC (V 1.9)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.11
* C.T.	24/03/98	: correction purge (V 1.10)
* PNI	02/09/11	: pas d'insertion en base si station en erreur DEM997  1.13
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage
* 
* Sequence d'appel
* SP	XZAS10_Ajout_Mesure_6_mn
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_Q_in
* XDY_Booleen		va_ValiditeQ_in
* XDY_Octet		va_V_in
* XDY_Booleen		va_ValiditeV_in
* XDY_Octet		va_TO_in
* XDY_Booleen		va_ValiditeTO_in
* XDY_Octet		va_PL_in
* XDY_Booleen		va_ValiditePL_in
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
* - va_Station_in, va_Sens_in, va_Voie_in, va_Horodate_in sont obligatoires
* - si va_Q_in est non value, utiliser XDC_RADT_Q_INCONNU
* de meme pour va_V_in (XDC_RADT_Q_INCONNU), va_TO_in (XDC_RADT_TO_INCONNU), 
* va_PL_in (XDC_RADT_PL_INCONNU)
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MESURE_TRAFIC_6_MINUTES
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS10' and type = 'P')
	drop procedure XZAS10
go


create procedure XZAS10
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		T_VOIE		= null,
	@va_Horodate_in		datetime	= null,
	@va_Q_in		smallint	= null,
	@va_ValiditeQ_in	bit,
	@va_V_in		tinyint		= null,
	@va_ValiditeV_in	bit,
	@va_TO_in		tinyint		= null,
	@va_ValiditeTO_in	bit,
	@va_PL_in		tinyint		= null,
	@va_ValiditePL_in	bit
as
	declare @vl_Site T_SITE, @vl_Q smallint, @vl_ValiditeQ bit,
		@vl_V tinyint, @vl_ValiditeV bit, @vl_TO tinyint,
		@vl_ValiditeTO bit, @vl_PL tinyint, @vl_ValiditePL bit,
		@vl_Minutes int, @vl_Retour int, @vl_Horodate smalldatetime

	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_Voie_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE or
	   @va_Q_in = null or @va_V_in = null or @va_TO_in = null or 
	   @va_PL_in = null
		return XDC_ARG_INV

	/*A si toutes les mesures sont inconnues : aucun traitement */
	if @va_Q_in = XDC_RADT_Q_INCONNU and @va_V_in = XDC_RADT_V_INCONNU and 
	   @va_TO_in = XDC_RADT_TO_INCONNU and @va_PL_in = XDC_RADT_PL_INCONNU
		return XDC_OK

	/* recherche le site local */
	select @vl_Site = numero
	from CFG..RES_DIS
	where serveur = @@servername

	if @vl_Site is null
		return XDC_NOK

	/*A verifier l'existance de la station */
	if not exists ( select * from CFG..RES_CHA 
			where	station = @va_Station_in and 
				sens = @va_Sens_in)
		return XDC_ARG_INV

	/*A Verifier si la station est opérationnelle */
	if exists ( select * from EXP..EQT_DSP
	            where type=XDC_EQT_RAD and
	            equipement=@va_Station_in and
	            dernier=1 and
	            (majeure=1 or HS=1 or desactive=1 or inhibe=1) )
		return XDC_OK

	/*A arrondi horodate */
	
	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))
	
	/*A arrondi horodate a 6 minutes */
	
	select @vl_Minutes=datepart(minute,@va_Horodate_in)%6
	
	if @vl_Minutes<>0
		select @va_Horodate_in=dateadd(minute,-@vl_Minutes,@va_Horodate_in)
	
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
		@vl_ValiditeTO	= validite_TO,
		@vl_PL		= PL,
		@vl_ValiditePL	= validite_PL
	from EXP..MES_TRA
	where	station = @va_Station_in and
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
		if @va_PL_in = XDC_RADT_PL_INCONNU
			select @va_PL_in = null
		if @va_V_in = XDC_RADT_V_INCONNU
			select @va_V_in = null

		insert EXP..MES_TRA (station, sens, voie, horodate,
		                     Q, validite_Q, V, validite_V,
		                     T_O, validite_TO, PL, validite_PL, sit)
		values ( @va_Station_in, @va_Sens_in, @va_Voie_in, @va_Horodate_in,
		         @va_Q_in, @va_ValiditeQ_in,  @va_V_in, @va_ValiditeV_in, 
		         @va_TO_in, @va_ValiditeTO_in, @va_PL_in, @va_ValiditePL_in, @vl_Site)
	end
	else
	begin
		/* prendre la nouvelle valeur de Q, V, PL, T_O si elles sont :                 */
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
		if (@va_PL_in != XDC_RADT_PL_INCONNU and @va_ValiditePL_in = XDC_RADT_CORRECT) or
		   (@va_PL_in != XDC_RADT_PL_INCONNU and @vl_PL = null) or
		   (@va_PL_in = XDC_RADT_PL_INCONNU and @va_ValiditePL_in = XDC_RADT_CORRECT and
		    @vl_PL = null and @vl_ValiditePL = XDC_RADT_RECONST) or
		   (@va_PL_in != XDC_RADT_PL_INCONNU and @va_ValiditePL_in = @vl_ValiditePL)
		begin
			if @va_PL_in = XDC_RADT_PL_INCONNU
				select	@vl_ValiditePL	= @va_ValiditePL_in,
					@vl_PL 		= null
			else 	select	@vl_ValiditePL	= @va_ValiditePL_in,
					@vl_PL		= @va_PL_in
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
		update EXP..MES_TRA set
			Q		= @vl_Q,
			validite_Q	= @vl_ValiditeQ,
			V		= @vl_V,
			validite_V	= @vl_ValiditeV,
			T_O		= @vl_TO,
			validite_TO	= @vl_ValiditeTO,
			PL		= @vl_PL,
			validite_PL	= @vl_ValiditePL
		where   station = @va_Station_in and
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
	delete EXP..MES_TRA
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @vl_Horodate

	return @vl_Retour
go
