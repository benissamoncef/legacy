/*E*/
/*  Fichier : $Id: xzae201sp.prc,v 1.29 2019/09/24 11:55:55 devgfi Exp $      $Revision: 1.29 $      $Date: 2019/09/24 11:55:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae201sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module ROL : Calcul de la synthese des messages tfm
* Remarque : RESTE A FAIRE
*               TDP
*               PR FANTOMES
*               AURIOL
*
* UTILISATION : en temps reel avec d'apres la liste TFM             : EXP..xzae201
*               en temps reel avec d'apres la liste TFM avec traces : EXP..xzae201 null,null,1
*               en temps differe,complete   sans traces             : HIS..xzae201 1,horodate,null
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     27/06/03  : Creation
* Cluchague     28/07/03  : ajout d'un en-tete horodate
* Cluchague     05/08/03  : vehicules de secours + pb "O km apres" v1.2
* Cluchague     05/11/03  : on met les lignes d'en tete en minuscule v1.3
* Cluchague     20/11/03  : erreur sur numero lit arret v1.4
* Cluchague     16/02/04  : on ne precise pas le type de bretelle sur f.ech v1.5
* Cluchague     10/06/04  : parametres d'entree pour permettre plusieurs types de recherche v1.6
* Cluchague     22/06/04  : le numero de fmc dans la synthese n'est demasque que quand on met les traces v1.7
* Cluchague     29/06/04  : on trie la liste finale avec le critere supplementaire de horodate debut v1.8
* Cluchague     11/02/05  : si les parametres d'entree sont a 0 on les met a null (bug applix) v1.9
* Niepceron	28/02/05  : Ajout du cas XZAEC_BOU_Accordeon v1.10 DEM428
* CHI           13/04/05  : Rajout sous type "grele sur chaussee" et "chaussee mouillee" v1.11 DEM456
* JPL		20/04/07  : Prise en compte du type fmc "Vehicule en marche arriere" (DEM 643) v1.12
* JPL		10/05/07  : Prise en compte du type fmc "Traitement Chaussees" (DEM 644) v1.13
* JPL		10/05/07  : Prise en compte du type fmc "Echangeur deconseille" (DEM 645) v1.14
* JPL		11/05/07  : Prise en compte du type fmc "Chaussee glissante" (DEM 646) v1.15
* JPL		06/06/07  : Prise en compte du type fmc "Basculement pour travaux" (DEM 647) v1.16
* JPL		30/10/07  : Filtrer les FMC ayant pour origine 'Exercice' (DEM 719) 1.17
* PNI		29/08/08  : Prise en compte de la nature de l'obstacle DEM487 1.18
* JPL		27/02/09  : Prise en compte du champ 'Feu maitrise' pour "Vehicule en feu" (DEM 845) 1.19
* JPL		27/02/09  : Gestion speciale du motif de config. des voies evitant sa modif. par SCCS 1.20
* PCL		15/07/09  : eviter la boucle infinie (max 50 boucles) lors de la recherche de l'arborescence des liens 
			    cause / consequences lorsque une cause est rebouclee par erreur sur une de ses consequences 1.21
* JPL		25/08/10 : Prise en compte du type Fmc "Zone de stockage" (DEM 942) 1.22
* PNI		28/06/11 : Prise en compte de echangeur de bifucation 98 et 198 1.23 FE63
* PCL		19/07/11 : idem mantiss 748 pour 3 autres cas 1.24 
* JPL		09/01/12 : Changement libelle "MESSAGES TFM" en "MESSAGES RVA" (DEM 1009)  1.25
* JPL		24/01/12 : Mention de la saturation a l'origine d'un bouchon avec tete en sortie (DEM 1010)  1.26
* JPL		26/06/12 : Noms de sortie usager et indicateurs de bifurcation pris en table echangeurs (DEM 1033)  1.27
* JPL		06/09/12 : Prise en compte du champ 'Feu maitrise' pour "Incendie de vegetation" (DEM 1037)  1.28
* JPL		23/09/19 : Type Incendie généralisé : mention du type d'incendie (DEM 1352)  1.29
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE201
* 
* Arguments en entree
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les renseignements necessaires a l'etablissement 
	  de la synthese des messages TFM
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

if exists (select * from sysobjects where name = 'XZAE201' and type = 'P')
	drop procedure XZAE201
go


create procedure XZAE201
		 @va_recherche_complete_in tinyint =null,
                 @va_horodate_in datetime =null,
		 @va_trace_in tinyint =null
as

declare 
	@C_MOTIF_VOIES_BN  char(3)      ,
	@vl_numero_fmc     int          ,
	@vl_duree_calcul   int          ,
	@vl_cle            tinyint      ,
	@vl_type           smallint     ,
	@vl_type_cause     smallint     ,
        @vl_debut          datetime     ,
	@vl_fin            datetime     ,
	@vl_fmc_confirmee  bit          ,
	@vl_autoroute      tinyint      ,
	@vl_autoroute1     tinyint      ,
	@vl_autoroute_trie tinyint      ,
	@vl_autoroute_precedent     tinyint      ,
	@vl_PR             int          ,
	@vl_PR1            int          ,
	@vl_PR_min         int          ,
	@vl_PR_max         int          ,
	@vl_sens           tinyint      ,
	@vl_sens_trie      int          ,
	@vl_sens_chaussee  tinyint      ,
	@vl_sens_precedent tinyint      ,
	@vl_pt_car         tinyint      ,
	@vl_numero_pt_car  tinyint      ,	-- numero d ordre en base, interne
	@vl_num_id_pt_car  tinyint      ,	-- numero identifiant un point caracteristique (lit d arret)
	@vl_nom_pt_car     char(30)     ,
	@vl_position       tinyint      ,
	@vl_pt_car1        tinyint      ,
	@vl_numero_pt_car1 tinyint      ,
	@vl_sortie_tete    char(20)     ,
	@vl_nom_pt_car1    char(30)     ,
        @vl_VR             tinyint      ,
        @vl_VM2            tinyint      , 
        @vl_VM1            tinyint      ,
	@vl_VL             tinyint      ,
	@vl_BAU            tinyint      ,
        @vl_VR_I           tinyint      ,
        @vl_VM2_I          tinyint      ,
	@vl_VM1_I          tinyint      ,
        @vl_VL_I           tinyint      ,
        @vl_BAU_I          tinyint      ,
        @vl_VR_T           char         ,
        @vl_VM2_T          char         , 
        @vl_VM1_T          char         ,
	@vl_VL_T           char         ,
	@vl_BAU_T          char         ,
        @vl_VR_I_T         char         ,
        @vl_VM2_I_T        char         ,
	@vl_VM1_I_T        char         ,
        @vl_VL_I_T         char         ,
        @vl_BAU_I_T        char         ,
        @vl_voies_retrecies bit         ,
        @vl_nom_autoroute  char(4)      ,
	@vl_localisation   char(100)    ,
	@vl_neutralisation char(100)    ,
	@vl_blocal_neutral char(100)    ,
	@vl_msg_neutralisation char(100),
        @vl_nature_evenement char(100)   ,
	@vl_type_meteo     tinyint      ,
        @vl_groupe_nominal char(150)    ,   
	@vl_groupe_verbal  char(150)    ,                            
	@vl_secours        char(100)    ,                            
	@vl_longueur_bouchon char(10)   ,
	@vl_etendue_evt    char(120)    ,
	@vl_debut_prevu    datetime     ,
	@vl_fin_prevue     datetime     ,
	@vl_lancement_calcul datetime   ,
	@vl_fin_calcul     datetime     ,
	@vl_cause          int          ,
	@vl_cle_cause      tinyint      ,
	@vl_voies_bloquees char(100)    ,
	@vl_msg_voies_bloquees char(100),
	@vl_config_voies   char(4)      ,
	@vl_config_voies_n char(4)      ,
	@vl_config_voies_b char(4)      ,
	@vl_config_voies_b_ou_n  char(4)      ,
	@vl_type_bouchon   tinyint      ,
	@vl_numero_carburant tinyint    ,
	@vl_flag_ferme     bit          ,
	@vl_longueur_arrondie int		,
	@vl_longueur_reelle int		,
	@vl_entree1		bit,
	@vl_entree2		bit,
	@vl_sortie1		bit,
	@vl_sortie2		bit,
	@vl_abrev_pt_car	char(25),
	@vl_nombre              int,
	@vl_msg_tdp		char(200),
        @vl_numero_zdp          smallint,
        @vl_numero_zdp_prec     smallint,
	@vl_PR_amont_zdp        int,
	@vl_PR_aval_zdp         int, 
	@vl_temps               smallint,
	@vl_temps_mesure        smallint,
	@vl_temps_habituel      smallint,
	@vl_PR_aval_zdp_prec    int,
	@vl_PR_debut_congestion int,
	@vl_PR_fin_congestion   int,
	@vl_compte              int,
	@vl_temps_cumule        smallint,
	@vl_PR_dest_amont       int,
        @vl_numero_dest_amont   smallint,
	@vl_PR_dest_aval        int,
        @vl_numero_dest_aval    smallint,
	@vl_horodate            datetime,
	@vl_msg_tfm             char(255),
	@vl_en_tete_horodate    char(255),
	@vl_entete_autoroute_sens char(100),
        @vl_km                  char(30),
        @vl_nom_tunnel          char(25),
        @vl_limite              char(40),
        @vl_distance            int,
	@vl_PR_ech_proche       int,
	@vl_sortie_proche       char(20),
	@vl_sortie_usager       char(20),
        @vl_nom_ech_proche      char(30),
	@vl_PR_ech_amont        int,
	@vl_sortie_amont        char(20),
        @vl_nom_ech_amont       char(30),
	@vl_indic_bifurc        bit,
	@vl_voies_sortie_sens1  tinyint,
	@vl_voies_sortie_sens2  tinyint,
	@vl_portion             smallint,
	@vl_type_evenement      tinyint,
	@vl_compteur_de_boucle   tinyint,
	@vl_feu_maitrise        tinyint,
	@vl_code_nature         tinyint,
	@vl_nature_incendie     varchar(100)

/* initialisation du motif de configuration des voies */
/* indiquant une voie au moins marquee (bloquee ou neutralisee) */
select @C_MOTIF_VOIES_BN = '%' + 'R' + '%'

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      analyse des parametres d'entree                                                 */
/************************************************************************************************************************/
/************************************************************************************************************************/
/* le parametre d'entree horodate est la date presente par defaut (si null)  */
if (@va_horodate_in is null or @va_horodate_in=XDC_DATE_NULLE) select @va_horodate_in=getdate()
if @va_trace_in=0 select @va_trace_in = null

/* le parametre d'entree est recherche partielle par defaut (si null)  */
if  @va_recherche_complete_in is null  select @va_recherche_complete_in=0


/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des fmc en cours et tri selectif                                   */
/************************************************************************************************************************/
/************************************************************************************************************************/

create table #TMP_SYNTHESE_TFM (valide bit, numero int null, cle tinyint null, autoroute tinyint null ,sens int null ,PR int null null ,message_tfm char(255) null , debut datetime null)
create table #TMP_LISTE_FMC (numero int null , cle tinyint null , type smallint null , cause int null ,cle_cause tinyint null )

if @va_recherche_complete_in = 0
begin
  /* recherche de toutes les FMC qui ont une action TFM en cours                        */
  /* on exclue les FMC operateur pour ne pas avoir de dead lock sur les synthese        */
  /* rattachees au poste operateur. De plus pas d'autoroute, ni de PR ni de sens        */
  /* a ce stade la la liste doit etre la meme que la liste des messages tfm a diffuser  */
  insert into #TMP_LISTE_FMC 
    select distinct 
      G.numero,G.cle,G.type,G.cause,G.cle_cause
      from
#ifdef HIST
      HIS..ACT_GEN A, HIS..FMC_GEN G
#else
      EXP..ACT_GEN A, EXP..FMC_GEN G
#endif
    where G.numero = A.evenement and G.cle = A.cle and
      G.type != XZAEC_FMC_PosteOperateur and
      A.type=XDC_ACT_TFM and 
      (A.heure_succes = null or  A.heure_succes < @va_horodate_in ) and
      (A.heure_fin = null or  A.heure_fin > @va_horodate_in )

  /* trace clu de tous les evenements dans la liste des messages TFM en diffusion */ 
  if @va_trace_in is not null
    select distinct numero,cle,type,cause,cle_cause from #TMP_LISTE_FMC

  /* on recherche les causes de facon iterative */   
  declare recherche_cause cursor for
    select distinct 
       T.numero, T.cle, T.type, T.cause , T.cle_cause
    from #TMP_LISTE_FMC T
    where T.cause is not null

  select @vl_compteur_de_boucle = 0

continue_a_rechercher_causes:
  select @vl_compteur_de_boucle= @vl_compteur_de_boucle + 1
  /* ouverture curseur */
  open recherche_cause
  fetch recherche_cause into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause


/* boucle curseur */
  while (@@sqlstatus = 0 )
  begin
    insert into #TMP_LISTE_FMC 
    select 
      G.numero,G.cle,G.type,G.cause,G.cle_cause
    from 
#ifdef HIST
      HIS..FMC_GEN G
#else 
      EXP..FMC_GEN G
#endif
    where G.numero = @vl_cause and G.cle=@vl_cle_cause

    update #TMP_LISTE_FMC
    set cause=null where numero=@vl_numero_fmc and cle=@vl_cle
    fetch recherche_cause into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause
  end

  /* fermeture curseur */
  close recherche_cause

  /* recherche d'autres causes eventuelles */
  select @vl_compte=count(*) from #TMP_LISTE_FMC where cause is not null
  if (@vl_compte != 0 and @vl_compteur_de_boucle < 50) goto continue_a_rechercher_causes

  /* filtre on enleve les evenements fils */
  delete #TMP_LISTE_FMC where numero is null or (cle_cause is not null and cause is null)

  /* trace clu de tous les evenements racine */ 
  if @va_trace_in is not null
    select distinct numero,cle,type,cause,cle_cause from #TMP_LISTE_FMC

  /* a ce stade on a toutes les causes premieres. on va rechercher toutes les FMC filles */
  select @vl_compteur_de_boucle = 0
  declare recherche_consequences cursor for
    select distinct 
       T.numero, T.cle, T.type, T.cause , T.cle_cause
    from #TMP_LISTE_FMC T
    where T.cause is null
continue_a_rechercher_conseq:
  select @vl_compteur_de_boucle= @vl_compteur_de_boucle + 1
  /* ouverture curseur */
  open recherche_consequences
  fetch recherche_consequences into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause

  /* boucle curseur */
  while (@@sqlstatus = 0 )
  begin
    insert into #TMP_LISTE_FMC 
    select 
      G.numero,G.cle,G.type,null, null           
    from 
#ifdef HIST 
      HIS..FMC_GEN G
#else
      EXP..FMC_GEN G
#endif
    where G.cause = @vl_numero_fmc and G.cle_cause=@vl_cle
    update #TMP_LISTE_FMC set cause=1 where numero=@vl_numero_fmc and cle= @vl_cle
    fetch recherche_consequences into @vl_numero_fmc, @vl_cle, @vl_type, @vl_cause , @vl_cle_cause
  end

  /* fermeture curseur */
  close recherche_consequences

  /* recherche d'autres consequences eventuelles */
  select @vl_compte=count(*) from #TMP_LISTE_FMC where cause is null
  if (@vl_compte != 0 and @vl_compteur_de_boucle < 50) goto continue_a_rechercher_conseq

  /* trace clu */ 
  if @va_trace_in is not null
   select distinct numero,cle,type,cause,cle_cause from #TMP_LISTE_FMC

  declare fmc_en_cours cursor for
#ifdef HIST
  select distinct 
       F.numero, F.cle, F.type, F.debut , F.fin , 
       F.debut_prevu , F.fin_prevue , F.cause, F.cle_cause,
       H.confirme_ou_signale, H.autoroute , H.PR , H.sens,
       H.point_caracteristique , H.numero_point_caracteristique ,
       H.position_sur_aire_ou_echangeur,
       H.VR, H.VM2,H.VM1,H.VL,H.BAU,
       H.VR_I, H.VM2_I,H.VM1_I,H.VL_I,H.BAU_I
  from HIS..FMC_GEN F, HIS..FMC_HIS H, #TMP_LISTE_FMC T
  where H.numero=F.numero and H.cle=F.cle and 
      F.numero = T.numero and F.cle = T.cle and
      ( F.fin is null or F.fin > @va_horodate_in ) and
      T.type != XZAEC_FMC_TeteBouchon and
      H.horodate_validation < @va_horodate_in
  group by H.numero , H.cle
  having 
  (
      H.numero=F.numero and H.cle=F.cle and 
      F.numero = T.numero and F.cle = T.cle and
      ( F.fin is null or F.fin > @va_horodate_in ) and
      T.type != XZAEC_FMC_TeteBouchon and
      H.horodate_validation < @va_horodate_in
  ) 
  and horodate_validation=max(horodate_validation)
  /* tri par sens y compris bretelles impair 1, 3, 7 pour sens 1, pair pour sens 2 et non precise d'ou calcul avec modulo */
  /* pour ne pas que soient repetees les en tetes si on trie uniquement par sens */
  order by H.autoroute,H.sens%2 desc
#else
  select distinct 
       F.numero, F.cle, F.type, F.debut , F.fin , 
       F.debut_prevu , F.fin_prevue , F.cause, F.cle_cause,
       H.confirme_ou_signale, H.autoroute , H.PR , H.sens,
       H.point_caracteristique , H.numero_point_caracteristique ,
       H.position_sur_aire_ou_echangeur,
       H.VR, H.VM2,H.VM1,H.VL,H.BAU,
       H.VR_I, H.VM2_I,H.VM1_I,H.VL_I,H.BAU_I
  from EXP..FMC_GEN F, EXP..FMC_HIS_DER H, #TMP_LISTE_FMC T
  where H.numero=F.numero and H.cle=F.cle and 
      F.numero = T.numero and F.cle = T.cle and
      ( F.fin is null or F.fin > @va_horodate_in ) and
      T.type != XZAEC_FMC_TeteBouchon
  /* tri par sens y compris bretelles impair 1, 3, 7 pour sens 1, pair pour sens 2 et non precise d'ou calcul avec modulo */
  /* pour ne pas que soient repetees les en tetes si on trie uniquement par sens */
  order by H.autoroute,H.sens%2 desc
#endif
end
else
begin
  declare fmc_en_cours cursor for
#ifdef HIST
  select distinct 
       F.numero, F.cle, F.type, F.debut , F.fin , 
       F.debut_prevu , F.fin_prevue , F.cause, F.cle_cause,
       H.confirme_ou_signale, H.autoroute , H.PR , H.sens,
       H.point_caracteristique , H.numero_point_caracteristique ,
       H.position_sur_aire_ou_echangeur,
       H.VR, H.VM2,H.VM1,H.VL,H.BAU,
       H.VR_I, H.VM2_I,H.VM1_I,H.VL_I,H.BAU_I
  from HIS..FMC_GEN F, HIS..FMC_HIS H
  where H.numero=F.numero and H.cle=F.cle and 
       F.debut < @va_horodate_in and F.fin > @va_horodate_in and
      F.origine_creation not like '%ESSAI%' and F.origine_creation not like '%EXERCICE%' and 
      F.type in 
      (
        XZAEC_FMC_Accident,XZAEC_FMC_Delestage, XZAEC_FMC_Deviation, 
        XZAEC_FMC_Meteo , XZAEC_FMC_Contresens , XZAEC_FMC_VehArrete, XZAEC_FMC_VehPanne, XZAEC_FMC_VehFeu, 
        XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil, XZAEC_FMC_VehLent, XZAEC_FMC_Basculement,
        XZAEC_FMC_IncSsConces, XZAEC_FMC_QueueBouchon, XZAEC_FMC_FermetureEch,
        XZAEC_FMC_Eboulement, XZAEC_FMC_Incendie, XZAEC_FMC_Inondation,XZAEC_FMC_Obstacle,
        XZAEC_FMC_FlaqueHuile, XZAEC_FMC_AnimalErrant, XZAEC_FMC_AireFermee,
        XZAEC_FMC_Salage, XZAEC_FMC_PietonChaussee,
        XZAEC_FMC_VehMarcheArr, XZAEC_FMC_TraitChaussees, XZAEC_FMC_EchDeconseille, XZAEC_FMC_ChausseeGliss,
        XZAEC_FMC_BasculTravaux, XZAEC_FMC_ZoneStockage
      ) and
      H.horodate_validation<@va_horodate_in  
  group by H.numero,H.cle
  having 
    (
      H.numero=F.numero and H.cle=F.cle and 
      F.debut < @va_horodate_in and F.fin > @va_horodate_in and
      F.origine_creation not like '%ESSAI%' and F.origine_creation not like '%EXERCICE%' and 
      F.type in 
      (
        XZAEC_FMC_Accident,XZAEC_FMC_Delestage, XZAEC_FMC_Deviation, 
        XZAEC_FMC_Meteo , XZAEC_FMC_Contresens , XZAEC_FMC_VehArrete, XZAEC_FMC_VehPanne, XZAEC_FMC_VehFeu, 
        XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil, XZAEC_FMC_VehLent, XZAEC_FMC_Basculement,
        XZAEC_FMC_IncSsConces, XZAEC_FMC_QueueBouchon, XZAEC_FMC_FermetureEch,
        XZAEC_FMC_Eboulement, XZAEC_FMC_Incendie, XZAEC_FMC_Inondation,XZAEC_FMC_Obstacle,
        XZAEC_FMC_FlaqueHuile, XZAEC_FMC_AnimalErrant, XZAEC_FMC_AireFermee,
        XZAEC_FMC_Salage, XZAEC_FMC_PietonChaussee,
        XZAEC_FMC_VehMarcheArr, XZAEC_FMC_TraitChaussees, XZAEC_FMC_EchDeconseille, XZAEC_FMC_ChausseeGliss,
        XZAEC_FMC_BasculTravaux, XZAEC_FMC_ZoneStockage
      ) and
      H.horodate_validation<@va_horodate_in  
     ) 
     and horodate_validation=max(horodate_validation)
  /* tri par sens y compris bretelles impair 1, 3, 7 pour sens 1, pair pour sens 2 et non precise d'ou calcul avec modulo */
  /* pour ne pas que soient repetees les en tetes si on trie uniquement par sens */
  order by H.autoroute,H.sens%2 desc
#else
  select distinct 
       T.evenement, T.cle, F.type, F.debut , F.fin , 
       F.debut_prevu , F.fin_prevue , F.cause, F.cle_cause,
       H.confirme_ou_signale, H.autoroute , H.PR , H.sens,
       H.point_caracteristique , H.numero_point_caracteristique ,
       H.position_sur_aire_ou_echangeur,
       H.VR, H.VM2,H.VM1,H.VL,H.BAU,
       H.VR_I, H.VM2_I,H.VM1_I,H.VL_I,H.BAU_I
  from EXP..FMC_TRT T, EXP..FMC_GEN F, EXP..FMC_HIS_DER H
  where F.numero=T.evenement and F.cle=T.cle and H.numero=F.numero and H.cle=F.cle and 
      F.fin is null and F.debut != NULL and  F.cloture is null and
      F.origine_creation not like '%ESSAI%' and F.origine_creation not like '%EXERCICE%' and 
      F.type in 
      (
        XZAEC_FMC_Accident,XZAEC_FMC_Delestage, XZAEC_FMC_Deviation, 
        XZAEC_FMC_Meteo , XZAEC_FMC_Contresens , XZAEC_FMC_VehArrete, XZAEC_FMC_VehPanne, XZAEC_FMC_VehFeu, 
        XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil, XZAEC_FMC_VehLent, XZAEC_FMC_Basculement,
        XZAEC_FMC_IncSsConces, XZAEC_FMC_QueueBouchon, XZAEC_FMC_FermetureEch,
        XZAEC_FMC_Eboulement, XZAEC_FMC_Incendie, XZAEC_FMC_Inondation,XZAEC_FMC_Obstacle,
        XZAEC_FMC_FlaqueHuile, XZAEC_FMC_AnimalErrant, XZAEC_FMC_AireFermee,
        XZAEC_FMC_Salage, XZAEC_FMC_PietonChaussee,
        XZAEC_FMC_VehMarcheArr, XZAEC_FMC_TraitChaussees, XZAEC_FMC_EchDeconseille, XZAEC_FMC_ChausseeGliss,
        XZAEC_FMC_BasculTravaux, XZAEC_FMC_ZoneStockage
      )
  /* tri par sens y compris bretelles impair 1, 3, 7 pour sens 1, pair pour sens 2 et non precise d'ou calcul avec modulo */
  /* pour ne pas que soient repetees les en tetes si on trie uniquement par sens */
  order by H.autoroute,H.sens%2 desc
#endif
end

/* releve de l'horodate de debut de calcul */
select @vl_lancement_calcul=getdate()

open fmc_en_cours
fetch fmc_en_cours into
       @vl_numero_fmc, @vl_cle, @vl_type, @vl_debut , @vl_fin , 
       @vl_debut_prevu , @vl_fin_prevue , @vl_cause, @vl_cle_cause,
       @vl_fmc_confirmee,@vl_autoroute , @vl_PR , @vl_sens,
       @vl_pt_car , @vl_numero_pt_car ,
       @vl_position,
       @vl_VR, @vl_VM2,@vl_VM1,@vl_VL,@vl_BAU,
       @vl_VR_I, @vl_VM2_I,@vl_VM1_I,@vl_VL_I,@vl_BAU_I

  /************************************************************************************************************************/
  /* raz des variables                                                                                                    */
  /************************************************************************************************************************/
  select 
	 @vl_autoroute_precedent=0,
	 @vl_sens_precedent=0

  /************************************************************************************************************************/
  /* recherche de l'en-tete autoroute et sens. Si c'est le meme que precedemment on ne repete pas                         */
  /************************************************************************************************************************/
while (@@sqlstatus = 0 )
begin
  /* trace clu */
  if @va_trace_in is not null 
    select  "traitement",@vl_numero_fmc,@vl_cle,@vl_type, @vl_debut , @vl_fin , @vl_debut_prevu , @vl_fin_prevue , @vl_cause, 
			 @vl_cle_cause, @vl_fmc_confirmee,@vl_autoroute , @vl_PR , @vl_sens, @vl_pt_car , @vl_numero_pt_car ,
			 @vl_position

  /************************************************************************************************************************/
  /************************************************************************************************************************/
  /*                                      recuperation des infos en base fmc par fmc                                      */
  /************************************************************************************************************************/
  /************************************************************************************************************************/

  /************************************************************************************************************************/
  /* raz des variables                                                                                                    */
  /************************************************************************************************************************/
  select  @vl_PR_ech_proche = NULL,
	  @vl_PR_ech_amont = NULL,
	  @vl_longueur_bouchon = "",
	  @vl_etendue_evt = "",
	  @vl_config_voies_b = NULL,
	  @vl_config_voies_n = NULL,
	  @vl_config_voies_b_ou_n =NULL,
	  @vl_voies_bloquees =NULL,
          @vl_neutralisation = NULL,
	  @vl_blocal_neutral = NULL,
	  @vl_msg_neutralisation=NULL,
	  @vl_msg_voies_bloquees=NULL,
	  @vl_PR1=@vl_PR

  /************************************************************************************************************************/
  /* recherche de l'en-tete autoroute et sens. Si c'est le meme que precedemment on ne repete pas                         */
  /************************************************************************************************************************/
  if ( @vl_sens = 3 or @vl_sens =7 or @vl_sens = 1 )
  begin
    select @vl_sens_chaussee = 1, @vl_sens_trie=1
  end
  else if ( @vl_sens = 4 or @vl_sens =8  or  @vl_sens = 2 ) 
  begin
    select @vl_sens_chaussee = 2, @vl_sens_trie =-1
  end
  else 
  begin
    select @vl_sens_chaussee = 0, @vl_sens_trie = 2
  end

  if (@vl_autoroute != @vl_autoroute_precedent or @vl_sens_chaussee != @vl_sens_precedent )
  begin
    select @vl_entete_autoroute_sens="********************"+lower(libelle_autoroute) + ", " + lower(libelle_sens)
  	  from CFG..SAD_AUT
	  where autoroute = @vl_autoroute
	  and sens = @vl_sens_chaussee
    select @vl_autoroute_precedent=@vl_autoroute, @vl_sens_precedent=@vl_sens_chaussee
    if @vl_autoroute = 1 select @vl_autoroute_trie =  1
    else if @vl_autoroute = 2 select @vl_autoroute_trie =  2
    else if @vl_autoroute = 8 select @vl_autoroute_trie =  3
    else if @vl_autoroute = 6 select @vl_autoroute_trie =  4
    else if @vl_autoroute = 3 select @vl_autoroute_trie =  5
    else if @vl_autoroute = 7 select @vl_autoroute_trie =  6
    else if @vl_autoroute = 4 select @vl_autoroute_trie =  7
    else if @vl_autoroute = 5 select @vl_autoroute_trie =  8
    else if @vl_autoroute = 9 select @vl_autoroute_trie =  9
    else if @vl_autoroute = 10 select @vl_autoroute_trie =  10
    
    /* la ligne d'en tete est non valide par defaut. Elle sera valide si un evenement correspondant est insere en table */
    insert #TMP_SYNTHESE_TFM values(0,@vl_numero_fmc,@vl_cle , @vl_autoroute_trie,@vl_sens_trie,null,@vl_entete_autoroute_sens,@vl_debut)
  end

  /************************************************************************************************************************/
  /* recherche des elements groupes nominaux et groupe verbaux pour l'evenement traite                                    */
  /************************************************************************************************************************/
  if (@vl_type= XZAEC_FMC_Accident)
  select @vl_groupe_nominal = "UN ACCIDENT ",
	 @vl_groupe_verbal = "UN ACCIDENT S'EST PRODUIT "

  else if (@vl_type= XZAEC_FMC_Delestage)
  select @vl_groupe_nominal = "UN ITINERAIRE CONSEILLE ",
         @vl_groupe_verbal = "UN ITINERAIRE CONSEILLE EST EN PLACE "

  else if (@vl_type= XZAEC_FMC_Deviation)
  select @vl_groupe_nominal = "UNE SORTIE OBIGATOIRE ",
         @vl_groupe_verbal = "UNE SORTIE OBLIGATOIRE EST EN PLACE "

  else if (@vl_type = XZAEC_FMC_Meteo)
  begin
    /* recherche du type de perturbation meteo et de la visibilite */
    select @vl_PR1=PR, @vl_type_meteo = type_de_perturbation 
#ifdef HIST
    from HIS..FMC_MET 
#else
    from EXP..FMC_MET 
#endif
    where numero=@vl_numero_fmc and cle=@vl_cle
    if ( @vl_type_meteo = XZAEC_MET_ChuteNeige ) 
    begin
      select @vl_groupe_nominal = "DES CHUTES DE NEIGE ",
             @vl_groupe_verbal  = "LA NEIGE TOMBE "
    end
    else if ( @vl_type_meteo = XZAEC_MET_NeigeChaus ) 
    begin
      select @vl_groupe_nominal = "DE LA NEIGE SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "LA CHAUSSEE EST ENNEIGEE "
    end
    else if ( @vl_type_meteo = XZAEC_MET_Pluie ) 
    begin
      select @vl_groupe_nominal = "DES ORAGES VIOLENTS ",
             @vl_groupe_verbal  = "DES ORAGES VIOLENTS SONT EN COURS "
    end
    else if ( @vl_type_meteo = XZAEC_MET_VentFort ) 
    begin
      select @vl_groupe_nominal = "DES RAFALES DE VENT ",
             @vl_groupe_verbal  = "LE VENT SOUFFLE EN RAFALES "
    end
    else if ( @vl_type_meteo = XZAEC_MET_Brouillard ) 
    begin
      select @vl_groupe_nominal = "DU BROUILLARD ",
             @vl_groupe_verbal  = "IL Y A DU BROUILLARD "
    end
    else if ( @vl_type_meteo = XZAEC_MET_BrouillardGiv ) 
    begin
      select @vl_groupe_nominal = "DU BROUILLARD GIVRANT ",
             @vl_groupe_verbal  = "IL Y A DU BROUILLARD GIVRANT "
    end
    else if ( @vl_type_meteo = XZAEC_MET_Grele ) 
    begin
      select @vl_groupe_nominal = "DES AVERSES DE GRELE ",
             @vl_groupe_verbal  = "IL Y A DES AVERSES DE GRELE "
    end
    else if ( @vl_type_meteo = XZAEC_MET_Verglas ) 
    begin
      select @vl_groupe_nominal = "UN RISQUE DE VERGLAS ",
             @vl_groupe_verbal  = "IL Y A DES RISQUES DE VERGLAS "
    end
    else if ( @vl_type_meteo = XZAEC_MET_GreleChaus) 
    begin
      select @vl_groupe_nominal = "DE LA GRELE SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "IL Y A DE LA GRELE SUR LA CHAUSSEE"
    end
    else if ( @vl_type_meteo = XZAEC_MET_ChausMouill) 
    begin
      select @vl_groupe_nominal = "DE LA PLUIE SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "LA CHAUSSEE EST MOUILLEE"
    end
    else 
    begin
      select @vl_groupe_nominal = "UNE PERTURBATION METEO ",
             @vl_groupe_verbal  = "IL Y A UNE PERTURBATION METEO "
    end
  end

  else if (@vl_type= XZAEC_FMC_Contresens)
  select @vl_groupe_nominal = "UN VEHICULE A CONTRESENS ",
	  @vl_groupe_verbal = "UN VEHICULE CIRCULE A CONTRESENS "

  else if (@vl_type= XZAEC_FMC_VehArrete)
  select @vl_groupe_nominal = "UN VEHICULE ARRETE ",
	  @vl_groupe_verbal = "UN VEHICULE EST ARRETE "

  else if (@vl_type= XZAEC_FMC_VehPanne)
  select @vl_groupe_nominal = "UN VEHICULE EN PANNE ",
	  @vl_groupe_verbal = "UN VEHICULE EST EN PANNE "

  else if (@vl_type= XZAEC_FMC_VehFeu)
  begin

    if ( select feu_maitrise
#ifdef HIST
         from HIS..FMC_VEH
#else
         from EXP..FMC_VEH
#endif
	 where numero=@vl_numero_fmc and cle=@vl_cle
       ) = XDC_VRAI
    begin
      select @vl_groupe_nominal = "UN VEHICULE ARRETE (INCENDIE MAITRISE) ",
             @vl_groupe_verbal  = "UN VEHICULE EST ARRETE (INCENDIE MAITRISE) "
    end
    else
    begin
      select @vl_groupe_nominal = "UN VEHICULE EN FEU ",
             @vl_groupe_verbal  = "UN VEHICULE EST EN FEU "
    end

  end

  else if (@vl_type= XZAEC_FMC_Cirque)
  select @vl_groupe_nominal = "UN CIRQUE EN CONVOI ",
	  @vl_groupe_verbal = "UN CIRQUE EN  CONVOI CIRCULE "

  else if (@vl_type= XZAEC_FMC_ConvoiExc)
  select @vl_groupe_nominal = "UN CONVOI EXCEPTIONNEL ",
	  @vl_groupe_verbal = "UN CONVOI EXCEPTIONNEL CIRCULE "

  else if (@vl_type= XZAEC_FMC_ConvoiMil)
  select @vl_groupe_nominal = "UN CONVOI MILITAIRE ",
	  @vl_groupe_verbal = "UN CONVOI MILITAIRE CIRCULE "

  else if (@vl_type= XZAEC_FMC_VehLent)
  select @vl_groupe_nominal = "UN CONVOI DE VEHICULES LENTS ",
	  @vl_groupe_verbal = "UN CONVOI DE VEHICULES LENTS CIRCULE "

  else if (@vl_type= XZAEC_FMC_Basculement)
  select @vl_groupe_nominal = "UN BASCULEMENT DE CIRCULATION ",
	  @vl_groupe_verbal = "LA CIRCULATION SE FAIT A DOUBLE SENS SUR LA CHAUSSEE OPPOSEE "

  else if (@vl_type = XZAEC_FMC_IncSsConces)
  begin

    select @vl_flag_ferme=ferme,@vl_numero_carburant=carburant 
#ifdef HIST
    from HIS..FMC_CNC 
#else
    from EXP..FMC_CNC 
#endif
	    where numero=@vl_numero_fmc and cle=@vl_cle
    if @vl_flag_ferme = 1 
    begin
      select @vl_groupe_nominal = "LA FERMETURE DE LA STATION SERVICE ",
             @vl_groupe_verbal  = "LA STATION SERVICE EST FERMEE "
    end
    else 
    begin
      if @vl_numero_carburant=XDC_CRB_SUPER
      begin
        select @vl_groupe_nominal = "UNE PENURIE DE SUPER A LA STATION SERVICE ",
               @vl_groupe_verbal  = "IL N'Y A PLUS DE SUPER "
      end
      else if @vl_numero_carburant=XDC_CRB_GAZOLE
      begin
        select @vl_groupe_nominal = "UNE PENURIE DE GAZOLE A LA STATION SERVICE ",
               @vl_groupe_verbal  = "IL N'Y A PLUS DE GAZOLE "
      end
      else if @vl_numero_carburant=XDC_CRB_SSPB95
      begin
        select @vl_groupe_nominal = "UNE PENURIE DE SANS PLOMB 95 A LA STATION SERVICE ",
               @vl_groupe_verbal  = "IL N'Y A PLUS DE SANS PLOMB 95 "
      end
      else if @vl_numero_carburant=XDC_CRB_SSPB98
      begin
        select @vl_groupe_nominal = "UNE PENURIE DE SANS PLOMB 98 A LA STATION SERVICE ",
               @vl_groupe_verbal  = "IL N'Y A PLUS DE SANS PLOMB 98 "
      end
      else if @vl_numero_carburant=XDC_CRB_GPL
      begin
        select @vl_groupe_nominal = "UNE PENURIE DE GPL A LA STATION SERVICE ",
               @vl_groupe_verbal  = "IL N'Y A PLUS DE GPL "
      end
      else 
      begin
        select @vl_groupe_nominal = "UNE PENURIE DE CARBURANT A LA STATION SERVICE ",
               @vl_groupe_verbal  = "IL N'Y A PLUS DE CARBURANT "
      end
    end
  end

  else if ( @vl_type = XZAEC_FMC_QueueBouchon )
  begin

    /* raz de la config des voies */
    select
	@vl_VR = XDC_VOIE_SENS_NORMAL ,
        @vl_VM2 = XDC_VOIE_SENS_NORMAL,
        @vl_VM1 = XDC_VOIE_SENS_NORMAL,
	@vl_VL  = XDC_VOIE_SENS_NORMAL,
	@vl_BAU         = XDC_VOIE_SENS_NORMAL,
        @vl_VR_I       = XDC_VOIE_SENS_NORMAL,
        @vl_VM2_I     = XDC_VOIE_SENS_NORMAL,
	@vl_VM1_I    = XDC_VOIE_SENS_NORMAL,
        @vl_VL_I    = XDC_VOIE_SENS_NORMAL,
        @vl_BAU_I = XDC_VOIE_SENS_NORMAL

    /* recherche du PR de tete de bouchon */
    select @vl_PR1 = PR, @vl_autoroute1 = autoroute,
	   @vl_pt_car1 = point_caracteristique, @vl_numero_pt_car1 = numero_point_caracteristique
#ifdef HIST
    from HIS..FMC_HIS where numero=@vl_cause and cle=@vl_cle_cause and horodate_validation < @va_horodate_in
    group by numero, cle having horodate_validation=max(horodate_validation)
#else
    from EXP..FMC_HIS_DER where numero=@vl_cause and cle=@vl_cle_cause
#endif

    /* recherche du type de bouchon */
    select @vl_type_bouchon = type 
#ifdef HIST
    from HIS..FMC_BOU 
    where numero=@vl_numero_fmc and cle=@vl_cle and horodate < @va_horodate_in
    group by numero,cle
    having numero=@vl_numero_fmc and cle=@vl_cle and horodate < @va_horodate_in and horodate=max(horodate)
#else
    from EXP..FMC_BOU 
    where numero=@vl_numero_fmc and cle=@vl_cle
#endif

    /* determination de l'etendue du bouchon */
    exec PRC..XZAO;22 @vl_autoroute,@vl_PR,@vl_autoroute1,@vl_PR1,@vl_longueur_reelle output

    if (@vl_longueur_reelle > 0)
    begin
      if (@vl_longueur_reelle >= 1000)
      begin
  	select @vl_longueur_arrondie = floor((@vl_longueur_reelle+500)/1000)
  	select @vl_longueur_bouchon = " " + convert (char(3),@vl_longueur_arrondie) + " KM"
      end
      else if (@vl_longueur_reelle > 100 )
      begin
    	select @vl_longueur_arrondie = floor(@vl_longueur_reelle/100)*100
  	select @vl_longueur_bouchon = " " + convert (char(5),@vl_longueur_arrondie) + " M"
      end
      else
      begin
  	select @vl_longueur_arrondie = floor(@vl_longueur_reelle/10)*10
  	select @vl_longueur_bouchon = " " + convert (char(5),@vl_longueur_arrondie) + " M"
      end

      /* mention de l'origine en sortie */
      if ( @vl_pt_car is null ) and ( @vl_pt_car1 = XDC_POINT_CARACT_ECHANGEUR )
      begin
	/* lire le numero de sortie de l'echangeur de la tete de bouchon, et son nom */
	select	@vl_nom_pt_car1 = nom,
		@vl_sortie_tete = sortie,
		@vl_indic_bifurc = bifurcation
	from CFG..RES_ECH where numero=@vl_numero_pt_car1

	if @vl_indic_bifurc <> XDC_VRAI
	  select @vl_etendue_evt = " QUI A POUR ORIGINE LA SATURATION DE LA SORTIE " +
	                              rtrim (@vl_sortie_tete) + " " + upper(@vl_nom_pt_car1)
      end
    end
  
    /* calcul de la nature du bouchon */
    if @vl_type_bouchon = XZAEC_BOU_Dense 
    begin
      select @vl_groupe_nominal = "UNE CIRCULATION DIFFICILE " ,
             @vl_groupe_verbal  = "LA CIRCULATION EST DIFFICILE "
      if ( @vl_longueur_reelle > 0 )
        select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
    end
    if @vl_type_bouchon = XZAEC_BOU_Accordeon
    begin
      select @vl_groupe_nominal = "UNE CIRCULATION EN ACCORDEON " ,
             @vl_groupe_verbal  = "LA CIRCULATION EST EN ACCORDEON "
      if ( @vl_longueur_reelle > 0 )
        select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
    end
    else if @vl_type_bouchon = XZAEC_BOU_Ralenti
    begin
      select @vl_groupe_nominal = "UN TRAFIC RALENTI " ,             
             @vl_groupe_verbal  = "LE TRAFIC EST RALENTI "
      if ( @vl_longueur_reelle > 0 )
        select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
    end
    else if @vl_type_bouchon = XZAEC_BOU_Bouche
    begin
      select @vl_groupe_nominal = "UN BOUCHON " ,             
             @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UN BOUCHON "
      if ( @vl_longueur_reelle > 0 )
        select @vl_etendue_evt="CE BOUCHON" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
    end
    else if @vl_type_bouchon = XZAEC_BOU_Bloque
    begin
      select @vl_groupe_nominal = "UNE CIRCULATION BLOQUEE "  ,
             @vl_groupe_verbal  = "LA CIRCULATION EST BLOQUEE "
      if ( @vl_longueur_reelle > 0 )
        select @vl_etendue_evt="CE BOUCHON" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
    end
  end

  else if ( @vl_type = XZAEC_FMC_FermetureEch)
  begin
    select 
         @vl_groupe_nominal = "LA FERMETURE ",
         @vl_groupe_verbal  = "ON NOUS INFORME DE LA FERMETURE "
    select
	 @vl_entree1 = entree1,
	 @vl_entree2 = entree2,
	 @vl_sortie1 = sortie1,
	 @vl_sortie2 = sortie2
#ifdef HIST
    from HIS..FMC_ECH
#else
    from EXP..FMC_ECH
#endif
    where numero = @vl_numero_fmc
    and cle = @vl_cle

    select  @vl_nombre = convert(int,@vl_entree1) + convert(int,@vl_entree2) + convert(int,@vl_sortie1) + convert(int,@vl_sortie2)
    if @vl_nombre = 1
    begin
      select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE LA BRETELLE"
      if (@vl_entree1=1 or @vl_entree2=1) select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE"
      else select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE SORTIE"
    end
    else
    begin
      select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DES BRETELLES"
      if (@vl_entree1=1 or @vl_entree2=1) 
      begin 
        if (@vl_sortie1=1 or @vl_sortie2=1) select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE ET DE SORTIE"
        else select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE"
      end
      else
      begin
        select @vl_groupe_verbal =  rtrim(@vl_groupe_verbal) + " DE SORTIE"
      end
    end
      select @vl_nom_pt_car = E.nom , @vl_abrev_pt_car = E.abreviation ,
	     @vl_sortie_usager = E.sortie , @vl_indic_bifurc = bifurcation
      from CFG..RES_ECH E where E.numero=@vl_numero_pt_car
      if ( @vl_indic_bifurc = XDC_VRAI )
      begin
          select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE LA BIFURCATION " + upper(@vl_nom_pt_car)
      end
      else
      begin
        select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE L'ECHANGEUR " + rtrim(@vl_sortie_usager) + " " + 
  				upper(@vl_nom_pt_car)
      end
  end

  else if (@vl_type= XZAEC_FMC_Eboulement)
  select @vl_groupe_nominal = "UN EBOULEMENT ",
	  @vl_groupe_verbal = "UN EBOULEMENT S'EST PRODUIT "

  else if (@vl_type= XZAEC_FMC_Incendie)
  begin
    select @vl_code_nature = code_nature,
           @vl_feu_maitrise = feu_maitrise
#ifdef HIST
         from HIS..FMC_INC
#else
         from EXP..FMC_INC
#endif
	 where numero=@vl_numero_fmc and cle=@vl_cle

    select @vl_nature_incendie = "Non précisé"
    select @vl_nature_incendie = libelle
    from CFG..LIB_PRE
    where notyp = XDC_LIB_TYPE_NATURE_INCENDIE  and  code = @vl_code_nature

    select @vl_groupe_nominal = "UN INCENDIE (" + @vl_nature_incendie + ") ",
           @vl_groupe_verbal = "UN INCENDIE (" + @vl_nature_incendie + ") S'EST PRODUIT "

    if (@vl_feu_maitrise = XDC_VRAI)
    begin
      select @vl_groupe_nominal  = rtrim(@vl_groupe_nominal) + " (INCENDIE MAITRISE) ",
             @vl_groupe_verbal  = rtrim(@vl_groupe_verbal) + " (INCENDIE MAITRISE) "
    end
  end

  else if (@vl_type= XZAEC_FMC_Inondation)
  select @vl_groupe_nominal = "UNE INONDATION ",
	  @vl_groupe_verbal = "LA CHAUSSEE EST INONDEE "

  else if (@vl_type= XZAEC_FMC_Obstacle)
  begin
  	select @vl_nature_evenement = nature
#ifdef HIST
    from HIS..FMC_NAT
#else
    from EXP..FMC_NAT 
#endif
	where numero=@vl_numero_fmc and cle=@vl_cle
  	select @vl_groupe_nominal = "UN OBSTACLE ("+rtrim(@vl_nature_evenement)+") SUR LA CHAUSSEE ",
	@vl_groupe_verbal = "IL Y A UN OBSTACLE ("+rtrim(@vl_nature_evenement)+") SUR LA CHAUSSEE "
  end

  else if (@vl_type= XZAEC_FMC_FlaqueHuile)
  select @vl_groupe_nominal = "LA PRESENCE D'HUILE SUR LA CHAUSSEE ",
	@vl_groupe_verbal = "DE L'HUILE S'EST REPANDUE SUR LA CHAUSSEE "

  else if (@vl_type= XZAEC_FMC_AnimalErrant)
  select @vl_groupe_nominal = "UN ANIMAL ERRANT SUR LA CHAUSSEE ",
	@vl_groupe_verbal = "UN ANIMAL ERRE SUR LA CHAUSSEE "

  else if (@vl_type= XZAEC_FMC_AireFermee)
  select @vl_groupe_nominal = "LA FERMETURE DES ACCES ET SERVICES ",
	@vl_groupe_verbal = "L'ACCES ET LES SERVICES SONT FERMES "

  else if (@vl_type= XZAEC_FMC_Salage)
  select @vl_groupe_nominal = "DES OPERATIONS DE SALAGE ",
	@vl_groupe_verbal = "DES OPERATIONS DE SALAGE SONT EN COURS "

  else if (@vl_type= XZAEC_FMC_PietonChaussee)
  select @vl_groupe_nominal = "DES PIETONS SUR LA CHAUSSEE ",
	@vl_groupe_verbal = "DES PIETONS SE DEPLACENT SUR LA CHAUSSEE "

  else if (@vl_type= XZAEC_FMC_Manifestation)
  select @vl_groupe_nominal = "UNE MANIFESTATION ",
	@vl_groupe_verbal = "DES HOMMES MANIFESTENT "

  else if (@vl_type= XZAEC_FMC_GreveESCOTA)
  select @vl_groupe_nominal = "UNE GREVE ESCOTA ",
	@vl_groupe_verbal = "UNE PARTIE DU PERSONNEL ESCOTA EST EN GREVE "

  else if (@vl_type= XZAEC_FMC_Escargot)
  select @vl_groupe_nominal = "UNE OPERATION ESCARGOT ",
	@vl_groupe_verbal = "VOUS ALLEZ RENCONTRER UNE OPERATION ESCARGOT "

  else if (@vl_type= XZAEC_FMC_Travaux)
  select @vl_groupe_nominal = "DES TRAVAUX ",
	@vl_groupe_verbal = "VOUS ALLEZ RENCONTRER DES TRAVAUX "

  else if (@vl_type= XZAEC_FMC_ZoneStockage)
  begin
    select @vl_voies_retrecies = voies_retrecies
#ifdef HIST
         from HIS..FMC_TRH
#else
         from EXP..FMC_TRH
#endif
         where numero=@vl_numero_fmc and cle=@vl_cle
         group by numero, cle having horodate = max(horodate)
    if @vl_voies_retrecies = XDC_VRAI
    begin
      select @vl_groupe_nominal = "UNE ZONE DE STOCKAGE POIDS LOURDS ",
             @vl_groupe_verbal  = "UNE ZONE DE STOCKAGE POIDS LOURDS EST MISE EN PLACE "
    end
    else
    begin
      select @vl_groupe_nominal = "DES TRAVAUX ",
             @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER DES TRAVAUX "
    end
  end

  else if (@vl_type= XZAEC_FMC_ChausseDeter)
  select @vl_groupe_nominal = "UNE CHAUSSEE DETERIORE ",
	@vl_groupe_verbal = "LA CHAUSSEE EST ABIMEE "

  else if (@vl_type= XZAEC_FMC_FrontiereFermee)
  select @vl_groupe_nominal = "LA FRONTIERE FERMEE ",
	@vl_groupe_verbal = "LA FRONTIERE EST FERMEE "

  else if (@vl_type= XZAEC_FMC_AltercationUsager)
  select @vl_groupe_nominal = "UNE ALTERCATION ENTRE USAGERS ",
	@vl_groupe_verbal = "DES USAGERS SE BAGARRENT "

  else if (@vl_type= XZAEC_FMC_Agression)
  select @vl_groupe_nominal = "UNE AGRESSION ",
	@vl_groupe_verbal = "UNE AGRESSION VIENT DE SE PRODUIRE "

  else if (@vl_type= XZAEC_FMC_MalaiseUsager)
  select @vl_groupe_nominal = "UN MALAISE",
	@vl_groupe_verbal = "UN USAGER  A ETE PRIS D'UN MALAISE "

  else if (@vl_type= XZAEC_FMC_EvtHorsReseau)
  select @vl_groupe_nominal = "UN EVENEMENT HORS RESEAU",
	@vl_groupe_verbal = "UN EVENEMENT EST SITUE EN DEHORS DU RESEAU "

  else if (@vl_type= XZAEC_FMC_HoldUpVol)
  select @vl_groupe_nominal = "UN HOLD UP",
	@vl_groupe_verbal = "UN AGENT ESCOTA A ETE BRAQUE "

  else if (@vl_type= XZAEC_FMC_VehMarcheArr)
  select @vl_groupe_nominal = "UN VEHICULE EN MARCHE ARRIERE ",
	  @vl_groupe_verbal = "UN VEHICULE CIRCULE EN MARCHE ARRIERE "

  else if (@vl_type = XZAEC_FMC_TraitChaussees)
  begin
    /* recherche du type d'operation et du Pr de tete */
    select @vl_PR1=PR, @vl_type_evenement = type_de_perturbation 
#ifdef HIST
    from HIS..FMC_MET 
#else
    from EXP..FMC_MET 
#endif
    where numero=@vl_numero_fmc and cle=@vl_cle
    if ( @vl_type_evenement = XZAEC_TRT_CH_Salage ) 
    begin
      select @vl_groupe_nominal = "DES OPERATIONS DE SALAGE ",
             @vl_groupe_verbal  = "DES OPERATIONS DE SALAGE SONT EN COURS "
    end
    else if ( @vl_type_evenement = XZAEC_TRT_CH_Deneigement ) 
    begin
      select @vl_groupe_nominal = "DES OPERATIONS DE DENEIGEMENT ",
             @vl_groupe_verbal  = "DES OPERATIONS DE DENEIGEMENT SONT EN COURS "
    end
    else 
    begin
      select @vl_groupe_nominal = "UN TRAITEMENT DES CHAUSSEES ",
             @vl_groupe_verbal  = "UN TRAITEMENT DES CHAUSSEES EST EN COURS "
    end
  end

  else if ( @vl_type = XZAEC_FMC_EchDeconseille)
  begin
    select 
         @vl_groupe_nominal = "DES DIFFICULTES D'ACCES A ",
         @vl_groupe_verbal  = "ON NOUS INFORME QU'IL EST DECONSEILLE D'EMPRUNTER "
    select
	 @vl_entree1 = entree1,
	 @vl_entree2 = entree2,
	 @vl_sortie1 = sortie1,
	 @vl_sortie2 = sortie2
#ifdef HIST
    from HIS..FMC_ECH
#else
    from EXP..FMC_ECH
#endif
    where numero = @vl_numero_fmc
    and cle = @vl_cle

    select  @vl_nombre = convert(int,@vl_entree1) + convert(int,@vl_entree2) + convert(int,@vl_sortie1) + convert(int,@vl_sortie2)
    if @vl_nombre = 1
    begin
      select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " LA BRETELLE"
      if (@vl_entree1=1 or @vl_entree2=1) select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE"
      else select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE SORTIE"
    end
    else
    begin
      select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " LES BRETELLES"
      if (@vl_entree1=1 or @vl_entree2=1) 
      begin 
        if (@vl_sortie1=1 or @vl_sortie2=1) select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE ET DE SORTIE"
        else select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE"
      end
      else
      begin
        select @vl_groupe_verbal =  rtrim(@vl_groupe_verbal) + " DE SORTIE"
      end
    end
      select @vl_nom_pt_car = E.nom , @vl_abrev_pt_car = E.sortie ,
	     @vl_sortie_usager = E.sortie , @vl_indic_bifurc = bifurcation
      from CFG..RES_ECH E where E.numero=@vl_numero_pt_car
      if ( @vl_indic_bifurc = XDC_VRAI )
      begin
          select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE LA BIFURCATION " + upper(@vl_nom_pt_car)
      end
      else
      begin
        select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE L'ECHANGEUR " + rtrim(@vl_sortie_usager) + " " + 
  				upper(@vl_nom_pt_car)
      end
  end

  else if (@vl_type = XZAEC_FMC_ChausseeGliss)
  begin
    /* recherche du type d'evenement et du Pr de tete */
    select @vl_PR1=PR, @vl_type_evenement = type_de_perturbation
#ifdef HIST
    from HIS..FMC_MET
#else
    from EXP..FMC_MET
#endif
    where numero=@vl_numero_fmc and cle=@vl_cle
    if ( @vl_type_evenement = XZAEC_MET_NeigeChaus )
    begin
      select @vl_groupe_nominal = "DE LA NEIGE SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "LA CHAUSSEE EST ENNEIGEE "
    end
    else if ( @vl_type_evenement = XZAEC_MET_Verglas )
    begin
      select @vl_groupe_nominal = "UN RISQUE DE VERGLAS ",
             @vl_groupe_verbal  = "IL Y A DES RISQUES DE VERGLAS "
    end
    else if ( @vl_type_evenement = XZAEC_MET_GreleChaus)
    begin
      select @vl_groupe_nominal = "DE LA GRELE SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "IL Y A DE LA GRELE SUR LA CHAUSSEE"
    end
    else if ( @vl_type_evenement = XZAEC_CH_GLIS_Huile)
    begin
      select @vl_groupe_nominal = "DE L'HUILE SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "IL Y A DE L'HUILE SUR LA CHAUSSEE"
    end
    else if ( @vl_type_evenement = XZAEC_CH_GLIS_Carburant)
    begin
      select @vl_groupe_nominal = "DU CARBURANT SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "IL Y A DU CARBURANT SUR LA CHAUSSEE"
    end
    else if ( @vl_type_evenement = XZAEC_CH_GLIS_Gasoil)
    begin
      select @vl_groupe_nominal = "DU GASOIL SUR LA CHAUSSEE ",
             @vl_groupe_verbal  = "IL Y A DU GASOIL SUR LA CHAUSSEE"
    end
    else
    begin
      select @vl_groupe_nominal = "UNE CHAUSSEE GLISSANTE ",
             @vl_groupe_verbal  = "LA CHAUSSEE EST GLISSANTE "
    end
  end

  else if (@vl_type= XZAEC_FMC_BasculTravaux)
  select @vl_groupe_nominal = "UN BASCULEMENT DE CIRCULATION ",
	  @vl_groupe_verbal = "LA CIRCULATION SE FAIT A DOUBLE SENS SUR LA CHAUSSEE OPPOSEE "

  else
  select @vl_groupe_nominal = "UN EVENEMENT ",
	@vl_groupe_verbal = "IL Y A UN EVENEMENT "

  /************************************************************************************************************************/
  /* calcul de l'element localisation                                                                                     */
  /************************************************************************************************************************/

  /************************************************************************************************************************/
  /* recherche du km                                                                                                      */
  /************************************************************************************************************************/
  if (@vl_sens_chaussee = 0 )
  begin
    select @vl_km = " ENTRE LE KM " + convert ( char(3) , (@vl_PR / 1000) + 1 ) + " ET LE KM " + 
				  convert ( char(3) , (@vl_PR1/ 1000)  ) 
  end
  else
  begin
    if   ( @vl_PR - ((@vl_PR/1000) * 1000)  not between 100 and 900 )
    begin 
      select @vl_km = " AU NIVEAU DU KM " + convert (char(3), (round(@vl_PR,-3)/1000))
    end
    else
    begin
      if ( (@vl_sens_chaussee=1 and @vl_autoroute != 8) or (@vl_sens_chaussee=2 and @vl_autoroute = 8) ) 
		     select @vl_km =  " APRES LE KM " + convert(char(3) , @vl_PR / 1000)
      else select @vl_km =  " APRES LE KM " + convert(char(3) , (@vl_PR / 1000) +  1 )
    end
  end

  /************************************************************************************************************************/
  /* recherche du point caracteristique s'il y en a un (HORS SECTION COURANTE)                                            */
  /************************************************************************************************************************/
  if @vl_pt_car > 0
  /********************************************************AIRE************************************************************/
  begin
    if @vl_pt_car = XDC_POINT_CARACT_AIRE 
    begin
  	select @vl_nom_pt_car =  nom ,
		@vl_abrev_pt_car = abreviation
  	from CFG..RES_AIR 
	where numero=@vl_numero_pt_car
        select @vl_localisation = "SUR L'AIRE DE "+ upper(@vl_nom_pt_car)
    end
    /******************************************************ECHANGEUR*********************************************************/
    else if @vl_pt_car = XDC_POINT_CARACT_ECHANGEUR 
    begin
      select @vl_nom_pt_car = E.nom , @vl_abrev_pt_car = E.abreviation ,
	     @vl_sortie_usager = E.sortie , @vl_indic_bifurc = bifurcation
      from CFG..RES_ECH E where E.numero = @vl_numero_pt_car
      if ( @vl_indic_bifurc = XDC_VRAI )
      begin
        if (@vl_sens = 7 or @vl_sens = 8 )
          select @vl_localisation = "DANS LA BRETELLE D'ENTREE DE LA BIFURCATION " + upper(@vl_nom_pt_car)
        else if (@vl_sens = 3 or @vl_sens = 4 )
          select @vl_localisation = "DANS LA BRETELLE DE SORTIE DE LA BIFURCATION " +  upper(@vl_nom_pt_car)
        else
          select @vl_localisation = "DANS LA BRETELLE DE LA BIFURCATION "+ upper(@vl_nom_pt_car)
      end
      else
      begin
        if (@vl_sens = 7 or @vl_sens = 8 )
        select @vl_localisation = "DANS LA BRETELLE D'ENTREE DE L'ECHANGEUR " + rtrim(@vl_sortie_usager) + " " + 
  				upper(@vl_nom_pt_car)
        else if (@vl_sens = 3 or @vl_sens = 4 )
        select @vl_localisation = "DANS LA BRETELLE DE SORTIE DE L'ECHANGEUR " +  rtrim(@vl_sortie_usager) + " " + 
  				upper(@vl_nom_pt_car)
        else
        select @vl_localisation = "DANS LA BRETELLE DE L'ECHANGEUR "+ rtrim(@vl_sortie_usager) + " " + 
  				upper(@vl_nom_pt_car)
      end
      /* si l'evenement concerne un echangeur la localisation a deja ete calculee dans le gn ou le gv */
      if (@vl_type=XZAEC_FMC_FermetureEch or @vl_type=XZAEC_FMC_EchDeconseille) select @vl_localisation=" "
    end
    /********************************************************PEAGE***********************************************************/
    else if @vl_pt_car = XDC_POINT_CARACT_PEAGE 
    begin
      select @vl_nom_pt_car =  nom ,
		@vl_abrev_pt_car = abreviation
	from CFG..RES_PEA where numero=@vl_numero_pt_car
      select @vl_localisation = "AU PEAGE DE " + upper(@vl_nom_pt_car)
    end
    /****************************************************LIT D ARRET*********************************************************/
    else if @vl_pt_car = XDC_POINT_CARACT_LIT_ARRET 
    begin
	select @vl_nom_pt_car = nom,
		@vl_abrev_pt_car = nom
	from CFG..EQT_GEN
	where numero = @vl_numero_pt_car and type = XDC_CAPT_LIT_ARRET

      /* dans le cas du lit d'arret exclusivement, le numero du lit est porte par le nom */
      select @vl_num_id_pt_car = convert(tinyint,right(rtrim(@vl_nom_pt_car),1)) 
      select @vl_localisation = "SUR LE LIT D'ARRET NUMERO " + convert (char(2),@vl_num_id_pt_car)

    end
  end
  /*********************************************EN SECTION COURANTE *******************************************************/
  else
  begin
    /* l'evenement est-il dans un tunnel ? */
    if @vl_sens != 0
    begin
      select @vl_nom_tunnel = S.nom_tunnel , @vl_portion=P.numero
      from CFG..RES_POR P, CFG..RES_SEG S 
      where P.numero=S.portion and P.autoroute=@vl_autoroute and S.sens=@vl_sens and 
	  @vl_PR between S.PR_debut and S.PR_fin  /* and S.tunnel=1 pour recuperer la portion dans tous les cas */
    end
    else
    begin
      select @vl_nom_tunnel = S.nom_tunnel , @vl_portion=P.numero
      from CFG..RES_POR P, CFG..RES_SEG S 
      where P.numero=S.portion and P.autoroute=@vl_autoroute and S.sens=2 and 
	  @vl_PR between S.PR_debut and S.PR_fin  /* and S.tunnel=1 pour recuperer la portion dans tous les cas */
    end

    if char_length(rtrim(@vl_nom_tunnel))>1 
    begin
      select @vl_localisation = "DANS LE TUNNEL DE "+ upper(@vl_nom_tunnel)
    end
    else
    begin
      /* on recherche s'il y a un echangeur ou une bifurcation a moins de 500 m */
      select @vl_PR_ech_proche=E.PR , @vl_nom_ech_proche=E.nom ,
	     @vl_sortie_proche=E.sortie , @vl_indic_bifurc=E.bifurcation ,
	     @vl_voies_sortie_sens1=E.voies_sortie_sens1, @vl_voies_sortie_sens2=E.voies_sortie_sens2
      from  CFG..RES_ECH E, CFG..RES_POR P
      where P.numero = E.portion and
			    P.autoroute=@vl_autoroute and  abs(@vl_PR-E.PR) <= 500 and
			    (@vl_sens_chaussee = 1 and E.voies_sortie_sens1 != 0 or 
			    @vl_sens_chaussee = 2 and E.voies_sortie_sens2 != 0 )
      if @vl_PR_ech_proche is not null
      begin
	/* mantiss 748 */
        if (@vl_indic_bifurc = XDC_VRAI)
          select @vl_localisation = "A LA HAUTEUR DE LA BIFURCATION " + upper(@vl_nom_ech_proche)
        else
          select @vl_localisation = "A LA HAUTEUR DE LA SORTIE " + rtrim(@vl_sortie_proche) +
	                             " " + upper(@vl_nom_ech_proche)
      end
      else
      begin
        /* on recherche la sortie ou la bifurcation amont */
        if ( @vl_sens_chaussee = 1 )
        begin
          select @vl_PR_ech_amont=E.PR , @vl_nom_ech_amont=E.nom ,
                 @vl_sortie_amont = E.sortie  , @vl_indic_bifurc = E.bifurcation
	  from  CFG..RES_ECH E, CFG..RES_POR P
	  where P.numero = E.portion and
		    P.autoroute = @vl_autoroute and  E.voies_sortie_sens1>0 and 
			    (E.PR<@vl_PR and @vl_autoroute != 8  or E.PR>@vl_PR and @vl_autoroute = 8)
	  group by P.autoroute
	  having P.numero = E.portion and
		    P.autoroute = @vl_autoroute and  E.voies_sortie_sens1>0 and 
			    (E.PR<@vl_PR and @vl_autoroute != 8 and E.PR=max(E.PR)
			       or E.PR>@vl_PR and @vl_autoroute = 8 and E.PR=min(E.PR))
          if ( @vl_PR_ech_amont is not null )
	  begin
	    /* mantiss 748 */
            if (@vl_indic_bifurc = XDC_VRAI)
              select @vl_localisation = 
		      convert( char(3), round( abs(@vl_PR-@vl_PR_ech_amont), -3)/1000) + 
					 " KM APRES LA BIFURCATION " + upper(@vl_nom_ech_amont)
            else 
	      select @vl_localisation = 
		      convert( char(3), round( abs(@vl_PR-@vl_PR_ech_amont), -3)/1000) + 
	                 " KM APRES LA SORTIE " + rtrim(@vl_sortie_amont) + " " + upper(@vl_nom_ech_amont)
	  end
          else
	  begin
            /* on recherche la limite amont de concession */
	    if (@vl_autoroute= 1 ) 
	    begin
	      select @vl_limite = " APRES  L'ECHANGEUR 29 AIX OUEST"
	      select @vl_distance = @vl_PR-17900
            end
	    else if (@vl_autoroute= 2) 
	    begin
	      select @vl_limite = " APRES LA TURBIE"
	      select @vl_distance = @vl_PR
            end
	    else if (@vl_autoroute= 3 ) 
	    begin
	      select @vl_limite = " APRES L'ECHANGEUR 5 LA PENNE"
	      select @vl_distance = @vl_PR-17900
            end
	    else if (@vl_autoroute= 4 ) 
	    begin
	      select @vl_limite = " APRES L'ECHANGEUR 5 LA PENNE"
	      select @vl_distance = @vl_PR-2518
            end
	    else if (@vl_autoroute= 5 ) 
	    begin
	      select @vl_limite = " APRES L'ECHANGEUR PUYRICARD"
	      select @vl_distance = @vl_PR-23310
            end
	    else if (@vl_autoroute= 6 ) 
	    begin
	      select @vl_limite = " APRES LA BIFURCATION VERS A8"
	      select @vl_distance = @vl_PR
            end
	    else if (@vl_autoroute= 7 ) 
	    begin
	      select @vl_limite = " APRES LA BIFURCATION VERS A52"
	      select @vl_distance = @vl_PR
            end
	    else if (@vl_autoroute= 8 ) 
	    begin
	      select @vl_limite = " APRES LA BIFURCATION VERS A8"
	      select @vl_distance = 52800- @vl_PR 
            end
	    else 
	    begin
	      select @vl_limite = " APRES ??????????????"
	      select @vl_distance = @vl_PR 
            end
	    if @vl_distance < 10 select @vl_localisation = "10 M" + @vl_limite
	    else if @vl_distance < 1000 select @vl_localisation = convert(char(3), @vl_distance ) + " M" + @vl_limite
	    else select @vl_localisation = convert ( char(3), (round(@vl_distance,-3)/1000)) + " KM" + @vl_limite
	  end
        end 
        else if ( @vl_sens_chaussee = 2)
        begin
          select @vl_PR_ech_amont=E.PR , @vl_nom_ech_amont=E.nom ,
                 @vl_sortie_amont = E.sortie , @vl_indic_bifurc = E.bifurcation
	  from  CFG..RES_ECH E,CFG..RES_POR P
	  where P.numero = E.portion and
		  P.autoroute = @vl_autoroute and  E.voies_sortie_sens2>0 and 
			    (E.PR>@vl_PR and @vl_autoroute != 8 or E.PR<@vl_PR and @vl_autoroute = 8)
	  group by P.autoroute
	  having P.numero = E.portion and
		  P.autoroute = @vl_autoroute and  E.voies_sortie_sens2>0 and 
			     (E.PR>@vl_PR and @vl_autoroute != 8 and E.PR=min(E.PR) 
			       or E.PR<@vl_PR and @vl_autoroute = 8 and E.PR=max(E.PR))
          if ( @vl_PR_ech_amont is not null )
	  begin
	    /* mantiss 748 */
            if (@vl_indic_bifurc = XDC_VRAI)
              select @vl_localisation = 
		      convert( char(3), round( abs(@vl_PR_ech_amont-@vl_PR), -3)/1000) + 
	              " KM APRES LA BIFURCATION " + upper(@vl_nom_ech_amont)
            else 
	      select @vl_localisation = 
		      convert( char(3), round( abs(@vl_PR_ech_amont-@vl_PR), -3)/1000) + 
		      " KM APRES LA SORTIE " + rtrim(@vl_sortie_amont) + " " + upper(@vl_nom_ech_amont)
	  end
          else
	  begin
            /* on recherche la limite amont de concession */
	    if (@vl_autoroute= 1 ) 
	    begin
	      select @vl_limite = " APRES LA FRONTIERE ITALIENNE"
	      select @vl_distance = 224000-@vl_PR
            end
	    else if (@vl_autoroute= 2) 
	    begin
	      select @vl_limite = " APRES MONACO"
	      select @vl_distance = 3000-@vl_PR
            end
	    else if (@vl_autoroute= 3 ) 
	    begin
	      select @vl_limite = " APRES TOULON"
	      select @vl_distance = 67700-@vl_PR
            end
	    else if (@vl_autoroute= 4 ) 
	    begin
	      select @vl_limite = " APRES LA BIFURCATION VERS A52"
	      select @vl_distance = 5375-@vl_PR
            end
	    else if (@vl_autoroute= 5 ) 
	    begin
	      select @vl_limite = " APRES LA SAULCE"
	      select @vl_distance = 153153-@vl_PR
            end
	    else if (@vl_autoroute= 6 ) 
	    begin
	      select @vl_limite = " APRES LA BIFURCATION VERS A50"
	      select @vl_distance = 26077-@vl_PR
            end
	    else if (@vl_autoroute= 7 ) 
	    begin
	      select @vl_limite = " APRES AURIOL"
	      select @vl_distance = 2900-@vl_PR
            end
	    else if (@vl_autoroute= 8 ) 
	    begin
	      select @vl_limite = " APRES LA BIFURCATION A57/A570"
	      select @vl_distance = @vl_PR - 6350
            end
	    else 
	    begin
	      select @vl_limite = " APRES ??????????????"
	      select @vl_distance = @vl_PR 
            end
	    if @vl_distance < 10 select @vl_localisation = "10 M" + @vl_limite
	    else if @vl_distance < 1000 select @vl_localisation = convert(char(3), @vl_distance ) + " M" + @vl_limite
	    else select @vl_localisation = convert ( char(3), (round(@vl_distance,-3)/1000)) + " KM" + @vl_limite
	  end
        end
        else if ( @vl_sens_chaussee = 0)
        begin
          select @vl_localisation = NULL /*"DANS LES 2 SENS DE CIRCULATION" */
        end
      end
    end
  end

  /************************************************************************************************************************/
  /* recherche des voies bloquees                                                                                         */
  /************************************************************************************************************************/

  if ( @vl_VL = XDC_VOIE_BLOQUEE or @vl_VL=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VL_T="R"
  else if ( @vl_VL = XDC_VOIE_INEXISTANTE ) select @vl_VL_T="I"
  else select @vl_VL_T="A"

  if ( @vl_VM1 = XDC_VOIE_BLOQUEE or @vl_VM1=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM1_T="R"
  else if ( @vl_VM1 = XDC_VOIE_INEXISTANTE ) select @vl_VM1_T="I"
  else select @vl_VM1_T="A"

  if ( @vl_VM2 = XDC_VOIE_BLOQUEE or @vl_VM2=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM2_T="R"
  else if ( @vl_VM2 = XDC_VOIE_INEXISTANTE ) select @vl_VM2_T="I"
  else select @vl_VM2_T="A"

  if ( @vl_VR = XDC_VOIE_BLOQUEE or @vl_VR=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VR_T="R"
  else if ( @vl_VR = XDC_VOIE_INEXISTANTE ) select @vl_VR_T="I"
  else select @vl_VR_T="A"

  select @vl_config_voies_b=@vl_VR_T+@vl_VM2_T+@vl_VM1_T+ @vl_VL_T

  if @vl_config_voies_b =  "AAAR"
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE DROITE."
  else if @vl_config_voies_b = "AIAR"
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE DROITE."
  else if @vl_config_voies_b =  "AIIR"
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE DROITE."
  else if @vl_config_voies_b = "AARR"
    select @vl_voies_bloquees =  "BLOQUE LES DEUX VOIES DE DROITE."
  else if @vl_config_voies_b = "AIRR"
    select @vl_voies_bloquees =  "BLOQUE LES DEUX VOIES DE DROITE."
  else if @vl_config_voies_b = "ARRR" 
    select @vl_voies_bloquees = "BLOQUE LES TROIS VOIES DE DROITE."
  else if @vl_config_voies_b = "RAAA" 
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE GAUCHE."
  else if @vl_config_voies_b = "RIAA" 
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE GAUCHE."
  else if @vl_config_voies_b = "RIIA" 
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE GAUCHE."
  else if @vl_config_voies_b = "RRAA" 
    select @vl_voies_bloquees = "BLOQUE LES DEUX VOIES DE GAUCHE."
  else if @vl_config_voies_b = "RIRA" 
    select @vl_voies_bloquees = "BLOQUE LES DEUX VOIES DE GAUCHE."
  else if @vl_config_voies_b = "RRRA"
    select @vl_voies_bloquees = "BLOQUE LES TROIS VOIES DE GAUCHE."
  else if @vl_config_voies_b = "ARAA"
    select @vl_voies_bloquees = "BLOQUE UNE VOIE CENTRALE."
  else if @vl_config_voies_b = "AARA"
    select @vl_voies_bloquees = "BLOQUE UNE VOIE CENTRALE."
  else if @vl_config_voies_b = "AIRA" 
    select @vl_voies_bloquees = "BLOQUE LA VOIE CENTRALE."
  else if @vl_config_voies_b = "ARRA" 
    select @vl_voies_bloquees =  "BLOQUE LES VOIES CENTRALES."  
  else if @vl_config_voies_b = "RRAR" 
    select @vl_voies_bloquees =  "BLOQUE TOUTES LES VOIES SAUF UNE VOIE CENTRALE."
  else if @vl_config_voies_b = "RARR" 
    select @vl_voies_bloquees =  "BLOQUE TOUTES LES VOIES SAUF UNE VOIE CENTRALE."
  else if @vl_config_voies_b = "RIAR"
    select @vl_voies_bloquees = "BLOQUE TOUTES LES VOIES SAUF LA VOIE CENTRALE."
  else if @vl_config_voies_b = "RAAR"
    select @vl_voies_bloquees = "BLOQUE TOUTES LES VOIES SAUF LES VOIES CENTRALES."
  else if @vl_config_voies_b = "RARA"
    select @vl_voies_bloquees = "BLOQUE LA VOIE DE GAUCHE ET UNE VOIE CENTRALE."
  else if @vl_config_voies_b = "ARAR"
    select @vl_voies_bloquees = "BLOQUE UNE VOIE CENTRALE ET LA VOIE DE DROITE."
  else if @vl_config_voies_b = "IIIR"
    select @vl_voies_bloquees = "BLOQUE LA BRETELLE."
  else if @vl_config_voies_b in ("RRRR","RIRR","RIIR")
    select @vl_voies_bloquees = "BLOQUE TOUTES LES VOIES DE CIRCULATION."
  else if @vl_config_voies_b in ("AAAA","AIIA","AIAA")
    select @vl_voies_bloquees = null

  if @vl_voies_bloquees is not null 
  begin
    if ( @vl_type = XZAEC_FMC_Accident) select @vl_msg_voies_bloquees="CET ACCIDENT "+rtrim(@vl_voies_bloquees)
    else
      select @vl_msg_voies_bloquees="CET INCIDENT "+rtrim(@vl_voies_bloquees)
  end
  /************************************************************************************************************************/
  /* recherche des voies neutralisees                                                                                     */
  /************************************************************************************************************************/

  if ( @vl_VL=XDC_VOIE_NEUTRALISEE or @vl_VL=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VL_T="R"
  else if ( @vl_VL = XDC_VOIE_INEXISTANTE ) select @vl_VL_T="I"
  else select @vl_VL_T="A"

  if ( @vl_VM1=XDC_VOIE_NEUTRALISEE or @vl_VM1=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM1_T="R"
  else if ( @vl_VM1 = XDC_VOIE_INEXISTANTE ) select @vl_VM1_T="I"
  else select @vl_VM1_T="A"

  if ( @vl_VM2=XDC_VOIE_NEUTRALISEE or @vl_VM2=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM2_T="R"
  else if ( @vl_VM2 = XDC_VOIE_INEXISTANTE ) select @vl_VM2_T="I"
  else select @vl_VM2_T="A"

  if ( @vl_VR=XDC_VOIE_NEUTRALISEE or @vl_VR=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VR_T="R"
  else if ( @vl_VR = XDC_VOIE_INEXISTANTE ) select @vl_VR_T="I"
  else select @vl_VR_T="A"

  select @vl_config_voies_n=@vl_VR_T+@vl_VM2_T+@vl_VM1_T+ @vl_VL_T


  if @vl_config_voies_n =  "AAAR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES TROIS VOIES DE GAUCHE" 
  else if @vl_config_voies_n = "AIAR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE GAUCHE" 
  else if @vl_config_voies_n =  "AIIR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE."
  else if @vl_config_voies_n = "AARR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE GAUCHE"
  else if @vl_config_voies_n = "AIRR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE"
  else if @vl_config_voies_n = "ARRR" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE"
  else if @vl_config_voies_n = "RAAA" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES TROIS VOIES DE DROITE"
  else if @vl_config_voies_n = "RIAA" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE DROITE"
  else if @vl_config_voies_n = "RIIA" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE"
  else if @vl_config_voies_n = "RRAA" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE DROITE"
  else if @vl_config_voies_n = "RIRA" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE"
  else if @vl_config_voies_n = "RRRA"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE"
  else if @vl_config_voies_n = "ARAA"
    select @vl_neutralisation = "LA CIRCULATION EST COUPEE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies_n = "AARA"
    select @vl_neutralisation = "LA CIRCULATION EST COUPEE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies_n = "AIRA" 
    select @vl_neutralisation = "LA CIRCULATION EST COUPEE SUR LA VOIE CENTRALE"
  else if @vl_config_voies_n = "ARRA" 
    select @vl_neutralisation = "LA CIRCULATION EST COUPEE SUR LES VOIES CENTRALES"
  else if @vl_config_voies_n = "RRAR" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies_n = "RARR" 
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies_n = "RIAR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE CENTRALE"
  else if @vl_config_voies_n = "RAAR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES VOIES CENTRALES"
  else if @vl_config_voies_n = "RARA"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE ET UNE VOIE CENTRALE"
  else if @vl_config_voies_n = "ARAR"
    select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE ET UNE VOIE CENTRALE"
  else if @vl_config_voies_n in ("RRRR","RIRR","RIIR")
    select @vl_neutralisation = "TOUTES LES VOIES DE CIRCULATION SONT FERMEES"
  else if @vl_config_voies_n = "IIIR"
    select @vl_neutralisation = "LA BRETELLE EST FERMEE"
  else if ( @vl_config_voies_n in ("AAAA","AIIA","AIAA","IIIA") and @vl_voies_bloquees is null )
    select @vl_neutralisation = NULL /*"LA CIRCULATION S'ECOULE NORMALEMENT"*/

  /************************************************************************************************************************/
  /* recherche des config ou on a des voies bloquees ou neutralisees ou bloquees et neutralisees                          */
  /************************************************************************************************************************/

  if ( @vl_config_voies_n like @C_MOTIF_VOIES_BN  or  @vl_config_voies_b like @C_MOTIF_VOIES_BN )

  begin

  if ( @vl_VL=XDC_VOIE_NEUTRALISEE or @vl_VL=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VL=XDC_VOIE_BLOQUEE ) select @vl_VL_T="R"
  else if ( @vl_VL = XDC_VOIE_INEXISTANTE ) select @vl_VL_T="I"
  else select @vl_VL_T="A"

  if ( @vl_VM1=XDC_VOIE_NEUTRALISEE or @vl_VM1=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM1=XDC_VOIE_BLOQUEE ) select @vl_VM1_T="R"
  else if ( @vl_VM1 = XDC_VOIE_INEXISTANTE ) select @vl_VM1_T="I"
  else select @vl_VM1_T="A"

  if ( @vl_VM2=XDC_VOIE_NEUTRALISEE or @vl_VM2=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM2=XDC_VOIE_BLOQUEE  ) select @vl_VM2_T="R"
  else if ( @vl_VM2 = XDC_VOIE_INEXISTANTE ) select @vl_VM2_T="I"
  else select @vl_VM2_T="A"

  if ( @vl_VR=XDC_VOIE_NEUTRALISEE or @vl_VR=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VR=XDC_VOIE_BLOQUEE  ) select @vl_VR_T="R"
  else if ( @vl_VR = XDC_VOIE_INEXISTANTE ) select @vl_VR_T="I"
  else select @vl_VR_T="A"

  select @vl_config_voies_b_ou_n=@vl_VR_T+@vl_VM2_T+@vl_VM1_T+ @vl_VL_T

  end
  /************************************************************************************************************************/
  /* recherche des config ou on a a la fois des voies bloquees et d'autres neutralisees                                   */
  /************************************************************************************************************************/

  if ( @vl_config_voies_n like @C_MOTIF_VOIES_BN  and  @vl_config_voies_b like @C_MOTIF_VOIES_BN )

  begin

  select @vl_config_voies=@vl_config_voies_b_ou_n                                  

  if @vl_config_voies =  "AAAR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES TROIS VOIES DE GAUCHE" 
  else if @vl_config_voies = "AIAR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE GAUCHE" 
  else if @vl_config_voies =  "AIIR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE."
  else if @vl_config_voies = "AARR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE GAUCHE"
  else if @vl_config_voies = "AIRR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE"
  else if @vl_config_voies = "ARRR" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE"
  else if @vl_config_voies = "RAAA" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES TROIS VOIES DE DROITE"
  else if @vl_config_voies = "RIAA" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE DROITE"
  else if @vl_config_voies = "RIIA" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE"
  else if @vl_config_voies = "RRAA" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE DROITE"
  else if @vl_config_voies = "RIRA" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE"
  else if @vl_config_voies = "RRRA"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE"
  else if @vl_config_voies = "ARAA"
    sElect @vl_blocal_neutral = "LA CIRCULATION EST COUPEE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies = "AARA"
    select @vl_blocal_neutral = "LA CIRCULATION EST COUPEE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies = "AIRA" 
    select @vl_blocal_neutral = "LA CIRCULATION EST COUPEE SUR LA VOIE CENTRALE"
  else if @vl_config_voies = "ARRA" 
    select @vl_blocal_neutral = "LA CIRCULATION EST COUPEE SUR LES VOIES CENTRALES"
  else if @vl_config_voies = "RRAR" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies = "RARR" 
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR UNE VOIE CENTRALE"
  else if @vl_config_voies = "RIAR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE CENTRALE"
  else if @vl_config_voies = "RAAR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES VOIES CENTRALES"
  else if @vl_config_voies = "RARA"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE DROITE ET UNE VOIE CENTRALE"
  else if @vl_config_voies = "ARAR"
    select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE ET UNE VOIE CENTRALE"
  else if @vl_config_voies in ("RRRR","RIRR","RIIR")
    select @vl_blocal_neutral = "TOUTES LES VOIES DE CIRCULATION SONT FERMEES"
  else if @vl_config_voies =  "IIIR"
    select @vl_blocal_neutral = "LA BRETELLE EST FERMEE"
  else if ( @vl_config_voies in ("AAAA","AIIA","AIAA","IIIA") and @vl_voies_bloquees is null )
    select @vl_blocal_neutral = NULL /*"LA CIRCULATION S'ECOULE NORMALEMENT"*/

  end

  /************************************************************************************************************************/
  /* synthese de l'analyse des voies bloquees, neutralisees  et bloquees / neutralisees                                   */
  /************************************************************************************************************************/
  if @vl_blocal_neutral is not null
  begin
    select @vl_msg_neutralisation=rtrim(@vl_blocal_neutral) /*+" AU NIVEAU DE L'INCIDENT."*/
    select @vl_msg_voies_bloquees=null                                         
  end
  else if @vl_neutralisation is not null
  begin
    select @vl_msg_neutralisation=rtrim(@vl_neutralisation) /*+" AU NIVEAU DE L'INCIDENT."*/
    select @vl_msg_voies_bloquees=null                                         
  end

  if @vl_type in (XZAEC_FMC_Delestage, XZAEC_FMC_Deviation, XZAEC_FMC_Basculement, XZAEC_FMC_BasculTravaux)
  begin
    select @vl_msg_voies_bloquees=null, @vl_msg_neutralisation=null
  end

  /************************************************************************************************************************/
  /* recherche de la presence des secours dans le cas des accidents et des vehicules en feu                               */
  /************************************************************************************************************************/
  if ( @vl_type = XZAEC_FMC_VehFeu or @vl_type = XZAEC_FMC_Accident )
  begin
    select @vl_compte=count(*) 
#ifdef HIST 
    from HIS..ACT_GEN G,HIS..ACT_DEP D 
#else
    from EXP..ACT_GEN G,EXP..ACT_DEP D
#endif
         where  
         G.numero=D.actio and G.sit=D.sit and G.evenement=@vl_numero_fmc and G.cle=@vl_cle and
         D.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
    if @vl_compte > 0 select @vl_secours="LES SERVICES DE SECOURS SONT SUR PLACE."
    else
    begin
      select @vl_compte=count(*) 
#ifdef HIST 
      from HIS..ACT_GEN G,HIS..ACT_SIS S 
#else
      from EXP..ACT_GEN G,EXP..ACT_SIS S
#endif
         where
         G.numero=S.actio and G.sit=S.sit and G.evenement=@vl_numero_fmc and G.cle=@vl_cle and
         S.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
      if @vl_compte > 0 select @vl_secours="LES SERVICES DE SECOURS SONT SUR PLACE."
      else
      begin
        select @vl_compte=count(*)
#ifdef HIST 
        from HIS..ACT_GEN G,HIS..ACT_INT I 
#else
        from EXP..ACT_GEN G,EXP..ACT_INT I
#endif
	where
	   G.numero=I.actio and G.sit=I.sit and G.evenement=@vl_numero_fmc and G.cle=@vl_cle and
	  I.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
        if @vl_compte > 0 select @vl_secours=" LES SERVICES DE SECOURS SONT SUR PLACE."
	else
	begin
          select  @vl_secours=NULL
	end
      end
    end
  end
  else
  begin
    select @vl_secours=NULL
  end

  /************************************************************************************************************************/
  /************************************************************************************************************************/
  /*                                      creation du message tdp pour cet evenement                                      */
  /************************************************************************************************************************/
  /************************************************************************************************************************/

  /************************************************************************************************************************/
  /************************************************************************************************************************/
  /*                                      creation du message tfm pour l'evenement traite                                 */
  /************************************************************************************************************************/
  /************************************************************************************************************************/

  if @vl_fmc_confirmee = XDC_FAUX
  begin
    select @vl_msg_tfm = "- ON NOUS SIGNALE "
		       + rtrim(ltrim(@vl_groupe_nominal)) + " " + rtrim(ltrim(@vl_localisation)) 
		       + ", " + rtrim(ltrim(@vl_km)) +"."
  end
  else if ( @vl_fmc_confirmee = XDC_VRAI )
  begin
    select @vl_msg_tfm = "- " + rtrim(ltrim(@vl_groupe_verbal)) + " " + rtrim(ltrim(@vl_localisation)) 
			+ ", " + rtrim(ltrim(@vl_km)) + ". " + rtrim(ltrim(@vl_etendue_evt)) + " " 
			+ rtrim(ltrim(@vl_msg_tdp)) + "  " + @vl_msg_neutralisation + " " +
			+ @vl_msg_voies_bloquees + ". " + @vl_secours  
			/*+ "(" + @vl_config_voies_b + " " + @vl_config_voies_n + " " + @vl_config_voies_b_ou_n + ")"*/
			/*+ "(neutrlisation" + @vl_neutralisation + " blocal_neutral " + @vl_blocal_neutral + ")"*/
  end

  /* restrictions sur certains evenements */

  if ( @vl_type in  (XZAEC_FMC_VehArrete,XZAEC_FMC_VehPanne ))
  begin
     /* on calcule les synthese pour les veh arretes et en panne si une voie est restreinte  en section courante     */
     /* ou en bretelle a condition qu'une voie soit restreinte que ce soit une voie circulee ou la BAU */
     if ( @vl_config_voies_b like @C_MOTIF_VOIES_BN or
          @vl_config_voies_n like @C_MOTIF_VOIES_BN or
          @vl_config_voies_b_ou_n like @C_MOTIF_VOIES_BN or  
	    ( @vl_sens > 2 and  @vl_BAU in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)))
     begin
       insert #TMP_SYNTHESE_TFM values(1,@vl_numero_fmc, @vl_cle,@vl_autoroute_trie,@vl_sens_trie,@vl_PR,@vl_msg_tfm,@vl_debut)
       update #TMP_SYNTHESE_TFM set valide=1 where valide=0 and autoroute=@vl_autoroute_trie and sens=@vl_sens_trie
     end
     else
     begin
       if @va_trace_in is not null
	 select "evenement filtre car",  @vl_config_voies_b, @vl_config_voies_n,@vl_config_voies_b_ou_n,@vl_BAU_T, @vl_sens
     end
  end
/*
  else if ( @vl_type = XZAEC_FMC_Basculement )
  begin
    select @vl_type_cause = type 
#ifdef HIST
    from HIS..FMC_GEN 
#else
    from EXP..FMC_GEN 
#endif
    where cle=@vl_cle_cause and numero = @vl_cause 
    if @vl_type_cause != XZAEC_FMC_Travaux
    begin
      if ( @vl_VR_I = XDC_VOIE_SENS_INVERSE)
	begin
          insert #TMP_SYNTHESE_TFM values(1,@vl_numero_fmc, @vl_cle,@vl_autoroute_trie,@vl_sens_trie,@vl_PR,@vl_msg_tfm,@vl_debut)
          update #TMP_SYNTHESE_TFM set valide=1 where valide=0 and autoroute=@vl_autoroute_trie and sens=@vl_sens_trie
        end
    end
  end
*/
  else
  begin
    insert #TMP_SYNTHESE_TFM values(1,@vl_numero_fmc, @vl_cle,@vl_autoroute_trie,@vl_sens_trie,@vl_PR,@vl_msg_tfm,@vl_debut)
    update #TMP_SYNTHESE_TFM set valide=1 where valide=0 and autoroute=@vl_autoroute_trie and sens=@vl_sens_trie
  end

/*    insert #TMP_SYNTHESE_TFM values(@vl_numero_fmc, @vl_cle,@vl_autoroute_trie,@vl_sens_trie,@vl_PR,@vl_msg_tfm,@vl_debut)*/
  fetch fmc_en_cours into
       @vl_numero_fmc, @vl_cle, @vl_type, @vl_debut , @vl_fin , 
       @vl_debut_prevu , @vl_fin_prevue , @vl_cause, @vl_cle_cause,
       @vl_fmc_confirmee,@vl_autoroute , @vl_PR , @vl_sens,
       @vl_pt_car , @vl_numero_pt_car ,
       @vl_position,
       @vl_VR, @vl_VM2,@vl_VM1,@vl_VL,@vl_BAU,
       @vl_VR_I, @vl_VM2_I,@vl_VM1_I,@vl_VL_I,@vl_BAU_I

end /*fin de la boucle recursive du curseur */

/* rajout d'un en-tete horodate */
select @vl_fin_calcul = getdate()
select @vl_duree_calcul = datediff (millisecond,@vl_lancement_calcul, @vl_fin_calcul)
select @vl_en_tete_horodate = "SYNTHESE DES MESSAGES RVA CALCULEE EN " + convert (char(5),@vl_duree_calcul) +
	                      " MILLIEMES DE  SECONDES LE " + 
		              convert ( char (10) , getdate() , 103 ) + ' ' + convert ( char (8) , getdate() , 8 )
insert #TMP_SYNTHESE_TFM values(1,0, 0,0,0,0,@vl_en_tete_horodate,null)

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                 calcul des temps de parcours                                                         */
/************************************************************************************************************************/
/************************************************************************************************************************/




/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                 on retourne de la procedure en renvoyant la synthese                                 */
/************************************************************************************************************************/
/************************************************************************************************************************/

if @va_trace_in is null
  select rtrim(ltrim(message_tfm)) from #TMP_SYNTHESE_TFM where valide=1 order by autoroute asc, sens desc, PR*sens asc, debut asc
else 
  select rtrim(ltrim(message_tfm))+'('+convert(char(6),numero)+'/'+convert(char(2),cle)+')' from #TMP_SYNTHESE_TFM where valide=1 order by autoroute asc, sens desc, PR*sens asc ,debut asc

return (XDC_OK)

go
