/*E*/
/*  Fichier : $Id: xzad02sp.prc,v 1.4 2019/08/28 17:11:58 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2019/08/28 17:11:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Mise a jour et ecriture des donnees temps de parcours 6mn
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(dem/tdp) (V 1.1)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.2) (dem/1536)
* JMG	13/01/12	: tdp etendu dem/1014 1.3
* PNI   28/08/2019      : Optimisation mécanisme de purge (delete correspondant à chaque insert ZDP_6MN) DEM1359 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees Meteo
* 
* Sequence d'appel
* SP	XZAD02_Ajout_Mesure_TDP_6mn
* 
* Arguments en entree
* XDY_Mot		va_Numero_in	: numero 
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_Temps_in
* XDY_Octet		va_Validite_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* - va_Numero_in, va_Horodate_in sont obligatoires
* - si va_fin_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table ZDP_6MN
-------------------------------------------------------*/

use PRC
go

drop procedure XZAD02
go

create procedure XZAD02
	@va_Numero_in		smallint	= null,
	@va_Horodate_in		datetime	= null,
	@va_Temps_in		smallint	= null,
	@va_Validite_in		tinyint	= null,
	@va_TempsRADT_in	smallint        = null,
	@va_ValiditeRADT_in	tinyint = null
	
as
	declare @vl_Site T_SITE,
	        @vl_Retour int,
		@vl_validite 		tinyint,
		@vl_temps		smallint,
		@vl_Minutes int,
		@vl_Horodate smalldatetime


	select @vl_Retour = XDC_OK



	/*A controle parametres en entree */
	if @va_Numero_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE 
		return XDC_ARG_INV

	/*A recherche du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
           	return XDC_NOK


	/*A verifier l'existance de la numero */
	if not exists ( select * from CFG..EQT_ZDP 
			where	numero = @va_Numero_in )
		return XDC_ARG_INV

        /*A Arrondi l'horodate */
	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))

	/*A recherche si la mesure existe en base */
	select 
		@vl_validite		= validite,
		@vl_temps		= temps
	from 	EXP..ZDP_6MN
	where	numero = @va_Numero_in and
		horodate = @va_Horodate_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_Validite_in = XDC_FAUX
		begin
			select @va_Temps_in = 0
		end


		insert EXP..ZDP_6MN (numero, horodate, temps, validite, sit, tempsRADT,validiteRADT)
		values( @va_Numero_in, @va_Horodate_in,@va_Temps_in, @va_Validite_in, @vl_Site,
					@va_TempsRADT_in, @va_ValiditeRADT_in)


	end
	else
	begin
		/* prendre la nouvelle valeur de horo_x si elle est valuee  */
		if (@va_Validite_in = XDC_VRAI) 
		begin
			select	@vl_temps	= @va_Temps_in
			select	@vl_validite	= @va_Validite_in
		end
		

		/* modification de cette mesure */
		update EXP..ZDP_6MN set
		validite		= @vl_validite,
		temps		= @vl_temps,
		tempsRADT	= @va_TempsRADT_in,
		validiteRADT	= @va_ValiditeRADT_in
		where   numero = @va_Numero_in and
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
	delete EXP..ZDP_6MN
	where numero = @va_Numero_in and horodate =@vl_Horodate

	return @vl_Retour
go
