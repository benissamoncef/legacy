/*E*/
/*  Fichier : $Id: xzae152sp.prc,v 1.8 2019/04/01 17:55:10 devgfi Exp $        $Revision: 1.8 $        $Date: 2019/04/01 17:55:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae152sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos d'une situation strada
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	21 JAn 1999 : creation
* Niepceron	17 MAr 1999 :  correction gravite dem/1748 1.2
* JPL		12/04/07 : Creation ligne dans FMC_NAT pour types de classe Nature 1.4
* JPL		21/05/07 : Creation ligne dans FMC_MET pour fmc Chaussee glissante 1.5
* JPL		01/09/08 : Texte de reveil marque urgent pour les Fmc Accident et Contresens (DEM 721) 1.6
* JMG	03/12/08 : ajout validation dans FMC_ACC 1.7 DEM/852
* JPL	15/03/19 : Ecriture date �v�nement en table compl�mentaire FMC_MNF historis�e (DEM 1327)  1.8
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE152_Creation_FMCDATEX
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

if exists (select * from sysobjects where name = 'XZAE152' and type = 'P')
	drop procedure XZAE152
go


create procedure XZAE152
	@va_alerte_in 		int=null,
	@va_FMC_out 		int=null output
	
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
	@vl_poste smallint,
	@vl_no_action int,
	@vl_degats tinyint,
	@vl_Status int,
	@vl_emetteur 		char(10),
	@vl_reference  	char(35),
	@vl_version 		int,
	@vl_dob 		char(3),
	@vl_lnk 		char(35),
	@vl_date 		datetime,
	@vl_type 		char(50),
	@vl_type_FMC 		smallint,
	@vl_debut 		datetime,
	@vl_fin 		datetime,
	@vl_fin_prevue 	datetime,
	@vl_duree 		tinyint,
	@vl_confirme_signale 	bit,
	@vl_fausse_alerte 	tinyint,
	@vl_autoroute_p 	tinyint,
	@vl_PR_p 		T_PR,
	@vl_sens_p 		tinyint,
	@vl_autoroute_s 	tinyint,
	@vl_PR_s 		T_PR,
	@vl_sens_s 		tinyint,
	@vl_niveau_trafic 	tinyint,
	@vl_gravite 		tinyint,
	@vl_vl 		tinyint,
	@vl_pl 		tinyint,
	@vl_caravanes 	tinyint,
	@vl_cars 		tinyint,
	@vl_blesses_graves 	tinyint,
	@vl_morts 		tinyint,
	@vl_personnalites 	bit,
	@vl_matieres_dangereuses  bit,
	@vl_bloquant		  bit,
	@vl_type_bouchon 	tinyint,
	@vl_longueur 		int,
	@vl_type_perturbation  tinyint,
	@vl_visibilite 		smallint,
	@vl_texte1 		char(255),
	@vl_texte2 		char(100),
	@vl_morceau 		smallint,
	@vl_code 		tinyint,
	@vl_texte_alt		T_NOM,
	@vl_TexteReveil		char(100),
	@vl_SiteMaitre T_SITE
	 
	 
	
	if @va_alerte_in  = null 
	   return XDC_ARG_INV
	 
	 
	select
		@vl_emetteur =emetteur,
		@vl_reference =reference,
		@vl_lnk =lien,
		@vl_date =horodate,
		@vl_type =type,
		@vl_type_FMC =type_FMC,
		@vl_debut =debut,
		@vl_fin =fin,
		@vl_fin_prevue =fin_prevue,
		@vl_duree =duree,
		@vl_confirme_signale =confirme_signale,
		@vl_fausse_alerte =fausse_alerte,
		@vl_autoroute_p =autoroute_prim,
		@vl_PR_p =PR_prim,
		@vl_sens_p =sens_prim,
		@vl_autoroute_s =autoroute_second,
		@vl_PR_s =PR_second,
		@vl_sens_s =sens_second,
		@vl_niveau_trafic =niveau_trafic,
		@vl_gravite =gravite,
		@vl_vl =vl,
		@vl_pl =pl,
		@vl_caravanes =caravanes,
		@vl_cars =cars,
		@vl_blesses_graves =blesses_graves,
		@vl_morts =morts,
		@vl_personnalites =personnalites,
		@vl_matieres_dangereuses =matieres_dangereuses,
		@vl_type_bouchon =type_bouchon,
		@vl_longueur =longueur,
		@vl_type_perturbation =type_perturbation,
		@vl_visibilite =visibilite,
		@vl_bloquant =bloquant,
		@vl_texte1 =texte1,
		@vl_texte2 =texte2
	from EXP..ADA_SIT
	where alerte=@va_alerte_in 

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD
	
	/*A Verification de l'existence de la situation */
	if @@rowcount=0
		return XDC_NOK
	if @vl_gravite=5
		select @vl_gravite=0
	else if @vl_gravite=4
		select @vl_gravite=0
	else if @vl_gravite=3
		select @vl_gravite=1
	else if @vl_gravite=2
		select @vl_gravite=2
	else if @vl_gravite=1
		select @vl_gravite=3
	else
		select @vl_gravite=0

	select
		@vl_texte_alt=texte
	from EXP..ALT_EXP
	where numero=@va_alerte_in and sit=@vl_SiteMaitre
		
	/*A Verification de l'existence du type de FMC */
	if not exists ( select * from CFG..LIB_PRE where notyp=XDC_TYPE_FICHE_MC and code=@vl_type_FMC )
		return XDC_NOK
		
	/*A calcul du numero de la fiche main courante a creer */
	select @vl_evt = isnull(max(numero), 0) + 1 from EXP..FMC_GEN
        where cle = @vl_SiteMaitre

	 
        begin tran
                /*B Valuer les degats a 0 si fmc concernee pas ce champ sinon null */
                if      @vl_type_FMC = XZAEC_FMC_Accident or
                        @vl_type_FMC = XZAEC_FMC_Agression or
                        @vl_type_FMC = XZAEC_FMC_AltercationUsager or
                        @vl_type_FMC = XZAEC_FMC_AnimalErrant or
                        @vl_type_FMC = XZAEC_FMC_Autre or
                        @vl_type_FMC = XZAEC_FMC_ChausseDeter or
                        @vl_type_FMC = XZAEC_FMC_ConvoiExc or
                        @vl_type_FMC = XZAEC_FMC_Eboulement or
                        @vl_type_FMC = XZAEC_FMC_FlaqueHuile or
                        @vl_type_FMC = XZAEC_FMC_GreveESCOTA or
                        @vl_type_FMC = XZAEC_FMC_HoldUpVol or
                        @vl_type_FMC = XZAEC_FMC_Incendie or
                        @vl_type_FMC = XZAEC_FMC_IncSsConces or
                        @vl_type_FMC = XZAEC_FMC_Inondation or
                        @vl_type_FMC = XZAEC_FMC_Manifestation or
                        @vl_type_FMC = XZAEC_FMC_Obstacle or
                        @vl_type_FMC = XZAEC_FMC_Meteo or
                        @vl_type_FMC = XZAEC_FMC_VehArrete or
                        @vl_type_FMC = XZAEC_FMC_VehPanne or
                        @vl_type_FMC = XZAEC_FMC_VehFeu
                        select @vl_degats = 0
                else    select @vl_degats = null

                /*B insertion de la partie generique */
                /*! ne pas inserer de chaine null    */
                insert EXP..FMC_GEN (numero, cle, type, sit, poste_enrichisseur,
                        origine_creation, debut, debut_prevu, fin, fin_prevue, 
                        degats_domaine, fausse_alerte, zz,degrade,datex,
                        alerte,sit_alerte)
                values ( @vl_evt, @vl_SiteMaitre,
                        @vl_type_FMC, @vl_SiteMaitre,
                        null, @vl_texte_alt,
                        @vl_debut, null, @vl_fin,@vl_fin_prevue,
                        @vl_degats, @vl_fausse_alerte, 1, XDC_FAUX,XDC_VRAI,
                        @va_alerte_in,@vl_SiteMaitre)

                if @@rowcount != 1
                        return XDC_NOK
        commit tran
	 
	if @vl_bloquant=XDC_VRAI
	 insert EXP..FMC_HIS (numero,cle,horodate_validation,confirme_ou_signale,type_anterieur,type_FMC_valide,
	 	autoroute,PR,sens,
	 	VR,VM2,VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite,BAU_I_etroite,
	 	sit,longueur)
	 values ( @vl_evt, @vl_SiteMaitre,@vl_date,@vl_confirme_signale,@vl_type_FMC,@vl_type_FMC,
	 	@vl_autoroute_s,@vl_PR_s,@vl_sens_s,
	 	XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,
	 	XDC_VOIE_BLOQUEE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 	@vl_SiteMaitre,@vl_longueur)
	else
	 insert EXP..FMC_HIS (numero,cle,horodate_validation,confirme_ou_signale,type_anterieur,type_FMC_valide,
	 	autoroute,PR,sens,
	 	VR,VM2,VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite,BAU_I_etroite,
	 	sit,longueur)
	 values ( @vl_evt, @vl_SiteMaitre,@vl_date,@vl_confirme_signale,@vl_type_FMC,@vl_type_FMC,
	 	@vl_autoroute_s,@vl_PR_s,@vl_sens_s,
	 	XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 	XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 	@vl_SiteMaitre,@vl_longueur)
	 
	 insert EXP..FMC_COM
	 select @vl_evt, @vl_SiteMaitre,morceau,@vl_date,null,texte,@vl_SiteMaitre
	 from EXP..ADA_TXT_SYN
	 where emetteur=@vl_emetteur and reference=@vl_reference
	 
	/*B Mise a jour des commentaires */
	select @vl_morceau=isnull(max(numero),0) from EXP..FMC_COM where evenement=@vl_evt and cle=@vl_SiteMaitre
	if ( @vl_texte1 != null and @vl_texte1 != "" )
		insert EXP..FMC_COM
			(evenement,cle,numero,horodate_validation,texte,sit)
		values (@vl_evt,@vl_SiteMaitre,@vl_morceau+1,@vl_date,@vl_texte1,@vl_SiteMaitre)
	if ( @vl_texte2 != null and @vl_texte2 != "" )
		insert EXP..FMC_COM
			(evenement,cle,numero,horodate_validation,texte,sit)
		values (@vl_evt,@vl_SiteMaitre,@vl_morceau+2,@vl_date,@vl_texte2,@vl_SiteMaitre)
	 
	 insert EXP..FMC_DAT (numero,cle,gravite,trafic,bloquant,duree,sit)
	 values (@vl_evt, @vl_SiteMaitre,@vl_gravite,@vl_niveau_trafic,@vl_bloquant,@vl_duree,@vl_SiteMaitre)
	 
	 if  @vl_type_FMC = XZAEC_FMC_Accident 
	 begin
	 	insert EXP..FMC_ACC (numero,cle,nombre_de_vl,nombre_de_pl,nombre_de_remorques,
	 		nombre_de_car,nombre_de_moto,vehicule_ESCOTA,matieres_dangereuses,blesses_legers,
	 		blesses_graves,morts,personnel_ESCOTA,personnalites,degats_domaine,sit,validation)
		values ( @vl_evt, @vl_SiteMaitre,@vl_vl,@vl_pl,@vl_caravanes,@vl_cars,0,@vl_personnalites,
			@vl_matieres_dangereuses,0,@vl_blesses_graves,@vl_morts,@vl_personnalites,@vl_personnalites,
			XDC_FAUX,@vl_SiteMaitre, @vl_date)

	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_QueueBouchon
	 begin
		/*A calcul du numero de la fiche main courante a creer */
		select @vl_evt_tete = isnull(max(numero), 0) + 1 from EXP..FMC_GEN
        	where cle = @vl_SiteMaitre

	 
        	begin tran
                	/*B Valuer les degats a 0 si fmc concernee pas ce champ sinon null */
                	select @vl_degats = null

                	/*B insertion de la partie generique */
                	/*! ne pas inserer de chaine null    */
                	insert EXP..FMC_GEN (numero, cle, type, sit, poste_enrichisseur,
                        origine_creation, debut, debut_prevu, fin, fin_prevue, 
                        degats_domaine, fausse_alerte, zz,degrade,datex,
                        alerte)
                	values ( @vl_evt_tete, @vl_SiteMaitre,
                        XZAEC_FMC_TeteBouchon, @vl_SiteMaitre,
                        null, XDC_CHAINE_VIDE,
                        @vl_debut, null, @vl_fin,@vl_fin_prevue,
                        @vl_degats, @vl_fausse_alerte, 1, XDC_FAUX,XDC_VRAI,
                        @va_alerte_in)

                if @@rowcount != 1
                        return XDC_NOK
        	commit tran
        	
        	if @vl_bloquant=XDC_VRAI
	 	 insert EXP..FMC_HIS (numero,cle,horodate_validation,confirme_ou_signale,type_anterieur,type_FMC_valide,
	 		autoroute,PR,sens,
	 		VR,VM2,VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite,BAU_I_etroite,
	 		sit,longueur)
		 values ( @vl_evt_tete, @vl_SiteMaitre,@vl_date,@vl_confirme_signale,XZAEC_FMC_TeteBouchon,XZAEC_FMC_TeteBouchon,
	 		@vl_autoroute_p,@vl_PR_p,@vl_sens_p,
	 		XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_BLOQUEE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 		XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 		@vl_SiteMaitre,@vl_longueur)
		else
	 	 insert EXP..FMC_HIS (numero,cle,horodate_validation,confirme_ou_signale,type_anterieur,type_FMC_valide,
	 		autoroute,PR,sens,
	 		VR,VM2,VM1,VL,BAU,VR_I,VM2_I,VM1_I,VL_I,BAU_I,BAU_etroite,BAU_I_etroite,
	 		sit,longueur)
		 values ( @vl_evt_tete, @vl_SiteMaitre,@vl_date,@vl_confirme_signale,XZAEC_FMC_TeteBouchon,XZAEC_FMC_TeteBouchon,
	 		@vl_autoroute_p,@vl_PR_p,@vl_sens_p,
	 		XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 		XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,XDC_VOIE_INEXISTANTE,
	 		@vl_SiteMaitre,@vl_longueur)
		
		update EXP..FMC_GEN set cause=@vl_evt_tete,cle_cause=@vl_SiteMaitre,zz=zz+1
		where numero=@vl_evt and cle=@vl_SiteMaitre
		
	 	insert EXP..FMC_BOU (numero,cle,horodate,type,sit)
		values ( @vl_evt, @vl_SiteMaitre,@vl_date,@vl_type_bouchon,@vl_SiteMaitre)
                        
	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_Travaux
	 begin
		insert EXP..FMC_TRF (numero,cle,nature,type,continu,mobile,sit,zz)
		values( @vl_evt, @vl_SiteMaitre,XDC_FAUX,null,XDC_FAUX,XDC_FAUX,@vl_SiteMaitre,1)
		insert EXP..FMC_TRH (numero,cle,horodate,localisation_tete,voies_retrecies,sit)
		values( @vl_evt, @vl_SiteMaitre,@vl_date,@vl_PR_p,XDC_FAUX,@vl_SiteMaitre)
	 end
	 else  if (@vl_type_FMC = XZAEC_FMC_Meteo or
	           @vl_type_FMC = XZAEC_FMC_ChausseeGliss)
	 begin
		insert EXP..FMC_MET (numero,cle,horodate,PR,visibilite,type_de_perturbation,sit)
		values ( @vl_evt, @vl_SiteMaitre,@vl_date,@vl_PR_p,@vl_visibilite,@vl_type_perturbation,@vl_SiteMaitre)
	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_Manifestation
	 begin
	 	insert EXP..FMC_MNF (numero,cle,perception_peage,violence,sit,zz,validation)
		values( @vl_evt, @vl_SiteMaitre,XDC_FAUX,XDC_FAUX,@vl_SiteMaitre,1,@vl_date)
	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_Deviation
	 begin
	 	insert EXP..FMC_DEL (numero,cle,sit,zz)
		values( @vl_evt, @vl_SiteMaitre,@vl_SiteMaitre,1)
	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_Obstacle
	 begin
	 	select @vl_code = code from CFG..LIB_PRE
		where notyp = XDC_TYPE_LIB_OBSTACLE and rtrim (libelle) = null

	 	insert EXP..FMC_NAT (numero,cle,horodate,nature,code,sit)
		values (@vl_evt, @vl_SiteMaitre, @vl_date, XDC_CHAINE_VIDE, @vl_code, @vl_SiteMaitre)
	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_PietonChaussee
	 begin
	 	select @vl_code = code from CFG..LIB_PRE
		where notyp = XDC_TYPE_LIB_PIETON and rtrim (libelle) = null

	 	insert EXP..FMC_NAT (numero,cle,horodate,nature,code,sit)
		values (@vl_evt, @vl_SiteMaitre, @vl_date, XDC_CHAINE_VIDE, @vl_code, @vl_SiteMaitre)
	 end
	 else  if  @vl_type_FMC = XZAEC_FMC_Autre
	 begin
	 	select @vl_code = code from CFG..LIB_PRE
		where notyp = XDC_TYPE_LIB_AUTRE_EVT and rtrim (libelle) = null

	 	insert EXP..FMC_NAT (numero,cle,horodate,nature,code,sit)
		values (@vl_evt, @vl_SiteMaitre, @vl_date, XDC_CHAINE_VIDE, @vl_code, @vl_SiteMaitre)
	 end
	 
	 
	insert EXP..FMC_TRT (evenement,cle,sit,zz)
	values  (@vl_evt, @vl_SiteMaitre,@vl_SiteMaitre,NULL)

	/*B Envoi du reveil sur FMC, marque urgent selon le type d'evenement */
	select @vl_TexteReveil = XDC_REV_FMC_CRE
	if @vl_type_FMC in (XZAEC_FMC_Accident, XZAEC_FMC_Contresens)
		select @vl_TexteReveil = XDC_REV_URGENT + @vl_TexteReveil

        declare postes cursor for
        select  poste_enrichisseur 
        from EXP..FMC_GEN 
        where   type=XZAEC_FMC_PosteOperateur and cle=@vl_SiteMaitre and
                fin=null
                        
        open postes
        fetch postes into @vl_poste
        while (@@sqlstatus = 0)
        begin
             insert EXP..FMC_REV
                  ( evenement, cle, machine_operateur, texte, sit )
             values ( @vl_evt, @vl_SiteMaitre,  @vl_poste, @vl_TexteReveil,  @vl_SiteMaitre)
             fetch  postes into @vl_poste
        end
        close postes
        deallocate cursor postes
	
	update EXP..ADA_SIT
	set evenement=@vl_evt, cle=@vl_SiteMaitre
	from EXP..ADA_SIT
	where alerte=@va_alerte_in
	
	update EXP..ALT_EXP
   	set qualification    = XDC_ALT_VRAIE_EVT,
        evenement        = @vl_evt,
        cle              = @vl_SiteMaitre
   	where numero = @va_alerte_in and sit=@vl_SiteMaitre


	select @va_FMC_out=@vl_evt
	
	return XDC_OK
go

