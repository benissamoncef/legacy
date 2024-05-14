/*E*/
/*  Fichier : $Id: xzas50sp.prc,v 1.3 1998/01/08 10:32:27 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/01/08 10:32:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des DEbit horaire par classe
* de silhouette
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(V 1.1)
* P.N.  18/10/94        : Correction du decalage lors de maj base v1.2
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage
* 
* Sequence d'appel
* SP	XZAS50_Ajout_Mesure_KC
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_KC1_in
* XDY_Mot		va_KC2_in
* XDY_Mot		va_KC3_in
* XDY_Mot		va_KC4_in
* XDY_Mot		va_KC5_in
* XDY_Mot		va_KC6_in
* XDY_Mot		va_KC7_in
* XDY_Mot		va_KC8_in
* XDY_Mot		va_KC9_in
* XDY_Mot		va_KC10_in
* XDY_Mot		va_KC11_in
* XDY_Mot		va_KC12_in
* XDY_Mot		va_KC13_in
* XDY_Mot		va_KC14_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 station inexistante
* 
* Conditions d'utilisation
* - va_Station_in, va_Sens_in, va_Voie_in, va_Horodate_in sont obligatoires
* - si va_KCx_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table DEBIT HORAIRE PAR CLASSE SIHL
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS50' and type = 'P')
    drop procedure XZAS50
go

create procedure XZAS50
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		T_VOIE		= null,
	@va_Horodate_in		datetime	= null,
	@va_KC1_in		smallint	= null,
	@va_KC2_in		smallint	= null,
	@va_KC3_in		smallint	= null,
	@va_KC4_in		smallint	= null,
	@va_KC5_in		smallint	= null,
	@va_KC6_in		smallint	= null,
	@va_KC7_in		smallint	= null,
	@va_KC8_in		smallint	= null,
	@va_KC9_in		smallint	= null,
	@va_KC10_in		smallint	= null,
	@va_KC11_in		smallint	= null,
	@va_KC12_in		smallint	= null,
	@va_KC13_in		smallint	= null,
	@va_KC14_in		smallint	= null
	
as
	declare @vl_Site T_SITE,
		@vl_Minutes int, @vl_Retour int,
		@vl_KC1 		smallint,
		@vl_KC2 		smallint,
		@vl_KC3 		smallint,
		@vl_KC4 		smallint,
		@vl_KC5 		smallint,
		@vl_KC6 		smallint,
		@vl_KC7 		smallint,
		@vl_KC8 		smallint,
		@vl_KC9 		smallint,
		@vl_KC10 		smallint,
		@vl_KC11 		smallint,
		@vl_KC12 		smallint,
		@vl_KC13 		smallint,
		@vl_KC14 		smallint

	select @vl_Retour = XDC_OK
	
#ifndef CI

	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_Voie_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE 
		return XDC_ARG_INV

	/*A si toutes les mesures sont inconnues : aucun traitement */
	if 	@va_KC1_in = null and 
		@va_KC2_in = null and 
		@va_KC3_in = null and 
		@va_KC4_in = null and 
		@va_KC5_in = null and 
		@va_KC6_in = null and 
		@va_KC7_in = null and 
		@va_KC8_in = null and 
		@va_KC9_in = null and 
		@va_KC10_in = null and 
		@va_KC11_in = null and 
		@va_KC12_in = null and 
		@va_KC12_in = null and 
		@va_KC13_in = null and 
		@va_KC13_in = null
		   return XDC_OK

	/*A recherche du site local */
	select @vl_Site = numero
	from CFG..RES_DIS
	where serveur = @@servername

	if @vl_Site = null
		return XDC_NOK

	/*A verifier l'existance de la station */
	if not exists ( select * from CFG..RES_CHA 
			where	station = @va_Station_in and 
				sens = @va_Sens_in)
		return XDC_ARG_INV

	/*A arrondi horodate */
	
	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))
	
	/*A arrondi horodate a 6 minutes */
	
/*	select @vl_Minutes=datepart(minute,@va_Horodate_in)%6*/
	select @vl_Minutes=0
	
	if @vl_Minutes<>0
		select @va_Horodate_in=dateadd(minute,-@vl_Minutes,@va_Horodate_in)
	
	/*A on jette les mesures vieilles de plus d'un mois */
	
	if @va_Horodate_in<dateadd(month,-1,getdate())
		return XDC_ARG_INV

	/*A recherche si la mesure existe en base */
	select 
		@vl_KC1		= KC1,
		@vl_KC2		= KC2,
		@vl_KC3		= KC3,
		@vl_KC4		= KC4,
		@vl_KC5		= KC5,
		@vl_KC6		= KC6,
		@vl_KC7		= KC7,
		@vl_KC8		= KC8,
		@vl_KC9		= KC9,
		@vl_KC10	= KC10,
		@vl_KC11	= KC11,
		@vl_KC12	= KC12,
		@vl_KC13	= KC13,
		@vl_KC14	= KC14
	from EXP..MES_KCS
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_KC1_in = XDC_RADT_Q_INCONNU
			select @va_KC1_in = null
		if @va_KC2_in = XDC_RADT_Q_INCONNU
			select @va_KC2_in = null
		if @va_KC3_in = XDC_RADT_Q_INCONNU
			select @va_KC3_in = null
		if @va_KC4_in = XDC_RADT_Q_INCONNU
			select @va_KC4_in = null
		if @va_KC5_in = XDC_RADT_Q_INCONNU
			select @va_KC5_in = null
		if @va_KC6_in = XDC_RADT_Q_INCONNU
			select @va_KC6_in = null
		if @va_KC7_in = XDC_RADT_Q_INCONNU
			select @va_KC7_in = null
		if @va_KC8_in = XDC_RADT_Q_INCONNU
			select @va_KC8_in = null
		if @va_KC9_in = XDC_RADT_Q_INCONNU
			select @va_KC9_in = null
		if @va_KC10_in = XDC_RADT_Q_INCONNU
			select @va_KC10_in = null
		if @va_KC11_in = XDC_RADT_Q_INCONNU
			select @va_KC11_in = null
		if @va_KC12_in = XDC_RADT_Q_INCONNU
			select @va_KC12_in = null
		if @va_KC13_in = XDC_RADT_Q_INCONNU
			select @va_KC13_in = null
		if @va_KC14_in = XDC_RADT_Q_INCONNU
			select @va_KC14_in = null


		insert EXP..MES_KCS (station,sens, voie, horodate, KC1, KC2, KC3, KC4,
		KC5, KC6, KC7, KC8, KC9, KC10, KC11, KC12, KC13, KC14, sit)
		values( @va_Station_in, @va_Sens_in, @va_Voie_in, @va_Horodate_in,
		 @va_KC1_in, @va_KC2_in, @va_KC3_in, @va_KC4_in, @va_KC5_in, @va_KC6_in, @va_KC7_in, @va_KC8_in, @va_KC9_in, @va_KC10_in, @va_KC11_in, @va_KC12_in, @va_KC13_in, @va_KC14_in, @vl_Site)
	end
	else
	begin
		/* prendre les nouvelles valeurs de KCx si elles sont valuees  */
		/* - correcte et null avec la valeur actuellement en base reconstituee et null */
		/* - valuee et conserve la meme validite par rapport a celle en base           */
		if (@va_KC1_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC1	= @va_KC1_in
		end
		if (@va_KC2_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC2	= @va_KC2_in
		end
		if (@va_KC3_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC3	= @va_KC3_in
		end
		if (@va_KC4_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC4	= @va_KC4_in
		end
		if (@va_KC5_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC5	= @va_KC5_in
		end
		if (@va_KC6_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC6	= @va_KC6_in
		end
		if (@va_KC7_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC7	= @va_KC7_in
		end
		if (@va_KC8_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC8	= @va_KC8_in
		end
		if (@va_KC9_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC9	= @va_KC9_in
		end
		if (@va_KC10_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC10	= @va_KC10_in
		end
		if (@va_KC11_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC11	= @va_KC11_in
		end
		if (@va_KC12_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC12	= @va_KC12_in
		end
		if (@va_KC13_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC13	= @va_KC13_in
		end
		if (@va_KC14_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_KC14	= @va_KC14_in
		end
		

		/* modification de cette mesure */
		update EXP..MES_KCS set
		KC1		= @vl_KC1,
		KC2		= @vl_KC2,
		KC3		= @vl_KC3,
		KC4		= @vl_KC4,
		KC5		= @vl_KC5,
		KC6		= @vl_KC6,
		KC7		= @vl_KC7,
		KC8		= @vl_KC8,
		KC9		= @vl_KC9,
		KC10		= @vl_KC10,
		KC11		= @vl_KC11,
		KC12		= @vl_KC12,
		KC13		= @vl_KC13,
		KC14		= @vl_KC14
		where   station = @va_Station_in and
			sens = @va_Sens_in and
			voie = @va_Voie_in and
			horodate = @va_Horodate_in
	end

	if @@rowcount != 1
		select @vl_Retour = XDC_NOK
	else	select @vl_Retour = XDC_OK

	/*A purge d'une mesure 32 jours avant la date courante */
	select @va_Horodate_in=getdate()
/*	select @vl_Minutes=datepart(minute,@va_Horodate_in)%6
	if @vl_Minutes<>0
		select @va_Horodate_in=dateadd(minute,-@vl_Minutes,@va_Horodate_in)
*/
	select @va_Horodate_in=dateadd(day,-XDC_OFFSET_EN_JOURS_PURGE_BASE,@va_Horodate_in)
	delete EXP..MES_KCS
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in
#endif
	return @vl_Retour
go
