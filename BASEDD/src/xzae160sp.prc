/*E*/
/* Fichier : $Id: xzae160sp.prc,v 1.11 2010/09/02 14:38:21 gesconf Exp $        Release : $Revision: 1.11 $        Date : $Date: 2010/09/02 14:38:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae160sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
* rechercher les evts a mettre sur le WEB
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou     16/01/01  : Creation
* Cluchague   19/03/03  : les travaux sont mentionnes a plus de 1000m + meteo pas conditionnee par visibilite
			  deviation corrigee, evts non filtres etc... (V1.2). Reste probleme deviation inter 
			  autoroutes et voies neutralisees et eventuellement detail meteo 
* Cluchague   03/03/04  : 1.3 
			  deviation corrigee, evts non filtres etc... (V1.2). Reste probleme deviation inter 
			  autoroutes et voies neutralisees et eventuellement detail meteo 
* Cluchague   13/09/2005 : v1.4 pour les deviations, on remet les troncons noirs (en + de l'icone)
* JPL		04/06/2007 : Prises en compte Fmc Vehicule en marche arriere 1.5
* JPL		04/06/2007 : Prises en compte Fmc Traitement chaussees 1.6
* JPL		05/06/2007 : Prises en compte Fmc Echangeur deconseille 1.7
* JPL		05/06/2007 : Prises en compte Fmc Chaussee glissante 1.8
* JPL		08/06/2007 : Prise en compte Fmc Basculement pour travaux 1.9
* JPL		30/10/2007 : Filtrer les FMC ayant pour origine 'Exercice' (DEM 719) 1.10
* JPL		02/09/2010 : Prises en compte Fmc Zone de stockage (DEM 942) 1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE160
* 
* Arguments en entree
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE160' and type = 'P')
	drop procedure XZAE160
go


create procedure XZAE160
as

declare @vl_numero int,
	@vl_cle tinyint,
	@vl_type smallint,
	@vl_cause int,
	@vl_cle_cause tinyint,
	@vl_cause_tete int,
	@vl_cle_cause_tete tinyint,
	@vl_PR_debut int,
	@vl_PR_fin int,
	@vl_cr int,@vl_update int,
	@vl_aut_debut tinyint,
	@vl_aut_fin tinyint,
	@vl_sens tinyint,
	@vl_sens_corrige tinyint,
	@vl_texte_web char(30),
	@vl_texte_final char(30),
	@vl_texte_ech char(30),
	@vl_txt_ech_complet char(30),
	@vl_txt_ech_1sortie char(30),
	@vl_txt_ech_1entree char(30),
	@vl_txt_ech_2sorties char(30),
	@vl_txt_ech_2entrees char(30),
	@vl_txt_ech_1entree_1sortie char(30),
	@vl_txt_ech_2entrees_2sorties char(30),
	@vl_icone char(30),
	@vl_priorite tinyint,
	@vl_nb_out smallint, @vl_perception_out bit, @vl_violence_out bit,
	@vl_sens_debut tinyint,@vl_sens_fin tinyint,
	@vl_perturbation tinyint, @vl_visibilite smallint, @vl_longueur int,
	@vl_VR tinyint, @vl_VM2 tinyint, @vl_VM1 tinyint, @vl_VL tinyint,
	@vl_VR_I tinyint, @vl_VM2_I tinyint, @vl_VM1_I tinyint, @vl_VL_I tinyint,
	@vl_entree1 bit,@vl_entree2 bit,@vl_sortie1 bit,@vl_sortie2 bit, @vl_entree bit,@vl_sortie bit,
	@vl_nature_trv bit,@vl_type_trv tinyint,@vl_continu_trv bit,@vl_debut_trv datetime, @vl_fin_trv datetime,
	@vl_mobile_trv bit,@vl_aut_trv tinyint,@vl_PRTM_trv int,@vl_PRQM_trv int,@vl_sens_trv int,
	@vl_PRT_trv int,@vl_voies_trv bit,@vl_longueur_trv int,
	@vl_horodate datetime,
	@vl_pt_car tinyint, @vl_num_pt_car tinyint,
	@vl_penurie bit, @vl_ferme bit, @vl_carburant tinyint, @vl_nb_voies tinyint,
	@vl_type_web tinyint, @vl_PR_min int, @vl_PR_max int,@vl_compte int

/*premiere recherche des FMC a exporter (avec premier filtre)*/
create table #TMP_WEB_FMC (
	numero int null,
	cle tinyint null,
	type smallint null,
	cause int null,
	cle_cause tinyint null,
	horodate_validation datetime null,
	autoroute tinyint null,
	PR int null,
	sens tinyint null,
	point_caracteristique tinyint null,
	numero_point_caracteristique tinyint null,
	VR tinyint null,
	VM2 tinyint null,
	VM1 tinyint null,
	VL tinyint null,
	BAU tinyint null,
	VR_I tinyint null,
	VM2_I tinyint null,
	VM1_I tinyint null,
	VL_I tinyint null,
	BAU_I tinyint null)

create unique index WEB_FMC_PK on #TMP_WEB_FMC (numero, cle)

create table #TMP_WEB_BOU (
	numero int null,
	cle tinyint null,
	cause int null,
	cle_cause tinyint null)

/*premiere recherche des FMC a exporter (avec premier filtre)*/
insert into #TMP_WEB_FMC
select	G.numero,
	G.cle,
	G.type,
	G.cause,
	G.cle_cause,
	H.horodate_validation,
	H.autoroute,
	H.PR,
	H.sens,
	H.point_caracteristique,
	H.numero_point_caracteristique,
	H.VR,
	H.VM2,
	H.VM1,
	H.VL,
	H.BAU,
	H.VR_I,
	H.VM2_I,
	H.VM1_I,
	H.VL_I,
	H.BAU_I

from EXP..FMC_GEN G, EXP..FMC_HIS_DER H

where  (G.type in (select type from CFG..WEB_FMC) or G.type=XZAEC_FMC_QueueBouchon or G.type=XZAEC_FMC_Deviation)
and    G.datex = XDC_FAUX
and    G.debut is not null
and    G.fin is NULL
and    G.numero = H.numero
and    G.cle = H.cle
and    H.confirme_ou_signale = 1
and    G.origine_creation not like "%ESSAI%"
and    G.origine_creation not like "%EXERCICE%"

/* curseur pour traiter bouchons et deviations */
declare filtre1 cursor
for select
	numero,
	cle,
	type,
	cause,
	cle_cause,
	sens,
	autoroute,
	PR
from #TMP_WEB_FMC
where type = XZAEC_FMC_QueueBouchon or type = XZAEC_FMC_Deviation

open filtre1
fetch filtre1 into @vl_numero,@vl_cle,@vl_type,@vl_cause,@vl_cle_cause,@vl_sens,@vl_aut_debut,@vl_PR_debut
while @@sqlstatus = 0
begin
  /*traitement des deviations*/
  if @vl_type = XZAEC_FMC_Deviation
  begin
    /*recherche PR sortie et reinjection de la deviation*/
    exec @vl_cr = PRC..XZAE161 @vl_numero,@vl_cle,
                               @vl_PR_debut output,
	                       @vl_PR_fin output
    select  @vl_sens_corrige=@vl_sens
    if      (@vl_sens=XDC_SENS_SORTIE_NORD or
	     @vl_sens=XDC_SENS_ENTREE_NORD)
      select  @vl_sens_corrige=XDC_SENS_NORD
    else if (@vl_sens=XDC_SENS_SORTIE_SUD or
	     @vl_sens=XDC_SENS_ENTREE_SUD)
      select  @vl_sens_corrige=XDC_SENS_SUD
    else if @vl_sens=XDC_SENS_INCONNU
      select @vl_sens_corrige=XDC_SENS_NORD

    /*si on a trouve*/

    if @vl_cr = XDC_OK
      exec XZAS84 @vl_aut_debut, @vl_sens_corrige,
                  @vl_PR_debut, @vl_PR_fin, XDC_WEB_NOIR
  end
   else
   begin

    /*recherche de la cause de la tete du bouchon*/
    select @vl_cause_tete=cause, @vl_cle_cause_tete=cle_cause
    from EXP..FMC_GEN
    where numero = @vl_cause
    and   cle = @vl_cle_cause

    /*recherche de la localisation de la tete de bouchon*/
    select @vl_aut_fin = autoroute,
    @vl_PR_fin = PR,
    @vl_sens_fin = sens
    from EXP..FMC_HIS_DER
    where numero = @vl_cause
    and   cle = @vl_cle_cause

    /*si on a trouve*/
    if @@rowcount <> 0
    begin
      /*mise a rouge des troncons concernes*/
      exec XZAS84 @vl_aut_debut, @vl_sens,
		@vl_PR_debut, @vl_PR_fin,
		XDC_WEB_ROUGE


      /*memorisation dans la table temporaire des bouchons*/
      insert #TMP_WEB_BOU
      values (@vl_numero,@vl_cle,@vl_cause_tete,@vl_cle_cause_tete)
    end
  end
  fetch filtre1 into @vl_numero,@vl_cle,@vl_type,@vl_cause,@vl_cle_cause,@vl_sens,@vl_aut_debut,@vl_PR_debut
end
close filtre1

/*deuxieme filtre plus precis*/
declare filtre2 cursor
for select
	numero,
	cle,
	type,
	VR,
	VM2,
	VM1,
	VL,
	VR_I,
	VM2_I,
	VM1_I,
	VL_I,
	autoroute,
	PR,
	sens,
	horodate_validation,
	point_caracteristique,
	numero_point_caracteristique

from #TMP_WEB_FMC
where type <> XZAEC_FMC_QueueBouchon and type <> XZAEC_FMC_TeteBouchon 

open filtre2

fetch filtre2 into @vl_numero,@vl_cle,@vl_type,
			@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I,
			@vl_aut_debut,@vl_PR_debut,@vl_sens_debut,@vl_horodate,
			@vl_pt_car,@vl_num_pt_car

select @vl_update = XDC_NOK
while @@sqlstatus=0
begin
  /*si manifestation*/
  if @vl_type = XZAEC_FMC_Manifestation
  begin
    /*recherche du nombre de manifestants*/
    exec @vl_cr = XZAE;47 null,@vl_numero,@vl_cle,
				@vl_nb_out output,@vl_perception_out output,@vl_violence_out output
    select @vl_PR_fin = @vl_PR_debut
    if @vl_cr <> XDC_OK select @vl_nb_out = 0

    /*ne garder que si plus de 50 manifestants*/
    if @vl_nb_out >= 50 select @vl_update = XDC_OK
  end

  /*si escargot*/
  else if @vl_type = XZAEC_FMC_Escargot
  begin
    /*recherche du nombre de manifestants*/
    exec @vl_cr = XZAE;47 null,@vl_numero,@vl_cle,
				@vl_nb_out output,@vl_perception_out output,@vl_violence_out output
    select @vl_PR_fin = @vl_PR_debut
    if @vl_cr <> XDC_OK select @vl_nb_out = 0

    /*ne garder que si plus de 10 manifestants*/
    if @vl_nb_out >= 10 select @vl_update = XDC_OK
  end

  /*accident*/
  else if @vl_type = XZAEC_FMC_Accident or
	  @vl_type = XZAEC_FMC_Delestage or
	  @vl_type = XZAEC_FMC_Deviation or
	  @vl_type = XZAEC_FMC_Contresens or
	  @vl_type = XZAEC_FMC_VehMarcheArr or
	  @vl_type = XZAEC_FMC_VehFeu or
	  @vl_type = XZAEC_FMC_Salage or
	  @vl_type = XZAEC_FMC_AireFermee or
	  @vl_type = XZAEC_FMC_IncSsConces
  begin
    select @vl_update = XDC_OK
    select @vl_PR_fin = @vl_PR_debut
  end

  /*meteo*/
  else if @vl_type = XZAEC_FMC_Meteo or
	  @vl_type = XZAEC_FMC_TraitChaussees
  begin
    /*recherche de la visibilite*/
    select @vl_visibilite=visibilite,@vl_PR_fin=PR from EXP..FMC_MET where numero=@vl_numero and cle=@vl_cle
			
    /*ne garder les evenements meteo que si visibilite > 100*/
/*    if (@vl_type <> XZAEC_FMC_Meteo  or  @vl_visibilite <= 100) */
      select @vl_update = XDC_OK
  end

  /*vehicule arrete ou en panne*/
  else if @vl_type = XZAEC_FMC_VehArrete or @vl_type = XZAEC_FMC_VehPanne
  begin
    /*ne garder que si sur section courante*/
    if @vl_VR=XDC_VOIE_BLOQUEE or @vl_VR=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VR=XDC_VOIE_NEUTRALISEE or
       @vl_VM2=XDC_VOIE_BLOQUEE or @vl_VM2=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM2=XDC_VOIE_NEUTRALISEE or
       @vl_VM1=XDC_VOIE_BLOQUEE or @vl_VM1=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM1=XDC_VOIE_NEUTRALISEE or
       @vl_VL=XDC_VOIE_BLOQUEE or @vl_VL=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VL=XDC_VOIE_NEUTRALISEE
       select @vl_update = XDC_OK 
    select @vl_PR_fin = @vl_PR_debut
  end

  /*fermeture echangeur*/
  else if @vl_type = XZAEC_FMC_FermetureEch or
          @vl_type = XZAEC_FMC_EchDeconseille
  begin
    /*recuperer les voies fermees*/
    exec @vl_cr = XZAE;52 null,@vl_numero,@vl_cle,
			@vl_entree1 output,@vl_entree2 output,@vl_sortie1 output,@vl_sortie2 output

    /*les differents textes selon le type d'evenement et les bretelles concernees*/
    if @vl_type = XZAEC_FMC_FermetureEch
    begin
      select @vl_txt_ech_complet = "Fermeture echangeur"
      select @vl_txt_ech_1sortie = "Sortie fermee"
      select @vl_txt_ech_1entree = "Entree fermee"
      select @vl_txt_ech_2sorties = "Sorties fermees"
      select @vl_txt_ech_2entrees = "Entrees fermees"
      select @vl_txt_ech_1entree_1sortie = "Ent & sortie fermee"
      select @vl_txt_ech_2entrees_2sorties = "Ent & sorties fermees"
    end
    else
    begin
      select @vl_txt_ech_complet = "Echangeur deconseille"
      select @vl_txt_ech_1sortie = "Sortie deconseillee"
      select @vl_txt_ech_1entree = "Entree deconseillee"
      select @vl_txt_ech_2sorties = "Sorties deconseillees"
      select @vl_txt_ech_2entrees = "Entrees deconseillees"
      select @vl_txt_ech_1entree_1sortie = "Ent & sortie deconseillee"
      select @vl_txt_ech_2entrees_2sorties = "Ent & sorties deconseillees"
    end

    /*choisir le texte selon les entrees et/ou sorties concernees*/
         if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_NON and @vl_entree2=XDC_NON and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_NON and @vl_entree2=XDC_NON and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_1sortie
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_NON and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_1entree
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_NON and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_1entree_1sortie
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_NON and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_1sortie
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_NON and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_2sorties
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_NON and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_NON and @vl_entree2=XDC_NON and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_1entree
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_NON and @vl_entree2=XDC_NON and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_NON and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_2entrees
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_NON and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_NON and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_1entree_1sortie
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_NON and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_NON) 
		     select @vl_texte_ech = @vl_txt_ech_complet
    else if (@vl_entree1=XDC_OUI and @vl_sortie1=XDC_OUI and @vl_entree2=XDC_OUI and @vl_sortie2=XDC_OUI) 
		     select @vl_texte_ech = @vl_txt_ech_2entrees_2sorties
   
    select @vl_PR_fin = @vl_PR_debut
    select @vl_update = XDC_OK 
  end

  else if @vl_type = XZAEC_FMC_Eboulement or
	  @vl_type = XZAEC_FMC_Obstacle or
	  @vl_type = XZAEC_FMC_FlaqueHuile or
	  @vl_type = XZAEC_FMC_ChausseeGliss
  begin
    /*ne garder que si sur section courante*/
    if @vl_VR=XDC_VOIE_BLOQUEE or @vl_VR=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VR=XDC_VOIE_NEUTRALISEE or
       @vl_VM2=XDC_VOIE_BLOQUEE or @vl_VM2=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM2=XDC_VOIE_NEUTRALISEE or
       @vl_VM1=XDC_VOIE_BLOQUEE or @vl_VM1=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM1=XDC_VOIE_NEUTRALISEE or
       @vl_VL=XDC_VOIE_BLOQUEE or @vl_VL=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VL=XDC_VOIE_NEUTRALISEE
       select @vl_update = XDC_OK
       else select @vl_update = XDC_NOK

    /*sinon regarder s'il existe un bouchon consequence*/
    if @vl_update = XDC_NOK
    begin
      select @vl_compte=count(*) from #TMP_WEB_BOU where cause = @vl_numero and cle_cause = @vl_cle
      if @vl_compte > 0 select @vl_update = XDC_OK
    end
  
    select @vl_PR_fin = @vl_PR_debut
    if @vl_update = XDC_OK
    begin
      if @vl_type = XZAEC_FMC_ChausseeGliss
        select @vl_PR_fin=PR from EXP..FMC_MET where numero=@vl_numero and cle=@vl_cle
    end
  end

  else if @vl_type = XZAEC_FMC_Incendie or
	  @vl_type = XZAEC_FMC_Inondation or 
	  @vl_type = XZAEC_FMC_AnimalErrant or
	  @vl_type = XZAEC_FMC_PietonChaussee
  begin
    /* garder quelquesoit l'emplacement */
    select @vl_update = XDC_OK
    select @vl_PR_fin = @vl_PR_debut
  end

  else if @vl_type = XZAEC_FMC_Travaux or
          @vl_type = XZAEC_FMC_ZoneStockage or
          @vl_type = XZAEC_FMC_BasculTravaux
  begin
    /*rechercher la longueur des travaux*/
    exec @vl_cr = XZAE;49 null, @vl_numero, @vl_cle,
		@vl_nature_trv output, @vl_type_trv output,@vl_continu_trv output,@vl_debut_trv output,@vl_fin_trv output,
		@vl_mobile_trv output,@vl_aut_trv output,@vl_PRTM_trv output,@vl_PRQM_trv output,@vl_sens_trv output,
		@vl_PRT_trv output,@vl_voies_trv output,@vl_longueur_trv output

    if @vl_cr <> XDC_OK select @vl_longueur_trv = 0
    select @vl_PR_fin = @vl_PRT_trv
    if @vl_longueur_trv >= 1000 select @vl_update=XDC_OK
  end
  
  /*si on doit garder l'evenement*/
  if @vl_update = XDC_OK
  begin
    /*recuperation des champs a utiliser pour le WEB*/
    select @vl_texte_web = nature,
		@vl_icone = icone,
		@vl_priorite = priorite,
		@vl_type_web = type_web
    from CFG..WEB_FMC 
    where type = @vl_type

    /*forcer le texte si fermeture echangeur*/
    if @vl_type = XZAEC_FMC_FermetureEch or
       @vl_type = XZAEC_FMC_EchDeconseille
	select @vl_texte_web = @vl_texte_ech

    /* forcer le texte si perturbation meteo ou evenement sur chaussee */
    if @vl_type = XZAEC_FMC_Meteo or
       @vl_type = XZAEC_FMC_TraitChaussees or
       @vl_type = XZAEC_FMC_ChausseeGliss
    begin
      select @vl_texte_web=L.libelle from CFG..LIB_PRE L , EXP..FMC_MET M 
	 where M.numero=@vl_numero and M.cle=@vl_cle and L.notyp=20 and L.code=M.type_de_perturbation

    end

    /* forcer le texte si incident sous concessionnaire */
    if @vl_type = XZAEC_FMC_IncSsConces
    begin
      select @vl_ferme = ferme, @vl_carburant=carburant, @vl_penurie = penurie
					  from EXP..FMC_CNC where numero=@vl_numero and cle=@vl_cle
      if (@vl_ferme=0 and @vl_penurie=0) select @vl_texte_web = "PAS DE CARBURANT"
      else if (@vl_ferme=1) select @vl_texte_web = "STATION FERMEE"
      else if (@vl_carburant = 1 ) select @vl_texte_web = "PAS DE SUPER"
      else if (@vl_carburant = 2 ) select @vl_texte_web = "PAS DE GAZOLE"
      else if (@vl_carburant = 3 ) select @vl_texte_web = "PAS DE SANS PLOMB 95"
      else if (@vl_carburant = 4 ) select @vl_texte_web = "PAS DE SANS PLOMB 98"
      else if (@vl_carburant = 5 ) select @vl_texte_web = "PAS DE GPL"
      else  select  @vl_texte_web = "PAS DE CARBURANT"
    end

    /*rajout de la localisation dans le texte*/
    if @vl_PR_debut = @vl_PR_fin
      select @vl_texte_final = rtrim(@vl_texte_web) + ", km " + convert(char(3),floor(round(@vl_PR_debut,-3)/1000))
    else
    begin
      if @vl_PR_debut > @vl_PR_fin
      begin
	select @vl_PR_min = @vl_PR_fin
	select @vl_PR_max = @vl_PR_debut
      end
      else
      begin
	select @vl_PR_min = @vl_PR_debut
	select @vl_PR_max = @vl_PR_fin
      end

      select @vl_texte_final = rtrim(@vl_texte_web) + ", km " + convert(char(3),floor(round(@vl_PR_min,-3)/1000))
					+ "-" + convert(char(3),floor(round(@vl_PR_max,-3)/1000))
    end

    /* pour les travaux on calcule le nb de voies neutralisees et le nbre de voies totales */
    if @vl_type = XZAEC_FMC_Travaux or
       @vl_type = XZAEC_FMC_ZoneStockage or
       @vl_type = XZAEC_FMC_BasculTravaux
    begin
      select @vl_nb_voies = nombre_de_voies from CFG..RES_SEG S, CFG..RES_POR P where S.portion= P.numero and 
		      P.autoroute=@vl_aut_trv and S.sens=@vl_sens_trv and @vl_PRT_trv between S.PR_debut and S.PR_fin
/*       select @vl_texte_final = rtrim(@vl_texte_final) + convert(char(1),@vl_voies_trv) + 
			      " voie(s) fermee(s)/" + convert(char(1),@vl_nb_voies)*/
    end

    /*calcul des troncons impactes*/
    exec XZAE162 @vl_aut_debut, @vl_sens_debut,@vl_PR_debut,
   		@vl_pt_car,@vl_num_pt_car, 
		@vl_horodate,
		@vl_texte_final,
		@vl_icone,
		@vl_priorite,
		@vl_type_web
		
  end
  select @vl_update = XDC_NOK
  fetch filtre2 into @vl_numero,@vl_cle,@vl_type,
			@vl_VR, @vl_VM2, @vl_VM1, @vl_VL,
			@vl_VR_I, @vl_VM2_I, @vl_VM1_I, @vl_VL_I,
			@vl_aut_debut,@vl_PR_debut,@vl_sens_debut,@vl_horodate,
			@vl_pt_car,@vl_num_pt_car
end

close filtre2

/*generation des textes associes aux icones*/
exec @vl_cr = XZAE163

drop table #TMP_WEB_FMC
drop table #TMP_WEB_BOU

return (@vl_cr)

go
