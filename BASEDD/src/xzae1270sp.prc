/*E*/
/*  Fichier : @(#)xzae1270sp.prc	1.6      Release : 1.6        Date : 03/19/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae1270sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des infos appartenant au bloc obligatoire
* d'un fichier STRADA.
------------------------------------------------------
* HISTORIQUE :
*
* P.CLUCHAGUE   12/01/2007 : creation pour METACOR DEM605 v1.1
* P.CLUCHAGUE   17/07/2007 : pour les evts qui ne sont pas bouchon, on dit si on est sur peage pour ne pas 
			     avoir une config des voies bizzare v1.3 DEM675
* P.NIEPCERON	23/08/2007 : Correction de la cond de test du pt caract de type peége v1.4
			     Evt sur péage traduit sur section courante v1.5  DEM690
* PNI	04/01/2017	: Si date debut > date fin prevue, alors date fin prevue=NULL et si evt terminé date fin prevu=date fin pour prevenir les incoherence saisie par les opérateur DEM1209
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE1270_Infos_obligatoires
*
* Arguments en entree
* - numero de la FMC
* - cle de la FMC
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

drop proc XZAE1270
go

create procedure XZAE1270
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_fin_prevue_out	datetime output,
	@va_type_loc_out	tinyint output,
	@va_nom_loc_out		char(25) output,
	@va_num_loc_out		int output,
	@va_restriction_out	char(255) output
as
declare
	@vl_numero 		int,
	@vl_cle			tinyint,
	@vl_pt_car		tinyint,
	@vl_num_pt_car		tinyint,
	@vl_lib_PR1 		char(7),
	@vl_lib_PR2		char(7),
	@vl_synoptique		char(23),
	@vl_pose_balisage	bit,
	@vl_PR_debut		int,
	@vl_PR_fin		int,
	@vl_PR			int,
	@vl_autoroute		int,
	@vl_no_type tinyint,
	@vl_BAU_etroite tinyint,@vl_BAU_I_etroite tinyint,
	@vl_BAU_I tinyint,@vl_VL_I tinyint,@vl_VM1_I tinyint,@vl_VM2_I tinyint,@vl_VR_I tinyint,
	@vl_BAU tinyint,@vl_VL tinyint,@vl_VM1 tinyint,@vl_VM2 tinyint,@vl_VR tinyint,
	@vl_lib_BAU_I char(1),@vl_lib_VL_I char(1),@vl_lib_VM1_I char(1),@vl_lib_VM2_I char(1),@vl_lib_VR_I char(1),
	@vl_lib_BAU char(1),@vl_lib_VL char(1),@vl_lib_VM1 char(1),@vl_lib_VM2 char(1),@vl_lib_VR char(1),
	@vl_sens tinyint,@vl_lib_sens char(1) ,@vl_lib_sens_oppose char(1),@vl_type_fmc tinyint,
	@vl_date_fin datetime,
	@vl_date_debut datetime

	
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV

	/* recherche des info dans la fmc generique */
	select @va_fin_prevue_out = isnull(fin,fin_prevue), @vl_date_debut =debut, @vl_type_fmc=type from EXP..FMC_GEN where numero=@va_numero_in and cle=@va_cle_in
	if @vl_date_debut>@va_fin_prevue_out
	begin
		select @va_fin_prevue_out=NULL
	end

	/* construction de la localisation */
	select @vl_pt_car=point_caracteristique, @vl_num_pt_car=numero_point_caracteristique,
	       @vl_BAU_etroite=BAU_etroite, @vl_BAU_I_etroite=BAU_I_etroite,
	       @vl_VR=VR, @vl_VM2=VM2, @vl_VM1=VM1, @vl_VL=VL, @vl_BAU=BAU,
	       @vl_VR_I=VR_I, @vl_VM2_I=VM2_I, @vl_VM1_I=VM1_I, @vl_VL_I=VL_I, @vl_BAU_I=BAU_I , @vl_sens=sens, 
	       @vl_PR =PR, @vl_autoroute=autoroute
	from EXP..FMC_HIS_DER where numero=@va_numero_in and cle=@va_cle_in

	if @vl_pt_car = XDC_POINT_CARACT_AIRE
	begin
	  select @va_type_loc_out = XDC_POINT_CARACT_AIRE
	  select @va_num_loc_out = @vl_num_pt_car
	  select @va_nom_loc_out = nom from CFG..RES_AIR where numero=@vl_num_pt_car 
	end
	else if @vl_pt_car = XDC_POINT_CARACT_ECHANGEUR
	begin
	  /* les bifurcations sont typees en base comme des echangeurs reste 99 modulo 100 (99,199,299...) */
	  if @vl_num_pt_car%100 = 99 select @va_type_loc_out = XDC_POINT_CARACT_BIFURCATION
	  else select @va_type_loc_out =XDC_POINT_CARACT_ECHANGEUR
	  select @va_nom_loc_out = nom from CFG..RES_ECH where numero=@vl_num_pt_car
	  select @va_num_loc_out = @vl_num_pt_car
	end
	else if (@vl_pt_car = XDC_POINT_CARACT_PEAGE) and (@vl_type_fmc not in (XZAEC_FMC_QueueBouchon,XZAEC_FMC_TeteBouchon ) )
	begin
	  /* DEM 675 : uniquement si l'evt n'est pas un bouchon ! */
	    select @va_type_loc_out = XDC_POINT_CARACT_PEAGE
	    select @va_num_loc_out = @vl_num_pt_car
	    select @va_nom_loc_out = "PEAGE! (DEM675)"
/* DEM 690 : si sur peag alors on le mais sur section courante */
	  select @va_type_loc_out = 0
	  select @va_nom_loc_out = nom from CFG..RES_AUT where numero=@vl_autoroute
	  select @va_num_loc_out = 0

	end
	else  
	begin
	  select @va_type_loc_out = 0
	  select @va_nom_loc_out = nom from CFG..RES_AUT where numero=@vl_autoroute
	  select @va_num_loc_out = 0
	end

	/* construction de la restriction */
        if @vl_sens in (XDC_SENS_SUD,XDC_SENS_SORTIE_SUD,XDC_SENS_ENTREE_SUD)
	begin
	  select @vl_lib_sens="1"
	  select @vl_lib_sens_oppose="2"
	end
	else
	begin
	  select @vl_lib_sens="2"
	  select @vl_lib_sens_oppose="1"
	end

	/* analyse de chaque etat de voies */
	if (@vl_BAU_I_etroite= 1 or @vl_BAU_I in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)) select @vl_lib_BAU_I = "X"
	else if @vl_BAU_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_BAU_I = "X"
	else select @vl_lib_BAU_I = "-"
	if @vl_VL_I in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)         select @vl_lib_VL_I = "X"
	else if @vl_VL_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VL_I = null
	else select @vl_lib_VL_I = @vl_lib_sens_oppose
	if @vl_VM1_I in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)        select @vl_lib_VM1_I = "X"
	else if @vl_VM1_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM1_I = null
	else select @vl_lib_VM1_I = @vl_lib_sens_oppose
	if @vl_VM2_I in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)        select @vl_lib_VM2_I = "X"
	else if @vl_VM2_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM2_I = null
	else select @vl_lib_VM2_I = @vl_lib_sens_oppose
        if @vl_VR_I in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)         select @vl_lib_VR_I = "X"
	else if @vl_VR_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VR_I =null
	else select @vl_lib_VR_I = @vl_lib_sens_oppose

        if (@vl_BAU_etroite= 1 or @vl_BAU in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)) select @vl_lib_BAU = "X"
	else if @vl_BAU in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_BAU = "X"
	else select @vl_lib_BAU = "-"
        if @vl_VL in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)   select @vl_lib_VL = "X"
	else if @vl_VL in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VL = null
	else select @vl_lib_VL = @vl_lib_sens
        if @vl_VM1 in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)          select @vl_lib_VM1 = "X"
	else if @vl_VM1 in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM1 =null
	else select @vl_lib_VM1 = @vl_lib_sens
        if @vl_VM2 in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)          select @vl_lib_VM2 = "X"
	else if @vl_VM2 in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM2 = null
	else select @vl_lib_VM2 = @vl_lib_sens
        if @vl_VR in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)         select @vl_lib_VR = "X"
	else if @vl_VR in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VR = null
	else select @vl_lib_VR = @vl_lib_sens

        /* concatenation */
	select @vl_synoptique = @vl_lib_BAU_I+@vl_lib_VL_I+@vl_lib_VM1_I+@vl_lib_VM2_I+@vl_lib_VR_I+":"+
                                @vl_lib_VR+@vl_lib_VM2+@vl_lib_VM1+@vl_lib_VL+@vl_lib_BAU
	/* on inverse la chaine selon le sens */
	if @vl_lib_sens = "2" select @vl_synoptique=reverse(@vl_synoptique)


	/* on cale la chaine a gauche */
	select @vl_synoptique=ltrim( @vl_synoptique )

        /* il faut que le double point se retrouve au milieu on cherche sa position dans la chaine */
	while charindex(":",@vl_synoptique) < 12
	begin
	  select @vl_synoptique = stuff(@vl_synoptique,1,0,"")
	end
	
        select @vl_pose_balisage=pose_balisage, @vl_PR_debut=PR_debut , @vl_PR_fin=PR_fin from EXP..ACT_GEN G, EXP..ACT_SIS S
		where G.numero=S.actio and G.sit=S.sit and G.evenement=@va_numero_in and G.sit=@va_cle_in
	if @vl_pose_balisage =1
	begin
	  select @vl_lib_PR1 = rtrim(convert(char(3),@vl_PR_debut/1000))+"."+right(rtrim(convert(char(6),@vl_PR_debut)),3)
	  select @vl_lib_PR2 = rtrim(convert(char(3),@vl_PR_fin/1000))+"."+right(rtrim(convert(char(6),@vl_PR_fin)),3)
	end
	else
	begin
	  select @vl_lib_PR1 = rtrim(convert(char(3),@vl_PR/1000))+"."+right(rtrim(convert(char(6),@vl_PR)),3)
	  select @vl_lib_PR2 = @vl_lib_PR1
	end
	select @va_restriction_out = rtrim(ltrim("{1|" + @vl_lib_PR1 + "|" +  @vl_lib_PR2 + "|"  + @vl_synoptique + "}"))

	return XDC_OK

go

