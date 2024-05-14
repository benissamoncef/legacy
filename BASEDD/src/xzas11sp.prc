/*E*/
/*  Fichier : $Id: xzas11sp.prc,v 1.6 1998/01/08 10:32:16 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/01/08 10:32:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/11/94	: Creation	(V 1.1)
* B.G.	12/01/95	: arrondi horodate	(V 1.2)
* B.G.	19/01/95	: filtre les horodates	(V 1.3)
* C.T.	27/01/95	: Changement du nom de la proc	(V 1.4)
* C.T.	16/09/96	: Ajout cas sur DS et DC (V 1.5)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les donnees comptage poids
* 
* Sequence d'appel
* SP	XZAS11_Ajout_Mesure_Poids
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Datetime		va_Horodate_in
* XDY_Entier		va_Cumul_in
* XDY_Entier		va_NbPl_in
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
* - va_Station_in, va_Sens_in, va_Horodate_in sont obligatoires
* - si va_Cumul_in est non value, utiliser XDC_RADT_CUMUL_INCONNU
* de meme pour va_NbPl_in (XDC_RADT_NBPL_INCONNU)
* - si une de ces mesures est valuee une fois, la fois suivante elle ne pourra
* pas etre non valuee en base(aucune erreur n'est retournee par la procedure)
* par modification
* 
* Fonction
* Inserer ou Modifier dans la table MESURE_POIDS
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS11' and type = 'P')

    drop procedure XZAS11

go

create procedure XZAS11
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Horodate_in		datetime	= null,
	@va_Cumul_in		int		= null,
	@va_NbPL_in		int		= null
as
	declare @vl_Site T_SITE, @vl_Cumul int, @vl_NbPL int

	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_Horodate_in = null or
	   @va_Cumul_in = null or @va_NbPL_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
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
		
	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in,3))

	/*A on jette les mesures vieilles de plus d'un mois */
	
	if @va_Horodate_in<dateadd(month,-1,getdate())
		return XDC_ARG_INV

	/*A recherche si la mesure existe en base */
	select 
		@vl_Cumul	= cumul_poids,
		@vl_NbPL	= nombre_pl
	from EXP..MES_POI
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		horodate = @va_Horodate_in

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if @@rowcount = 0
	begin
		/* les elements non values sont mis a null en base */
		if @va_Cumul_in = XDC_RADT_CUMUL_INCONNU
			select @va_Cumul_in = null
		if @va_NbPL_in = XDC_RADT_NBPL_INCONNU
			select @va_NbPL_in = null

		insert EXP..MES_POI (station,sens, horodate, cumul_poids,
		nombre_pl, sit)
		values( @va_Station_in, @va_Sens_in, @va_Horodate_in,
		@va_Cumul_in, @va_NbPL_in, @vl_Site)
	end
	else
	begin
		/* prendre les nouvelles valeurs de cumul, nombre_pl si elles sont */
		/* valuees                                                         */
		if (@va_Cumul_in != XDC_RADT_CUMUL_INCONNU and @vl_Cumul = null) 
		begin
			select	@vl_Cumul = @va_Cumul_in
		end
		if (@va_NbPL_in != XDC_RADT_NBPL_INCONNU and  @vl_NbPL = null)
		begin
			select	@vl_NbPL = @va_NbPL_in
		end

		/* modification de cette mesure */
		update EXP..MES_POI set
			cumul_poids	= @vl_Cumul,
			nombre_pl	= @vl_NbPL
		where   station = @va_Station_in and
			sens = @va_Sens_in and
			horodate = @va_Horodate_in
	end

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
