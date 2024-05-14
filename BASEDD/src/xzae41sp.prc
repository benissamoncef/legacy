/*E*/
/*  Fichier : $Id: xzae41sp.prc,v 1.12 2011/12/02 14:09:08 gesconf Exp $      Release : $Revision: 1.12 $        Date : $Date: 2011/12/02 14:09:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae41sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
* C.T.	12/08/94	: Modif table FMC_VEH et position
*                         param entree
* C.T.	04/10/94	: Ajout en parametre de retour dans la
*                         liste site_depannage	(V 1.5)
* C.T.  10/11/94	: Modif bugs applix V 1.6
* C.T.	14/11/94	: Modif recherche des infos au CI (V 1.7)
* C.T.	27/09/96	: Recherche des degats domaine dans FMC_GEN (FMC10) (V 1.8)
* C.P.	17/06/98	: Modif separation EXP et HIS (V 1.9)
* JMG	03/12/08	: ajout validation dans FMC_ACC 1.10 DEM/852
* JPL	10/11/11	: Procedure degroupee (renomme sans ';')  1.11
* JPL	10/11/11	: Degroupee; ajout champs de conditions accident dans FMC_ACC (DEM 1006)  1.12
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'un complement de fiche main courante 
* de type accident 
* 
* Sequence d'appel
* SP	XZAE41_Lire_Fiche_Accident
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_VL_out
* XDY_Octet		va_PL_out
* XDY_Octet		va_Remorque_out
* XDY_Octet		va_Car_out
* XDY_Octet		va_Moto_out
* XDY_Octet		va_BlesseLeg_out
* XDY_Octet		va_BlesseGra_out
* XDY_Octet		va_Mort_out
* XDY_Booleen		va_VehEscota_out
* XDY_Booleen		va_MatDang_out
* XDY_Booleen		va_HomEscota_out
* XDY_Booleen		va_Personalite_out
* XDY_Booleen		va_DegatsDom_out	Pour compatibilite seulement
* XDY_Mot		va_Cond_Meteo_out
* XDY_Mot		va_Cond_Visibilite_out
* XDY_Mot		va_Cond_Etat_Chau_out
* XDY_Mot		va_Cond_Chantier_out
*
* XDY_Octet		va_Type_out	-- informations par vehicule implique 
* XDY_Octet		va_Passagers_out
* XDY_Booleen		va_Bebe_out
* XDY_Booleen		va_PersAgee_out
* XDY_Booleen		va_Animaux_out
* XDY_Booleen		va_VehEscota_out
* XDY_Octet		va_Chargement_out
* char(6)		va_CodeMat_out
* XDY_Entier		va_Depannage_out
* XDY_Site		va_SiteDepannage_out
* char(100)		va_Texte_out
* XDY_Octet		va_NumeroVehicule_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
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
* Select dans les tables ACCIDENT et VEHICULE
* where numero numero and horodate
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE41' and type = 'P')
	drop procedure XZAE41
go


create procedure XZAE41
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_VL_out		tinyint = null	output,
	@va_PL_out		tinyint = null	output,
	@va_Remorque_out	tinyint = null	output,
	@va_Car_out		tinyint = null	output,
	@va_Moto_out		tinyint = null	output,
	@va_BlesseLeg_out	tinyint = null	output,
	@va_BlesseGra_out	tinyint = null	output,
	@va_Mort_out		tinyint = null	output,
	@va_VehEscota_out	bit = XDC_NON	output,
	@va_MatDang_out		bit = XDC_NON	output,
	@va_HomEscota_out	bit = XDC_NON	output,
	@va_Personalite_out	bit = XDC_NON	output,
	@va_DegatsDom_out	bit = XDC_NON	output,
	@va_Cond_Meteo_out	smallint = null	output,
	@va_Cond_Visibilite_out	smallint = null	output,
	@va_Cond_Etat_Chau_out	smallint = null	output,
	@va_Cond_Chantier_out	smallint = null	output
as
	declare @vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A Controle les arguments en entree obligatoire */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV


#ifdef HIST
/*B lecture des infos sur l'accident */
	select 
		@va_VL_out		= nombre_de_vl,
		@va_PL_out		= nombre_de_pl,
		@va_Remorque_out	= nombre_de_remorques,
		@va_Car_out		= nombre_de_car,
		@va_Moto_out		= nombre_de_moto,
		@va_BlesseLeg_out	= blesses_legers,
		@va_BlesseGra_out	= blesses_graves,
		@va_Mort_out		= morts,
		@va_VehEscota_out	= vehicule_ESCOTA,
		@va_MatDang_out		= matieres_dangereuses,
		@va_HomEscota_out	= personnel_ESCOTA,
		@va_Personalite_out	= personnalites,
	/*	@va_DegatsDom_out	= HIS..FMC_GEN.degats_domaine*/
		@va_Cond_Meteo_out	= isnull(cond_meteo,0),
		@va_Cond_Visibilite_out	= isnull(cond_visibilite,0),
		@va_Cond_Etat_Chau_out	= isnull(cond_etat_chaus,0),
		@va_Cond_Chantier_out	= isnull(cond_chantier,0)
	from HIS..FMC_ACC /*, HIS..FMC_GEN */
	where	HIS..FMC_ACC.numero = @va_NumEvt_in and
		HIS..FMC_ACC.cle = @va_CleEvt_in/* and
		HIS..FMC_GEN.numero = HIS..FMC_ACC.numero and
		HIS..FMC_GEN.cle = HIS..FMC_ACC.cle*/
	and HIS..FMC_ACC.zz > 1
	having validation = max(validation)

	/*B test si complement accident a ete trouve */
	if @@rowcount = 0
		return XDC_NOK

	/*B lecture des vehicules impliques ds l'accident */
	select 
		type_de_vehicule,
		nombre_passagers,
		bebe,
		personne_agee,
		animaux_vivants,
		vehicule_ESCOTA,
		volume_de_chargement,
		matieres_dangereuses,
		depannage,
		site_depannage,
		commentaire,
		numero_vehicule
	from HIS..FMC_VEH
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 


#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAE41
			@va_Horodate_in, @va_NumEvt_in, @va_CleEvt_in,
			@va_VL_out output, @va_PL_out output,
			@va_Remorque_out output, @va_Car_out output, @va_Moto_out output,
			@va_BlesseLeg_out output, @va_BlesseGra_out output, @va_Mort_out output,
			@va_VehEscota_out output, @va_MatDang_out output, @va_HomEscota_out output,
			@va_Personalite_out output, @va_DegatsDom_out output,
			@va_Cond_Meteo_out output, @va_Cond_Visibilite_out output,
			@va_Cond_Etat_Chau_out output, @va_Cond_Chantier_out output

 		if @vl_Status = null
                        return XDC_PRC_INC
                else return @vl_Status
	end
	else
	begin
		/*B lecture des infos sur l'accident */
		select 
			@va_VL_out		= nombre_de_vl,
			@va_PL_out		= nombre_de_pl,
			@va_Remorque_out	= nombre_de_remorques,
			@va_Car_out		= nombre_de_car,
			@va_Moto_out		= nombre_de_moto,
			@va_BlesseLeg_out	= blesses_legers,
			@va_BlesseGra_out	= blesses_graves,
			@va_Mort_out		= morts,
			@va_VehEscota_out	= vehicule_ESCOTA,
			@va_MatDang_out		= matieres_dangereuses,
			@va_HomEscota_out	= personnel_ESCOTA,
			@va_Personalite_out	= personnalites,
			/*@va_DegatsDom_out	= EXP..FMC_GEN.degats_domaine*/
			@va_Cond_Meteo_out	= isnull(cond_meteo,0),
			@va_Cond_Visibilite_out	= isnull(cond_visibilite,0),
			@va_Cond_Etat_Chau_out	= isnull(cond_etat_chaus,0),
			@va_Cond_Chantier_out	= isnull(cond_chantier,0)
		from EXP..FMC_ACC /*, EXP..FMC_GEN*/
		where	EXP..FMC_ACC.numero = @va_NumEvt_in and
			EXP..FMC_ACC.cle = @va_CleEvt_in /*and
			EXP..FMC_GEN.numero = EXP..FMC_ACC.numero and
			EXP..FMC_GEN.cle = EXP..FMC_ACC.cle*/
			and EXP..FMC_ACC.zz > 1
		having validation = max(validation)

		/*B test si complement accident a ete trouve */
		if @@rowcount = 0
			return XDC_NOK
	
		/*B lecture des vehicules impliques ds l'accident */
		select 
			type_de_vehicule,
			nombre_passagers,
			bebe,
			personne_agee,
			animaux_vivants,
			vehicule_ESCOTA,
			volume_de_chargement,
			matieres_dangereuses,
			depannage,
			site_depannage,
			commentaire,
			numero_vehicule
		from EXP..FMC_VEH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 
	end

#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des infos sur l'accident */
	select 
		@va_VL_out		= nombre_de_vl,
		@va_PL_out		= nombre_de_pl,
		@va_Remorque_out	= nombre_de_remorques,
		@va_Car_out		= nombre_de_car,
		@va_Moto_out		= nombre_de_moto,
		@va_BlesseLeg_out	= blesses_legers,
		@va_BlesseGra_out	= blesses_graves,
		@va_Mort_out		= morts,
		@va_VehEscota_out	= vehicule_ESCOTA,
		@va_MatDang_out		= matieres_dangereuses,
		@va_HomEscota_out	= personnel_ESCOTA,
		@va_Personalite_out	= personnalites,
		/* @va_DegatsDom_out	= EXP..FMC_GEN.degats_domaine*/
		@va_Cond_Meteo_out	= isnull(cond_meteo,0),
		@va_Cond_Visibilite_out	= isnull(cond_visibilite,0),
		@va_Cond_Etat_Chau_out	= isnull(cond_etat_chaus,0),
		@va_Cond_Chantier_out	= isnull(cond_chantier,0)
	from EXP..FMC_ACC/*, EXP..FMC_GEN*/
	where	EXP..FMC_ACC.numero = @va_NumEvt_in and 
		EXP..FMC_ACC.cle = @va_CleEvt_in /*and
		EXP..FMC_GEN.numero = EXP..FMC_ACC.numero and
		EXP..FMC_GEN.cle = EXP..FMC_ACC.cle*/
		and EXP..FMC_ACC.zz > 1
	having validation = max(validation)

	/*A test si complement accident a ete trouve */
	if @@rowcount = 0
		return XDC_NOK

	/*A lecture des vehicules impliques ds l'accident */
	select 
		type_de_vehicule,
		nombre_passagers,
		bebe,
		personne_agee,
		animaux_vivants,
		vehicule_ESCOTA,
		volume_de_chargement,
		matieres_dangereuses,
		depannage,
		site_depannage,
		commentaire,
		numero_vehicule
	from EXP..FMC_VEH
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 
#endif
#endif

	return XDC_OK
go
