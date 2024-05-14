/*E*/
/*  Fichier : $Id: xzae141sp.prc,v 1.9 2008/12/10 13:19:09 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2008/12/10 13:19:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae141sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos d'une situation strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* Niepceron	08 Mar 1999 : Modif intergest dem/1724 V1.4
* Niepceron     19 Mar 1999 : Modif intergest dem/1743 V1.5
* F. Lizot      05 Mar 2001 : Pas d'alerte pour evt DDE83 et
*                             gestion du droit de proriete ESCOTA/DDE83 (SAGA) V1.6
* F.Lizot  	27/06/2005 	:  Correction SAGA DEM473 1.7
* CHI		20/07/05    : Cas nature de l'obstacle table FMC_NAT DEM487 v1.6!
* JPL		08/06/2007 : Cas Basculement pour travaux : idem basculement + travaux (DEM 647) 1.8
* JMG	03/12/08	ajout validation dans FMC_ACC 1.8 DEM/852
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE141_Stocke_Situation_Strada
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos obligatoires pour generer
* un fichier STRADA
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE141
go


create procedure XZAE141
	@va_emetteur_in		char(10)=null,
	@va_reference_in 	char(35)=null,
	@va_version_in		int=null,
	@va_dob_in		char(3)=null,
	@va_lnk_in		char(35)=null,
	@va_date_in		datetime,
	@va_type_in		char(50)=null,
	@va_type_FMC_in		smallint,
	@va_debut_in		datetime,
	@va_fin_in		datetime,
	@va_fin_prevue_in	datetime,
	@va_duree_in		tinyint,
	@va_confirme_signale_in	bit,
	@va_fausse_alerte_in	tinyint,
	@va_autoroute_p_in	tinyint,
	@va_PR_p_in		T_PR,
	@va_sens_p_in		tinyint,
	@va_autoroute_s_in	tinyint,
	@va_PR_s_in		T_PR,
	@va_sens_s_in		tinyint,
	@va_niveau_trafic_in	tinyint,
	@va_gravite_in		tinyint,
	@va_vl_in		tinyint,
	@va_pl_in		tinyint,
	@va_caravanes_in	tinyint,
	@va_cars_in		tinyint,
	@va_blesses_graves_in	tinyint,
	@va_morts_in		tinyint,
	@va_personnalites_in	bit,
	@va_matieres_dangereuses_in bit,
	@va_type_bouchon_in	tinyint,
	@va_longueur_in		int,
	@va_type_perturbation_in tinyint,
	@va_visibilite_in		smallint,
	@va_bloquant_in		bit,
	@va_texte1_in		char(255)=null,
	@va_texte2_in		char(100)=null

	
as
declare @vl_evt int,
	@vl_cle tinyint,
	@vl_evt_tete int,
	@vl_cle_tete tinyint,
	@vl_lien char(35),
	@vl_nom_autoroute char(4),
	@vl_nom_PMV char(15),
	@vl_TypeEvt T_SITE, @vl_ClasseEvt tinyint,
	@vl_alerte int, @vl_qualif tinyint,
	@vl_texte char(150),
	@vl_morceau smallint,
	@vl_poste smallint,
	@vl_site tinyint,
	@vl_no_action int,
	@vl_Status int,
	@vl_date_in char(20),
	@vl_gravite tinyint,
	@vl_textec	char(250),
	@vl_propriete_fmc bit,  /* Ajout SAGA */
	@vl_propriete_queue bit, /* Ajout SAGA */
	@vl_bloquant bit,
	@vl_SiteMaitre T_SITE

	 
	if @va_emetteur_in = null or 
		@va_reference_in = null or 
		@va_dob_in = null or
		@va_date_in = null
	   return XDC_ARG_INV
	   
	if @va_fin_in <= "01/01/1970" 		select 	@va_fin_in=null
	if @va_fin_prevue_in <= "01/01/1970" 	select 	@va_fin_prevue_in=null

	if @va_autoroute_s_in = null	select @va_autoroute_s_in=@va_autoroute_p_in
	if @va_PR_s_in = null	select @va_autoroute_s_in=@va_PR_p_in
	if @va_sens_s_in = null	 select @va_autoroute_s_in=@va_sens_p_in

	delete EXP..ADA_TXT_SYN where reference=@va_reference_in and emetteur=@va_emetteur_in

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*A Message TFM */  
	if @va_dob_in = "INF"
	begin
		if @va_lnk_in = null
			return XDC_OK
		if exists ( select * from EXP..ADA_SIT where emetteur=@va_emetteur_in and reference= @va_reference_in )
		begin
			update EXP..ADA_SIT 
			set 	texte1=@va_texte1_in, texte2=@va_texte2_in
			where	emetteur=@va_emetteur_in and reference= @va_reference_in
		end
		else
		begin
			insert into EXP..ADA_SIT 
				(emetteur,reference, horodate, type, type_FMC,debut,fin,fin_prevue,duree,confirme_signale,
				fausse_alerte,autoroute_prim,PR_prim,sens_prim,autoroute_second, PR_second, sens_second, 
				niveau_trafic, gravite, vl, pl, caravanes, cars, blesses_graves, morts, personnalites, 
				matieres_dangereuses, type_bouchon, longueur, type_perturbation, visibilite, bloquant, texte1, texte2, lien)
			values (@va_emetteur_in, @va_reference_in, @va_date_in, @va_type_in, 0, @va_debut_in, @va_fin_in, @va_fin_prevue_in, @va_duree_in, @va_confirme_signale_in,
				@va_fausse_alerte_in,@va_autoroute_p_in,@va_PR_p_in,@va_sens_p_in,@va_autoroute_s_in, @va_PR_s_in, @va_sens_s_in, 
				@va_niveau_trafic_in, @va_gravite_in, @va_vl_in, @va_pl_in, @va_caravanes_in, @va_cars_in, @va_blesses_graves_in, @va_morts_in, @va_personnalites_in, 
				@va_matieres_dangereuses_in, @va_type_bouchon_in, @va_longueur_in, @va_type_perturbation_in, @va_visibilite_in, @va_bloquant_in, @va_texte1_in, @va_texte2_in, @va_lnk_in)
		end
		/* S'il existe une FMC DATEX concernee par le message alors envoyer un reveil sur cette fmc au CI */
		select @vl_evt=evenement,@vl_cle=cle from EXP..ADA_SIT
		where emetteur=@va_emetteur_in and reference=@va_lnk_in and evenement<>null and cle<>null
		if @vl_evt<>null and @vl_cle<>null
		begin
                        declare postes cursor for
                        select  poste_enrichisseur 
                        from EXP..FMC_GEN 
                        where   type=XZAEC_FMC_PosteOperateur and
                                sit=@vl_SiteMaitre and
                                fin=null
                        
                	open postes
                	fetch postes into @vl_poste
                	while (@@sqlstatus = 0)
                	begin
                        	insert EXP..FMC_REV
                              		( evenement, cle, machine_operateur, texte, sit )
                                        values ( @vl_evt, @vl_cle,  @vl_poste, "NOUVEAU MESSAGE",  @vl_SiteMaitre)
                        	fetch  postes into @vl_poste
               		end
                	close postes
                	deallocate cursor postes
			
		end
		
		
	end
	else if @va_dob_in = "EQU"
	begin
		if @va_lnk_in = null
			return XDC_OK
			
		select @vl_nom_autoroute=nom
		from CFG..RES_AUT
		where numero=@va_autoroute_p_in
			
		if @vl_nom_autoroute=null
			return XDC_OK
			
		select @vl_nom_PMV=rtrim(@vl_nom_autoroute) + '-' + convert(varchar(6),@va_PR_p_in) + '-' + convert(varchar(1),@va_sens_p_in)
		
		/* S'il existe une FMC DATEX concernee par le message alors envoyer un reveil sur cette fmc au CI */
		select @vl_evt=evenement,@vl_cle=cle from EXP..ADA_SIT
		where emetteur=@va_emetteur_in and reference=@va_lnk_in and evenement<>null and cle<>null
		if @vl_evt<>null and @vl_cle<>null
		begin
			select  @vl_no_action=actio 
			from EXP..ACT_PMV_ADA, EXP..ACT_GEN
			where PMV=@vl_nom_PMV and EXP..ACT_PMV_ADA.sit=@vl_SiteMaitre and 
				evenement=@vl_evt and cle=@vl_cle
			
			if @vl_no_action=null or @va_fin_in=null
			begin
				select @vl_no_action = isnull(max(numero), 0) + 1 
				from EXP..ACT_GEN
				where sit = @vl_SiteMaitre
				
				insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
					heure_succes, heure_echec, prioritaire, heure_lancement_fin,
					heure_fin, operateur, sit, distance_evt, sit_origine)
				values( @vl_no_action, @vl_evt, @vl_cle, 
					XDC_ACT_PMVDAT, @va_debut_in, @va_debut_in,
					null, XDC_NON, null,
					@va_fin_in, 0, @vl_SiteMaitre, null, @vl_SiteMaitre)
					
				insert EXP..ACT_PMV_ADA (actio,sit,PMV,texte1,texte2)
				values (@vl_no_action, @vl_cle,@vl_nom_PMV,@va_texte1_in,@va_texte2_in)
				
			end
			else
			begin
				update EXP..ACT_GEN
				set heure_succes=@va_debut_in, heure_fin=@va_fin_in
				where numero=@vl_no_action and sit=@vl_SiteMaitre
				update EXP..ACT_PMV_ADA
				set texte1=@va_texte1_in,texte2=@va_texte2_in
				where actio=@vl_no_action and sit=@vl_SiteMaitre
				

			end
			

		end
		update EXP..ACT_GEN
		set heure_fin=isnull(@va_fin_in,@va_debut_in)
		where heure_fin=null and 
			type=XDC_ACT_PMVDAT and
			sit=@vl_SiteMaitre and
			numero <> @vl_no_action and 
			numero in ( select actio from EXP..ACT_PMV_ADA where PMV=@vl_nom_PMV) 
	end
	else
	begin
		/*B Mise a jour ou insertion dans ADA_SIT */
		if exists ( select * from EXP..ADA_SIT where emetteur=@va_emetteur_in and reference= @va_reference_in )
		begin
			update EXP..ADA_SIT 
			set 	horodate=@va_date_in,debut=@va_debut_in,fin=@va_fin_in,fin_prevue=@va_fin_prevue_in,duree=@va_duree_in,confirme_signale=@va_confirme_signale_in,
			fausse_alerte=@va_fausse_alerte_in,autoroute_prim=@va_autoroute_p_in,PR_prim=@va_PR_p_in,sens_prim=@va_sens_p_in,autoroute_second=@va_autoroute_s_in, 
			PR_second=@va_PR_s_in, sens_second=@va_sens_s_in, 
			niveau_trafic=@va_niveau_trafic_in, gravite=@va_gravite_in, vl=@va_vl_in, pl=@va_pl_in, caravanes=@va_caravanes_in, cars=@va_cars_in, 
			blesses_graves=@va_blesses_graves_in, morts=@va_morts_in, personnalites=@va_personnalites_in, 
			matieres_dangereuses=@va_matieres_dangereuses_in, type_bouchon=@va_type_bouchon_in, longueur=@va_longueur_in, type_perturbation=@va_type_perturbation_in, 
			visibilite=@va_visibilite_in, bloquant=@va_bloquant_in, texte1=@va_texte1_in, texte2=@va_texte2_in, lien=@va_lnk_in
			where	emetteur=@va_emetteur_in and reference= @va_reference_in
		end
		else
		begin
			insert into EXP..ADA_SIT 
				(emetteur,reference, horodate, type, type_FMC,debut,fin,fin_prevue,duree,confirme_signale,
				fausse_alerte,autoroute_prim,PR_prim,sens_prim,autoroute_second, PR_second, sens_second, 
				niveau_trafic, gravite, vl, pl, caravanes, cars, blesses_graves, morts, personnalites, 
				matieres_dangereuses, type_bouchon, longueur, type_perturbation, visibilite, bloquant, texte1, texte2, lien)
			values (@va_emetteur_in, @va_reference_in, @va_date_in, @va_type_in, @va_type_FMC_in, @va_debut_in, @va_fin_in, @va_fin_prevue_in, @va_duree_in, @va_confirme_signale_in,
				@va_fausse_alerte_in,@va_autoroute_p_in,@va_PR_p_in,@va_sens_p_in,@va_autoroute_s_in, @va_PR_s_in, @va_sens_s_in, 
				@va_niveau_trafic_in, @va_gravite_in, @va_vl_in, @va_pl_in, @va_caravanes_in, @va_cars_in, @va_blesses_graves_in, @va_morts_in, @va_personnalites_in, 
				@va_matieres_dangereuses_in, @va_type_bouchon_in, @va_longueur_in, @va_type_perturbation_in, @va_visibilite_in, @va_bloquant_in, @va_texte1_in, @va_texte2_in, @va_lnk_in)
		end
		if @@rowcount != 1
			return XDC_NOK
			
			
		/*B s'il existe une alerte correspondant a l'evt */
		select @vl_date_in= convert(varchar(10),@va_date_in,103) + " " + convert(varchar(8),@va_date_in,108) /*convert(varchar(2),datepart(day,@va_date_in)) + "/" + convert(varchar(2),datepart(month,@va_date_in)) + "/" + convert(char(4),datepart(year,@va_date_in)) + " " + convert(varchar(2),datepart(hour,@va_date_in)) + ":" + convert(varchar(2),datepart(minute,@va_date_in))*/
		
		select @vl_evt=evenement,@vl_cle=cle,@vl_alerte=alerte from EXP..ADA_SIT where emetteur=@va_emetteur_in and reference= @va_reference_in
		/* ajout SAGA : dans le cas des messages de la DDE83, il n'y a pas d'alerte mais elle est implicite */
		if @vl_alerte <> null   or @va_emetteur_in = "frd83"
		begin
			/*B s'il existe une FMC pour cet evt */
			if @vl_evt<>null and @vl_cle<>null
			begin	
			/* Ajout SAGA : La FMC n'est mise a jour que si ESCOTA n'a pas le droit de propriete */
			select
                                @vl_ClasseEvt = classe,
                                @vl_TypeEvt = sit
                        from EXP..FMC_GEN, CFG..TYP_FMC
			where   EXP..FMC_GEN.numero = @vl_evt and cle = @vl_cle and
                                CFG..TYP_FMC.numero = EXP..FMC_GEN.type
			select @vl_propriete_fmc   = XDC_NON
                        select @vl_propriete_queue = XDC_NON
			exec XZAE551 @vl_evt,@vl_cle,@vl_propriete_fmc output,@vl_propriete_queue output
			if @vl_ClasseEvt=XZAEC_ClasseBouchon
				select @vl_bloquant=XDC_VRAI
			else
				select @vl_bloquant = @va_bloquant_in

			if @vl_propriete_fmc=XDC_NON /* Ajout SAGA */
			begin /*B Mise a jour de la FMC */
				if @va_gravite_in=5 
					select @vl_gravite=0
				else if @va_gravite_in=4
					select @vl_gravite=0
				else if @va_gravite_in=3
					select @vl_gravite=1
				else if @va_gravite_in=2
					select @vl_gravite=2
				else if @va_gravite_in=1
					select @vl_gravite=3
				else 
					select @vl_gravite=0
				update EXP..FMC_DAT
				set gravite=@vl_gravite,trafic=@va_niveau_trafic_in,bloquant=@va_bloquant_in,duree=@va_duree_in
				where	numero = @vl_evt and
					cle = @vl_cle and
					sit = @vl_SiteMaitre
				select 
					@vl_ClasseEvt = classe,
					@vl_TypeEvt = sit
				from EXP..FMC_GEN, CFG..TYP_FMC
				where	EXP..FMC_GEN.numero = @vl_evt and cle = @vl_cle and
					CFG..TYP_FMC.numero = EXP..FMC_GEN.type
					
				if @vl_ClasseEvt = XZAEC_ClasseAccident
					insert EXP..FMC_ACC (numero,cle,nombre_de_vl,nombre_de_pl,
							nombre_de_remorques, nombre_de_car,
							nombre_de_moto,vehicule_ESCOTA,
							matieres_dangereuses,blesses_legers, 
							blesses_graves,morts,personnel_ESCOTA,
							personnalites,degats_domaine,sit,validation)
					values ( @vl_evt, @vl_SiteMaitre,@va_vl_in,@va_pl_in,@va_caravanes_in,@va_cars_in,0,0,
						@va_matieres_dangereuses_in,0,
						@va_blesses_graves_in,@va_morts_in,
						0,@va_personnalites_in,
						XDC_FAUX,@vl_SiteMaitre,@va_date_in)
				else if @vl_ClasseEvt = XZAEC_ClasseDelestage
					update EXP..FMC_DEL 
					set	zz=zz+1
					where	numero = @vl_evt and
					cle = @vl_cle and
					sit = @vl_SiteMaitre
				else if @vl_ClasseEvt = XZAEC_ClasseManif
					update EXP..FMC_MNF 
					set zz=zz+1
					where	numero = @vl_evt and
						cle = @vl_cle and
						sit = @vl_SiteMaitre
				else if @vl_ClasseEvt = XZAEC_ClasseVehLent
					update EXP..FMC_VLN 
					set sit = @vl_SiteMaitre
					where	numero = @vl_evt and
						cle = @vl_cle and
						sit = @vl_SiteMaitre
				else if @vl_ClasseEvt = XZAEC_ClasseMeteo
					update EXP..FMC_MET 
					set horodate=@va_date_in,PR=@va_PR_p_in,visibilite=@va_visibilite_in,type_de_perturbation=@va_type_perturbation_in
					where	numero = @vl_evt and
						cle = @vl_cle and
						sit = @vl_SiteMaitre
				else if @vl_ClasseEvt = XZAEC_ClasseBouchon
				begin
					select @vl_evt_tete=cause
					from EXP..FMC_GEN
					where	numero = @vl_evt and
						cle = @vl_cle
					update EXP..FMC_GEN
					set zz=zz+1, fausse_alerte=@va_fausse_alerte_in, debut=@va_debut_in, fin=@va_fin_in,
					fin_prevue=@va_fin_prevue_in
					where	numero = @vl_evt_tete and
						cle = @vl_cle
						
					if @vl_bloquant=XDC_VRAI
						insert EXP..FMC_HIS
							(numero,cle, horodate_validation,confirme_ou_signale,type_anterieur,
         						type_FMC_valide,autoroute, PR,sens,point_caracteristique,
         						numero_point_caracteristique,position_sur_aire_ou_echangeur,VR,VM2,
         						VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite, BAU_I_etroite,sit,longueur)
         					values (@vl_evt_tete,@vl_cle,@va_date_in,@va_confirme_signale_in,XZAEC_FMC_TeteBouchon, XZAEC_FMC_TeteBouchon, /* SAGA : mise de XZAEC_FMC_TeteBouchon */ 
							@va_autoroute_p_in,@va_PR_p_in,@va_sens_p_in,null,null,null,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,
							XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
							XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,@vl_SiteMaitre,@va_longueur_in)
					else
						insert EXP..FMC_HIS
							(numero,cle, horodate_validation,confirme_ou_signale,type_anterieur,
         						type_FMC_valide,autoroute, PR,sens,point_caracteristique,
         						numero_point_caracteristique,position_sur_aire_ou_echangeur,VR,VM2,
         						VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite, BAU_I_etroite,sit,longueur)
         					values (@vl_evt_tete,@vl_cle,@va_date_in,@va_confirme_signale_in,XZAEC_FMC_TeteBouchon, XZAEC_FMC_TeteBouchon, /* SAGA : mise de XZAEC_FMC_TeteBouchon */ 
							@va_autoroute_p_in,@va_PR_p_in,@va_sens_p_in,null,null,null,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
							XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
							XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,@vl_SiteMaitre,@va_longueur_in)
					
				end

				if (@vl_ClasseEvt = XZAEC_ClasseBasculement or
				    @vl_ClasseEvt = XZAEC_ClasseBasculTrav)
				begin
					update EXP..FMC_BAS 
					set zz=zz+1
					where	numero = @vl_evt and
						cle = @vl_cle and
						sit = @vl_SiteMaitre
				end
				if (@vl_ClasseEvt = XZAEC_ClasseTravaux or
				    @vl_ClasseEvt = XZAEC_ClasseBasculTrav)
				begin
					/* EXP..FMC_TRF, EXP..FMC_TRH */
					update EXP..FMC_TRH
					set horodate=@va_date_in, localisation_tete=@va_PR_p_in
					where	numero = @vl_evt and
						cle = @vl_cle and
						sit = @vl_SiteMaitre
				end

				end  /* Ajout SAGA */


				if (@vl_ClasseEvt = XZAEC_ClasseBouchon and @vl_propriete_queue=XDC_NON)           /* Ajout SAGA */
                                        insert EXP..FMC_BOU (numero,cle,horodate,type,sit)                 /* Ajout SAGA */
					values ( @vl_evt, @vl_cle,@va_date_in,@va_type_bouchon_in,@vl_SiteMaitre)  /* Ajout SAGA */
				if (@vl_ClasseEvt = XZAEC_ClasseBouchon and @vl_propriete_queue=XDC_NON) or
                           		(@vl_ClasseEvt <> XZAEC_ClasseBouchon and @vl_propriete_fmc=XDC_NON)/* Ajout SAGA */
				begin                          /* Ajout SAGA */
				/*B Mise a jour du tronc commun des FMC */
				if @vl_bloquant=XDC_VRAI
					insert EXP..FMC_HIS
						(numero,cle, horodate_validation,confirme_ou_signale,type_anterieur,
         					type_FMC_valide,autoroute, PR,sens,point_caracteristique,
         					numero_point_caracteristique,position_sur_aire_ou_echangeur,VR,VM2,
         					VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite, BAU_I_etroite,sit,longueur)
         				values (@vl_evt,@vl_cle,@va_date_in,@va_confirme_signale_in,@va_type_FMC_in, @va_type_FMC_in, 
						@va_autoroute_s_in,@va_PR_s_in,@va_sens_s_in,null,null,null,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,
						XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
						XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,@vl_SiteMaitre,@va_longueur_in)
				else
					insert EXP..FMC_HIS
						(numero,cle, horodate_validation,confirme_ou_signale,type_anterieur,
         					type_FMC_valide,autoroute, PR,sens,point_caracteristique,
         					numero_point_caracteristique,position_sur_aire_ou_echangeur,VR,VM2,
         					VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite, BAU_I_etroite,sit,longueur)
         				values (@vl_evt,@vl_cle,@va_date_in,@va_confirme_signale_in,@va_type_FMC_in, @va_type_FMC_in, 
						@va_autoroute_s_in,@va_PR_s_in,@va_sens_s_in,null,null,null,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
						XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
						XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,@vl_SiteMaitre,@va_longueur_in)
				end	
				
				if @vl_propriete_fmc=XDC_NON or @vl_propriete_queue=XDC_NON   /* Ajout SAGA */
				begin                          /* Ajout SAGA */
				/*B Mise a jour des commentaires */
				update EXP..FMC_COM set horodate_fin=@va_date_in where evenement=@vl_evt and cle=@vl_cle and horodate_fin=null
				/* Ajout SAGA : traitement du cas isnull */
                                select @vl_morceau=isnull(max(numero),0) from EXP..FMC_COM where evenement=@vl_evt and cle=@vl_cle
	
				if ( @va_texte1_in != null and @va_texte1_in != "" )
				insert EXP..FMC_COM
					(evenement,cle,numero,horodate_validation,texte,sit)
				values (@vl_evt,@vl_cle,@vl_morceau+1,@va_date_in,@va_texte1_in,@vl_SiteMaitre)
				if ( @va_texte2_in!= null and @va_texte2_in != "" )
				insert EXP..FMC_COM
					(evenement,cle,numero,horodate_validation,texte,sit)
				values (@vl_evt,@vl_cle,@vl_morceau+2,@va_date_in,@va_texte2_in,@vl_SiteMaitre)
				
				update EXP..FMC_GEN
				set zz=zz+1, fausse_alerte=@va_fausse_alerte_in, debut=@va_debut_in, fin=@va_fin_in,
				/*set zz=zz+1, fausse_alerte=@va_fausse_alerte_in, type=@va_type_FMC_in, debut=@va_debut_in, fin=@va_fin_in,*/
					fin_prevue=@va_fin_prevue_in
				where	numero = @vl_evt and
					cle = @vl_cle
					
				/*B Envoi du reveil sur FMC */
                        	declare postes cursor for
                        	select  poste_enrichisseur,sit 
                        	from EXP..FMC_GEN 
                        	where   type=XZAEC_FMC_PosteOperateur and
                                	fin=null
                        
                		open postes
                		fetch postes into @vl_poste,@vl_site
                		while (@@sqlstatus = 0)
                		begin
                        		insert EXP..FMC_REV
                              			( evenement, cle, machine_operateur, texte, sit )
                                        	values ( @vl_evt, @vl_cle,  @vl_poste, XDC_REV_FMC_MOD,  @vl_site)
                        		fetch  postes into @vl_poste,@vl_site
               			end
                		close postes
                		deallocate cursor postes
					
				
			end                            /* Ajout SAGA */
			end
			else
			begin
				/*B Reveil de  l'alerte*/
				/* sauf si l'evenement provient de la DDE83 (SAGA) */
                                if @va_emetteur_in <> "frd83"
                                begin
				select @vl_qualif=qualification 
				from EXP..ALT_EXP
				where numero=@vl_alerte and
					sit=@vl_SiteMaitre
				if @vl_qualif=XDC_ALT_INCONNUE
					update EXP..ALT_EXP
					set horodate=@va_date_in
					where numero=@vl_alerte and
						sit=@vl_SiteMaitre
				else
				begin
					/* creation nouvelle alerte car la precedente a ete supprimer (qualif=XDC_ALT_FAUSSE) */
					select @vl_texte=rtrim(@va_emetteur_in) + " " + rtrim(@va_reference_in) + " " + rtrim(@va_type_in) + " " + @vl_date_in
					exec @vl_Status=PRC..XZAA;01 @va_date_in,13,8,38,@vl_texte
					select @vl_alerte=numero
					from EXP..ALT_EXP
					where horodate=@va_date_in and type=38 and texte=@vl_texte and sit=@vl_SiteMaitre
					update EXP..ADA_SIT 
					set 	alerte=@vl_alerte
					where	emetteur=@va_emetteur_in and reference= @va_reference_in
				end
			      end
			end
		end
		else
		begin
			/* creation nouvelle alerte */
			/* sauf si l'evenement provient de la DDE83 (SAGA) */
                        if @va_emetteur_in <> "frd83"
                        begin
			select @vl_texte=rtrim(@va_emetteur_in) + " " + rtrim(@va_reference_in) + " " + rtrim(@va_type_in) + " " + @vl_date_in
			exec @vl_Status=PRC..XZAA;01 @va_date_in,13,8,38,@vl_texte
			
			/* Mise a jour ADA_SIT avec no d'alerte cree */
			select @vl_alerte=numero
			from EXP..ALT_EXP
			where horodate=@va_date_in and type=38 and texte=@vl_texte and sit=@vl_SiteMaitre
			update EXP..ADA_SIT 
			set 	alerte=@vl_alerte
			where	emetteur=@va_emetteur_in and reference= @va_reference_in
		end
	     end	
	end

	return XDC_OK
go

