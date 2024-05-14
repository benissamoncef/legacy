/*E*/
/*  Fichier : $Id: xzag12sp.prc,v 1.22 2021/05/04 13:29:13 pc2dpdy Exp $      Release : $Revision: 1.22 $        Date : $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	02/11/94	: Creation	(V 1.1)
* C.T.	16/11/94	: Correction erreur sur complement action
*                         (rajout le sit dans la clause) (V 1.2)
*                         non teste
* C.T.	11/01/95	: Aucune modif (V 1.3)
* C.T.	08/06/95	: correction controle historique pour FMC 
*                         operateur(V 1.5)
* C.T.	15/06/95	: Ajout des alertes liees a un evt (V 1.6)
* C.T.	12/09/95	: Ajout controle sur cause (V 1.8)
* C.T.	02/02/98	: Ajouter la purge sur les actions PAL, FAC, FAU (1538) (V1.11)
* C.T.	16/03/98	: Ajout detail des colonnes pour insert 
*                         (SVG et HIS n'ont pas les colonnes dans le meme ordre) (V1.12)
* P.N.	27/01/99	: Ajouter les actions sur ACT_EXP ACT_EXPTFM ACT_PMVDAT  et FMC_DAT dem/1724 (V1.13)
* P.C.	06/03/02	: Correction pour HIS V1.14
* JPL	11/04/07	: Prise en compte table FMC_NAT pour la classe Nature (DEM 639) V1.15
* JPL	11/06/07	: Prise en compte de la classe Basculement pour Travaux (DEM 647) V1.16
* JPL	27/02/09	: Prise en compte du champ 'Feu maitrise' dans la table des vehicules (DEM 845) 1.17
* VR	14/12/11 	: Ajout PRV (DEM/1016) 1.18
* JPL	13/09/12	: Prise en compte table FMC_INC pour la classe Incendie (DEM 1037)  1.19
* LCL	02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.20 DEM1284
* ABE	17/12/20	: Ajout IMU DEM-SAE155 1.21
* ABE	07/04/21	: Ajout PAU SONO DEM-SAE244 1.21
* ABE   19/01/22    : Ajout champ id_ava et dangerosité pour AVA DEM-SAE367
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge les donnees situees la base historique locales relatives 
* a un evenement
* 
* Sequence d'appel
* SP	XZAG12_Sauvegarde_Evenement_historise
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in	: numero d'evenement
* XDY_Octet		va_CleEvt_in	: cle d'evenement
* XDY_Octet		va_ClasseEvt_in	: classe de l'evenement
* XDY_Entier		va_NumCause_in	: numero de la cause d'evenement
* XDY_Octet		va_CleCause_in	: cle de la cause d'evenement
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: probleme a l'insertion dans la base de 
*                 sauvegarde/restauration
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* procedure appelee en interne (par XZAG;01)
* Cette procedure n'existe qu'au CI
* 
* Fonction
* Sauvegarder les actions, l'alerte relatif a l'evenement,
* l'evenement et sa cause
-------------------------------------------------------*/

use PRC
go

#ifdef HIST

create procedure XZAG;12
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_ClasseEvt_in	tinyint = null,
	@va_NumCause_in		int = null,
	@va_CleCause_in		tinyint = null
as
	declare @vl_Action int, @vl_NumEvt int,
		@vl_CleEvt tinyint, @vl_TypeAction tinyint,
		@vl_ClasseCause tinyint, @vl_Site T_SITE, @vl_Erreur bit

	if @va_NumCause_in = 0 or @va_CleCause_in = 0
		select @va_NumCause_in = null , @va_CleCause_in = null

	/*A controle des parametres en entree */
	if @va_ClasseEvt_in = null or @va_NumEvt_in = null or 
	   @va_CleEvt_in = null or (@va_NumCause_in  != null and
	   @va_CleCause_in = null)
		return XDC_ARG_INV

	/* intialisation de la variable d'erreur */
	select @vl_Erreur = XDC_NON

	/*A rechercher la liste des actions liees a l'evenement */
	select
		numero,
		type,
		sit
	into #ACT_SVG
	from HIS..ACT_GEN 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	declare Pointeur_actions cursor
	for select numero, type, sit
	from #ACT_SVG

	/*A Sauvegarder toutes les actions liees a l'evenement */
	open Pointeur_actions
	fetch Pointeur_actions into @vl_Action, @vl_TypeAction, @vl_Site
	while @@sqlstatus = 0 and @vl_Erreur = XDC_NON
	begin
		/* supprimer le complement de l'action selectionnee */
		if @vl_TypeAction = XDC_ACT_APPEL or 
		   @vl_TypeAction = XDC_ACT_FAX or
		   @vl_TypeAction = XDC_ACT_BIP 
		begin
			/* sauvegarder le texte du fax */
			if @vl_TypeAction = XDC_ACT_FAX
				insert SVG..ACT_FAX
				select * from HIS..ACT_FAX
				where actio = @vl_Action and sit = @vl_Site

			/* sauvegarder le complement du fax */
			/* attention ne pas changer l'ordre des insertions */
			insert SVG..ACT_APL
			select * from HIS..ACT_APL 
			where	actio = @vl_Action  and sit = @vl_Site
		end
		else if @vl_TypeAction = XDC_ACT_NAV or @vl_TypeAction = XDC_ACT_FAU or @vl_TypeAction = XDC_ACT_FAC
			insert SVG..ACT_NAV 
			select * from HIS..ACT_NAV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_ECHANGEUR
			insert SVG..ACT_ECH
			select * from HIS..ACT_ECH 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_TUNNEL
			insert SVG..ACT_TUB
			select * from HIS..ACT_TUB 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_DEPANNEUR
			insert SVG..ACT_DEP
			select * from HIS..ACT_DEP 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_AFF_GARE
			insert SVG..ACT_GAR
			select * from HIS..ACT_GAR 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PAT_SIS
			insert SVG..ACT_SIS
			select * from HIS..ACT_SIS 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PAL
			insert SVG..ACT_PAL
			select * from HIS..ACT_PAL 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_EXP
			insert SVG..ACT_EXP_ADA
			select * from HIS..ACT_EXP_ADA 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_EXPTFM
			insert SVG..ACT_TFM_EXP
			select * from HIS..ACT_TFM_EXP 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMVDAT
			insert SVG..ACT_PMV_ADA
			select * from HIS..ACT_PMV_ADA 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV
			insert SVG..ACT_PMV
			select * from HIS..ACT_PMV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PRV
			insert SVG..ACT_PRV
			select * from HIS..ACT_PRV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PIC
			insert SVG..ACT_PIC
			select * from HIS..ACT_PIC 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_CFE
			insert SVG..ACT_CFE
			select * from HIS..ACT_CFE 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_IMU 
			insert SVG..ACT_IMU 
			select * from HIS..ACT_IMU 
			where actio = @vl_Action and sit = @vl_Site 
		else if @vl_TypeAction = XDC_ACT_PAU
			insert SVG..ACT_PAU
			select * from HIS..ACT_PAU
			where actio = @vl_Action and sit = @vl_Site 		
		else if @vl_TypeAction = XDC_ACT_PMV_REMORQUE
			insert SVG..ACT_RMQ
			select * from HIS..ACT_RMQ 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV_FOURGON
			insert SVG..ACT_FRG
			select * from HIS..ACT_FRG 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_INTERVENTION
			insert SVG..ACT_INT
			select * from HIS..ACT_INT 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_TFM
		begin
			/* Sauvegarder le texte de TFM */
			insert SVG..ACT_TXT_TFM
			select * from HIS..ACT_TXT_TFM
			where actio = @vl_Action and sit = @vl_Site

			/* Sauvegarder le complement de TFM */
			/* attention ne pas changer l'ordre des insertions */
			insert SVG..ACT_TFM
			select * from HIS..ACT_TFM 
			where actio = @vl_Action and sit = @vl_Site
		end

		if @@rowcount != 1
		begin
			select @vl_Erreur = XDC_OUI
		end
	
		/* passage a l'action suivante selectionnee */
		else	fetch Pointeur_actions into @vl_Action, @vl_TypeAction, @vl_Site
	end
	/* fermer le curseur */
	close Pointeur_actions

	/* Tester si un probleme d'insertion a eu lieu sinon continuer */
	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	/* Sauvegarder de la partie generique de toutes les actions */
	insert SVG..ACT_GEN
	select * from HIS..ACT_GEN 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	/*A Sauvegarder le complement de l'evenement et le complement */
	if @va_ClasseEvt_in = XZAEC_ClasseAccident
		insert SVG..FMC_ACC
		select * from HIS..FMC_ACC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	else if @va_ClasseEvt_in = XZAEC_ClasseVehicule
		insert SVG..FMC_VEH (numero, numero_vehicule, cle, depannage, site_depannage, type_de_vehicule, nombre_passagers,
			bebe, personne_agee, animaux_vivants, vehicule_ESCOTA, volume_de_chargement, matieres_dangereuses, 
			commentaire, sit, zz, marque, code_marque, type, couleur, code_couleur, immatriculation, pays, code_pays, 
			nom, ville, adresse, presence_matiere_dangereuse, code_danger, nature_panne, code_nature_panne, 
			positionnement, vehicule_roulant,enlevement_demande_par, code_enlev_demande_par, 
			numero_fiche_appel, fax_envoye, site_creation, nb_bebes, nb_agees, feu_maitrise, id_ava, dangerosite)
		select  
			numero, numero_vehicule, cle, depannage, site_depannage, type_de_vehicule, nombre_passagers,
			bebe, personne_agee, animaux_vivants, vehicule_ESCOTA, volume_de_chargement, matieres_dangereuses, 
			commentaire, sit, zz, marque, code_marque, type, couleur, code_couleur, immatriculation, pays, code_pays, 
			nom, ville, adresse, presence_matiere_dangereuse, code_danger, nature_panne, code_nature_panne, 
			positionnement, vehicule_roulant,enlevement_demande_par, code_enlev_demande_par, 
			numero_fiche_appel, fax_envoye, site_creation, nb_bebes, nb_agees, feu_maitrise, id_ava, dangerosite
		from HIS..FMC_VEH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseDelestage
		insert SVG..FMC_DEL
		select * from HIS..FMC_DEL 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseEchangeur
		insert SVG..FMC_ECH
		select * from HIS..FMC_ECH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseSousConcess
		insert SVG..FMC_CNC
		select * from HIS..FMC_CNC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseManif
		insert SVG..FMC_MNF
		select * from HIS..FMC_MNF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseVehLent
		insert SVG..FMC_VLN
		select * from HIS..FMC_VLN 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseMeteo
		insert SVG..FMC_MET
		select * from HIS..FMC_MET 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseBouchon
		insert SVG..FMC_BOU
		select * from HIS..FMC_BOU 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClassePosteOper
		insert SVG..FMC_OPR
		select * from HIS..FMC_OPR 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseNature
		insert SVG..FMC_NAT
		select * from HIS..FMC_NAT 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @va_ClasseEvt_in = XZAEC_ClasseIncendie
		insert SVG..FMC_INC
		select * from HIS..FMC_INC
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

	if @va_ClasseEvt_in in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
	begin
		insert SVG..FMC_TRF (
		    nature                          ,
		    type                            ,
		    continu                         ,
		    horodate_debut_planifiee        ,
		    horodate_fin_planifiee          ,
		    mobile                          ,
		    autoroute_planifiee             ,
		    PR_queue_planifie               ,
		    PR_tete_planifie                ,
		    sens_planifie                   ,
		    VR                              ,
		    VM2                             ,
		    VM1                             ,
		    VL                              ,
		    BAU                             ,
		    VR_I                            ,
		    VM2_I                           ,
		    VM1_I                           ,
		    VL_I                            ,
		    BAU_I                           ,
		    PR_debut_VR                     ,
		    PR_fin_VR                       ,
		    debut_VR                        ,
		    fin_VR                          ,
		    PR_debut_VM2                    ,
		    PR_fin_VM2                      ,
		    debut_VM2                       ,
		    fin_VM2                         ,
		    PR_debut_VM1                    ,
		    PR_fin_VM1                      ,
		    debut_VM1                       ,
		    fin_VM1                         ,
		    PR_debut_VL                     ,
		    PR_fin_VL                       ,
		    debut_VL                        ,
		    fin_VL                          ,
		    PR_debut_BAU                    ,
		    PR_fin_BAU                      ,
		    debut_BAU                       ,
		    fin_BAU                         ,
		    PR_debut_VR_I                   ,
		    PR_fin_VR_I                     ,
		    debut_VR_I                      ,
		    fin_VR_I                        ,
		    PR_debut_VM2_I                  ,
		    PR_fin_VM2_I                    ,
		    debut_VM2_I                     ,
		    fin_VM2_I                       ,
		    PR_debut_VM1_I                  ,
		    PR_fin_VM1_I                    ,
		    debut_VM1_I                     ,
		    fin_VM1_I                       ,
		    PR_debut_VL_I                   ,
		    PR_fin_VL_I                     ,
		    debut_VL_I                      ,
		    fin_VL_I                        ,
		    PR_debut_BAU_I                  ,
		    PR_fin_BAU_I                    ,
		    debut_BAU_I                     ,
		    fin_BAU_I                       ,
		    sit 			    ,
		    zz 				    )
		select 
		    nature                          ,
		    type                            ,
		    continu                         ,
		    horodate_debut_planifiee        ,
		    horodate_fin_planifiee          ,
		    mobile                          ,
		    autoroute_planifiee             ,
		    PR_queue_planifie               ,
		    PR_tete_planifie                ,
		    sens_planifie                   ,
		    VR                              ,
		    VM2                             ,
		    VM1                             ,
		    VL                              ,
		    BAU                             ,
		    VR_I                            ,
		    VM2_I                           ,
		    VM1_I                           ,
		    VL_I                            ,
		    BAU_I                           ,
		    PR_debut_VR                     ,
		    PR_fin_VR                       ,
		    debut_VR                        ,
		    fin_VR                          ,
		    PR_debut_VM2                    ,
		    PR_fin_VM2                      ,
		    debut_VM2                       ,
		    fin_VM2                         ,
		    PR_debut_VM1                    ,
		    PR_fin_VM1                      ,
		    debut_VM1                       ,
		    fin_VM1                         ,
		    PR_debut_VL                     ,
		    PR_fin_VL                       ,
		    debut_VL                        ,
		    fin_VL                          ,
		    PR_debut_BAU                    ,
		    PR_fin_BAU                      ,
		    debut_BAU                       ,
		    fin_BAU                         ,
		    PR_debut_VR_I                   ,
		    PR_fin_VR_I                     ,
		    debut_VR_I                      ,
		    fin_VR_I                        ,
		    PR_debut_VM2_I                  ,
		    PR_fin_VM2_I                    ,
		    debut_VM2_I                     ,
		    fin_VM2_I                       ,
		    PR_debut_VM1_I                  ,
		    PR_fin_VM1_I                    ,
		    debut_VM1_I                     ,
		    fin_VM1_I                       ,
		    PR_debut_VL_I                   ,
		    PR_fin_VL_I                     ,
		    debut_VL_I                      ,
		    fin_VL_I                        ,
		    PR_debut_BAU_I                  ,
		    PR_fin_BAU_I                    ,
		    debut_BAU_I                     ,
		    fin_BAU_I                       ,
		    sit 			    ,
		    zz 				    
		from HIS..FMC_TRF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 

		if @@rowcount != 1
			return XDC_NOK

		insert SVG..FMC_TRH
		select * from HIS..FMC_TRH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

		if @@rowcount = 0
			return XDC_NOK
	end

	if @va_ClasseEvt_in in (XZAEC_ClasseBasculement, XZAEC_ClasseBasculTrav)
	begin
		insert SVG..FMC_BAS
		select * from HIS..FMC_BAS 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	end

	if @@rowcount = 0 and @va_ClasseEvt_in != XZAEC_ClasseGenerique
	begin
		return XDC_NOK
	end

	/*A Sauvegarder l'historique de l'evenement */
	insert SVG..FMC_HIS
	select * from HIS..FMC_HIS 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in

	if @@rowcount = 0 and @va_ClasseEvt_in != XZAEC_ClassePosteOper
	begin
		return XDC_NOK
	end

	/*A Sauvegarder les commentaires de l'evenement */
	insert SVG..FMC_COM
	select * from HIS..FMC_COM 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in

	/*A Sauvegarder les attribut datex de l'evenement */
	insert SVG..FMC_DAT
	select * from HIS..FMC_DAT 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in

	/*A Sauvegarder l'alerte pointant sur l'evenement */
	insert SVG..ALT_EXP
	select * from HIS..ALT_EXP 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in 

	/*A Sauvegarder l'alerte pointee par l'evenement */
	insert SVG..ALT_EXP 
	select * from HIS..ALT_EXP 
	where	numero in (select alerte from HIS..FMC_GEN
			   where numero = @va_NumEvt_in and
				cle = @va_CleEvt_in  ) and
		sit = @va_CleEvt_in 

	/*A Sauvegarder de la partie generique de l'evenement */
	insert SVG..FMC_GEN (numero, cle, cle_cause, cause, horodate_lien_cause, horodate_chgt_type, alerte, 
		type, debut_prevu, debut, fin_prevue, fin, cloture, origine_creation, degats_domaine, 
		degrade, sit, poste_enrichisseur, zz, num_fmc_init, cle_fmc_init, sit_alerte, com_fmc_init)
	select 
		numero, cle, cle_cause, cause, horodate_lien_cause, horodate_chgt_type, alerte, 
		type, debut_prevu, debut, fin_prevue, fin, cloture, origine_creation, degats_domaine, 
		degrade, sit, poste_enrichisseur, zz, num_fmc_init, cle_fmc_init, sit_alerte, com_fmc_init
	from HIS..FMC_GEN
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

	if @@rowcount != 1
	begin
		return XDC_NOK
	end

	/*A Sauvegarder la cause de l'evenement */
	if @va_NumCause_in != null
	begin
		/*B Sauvegarder l'alerte pointant sur la cause de l'evenement */
		insert SVG..ALT_EXP
		select * from HIS..ALT_EXP
		where	evenement = @va_NumCause_in and cle = @va_CleCause_in

		/*A Sauvegarder l'alerte pointee par la cause de l'evenement */
		insert SVG..ALT_EXP 
		select * from HIS..ALT_EXP 
		where	numero in (select alerte from HIS..FMC_GEN
			   	where numero = @va_NumCause_in and
					cle = @va_CleCause_in  ) and
			sit = @va_CleCause_in 

		/*B Sauvegarder de la partie historique de la cause de l'evenement */
		insert SVG..FMC_HIS
		select * from HIS..FMC_HIS
		where	numero = @va_NumCause_in and cle = @va_CleCause_in

		/*B Sauvegarder du complement de la cause de l'evenement */

		/* recherche la classe de la cause */
		select @vl_ClasseCause = CFG..TYP_FMC.classe
		from HIS..FMC_GEN, CFG..TYP_FMC
		where	HIS..FMC_GEN.numero = @va_NumCause_in and 
			HIS..FMC_GEN.cle = @va_CleCause_in and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type

		/* Sauvegarder le complement de l'evenement et le complement */
		if @vl_ClasseCause = XZAEC_ClasseAccident
			insert SVG..FMC_ACC
			select * from HIS..FMC_ACC 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in 
		else if @vl_ClasseCause = XZAEC_ClasseVehicule
			insert SVG..FMC_VEH (numero, numero_vehicule, cle, depannage, site_depannage, type_de_vehicule, nombre_passagers,
				bebe, personne_agee, animaux_vivants, vehicule_ESCOTA, volume_de_chargement, matieres_dangereuses, 
				commentaire, sit, zz, marque, code_marque, type, couleur, code_couleur, immatriculation, pays, code_pays, 
				nom, ville, adresse, presence_matiere_dangereuse, code_danger, nature_panne, code_nature_panne, 
				positionnement, vehicule_roulant,enlevement_demande_par, code_enlev_demande_par, 
				numero_fiche_appel, fax_envoye, site_creation, nb_bebes, nb_agees, feu_maitrise, id_ava, dangerosite)
			select  
				numero, numero_vehicule, cle, depannage, site_depannage, type_de_vehicule, nombre_passagers,
				bebe, personne_agee, animaux_vivants, vehicule_ESCOTA, volume_de_chargement, matieres_dangereuses, 
				commentaire, sit, zz, marque, code_marque, type, couleur, code_couleur, immatriculation, pays, code_pays, 
				nom, ville, adresse, presence_matiere_dangereuse, code_danger, nature_panne, code_nature_panne, 
				positionnement, vehicule_roulant,enlevement_demande_par, code_enlev_demande_par, 
				numero_fiche_appel, fax_envoye, site_creation, nb_bebes, nb_agees, feu_maitrise, id_ava, dangerosite)
			from HIS..FMC_VEH 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseDelestage
			insert SVG..FMC_DEL
			select * from HIS..FMC_DEL 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseEchangeur
			insert SVG..FMC_ECH
			select * from HIS..FMC_ECH 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseSousConcess
			insert SVG..FMC_CNC
			select * from HIS..FMC_CNC 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseManif
			insert SVG..FMC_MNF
			select * from HIS..FMC_MNF 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseVehLent
			insert SVG..FMC_VLN
			select * from HIS..FMC_VLN 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseMeteo
			insert SVG..FMC_MET
			select * from HIS..FMC_MET 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseBouchon
			insert SVG..FMC_BOU
			select * from HIS..FMC_BOU 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClassePosteOper
			insert SVG..FMC_OPR
			select * from HIS..FMC_OPR 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseNature
			insert SVG..FMC_NAT
			select * from HIS..FMC_NAT 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in
		else if @vl_ClasseCause = XZAEC_ClasseIncendie
			insert SVG..FMC_INC
			select * from HIS..FMC_INC
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in

		if @vl_ClasseCause in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
		begin
			insert SVG..FMC_TRF (
				    nature                          ,
				    type                            ,
				    continu                         ,
				    horodate_debut_planifiee        ,
				    horodate_fin_planifiee          ,
				    mobile                          ,
				    autoroute_planifiee             ,
				    PR_queue_planifie               ,
				    PR_tete_planifie                ,
				    sens_planifie                   ,
				    VR                              ,
				    VM2                             ,
				    VM1                             ,
				    VL                              ,
				    BAU                             ,
				    VR_I                            ,
				    VM2_I                           ,
				    VM1_I                           ,
				    VL_I                            ,
				    BAU_I                           ,
				    PR_debut_VR                     ,
				    PR_fin_VR                       ,
				    debut_VR                        ,
				    fin_VR                          ,
				    PR_debut_VM2                    ,
				    PR_fin_VM2                      ,
				    debut_VM2                       ,
				    fin_VM2                         ,
				    PR_debut_VM1                    ,
				    PR_fin_VM1                      ,
				    debut_VM1                       ,
				    fin_VM1                         ,
				    PR_debut_VL                     ,
				    PR_fin_VL                       ,
				    debut_VL                        ,
				    fin_VL                          ,
				    PR_debut_BAU                    ,
				    PR_fin_BAU                      ,
				    debut_BAU                       ,
				    fin_BAU                         ,
				    PR_debut_VR_I                   ,
				    PR_fin_VR_I                     ,
				    debut_VR_I                      ,
				    fin_VR_I                        ,
				    PR_debut_VM2_I                  ,
				    PR_fin_VM2_I                    ,
				    debut_VM2_I                     ,
				    fin_VM2_I                       ,
				    PR_debut_VM1_I                  ,
				    PR_fin_VM1_I                    ,
				    debut_VM1_I                     ,
				    fin_VM1_I                       ,
				    PR_debut_VL_I                   ,
				    PR_fin_VL_I                     ,
				    debut_VL_I                      ,
				    fin_VL_I                        ,
				    PR_debut_BAU_I                  ,
				    PR_fin_BAU_I                    ,
				    debut_BAU_I                     ,
				    fin_BAU_I                       ,
				    sit 			    ,
				    zz 				    )
			select 
				    nature                          ,
				    type                            ,
				    continu                         ,
				    horodate_debut_planifiee        ,
				    horodate_fin_planifiee          ,
				    mobile                          ,
				    autoroute_planifiee             ,
				    PR_queue_planifie               ,
				    PR_tete_planifie                ,
				    sens_planifie                   ,
				    VR                              ,
				    VM2                             ,
				    VM1                             ,
				    VL                              ,
				    BAU                             ,
				    VR_I                            ,
				    VM2_I                           ,
				    VM1_I                           ,
				    VL_I                            ,
				    BAU_I                           ,
				    PR_debut_VR                     ,
				    PR_fin_VR                       ,
				    debut_VR                        ,
				    fin_VR                          ,
				    PR_debut_VM2                    ,
				    PR_fin_VM2                      ,
				    debut_VM2                       ,
				    fin_VM2                         ,
				    PR_debut_VM1                    ,
				    PR_fin_VM1                      ,
				    debut_VM1                       ,
				    fin_VM1                         ,
				    PR_debut_VL                     ,
				    PR_fin_VL                       ,
				    debut_VL                        ,
				    fin_VL                          ,
				    PR_debut_BAU                    ,
				    PR_fin_BAU                      ,
				    debut_BAU                       ,
				    fin_BAU                         ,
				    PR_debut_VR_I                   ,
				    PR_fin_VR_I                     ,
				    debut_VR_I                      ,
				    fin_VR_I                        ,
				    PR_debut_VM2_I                  ,
				    PR_fin_VM2_I                    ,
				    debut_VM2_I                     ,
				    fin_VM2_I                       ,
				    PR_debut_VM1_I                  ,
				    PR_fin_VM1_I                    ,
				    debut_VM1_I                     ,
				    fin_VM1_I                       ,
				    PR_debut_VL_I                   ,
				    PR_fin_VL_I                     ,
				    debut_VL_I                      ,
				    fin_VL_I                        ,
				    PR_debut_BAU_I                  ,
				    PR_fin_BAU_I                    ,
				    debut_BAU_I                     ,
				    fin_BAU_I                       ,
				    sit 			    ,
				    zz
			from HIS..FMC_TRF 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in 
	
			if @@rowcount != 1
				return XDC_NOK
	
			insert SVG..FMC_TRH
			select * from HIS..FMC_TRH 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in

			if @@rowcount = 0
				return XDC_NOK
		end

		if @vl_ClasseCause in (XZAEC_ClasseBasculement, XZAEC_ClasseBasculTrav)
		begin
			insert SVG..FMC_BAS
			select * from HIS..FMC_BAS 
			where	numero = @va_NumCause_in and
				cle = @va_CleCause_in 
		end
	
		if @@rowcount = 0 and @vl_ClasseCause != XZAEC_ClasseGenerique
		begin
			return XDC_NOK
		end

		/*B Sauvegarder de la partie generique de la cause de l'evenement */
		insert SVG..FMC_GEN (numero, cle, cle_cause, cause, horodate_lien_cause, horodate_chgt_type, alerte, 
			type, debut_prevu, debut, fin_prevue, fin, cloture, origine_creation, degats_domaine, 
			degrade, sit, poste_enrichisseur, zz, num_fmc_init, cle_fmc_init, sit_alerte, com_fmc_init)
		select 
			numero, cle, cle_cause, cause, horodate_lien_cause, horodate_chgt_type, alerte, 
			type, debut_prevu, debut, fin_prevue, fin, cloture, origine_creation, degats_domaine, 
			degrade, sit, poste_enrichisseur, zz, num_fmc_init, cle_fmc_init, sit_alerte, com_fmc_init
		from HIS..FMC_GEN
		where	numero = @va_NumCause_in and cle = @va_CleCause_in

		if @@rowcount != 1
		begin
			return XDC_NOK
		end
	end

	return XDC_OK
go
#endif
