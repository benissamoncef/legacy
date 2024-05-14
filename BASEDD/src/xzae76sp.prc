/*E*/
/*  Fichier : $Id: xzae76sp.prc,v 1.3 2012/10/31 21:34:23 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2012/10/31 21:34:23 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzae76sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des fiches Main courante
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	16/10/12 : Creation (DEM 1032)  1.1
* JPL	31/10/12 : Selection des Fmc sans date de fin ni de cloture  1.2
* JPL	31/10/12 : Ordonner la selection selon l'autoroute et les Pr  1.3
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne les données de Conditions de Conduite Hivernale
*  attachees à une ou plusieurs Fmc de type Meteo.
*
*   . si un identifiant (numero,cle) de Fmc est fourni
*     retourne les conditions attachees a cette Fmc,
*     à l'horodate spécifiée si elle est fournie, sinon les plus récentes.
*
*   . si aucune Fmc n'est indiquée,
*     retourne les conditions attachees a toutes les Fmc Meteo en cours
*     (c'est à dire non clôturées) EN DATE DE LA DERNIERE VALIDATION
*     (fournissant le lieu de l'evenement -l'autoroute- indispensable).
*
* SEQUENCE D'APPEL
*
* SP	XZAE76_Lire_Conditions_Conduite
*
* Arguments en entree :
*
*
* Arguments en sortie : Aucun
*
*
* Code retour :
*
* XDC_OK
* XDC_NOK           : evenement inconnu ou de type incorrect pour l'operation
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* XDC_OK
* <0                : erreur sybase
*
* Conditions d'utilisation :
*
* La recherche des conditions de conduite
* pour toutes les Fmc Meteo en cours n'est disponible qu'au CI.
*
* Fonction :
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE76' and type = 'P')
	drop procedure XZAE76
go


create procedure XZAE76
	@va_Horodate_in		datetime	= null,	-- par defaut heure courante
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null
as
	declare	@vl_Status	int


	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumEvt_in = 0	select @va_NumEvt_in = null
	if @va_CleEvt_in = 0	select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null


	/*A Verifier la validite des parametres d'entree */
	if (@va_NumEvt_in = null  and  @va_CleEvt_in <> null)  or
	   (@va_NumEvt_in <> null  and  @va_CleEvt_in = null)
		return XDC_ARG_INV


	/*A Si un identifiant de Fmc est specifie */
	if (@va_NumEvt_in <> null  and  @va_CleEvt_in <> null)
	begin
		/*A Prendre l'heure courante dans le cas ou elle est nulle */
		if (@va_Horodate_in = null)
			select @va_Horodate_in = XDC_DATE_INFINIE

#ifdef HIST
		/*A retourner les conditions de conduite a l'horodate precisee ou anterieures */
		select	H.numero, H.cle,
			CC.horodate_validation,
			H.autoroute,
			CC.PR_debut, CC.PR_fin,
			CC.cch_VR, CC.cch_VM2, CC.cch_VM1, CC.cch_VL, CC.cch_BAU,
			CC.cch_VR_I, CC.cch_VM2_I, CC.cch_VM1_I, CC.cch_VL_I, CC.cch_BAU_I,
			CC.precipitations,
			CC.sit

		from	HIS..FMC_HIS H, CFG..RES_AUT_INV AI, HIS..FMC_MET_CCH CC

		where	H.numero = @va_NumEvt_in  and  H.cle = @va_CleEvt_in
		  and	H.horodate_validation = (select max (horodate_validation) from HIS..FMC_HIS
			                          where numero = @va_NumEvt_in and cle = @va_CleEvt_in
			                            and horodate_validation <= @va_Horodate_in)
		  and	AI.numero = H.autoroute
		  and	CC.numero = H.numero  and  CC.cle = H.cle
		  and	CC.horodate_validation = H.horodate_validation

		order by AI.inversee * CC.PR_debut

#else
#ifdef CI
		/*! cas ou la procedure est creee au CI */

		/*A si la date donnee est anterieure d'un mois au moins */
		/*A rechercher l'evenement dans la base HIS sinon dans  */
		/*A la base EXP (au niveau du CI)                       */
		if @va_Horodate_in < (select dateadd (month, -1, getdate()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAE76
			                     @va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in
			if @vl_Status = null
				return XDC_PRC_INC
			else return @vl_Status
		end
		else
		begin
			/*A retourner les conditions de conduite a l'horodate precisee ou anterieures */
			select	H.numero, H.cle,
				CC.horodate_validation,
				H.autoroute,
				CC.PR_debut, CC.PR_fin,
				CC.cch_VR, CC.cch_VM2, CC.cch_VM1, CC.cch_VL, CC.cch_BAU,
				CC.cch_VR_I, CC.cch_VM2_I, CC.cch_VM1_I, CC.cch_VL_I, CC.cch_BAU_I,
				CC.precipitations,
				CC.sit

			from	EXP..FMC_HIS H, CFG..RES_AUT_INV AI, EXP..FMC_MET_CCH CC

			where	H.numero = @va_NumEvt_in  and  H.cle = @va_CleEvt_in
			  and	H.horodate_validation = (select max (horodate_validation) from EXP..FMC_HIS
							  where numero = @va_NumEvt_in and cle = @va_CleEvt_in
							    and horodate_validation <= @va_Horodate_in)
			  and	AI.numero = H.autoroute
			  and	CC.numero = H.numero  and  CC.cle = H.cle
			  and	CC.horodate_validation = H.horodate_validation

			order by AI.inversee * CC.PR_debut
		end

#else
		/*! cas ou la procedure est creee sur le serveur d'un district */

		/*A retourner les conditions de conduite a l'horodate precisee ou anterieures */
		select	H.numero, H.cle,
			CC.horodate_validation,
			H.autoroute,
			CC.PR_debut, CC.PR_fin,
			CC.cch_VR, CC.cch_VM2, CC.cch_VM1, CC.cch_VL, CC.cch_BAU,
			CC.cch_VR_I, CC.cch_VM2_I, CC.cch_VM1_I, CC.cch_VL_I, CC.cch_BAU_I,
			CC.precipitations,
			CC.sit

		from	EXP..FMC_HIS H, CFG..RES_AUT_INV AI, EXP..FMC_MET_CCH CC

		where	H.numero = @va_NumEvt_in  and  H.cle = @va_CleEvt_in
		  and	H.horodate_validation = (select max (horodate_validation) from EXP..FMC_HIS
			                          where numero = @va_NumEvt_in and cle = @va_CleEvt_in
			                            and horodate_validation <= @va_Horodate_in)
		  and	AI.numero = H.autoroute
		  and	CC.numero = H.numero  and  CC.cle = H.cle
		  and	CC.horodate_validation = H.horodate_validation

		order by AI.inversee * CC.PR_debut
#endif
#endif
		return XDC_OK
	end



#ifdef CI
	/*A Si aucune Fmc n'est specifiee */
	if (@va_NumEvt_in = null  and  @va_CleEvt_in = null)
	begin
		/*A On ne recherche pas les conditions generales a une date donnee */
		if (@va_Horodate_in <> null)
			return XDC_NOK

		/*A retourner les conditions de conduite en date de la derniere validation  */
		/*A pour les Fmc Meteo en cours (ni finies ni cloturees) sur tous les sites */
		select	G.numero, G.cle,
			CC.horodate_validation,
			H.autoroute,
			CC.PR_debut, CC.PR_fin,
			CC.cch_VR, CC.cch_VM2, CC.cch_VM1, CC.cch_VL, CC.cch_BAU,
			CC.cch_VR_I, CC.cch_VM2_I, CC.cch_VM1_I, CC.cch_VL_I, CC.cch_BAU_I,
			CC.precipitations,
			CC.sit

			from	EXP..FMC_GEN G, EXP..FMC_HIS H, EXP..FMC_MET_CCH CC

			where	G.type = XZAEC_FMC_Meteo
			  and	fin is null  and  cloture is null
			  and	H.numero = G.numero  and  H.cle = G.cle
			  and	CC.numero = G.numero  and  CC.cle = G.cle

			group by G.numero, CC.cle

			having	H.horodate_validation = max (H.horodate_validation)
			  and	CC.numero = G.numero  and  CC.cle = G.cle
			  and	CC.horodate_validation = H.horodate_validation

			order by H.autoroute, CC.PR_debut

		return XDC_OK
	end
#endif

	return XDC_NOK
go
