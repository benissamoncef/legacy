/*E*/
/*  Fichier : $Id: xzas51sp.prc,v 1.3 1998/01/08 10:32:29 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/01/08 10:32:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas51sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des Nombres d'essieux par classe
* de poids
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
* SP	XZAS51_Ajout_Mesure_EC
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_Horodate_in
* XDY_Mot		va_EC1_in
* XDY_Mot		va_EC2_in
* XDY_Mot		va_EC3_in
* XDY_Mot		va_EC4_in
* XDY_Mot		va_EC5_in
* XDY_Mot		va_EC6_in
* XDY_Mot		va_EC7_in
* XDY_Mot		va_EC8_in
* XDY_Mot		va_EC9_in
* XDY_Mot		va_EC10_in
* XDY_Mot		va_EC11_in
* XDY_Mot		va_EC12_in
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
* - si va_ECx_in est non value, alors elle aura null en base
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table DEBIT HORAIRE PAR CLASSE SIHL
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS51' and type = 'P')
    drop procedure XZAS51
go

create procedure XZAS51
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		T_VOIE		= null,
	@va_Horodate_in		datetime	= null,
	@va_EC1_in		smallint	= null,
	@va_EC2_in		smallint	= null,
	@va_EC3_in		smallint	= null,
	@va_EC4_in		smallint	= null,
	@va_EC5_in		smallint	= null,
	@va_EC6_in		smallint	= null,
	@va_EC7_in		smallint	= null,
	@va_EC8_in		smallint	= null,
	@va_EC9_in		smallint	= null,
	@va_EC10_in		smallint	= null,
	@va_EC11_in		smallint	= null,
	@va_EC12_in		smallint	= null
	
as
	declare @vl_Site T_SITE,
		@vl_Minutes int, @vl_Retour int,
		@vl_EC1 		smallint,
		@vl_EC2 		smallint,
		@vl_EC3 		smallint,
		@vl_EC4 		smallint,
		@vl_EC5 		smallint,
		@vl_EC6 		smallint,
		@vl_EC7 		smallint,
		@vl_EC8 		smallint,
		@vl_EC9 		smallint,
		@vl_EC10 		smallint,
		@vl_EC11 		smallint,
		@vl_EC12 		smallint


	select @vl_Retour = XDC_OK

#ifndef CI
	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_Voie_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE 
		return XDC_ARG_INV

	/*A si toutes les mesures sont inconnues : aucun traitement */
	if 	@va_EC1_in = null and 
		@va_EC2_in = null and 
		@va_EC3_in = null and 
		@va_EC4_in = null and 
		@va_EC5_in = null and 
		@va_EC6_in = null and 
		@va_EC7_in = null and 
		@va_EC8_in = null and 
		@va_EC9_in = null and 
		@va_EC10_in = null and 
		@va_EC11_in = null and 
		@va_EC12_in = null 
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
		@vl_EC1		= EC1,
		@vl_EC2		= EC2,
		@vl_EC3		= EC3,
		@vl_EC4		= EC4,
		@vl_EC5		= EC5,
		@vl_EC6		= EC6,
		@vl_EC7		= EC7,
		@vl_EC8		= EC8,
		@vl_EC9		= EC9,
		@vl_EC10	= EC10,
		@vl_EC11	= EC11,
		@vl_EC12	= EC12
	from EXP..MES_NEC
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_EC1_in = XDC_RADT_Q_INCONNU
			select @va_EC1_in = null
		if @va_EC2_in = XDC_RADT_Q_INCONNU
			select @va_EC2_in = null
		if @va_EC3_in = XDC_RADT_Q_INCONNU
			select @va_EC3_in = null
		if @va_EC4_in = XDC_RADT_Q_INCONNU
			select @va_EC4_in = null
		if @va_EC5_in = XDC_RADT_Q_INCONNU
			select @va_EC5_in = null
		if @va_EC6_in = XDC_RADT_Q_INCONNU
			select @va_EC6_in = null
		if @va_EC7_in = XDC_RADT_Q_INCONNU
			select @va_EC7_in = null
		if @va_EC8_in = XDC_RADT_Q_INCONNU
			select @va_EC8_in = null
		if @va_EC9_in = XDC_RADT_Q_INCONNU
			select @va_EC9_in = null
		if @va_EC10_in = XDC_RADT_Q_INCONNU
			select @va_EC10_in = null
		if @va_EC11_in = XDC_RADT_Q_INCONNU
			select @va_EC11_in = null
		if @va_EC12_in = XDC_RADT_Q_INCONNU
			select @va_EC12_in = null


		insert EXP..MES_NEC (station,sens, voie, horodate, EC1, EC2, EC3, EC4,
		EC5, EC6, EC7, EC8, EC9, EC10, EC11, EC12, sit)
		values( @va_Station_in, @va_Sens_in, @va_Voie_in, @va_Horodate_in,
		 	@va_EC1_in, @va_EC2_in, @va_EC3_in, @va_EC4_in, @va_EC5_in, @va_EC6_in, 
		 	@va_EC7_in, @va_EC8_in, @va_EC9_in, @va_EC10_in, @va_EC11_in, @va_EC12_in, 
		 	@vl_Site)
	end
	else
	begin
		/* prendre les nouvelles valeurs de ECx si elles sont valuees  */
		/* - correcte et null avec la valeur actuellement en base reconstituee et null */
		/* - valuee et conserve la meme validite par rapport a celle en base           */
		if (@va_EC1_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC1	= @va_EC1_in
		end
		if (@va_EC2_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC2	= @va_EC2_in
		end
		if (@va_EC3_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC3	= @va_EC3_in
		end
		if (@va_EC4_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC4	= @va_EC4_in
		end
		if (@va_EC5_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC5	= @va_EC5_in
		end
		if (@va_EC6_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC6	= @va_EC6_in
		end
		if (@va_EC7_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC7	= @va_EC7_in
		end
		if (@va_EC8_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC8	= @va_EC8_in
		end
		if (@va_EC9_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC9	= @va_EC9_in
		end
		if (@va_EC10_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC10	= @va_EC10_in
		end
		if (@va_EC11_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC11	= @va_EC11_in
		end
		if (@va_EC12_in != XDC_RADT_Q_INCONNU) 
		begin
			select	@vl_EC12	= @va_EC12_in
		end
		

		/* modification de cette mesure */
		update EXP..MES_NEC set
		EC1		= @vl_EC1,
		EC2		= @vl_EC2,
		EC3		= @vl_EC3,
		EC4		= @vl_EC4,
		EC5		= @vl_EC5,
		EC6		= @vl_EC6,
		EC7		= @vl_EC7,
		EC8		= @vl_EC8,
		EC9		= @vl_EC9,
		EC10		= @vl_EC10,
		EC11		= @vl_EC11,
		EC12		= @vl_EC12
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
	delete EXP..MES_NEC
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		voie = @va_Voie_in and
		horodate = @va_Horodate_in
#endif
	return @vl_Retour
go
