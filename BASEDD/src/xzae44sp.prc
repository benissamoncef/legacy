/*E*/
/*  Fichier : $Id: xzae44sp.prc,v 1.10 2008/11/17 11:21:21 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2008/11/17 11:21:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae44sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
* C.T.	12/08/94	: Modif position param entree
*                         (test ne fonctionne plus)
* C.T.  10/11/94	: Modif bugs applix V 1.4
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.5)
* B.G.	23/12/94	: Modif fin<->debut (1.6)
* B.G.	17/01/95	: Modif date nulle (1.7)
* C.T.	05/02/98	: Suppression de retour d'erreur apres XZAO;22 si loc incorrecte (1.8)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.9)
* JMG	16/09/08	: ajout libelle nature en sortie 1.10 DEM/836
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante meteo a 
* l'heure precisee
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAE44_Lire_Fiche_Meteo
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_PR	va_PRFin_out
* XDY_Octet	va_Perturbation_out
* XDY_Mot	va_Visibilite_out
* XDY_Entier	va_Longueur_out
* 
* Code retour
* XDC_OK
* XDC_NOK		: pas de complement de fiche trouve
* XDC_ARG_INV        	: parametre obligatoire manquant
* XDC_PRC_INC        	: XZAO;22 inexistante
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
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
* Select dans la table METEO_HISTORIQUE
* where numero evenement and horodate
---------------------------------------------------------*/

use PRC
go

create procedure XZAE;44
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_PRFin_out		T_PR = null	output,
	@va_Perturbation_out	tinyint = null	output,
	@va_Visibilite_out	smallint = null	output,
	@va_Longueur_out	int = null	output,
	@va_nature_out		smallint = null output,
	@va_libelle_nature_out	char(25) = null output
as
	declare @vl_Autoroute T_AUTOROUTE,
		@vl_PRDebut T_PR,
		@vl_Horodate datetime,
		@vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres d'entree obligatoire */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A prendre l'heure courante dans le cas ou elle est nulle */
	if @va_Horodate_in = null
		select @va_Horodate_in = XDC_DATE_INFINIE

#ifdef HIST
	begin
		/*B lecture des informations de la meteo a l'heure precisee */
		select
			@vl_PRDebut = PR,
			@va_Perturbation_out = type_de_perturbation,
			@va_Visibilite_out = visibilite,
			@vl_Horodate = horodate,
			@va_nature_out = type_de_perturbation
		from HIS..FMC_MET
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate in ( 	select max(horodate) from HIS..FMC_MET
					where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
						horodate <= @va_Horodate_in )
	
		/*B test si un seul complement meteo est trouve */
		if @@rowcount != 1
			return XDC_NOK
	
		/*libelle de la nature*/
		select @va_libelle_nature_out=L.libelle from CFG..LIB_PRE L , HIS..FMC_MET M
			 where M.numero=@va_NumEvt_in and M.cle=@va_CleEvt_in and L.notyp=20 and L.code=M.type_de_perturbation

		/*B recherche du PR de fin de la perturbation et le numero de */
		/*B l'autoroute                                               */
		select
			@vl_Autoroute = autoroute,
			@va_PRFin_out = PR
		from HIS..FMC_HIS
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_validation = @vl_Horodate
	end

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE;44
				@va_Horodate_in	, @va_NumEvt_in	, @va_CleEvt_in, @va_PRFin_out output, @va_Perturbation_out output,
				@va_Visibilite_out output, @va_Longueur_out output, @va_nature_out output, @va_libelle_nature_out	output
 			if @vl_Status = null
                       	 	return XDC_PRC_INC
                	else return @vl_Status
		end
	else
	begin
		/*B lecture des informations de la meteo a l'heure precisee */
		select
			@vl_PRDebut = PR,
			@va_Perturbation_out = type_de_perturbation,
			@va_Visibilite_out = visibilite,
			@vl_Horodate = horodate,
			@va_nature_out = type_de_perturbation
		from EXP..FMC_MET
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate in ( 	select max(horodate) from EXP..FMC_MET
					where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
						horodate <= @va_Horodate_in )
	
		/*B test si un seul complement meteo est trouve */
		if @@rowcount != 1
			return XDC_NOK
	
		/*libelle de la nature*/
		select @va_libelle_nature_out=L.libelle from CFG..LIB_PRE L , EXP..FMC_MET M
			 where M.numero=@va_NumEvt_in and M.cle=@va_CleEvt_in and L.notyp=20 and L.code=M.type_de_perturbation

		/*B recherche du PR de fin de la perturbation et le numero de */
		/*B l'autoroute                                               */
		select
			@vl_Autoroute = autoroute,
			@va_PRFin_out = PR
		from EXP..FMC_HIS
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
			horodate_validation = @vl_Horodate
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des informations de la meteo a l'heure precisee */
	select
		@vl_PRDebut = PR,
		@va_Perturbation_out = type_de_perturbation,
		@va_Visibilite_out = visibilite,
		@vl_Horodate = horodate,
		@va_nature_out = type_de_perturbation
	from EXP..FMC_MET
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate in ( 	select max(horodate) from EXP..FMC_MET
				where 	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
					horodate <= @va_Horodate_in )

	/*A test si un seul complement meteo est trouve */
	if @@rowcount != 1
		return XDC_NOK

	/*libelle de la nature*/
	select @va_libelle_nature_out=L.libelle from CFG..LIB_PRE L , EXP..FMC_MET M
		 where M.numero=@va_NumEvt_in and M.cle=@va_CleEvt_in and L.notyp=20 and L.code=M.type_de_perturbation

	/*A recherche du PR de fin de la perturbation et le numero de l'autoroute */
	select
		@vl_Autoroute = autoroute,
		@va_PRFin_out = PR
	from EXP..FMC_HIS
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		horodate_validation = @vl_Horodate
#endif
#endif

	/*A test si une fiche meteo est trouve */
	if @@rowcount != 1
		return XDC_NOK

	/*A calcul de la longueur de la perturbation                                   */
	/*! la perturbation est sur une meme autoroute (sinon un 2eme evt est inserer) */
	exec @vl_Status = XZAO;22 @vl_Autoroute, @vl_PRDebut, @vl_Autoroute, @va_PRFin_out, 
				 @va_Longueur_out output

	/*A test si la procedure s'est correctement deroulee */
	if @vl_Status = null
		return XDC_PRC_INC

	select @va_PRFin_out=@vl_PRDebut
	
	return XDC_OK
go


