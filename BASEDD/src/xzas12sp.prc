/*E*/
/*  Fichier : $Id: xzas12sp.prc,v 1.9 1998/03/24 09:51:36 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 1998/03/24 09:51:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/12/94	: Creation	(V 1.1)
* B.G.	12/01/95	: arrondi horodate	(V 1.2)
* B.G.	19/01/95	: filtre les horodates	(V 1.3)
* C.T.	27/01/95	: Changement du nom de la proc	(V 1.4)
* C.T.	09/08/95	: ajout de la suppreesion de la mesure a 
*                         meme date moins 32 jours (V 1.5)
* C.T.	22/08/95	: Modif date de delete (V 1.6)
* C.T.  16/09/96        : Ajout cas sur DS et DC (V 1.7)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.8
* C.T.  24/03/98        : Correction purge (V 1.9)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met en base les niveaux de service des autoroutes
* 
* Sequence d'appel
* SP	XZAS12_Ajout_Niveau_Service
* 
* Arguments en entree
* XDY_Mot		va_Station_in	: numero de station
* XDY_Sens		va_Sens_in
* XDY_Datetime		va_Horodate_in
* XDY_Octet		va_Niveau_in
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
* - si va_Niveau_in est non value, utiliser XDC_RADT_NIV_INCONNU
* - si va_Niveau_in est non value, aucune action en base
* (aucune erreur n'est retournee par la procedure)
* 
* Fonction
* Inserer ou Modifier dans la table NIVEAU_SERVICE
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS12' and type = 'P')
    drop procedure XZAS12
go

create procedure XZAS12
	@va_Station_in		smallint	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Horodate_in		datetime	= null,
	@va_Niveau_in		tinyint		= null
as
	declare @vl_Site T_SITE, @vl_Retour int, @vl_Horodate smalldatetime

	/*A controle parametres en entree */
	if @va_Station_in = null or @va_Sens_in = null or
	   @va_Horodate_in = null or @va_Niveau_in = null
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

	select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))

	/*A on jette les mesures vieilles de plus d'un mois */
	
	if @va_Horodate_in<dateadd(month,-1,getdate())
		return XDC_ARG_INV

	if @va_Niveau_in = XDC_RADT_NIV_INCONNU
		return XDC_OK

	/*A si la mesure n'existe pas alors inserer la mesure */
	/*A sinon modifier la mesure                          */
	if not exists (	select * from EXP..MES_NIV
			where   station = @va_Station_in and
				sens = @va_Sens_in and
				horodate = @va_Horodate_in)
	begin
		insert EXP..MES_NIV (station,sens, horodate, niveau, sit)
		values( @va_Station_in, @va_Sens_in, @va_Horodate_in,
		@va_Niveau_in, @vl_Site)
	end
	else
	begin
		/* modification de cette mesure */
		update EXP..MES_NIV set
			niveau	= @va_Niveau_in
		where   station = @va_Station_in and
			sens = @va_Sens_in and
			horodate = @va_Horodate_in
	end

	if @@rowcount != 1
		select @vl_Retour = XDC_NOK
	else	select @vl_Retour = XDC_OK

	/*A purge d'une mesure 32 jours avant la date courante */
	select @vl_Horodate=dateadd(day,-XDC_OFFSET_EN_JOURS_PURGE_BASE,getdate())
	delete EXP..MES_NIV
	where	station = @va_Station_in and
		sens = @va_Sens_in and
		horodate = @vl_Horodate

	return @vl_Retour
go
