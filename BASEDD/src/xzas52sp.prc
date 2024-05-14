/*E*/
/*  Fichier : $Id: xzas52sp.prc,v 1.2 1998/01/08 10:32:31 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/01/08 10:32:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas52sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Mise a jour et ecriture des Quantites de Poids par
* Classe de poids
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	02/10/94	: Creation	(V 1.1)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage
* 
* Sequence d'appel
* SP	XZAS52_Ajout_Mesure_PC
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_PC1_in
* XDY_Mot		va_PC2_in
* XDY_Mot		va_PC3_in
* XDY_Mot		va_PC4_in
* XDY_Mot		va_PC5_in
* XDY_Mot		va_PC6_in
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
* - si va_PCx_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table DEBIT HORAIRE PAR CLASSE SIHL
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS52' and type = 'P')

    drop procedure XZAS52

go

create procedure XZAS52
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		T_VOIE		= null,
	@va_Horodate_in		datetime	= null,
	@va_PC1_in		smallint	= null,
	@va_PC2_in		smallint	= null,
	@va_PC3_in		smallint	= null,
	@va_PC4_in		smallint	= null,
	@va_PC5_in		smallint	= null,
	@va_PC6_in		smallint	= null
	
as
	declare @vl_Site T_SITE,
		@vl_Minutes int, @vl_Retour int,
		@vl_PC1 		smallint,
		@vl_PC2 		smallint,
		@vl_PC3 		smallint,
		@vl_PC4 		smallint,
		@vl_PC5 		smallint,
		@vl_PC6 		smallint


	select @vl_Retour = XDC_OK


#ifndef CI

	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_Voie_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE 
		return XDC_ARG_INV

	/*A si toutes les mesures sont inconnues : aucun traitement */
	if 	@va_PC1_in = null and 
		@va_PC2_in = null and 
		@va_PC3_in = null and 
		@va_PC4_in = null and 
		@va_PC5_in = null and 
		@va_PC6_in = null  
		   return XDC_OK

	/*A recherche du site local */
	select @vl_Site = numero
	from CFG..RES_DIS
	where serveur = @@servername

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
		@vl_PC1		= PC1,
		@vl_PC2		= PC2,
		@vl_PC3		= PC3,
		@vl_PC4		= PC4,
		@vl_PC5		= PC5,
		@vl_PC6		= PC6
	from EXP..MES_QPC
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_PC1_in = XDC_RADT_Q_INCONNU
			select @va_PC1_in = null
		if @va_PC2_in = XDC_RADT_Q_INCONNU
			select @va_PC2_in = null
		if @va_PC3_in = XDC_RADT_Q_INCONNU
			select @va_PC3_in = null
		if @va_PC4_in = XDC_RADT_Q_INCONNU
			select @va_PC4_in = null
		if @va_PC5_in = XDC_RADT_Q_INCONNU
			select @va_PC5_in = null
		if @va_PC6_in = XDC_RADT_Q_INCONNU
			select @va_PC6_in = null


		insert EXP..MES_QPC (station,sens, voie, horodate, PC1, PC2, PC3, PC4,
		PC5, PC6, sit)
		values( @va_Station_in, @va_Sens_in, @va_Voie_in, @va_Horodate_in,
		 	@va_PC1_in, @va_PC2_in, @va_PC3_in, @va_PC4_in, @va_PC5_in, 
		 	@va_PC6_in, @vl_Site)
	end
	else
	begin
		/* prendre les nouvelles valeurs de PCx si elles sont valuees  */
		/* - correcte et null avec la valeur actuellement en base reconstituee et null */
		/* - valuee et conserve la meme validite par rapport a celle en base           */
		if (@va_PC1_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_PC1	= @va_PC1_in
		end
		if (@va_PC2_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_PC2	= @va_PC2_in
		end
		if (@va_PC3_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_PC3	= @va_PC3_in
		end
		if (@va_PC4_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_PC4	= @va_PC4_in
		end
		if (@va_PC5_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_PC5	= @va_PC5_in
		end
		if (@va_PC6_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_PC6	= @va_PC6_in
		end
		

		/* modification de cette mesure */
		update EXP..MES_QPC set
		PC1		= @vl_PC1,
		PC2		= @vl_PC2,
		PC3		= @vl_PC3,
		PC4		= @vl_PC4,
		PC5		= @vl_PC5,
		PC6		= @vl_PC6
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
	delete EXP..MES_QPC
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in
#endif
	return @vl_Retour
go
