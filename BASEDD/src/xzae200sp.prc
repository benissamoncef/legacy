/*E*/
/*  Fichier : $Id: xzae200sp.prc,v 1.74 2020/12/07 11:06:38 gesconf Exp $        $Revision: 1.74 $        $Date: 2020/12/07 11:06:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae200sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module ROL : Recherche des renseignements necessaires a la realisation d'un message TFM
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     31/01/01  : Creation
* Cluchague     06/06/02  : traitement des voies bloquees et/ou neutralisees pour picto rabattement
* JMG		28/02/03 : modif fin evenement sur Fermeture Echangeur
* Cluchague     23/03/03  : filtre en debut de temps parcours uniquement en zone concernee (V1.3)
* Cluchague     09/05/03  : filtre sur tdp pour que n'apparaissent plus des tdp concernant des bouchons
			    sur aix + >= et <= pour la recherche de la destination tdp pour que LA BOCCA
			    soit selectionnee v1.4
* Cluchague     09/08/03  : ESCOTA prevoit un retour a la normale V1.3
* Cluchague     31/10/03  : rtrim sur BRETELLE sinon D'ENTREE pas vu pas pris V1.4
* Cluchague     04/11/03  : on remplace pourcent R pourcent car mot reserve sccs V1.5
* Cluchague     04/11/03  : idem V1.6
* Cluchague     04/11/03  : regression toutes les voies sont fermees pour bouchon V1.7
* Cluchague     04/03/04  : on ne dit plus "la circulation s'ecoule normallement..." V1.9
* Niepceron	28/02/05  : Ajout du cas XZAEC_BOU_Accordeon V1.10 DEM428
* CHI           13/04/05  : Rajout sous type "grele sur chaussee" et "chaussee mouillee" v1.11 DEM456
* JMG           17/02/04 : ajout de datex dans le select (SAGA) 1.12
* CHI		20/07/05  : Ajout de la nature de l'obstacle pour FMCObstacle DEM487 v1.13
* Niepceron	24/08/05  : Introduction des tdp invalides DEM502 v1.13
* Niepceron     26/08/05  : filtre sur tdp pour que n'apparaissent plus des tdp concernant des bouchons
			    sur aix v1.14 DEM258
				+ Modif msg pour r�ouverture �changeur DEM407
* Niepceron	22/09/05  : Correction du filtre final DEM258 v1.15
* CHI		30/09/05  : modif formulation de la nature de l'obstacle DEM487 v1.16
* JMG		25/10/05 : ajout PMVA 1.17
* Niepceron	17/08/06 : Suppression du traitement des zdp n'aynat pas de libelle d'origine ou de destination DEM579 v1.18
* Niepceron	29/08/06 : Correction faute d'orthographe CHAUSSE DEM556 v1.19
* Cluchague	16/11/06 : Correction bug matiere dangereuse (a 0 considerait mat dang ) v1.20
* Niepceron	11/12/06 : Correction destination aval DEM602 v1.21 v1.22
* Niepceron	08/03/07 : Correction de l'objet fax pour neige sur chaussee FI362/CI DEM631 
			   Correction calul TDP (modif appel suivant sens + arrondi ) DEM 602 v1.23
* Lecrivain	11/04/07 : Ajout nature FMC "Pieton" et "Aute evt." (DEM 639) 1.24
* JPL		20/04/07 : Prise en compte du type fmc "Vehicule en marche arriere" (DEM 643) v1.25
* JPL		10/05/07 : Prise en compte du type fmc "Traitement Chaussees" (DEM 644) v1.26
* JPL		10/05/07 : Prise en compte du type fmc "Echangeur deconseille" (DEM 645) v1.27
* JPL		11/05/07 : Prise en compte du type fmc "Chaussee glissante" (DEM 646) v1.28
* JPL		06/06/07 : Prise en compte du type fmc "Basculement pour travaux" (DEM 647) v1.29
* PNI		17/07/07 : Correction recuperation PR_tete "Basculement pour travaux" (DEM 647) v1.30
* JPL		31/10/07 : Correction cause pour "Basculement pour travaux" (DEM 647) v1.31
* JMG		03/12/08 : ajout validation dans FMC_ACC 1.32 1.33 DEM/852
* JPL		27/02/09 : Prise en compte du champ 'Feu maitrise' pour "Vehicule en feu" (DEM 845) 1.34
* PNI		30/03/09 : Ajout retour d'un flag cause_operation pour affichage cause sur PMVA DEM876 1.36
* JMG           02/10/09        : secto DEM887 1.37
* PNI 		09/10/09	: Traitement du picto en sens inverse DEM915 1.36
* PNI		22/10/09 : suppression du point a la fin de "VOIE DE GAUCHE"du vl_blocal_neutral "...VOIE DE GAUCHE" FI842/CI FT 57 v1.37
* PNI		06/01/10 : modification du group nominal pour une fermeture �changeur DEM926 v1.38
* PNI		30/03/10 : Suppression du calcul du temps compl�mentaire inutile  DEM929 V1.39
* JPL		25/08/10 : Prise en compte du type Fmc "Zone de stockage" (DEM 942) 1.40
* PNI		28/06/11 : Prise en compte de echangeur de bifucation 98 1.41
* JPL		12/10/11 : Ajout retour du champ "origine" de l'evenement (DEM 995)  1.42
* LCL		02/12/11 : Ajout vitesse et vitesse opposee (DEM 1015)  1.43
* JPL		20/01/12 : Distinction entre numero pt. car. et numero (id.) sortie echangeur ou lit d'arret  1.44
* JPL		20/01/12 : Mention de la saturation a l'origine d'un bouchon avec tete en sortie (DEM 1010)  1.45
* VR		15/02/12	: Ajout de la vl_zone vl_scenario _vl_transitoire en sortie (DEM/1016)
* JPL		06/07/12 : Noms sortie usager et indic. bifurcation pris en table echangeurs, et retournes (DEM 1033)  1.47
* JPL		06/09/12 : Prise en compte du champ 'Feu maitrise' pour "Incendie de vegetation" (DEM 1037)  1.48
* PNI		23/08/12 : modif pour la recuperation du scenario de regul en ajoutant la clause and scenario!=-1 1.49
* PNI		23/08/12 : modif pour la recuperation du scenario de regul en ajoutant la clause and scenario>0 1.51
* PNI		13/11/14 : ajout lib cause et lib secours DEM1097 v1.52 1.53
* PNI		13/11/14 : Mise en valeur absolue de la duree pour �viter plantage lors de la conversion  en cas d'erreur de saisie 1.54
* PNI		22/01/15 : si pas de transitoire trouv� on recherche les donnees de la regulation sans contrainte sur transitoire (cas PRV en panne)  1.55 FI3174
* PNI		29/06/15 : Prise en compte des num�ros de lit > 9 DEM1133 1.56 
* JMG		16/06/15 : ajout horodates en sortie lotPA 1.56 DEM1130
* PNI		16/09/15 : Traitement du type de veh pour contre sens 1.57 DEM1146
* JMG		16/01/16 : cumul longueur de bouchon si FMC SAGA 1.58
* JMG		26/11/16 : ajout majeur lot 23 1.60
* PNI		13/12/16 : Remise en place du filtrage PR pour la direction de l'autoroute FI3955 DEM1207
* JPL		17/02/17 : Prise en compte du champ 'Complement rappel' pour depanneurs (DEM 1192)  1.62
* JMG		20/03/17 : ajout type_cause en sortie 1.63
* JMG		26/06/17 : convergence 1.64
* JMG		15/10/17 : info fermeture toulon 1.65
* JMG		08/03/18 : ajout personnel travaux 1.66 DEM1282
* JPL		28/06/18 : Retour du nombre de passagers pour les FMC de classe Vehicule (DEM 1295)  1.67
* JMG		02/09/18 : ajout cause et cle_cause en sortie LOT25 DEM1301 1.68
* LCL		24/10/18 : correction nom de proc 1.69
* JPL		15/03/19 : Retour des donn�es Manifestation historis�es les plus r�centes (DEM 1327)  1.70
* JPL		08/04/19 : Retour de l'indicateur d'�coulement pour Fmc Accident et Vehicule (DEM 1326)  1.71
* JPL		23/09/19 : Mention de la nature de l'incendie dans les groupes nominaux (DEM 1352)  1.72
* JPL		25/09/19 : Retour de l'indicateur de pr�sence de bouchon pour Fmc Basculement pour travaux (DEM 1346)  1.73
* PNI		25/11/20 : si evt sans cause, r�cup�re le cause_operation � partir du type de l'evt SAE197 1.74
* LCL	  15/05/23	: Attente remontee replication sur FMC_HIS_DER pour COCKPIT
* LCL	  17/05/23	: Attente remontee replication sur FMC_BOU pour COCKPIT
* LCL	  31/05/23	: Suppression remontee replication sur FMC_HIS_DER/FMC_BOU pour COCKPIT
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE200
* 
* Arguments en entree
*  int numero_fmc    
*  tinyint cle 
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les renseignements necessaires a l'etablissement des messages TFM
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

if exists (select 1 from sysobjects where name = 'XZAE200' and type = 'P')
	drop procedure XZAE200
go


create procedure XZAE200
        @va_numero_fmc int ,
	@va_cle_fmc    tinyint   
as

declare @vl_type           smallint     ,
	@vl_origine        char(25)     ,
        @vl_type_cause     smallint     ,
        @vl_cause_operation     tinyint     ,
        @vl_debut          datetime     ,
        @vl_debut_formatee char(25)     ,
	@vl_fin            datetime     ,
	@vl_fin_formatee   char(25)     ,   
	@vl_fmc_confirmee  bit          ,
	@vl_autoroute      tinyint      ,
	@vl_autoroute1     tinyint      ,
	@vl_PR             int          ,
	@vl_PR1            int          ,
	@vl_PR_min         int          ,
	@vl_PR_max         int          ,
        @vl_PR_Min_dtp  int,
	@vl_PR_Max_dtp  int,
	@vl_sens           tinyint      ,
	@vl_sens_chaussee  tinyint      ,
	@vl_pt_car         tinyint      ,
	@vl_numero_pt_car  tinyint      ,	-- numero d ordre en base, interne
	@vl_num_id_pt_car  tinyint      ,	-- numero identifiant un point caracteristique (lit d arret)
	@vl_nom_pt_car     char(25)     ,
	@vl_sortie_usager  char(20)     ,
	@vl_indic_bifurc   bit          ,
	@vl_position       tinyint      ,
	@vl_pt_car1        tinyint      ,
	@vl_numero_pt_car1 tinyint      ,
	@vl_nom_pt_car1    char(25)     ,
	@vl_sortie_tete    char(20)     ,
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
        @vl_nom_autoroute  char(4)      ,
	@vl_lib_autoroute  char(50)  ,
	@vl_lib_sens       char(50)  ,
	@vl_duree          int          ,
	@vl_duree_formatee char(25)     ,
	@vl_secours        char(255)    ,
        @vl_debut_reel     char(25)     ,
	@vl_localisation   char(50)     ,
	@vl_neutralisation char(100)    ,
	@vl_blocal_neutral char(100)    ,
	@vl_msg_neutralisation char(100),
        @vl_morts          tinyint      ,
	@vl_blesses_graves tinyint      ,
	@vl_blesses_legers tinyint      ,
	@vl_vl             tinyint      ,
	@vl_pl             tinyint      ,
	@vl_moto           tinyint      ,
	@vl_car            tinyint      ,
	@vl_remorque       tinyint      ,
	@vl_type_perturbation     tinyint      ,
        @vl_nature_evenement char(100)   ,
        @vl_victimes       char(100)    ,
	@vl_gravite        char(50)     ,
	@vl_vehicules      char(100)    ,
        @vl_chaussee_glissante char(25) ,
        @vl_conseil        char(200)     ,
        @vl_groupe_nominal char(100)    ,   
	@vl_groupe_verbal  char(100)    ,                            
        @vl_fin_evenement  char(100)    ,
	@vl_longueur_bouchon char(10)   ,
	@vl_etendue_evt    char(120)   ,
        @vl_periode        char(50)     ,
	@vl_retour_normale char(100)    ,
        @vl_visibilite     char(50)     ,
	@vl_debut_prevu    datetime     ,
	@vl_fin_prevue     datetime     ,
	@vl_fmc_prevue     bit          ,
	@vl_fmc_terminee   bit          ,
	@vl_cause          int          ,
	@vl_cle_cause      tinyint      ,
	@vl_consequence    int          ,
	@vl_cle_consequence tinyint     ,
	@vl_cause_toujours char(100)     ,
	@vl_compte         int          ,
	@vl_voies_bloquees char(100)    ,
	@vl_msg_voies_bloquees char(100),
	@vl_config_voies_b_n_totale   char(9)      ,
	@vl_config_voies   char(4)      ,
	@vl_config_voies_n char(4)      ,
	@vl_config_voies_b char(4)      ,
	@vl_config_voies_b_ou_n  char(4)      ,
	@vl_config_voies_i   char(4)      ,
	@vl_config_voies_n_i char(4)      ,
	@vl_config_voies_b_i char(4)      ,
	@vl_config_voies_b_ou_n_i  char(4)      ,
	@vl_type_bouchon   tinyint      ,
	@vl_numero_carburant tinyint    ,
	@vl_flag_ferme     bit          ,
	@vl_distance_visibilite int,
	@vl_longueur_arrondie int		,
	@vl_longueur_reelle int		,
	@vl_longueur_reelle2 int		,
	@vl_objet_fax	char(100),
	@vl_cr 			int	,
	@vl_personnalites	bit	,
	@vl_agent_escota	bit	,
	@vl_type_vehicule	tinyint	,
	@vl_nb_passagers	tinyint ,
	@vl_nb_victimes		tinyint	,
	@vl_nb_vehicules	tinyint ,
	@vl_matieres_dangereuses bit    ,
	@vl_presence_mat tinyint    ,
	@vl_animaux_vivants      bit,
	@vl_complement_rappel	char(20),
	@vl_voies_retrecies	 bit,
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
	@vl_temps_cumule        smallint,
	@vl_PR_dest_amont       int,
        @vl_numero_dest_amont   smallint,
        @vl_TFM_amont           char(50),
        @vl_PMV_amont           char(50),
	@vl_PR_dest_aval        int,
        @vl_numero_dest_aval    smallint,
        @vl_TFM_aval            char(50),
        @vl_PMV_aval            char(50),
	@vl_horodate            datetime,
	@vl_msg_tfm             char(255),
	@vl_type_travaux	int,
	@vl_nb_manifestants	smallint,
	@vl_violence		bit,
	@vl_perception_peage	bit,
	@vl_datex               tinyint,
	@vl_horaire_debut_prevu char(5),
	@vl_horaire_fin_prevue  char(5),
	@vl_nuit		char(10),
	@vl_nature_travaux	smallint,
	@vl_sous_conc		smallint,
	@vl_feu_maitrise	tinyint,
	@vl_vitesse	tinyint,
	@vl_personnel	tinyint,
	@vl_vitesse_opposee tinyint,
	@vl_zone					varchar(100),
	@vl_numero_zone		smallint,
	@vl_scenario				varchar(100),
	@vl_vitesse_transitoire 	varchar(100),
	@vl_lib_cause	varchar(100),
	@vl_lib_secours varchar(100),
	@vl_l int,
	@vl_com_fmc_init char(50),
	@vl_com_fmc_init2 char(50),
	@vl_majeur tinyint,
	@vl_zone_urbaine tinyint,
	@vl_intervention tinyint,
	@vl_ecoulement tinyint,
	@vl_code_nature		tinyint,
	@vl_nature_incendie	varchar(100),
	@vl_bouchon		tinyint, 
  @vl_replication_ok int, 
  @vl_timeout int,
  @vl_numero int
 

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/
select @vl_cr = XDC_OK
  select @vl_cause =0
  select @vl_cle_cause = 0

select @vl_type=type, @vl_origine=origine_creation,
	@vl_debut=debut, @vl_fin=fin, @vl_debut_prevu=debut_prevu, @vl_fin_prevue=fin_prevue,
	@vl_cause=cause, @vl_cle_cause=cle_cause, @vl_datex=datex, @vl_com_fmc_init = com_fmc_init
	    from EXP..FMC_GEN where numero=@va_numero_fmc and cle=@va_cle_fmc

/* Adaptation PIT pour attente remontee replication dans FMC_HIS_DER lors de la creation de la FMC*/
/* if @vl_origine like "PIT%" or @vl_origine=null
begin
*/
  /* On attend l'arrivee de la FMC dans FMC_HIS_DER */
/*
  select @vl_replication_ok=0
  select @vl_timeout=0

  while ( @vl_replication_ok = 0 and @vl_timeout < 50 )
  begin
    select @vl_timeout=@vl_timeout+1
    select @vl_numero=numero from EXP..FMC_HIS_DER
    where EXP..FMC_HIS_DER.numero = @va_numero_fmc and
      EXP..FMC_HIS_DER.cle = @va_cle_fmc

    if @@rowcount > 0
      select @vl_replication_ok = 1
    else

  while ( @vl_replication_ok = 0 and @vl_timeout < 50 )
  begin
    select @vl_timeout=@vl_timeout+1
    select @vl_numero=numero from EXP..FMC_BOU
    where EXP..FMC_BOU.numero = @va_numero_fmc and
      EXP..FMC_BOU.cle = @va_cle_fmc

    if @@rowcount > 0
      select @vl_replication_ok = 1
    else
      waitfor delay "00:00:00.100"
  end

end
*/
select @vl_horaire_debut_prevu = convert(char(5),datepart(hour,@vl_debut_prevu))
select @vl_horaire_fin_prevue = convert(char(5),datepart(hour, @vl_fin_prevue))
select @vl_nuit = rtrim(convert(char(2),datepart(day,@vl_debut_prevu))) + '/' +
			rtrim(convert(char(2),datepart(month,@vl_debut_prevu)))


select @vl_fmc_confirmee=confirme_ou_signale,@vl_autoroute=autoroute , @vl_PR=PR , @vl_sens=sens,
       @vl_position=position_sur_aire_ou_echangeur,
       @vl_VR=VR, @vl_VM2=VM2,@vl_VM1=VM1,@vl_VL=VL,@vl_BAU=BAU,
       @vl_VR_I=VR_I, @vl_VM2_I=VM2_I,@vl_VM1_I=VM1_I,@vl_VL_I=VL_I,@vl_BAU_I=BAU_I,@vl_majeur = majeur,
       @vl_ecoulement = isnull(ecoulement,XDC_FAUX)
       from EXP..FMC_HIS_DER where numero=@va_numero_fmc and cle=@va_cle_fmc

/*suis je en zone urbaine*/
select @vl_zone_urbaine = count(*)
	from CFG..ZON_URB
	where autoroute = @vl_autoroute
	and debut <= @vl_PR
	and fin >= @vl_PR
	and sens = @vl_sens
if @@rowcount=0
select @vl_zone_urbaine=0

if @vl_zone_urbaine > 0
select @vl_zone_urbaine=1

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      renseignements generaux valables pour tous les types de fmc                     */
/************************************************************************************************************************/
/************************************************************************************************************************/

/************************************************************************************************************************/
/* recherche des champs generaux                                                                                        */
/************************************************************************************************************************/
  select @vl_num_id_pt_car=null
  select @vl_sortie_usager=" "
  select @vl_cause_operation=1
  select @vl_victimes=" "
  select @vl_gravite = " "
  select @vl_vehicules=" "
  select @vl_chaussee_glissante=" "
  select @vl_conseil=" ",
         @vl_longueur_bouchon=" ",
         @vl_etendue_evt="",
         @vl_periode=" ",
         @vl_retour_normale=" ",
         @vl_visibilite=" ",
	 @vl_PR1=null,
	 @vl_longueur_reelle = null,
	 @vl_personnalites=0,
	 @vl_agent_escota=0,
	 @vl_type_vehicule=0,
	 @vl_nb_passagers=0,
	 @vl_nb_victimes=0,
	 @vl_nb_vehicules=0,
	 @vl_matieres_dangereuses=0,
	 @vl_animaux_vivants=0,
	 @vl_voies_retrecies=0,
	 @vl_entree1=0,
	 @vl_entree2=0,
	 @vl_sortie1=0,
	 @vl_sortie2=0,
	 @vl_flag_ferme=0,
	 @vl_numero_carburant=0,
	 @vl_abrev_pt_car="",
	 @vl_type_bouchon=0,
	 @vl_type_travaux=0,
	 @vl_nb_manifestants=0,
	 @vl_config_voies="",
	 @vl_numero_zone=0,
	 @vl_zone="",
	 @vl_scenario="",
	 @vl_vitesse_transitoire="",
	 @vl_zone_urbaine=0,
	 @vl_personnel = 0,
	 @vl_bouchon = XDC_FAUX
	 

/************************************************************************************************************************/
/* recherche de l'autoroute et du sens                                                                                  */
/************************************************************************************************************************/
if ( @vl_sens = 3 or @vl_sens =7 or @vl_sens = 1 ) select @vl_sens_chaussee = 1
else if ( @vl_sens = 4 or @vl_sens =8  or  @vl_sens = 2 ) select @vl_sens_chaussee = 2

select @vl_lib_autoroute=libelle_autoroute,
	@vl_lib_sens = libelle_sens
from CFG..SAD_AUT
where autoroute = @vl_autoroute 
and sens = @vl_sens
#ifdef CI
and PR_fin > @vl_PR
and PR_debut <= @vl_PR
#endif

/************************************************************************************************************************/
/* recherche du point caracteristique s'il y en a un                                                                    */
/************************************************************************************************************************/
select @vl_sous_conc=0
if @vl_pt_car > 0
begin
  if @vl_pt_car = XDC_POINT_CARACT_AIRE 
  	select @vl_nom_pt_car =  nom ,
		@vl_abrev_pt_car = abreviation,
		@vl_sous_conc = sous_concessionnaire
  	from CFG..RES_AIR 
	where numero=@vl_numero_pt_car
  else if @vl_pt_car = XDC_POINT_CARACT_ECHANGEUR 
  begin
    select @vl_num_id_pt_car = @vl_numero_pt_car
    select @vl_nom_pt_car = nom,
	   @vl_abrev_pt_car = abreviation,
	   @vl_sortie_usager = sortie,
	   @vl_indic_bifurc = bifurcation
    from CFG..RES_ECH where numero=@vl_numero_pt_car
  end
  else if @vl_pt_car = XDC_POINT_CARACT_PEAGE 
	select @vl_nom_pt_car =  nom ,
		@vl_abrev_pt_car = abreviation
	from CFG..RES_PEA where numero=@vl_numero_pt_car
  else if @vl_pt_car = XDC_POINT_CARACT_LIT_ARRET 
  begin
	select @vl_nom_pt_car = nom,
		@vl_abrev_pt_car = nom
	from CFG..EQT_GEN
	where numero = @vl_numero_pt_car and type = XDC_CAPT_LIT_ARRET

	/* dans le cas du lit d'arret exclusivement, le numero du lit est porte par le nom */
	if charindex("-",@vl_nom_pt_car)=0
		 select @vl_num_id_pt_car = convert(tinyint,right(rtrim(@vl_nom_pt_car),1))
	else
		select @vl_num_id_pt_car = convert(tinyint,substring(@vl_nom_pt_car,charindex("-",@vl_nom_pt_car)+1,2))
  end
end

/************************************************************************************************************************/
/* calcul de la duree                                                                                                   */
/************************************************************************************************************************/
select @vl_duree=abs(datediff(minute,@vl_debut,getdate()))

if ( @vl_duree > 1440 ) select @vl_duree_formatee = convert (char(3), @vl_duree / 1440)  + " JOURS"
else if ( @vl_duree > 60 ) select @vl_duree_formatee  = convert(char(2),@vl_duree / 60) + " HEURES"
else select @vl_duree_formatee  = convert( char(2),@vl_duree ) + " MINUTES"

select @vl_debut_formatee = convert(char(10),@vl_debut,103) + ' ' + convert(char(8),@vl_debut,8)
select @vl_fin_formatee = convert(char(10),@vl_fin,103) + ' ' + convert(char(8),@vl_fin,8)

/************************************************************************************************************************/
/* calcul du retour a la normale                                                                                        */
/************************************************************************************************************************/
if @vl_fin_prevue is not null
begin
  if ( datepart(day,@vl_fin_prevue) = datepart(day,getdate()) and datepart(month,@vl_fin_prevue) =datepart (month,getdate()) )
    select @vl_retour_normale  = "RETOUR A LA NORMALE PREVU AUJOURD'HUI VERS "+convert(char(8),@vl_fin_prevue,8) 
  else if ( datepart(day,@vl_fin_prevue) = datepart(day,getdate())+1  and datepart(month,@vl_fin_prevue) =datepart (month,getdate()) )
    select @vl_retour_normale  = "RETOUR A LA NORMALE PREVU DEMAIN VERS "+convert(char(8),@vl_fin_prevue,8)
  else
    begin
      select @vl_retour_normale  = "RETOUR A LA NORMALE PREVU LE "+convert(char(10),@vl_fin_prevue,103) 

      select @vl_retour_normale = @vl_retour_normale + " VERS " +
				convert(char(8),@vl_fin_prevue,8)
    end
end

/************************************************************************************************************************/
/* recherche de la presence des secours                                                                                 */
/************************************************************************************************************************/
select @vl_secours=null,@vl_lib_secours=null, @vl_intervention=XDC_FAUX
select @vl_compte=count(*) from EXP..ACT_GEN G,EXP..ACT_DEP D where
       G.numero=D.actio and G.sit=D.sit and G.evenement=@va_numero_fmc and G.cle=@va_cle_fmc and 
       D.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
if @vl_compte > 0 select @vl_secours="UN DEPANNEUR EST SUR PLACE.",@vl_lib_secours="DEPANNEUR, "

select @vl_compte=count(*) from EXP..ACT_GEN G,EXP..ACT_SIS S where
       G.numero=S.actio and G.sit=S.sit and G.evenement=@va_numero_fmc and G.cle=@va_cle_fmc and 
       S.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
if @vl_compte > 0 select @vl_secours=rtrim(@vl_secours)+" LES SERVICES DE SECURITE SONT SUR PLACE.",@vl_lib_secours=@vl_lib_secours + "PATROUILLE, ",
		@vl_intervention=XDC_VRAI

select @vl_compte=count(*) from EXP..ACT_GEN G,EXP..ACT_INT I where
       G.numero=I.actio and G.sit=I.sit and G.evenement=@va_numero_fmc and G.cle=@va_cle_fmc and 
       I.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
if @vl_compte > 0 
    begin
	select @vl_secours=rtrim(@vl_secours)+" LES SERVICES DE SECOURS SONT SUR PLACE."
	select @vl_compte=count(*) from EXP..ACT_GEN G,EXP..ACT_INT I where
       G.numero=I.actio and G.sit=I.sit and G.evenement=@va_numero_fmc and G.cle=@va_cle_fmc and I.type=XDC_ACT_INT_POM and
       I.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
	if @vl_compte > 0 select @vl_lib_secours=@vl_lib_secours + "POMPIERS, "
	select @vl_compte=count(*) from EXP..ACT_GEN G,EXP..ACT_INT I where
       G.numero=I.actio and G.sit=I.sit and G.evenement=@va_numero_fmc and G.cle=@va_cle_fmc and I.type=XDC_ACT_INT_GND and
       I.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
	if @vl_compte > 0 select @vl_lib_secours=@vl_lib_secours + "GENDARMERIE, "
	select @vl_compte=count(*) from EXP..ACT_GEN G,EXP..ACT_INT I where
       G.numero=I.actio and G.sit=I.sit and G.evenement=@va_numero_fmc and G.cle=@va_cle_fmc and I.type=XDC_ACT_INT_AUTRE and
       I.annulation_intervention !=1  and G.heure_fin is null and heure_succes is not null
	if @vl_compte > 0 select @vl_lib_secours=@vl_lib_secours + "AUTRE ", @vl_intervention=XDC_VRAI
    end
    select @vl_l=LEN( @vl_lib_secours)

if @vl_l > 1
	select @vl_lib_secours=substring(@vl_lib_secours,1,@vl_l-2)
/* cas particulier d'un bouchon */
if ( @vl_secours is not null and @vl_type in (XZAEC_FMC_QueueBouchon,XZAEC_FMC_TeteBouchon )  ) 
				      select @vl_secours= "UN VEHICULE DE SERVICE EST SUR PLACE POUR VOUS SIGNALER LE BOUCHON"
/************************************************************************************************************************/
/* recherche du debut d'evenement                                                                                       */
/************************************************************************************************************************/
select @vl_debut_reel = convert(char(10),@vl_debut,103) + " A " + convert(char(8),@vl_debut,8)

/************************************************************************************************************************/
/* recherche de la localisation                                                                                         */
/************************************************************************************************************************/
select @vl_localisation= "ENTRE LE DEBUT ET LA FIN DE L'AUTOROUTE"

/************************************************************************************************************************/
/* recherche des voies bloquees                                                                                         */
/************************************************************************************************************************/
if ( @vl_VL_I = XDC_VOIE_BLOQUEE or @vl_VL_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VL_T="R"
else if ( @vl_VL_I = XDC_VOIE_INEXISTANTE ) select @vl_VL_T="I"
else select @vl_VL_T="A"

if ( @vl_VM1_I = XDC_VOIE_BLOQUEE or @vl_VM1_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM1_T="R"
else if ( @vl_VM1_I = XDC_VOIE_INEXISTANTE ) select @vl_VM1_T="I"
else select @vl_VM1_T="A"

if ( @vl_VM2_I = XDC_VOIE_BLOQUEE or @vl_VM2_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM2_T="R"
else if ( @vl_VM2_I = XDC_VOIE_INEXISTANTE ) select @vl_VM2_T="I"
else select @vl_VM2_T="A"

if ( @vl_VR_I = XDC_VOIE_BLOQUEE or @vl_VR_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VR_T="R"
else if ( @vl_VR_I = XDC_VOIE_INEXISTANTE ) select @vl_VR_T="I"
else select @vl_VR_T="A"

select @vl_config_voies_b_i=@vl_VR_T+@vl_VM2_T+@vl_VM1_T+ @vl_VL_T


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
else if @vl_config_voies_b in ("RRRR","RIRR","RIIR","IIIR")
  select @vl_voies_bloquees = "BLOQUE TOUTES LES VOIES DE CIRCULATION."
else if @vl_config_voies_b in ("AAAA","AIIA","AIAA")
  select @vl_voies_bloquees = null

  if @vl_voies_bloquees is not null select @vl_msg_voies_bloquees="CET INCIDENT "+rtrim(@vl_voies_bloquees)
/************************************************************************************************************************/
/* recherche des voies neutralisees                                                                                     */
/************************************************************************************************************************/
if ( @vl_VL_I=XDC_VOIE_NEUTRALISEE or @vl_VL_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VL_T="R"
else if ( @vl_VL_I = XDC_VOIE_INEXISTANTE ) select @vl_VL_T="I"
else select @vl_VL_T="A"

if ( @vl_VM1_I=XDC_VOIE_NEUTRALISEE or @vl_VM1_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM1_T="R"
else if ( @vl_VM1_I = XDC_VOIE_INEXISTANTE ) select @vl_VM1_T="I"
else select @vl_VM1_T="A"

if ( @vl_VM2_I=XDC_VOIE_NEUTRALISEE or @vl_VM2_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VM2_T="R"
else if ( @vl_VM2_I = XDC_VOIE_INEXISTANTE ) select @vl_VM2_T="I"
else select @vl_VM2_T="A"

if ( @vl_VR_I=XDC_VOIE_NEUTRALISEE or @vl_VR_I=XDC_VOIE_BLOQUEE_NEUTRALISEE ) select @vl_VR_T="R"
else if ( @vl_VR_I = XDC_VOIE_INEXISTANTE ) select @vl_VR_T="I"
else select @vl_VR_T="A"
select @vl_config_voies_n_i=@vl_VR_T+@vl_VM2_T+@vl_VM1_T+ @vl_VL_T


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
  select @vl_neutralisation = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE"
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
else if @vl_config_voies_n in ("RRRR","RIRR","RIIR","IIIR")
  select @vl_neutralisation = "TOUTES LES VOIES DE CIRCULATION SONT FERMEES"
else if ( @vl_config_voies_n in ("AAAA","AIIA","AIAA","IIIA") and @vl_voies_bloquees is null )
  select @vl_neutralisation = NULL

/************************************************************************************************************************/
/* recherche des config ou on a des voies bloquees ou neutralisees ou bloquees et neutralisees                          */
/************************************************************************************************************************/

if ( @vl_config_voies_n_i like "R___" or  @vl_config_voies_n_i like "_R__" or  
     @vl_config_voies_n_i like "__R_" or  @vl_config_voies_n_i like "___R" or 
     @vl_config_voies_b_i like "R___" or  @vl_config_voies_b_i like "_R__" or
     @vl_config_voies_b_i like "__R_" or  @vl_config_voies_b_i like "___R" 
   )

begin

if ( @vl_VL_I=XDC_VOIE_NEUTRALISEE or @vl_VL_I=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VL_I=XDC_VOIE_BLOQUEE ) select @vl_VL_T="R"
else if ( @vl_VL_I = XDC_VOIE_INEXISTANTE ) select @vl_VL_T="I"
else select @vl_VL_T="A"

if ( @vl_VM1_I=XDC_VOIE_NEUTRALISEE or @vl_VM1_I=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM1_I=XDC_VOIE_BLOQUEE ) select @vl_VM1_T="R"
else if ( @vl_VM1_I = XDC_VOIE_INEXISTANTE ) select @vl_VM1_T="I"
else select @vl_VM1_T="A"

if ( @vl_VM2_I=XDC_VOIE_NEUTRALISEE or @vl_VM2_I=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VM2_I=XDC_VOIE_BLOQUEE  ) select @vl_VM2_T="R"
else if ( @vl_VM2_I = XDC_VOIE_INEXISTANTE ) select @vl_VM2_T="I"
else select @vl_VM2_T="A"

if ( @vl_VR_I=XDC_VOIE_NEUTRALISEE or @vl_VR_I=XDC_VOIE_BLOQUEE_NEUTRALISEE or @vl_VR_I=XDC_VOIE_BLOQUEE  ) select @vl_VR_T="R"
else if ( @vl_VR_I = XDC_VOIE_INEXISTANTE ) select @vl_VR_T="I"
else select @vl_VR_T="A"

select @vl_config_voies_b_ou_n_i=@vl_VR_T+@vl_VM2_T+@vl_VM1_T+ @vl_VL_T

end
else
select @vl_config_voies_b_ou_n_i=@vl_config_voies_b_i


if ( @vl_config_voies_n like "R___" or  @vl_config_voies_n like "_R__" or  
     @vl_config_voies_n like "__R_" or  @vl_config_voies_n like "___R" or 
     @vl_config_voies_b like "R___" or  @vl_config_voies_b like "_R__" or
     @vl_config_voies_b like "__R_" or  @vl_config_voies_b like "___R" 
   )

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

if ( 
    (@vl_config_voies_n like "R___" or  @vl_config_voies_n like "_R__" or  
     @vl_config_voies_n like "__R_" or  @vl_config_voies_n like "___R") and 
    (@vl_config_voies_b like "R___" or  @vl_config_voies_b like "_R__" or
     @vl_config_voies_b like "__R_" or  @vl_config_voies_b like "___R" )
   )

begin

select @vl_config_voies=@vl_config_voies_b_ou_n                                  

if @vl_config_voies =  "AAAR"
  select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES TROIS VOIES DE GAUCHE" 
else if @vl_config_voies = "AIAR"
  select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LES DEUX VOIES DE GAUCHE" 
else if @vl_config_voies =  "AIIR"
  select @vl_blocal_neutral = "LA CIRCULATION NE SE FAIT PLUS QUE SUR LA VOIE DE GAUCHE"
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
  select @vl_blocal_neutral = "LA CIRCULATION EST COUPEE SUR UNE VOIE CENTRALE"
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
else if @vl_config_voies in ("RRRR","RIRR","RIIR","IIIR")
  select @vl_blocal_neutral = "TOUTES LES VOIES DE CIRCULATION SONT FERMEES"
else if ( @vl_config_voies in ("AAAA","AIIA","AIAA","IIIA") and @vl_voies_bloquees is null )
  select @vl_blocal_neutral = NULL

end

/************************************************************************************************************************/
/* synthese de l'analyse des voies bloquees, neutralisees  et bloquees / neutralisees                                   */
/************************************************************************************************************************/
if @vl_blocal_neutral is not null
  begin
    select @vl_msg_neutralisation=rtrim(@vl_blocal_neutral)+" AU NIVEAU DE L'INCIDENT."
    select @vl_msg_voies_bloquees=null                                         
  end
else if @vl_neutralisation is not null
  begin
    select @vl_msg_neutralisation=rtrim(@vl_neutralisation)+" AU NIVEAU DE L'INCIDENT."
    select @vl_msg_voies_bloquees=null                                         
  end

/************************************************************************************************************************/
/* recherche de l'objectif de la fmc                                                                                    */
/************************************************************************************************************************/
select @vl_fmc_prevue=0, @vl_fmc_terminee=0
if ( @vl_debut_prevu > getdate() and @vl_debut is null ) select @vl_fmc_prevue = 1
if ( @vl_fin is not null ) select @vl_fmc_terminee = 1

/************************************************************************************************************************/
/*                       recherche des textes pour le fax                                                               */
/************************************************************************************************************************/
select @vl_groupe_nominal = groupe_nominal,
	@vl_groupe_verbal = groupe_verbal,
	@vl_fin_evenement = fin_evenement,
	@vl_objet_fax = objet_fax
from CFG..SAD_CFG
where type = @vl_type

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement a ne pas mentionner                                          */
/************************************************************************************************************************/
/************************************************************************************************************************/
/*
if @vl_type in (XZAEC_FMC_AltercationUsager, XZAEC_FMC_MalaiseUsager,XZAEC_FMC_Agression,
		XZAEC_FMC_HoldUpVol,XZAEC_FMC_GreveESCOTA)
	select @vl_cr = XDC_ARG_INV
*/

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un vehicule arrete, en panne ou en feu                                          */
/************************************************************************************************************************/
/************************************************************************************************************************/
if @vl_type in (XZAEC_FMC_Contresens,XZAEC_FMC_VehArrete, XZAEC_FMC_VehPanne, XZAEC_FMC_VehFeu)
begin
  select @vl_type_vehicule = type_de_vehicule,
	 @vl_nb_passagers=nombre_passagers,
	 @vl_presence_mat=presence_matiere_dangereuse,
	 @vl_animaux_vivants=animaux_vivants,
	 @vl_complement_rappel=complement_rappel,
	 @vl_feu_maitrise=feu_maitrise,
	 @vl_agent_escota= vehicule_ESCOTA
	 from EXP..FMC_VEH
  where numero=@va_numero_fmc and cle=@va_cle_fmc

  if (@vl_presence_mat <> NULL and @vl_presence_mat <> 0)
    select @vl_matieres_dangereuses=1

  if @vl_type = XZAEC_FMC_VehArrete
  begin
    if ( @vl_pt_car = XDC_POINT_CARACT_LIT_ARRET )
    begin
      select @vl_groupe_nominal = "UN VEHICULE ARRETE DANS UN LIT D'ARRET",
             @vl_groupe_verbal  = "UN VEHICULE S'EST ARRETE DANS UN LIT D'ARRET",
             @vl_objet_fax = "VEHICULE ARRETE DANS LIT D'ARRET"
    end
/*
    else
    begin
      select @vl_groupe_nominal = "UN VEHICULE ARRETE SUR LA CHAUSSEE",
             @vl_groupe_verbal  = "UN VEHICULE S'EST ARRETE SUR LA CHAUSSEE",
             @vl_objet_fax = "VEHICULE ARRETE SUR LA CHAUSSEE"
    end
*/
  end

  else if @vl_type = XZAEC_FMC_VehFeu
  begin
	if @vl_feu_maitrise = XDC_VRAI
		select @vl_groupe_nominal = "UN VEHICULE ARRETE (INCENDIE MAITRISE)",
			@vl_groupe_verbal = "ON NOUS SIGNALE UN VEHICULE ARRETE (INCENDIE MAITRISE)",
			@vl_fin_evenement = @vl_fin_evenement,
			@vl_objet_fax = "VEHICULE ARRETE (INCENDIE MAITRISE)"
  end
  else if @vl_type = XZAEC_FMC_VehPanne
  begin
	select @vl_lib_cause=nature_panne from EXP..FMC_VEH where numero=@va_numero_fmc and cle=@va_cle_fmc and numero_vehicule=1
  end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'une FMC Regulation			                                            */
/************************************************************************************************************************/
/************************************************************************************************************************/
if @vl_type in (XZAEC_FMC_Regulation)
begin
  select @vl_zone = ZON.nom,
	 @vl_numero_zone = FMC.zone,
	 @vl_scenario=SCE.libelle,
	 @vl_vitesse_transitoire=transitoire,
	 @vl_PR = ZON.debut,
	 @vl_PR1 = ZON.fin
	 from EXP..FMC_REG FMC, CFG..ZON_REG ZON, CFG..REG_SCE SCE
  where FMC.numero=@va_numero_fmc and FMC.cle=@va_cle_fmc
  and ZON.numero = FMC.zone
  and ZON.numero = SCE.zone
  and SCE.numero = FMC.scenario
  and FMC.horodate in (select max(horodate) from 
			EXP..FMC_REG
			where  numero = @va_numero_fmc 
			and cle = @va_cle_fmc and scenario>0 and transitoire!=NULL and transitoire!=" ")
  if @@rowcount=0 
    select @vl_zone = ZON.nom,
	 @vl_numero_zone = FMC.zone,
	 @vl_scenario=SCE.libelle,
	 @vl_vitesse_transitoire=transitoire,
	 @vl_PR = ZON.debut,
	 @vl_PR1 = ZON.fin
	 from EXP..FMC_REG FMC, CFG..ZON_REG ZON, CFG..REG_SCE SCE
 	 where FMC.numero=@va_numero_fmc and FMC.cle=@va_cle_fmc
  	and ZON.numero = FMC.zone
  	and ZON.numero = SCE.zone
  	and SCE.numero = FMC.scenario
  	and FMC.horodate in (select max(horodate) from 
			EXP..FMC_REG
			where  numero = @va_numero_fmc 
			and cle = @va_cle_fmc and scenario>0 and transitoire!=NULL)

select @vl_groupe_nominal = "UNE REGULATION DE VITESSE",
	@vl_groupe_verbal = "LA CIRCULATION EST TRES DENSE " ,
	/*			rtrim(@vl_zone) + ". LE SCENARIO EN COURS EST " + rtrim(@vl_scenario),*/
	@vl_fin_evenement = "LA FIN DE LA REGULATION DE VITESSE",
	@vl_objet_fax = "REGULATION DE VITESSE"

  select @vl_secours = " AFIN DE MAINTENIR L'ECOULEMENT DU TRAFIC ET DE REDUIRE LES RISQUES DE PERTURBATION, " +
		"LA VITESSE EST LIMITEE SUR CETTE SECTION. " 
		select @vl_conseil = "MERCI DE RESPECTER LES PRESCRIPTIONS DE VITESSE QUI SONT INDIQUEES SUR LES PANNEAUX LUMINEUX."

if @vl_PR < @vl_PR1
begin
select @vl_PR_min = @vl_PR
select @vl_PR_max = @vl_PR1
end
else
begin
select @vl_PR_min = @vl_PR1
select @vl_PR_max = @vl_PR
end

if @vl_sens = XDC_SENS_NORD
begin
	select @vl_PR = @vl_PR_max
	select @vl_PR1 = @vl_PR_min
end
else
begin
	select @vl_PR = @vl_PR_min
	select @vl_PR1 = @vl_PR_max
end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un accident                                                               */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Accident
begin
  select @vl_morts=morts,@vl_blesses_graves=blesses_graves , @vl_blesses_legers=blesses_legers ,
	 @vl_vl=nombre_de_vl ,@vl_pl=nombre_de_pl, @vl_remorque= nombre_de_remorques,
	 @vl_car =nombre_de_car,@vl_moto = nombre_de_moto,
	 @vl_personnalites=personnalites,@vl_agent_escota=personnel_ESCOTA,
	 @vl_matieres_dangereuses=matieres_dangereuses
  from  EXP..FMC_ACC where numero=@va_numero_fmc and cle=@va_cle_fmc and zz >1
  group by numero,cle
  having validation=max(validation)

  select @vl_nb_passagers=isnull(sum(nombre_passagers),0)
  from  EXP..FMC_VEH where numero=@va_numero_fmc and cle=@va_cle_fmc

  select @vl_nb_victimes=@vl_morts+@vl_blesses_graves+@vl_blesses_legers
  select @vl_nb_vehicules=@vl_vl+@vl_pl+@vl_remorque+@vl_car+@vl_moto
  select @vl_victimes="CET ACCIDENT A FAIT "
  if @vl_blesses_legers>0 select @vl_victimes=rtrim(@vl_victimes)+" BLESSE(S) LEGER(S) "
  if @vl_blesses_graves>0 select @vl_victimes=rtrim(@vl_victimes)+" BLESSE(S) GRAVE(S) "
  if @vl_morts>0 select @vl_victimes=rtrim(@vl_victimes)+" MORT(S) "

  select @vl_gravite = "IL S'AGIT D'UN ACCIDENT "
  if ( @vl_morts+@vl_blesses_graves+@vl_blesses_legers > 0 ) select @vl_gravite = rtrim(@vl_gravite) + " MATERIEL"
  else select @vl_gravite = rtrim(@vl_gravite) + " CORPOREL"

  select @vl_vehicules="IL S'AGIT D'UN ACCIDENT ENTRE "
  if @vl_vl>0 select @vl_vehicules=rtrim(@vl_vehicules)+" "+convert (char(2) , @vl_vl) + " VOITURES "
  if @vl_pl>0 select @vl_vehicules=rtrim(@vl_vehicules)+" "+convert (char(2) , @vl_pl) + " POIDS LOURDS "
  if @vl_moto>0 select @vl_vehicules=rtrim(@vl_vehicules)+" "+convert (char(2) , @vl_moto) + " MOTOS "
  if @vl_remorque>0 select @vl_vehicules=rtrim(@vl_vehicules)+" "+convert (char(2) , @vl_remorque) + " REMORQUES "
  if @vl_car>0 select @vl_vehicules=rtrim(@vl_vehicules)+" "+convert (char(2) , @vl_car) + " CARS "

  if @vl_voies_bloquees is not null select @vl_msg_voies_bloquees="CET ACCIDENT "+rtrim(@vl_voies_bloquees)
if @vl_blocal_neutral is not null
  begin
    select @vl_msg_neutralisation=rtrim(@vl_blocal_neutral)+" AU NIVEAU DE L'ACCIDENT."
    select @vl_msg_voies_bloquees=null                                         
  end
else if @vl_neutralisation is not null
  begin
    select @vl_msg_neutralisation=rtrim(@vl_neutralisation)+" AU NIVEAU DE L'ACCIDENT."
    select @vl_msg_voies_bloquees=null                                         
  end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un bouchon (tete)                                                         */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_TeteBouchon 
begin

  /* recherche de la consequence */
  select @vl_consequence=numero,@vl_cle_consequence=cle from EXP..FMC_GEN 
		  where cause=@va_numero_fmc and cle_cause=@va_cle_fmc and type=XZAEC_FMC_QueueBouchon 

  /* recherche de la localisation de la queue de bouchon */
  select @vl_PR1 = PR, @vl_autoroute1 = autoroute,
         @vl_pt_car1=point_caracteristique, @vl_numero_pt_car1=numero_point_caracteristique
  from EXP..FMC_HIS_DER where numero=@vl_consequence and cle=@vl_cle_consequence
 
  /* recherche du type de bouchon */
  select @vl_type_bouchon = type from EXP..FMC_BOU where numero=@vl_consequence and cle=@vl_cle_consequence
 
  /* on ne mentionne pas les voies bloquees pour 1 bouchon */
  select @vl_msg_voies_bloquees = ""

  select @vl_objet_fax = "BOUCHON"

  /* determination de l'etendue du bouchon */
  exec PRC..XZAO;22 @vl_autoroute,@vl_PR,@vl_autoroute1,@vl_PR1,@vl_longueur_reelle output

  exec PRC..XZAE157 @va_numero_fmc, @va_cle_fmc, XZAEC_FMC_QueueBouchon,  @vl_com_fmc_init, 
		@vl_longueur_reelle2 output

  select @vl_longueur_reelle = @vl_longueur_reelle + @vl_longueur_reelle2
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
    if ( @vl_pt_car = XDC_POINT_CARACT_ECHANGEUR ) and ( @vl_indic_bifurc <> XDC_VRAI ) and
          ( @vl_pt_car1 is null )
    begin
      /* le numero de sortie de l'echangeur et son nom ont ete lus */
      select @vl_etendue_evt = " QUI A POUR ORIGINE LA SATURATION DE LA SORTIE " +
                                      rtrim(@vl_sortie_usager) + " " + upper(@vl_nom_pt_car)
    end
  end
  /* calcul de la nature du bouchon */
  if @vl_type_bouchon = XZAEC_BOU_Dense 
  begin
    select @vl_groupe_nominal = "UNE CIRCULATION DIFFICILE" ,
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UNE CIRCULATION DIFFICILE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
	   if ( @vl_longueur_reelle > 0 )
		select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Accordeon
  begin
    select @vl_groupe_nominal = "UNE CIRCULATION EN ACCORDEON" ,             
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UNE CIRCULATION EN ACCORDEON",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
	   if ( @vl_longueur_reelle > 0 )
		select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Ralenti
  begin
    select @vl_groupe_nominal = "UN TRAFIC RALENTI" ,             
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UN TRAFIC RALENTI",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
	   if ( @vl_longueur_reelle > 0 )
		select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Bouche
  begin
    select @vl_groupe_nominal = "UN BOUCHON" ,             
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UN BOUCHON",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
	   if ( @vl_longueur_reelle > 0 )
		select @vl_etendue_evt="CE BOUCHON" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Bloque
  begin
    select @vl_groupe_nominal = "UNE CIRCULATION BLOQUEE"  ,
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UNE CIRCULATION BLOQUEE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
	   if ( @vl_longueur_reelle > 0 )
		select @vl_etendue_evt="CE BOUCHON" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end

end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un bouchon (queue)                                                        */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_QueueBouchon 
begin

  /* recherche de la localisation de la tete de bouchon */
  select @vl_PR1 = PR, @vl_autoroute1 = autoroute,
         @vl_pt_car1=point_caracteristique, @vl_numero_pt_car1=numero_point_caracteristique
  from EXP..FMC_HIS_DER where numero=@vl_cause and cle=@vl_cle_cause

  /* recherche du type de bouchon */
  select @vl_type_bouchon = type from EXP..FMC_BOU where numero=@va_numero_fmc and cle=@va_cle_fmc

  /* on ne mentionne pas les voies bloquees pour 1 bouchon */
  select @vl_msg_voies_bloquees = ""

  select @vl_objet_fax = "BOUCHON"

  /* determination de l'etendue du bouchon */
  exec PRC..XZAO;22 @vl_autoroute,@vl_PR,@vl_autoroute1,@vl_PR1,@vl_longueur_reelle output

  exec PRC..XZAE157 @va_numero_fmc, @va_cle_fmc, XZAEC_FMC_QueueBouchon,  @vl_com_fmc_init, 
		@vl_longueur_reelle2 output
  select @vl_longueur_reelle = @vl_longueur_reelle + @vl_longueur_reelle2

  select @vl_longueur_bouchon = ""
  select @vl_etendue_evt = ""
  if (@vl_longueur_reelle > 0)
  begin
    /* calcul de la longueur */
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
      select @vl_nom_pt_car1 = nom,
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
    select @vl_groupe_nominal = "UNE CIRCULATION DIFFICILE" ,
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UNE CIRCULATION DIFFICILE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
    if ( @vl_longueur_reelle > 0 )
	select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  if @vl_type_bouchon = XZAEC_BOU_Accordeon
  begin
    select @vl_groupe_nominal = "UNE CIRCULATION EN ACCORDEON" ,
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UNE CIRCULATION EN ACCORDEON",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
    if ( @vl_longueur_reelle > 0 )
	select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Ralenti
  begin
    select @vl_groupe_nominal = "UN TRAFIC RALENTI" ,             
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UN TRAFIC RALENTI",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
    if ( @vl_longueur_reelle > 0 )
	select @vl_etendue_evt="CETTE DIFFICULTE" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Bouche
  begin
    select @vl_groupe_nominal = "UN BOUCHON" ,             
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UN BOUCHON",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
    if ( @vl_longueur_reelle > 0 )
	select @vl_etendue_evt="CE BOUCHON" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end
  else if @vl_type_bouchon = XZAEC_BOU_Bloque
  begin
    select @vl_groupe_nominal = "UNE CIRCULATION BLOQUEE"  ,
           @vl_groupe_verbal  = "VOUS ALLEZ RENCONTRER UNE CIRCULATION BLOQUEE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BOUCHON"
    if ( @vl_longueur_reelle > 0 )
	select @vl_etendue_evt="CE BOUCHON" + rtrim(@vl_etendue_evt) + " S'ETEND SUR " + @vl_longueur_bouchon
  end

end
/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement meteo                                                        */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Meteo
begin

  /* recherche du type de perturbation meteo et de la visibilite */
  select @vl_PR1=PR, @vl_type_perturbation = type_de_perturbation,@vl_distance_visibilite=visibilite from EXP..FMC_MET 
					  where numero=@va_numero_fmc and cle=@va_cle_fmc
  select @vl_autoroute1=@vl_autoroute
  if @vl_distance_visibilite>= 0 select @vl_visibilite= "LA VISIBILITE EST INFERIEURE A "+convert(char(3), @vl_distance_visibilite)+" METRES"

  if ( @vl_type_perturbation = XZAEC_MET_ChuteNeige ) 
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DES CHUTES DE NEIGE",
           @vl_groupe_verbal  = "ON NOUS SIGNALE DES CHUTES DE NEIGE",        
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE CHUTES DE NEIGE",
	   @vl_objet_fax = "CHUTES DE NEIGE"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_NeigeChaus ) 
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DE LA NEIGE SUR LA CHAUSSEE",
           @vl_groupe_verbal  = "ON NOUS SIGNALE QUE LA CHAUSSEE EST ENNEIGEE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE NEIGE SUR LA CHAUSSEE",
	   @vl_objet_fax = "NEIGE SUR CHAUSSEE"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_Pluie ) 
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DES RAFALES DE PLUIE",
           @vl_groupe_verbal  = "ON NOUS SIGNALE DES RAFALES DE PLUIE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE RAFALES DE PLUIE",
	   @vl_objet_fax = "RAFALES DE PLUIE"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_VentFort ) 
  begin
    select @vl_chaussee_glissante=" "
    select @vl_groupe_nominal = "UN VENT FORT",
           @vl_groupe_verbal  = "ON NOUS SIGNALE UN VENT FORT",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE RAFALES DE VENT",
	   @vl_objet_fax = "RAFALES DE VENT"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_Brouillard ) 
  begin
    select @vl_chaussee_glissante=" "
    select @vl_groupe_nominal = "DU BROUILLARD",
           @vl_groupe_verbal  = "ON NOUS SIGNALE DU BROUILLARD",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BROUILLARD",
	   @vl_objet_fax = "BROUILLARD"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_BrouillardGiv ) 
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DU BROUILLARD GIVRANT",
           @vl_groupe_verbal  = "ON NOUS SIGNALE DU BROUILLARD GIVRANT",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE BROUILLARD GIVRANT",
	   @vl_objet_fax = "BROUILLARD GIVRANT"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_Grele ) 
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DES AVERSES DE GRELE",
           @vl_groupe_verbal  = "ON NOUS SIGNALE DES AVERSES DE GRELE",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS D'AVERSES DE GRELE",
	   @vl_objet_fax = "GRELE"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_Verglas ) 
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DU VERGLAS",
           @vl_groupe_verbal  = "ON NOUS SIGNALE DU VERGLAS",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE VERGLAS",
	   @vl_objet_fax = "VERGLAS"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_GreleChaus)
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DE LA GRELE SUR LA CHAUSSEE",
	   @vl_groupe_verbal  = "ON NOUS SIGNALE DE LA GRELE SUR LA CHAUSSEE",
	   @vl_fin_evenement  = "QU'IL N'Y A PLUS DE GRELE SUR LA CHAUSSEE",
	   @vl_objet_fax = "GRELE SUR LA CHAUSSEE"
  end
  else if ( @vl_type_perturbation = XZAEC_MET_ChausMouill)
  begin
    select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
    select @vl_groupe_nominal = "DE LA PLUIE SUR LA CHAUSSEE",
	   @vl_groupe_verbal  = "ON NOUS SIGNALE QUE LA CHAUSSEE EST MOUILLEE",
	   @vl_fin_evenement  = "QU'IL N'Y A PLUS DE PLUIE SUR LA CHAUSSEE",
	   @vl_objet_fax = "CHAUSSEE MOUILLEE"
  end
  else 
  begin
    select @vl_groupe_nominal = "UNE PERTURBATION METEO",
           @vl_groupe_verbal  = "ON NOUS SIGNALE UNE PERTURBATION METEO",
           @vl_fin_evenement  = "QU'IL N'Y A PLUS DE PERTURBATION METEO",
	   @vl_objet_fax = "PERTURBATION METEO"
  end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement Obstacle                                                     */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Obstacle
begin

  	/* recherche de la nature de l'obstacle */
  	select @vl_nature_evenement = nature from EXP..FMC_NAT where numero=@va_numero_fmc and cle=@va_cle_fmc
        if ( @vl_nature_evenement <> NULL )
	begin
		select @vl_lib_cause=@vl_nature_evenement
		select @vl_groupe_nominal = "UN OBSTACLE ("+ @vl_nature_evenement
		select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + ")"

		select @vl_groupe_verbal  = "ON NOUS SIGNALE UN OBSTACLE (" +@vl_nature_evenement
		select @vl_groupe_verbal  = rtrim(@vl_groupe_verbal) + ")"

		select @vl_fin_evenement  = "QU'IL N'Y A PLUS D'OBSTACLE "

		select @vl_objet_fax = "OBSTACLE (" +@vl_nature_evenement
		select @vl_objet_fax = rtrim(@vl_objet_fax) + ")"
	end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement Pieton                                                       */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_PietonChaussee
begin

  	/* recherche de la nature */
  	select @vl_nature_evenement = nature from EXP..FMC_NAT where numero=@va_numero_fmc and cle=@va_cle_fmc
        if ( @vl_nature_evenement <> NULL )
	begin
		select @vl_lib_cause=@vl_nature_evenement
		select @vl_groupe_nominal = "UN PIETON ("+ @vl_nature_evenement
		select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + ") SUR LA CHAUSSEE"

		select @vl_groupe_verbal  = "ON NOUS SIGNALE LA PRESENCE DE PIETON(S) (" +@vl_nature_evenement
		select @vl_groupe_verbal  = rtrim(@vl_groupe_verbal) + ") SUR LA CHAUSSEE"

		select @vl_fin_evenement  = "QU'IL N'Y A PLUS DE PIETONS (" + @vl_nature_evenement
		select @vl_fin_evenement  = rtrim(@vl_fin_evenement) + ") SUR LA CHAUSSEE"

		select @vl_objet_fax = "PIETON(S) (" +@vl_nature_evenement
		select @vl_objet_fax = rtrim(@vl_objet_fax) + ") SUR LA CHAUSSEE"
	end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement Autre                                                        */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Autre
begin

  	/* recherche de la nature de l'evenement */
  	select @vl_nature_evenement = nature from EXP..FMC_NAT where numero=@va_numero_fmc and cle=@va_cle_fmc
        if ( @vl_nature_evenement <> NULL )
	begin
		select @vl_lib_cause=@vl_nature_evenement
		select @vl_groupe_nominal = "UN AUTRE EVENEMENT ("+ @vl_nature_evenement
		select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + ")"

		select @vl_groupe_verbal  = "ON NOUS SIGNALE UN AUTRE EVENEMENT (" +@vl_nature_evenement
		select @vl_groupe_verbal  = rtrim(@vl_groupe_verbal) + ")"

		select @vl_fin_evenement  = "LA FIN DE L'EVENEMENT (" + @vl_nature_evenement
		select @vl_fin_evenement  = rtrim(@vl_fin_evenement) + ")"

		select @vl_objet_fax = "AUTRE EVENEMENT (" +@vl_nature_evenement
		select @vl_objet_fax = rtrim(@vl_objet_fax) + ")"
	end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un Traitement des Chaussees                                               */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_TraitChaussees
begin

	/* recherche du type d'operation et du PR de tete */
	select @vl_autoroute1 = @vl_autoroute
	select @vl_PR1 = PR, @vl_type_perturbation = type_de_perturbation
	from EXP..FMC_MET
	where numero = @va_numero_fmc and cle = @va_cle_fmc

	if ( @vl_type_perturbation = XZAEC_TRT_CH_Salage )
	begin
		select @vl_lib_cause="SALAGE"
		select @vl_groupe_nominal = "DES OPERATIONS DE SALAGE"
		select @vl_groupe_verbal = "DES OPERATIONS DE SALAGE SONT EN COURS"
		select @vl_fin_evenement = "QUE LES OPERATIONS DE SALAGE SONT TERMINEES"
		select @vl_objet_fax = "SALAGE"
	end
	else if ( @vl_type_perturbation = XZAEC_TRT_CH_Deneigement )
	begin
		select @vl_lib_cause="DENEIGEMENT"
		select @vl_groupe_nominal = "DES OPERATIONS DE DENEIGEMENT"
		select @vl_groupe_verbal = "DES OPERATIONS DE DENEIGEMENT SONT EN COURS"
		select @vl_fin_evenement = "QUE LES OPERATIONS DE DENEIGEMENT SONT TERMINEES"
		select @vl_objet_fax = "DENEIGEMENT"
	end
end
/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement Chaussee Glissante                                           */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_ChausseeGliss
begin

	/* recherche du type d'evenement et du PR de tete */
	select @vl_autoroute1 = @vl_autoroute
	select @vl_PR1 = PR, @vl_type_perturbation = type_de_perturbation
	from EXP..FMC_MET
	where numero = @va_numero_fmc and cle = @va_cle_fmc

	select @vl_chaussee_glissante = "LA CHAUSSEE EST GLISSANTE"

	if ( @vl_type_perturbation = XZAEC_CH_GLIS_Neige )
	begin
		select @vl_groupe_nominal = "DE LA NEIGE SUR LA CHAUSSEE"
		select @vl_groupe_verbal = "ON NOUS SIGNALE QUE LA CHAUSSEE EST ENNEIGEE"
		select @vl_fin_evenement = "QU'IL N'Y A PLUS DE NEIGE SUR LA CHAUSSEE"
		select @vl_objet_fax = "NEIGE SUR CHAUSSEE"
	end
	else if ( @vl_type_perturbation = XZAEC_CH_GLIS_Verglas )
	begin
		select @vl_groupe_nominal = "DU VERGLAS"
		select @vl_groupe_verbal = "ON NOUS SIGNALE DU VERGLAS"
		select @vl_fin_evenement = "QU'IL N'Y A PLUS DE VERGLAS"
		select @vl_objet_fax = "VERGLAS"
	end
	else if ( @vl_type_perturbation = XZAEC_CH_GLIS_Grele )
	begin
		select @vl_groupe_nominal = "DE LA GRELE SUR LA CHAUSSEE"
		select @vl_groupe_verbal = "ON NOUS SIGNALE DE LA GRELE SUR LA CHAUSSEE"
		select @vl_fin_evenement = "QU'IL N'Y A PLUS DE GRELE SUR LA CHAUSSEE"
		select @vl_objet_fax = "GRELE SUR LA CHAUSSEE"
	end
	else if ( @vl_type_perturbation = XZAEC_CH_GLIS_Huile )
	begin
		select @vl_groupe_nominal = "DE L'HUILE SUR LA CHAUSSEE"
		select @vl_groupe_verbal = "ON NOUS SIGNALE DE L'HUILE SUR LA CHAUSSEE"
		select @vl_fin_evenement = "QU'IL N'Y A PLUS D'HUILE SUR LA CHAUSSEE"
		select @vl_objet_fax = "HUILE SUR LA CHAUSSEE"
	end
	else if ( @vl_type_perturbation = XZAEC_CH_GLIS_Carburant )
	begin
		select @vl_groupe_nominal = "DU CARBURANT SUR LA CHAUSSEE"
		select @vl_groupe_verbal = "ON NOUS SIGNALE DU CARBURANT SUR LA CHAUSSEE"
		select @vl_fin_evenement = "QU'IL N'Y A PLUS DE CARBURANT SUR LA CHAUSSEE"
		select @vl_objet_fax = "CARBURANT SUR LA CHAUSSEE"
	end
	else if ( @vl_type_perturbation = XZAEC_CH_GLIS_Gasoil )
	begin
		select @vl_groupe_nominal = "DU GASOIL SUR LA CHAUSSEE"
		select @vl_groupe_verbal = "ON NOUS SIGNALE DU GASOIL SUR LA CHAUSSEE"
		select @vl_fin_evenement = "QU'IL N'Y A PLUS DE GASOIL SUR LA CHAUSSEE"
		select @vl_objet_fax = "GASOIL SUR LA CHAUSSEE"
	end
	else /* vl_type_perturbation = XZAEC_CH_GLIS_Autre */
	begin
		select @vl_groupe_nominal = "UNE CHAUSSEE GLISSANTE"
		select @vl_groupe_verbal = "LA CHAUSSEE EST GLISSANTE"
		select @vl_fin_evenement = "QUE LA CHAUSSEE N'EST PLUS GLISSANTE"
		select @vl_objet_fax = "CHAUSSEE GLISSANTE"
	end
	select @vl_lib_cause=@vl_objet_fax
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un incident sous concessionaire                                           */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_IncSsConces
begin

  select @vl_flag_ferme=ferme,@vl_numero_carburant=carburant from EXP..FMC_CNC where numero=@va_numero_fmc and cle=@va_cle_fmc
  if @vl_flag_ferme = 1 
  begin
    select @vl_groupe_nominal = "LA FERMETURE DE LA STATION SERVICE",
           @vl_groupe_verbal  = "LA STATION SERVICE EST FERMEE",
	   @vl_objet_fax = "STATION SERVICE FERMEE"
    select @vl_fin_evenement  = "LA REOUVERTURE DE LA STATION SERVICE "
  end
  else 
  begin
    select @vl_objet_fax = "PENURIE DE CARBURANT"
    if @vl_numero_carburant=XDC_CRB_SUPER
    begin
      select @vl_groupe_nominal = "UNE PENURIE DE SUPER A LA STATION SERVICE",
             @vl_groupe_verbal  = "IL N'Y A PLUS DE SUPER"
      select @vl_fin_evenement  = "LE REAPPROVISIONNEMENT EN SUPER DE LA STATION SERVICE "
    end
    else if @vl_numero_carburant=XDC_CRB_GAZOLE
    begin
      select @vl_groupe_nominal = "UNE PENURIE DE GAZOLE A LA STATION SERVICE",
             @vl_groupe_verbal  = "IL N'Y A PLUS DE GAZOLE"
      select @vl_fin_evenement  = "LE REAPPROVISIONNEMENT EN GAZOLE DE LA STATION SERVICE "
    end
    else if @vl_numero_carburant=XDC_CRB_SSPB95
    begin
      select @vl_groupe_nominal = "UNE PENURIE DE SANS PLOMB 95 A LA STATION SERVICE",
             @vl_groupe_verbal  = "IL N'Y A PLUS DE SANS PLOMB 95"
      select @vl_fin_evenement  = "LE REAPPROVISIONNEMENT EN SANS PLOMB 95 DE LA STATION SERVICE "
    end
    else if @vl_numero_carburant=XDC_CRB_SSPB98
    begin
      select @vl_groupe_nominal = "UNE PENURIE DE SANS PLOMB 98 A LA STATION SERVICE",
             @vl_groupe_verbal  = "IL N'Y A PLUS DE SANS PLOMB 98"
      select @vl_fin_evenement  = "LE REAPPROVISIONNEMENT EN SANS PLOMB 98 DE LA STATION SERVICE "
    end
    else if @vl_numero_carburant=XDC_CRB_GPL
    begin
      select @vl_groupe_nominal = "UNE PENURIE DE GPL A LA STATION SERVICE",
             @vl_groupe_verbal  = "IL N'Y A PLUS DE GPL"
      select @vl_fin_evenement  = "LE REAPPROVISIONNEMENT EN GPL DE LA STATION SERVICE "
    end
    else 
    begin
      select @vl_groupe_nominal = "UNE PENURIE DE CARBURANT A LA STATION SERVICE",
             @vl_groupe_verbal  = "IL N'Y A PLUS DE CARBURANT"
      select @vl_fin_evenement  = "LE REAPPROVISIONNEMENT EN CARBURANT DE LA STATION SERVICE "
    end
  end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un incendie de vegetation                                                 */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Incendie
begin
  select @vl_code_nature = code_nature,
	 @vl_feu_maitrise = feu_maitrise
	 from EXP..FMC_INC
  where numero=@va_numero_fmc and cle=@va_cle_fmc

  select @vl_nature_incendie = "Non pr�cis�"
  select @vl_nature_incendie = libelle  from CFG..LIB_PRE
  where notyp = XDC_LIB_TYPE_NATURE_INCENDIE  and  code = @vl_code_nature

  select @vl_groupe_nominal = "UN INCENDIE (" + @vl_nature_incendie + ")",
	 @vl_groupe_verbal = "UN INCENDIE (" + @vl_nature_incendie + ") S'EST PRODUIT"

  if @vl_feu_maitrise = XDC_VRAI
  begin
	select	@vl_groupe_nominal = rtrim (@vl_groupe_nominal) + " (INCENDIE MAITRISE)",
		@vl_groupe_verbal = rtrim (@vl_groupe_verbal) + " (INCENDIE MAITRISE)",
		@vl_objet_fax = rtrim (@vl_objet_fax) + " (INCENDIE MAITRISE)"
  end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un evenement flaque d'huile                                               */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_FlaqueHuile
begin

  select @vl_chaussee_glissante="LA CHAUSSEE EST GLISSANTE"
  
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'une manifestation							        */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Manifestation
begin

  select @vl_nb_manifestants = nombre_de_manifestants ,
  	@vl_violence = violence,
  	@vl_perception_peage = perception_peage
  from EXP..FMC_MNF
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
  group by numero,cle
  having validation = max(validation)
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'une aire fermee                                                           */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_AireFermee
begin

  select @vl_groupe_nominal = "LA FERMETURE DE L'AIRE "+upper(@vl_nom_pt_car),
         @vl_groupe_verbal  = "L'AIRE "+@vl_nom_pt_car+" EST FERMEE",
         @vl_fin_evenement  = "LA REOUVERTURE DE L'AIRE DE "+ upper(@vl_nom_pt_car),
	 @vl_objet_fax = "AIRE FERMEE"
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un basculement                                                            */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Basculement
begin

  select @vl_conseil = " SOYEZ PRUDENTS EN ARRIVANT SUR LES LIEUX OU LES DISPOSITIFS DE SECURITE GUIDERONT VOTRE PASSAGE."

  select @vl_msg_neutralisation=""
  /*recuperation PR tete*/
  select @vl_PR1 = localisation_PR_fin,
	@vl_vitesse = vitesse,
	@vl_vitesse_opposee = vitesse_opposee
  from EXP..FMC_BAS
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
  select @vl_autoroute1=@vl_autoroute
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'une fermeture echangeur                                                   */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_FermetureEch
begin

  /*JMG 28/02/03*/
  /*DEM 71*/
  select 
         @vl_groupe_verbal  = "ON NOUS SIGNALE LA FERMETURE",
         @vl_groupe_nominal  = "LA FERMETURE",
	 @vl_objet_fax = "FERMETURE ECHANGEUR",
	 @vl_fin_evenement =" LA REOUVERTURE DE L'ECHANGEUR " +  
				rtrim (@vl_sortie_usager) + " " + upper(@vl_nom_pt_car)
  select
	 @vl_entree1 = entree1,
	 @vl_entree2 = entree2,
	 @vl_sortie1 = sortie1,
	 @vl_sortie2 = sortie2
  from EXP..FMC_ECH
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc

  select  @vl_nombre = convert(int,@vl_entree1) + convert(int,@vl_entree2) + convert(int,@vl_sortie1) + convert(int,@vl_sortie2)
  if @vl_nombre = 1
  begin
    select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE LA BRETELLE"
    select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " DE LA BRETELLE"
    if (@vl_entree1=1 or @vl_entree2=1) 
    begin
	select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE"
	select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " D'ENTREE"
    end
    else 
    begin
	select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE SORTIE"
	select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " DE SORTIE"
    end
  end
  else
  begin
    select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DES BRETELLES"
    select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " DES BRETELLES"
    if (@vl_entree1=1 or @vl_entree2=1) 
    begin 
      if (@vl_sortie1=1 or @vl_sortie2=1) 
      begin
	select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE ET DE SORTIE"
	select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " D'ENTREE ET DE SORTIE"
      end
      else  
      begin
	select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " D'ENTREE"
	select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " D'ENTREE"
      end
    end
    else
    begin
      select @vl_groupe_verbal =  rtrim(@vl_groupe_verbal) + " DE SORTIE"
      select @vl_groupe_nominal =  rtrim(@vl_groupe_nominal) + " DE SORTIE"
    end
  end

  /* Traitement de bifurcations modelisees en echangeur */  
  if ( @vl_indic_bifurc = XDC_VRAI ) 
  begin
  select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE LA BIFURCATION " +
			     rtrim(upper(@vl_nom_pt_car))+"."
  select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " DE LA BIFURCATION " +
			     rtrim(upper(@vl_nom_pt_car))+"."
	select @vl_fin_evenement =" LA REOUVERTURE DE BIFURCATION " +  
				rtrim (upper(@vl_nom_pt_car))
  end
  else
  begin
  select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE L'ECHANGEUR " + rtrim (@vl_sortie_usager) + " "+
			     rtrim(upper(@vl_nom_pt_car))+"."
  select @vl_groupe_nominal = rtrim(@vl_groupe_nominal) + " DE L'ECHANGEUR " + rtrim (@vl_sortie_usager) + " "+
			     rtrim(upper(@vl_nom_pt_car))+"."
	 select @vl_fin_evenement =" LA REOUVERTURE DE L'ECHANGEUR " +  
				rtrim (@vl_sortie_usager) + " " + upper(@vl_nom_pt_car)
  end
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un Echangeur deconseille                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_EchDeconseille
begin

  select 
         @vl_groupe_verbal  = "IL EST DECONSEILLE D'EMPRUNTER",
	 @vl_fin_evenement = "LE RETOUR A LA NORMALE SUR L'ECHANGEUR " +  
				rtrim (@vl_sortie_usager) + " " + upper(@vl_nom_pt_car)
  select
	 @vl_entree1 = entree1,
	 @vl_entree2 = entree2,
	 @vl_sortie1 = sortie1,
	 @vl_sortie2 = sortie2
  from EXP..FMC_ECH
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc

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
  select @vl_groupe_verbal = rtrim(@vl_groupe_verbal) + " DE L'ECHANGEUR " + rtrim (@vl_sortie_usager) + " "+
			     rtrim(upper(@vl_nom_pt_car))+"."
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas de travaux                                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_Travaux
begin

  /*recherche du PR Tete des travaux*/
  select @vl_PR1=localisation_tete,
         @vl_voies_retrecies=voies_retrecies,
	 @vl_vitesse = vitesse,
	 @vl_personnel = personnel
  from EXP..FMC_TRH
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
  group by numero,cle
  having horodate = max(horodate)

  select @vl_autoroute1=@vl_autoroute

  /*type de travaux*/
  select @vl_type_travaux = type,
	@vl_nature_travaux = nature
  from EXP..FMC_TRF
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'une zone de stockage                                                      */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_ZoneStockage
begin

  /*recherche du PR Tete de stockage*/
  select @vl_PR1=localisation_tete,
         @vl_voies_retrecies=voies_retrecies
  from EXP..FMC_TRH
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
  group by numero,cle
  having horodate = max(horodate)

  select @vl_autoroute1=@vl_autoroute

  /* si le stockage est demarre alors retourner les donnees de stockage avec le conseil */
  if @vl_voies_retrecies = XDC_VRAI
    select @vl_conseil = " EN ARRIVANT SUR CETTE ZONE, LES POIDS LOURDS SERONT INVITES PAR LES FORCES DE L'ORDRE A ENTRER " +
                         "DANS CETTE ZONE DE STOCKAGE. IL VOUS EST DEMANDE DE RALENTIR EN ARRIVANT SUR CETTE ZONE."
  else
    /* si le stockage n'est pas demarre alors surcharger les donnees avec celles de travaux */
    select @vl_groupe_nominal = groupe_nominal,
	    @vl_groupe_verbal = groupe_verbal,
	    @vl_fin_evenement = fin_evenement,
	    @vl_objet_fax = objet_fax
    from CFG..SAD_CFG
    where type = XZAEC_FMC_Travaux
end

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      cas d'un basculement pour travaux                                               */
/************************************************************************************************************************/
/************************************************************************************************************************/
else if @vl_type = XZAEC_FMC_BasculTravaux
begin

  select @vl_conseil = " SOYEZ PRUDENTS EN ARRIVANT SUR LES LIEUX OU LES DISPOSITIFS DE SECURITE GUIDERONT VOTRE PASSAGE."

  select @vl_msg_neutralisation=""

  /*recherche du PR de tete*/
  select @vl_PR1=localisation_tete,
         @vl_voies_retrecies=voies_retrecies,
	 @vl_vitesse=vitesse,
	 @vl_personnel = personnel,
	 @vl_bouchon = bouchon
  from EXP..FMC_TRH
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
  group by numero,cle
  having horodate = max(horodate)

  select @vl_autoroute1=@vl_autoroute

  /*type de travaux*/
  select @vl_type_travaux = type,
	@vl_nature_travaux = nature
  from EXP..FMC_TRF
  where numero = @va_numero_fmc
  and cle = @va_cle_fmc
end





/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                       recherche du texte cause s'il y en a une                                       */
/************************************************************************************************************************/
/************************************************************************************************************************/
recherche_cause:
select @vl_cause_toujours=" "
select @vl_type_cause=0

if ( @vl_cause is not null and @vl_fin is null  )
begin
  select @vl_type_cause = type from EXP..FMC_GEN where numero=@vl_cause and cle=@vl_cle_cause

  if @vl_type_cause = XZAEC_FMC_TeteBouchon
  begin
    select @vl_cause = cause, @vl_cle_cause = cle_cause from EXP..FMC_GEN where numero=@vl_cause and cle=@vl_cle_cause
    goto recherche_cause
  end
  
  else if @vl_type_cause = XZAEC_FMC_Accident select @vl_cause_toujours="SUITE A CET ACCIDENT, "

  else if @vl_type_cause  = XZAEC_FMC_Travaux select @vl_cause_toujours="SUITE A CES TRAVAUX, "

  else if @vl_type_cause  = XZAEC_FMC_ZoneStockage select @vl_cause_toujours="SUITE A CETTE OPERATION, "

  else if @vl_type_cause in (XZAEC_FMC_QueueBouchon,XZAEC_FMC_TeteBouchon) 
				   select @vl_cause_toujours="SUITE A CES DIFFICULTES DE CIRCULATION, "

  else if @vl_type_cause  = XZAEC_FMC_Meteo select @vl_cause_toujours="SUITE A CETTE PERTURBATION METEOROLOGIQUE, "

  else if @vl_type_cause  = XZAEC_FMC_AireFermee select @vl_cause_toujours="SUITE A CETTE FERMETURE, "
  
  else if @vl_type_cause in (XZAEC_FMC_Basculement, XZAEC_FMC_BasculTravaux)
                                   select @vl_cause_toujours="SUITE A CE BASCULEMENT DE CIRCULATION, "

  else if @vl_type_cause  = XZAEC_FMC_Delestage select @vl_cause_toujours="SUITE A CE DELESTAGE, "

  else if @vl_type_cause  = XZAEC_FMC_Deviation select @vl_cause_toujours="SUITE A CETTE DEVIATION, "

  else if @vl_type_cause  = XZAEC_FMC_FermetureEch select @vl_cause_toujours="SUITE A CETTE FERMETURE, "

  else if @vl_type_cause  = XZAEC_FMC_TraitChaussees select @vl_cause_toujours="SUITE A CES OPERATIONS, "

  else if @vl_type_cause in (
			XZAEC_FMC_Contresens,XZAEC_FMC_VehArrete,XZAEC_FMC_VehPanne,XZAEC_FMC_VehFeu,XZAEC_FMC_Cirque,
			XZAEC_FMC_ConvoiExc,XZAEC_FMC_ConvoiMil,XZAEC_FMC_VehLent,XZAEC_FMC_Manifestation,XZAEC_FMC_GreveESCOTA,
			XZAEC_FMC_Escargot,XZAEC_FMC_IncSsConces,
			XZAEC_FMC_Eboulement,XZAEC_FMC_Incendie,XZAEC_FMC_Inondation,XZAEC_FMC_Obstacle,
			XZAEC_FMC_ChausseDeter,XZAEC_FMC_FlaqueHuile,XZAEC_FMC_AnimalErrant,XZAEC_FMC_FrontiereFermee,
			XZAEC_FMC_AltercationUsager,XZAEC_FMC_Agression,XZAEC_FMC_MalaiseUsager,XZAEC_FMC_EvtHorsReseau,
			XZAEC_FMC_Salage,XZAEC_FMC_HoldUpVol,XZAEC_FMC_PietonChaussee,
			XZAEC_FMC_Autre,XZAEC_FMC_VehMarcheArr,XZAEC_FMC_EchDeconseille,XZAEC_FMC_ChausseeGliss)
 
			select @vl_cause_toujours= "SUITE A CET INCIDENT, "

   select @vl_cause_operation=operation from CFG..TYP_FMC where numero=@vl_type_cause
end
else
begin
   select @vl_cause_operation=operation from CFG..TYP_FMC where numero=@vl_type
end






/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      calcul de la longueur de l'evenement                                            */
/************************************************************************************************************************/
/************************************************************************************************************************/
/* reajustement des variables si besoin */
if @vl_PR1 is null
  select @vl_PR1=@vl_PR, @vl_autoroute1=@vl_autoroute

if @vl_longueur_reelle is null
   exec PRC..XZAO;22 @vl_autoroute,@vl_PR,@vl_autoroute1,@vl_PR1,@vl_longueur_reelle output                       





/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      creation du message tdp si besoin                                               */
/************************************************************************************************************************/
/************************************************************************************************************************/

select @vl_msg_tdp=""
if @vl_type in (XZAEC_FMC_QueueBouchon,XZAEC_FMC_TeteBouchon)
begin
/* Recherche des PR des destination extremes a ne pas depasser  DEM579*/
	select @vl_PR_Max_dtp=max(PR) from CFG..RES_DTP where inhibition=0 and sens=@vl_sens_chaussee and autoroute=@vl_autoroute
	select @vl_PR_Min_dtp=min(PR) from CFG..RES_DTP where inhibition=0 and sens=@vl_sens_chaussee and autoroute=@vl_autoroute

  /* recherche de toutes les zdp memes autoroutes et sens congestionnees triees par PR */
  declare zdp_en_congestion cursor
  for select distinct G.numero, G.PR, Z.PR_fin, M.temps , Z.TDP_habituel , M.horodate
      from EXP..FRN_TDP F, CFG..EQT_GEN G, CFG..EQT_ZDP Z , EXP..ZDP_6MN M
      where F.zdp=Z.numero and Z.numero=G.numero and M.numero=Z.numero
        and G.type=39
        and G.autoroute=@vl_autoroute                
        and G.sens=@vl_sens_chaussee           
        and F.fin is null and F.debut is not null
	and F.debut > dateadd(day,-1,getdate())
	and G.PR > @vl_PR_Min_dtp and Z.PR_fin< @vl_PR_Max_dtp
      group by Z.numero having 
	F.zdp=Z.numero and Z.numero=G.numero and M.numero=Z.numero
	and G.type=39
	and G.autoroute=@vl_autoroute
	and G.sens=@vl_sens_chaussee
	and F.fin is null and F.debut is not null
	and F.debut > dateadd(day,-1,getdate())
	and G.PR > @vl_PR_Min_dtp and Z.PR_fin< @vl_PR_Max_dtp
	and M.horodate= max(M.horodate)
      order by G.PR asc , Z.PR_fin desc

  open zdp_en_congestion
  fetch zdp_en_congestion into @vl_numero_zdp, @vl_PR_amont_zdp, @vl_PR_aval_zdp, @vl_temps_mesure,
			       @vl_temps_habituel , @vl_horodate

  /* recherche par le curseur des PR de debut et de fin de zone congestionnee */
  select @vl_PR_aval_zdp_prec = 0 , @vl_PR_debut_congestion=0 , @vl_PR_fin_congestion=0, @vl_temps_cumule=0 , @vl_numero_zdp_prec=0
  while (@@sqlstatus = 0 )
  begin


    /* verrue : traitement du cas particulier de la fourche */
    if @vl_numero_zdp_prec = 6 and @vl_numero_zdp = 5 goto AU_SUIVANT 
    else select @vl_numero_zdp_prec = @vl_numero_zdp
                              
    /* si la zdp etudiee est jointe a la precedente, on met a jour le PR fin de congestion */
    if ( @vl_PR_amont_zdp = @vl_PR_aval_zdp_prec )
    begin
      select @vl_PR_fin_congestion = @vl_PR_aval_zdp,
	     @vl_PR_aval_zdp_prec  = @vl_PR_aval_zdp
    end
    /* sinon on repart sur une nouvelle zone de congestion ou on considere que l'ancienne est la bonne */
    else
    begin
      if ( @vl_PR < @vl_PR1 ) 
      begin
	select @vl_PR_min = @vl_PR ,
	       @vl_PR_max = @vl_PR1
      end
      else
      begin
	select @vl_PR_min = @vl_PR1 ,
	       @vl_PR_max = @vl_PR
      end
      if ( @vl_PR_debut_congestion < @vl_PR_min and @vl_PR_fin_congestion < @vl_PR_min )
      begin
        /* dans ce cas, l'ancienne zone de congestion etait trop en aval, on repart sur une nouvelle */
        select @vl_PR_aval_zdp_prec=@vl_PR_aval_zdp, 
	       @vl_PR_fin_congestion=@vl_PR_aval_zdp, 
	       @vl_PR_debut_congestion=@vl_PR_amont_zdp
      end
      else if ( @vl_PR_debut_congestion > @vl_PR_max and @vl_PR_fin_congestion > @vl_PR_max )
      begin
        /* dans ce cas, l'ancienne zone de congestion etait trop en amont,  on repart sur une nouvelle */
        select @vl_PR_aval_zdp_prec=0, @vl_PR_fin_congestion=0, @vl_PR_debut_congestion=0,
               @vl_temps_cumule = 0
      end
      /* sinon, l'ancienne zone de congestion etait ok, on la garde */
    end
AU_SUIVANT:
    fetch zdp_en_congestion into @vl_numero_zdp, @vl_PR_amont_zdp, @vl_PR_aval_zdp, @vl_temps_mesure,
			       @vl_temps_habituel , @vl_horodate
  end

  /* INFO a ce stade, on a les PR de debut et de fin de congestion ainsi que le temps de traversee de la congestion */
/*  select @vl_PR_debut_congestion , @vl_PR_fin_congestion , @vl_temps_cumule*/

  if ( @vl_PR_debut_congestion != 0 )
  begin
    /* on recherche maintenant la	destination amont */
    select @vl_PR_dest_amont = PR  , 
           @vl_numero_dest_amont = numero , 
  	   @vl_TFM_amont = libelle_TFM , 
  	   @vl_PMV_amont = libelle_PMV
    from CFG..RES_DTP 
    where autoroute = @vl_autoroute 
      and sens = @vl_sens_chaussee 
      and PR <= @vl_PR_debut_congestion
      and inhibition =0
    group by autoroute
    having autoroute = @vl_autoroute 
       and sens = @vl_sens_chaussee 
       and PR <= @vl_PR_debut_congestion
       and inhibition =0
       and PR = max(PR)

    /* on recherche maintenant la destination aval */
    select @vl_PR_dest_aval = PR  , 
  	   @vl_numero_dest_aval = numero , 
           @vl_TFM_aval = libelle_TFM , 
           @vl_PMV_aval = libelle_PMV
    from CFG..RES_DTP 
    where autoroute = @vl_autoroute 
      and sens = @vl_sens_chaussee 
      and PR >= @vl_PR_fin_congestion
      and inhibition =0
    group by autoroute
    having autoroute = @vl_autoroute 
      and sens = @vl_sens_chaussee 
      and PR >= @vl_PR_fin_congestion
      and PR = min(PR)
      and inhibition =0

    if ( @vl_sens_chaussee = 1 ) 
		exec XZAO437 @vl_sens_chaussee,@vl_TFM_amont,@vl_TFM_aval,@vl_temps_cumule output
    else
		exec XZAO437 @vl_sens_chaussee,@vl_TFM_aval,@vl_TFM_amont,@vl_temps_cumule output

    if @vl_temps_cumule < 0
	select @vl_msg_tdp  = "IL VOUS FAUDRA ENVIRON $$ MINUTES POUR VOUS RENDRE DE $$ A $$. TDP INVALIDE"
    else
    begin
        /* on recherche le temps complementaire (pour une vitesse de 120 km /h )*/
/* DEM929 suppression:        select @vl_temps_cumule  = @vl_temps_cumule + 
	    round (((@vl_PR_dest_aval - @vl_PR_dest_amont) - (@vl_PR_fin_congestion - @vl_PR_debut_congestion)) / 33 , 0)*/

        /* calcul du message tfm */
	/* Arrondi */
	if (@vl_temps_cumule%60 >29)
		select @vl_temps_cumule=(@vl_temps_cumule /60)+1
        else
		select @vl_temps_cumule=(@vl_temps_cumule /60)

       if ( @vl_sens_chaussee = 1 ) select @vl_msg_tdp  = "IL VOUS FAUDRA ENVIRON "+ rtrim(convert (char(3),@vl_temps_cumule))+ 
			   " MINUTES POUR VOUS RENDRE DE " + rtrim(@vl_TFM_amont) + " A " + rtrim(@vl_TFM_aval )
       else select @vl_msg_tdp  = "IL VOUS FAUDRA ENVIRON "+ rtrim(convert (char(3),@vl_temps_cumule ))+ 
			   " MINUTES POUR VOUS RENDRE DE " + rtrim(@vl_TFM_aval) + " A " + rtrim(@vl_TFM_amont )
    end
  end
  /* controle final : si en sens 2 les destinations amont et aval sont superieures au PR du bouchon on raz. idem sens1 */
/*  if ((@vl_sens=2 and @vl_PR>@vl_PR_dest_aval and @vl_PR1>@vl_PR_dest_aval) 
   or (@vl_sens=1 and @vl_PR<@vl_PR_dest_amont and @vl_PR1<@vl_PR_dest_amont) )
   select @vl_msg_tdp  = ""*/

  if ((@vl_PR<@vl_PR_dest_amont or @vl_PR>@vl_PR_dest_aval) and (@vl_PR1<@vl_PR_dest_amont or @vl_PR1>@vl_PR_dest_aval) )   OR (@vl_PR_dest_amont=@vl_PR_dest_aval) OR ((@vl_PR_dest_aval is null) OR (@vl_PR_dest_amont is null))
   select @vl_msg_tdp  = ""
end





/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      creation du message tfm                                                         */
/************************************************************************************************************************/
/************************************************************************************************************************/

if @vl_fmc_prevue = 1
begin
  select @vl_msg_tfm = "FMC PREVUE."
         +  @vl_cause_toujours+", "+"SUR L AUTOROUTE "+@vl_lib_autoroute+", "+ @vl_lib_sens + ", "+
		 " ON NOUS ANNONCE "+@vl_groupe_nominal+", "+
			    @vl_localisation+", "+@vl_conseil+". "+@vl_msg_tdp

end
else if @vl_fmc_confirmee = 0
begin
  select @vl_msg_tfm = "FMC SIGNALEE."
     +   @vl_cause_toujours+", "+", "+"SUR L AUTOROUTE "+@vl_lib_autoroute+", "+ @vl_lib_sens + ", "+
		    "ON NOUS SIGNALE "+@vl_groupe_nominal+", "+
                               @vl_localisation+", "+@vl_conseil+". "+@vl_msg_tdp
end
else if ( @vl_fmc_confirmee = 1 and @vl_fmc_terminee = 0 )
begin
  select @vl_msg_tfm = "FMC CONFIRMEE EN COURS."
     +   @vl_cause_toujours+", "+", "+"SUR L AUTOROUTE "+@vl_lib_autoroute+", "+ @vl_lib_sens + ", "+@vl_groupe_verbal+", "+
	@vl_localisation+", "+@vl_conseil+". "+@vl_msg_tdp
end
else if ( @vl_fmc_terminee = 1 )
begin
  select @vl_msg_tfm = "FMC TERMINEE."
     +   @vl_cause_toujours+", "+"SUR L AUTOROUTE "+@vl_lib_autoroute+", "+ @vl_lib_sens + ", "+
			    "ON NOUS ANNONCE "+ @vl_fin_evenement+", "+@vl_localisation+". "+@vl_msg_tdp
end
else
begin
  select @vl_msg_tfm = "FMC CONFIRMEE ET TERMINEE DANS  UN ETAT INCONNU."+convert(char(1),@vl_fmc_prevue) +
	      convert(char(1),@vl_fmc_confirmee) +convert(char(1), @vl_fmc_terminee)+". "+@vl_msg_tdp
end


select 	@vl_config_voies_b_n_totale=@vl_config_voies_b_ou_n_i+"|"+@vl_config_voies_b_ou_n

/*valeurs a retourner*/
select  
/*        rtrim(@vl_msg_tfm),*/
	rtrim(@vl_lib_autoroute),
	rtrim(@vl_duree_formatee),
	rtrim(@vl_secours),
	rtrim(@vl_debut_formatee),
	rtrim(@vl_localisation),
	rtrim(@vl_msg_neutralisation),
	rtrim(@vl_victimes),
	rtrim(@vl_gravite),
	rtrim(@vl_vehicules),
	rtrim(@vl_chaussee_glissante),
	rtrim(@vl_conseil),
	rtrim(@vl_msg_voies_bloquees),
	rtrim(@vl_groupe_nominal),
	rtrim(@vl_groupe_verbal),
	rtrim(@vl_fin_evenement),
	rtrim(@vl_etendue_evt),
	rtrim(@vl_periode),
	rtrim(@vl_retour_normale),
	rtrim(@vl_visibilite),
	rtrim(@vl_cause_toujours),
	@vl_fmc_prevue, 
	@vl_fmc_confirmee ,
	@vl_fmc_terminee,
	@vl_autoroute,
	@vl_sens,
	@vl_PR,
	@vl_PR1,
	@vl_num_id_pt_car,
	@vl_pt_car,
	@vl_nom_pt_car,
	@vl_longueur_reelle,
	@vl_BAU,
	@vl_VL,
	@vl_VM1,
	@vl_VM2,
	@vl_VR,
	rtrim(@vl_objet_fax),
	rtrim(@vl_lib_sens),
	@vl_nb_vehicules,
	@vl_type_perturbation,
	@vl_personnalites,
	@vl_agent_escota,
	@vl_duree,
	@vl_distance_visibilite,
	@vl_type_vehicule,
	@vl_vl,
	@vl_pl,
	@vl_moto,
	@vl_nb_victimes,
	@vl_morts,
	@vl_blesses_graves,
	@vl_car,
	@vl_matieres_dangereuses,
	@vl_animaux_vivants,
	@vl_VL_I,
	@vl_VM1_I,
	@vl_VM2_I,
	@vl_VR_I,
	@vl_voies_retrecies,
	@vl_entree1,
	@vl_entree2,
	@vl_sortie1,
	@vl_sortie2,
	@vl_flag_ferme,
	@vl_numero_carburant,
	@vl_abrev_pt_car,
	@vl_type_bouchon,
	rtrim(@vl_msg_tdp),
	@vl_type_travaux,
	@vl_remorque,
	rtrim(@vl_fin_formatee),
	@vl_nb_manifestants,
	@vl_violence,
	@vl_perception_peage,
	@vl_config_voies_b_n_totale,
	@vl_datex,
	@vl_horaire_debut_prevu,
	@vl_horaire_fin_prevue,
	@vl_nuit,
	@vl_nature_travaux,
	@vl_sous_conc,
	@vl_feu_maitrise,
	@vl_cause_operation,
	rtrim(@vl_origine),
	@vl_vitesse,
	@vl_vitesse_opposee,
	@vl_zone,
	@vl_scenario,
	@vl_vitesse_transitoire,
	@vl_numero_zone,
	rtrim(@vl_sortie_usager),
	@vl_indic_bifurc,
	@vl_lib_cause,
	@vl_lib_secours,
	@vl_debut,
	@vl_debut_prevu,
	@vl_majeur,
	@vl_complement_rappel,
	@vl_type_cause,
	@vl_zone_urbaine,
	@vl_intervention,
	@vl_personnel,
	@vl_nb_passagers,
	@vl_cause,
	@vl_cle_cause,
	@vl_ecoulement,
	@vl_bouchon

return (@vl_cr)

go
