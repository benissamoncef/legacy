/*E*/
/*  Fichier : $Id: xzae95sp.prc,v 1.4 2011/09/23 09:27:13 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2011/09/23 09:27:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae95sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  19/07/96: Creation
* P.N.  29/06/98: separation EXP/HIS
* JPL	10/02/09 : Degroupee ; retour de la colonne 'feu maitrise' (DEM845) 1.3
* LCL	18/07/11 : Ajout rappel client 1.4 DEM 1000
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante vehicule
* implique dans un accident
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE95_Lire_Liste_Vehicule
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_Type_out
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
* char(25)		va_marque_out
* char(25)		va_type_out
* char(25)		va_couleur_out
* char(25)		va_immatriculation_out
* XDY_Mot		va_code_pays_out
* char(25)		va_pays_out
* char(25)		va_nom_out
* char(25)		va_ville_out
* char(75)		va_adresse_out
* char(4)		va_code_danger_out
* XDY_Mot		va_code_nature_panne_out
* char(25)		va_nature_panne_out
* char(25)		va_positionnement_out
* XDY_Octet		va_vehicule_roulant_out
* char(25)		va_enlev_dem_par_out
* XDY_Entier		va_numero_fiche_appel_out
* XDY_Octet		va_pres_mat_dangereuse_out
* XDY_Octet		va_fax_envoye_out
* XDY_Mot		va_code_marque_out
* XDY_Mot		va_code_couleur_out
* XDY_Mot		va_code_enlev_demande_par_out
* XDY_Mot		va_num_veh_out
* XDY_Octet		va_sit_out
* XDY_Octet 		va_site_creation_out
* XDY_Mot		va_nb_bebes_out
* XDY_Mot		va_nb_agees_out
* XDY_Octet		va_feu_maitrise
* tinyint		va_type_rappel_out
* char(20)		va_complement_rappel_out
* datetime		va_horodate_rappel
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
* Suite a un ajout de colonnes dans ACT_DEP, les champs vehicule_roulant,
* presence_matiere_dangereuse et fax_envoye n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
* 
* Fonction
* Select dans la table VEHICULE
* where numero evenement and horodate
-------------------------------------------------------*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAE95' and type = 'P')
	drop procedure XZAE95
go


create procedure XZAE95
	@va_Horodate_in		datetime = null, -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null
as
	declare @vl_Status int
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/* test la presence des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV


	/*A lecture des informations sur complement vehicule     */
#ifdef HIST
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
			marque,
			type,
			couleur,
			immatriculation,
			code_pays,
			pays,
			nom,
			ville,
			adresse,
			code_danger,
			code_nature_panne,
			nature_panne,
			positionnement,
			vehicule_roulant,
			enlevement_demande_par,
			numero_fiche_appel,
			presence_matiere_dangereuse,
			fax_envoye,
			code_marque,
			code_couleur,
			code_enlev_demande_par,
			numero_vehicule,
			sit,
			site_creation,
			nb_bebes,
			nb_agees,
			feu_maitrise,
			type_rappel,
			complement_rappel,
			horodate_rappel
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
			exec @vl_Status = SQL_HIS.PRC..XZAE95
				@va_Horodate_in	,
				@va_NumEvt_in	,
				@va_CleEvt_in	

			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
	
	end
	else
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
			marque,
			type,
			couleur,
			immatriculation,
			code_pays,
			pays,
			nom,
			ville,
			adresse,
			code_danger,
			code_nature_panne,
			nature_panne,
			positionnement,
			vehicule_roulant,
			enlevement_demande_par,
			numero_fiche_appel,
			presence_matiere_dangereuse,
			fax_envoye,
			code_marque,
			code_couleur,
			code_enlev_demande_par,
			numero_vehicule,
			sit,
			site_creation,
			nb_bebes,
			nb_agees,
			feu_maitrise,
			type_rappel,
			complement_rappel,horodate_rappel
		from EXP..FMC_VEH
		where	numero = @va_NumEvt_in and cle = @va_CleEvt_in 
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

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
		marque,
		type,
		couleur,
		immatriculation,
		code_pays,
		pays,
		nom,
		ville,
		adresse,
		code_danger,
		code_nature_panne,
		nature_panne,
		positionnement,
		vehicule_roulant,
		enlevement_demande_par,
		numero_fiche_appel,
		presence_matiere_dangereuse,
		fax_envoye,
		code_marque,
		code_couleur,
		code_enlev_demande_par,
		numero_vehicule,
		sit,
		site_creation,
		nb_bebes,
		nb_agees,
		feu_maitrise,
		type_rappel,
		complement_rappel,
		horodate_rappel
	from EXP..FMC_VEH
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#endif
#endif

	return XDC_OK
go
