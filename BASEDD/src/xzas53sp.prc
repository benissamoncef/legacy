/*E*/
/*  Fichier : $Id: xzas53sp.prc,v 1.2 1998/09/22 13:32:08 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/09/22 13:32:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas53sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Mise a jour et ecriture des donnees Meteo
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(dem/1234) (V 1.1)
* P.N.	07/07/98	: separation EXP/HIS dem/1696 (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees Meteo
* 
* Sequence d'appel
* SP	XZAS53_Ajout_Mesure_MET
* 
* Arguments en entree
* XDY_Mot		va_Numero_in	: numero 
* XDY_Datetime		va_Jour_in
* XDY_Booleen		va_Dispo_in
* XDY_Mot		va_code_in
* XDY_Datetime		va_debut_in
* XDY_Datetime		va_fin_in
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
* - va_Numero_in, va_Jour_in sont obligatoires
* - si va_fin_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MES_MET
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS53' and type = 'P')

    drop procedure XZAS53

go

create procedure XZAS53
	@va_Numero_in		smallint	= null,
	@va_Jour_in		datetime	= null,
	@va_Dispo_in		bit	= null,
	@va_Code_in		tinyint	= null,
	@va_Debut_in	datetime	= null,
	@va_Fin_in		datetime	= null
	
as
	declare  @vl_Retour int,
		@vl_dispo 		bit,
		@vl_code 		smallint,
		@vl_debut 		datetime,
		@vl_fin 		datetime


	select @vl_Retour = XDC_OK


#ifdef HIST

	/*A controle parametres en entree */
	if @va_Numero_in = null or @va_Jour_in = null or
	   @va_Jour_in = XDC_DATE_NULLE or
	   ( @va_Dispo_in != XDC_VRAI and @va_Dispo_in != XDC_FAUX )
		return XDC_ARG_INV



	/*A verifier l'existance de la numero */
/*	if not exists ( select * from CFG..EQT_LCR 
			where	numero = @va_Numero_in)
		return XDC_ARG_INV
*/

	/*A recherche si la mesure existe en base */
	select 
		@vl_dispo		= dispo,
		@vl_code		= code,
		@vl_debut		= debut,
		@vl_fin		= fin
	from HIS..MES_MET
	where	numero = @va_Numero_in and
		jour = @va_Jour_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_Dispo_in = XDC_FAUX
		begin
			select @va_Debut_in = null
			select @va_Fin_in = null
			select @va_Code_in =null
		end


		insert HIS..MES_MET (numero, jour, dispo, code, debut, fin)
		values( @va_Numero_in, @va_Jour_in,
		 	@va_Dispo_in, @va_Code_in, @va_Debut_in, @va_Fin_in)


	end
	else
	begin
		/* prendre les nouvelles valeurs de horo_x si elles sont valuees  */
		/* - correcte et null avec la valeur actuellement en base reconstituee et null */
		/* - valuee et conserve la meme validite par rapport a celle en base           */
		if (@va_Dispo_in = XDC_VRAI) 
		begin
			select	@vl_debut	= @va_Debut_in
			select	@vl_fin	= @va_Fin_in
			select	@vl_code	= @va_Code_in
			select	@vl_dispo	= @va_Dispo_in
		end
		

		/* modification de cette mesure */
		update HIS..MES_MET set
		dispo		= @vl_dispo,
		code		= @vl_code,
		debut		= @vl_debut,
		fin		= @vl_fin
		where   numero = @va_Numero_in and
			jour = @va_Jour_in
	end

	if @@rowcount != 1
		select @vl_Retour = XDC_NOK
	else	select @vl_Retour = XDC_OK

#endif
	return @vl_Retour
go
