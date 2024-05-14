/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap26sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	02/12/01 : creation
* JMG	27/04/04 : ajout extinction PAL 1.2
* hilmarcher 28/06/04: modif nom scenario pour extinction: M_SCEN11 devient M_SC11 v1.3
* JMG	06/12/04 : plus de gestion des PAL (cf drol_pal.c) 1.4
* JMG	08/03/05 : le filtre se fait sur sit_origine 1.5
* PNI	21/10/05	: Suppression de la condition sur fin de fmc_gen pour resoudre momentanement FI 5 de Nice v1.6 DEM524
* JMG	25/10/05	: ajout PMVA BAF BAD
* JMG	14/11/05	: ajout extinction automatique des NAV (si scenario AL%)
* LCL   09/07/07        : Suppression DY
* PNI	25/09/08	: Suppression de la proposition d'extinction BRA pour viter l'envoi du scenario de repli global v1.13 DEM813
* PNI	30/09/08	: Ajout des propositions d'extinction presignal , liberation biseau + extinction presignal suivant les scenarii en cours v1.16 DEM813
* JMG           02/10/09        : secto DEM887 1.17
* JMG		06/02/12	: tdp etendu 1.18 DEM/1014
* VR	05/03/12	: ajout PRV (DEM/1016)
* JMG	10/07/12 : bandeau rappel sur extinction PRV 1.21
* PNI	30/10/13 : proposition d'extinction eqt secteur au CI 1.22 1.23 DEM1075
* NDE   03/08/15 : passage au type d'affichage par defaut plutot que XDC_PMV_HEURE
* JMG   01/09/15 : 130 rappel DEM1131 1.25
* JMG	20/02/17 : ajout SAGA 1.26 DEM1191
* JMG	19/09/17 : convergence 1.27 1.28
* LCL 	16/04/18 : CTRL_FEUX 1.29 DEM1284
* JMG	12/06/18 : extinction auto fmc GABEB 1.30
* JMG	19/11/19 : EQT_FIL DEM1347 1.31
* LCL   22/04/20 : MOVIS Ajout site local pilotage DEM-SAE93
* ABE	11/05/21 : Ajout RAU SONO DEM-SAE244 1.36
* ABE	15/07/21 : Modif pour drol IMU DEM-SAE244 1.37
* PNI	22/09/21 : Pas de proposition de fin pour TUNNEL si sequence 101 (Fermeture LB)
* LCL	17/05/23 : Pilotage automatique en fin de FMC Bouchon Cockpit
* LCL	31/05/23 : Correction Pilotage automatique en fin de FMC Bouchon COCKPIT / CI
* LCL 	23/08/23 : Correction pilotage en fin des equipements
* GGY	26/09/23 : Ajout district CA (DEM483)
* LCL	27/09/23 : Adaptations CNA DEM-483	
* GGY	29/11/23 : Ajout Flash XZAP02 (DEM573)
* PNI   11/10/23 : Suppression du filtrage sur seq=101 pour la GTC, inutile après separation LB&BR  SAE-461
* GGY	29/11/23 : Ajout Flash XZAP02 (DEM573)
* JPL   07/12/23 : Purge prealable de EQT_FIL de toute action en attente si la FMC est terminee  SAE-540
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP26' and type = 'P')
	drop procedure XZAP26
go
/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  proposition des extinctions d'equipements
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
/* A voir avec JMG : Pourquoi la condition heure_succes < @va_horodate_in?*/
/* Pourquoi SAD_PMV est-elle utilisee pour l'extinction des BRA et des tubes ? */
/* J'ai remplace SAD_PMV resp par SAD_NAV et SAD_TUB */
/* J'ai rajoutte une condition sur fin de fmc_gen pour resoudre momentanement dem nice 5 */

create procedure XZAP26

	@va_num_pa_in		smallint,
	@va_horodate_in		datetime,
	@va_numero_evt_in	int,
	@va_cle_evt_in		tinyint,
        @va_NomSiteLocal_in             char(2)         = null
	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------*/

as

declare @vl_eqt 	smallint,
	@vl_sit		tinyint,
	@vl_ordre	smallint,
	@vl_ordre_max	smallint,
	@vl_fin         datetime,
	@vl_date	datetime,
	@vl_num_pa	smallint,
	@vl_site 	tinyint,
	@vl_action    int	,
	@vl_type	tinyint,
	@vl_vitesse	smallint,
	@vl_vitesse_texte char(6),
	@vl_bandeau char(10),
	@vl_nb int,
	@vl_type_fmc	smallint,
	@vl_rappel_nominal bit,@vl_priorite smallint,
	@vl_statut int,
	@vl_origine char(100),
	@vl_num_fil int,
	@vl_cle_fil tinyint,
	@vl_premiere_ligne char(18),
	@vl_deuxieme_ligne char(18),
	@vl_troisieme_ligne char(18),
	@vl_quatrieme_ligne char(18),
	@vl_cinquieme_ligne char(18),
	@vl_premier_alternat char(18),
	@vl_deuxieme_alternat char(18),
	@vl_troisieme_alternat char(18),
	@vl_quatrieme_alternat char(18),
	@vl_cinquieme_alternat char(18),
	@vl_picto char(4),
	@vl_flash tinyint,
	@vl_libelle T_NOM,
	@vl_remarque char(50),
	@vl_explication char(250),	
	@vl_destination smallint,
	@vl_destination_alternat smallint,
	@vl_destination_noeud smallint,
	@vl_destination_un_proche smallint,
	@vl_destination_un_lointaine smallint,
	@vl_destination_deux_proche smallint,
	@vl_destination_deux_lointaine smallint,
	@vl_clignotement int,
	@vl_autoroute tinyint,
	@vl_PR_debut int,
	@vl_PR_fin int,
	@vl_sens tinyint, 
	@vl_SiteMaitre tinyint

	/* Verification du site */
	select  @vl_SiteMaitre = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

/*
select @vl_site = numero from CFG..RES_DIS
where serveur = @@servername
*/

/* Determiner le site local */
/* MOVIS */
if @va_NomSiteLocal_in = null
        select  @vl_site = numero
        from CFG..RES_DIS
        where serveur = @@servername
else
        select  @vl_site = numero
        from CFG..RES_DIS
        where code = @va_NomSiteLocal_in

select @vl_fin=fin, @vl_origine = origine_creation, @vl_type_fmc = type from EXP..FMC_GEN where numero=@va_numero_evt_in and cle=@va_cle_evt_in 
/*if (@vl_fin is null ) return XDC_OK*/

/*si evt fini supprimer de EQT_FIL*/
/* SAE-540 Plus generalement, purge de toute action sur tout equipement si l'evenement est termine */

delete from EXP..EQT_FIL
where not exists ( select 1 from EXP..FMC_GEN
                      where numero = EXP..EQT_FIL.evenement  and  cle = EXP..EQT_FIL.cle
                      and fin is null )


/* si plus de PMV eligible supprimer de EQT_FIL*/

select @vl_nb = count(*) from EXP..SAD_PMV
			where numero = @va_num_pa_in
			and district =@vl_site
if @vl_nb = 0
begin
	delete from EXP..EQT_FIL
	where evenement = @va_numero_evt_in
	and cle = @va_cle_evt_in
	and type_equipement = XDC_EQT_PMV
end


/* si plus de PMVA eligible supprimer de EQT_FIL*/

select @vl_nb = count(*) from EXP..SAD_PMA
			where numero = @va_num_pa_in
			and district =@vl_site
if @vl_nb = 0
begin
	delete from EXP..EQT_FIL
	where evenement = @va_numero_evt_in
	and cle = @va_cle_evt_in
	and type_equipement = XDC_EQT_PMVA
end


select @vl_ordre_max = max(ordre)
from EXP..SAD_PMV
where numero = @va_num_pa_in
and district =@vl_site
if @@rowcount = 0
	select @vl_ordre_max = 0
	
declare pmv cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
  	and type=XDC_EQT_PMV
  	and  heure_fin is null
/*  and heure_succes >= @va_horodate_in*/
  	and heure_succes is not null         
  	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
  	and equipement not in (select equipement from EXP..SAD_PMV
			where numero = @va_num_pa_in
			and district = EXP..ACT_GEN.sit)

open pmv 

fetch pmv into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	if (@vl_type_fmc != XZAEC_FMC_Regulation and @vl_origine != "DETECTION BOUCHON" and @vl_origine not like "PIT%") or (@vl_Site = @vl_SiteMaitre)
	begin
		/*regarder si on a une FMC en attente dans EQT_FIL*/
		select @vl_num_fil = 0
		set rowcount 1
		select @vl_num_fil = f.evenement,
				@vl_cle_fil = f.cle,
				@vl_num_pa = p.numero
		from EXP..EQT_FIL f, EXP..SAD_PLN p, EXP..SAD_PMV m
		where f.type_equipement = XDC_EQT_PMV
		and f.equipement = @vl_eqt
		and p.numero = m.numero
		and f.evenement = p.evenement 
		and f.cle = p.cle
		and ( f.evenement <> @va_numero_evt_in  or f.cle <>@va_cle_evt_in)
                and p.numero in (select numero from EXP..SAD_PLN
                        where evenement = p.evenement and cle =p.cle having numero = max(numero))
                order by m.priorite , p.numero desc

		/*si oui, on propose cette FMC en pilotage*/
		/*on recupere les infos de SAD_PMV*/
		if @@rowcount !=0
		begin
			set rowcount 0
			select @vl_num_fil, @vl_cle_fil,                         @vl_num_pa
			select @vl_priorite = priorite,
				@vl_premiere_ligne = premiere_ligne,
			@vl_deuxieme_ligne = deuxieme_ligne,
			@vl_troisieme_ligne = troisieme_ligne,
			@vl_premier_alternat = premier_alternat,
			@vl_deuxieme_alternat = deuxieme_alternat,
			@vl_troisieme_alternat = troisieme_alternat,
			@vl_picto = picto, @vl_bandeau = bandeau,
			@vl_libelle = libelle,
			@vl_destination = destination,
			@vl_destination_alternat = destination_alternat,
			@vl_destination_noeud = destination_noeud,
			@vl_clignotement = clignotement,
			@vl_explication = explication
			from EXP..SAD_PMV
			where numero = @vl_num_pa
			and equipement =@vl_eqt
			and libelle != "EXTINCTION"

			/*pilotage*/
			if @@rowcount !=0
			begin
				select @vl_ordre_max = @vl_ordre_max +1
				select @vl_remarque = "FMC " + convert (char(6),@vl_num_fil) + "/" +
							convert (char(2),@vl_cle_fil) + " : " +
							@vl_libelle
				exec PRC..XZAP02       @va_num_pa_in,
						@vl_ordre,
						@vl_sit,
						"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_eqt,
						@vl_remarque,
						XDC_CHAINE_VIDE, --ligne 1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --ligne3
						XDC_CHAINE_VIDE, --alter1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --alter3
						"EXTINCTION",
						XDC_CHAINE_VIDE, --texte picto
						XDC_PICTO_PASPICTO,
						null,
						null,
						null,
						XDC_VRAI,
						0,
						0
			end
			else /*on n a rien trouve dans SAD_PMV bizarre mais au cas ou*/
			begin
				set rowcount 0
				/* sinon extinction*/
				exec PRC..XZAP02       @va_num_pa_in,
						@vl_ordre,
						@vl_sit,
						"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_eqt,
						"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
						XDC_CHAINE_VIDE, --ligne 1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --ligne3
						XDC_CHAINE_VIDE, --alter1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --alter3
						"EXTINCTION",
						XDC_CHAINE_VIDE, --texte picto
						XDC_PICTO_PASPICTO,
						null,
						null,
						null,
						XDC_VRAI,
						0,
						0
			end
  		end
		else /* pas de FMC en file d attente*/
		begin
			set rowcount 0
			/* sinon extinction*/
			exec PRC..XZAP02       @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
					XDC_CHAINE_VIDE, --ligne 1
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --ligne3
					XDC_CHAINE_VIDE, --alter1
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --alter3
					"EXTINCTION",
					XDC_CHAINE_VIDE, --texte picto
					XDC_PICTO_PASPICTO,
					null,
					null,
					null,
					XDC_VRAI,
					0,
					0
		end
	end
	else
	begin
		exec @vl_statut= PRC..XZAC610   @vl_eqt,
										@vl_sit,
										XDC_OUI,
										"",
										"",
										"",
										XDC_DTP_AUCUNE,
										XDC_DTP_AUCUNE,
										XDC_DTP_AUCUNE,
										@vl_action out,
										@vl_type out,
										XDC_NON,
										@va_NomSiteLocal_in,
										XDC_NON
	end

	fetch pmv into @vl_eqt, @vl_sit
end

select @vl_ordre_max = max(ordre)
from EXP..SAD_PMA
where numero = @va_num_pa_in
and district =@vl_site
if @@rowcount = 0
	select @vl_ordre_max = 0

declare pmva cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_PMVA
	and  heure_fin is null
/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null         
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_PMA
			where numero = @va_num_pa_in
			and district = EXP..ACT_GEN.sit)

open pmva

fetch pmva into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	if (@vl_type_fmc != XZAEC_FMC_Regulation and @vl_origine != "DETECTION BOUCHON" and @vl_origine not like "PIT%") or (@vl_Site = @vl_SiteMaitre)
	begin
		/*regarder si on a une FMC en attente dans EQT_FIL*/
		select @vl_num_fil = 0
		set rowcount 1
		select @vl_num_fil = f.evenement,
			   @vl_cle_fil = f.cle,
			   @vl_num_pa = p.numero
		from EXP..EQT_FIL f, EXP..SAD_PLN p, EXP..SAD_PMV m
		where f.type_equipement = XDC_EQT_PMVA
		and f.equipement = @vl_eqt
		and p.numero = m.numero
		and f.evenement = p.evenement 
		and f.cle = p.cle
		and ( f.evenement <> @va_numero_evt_in  or f.cle <>@va_cle_evt_in)
		and p.numero in (select numero from EXP..SAD_PLN
                         where evenement = p.evenement and cle =p.cle having numero = max(numero))
                		 order by m.priorite , p.numero desc
		set rowcount 0
	
		/*si oui, on propose cette FMC en pilotage*/
		/*on recupere les infos de SAD_PMA*/
		if @@rowcount <> 0
		begin

			select @vl_priorite = priorite,
					@vl_premiere_ligne = premiere_ligne,
					@vl_deuxieme_ligne = deuxieme_ligne,
					@vl_troisieme_ligne = troisieme_ligne,
					@vl_quatrieme_ligne = quatrieme_ligne,
					@vl_cinquieme_ligne = cinquieme_ligne,
					@vl_premier_alternat = premier_alternat,
					@vl_deuxieme_alternat = deuxieme_alternat,
					@vl_troisieme_alternat = troisieme_alternat,
					@vl_quatrieme_alternat = quatrieme_alternat,
					@vl_cinquieme_alternat = cinquieme_alternat,
					@vl_libelle = libelle,
					@vl_destination_un_proche = dest_un_proche,
					@vl_destination_un_lointaine = dest_un_lointaine,
					@vl_destination_deux_proche = dest_deux_proche,
					@vl_destination_deux_lointaine = dest_deux_lointaine,
					@vl_flash = flash,
					@vl_explication = explication
			from EXP..SAD_PMA
			where numero = @vl_num_pa
			and equipement =@vl_eqt
			and libelle != "EXTINCTION"

			/*pilotage*/
			if @@rowcount <> 0
			begin
				select @vl_remarque = "FMC " + convert (char(6),@vl_num_fil) + "/" +
						convert (char(2),@vl_cle_fil) + " : " +
						@vl_libelle
				exec PRC..XZAP28       @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					@vl_remarque,
					XDC_CHAINE_VIDE, --ligne 1
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --ligne3
					XDC_CHAINE_VIDE, --ligne4
					XDC_CHAINE_VIDE, --ligne5
					XDC_CHAINE_VIDE, --alter5
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --alter3
					XDC_CHAINE_VIDE, --alter4
					XDC_CHAINE_VIDE, --alter5
					0, --flash
					null,
					null,
					null,
					null,
					"EXTINCTION",
					XDC_VRAI
			end
			else /*on n a rien trouve dans SAD_PMV bizarre mais au cas ou*/
			begin
				/* sinon extinction*/
				exec PRC..XZAP28       @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
					XDC_CHAINE_VIDE, --ligne 1
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --ligne3
					XDC_CHAINE_VIDE, --ligne4
					XDC_CHAINE_VIDE, --ligne5
					XDC_CHAINE_VIDE, --alter5
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --alter3
					XDC_CHAINE_VIDE, --alter4
					XDC_CHAINE_VIDE, --alter5
					0, --flash
					null,
					null,
					null,
					null,
					"EXTINCTION",
					XDC_VRAI
			end
		end
		else
		begin
			/* sinon extinction*/
			exec PRC..XZAP28       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"CE PMVA N'EST PLUS UTILE POUR L'EVENEMENT",
				XDC_CHAINE_VIDE, --ligne 1
				XDC_CHAINE_VIDE,
				XDC_CHAINE_VIDE, --ligne3
				XDC_CHAINE_VIDE, --ligne4
				XDC_CHAINE_VIDE, --ligne5
				XDC_CHAINE_VIDE, --alter5
				XDC_CHAINE_VIDE,
				XDC_CHAINE_VIDE, --alter3
				XDC_CHAINE_VIDE, --alter4
				XDC_CHAINE_VIDE, --alter5
				0, --flash
				null,
				null,
				null,
				null,
				"EXTINCTION",
				XDC_VRAI
		end
	end
	else
	begin
		exec @vl_statut= PRC..XZAC840   @vl_eqt,
										@vl_sit,
										XDC_OUI,
										"",
										"",
										"",
										"",
										"",
										XDC_DTP_AUCUNE,
										XDC_DTP_AUCUNE,
										XDC_DTP_AUCUNE,
										XDC_DTP_AUCUNE,
										@vl_action out,
										@vl_type out,
										XDC_NON,
										@va_NomSiteLocal_in,
										XDC_NON

	end

	fetch pmva into @vl_eqt, @vl_sit
end

declare prv cursor 
for 
	select equipement, sit, vitesse_nominale, rappel_nominal
	from EXP..ACT_GEN , CFG..EQT_PRV
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and EXP..ACT_GEN.type=XDC_EQT_PRV
	and  heure_fin is null
	and EXP..ACT_GEN.equipement = CFG..EQT_PRV.numero
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null         
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_PRV
						   where numero = @va_num_pa_in
						   and district = EXP..ACT_GEN.sit)

open prv

fetch prv into @vl_eqt, @vl_sit, @vl_vitesse, @vl_rappel_nominal

while (@@sqlstatus = 0)
begin
	if (@vl_type_fmc != XZAEC_FMC_Regulation and @vl_origine != "DETECTION BOUCHON" and @vl_origine not like "PIT%") or (@vl_Site = @vl_SiteMaitre)
	begin
		select @vl_ordre=isnull(@vl_ordre,256)-1

		select @vl_bandeau=""
		if @vl_rappel_nominal=1
			select @vl_bandeau="RAPPEL"
		if (@vl_vitesse>=100)
			select @vl_vitesse_texte = 'L'+convert(char(3),@vl_vitesse)
		else
			select @vl_vitesse_texte = 'L'+convert(char(2),@vl_vitesse)


		exec PRC..XZAP82       @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE PRV N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					"CE PRV N'EST PLUS UTILE POUR L'EVENEMENT",			  
					@vl_bandeau, -- Bandeau
					@vl_vitesse_texte, -- Vitesse
					"EXTINCTION",
					XDC_VRAI
	end
	else
	begin
		exec @vl_statut= PRC..XZAC303   @va_horodate_in,
										0,
										0,
										0,
										@vl_eqt,
										@vl_sit,
										"RAPPEL",
										"",
										0,
										@vl_site,
										@vl_action out,
										XDC_NON

	end

	fetch prv into @vl_eqt, @vl_sit, @vl_vitesse, @vl_rappel_nominal
end


if @vl_fin is not null
begin
	declare cfe cursor 
	for 
		select equipement, sit
		from EXP..ACT_GEN , CFG..EQT_CFE
		where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
		and EXP..ACT_GEN.type=XDC_EQT_CFE
		and  heure_fin is null
		and EXP..ACT_GEN.equipement = CFG..EQT_CFE.numero
		and heure_succes is not null         
		and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)

	open cfe

	fetch cfe into @vl_eqt, @vl_sit

	while (@@sqlstatus = 0)
	begin
		select @vl_ordre=isnull(@vl_ordre,256)-1

		exec PRC..XZAP83       @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE CFE N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					"CE CFE N'EST PLUS UTILE POUR L'EVENEMENT",			  
					0,
					"MODE CALENDAIRE",
					XDC_VRAI
		fetch cfe into @vl_eqt, @vl_sit
	end
end






/* SUPPRESSION DE LA PROSITION DE SUPPRESSION : EVITER L'ENVOI DE SCENARIO GLOABL DE LIBERATION DEM813*/

/***********************************************
declare nav cursor 
for 
select equipement, EXP..ACT_GEN.sit
from EXP..ACT_GEN , EXP..ACT_NAV
where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
  and type=XDC_EQT_NAV
  and  heure_fin is null
  and heure_succes is not null               
  and EXP..ACT_GEN.sit = @vl_site
  and equipement not in (select equipement from EXP..SAD_NAV
			where numero = @va_num_pa_in
			and district = EXP..ACT_GEN.sit)
  and EXP..ACT_NAV.sit=EXP..ACT_GEN.sit
  and EXP..ACT_NAV.actio=EXP..ACT_GEN.numero
  and EXP..ACT_NAV.scenario not like 'AL%'
open nav 

fetch nav into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
  select @vl_ordre=isnull(@vl_ordre,256)-1

  exec PRC..XZAP;03       @va_num_pa_in,
			  @vl_ordre,
			  @vl_sit,
			  "CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
			  0,
			  @vl_eqt,
			  "CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
			  "LIBERATION",
			  XDC_NAV_RENTREE,
			  XDC_VRAI
fetch nav into @vl_eqt, @vl_sit
end

************************************************/



/**** extinction presignal  *****/
declare nav0 cursor 
for 
	select equipement, EXP..ACT_GEN.sit
	from EXP..ACT_GEN , EXP..ACT_NAV
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_NAV
	and  heure_fin is null
	and heure_succes is not null               
	and (EXP..ACT_GEN.sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_NAV
				where numero = @va_num_pa_in
				and district = EXP..ACT_GEN.sit)
	and EXP..ACT_NAV.sit=EXP..ACT_GEN.sit
	and EXP..ACT_NAV.actio=EXP..ACT_GEN.numero
	and (EXP..ACT_NAV.scenario like 'PV%' OR EXP..ACT_NAV.scenario like 'PRV%')
open nav0 

fetch nav0 into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP;03       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				"EXTINCTION",
				"APS1",
				XDC_VRAI
	fetch nav0 into @vl_eqt, @vl_sit
end


/**** Libration + extinction VR *****/
declare nav1r cursor 
for 
	select equipement, EXP..ACT_GEN.sit
	from EXP..ACT_GEN , EXP..ACT_NAV
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_NAV
	and  heure_fin is null
	and heure_succes is not null               
	and EXP..ACT_GEN.sit = @vl_site
	and equipement not in (select equipement from EXP..SAD_NAV
			where numero = @va_num_pa_in
			and district = EXP..ACT_GEN.sit)
	and EXP..ACT_NAV.sit=EXP..ACT_GEN.sit
	and EXP..ACT_NAV.actio=EXP..ACT_GEN.numero
	and (EXP..ACT_NAV.scenario like '%NVR%' )
open nav1r 

fetch nav1r into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP;03       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"EXTINCTION SIGNALISATION BRA",
				"EXTINCTION",
				"APS1",
				XDC_VRAI
	select @vl_ordre=isnull(@vl_ordre,256)-1
	exec PRC..XZAP;03       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"LIBERATION BISEAUX BRA",
				"LIBERATION",
				"LVR1",
				XDC_VRAI
	
	fetch nav1r into @vl_eqt, @vl_sit
end


/**** Libration + extinction VL *****/
declare nav1l cursor 
for 
	select equipement, EXP..ACT_GEN.sit
	from EXP..ACT_GEN , EXP..ACT_NAV
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_NAV
	and  heure_fin is null
	and heure_succes is not null               
	and EXP..ACT_GEN.sit = @vl_site
	and equipement not in (select equipement from EXP..SAD_NAV
			where numero = @va_num_pa_in
			and district = EXP..ACT_GEN.sit)
	and EXP..ACT_NAV.sit=EXP..ACT_GEN.sit
	and EXP..ACT_NAV.actio=EXP..ACT_GEN.numero
	and (EXP..ACT_NAV.scenario like '%NVL%' )
open nav1l

fetch nav1l into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP;03       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				"LIBERATION",
				"LIVL",
				XDC_VRAI
	select @vl_ordre=isnull(@vl_ordre,256)-1
	exec PRC..XZAP;03       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
				"EXTINCTION",
				"APS1",
				XDC_VRAI
	fetch nav1l into @vl_eqt, @vl_sit
end



declare nav2 cursor 
for 
	select equipement, EXP..ACT_GEN.sit
	from EXP..ACT_GEN , EXP..ACT_NAV
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_NAV
	and  heure_fin is null
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null               
	and EXP..ACT_GEN.sit = @vl_site
	and equipement not in (select equipement from EXP..SAD_NAV
				where numero = @va_num_pa_in
				and district = EXP..ACT_GEN.sit)
	and EXP..ACT_NAV.sit=EXP..ACT_GEN.sit
	and EXP..ACT_NAV.actio=EXP..ACT_GEN.numero
	and EXP..ACT_NAV.scenario like 'AL%'
open nav2

fetch nav2 into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	if ((@vl_Site = @vl_SiteMaitre) and (@vl_sit=XDC_DP))
	begin
		select @vl_ordre=isnull(@vl_ordre,256)-1

		select @vl_date=getdate()
		exec PRC..XZAC;62       @vl_eqt,
				@vl_sit,
				@vl_date,
				0,
				"AL11",
				@vl_action output,
				XDC_NOM_SITE_DP
	end
	else if (@vl_Site != @vl_SiteMaitre)
	begin
		select @vl_ordre=isnull(@vl_ordre,256)-1

		select @vl_date=getdate()
		exec PRC..XZAC;62       @vl_eqt,
				@vl_sit,
				@vl_date,
				0,
				"AL11",
				@vl_action output,
				@va_NomSiteLocal_in
	end
	else
	begin
		select @vl_ordre=isnull(@vl_ordre,256)-1

		exec PRC..XZAP;03       @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					"CE BRA N'EST PLUS UTILE POUR L'EVENEMENT",
					"LIBERATION",
					XDC_NAV_RENTREE,
					XDC_VRAI
	end
	fetch nav2 into @vl_eqt, @vl_sit
end

declare baf cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_BAF
	and  heure_fin is null
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null               
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_BAF
				where numero = @va_num_pa_in
				and district = EXP..ACT_GEN.sit)

open baf 

fetch baf into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1
	exec PRC..XZAP29       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"",
				0,
				@vl_eqt,
				"LIBERATION SIGNALISATION",
				"",
				0,
				0,
				0,
				0,
				0,
				0,
				XDC_VRAI

	select @vl_ordre=isnull(@vl_ordre,256)-1
	exec PRC..XZAP29       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"",
				0,
				@vl_eqt,
				"LIBERATION BARRIERE",
				"",
				1,
				1,
				0,
				1,
				1,
				0,
				XDC_VRAI

	fetch baf into @vl_eqt, @vl_sit
end

declare bad cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_BAD
	and  heure_fin is null
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null               
	and sit = @vl_site
	and equipement not in (select equipement from EXP..SAD_BAD
				where numero = @va_num_pa_in
				and district = EXP..ACT_GEN.sit)

open bad 

fetch bad into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP33       @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				"CE BAD N'EST PLUS UTILE POUR L'EVENEMENT",
				0,
				@vl_eqt,
				"CE BAD N'EST PLUS UTILE POUR L'EVENEMENT",
				"LIBERATION",
				"L_GLOVR1",
				XDC_VRAI
	fetch bad into @vl_eqt, @vl_sit
end

/*
declare pal cursor
for
select equipement, sit
from EXP..ACT_GEN
where evenement=@va_numero_evt_in and cle=@va_cle_evt_in
  and type=XDC_EQT_PAL
  and  heure_fin is null
*/
/*  and heure_succes >= @va_horodate_in*/
  /*
  and heure_succes is not null
  and sit = @vl_site
  and equipement not in (select equipement from EXP..SAD_PAL
                        where numero = @va_num_pa_in
                        and district = EXP..ACT_GEN.sit)

open pal

fetch pal into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
  select @vl_ordre=isnull(@vl_ordre,256)-1

	if @vl_type_fmc != XZAEC_FMC_Regulation and @vl_origine != "DETECTION BOUCHON"
  begin
  exec PRC..XZAP27       @va_num_pa_in,
                          @vl_ordre,
                          @vl_sit,
                          "CE PANNEAU N'EST PLUS UTILE POUR L'EVENEMENT",
                          0,
                          @vl_eqt,
                          "CE PANNEAU N'EST PLUS UTILE POUR L'EVENEMENT",
                          "LIBERATION",
                          "M_SC11",
                          XDC_VRAI
end
else
begin

	select @vl_date=getdate()
	exec PRC..XZAC;67 @vl_eqt,
			@vl_sit,
			@vl_date,
			XDC_VRAI,
			"M_SC11",
			"M_SC11",
			@vl_action output,
			@va_NomSiteLocal_in
end
fetch pal into @vl_eqt, @vl_sit
end
*/


declare tun cursor 
for 
	select A.equipement, A.sit, A.type
	from EXP..ACT_GEN A
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in
	and A.type in (XDC_EQT_TUB,
					XDC_ACT_GTC_SIG,
					XDC_ACT_GTC_ECL,
					XDC_ACT_GTC_VEN,
					XDC_ACT_GTC_ENR,
					XDC_ACT_GTC_INC,
					XDC_ACT_GTC_EXP)
	and  (heure_fin is null or heure_echec is not null)
	and A.sit = @vl_site
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null
	and A.equipement not in (select equipement from EXP..SAD_TUB
							where numero = @va_num_pa_in
							and district = A.sit)

open tun 

fetch tun into @vl_eqt, @vl_sit, @vl_type

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP50 @va_num_pa_in,
					 @vl_ordre,								
					 @vl_sit,
					 @vl_eqt,
					 @vl_type

/*  exec PRC..XZAP;04       @va_num_pa_in,
			  @vl_ordre,
			  @vl_sit,
			  "CE TUNNEL N'EST PLUS UTILE POUR L'EVENEMENT",
			  0,
			  @vl_eqt,
			  "CE TUNNEL N'EST PLUS UTILE POUR L'EVENEMENT",
			  "EXTINCTION TUNNEL",
			  XDC_TUB_ETEINT,
			  XDC_VRAI
*/
	fetch tun into @vl_eqt, @vl_sit, @vl_type
end


/*PMV PMVA*/
declare saga cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_SAGA
	and  heure_fin is null
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null         
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_SAGA
						   where numero = @va_num_pa_in
						   and district = EXP..ACT_GEN.sit)
	and equipement not in (select numero from CFG..EQT_SAGA
						   where type_saga= XDC_SAGA_META_EQT or type_saga= XDC_SAGA_BAF or type_saga= XDC_SAGA_BRA)
open saga 

fetch saga into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP70 @va_num_pa_in,
					@vl_ordre,
					@vl_sit,
					"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
					0,
					@vl_eqt,
					"CE PMV N'EST PLUS UTILE POUR L'EVENEMENT",
					XDC_CHAINE_VIDE, --ligne 1
					XDC_CHAINE_VIDE,
					XDC_CHAINE_VIDE, --ligne3
					XDC_CHAINE_VIDE, --ligne4
					XDC_CHAINE_VIDE, -- ligne5
					"EXTINCTION",
					XDC_CHAINE_VIDE, --texte picto
					XDC_PICTO_PASPICTO,
					0, --barriere
					0, --scenario
					null,
					null,
					null,
					XDC_VRAI
	fetch saga into @vl_eqt, @vl_sit
end


/*BAF BRA*/
declare saga2 cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_SAGA
	and  heure_fin is null
	/*  and heure_succes >= @va_horodate_in*/
	and heure_succes is not null         
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_SAGA
				where numero = @va_num_pa_in
				and district = EXP..ACT_GEN.sit)
	and equipement in (select numero from CFG..EQT_SAGA
			where type_saga= XDC_SAGA_BAF or type_saga= XDC_SAGA_BRA)
open saga2 

fetch saga2 into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP70    @va_num_pa_in,
						@vl_ordre,
						@vl_sit,
						"CE BAF N'EST PLUS UTILE POUR L'EVENEMENT",
						0,
						@vl_eqt,
						"CE BAF N'EST PLUS UTILE POUR L'EVENEMENT",
						XDC_CHAINE_VIDE, --ligne 1
						XDC_CHAINE_VIDE,
						XDC_CHAINE_VIDE, --ligne3												
						XDC_CHAINE_VIDE, --ligne4
						XDC_CHAINE_VIDE, -- ligne5
						"EXTINCTION",
						XDC_CHAINE_VIDE, --texte picto
						XDC_PICTO_PASPICTO,
						0, --barriere
						0, --scenario
						null,
						null,
						null,
						XDC_VRAI
	fetch saga2 into @vl_eqt, @vl_sit
end


/* RAU SONO */
declare SONO cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_ACT_PAU
	and  heure_fin is null
	and heure_succes is not null         
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and not exists (select equipement from EXP..SAD_SONO								
					where numero = @va_num_pa_in
					and district = EXP..ACT_GEN.sit)
	

open SONO

fetch SONO into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	select @vl_autoroute= autoroute, @vl_PR_debut=PR_debut, @vl_PR_fin=PR_fin, @vl_sens=sens from EXP..ACT_PAU P, EXP..ACT_GEN G
	where P.actio = G.numero
		and G.evenement=@va_numero_evt_in
		and G.cle=@va_cle_evt_in 
		and type=XDC_ACT_PAU
		
													
	exec PRC..XZAP35      @va_num_pa_in,
				@vl_ordre,
				@vl_sit,								
				@vl_eqt,
				0,
				"CE PAU SONO N'EST PLUS UTILE POUR L'EVENEMENT",
				"CE PAU SONO N'EST PLUS UTILE POUR L'EVENEMENT",
				XDC_VRAI,
				"REPOS",
				@vl_autoroute,
				@vl_sens,
				@vl_PR_debut,
				@vl_PR_fin,
				""
	fetch SONO into @vl_eqt, @vl_sit
end

/* IMU */
declare imu cursor 
for 
	select equipement, sit
	from EXP..ACT_GEN 
	where evenement=@va_numero_evt_in and cle=@va_cle_evt_in  
	and type=XDC_EQT_IMU
	and  heure_fin is null
	and heure_succes is not null         
	and (sit_origine = @vl_site or sit = @vl_site or @vl_Site = @vl_SiteMaitre)
	and equipement not in (select equipement from EXP..SAD_IMU
				where numero = @va_num_pa_in
				and district = EXP..ACT_GEN.sit)
open imu 

fetch imu into @vl_eqt, @vl_sit

while (@@sqlstatus = 0)
begin
	select @vl_ordre=isnull(@vl_ordre,256)-1

	exec PRC..XZAP36      @va_num_pa_in,
				@vl_ordre,
				@vl_sit,
				@vl_eqt,
				0,
				"L'IMU N'EST PLUS UTILE POUR L'EVENEMENT",
				"L'IMU N'EST PLUS UTILE POUR L'EVENEMENT",
				XDC_VRAI,
				"",
				0,
				0,
				0,
				@vl_sit,
				""

	fetch saga2 into @vl_eqt, @vl_sit
end

return XDC_OK
go
