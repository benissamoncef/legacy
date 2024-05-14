/*E*/
/*  Fichier : $Id: xzae207sp.prc,v 1.6 2020/09/16 08:39:43 gesconf Exp $        Release : $Revision: 1.6 $        Date : $Date: 2020/09/16 08:39:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae207sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI	18/07/2005 : Creation DEM487 v1.1
* JPL	05/11/2007 : Retour cr <> Nok en cas d'absence (pour DEM 706 mais cas general) 1.2
* JMG	13/10/2008 : ajout libelle nature en sortie 1.3 DEM/836
* JPL	18/05/2009 : Retourne le libelle en cas d'appel en cascade sur HIStorique 1.4
* ABE	10/09/2020 : DEM-SAE130 Datex2 Récuperation du code et libelle avec le curseur (pb récuperation par asql_ctparam_char_output) V1.5
* PNI	16/09/2020 : correctif suite a la liv SAE_130 : code nature et libelle n'etait plus mise a jour 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante de classe Nature a l'heure precisee
* 
* Sequence d'appel
* SP	XZAE207_Lire_nature
* 
* Arguments en entree
* XDY_Horodate          va_Horodate_in : par defaut heure courante
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet 		va_code_nature_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* 
* Conditions d'utilisation
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans la table FMC_NAT
* where numero evenement and horodate
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE207' and type = 'P')
	drop procedure XZAE207
go


create procedure XZAE207
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_code_nature_out	tinyint = null output,
	@va_libelle_nature_out	char(100) = null output
as

	declare	@vl_TypeEvt	smallint,
		@vl_Status	int
																					
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A date servant a la recherche du complement de la fiche main courante */
	if @va_Horodate_in = null
	begin
		select @va_Horodate_in = XDC_DATE_INFINIE
	end

	
	/*A lecture du code de la nature de l'evenement */
#ifdef HIST
	select
		--@va_code_nature_out		= code,
		--@va_libelle_nature_out	= nature
		code,
		nature
	from HIS..FMC_NAT
	where	numero = @va_NumEvt_in and 				
		cle = @va_CleEvt_in and 
		horodate in 
		(select max(horodate) from HIS..FMC_NAT
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and 
			horodate <= @va_Horodate_in) 

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE207
						@va_Horodate_in,
						@va_NumEvt_in,
						@va_CleEvt_in,
						@va_code_nature_out output,
						@va_libelle_nature_out output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
		select
			@va_code_nature_out		= code,
			@va_libelle_nature_out          = nature
		from EXP..FMC_NAT
		where	numero = @va_NumEvt_in and 
			cle = @va_CleEvt_in and 
			horodate in 
			(select max(horodate) from EXP..FMC_NAT
			where	numero = @va_NumEvt_in and 
				cle = @va_CleEvt_in and 
				horodate <= @va_Horodate_in) 
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	select
		@va_code_nature_out		= code,
		@va_libelle_nature_out        = nature
	from EXP..FMC_NAT
	where	EXP..FMC_NAT.numero = @va_NumEvt_in and 
		EXP..FMC_NAT.cle = @va_CleEvt_in and 
		EXP..FMC_NAT.horodate in 
			(select max(horodate) from EXP..FMC_NAT
			where	EXP..FMC_NAT.numero = @va_NumEvt_in and 
				EXP..FMC_NAT.cle = @va_CleEvt_in and 
				EXP..FMC_NAT.horodate <= @va_Horodate_in) 
#endif
#endif

	/*A test si une fiche a ete trouvee sinon erreur */
	if @@rowcount = 0
	begin
		select @va_code_nature_out = 0
		select @va_libelle_nature_out=""
		/* En toute rigueur, on devrait trouver le @vl_TypeEvt dans FMC_GEN, puis
		 * select @va_code_nature_out = code from CFG..LIB_PRE
		 * where notyp = XDC_TYPE_LIB_XXXXX and rtrim (libelle) = null
		 * avec XXXXX = OBSTACLE, PIETON ou AUTRE_EVT selon le vl_TypeEvt.
						 */
		select @va_code_nature_out,@va_libelle_nature_out
		return XZAEC_FMC_PAS_DONNEES
	end								
		select @va_code_nature_out,@va_libelle_nature_out
	return XDC_OK
go
