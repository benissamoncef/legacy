/*E*/
/*  Fichier : $Id: xzad03sp.prc,v 1.2 2018/05/03 13:38:25 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/05/03 13:38:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Mise a jour et ecriture des donnees Meteo
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(dem/tdp) (V 1.1)
* JPL	03/05/18	: Suppression de la procedure avant compilation  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees Meteo
* 
* Sequence d'appel
* SP	XZAD03_Ajout_TDP_1h
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
* Inserer ou Modifier dans la table TDP_6MN
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAD03' and type = 'P')
	drop procedure XZAD03
go


create procedure XZAD03
	@va_Numero_in		smallint	= null,
	@va_Horodate_in		datetime	= null,
	@va_Temps_in		smallint	= null,
	@va_Validite_in		tinyint		= null,
	@va_Appariement_in	smallint	= null
	
as
	declare  @vl_Retour int,
		@vl_validite 		tinyint,
		@vl_temps		smallint,
		@vl_app			smallint


	select @vl_Retour = XDC_OK



	/*A controle parametres en entree */
	if @va_Numero_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE 
		return XDC_ARG_INV



	/*A verifier l'existance de la numero */
	if not exists ( select * from CFG..EQT_ZDP 
			where	numero = @va_Numero_in )
		return XDC_ARG_INV

        /*A Arrondi l'horodate */
	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))


	/*A recherche si la mesure existe en base */
	select 
		@vl_validite		= validite,
		@vl_app			= appariement,
		@vl_temps		= temps
	from 	HIS..ZDP_HEU
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


		insert HIS..ZDP_HEU (numero, horodate, temps, appariement, validite)
		values( @va_Numero_in, @va_Horodate_in,@va_Temps_in, @va_Appariement_in, @va_Validite_in)


	end
	else
	begin
		/* prendre la nouvelle valeur de horo_x si elle est valuee  */
		if (@va_Validite_in = XDC_VRAI) 
		begin
			select	@vl_temps	= @va_Temps_in
			select	@vl_validite	= @va_Validite_in
			select	@vl_app		= @va_Appariement_in
		end
		

		/* modification de cette mesure */
		update HIS..ZDP_HEU set
		validite		= @vl_validite,
		temps		= @vl_temps,
		appariement		= @vl_app
		where   numero = @va_Numero_in and
			horodate = @va_Horodate_in
	end

	if @@rowcount != 1
		select @vl_Retour = XDC_NOK
	else	select @vl_Retour = XDC_OK


	return @vl_Retour
go
