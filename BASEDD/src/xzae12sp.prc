/*X  Fichier : $Id: xzae12sp.prc,v 1.11 2019/03/20 15:28:49 devgfi Exp $        $Revision: 1.11 $        $Date: 2019/03/20 15:28:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae12sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* < description rapide de la procedure
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/12/97 : Creation
* C.T.	08/01/98 : ajout sit_alerte (1532) V1.2
* C.T.	05/02/98 : Correction ajout des commentaires (1531) V1.3
* C.T.	05/02/98 : Conserver les valeurs de la fmc initiales 
*                         si valeurs de la fmc degrade nulles  (1586) V1.4
* C.T.	14/04/98 : Envoi reveils sue les districts traitant la fmc initale V1.6 (1635)
* PN	29/06/98 : Separation EXP/HIS 1.7
* CHI   20/07/05 : Ajout cas nature obstacle table FMC_NAT DEM487 v1.8
* JPl	08/06/07 : Cas Basculement pour travaux : idem basculement + travaux (DEM 647) 1.9
* JMG   18/11/08 : ajout validation dans FMC_ACC 1.10 DEM/852
* JPL	14/03/19 : Classe Manifestation : idem autres classes historis�es (DEM 1327)  1.11
* ABE   19/01/22    : Ajout champ id_ava et dangerosité pour AVA DEM-SAE367
* ABK	26/09/23 : Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transferer les informations d'une fmc de type degrade
* sur la fmc initiale et cloturer a la date 1900 pour qu'elle
* soit supprimee lors de la purge journaliere
*
* Sequence d'appel
* XZAE12_Transferer_Info_FMC_Degradee
*
* Arguments en entree
* XDY_Entier    va_NumEvt_in
* XDY_Octet     va_CleEvt_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme de transfert d'info
*
* Conditions d'utilisation
* a utiliser uniquement au CI sur fmc de type degrade non operateur
* la table FMC_TRS existe dans les bases EXP des PC niveau 1, 2 et au CI
*
* Fonction
-------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAE12' and type = 'P')
	drop procedure XZAE12
go

create procedure XZAE12
	@va_NumEvt_in           int = null,
	@va_CleEvt_in           tinyint = null
as		
	declare @vl_NumFMCInit int, @vl_CleFMCInit tinyint,
		@vl_HorodateLien datetime, @vl_HorodateType datetime,
		@vl_Alerte int, @vl_SiteAlerte T_SITE, @vl_Type smallint, @vl_DebutPrevu datetime,
		@vl_Debut datetime, @vl_FinPrevue datetime, @vl_Fin datetime,
		@vl_DebutInit datetime, @vl_DebutPrevuInit datetime,
		@vl_Origine T_NOM, @vl_Degats tinyint, @vl_FausseAlerte tinyint,
		@vl_DegradeCause tinyint, @vl_CauseNumFMCInit int, @vl_CauseCleFMCInit tinyint,
		@vl_NumCause int, @vl_CleCause tinyint, @vl_Status int, @vl_ClasseFmc tinyint,
		@vl_nb int, @vl_Poste smallint,@vl_SiteCI T_SITE

	/* initialisation variable locale */
	select @vl_SiteCI = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/* rechercher le type de fmc */
	select
		@vl_Type = EXP..FMC_GEN.type,
		@vl_ClasseFmc = CFG..TYP_FMC.classe,
		@vl_NumFMCInit = num_fmc_init,
		@vl_CleFMCInit = cle_fmc_init,
		@vl_NumCause = cause,
		@vl_CleCause = cle_cause,
		@vl_HorodateLien = horodate_lien_cause,
		@vl_HorodateType = horodate_chgt_type,
		@vl_Alerte = alerte ,
		@vl_SiteAlerte = sit_alerte ,
		@vl_Type = type,
		@vl_DebutPrevu = debut_prevu,
		@vl_Debut = debut,
		@vl_FinPrevue = fin_prevue,
		@vl_Fin = fin,
		@vl_Origine = origine_creation,
		@vl_Degats = degats_domaine,
		@vl_FausseAlerte = fausse_alerte
	from  EXP..FMC_GEN, CFG..TYP_FMC
	where EXP..FMC_GEN.numero = @va_NumEvt_in and cle = @va_CleEvt_in and 
		degrade = XDC_OUI and CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	if @@rowcount = 0
		return XDC_NOK

	/* rechercher les dates de debut de la fmc initiale */
	select
		@vl_DebutPrevuInit = debut_prevu,
		@vl_DebutInit = debut
	from  EXP..FMC_GEN
	where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit

	if @@rowcount = 0
		return XDC_NOK

	/*A les dates de debut ... les plus anterieures sont conserv�es pour la fmc init */
	if @vl_DebutPrevuInit is not null and @vl_DebutPrevuInit < @vl_DebutPrevu
		select @vl_DebutPrevu = @vl_DebutPrevuInit
	if @vl_DebutInit is not null and @vl_DebutInit < @vl_Debut
		select @vl_Debut = @vl_DebutInit

	if @vl_Type = XZAEC_FMC_QueueBouchon
	begin
		/* transferer la tete de bouchon */
		exec @vl_Status = XZAE12 @vl_NumCause, @vl_CleCause

		if @vl_Status != XDC_OK
			return  XDC_NOK
	end
	else if @vl_Type = XZAEC_FMC_TeteBouchon
		/* rechercher la tete du bouchon init */
		select @vl_NumFMCInit = cause, @vl_CleFMCInit = cle_cause
		from  EXP..FMC_GEN
		where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit


	/* transferer la partie specifique de la fmc */
        if @vl_ClasseFmc = XZAEC_ClasseAccident
	begin
		select * into #FMC_COMP
		from EXP..FMC_ACC
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in
		having validation=max(validation)

		update #FMC_COMP set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_ACC  set 
			nombre_de_vl = C.nombre_de_vl,
			nombre_de_pl = C.nombre_de_pl,
			nombre_de_remorques = C.nombre_de_remorques,
			nombre_de_car = C.nombre_de_car,
			nombre_de_moto = C.nombre_de_moto,
			vehicule_ESCOTA = C.vehicule_ESCOTA,
			matieres_dangereuses = C.matieres_dangereuses,
			blesses_legers = C.blesses_legers,
			blesses_graves = C.blesses_graves,
			morts = C.morts,
			personnel_ESCOTA = C.personnel_ESCOTA,
			personnalites = C.personnalites,
			degats_domaine = C.degats_domaine,
			zz = A.zz + 1,
			validation = C.validation
		from #FMC_COMP C, EXP..FMC_ACC A
		where A.numero = C.numero and A.cle = C.cle

		/* recuperer les vehicules */
		if exists ( select * from  EXP..FMC_VEH where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		begin
			/* rechercher les vehicules impliques dans l'accident */
			select * into #FMC_COMPB
			from EXP..FMC_VEH
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in

			update #FMC_COMPB set zz = zz + 1 , numero = @vl_NumFMCInit, cle = @vl_CleFMCInit
			update #FMC_COMPB set matieres_dangereuses = null where matieres_dangereuses = XDC_CHAINE_VIDE

			/* modifier les existants */
			update EXP..FMC_VEH  set 
				numero_vehicule = C.numero_vehicule ,
				depannage = C.depannage ,
				site_depannage = C.site_depannage ,
				type_de_vehicule = C.type_de_vehicule ,
				nombre_passagers = C.nombre_passagers ,
				bebe = C.bebe ,
				personne_agee = C.personne_agee ,
				animaux_vivants = C.animaux_vivants ,
				vehicule_ESCOTA = C.vehicule_ESCOTA ,
				volume_de_chargement = C.volume_de_chargement ,
				matieres_dangereuses = isnull(C.matieres_dangereuses, A.matieres_dangereuses) ,
				commentaire = C.commentaire ,
				marque = C.marque ,
				code_marque = C.code_marque ,
				type = C.type ,
				couleur = C.couleur ,
				code_couleur = C.code_couleur ,
				immatriculation = C.immatriculation ,
				pays = C.pays ,
				code_pays = C.code_pays,
				nom = C.nom ,
				ville = C.ville ,
				adresse= C.adresse,
				presence_matiere_dangereuse = C.presence_matiere_dangereuse ,
				code_danger = C.code_danger ,
				nature_panne = C.nature_panne ,
				code_nature_panne = C.code_nature_panne ,
				positionnement = C.positionnement ,
				vehicule_roulant = C.vehicule_roulant ,
				enlevement_demande_par = C.enlevement_demande_par ,
				code_enlev_demande_par = C.code_enlev_demande_par ,
				numero_fiche_appel = C.numero_fiche_appel ,
				fax_envoye = C.fax_envoye ,
				site_creation = C.site_creation ,
				nb_bebes = C.nb_bebes ,
				nb_agees = C.nb_agees ,
				sit = C.sit,
				id_ava = C.id_ava, 
				dangerosite = C.dangerosite,
				zz = A.zz + 1
			from #FMC_COMPB C, EXP..FMC_VEH A
			where A.numero = C.numero and A.cle = C.cle and A.numero_vehicule = C.numero_vehicule
				
			/* ajouter les vehicules supplementaires de l'accident */
			if exists ( select * from #FMC_COMPB
					where numero_vehicule not in ( select numero_vehicule from EXP..FMC_VEH
							where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit))
				insert EXP..FMC_VEH
				select * from #FMC_COMPB
				where numero_vehicule not in ( select numero_vehicule from EXP..FMC_VEH
							where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)

			/* supprimer si necessaire */
			delete EXP..FMC_VEH
			where  numero = @vl_NumFMCInit and cle = @vl_CleFMCInit and numero_vehicule not in ( select numero_vehicule from #FMC_COMPB)
/* separation EXP/HIS
			delete HIS..FMC_VEH
			where  numero = @vl_NumFMCInit and cle = @vl_CleFMCInit and numero_vehicule not in ( select numero_vehicule from #FMC_COMPB)
			*/
		end
	end
	else if @vl_ClasseFmc = XZAEC_ClasseDelestage
	begin
		select * into #FMC_COMP2
		from EXP..FMC_DEL
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP2 set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_DEL  set 
			plan_associe = C.plan_associe,
			responsable = C.responsable,
			sit = C.sit,
			zz = A.zz + 1
		from #FMC_COMP2 C, EXP..FMC_DEL A
		where A.numero = C.numero and A.cle = C.cle
	end
	else if @vl_ClasseFmc = XZAEC_ClasseVehicule
	begin
		select * into #FMC_COMP3
		from EXP..FMC_VEH
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP3 set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_VEH  set 
			numero_vehicule = C.numero_vehicule ,
			depannage = C.depannage ,
			site_depannage = C.site_depannage ,
			type_de_vehicule = C.type_de_vehicule ,
			nombre_passagers = C.nombre_passagers ,
			bebe = C.bebe ,
			personne_agee = C.personne_agee ,
			animaux_vivants = C.animaux_vivants ,
			vehicule_ESCOTA = C.vehicule_ESCOTA ,
			volume_de_chargement = C.volume_de_chargement ,
			matieres_dangereuses = C.matieres_dangereuses ,
			commentaire = C.commentaire ,
			marque = C.marque ,
			code_marque = C.code_marque ,
			type = C.type ,
			couleur = C.couleur ,
			code_couleur = C.code_couleur ,
			immatriculation = C.immatriculation ,
			pays = C.pays ,
			code_pays = C.code_pays,
			nom = C.nom ,
			ville = C.ville ,
			adresse= C.adresse,
			presence_matiere_dangereuse = C.presence_matiere_dangereuse ,
			code_danger = C.code_danger ,
			nature_panne = C.nature_panne ,
			code_nature_panne = C.code_nature_panne ,
			positionnement = C.positionnement ,
			vehicule_roulant = C.vehicule_roulant ,
			enlevement_demande_par = C.enlevement_demande_par ,
			code_enlev_demande_par = C.code_enlev_demande_par ,
			numero_fiche_appel = C.numero_fiche_appel ,
			fax_envoye = C.fax_envoye ,
			site_creation = C.site_creation ,
			nb_bebes = C.nb_bebes ,
			nb_agees = C.nb_agees ,
			sit = C.sit,
			id_ava = C.id_ava, 
			dangerosite = C.dangerosite,
			zz = A.zz + 1
		from #FMC_COMP3 C, EXP..FMC_VEH A
		where A.numero = C.numero and A.cle = C.cle
	end
	else if @vl_ClasseFmc = XZAEC_ClasseVehLent
	begin
		select * into #FMC_COMPH
		from EXP..FMC_VLN
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMPH set sit = @vl_SiteCI , numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		insert EXP..FMC_VLN
		select * from #FMC_COMPH
		where horodate not in (select  horodate from EXP..FMC_VLN
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)
	end
	else if @vl_ClasseFmc = XZAEC_ClasseMeteo
	begin
		select * into #FMC_COMPH2
		from EXP..FMC_MET
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMPH2 set sit = @vl_SiteCI , numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		insert EXP..FMC_MET
		select * from #FMC_COMPH2
		where horodate not in (select  horodate from EXP..FMC_MET
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)
	end
	else if @vl_ClasseFmc = XZAEC_ClasseNature
	begin
		select * into #FMC_COMP17
		from EXP..FMC_NAT
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP17 set sit = @vl_SiteCI , numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		insert EXP..FMC_NAT
		select * from #FMC_COMP17
		where horodate not in (select  horodate from EXP..FMC_NAT
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)
	end
	else if @vl_ClasseFmc = XZAEC_ClasseEchangeur
	begin
		select * into #FMC_COMP4
		from EXP..FMC_ECH
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP4 set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_ECH  set 
			entree1 = C.entree1,
			entree2 = C.entree2,
			sortie1 = C.sortie1,
			sortie2 = C.sortie2,
			sit = C.sit,
			zz = A.zz + 1
		from #FMC_COMP4 C, EXP..FMC_ECH A
		where A.numero = C.numero and A.cle = C.cle
	end
	else if @vl_ClasseFmc = XZAEC_ClasseSousConcess
	begin
		select * into #FMC_COMP5
		from EXP..FMC_CNC
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP5 set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_CNC  set 
			nom_ss_concessionnaire = C.nom_ss_concessionnaire,
			ferme = C.ferme,
			penurie = C.penurie,
			carburant = C.carburant ,
			sit = C.sit,
			zz = A.zz + 1
		from #FMC_COMP5 C, EXP..FMC_CNC A
		where A.numero = C.numero and A.cle = C.cle
	end
	else if @vl_ClasseFmc = XZAEC_ClasseManif
	begin
		select * into #FMC_COMP6
		from EXP..FMC_MNF
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP6 set sit = @vl_SiteCI , numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		insert EXP..FMC_MNF
		select * from #FMC_COMP6
		where horodate not in (select  horodate from EXP..FMC_MNF
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)
	end
	else if @vl_ClasseFmc = XZAEC_ClasseBouchon
	begin
		select * into #FMC_COMPH3
		from EXP..FMC_BOU
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMPH3 set sit = @vl_SiteCI , numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		insert EXP..FMC_BOU
		select * from #FMC_COMPH3
		where horodate not in (select  horodate from EXP..FMC_BOU
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)
	end

	if (@vl_ClasseFmc = XZAEC_ClasseTravaux or
	    @vl_ClasseFmc = XZAEC_ClasseBasculTrav)
	begin
		select * into #FMC_COMP7
		from EXP..FMC_TRF
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP7 set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_TRF  set 
		    nature                          = C.nature,
		    type                            = isnull(C.type, A.type),
		    continu                         = C.continu,
		    horodate_debut_planifiee        = C.horodate_debut_planifiee,
		    horodate_fin_planifiee          = C.horodate_fin_planifiee,
		    mobile                          = C.mobile,
		    autoroute_planifiee             = C.autoroute_planifiee,
		    PR_queue_planifie               = C.PR_queue_planifie,
		    PR_tete_planifie                = C.PR_tete_planifie,
		    sens_planifie                   = C.sens_planifie,
		    VR                              = C.VR,
		    VM2                             = C.VM2,
		    VM1                             = C.VM1,
		    VL                              = C.VL,
		    BAU                             = C.BAU,
		    VR_I                            = C.VR_I,
		    VM2_I                           = C.VM2_I,
		    VM1_I                           = C.VM1_I,
		    VL_I                            = C.VL_I,
		    BAU_I                           = C.BAU_I,
		    PR_debut_VR                     = C.PR_debut_VR,
		    PR_fin_VR                       = C.PR_fin_VR,
		    debut_VR                        = C.debut_VR,
		    fin_VR                          = C.fin_VR,
		    PR_debut_VM2                    = C.PR_debut_VM2,
		    PR_fin_VM2                      = C.PR_fin_VM2,
		    debut_VM2                       = C.debut_VM2,
		    fin_VM2                         = C.fin_VM2,
		    PR_debut_VM1                    = C.PR_debut_VM1,
		    PR_fin_VM1                      = C.PR_fin_VM1,
		    debut_VM1                       = C.debut_VM1,
		    fin_VM1                         = C.fin_VM1,
		    PR_debut_VL                     = C.PR_debut_VL,
		    PR_fin_VL                       = C.PR_fin_VL,
		    debut_VL                        = C.debut_VL,
		    fin_VL                          = C.fin_VL,
		    PR_debut_BAU                    = C.PR_debut_BAU,
		    PR_fin_BAU                      = C.PR_fin_BAU,
		    debut_BAU                       = C.debut_BAU,
		    fin_BAU                         = C.fin_BAU,
		    PR_debut_VR_I                   = C.PR_debut_VR_I,
		    PR_fin_VR_I                     = C.PR_fin_VR_I,
		    debut_VR_I                      = C.debut_VR_I,
		    fin_VR_I                        = C.fin_VR_I,
		    PR_debut_VM2_I                  = C.PR_debut_VM2_I,
		    PR_fin_VM2_I                    = C.PR_fin_VM2_I,
		    debut_VM2_I                     = C.debut_VM2_I,
		    fin_VM2_I                       = C.fin_VM2_I,
		    PR_debut_VM1_I                  = C.PR_debut_VM1_I,
		    PR_fin_VM1_I                    = C.PR_fin_VM1_I,
		    debut_VM1_I                     = C.debut_VM1_I,
		    fin_VM1_I                       = C.fin_VM1_I,
		    PR_debut_VL_I                   = C.PR_debut_VL_I,
		    PR_fin_VL_I                     = C.PR_fin_VL_I,
		    debut_VL_I                      = C.debut_VL_I,
		    fin_VL_I                        = C.fin_VL_I,
		    PR_debut_BAU_I                  = C.PR_debut_BAU_I,
		    PR_fin_BAU_I                    = C.PR_fin_BAU_I,
		    debut_BAU_I                     = C.debut_BAU_I,
		    fin_BAU_I                       = C.fin_BAU_I,
		    sit 			    = C.sit,
		    zz 				    = A.zz + 1
		from #FMC_COMP7 C, EXP..FMC_TRF A
		where A.numero = C.numero and A.cle = C.cle

		select * into #FMC_COMPH4
		from EXP..FMC_TRH
		where  numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMPH4 set sit = @vl_SiteCI, numero = @vl_NumFMCInit, cle = @vl_CleFMCInit 

		insert EXP..FMC_TRH
		select * from #FMC_COMPH4
		where horodate not in ( select  horodate from EXP..FMC_TRH
					where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)
	end

	if (@vl_ClasseFmc = XZAEC_ClasseBasculement or
	    @vl_ClasseFmc = XZAEC_ClasseBasculTrav)
	begin
		select * into #FMC_COMP8
		from EXP..FMC_BAS
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in

		update #FMC_COMP8 set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit

		update EXP..FMC_BAS  set 
			localisation_PR_fin = C.localisation_PR_fin,
			VR = C.VR,
			VL = C.VL,
			VM1 = C.VM1,
			VM2 = C.VM2,
			BAU = C.BAU,
			VR_I = C.VR_I,
			VL_I = C.VL_I,
			VM1_I = C.VM1_I,
			VM2_I = C.VM2_I,
			BAU_I = C.BAU_I,
			sit = C.sit,
			zz = A.zz + 1
		from #FMC_COMP8 C, EXP..FMC_BAS A
		where A.numero = C.numero and A.cle = C.cle

	end


	/* si la cause de la fmc est de type de degrade liee a une fmc init alors positionner la cause a la fmc init */
	if @vl_Type <> XZAEC_FMC_QueueBouchon
	begin
		select 
			@vl_DegradeCause = degrade, 
			@vl_CauseNumFMCInit = num_fmc_init, 
			@vl_CauseCleFMCInit = cle_fmc_init
		from EXP..FMC_GEN
		where numero = @vl_NumCause and cle = @vl_CleCause

		if @vl_DegradeCause = XDC_OUI and @vl_CauseNumFMCInit is not null
			select @vl_NumCause = @vl_CauseNumFMCInit, @vl_CleCause = @vl_CauseCleFMCInit
	end
	else
	begin
		/* la cause est la tete de la fmc init */
		select @vl_NumCause = cause, @vl_CleCause = cle_cause
		from EXP..FMC_GEN
		where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit
	end

	if @vl_Origine = XDC_CHAINE_VIDE
		select @vl_Origine = null

	/* transferer la partie generique de la fmc */
	update EXP..FMC_GEN set cause = isnull(@vl_NumCause, cause),
		cle_cause = isnull(@vl_CleCause, cle_cause),
		horodate_lien_cause = @vl_HorodateLien,
		horodate_chgt_type = @vl_HorodateType,
		alerte = isnull(@vl_Alerte, alerte),
		sit_alerte = isnull(@vl_SiteAlerte, sit_alerte),
		type = @vl_Type,
		debut_prevu = @vl_DebutPrevu,
		debut = @vl_Debut,
		fin_prevue = @vl_FinPrevue,
		fin = @vl_Fin,
		origine_creation = isnull(@vl_Origine, origine_creation),
		degats_domaine = @vl_Degats,
		fausse_alerte = @vl_FausseAlerte,
		zz = zz + 1
	where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit

	/* transferer la partie validations de la fmc */
	select *
	into #VALID_FMC
	from EXP..FMC_HIS
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	update #VALID_FMC set numero = @vl_NumFMCInit, cle = @vl_CleFMCInit, sit = @vl_SiteCI

	if exists( select * from #VALID_FMC
			where horodate_validation not in (	select horodate_validation from EXP..FMC_HIS
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit))
	insert EXP..FMC_HIS
	select * from #VALID_FMC
	where horodate_validation not in (	select horodate_validation from EXP..FMC_HIS
						where numero = @vl_NumFMCInit and cle = @vl_CleFMCInit)

	/* commentaires inseres */
	select  *, indice = identity(1)
	into #FMC_COM
	from EXP..FMC_COM
	where evenement = @va_NumEvt_in and cle = @va_CleEvt_in
	order by sit, horodate_validation, numero

	/* nombre de commentaire de la fmc init crees au CI */
	select @vl_nb = count(*) from EXP..FMC_COM
	where evenement = @vl_NumFMCInit and cle = @vl_CleFMCInit and sit = @vl_SiteCI
	/* modifier la num�rotation des comm. de la fmc degradee pour eviter l'insertion de cles en double */
	update #FMC_COM set numero = indice + @vl_nb, evenement = @vl_NumFMCInit, cle = @vl_CleFMCInit, sit = @vl_SiteCI

	insert EXP..FMC_COM (numero, evenement, cle, horodate_validation, horodate_fin, texte, sit)
	select 
		numero, evenement, cle, horodate_validation, horodate_fin, texte, sit 
	from #FMC_COM

	/* transferer les actions de la fmc des districts */
	/* ! attention pas de replication possible */
	if exists ( select sit from EXP..ACT_GEN where evenement = @va_NumEvt_in and cle = @va_CleEvt_in and sit != @vl_SiteCI)
	begin
		/* passer par nouvelle table FMC_TRS */
		insert EXP..FMC_TRS ( numero, cle, num_fmc_init, cle_fmc_init, horodate)
		values(@va_NumEvt_in, @va_CleEvt_in,@vl_NumFMCInit, @vl_CleFMCInit, getdate())
	end
	/* transferer les actions au CI */
	update EXP..ACT_GEN set evenement = @vl_NumFMCInit, cle = @vl_CleFMCInit
	where evenement = @va_NumEvt_in and cle = @va_CleEvt_in
/* separation EXP/HIS
	update HIS..ACT_GEN set evenement = @vl_NumFMCInit, cle = @vl_CleFMCInit
	where evenement = @va_NumEvt_in and cle = @va_CleEvt_in and sit != XDC_CI
	*/

	/* positionner la date de cloture a 1900 */
	if @vl_Type != XZAEC_FMC_TeteBouchon
	begin
		/* modifier les fmc ayant cette fmc pour cause par celle de la fmc initiale sauf si c'est une queue de bouchon */
		update EXP..FMC_GEN set cause = @vl_NumFMCInit , cle_cause = @vl_CleFMCInit, zz=zz+1 
		where cause = @va_NumEvt_in  and cle_cause = @va_CleEvt_in and type != XZAEC_FMC_QueueBouchon

		if @vl_Type != XZAEC_FMC_QueueBouchon	
			update EXP..FMC_GEN set cloture = XDC_DATE_CLOTURE_PURGE, alerte=null, sit_alerte=null, zz = zz + 1
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in
		else
			update EXP..FMC_GEN set cloture = XDC_DATE_CLOTURE_PURGE, alerte=null, sit_alerte=null, zz = zz + 1
			where (numero = @va_NumEvt_in or 
				(numero in (select cause from EXP..FMC_GEN where numero = @va_NumEvt_in  and cle = @va_CleEvt_in))) and 
				cle = @va_CleEvt_in
	end

	/* creation des curseurs */
	declare Pointeur_poste cursor
	for select distinct poste_enrichisseur
	from EXP..FMC_TRT, EXP..FMC_GEN
	where evenement = @vl_NumFMCInit and EXP..FMC_TRT.cle = @vl_CleFMCInit and 
		EXP..FMC_TRT.sit != @vl_SiteCI and EXP..FMC_GEN.type = XZAEC_FMC_PosteOperateur and
		EXP..FMC_GEN.sit = EXP..FMC_TRT.sit and EXP..FMC_GEN.fin is null and 
		EXP..FMC_GEN.poste_enrichisseur is not null

	/*B declencher les reveils sur les sites traitant la fmc initiale */
	open Pointeur_poste
	fetch Pointeur_poste into @vl_Poste
	while (@@sqlstatus = 0)
	begin
		/* declencher le reveil */
select @vl_Poste
		exec @vl_Status = XZAR;03 @vl_NumFMCInit, @vl_CleFMCInit,
					@vl_Poste, XDC_REV_FMC_MOD
		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return @vl_Status

		/* machine suivante a prevenir */
		fetch Pointeur_poste into @vl_Poste
	end
	close Pointeur_poste

	/* rafraichir en local la liste a traiter */
	/* utile pour le changement de type pour le poste voisin */
	exec @vl_Status = XZAR;03 0,0, null,"",@vl_SiteCI
	if @vl_Status = null
		return XDC_PRC_INC
	else if @vl_Status != XDC_OK
		return @vl_Status

	return XDC_OK
go
