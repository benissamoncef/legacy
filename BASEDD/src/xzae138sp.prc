/*E*/
/*  Fichier : $Id: xzae138sp.prc,v 1.4 2011/12/06 15:37:18 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2011/12/06 15:37:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc meteo
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* Niepceron	04 Mar 1999 : Ajout pr tete v1.2
* Cluchague     30 Jan 2007 : parametres de sortie supplémentaires pour METACOR DEM605 v1.3
* VR		05 Oct 2011 : Ajout colonnes vitesse et vitesse_opposee v1.4 (DEM/1015)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE138_Infos_Travaux_Pour_Strada
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

drop proc XZAE138
go

create procedure XZAE138
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_longueur_out	T_PR output,
	@va_prtete_out        T_PR output,
	@va_type_loc_out        tinyint output,
	@va_nom_loc_out         char(25)  output,
	@va_num_loc_out         int output,
	@va_restriction_out_1   char(255) output,
	@va_restriction_out_2   char(255) output,
	@va_vitesse_out		tinyint = null,
	@va_vitesse_opposee_out	tinyint = null

as
declare @vl_no_type tinyint, @vl_pt_car tinyint, @vl_num_pt_car tinyint,
	@vl_BAU_etroite tinyint,@vl_BAU_I_etroite tinyint,
	@vl_BAU_I tinyint,@vl_VL_I tinyint,@vl_VM1_I tinyint,@vl_VM2_I tinyint,@vl_VR_I tinyint,
	@vl_BAU tinyint,@vl_VL tinyint,@vl_VM1 tinyint,@vl_VM2 tinyint,@vl_VR tinyint,
        @vl_lib_BAU_I char(1),@vl_lib_VL_I char(1),@vl_lib_VM1_I char(1),@vl_lib_VM2_I char(1),@vl_lib_VR_I char(1),
        @vl_lib_BAU char(1),@vl_lib_VL char(1),@vl_lib_VM1 char(1),@vl_lib_VM2 char(1),@vl_lib_VR char(1),
        @vl_sens tinyint,@vl_lib_sens char(1) ,@vl_lib_sens_oppose char(1), @vl_synoptique char(23),
        @vl_PR_debut_BAU int, @vl_PR_fin_BAU int,@vl_PR_debut_BAU_I int, @vl_PR_fin_BAU_I int,
	@vl_PR_debut_VL int,@vl_PR_fin_VL int, @vl_PR_debut_VL_I int, @vl_PR_fin_VL_I int,
	@vl_PR_debut_VM1 int,@vl_PR_fin_VM1 int, @vl_PR_debut_VM1_I int, @vl_PR_fin_VM1_I int,
	@vl_PR_debut_VM2 int,@vl_PR_fin_VM2 int, @vl_PR_debut_VM2_I int, @vl_PR_fin_VM2_I int,
	@vl_PR_debut_VR int,@vl_PR_fin_VR int, @vl_PR_debut_VR_I int, @vl_PR_fin_VR_I int,
	@vl_lib_PR1 char(7), @vl_lib_PR2 char(7),
	@vl_compte int,@vl_PR1 int, @vl_PR2 int,@vl_PR int, @vl_autoroute tinyint

	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_prtete_out=BAS.localisation_PR_fin,@vl_PR2=HIS.PR,
		@va_longueur_out = abs(HIS.PR - BAS.localisation_PR_fin),
	        @vl_BAU_etroite=HIS.BAU_etroite, @vl_BAU_I_etroite=HIS.BAU_I_etroite,
	       @vl_sens=HIS.sens, @vl_autoroute =HIS.autoroute, @vl_PR1=BAS.localisation_PR_fin,
		@va_vitesse_out=BAS.vitesse, @va_vitesse_opposee_out=BAS.vitesse_opposee
	from EXP..FMC_BAS BAS, EXP..FMC_HIS_DER HIS
	where 
		BAS.numero = @va_numero_in and
		BAS.cle = @va_cle_in and
		BAS.numero = HIS.numero and
		BAS.cle = HIS.cle

        /* METACOR */
	/* construction de la restriction */
        select
	      @vl_VR=VR, @vl_VM2=VM2, @vl_VM1=VM1, @vl_VL=VL, @vl_BAU=BAU,
	     @vl_VR_I=VR_I, @vl_VM2_I=VM2_I, @vl_VM1_I=VM1_I, @vl_VL_I=VL_I, @vl_BAU_I=BAU_I,
		@va_vitesse_out=vitesse, @va_vitesse_opposee_out = vitesse_opposee
        from EXP..FMC_BAS where numero=@va_numero_in and cle=@va_cle_in

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
	if @vl_VL_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)         select @vl_lib_VL_I = "X"
        else if  @vl_VL_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VL_I = @vl_lib_sens
	else if @vl_VL_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VL_I = null
	else select @vl_lib_VL_I = @vl_lib_sens_oppose
        if @vl_VM1_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)        select @vl_lib_VM1_I = "X"
        else if  @vl_VM1_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VM1_I = @vl_lib_sens
	else if @vl_VM1_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM1_I = null
	else select @vl_lib_VM1_I = @vl_lib_sens_oppose
	if @vl_VM2_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)        select @vl_lib_VM2_I = "X"
        else if  @vl_VM2_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VM2_I = @vl_lib_sens
	else if @vl_VM2_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM2_I = null
	else select @vl_lib_VM2_I = @vl_lib_sens_oppose
	if @vl_VR_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)         select @vl_lib_VR_I = "X"
        else if  @vl_VR_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VR_I = @vl_lib_sens
	else if @vl_VR_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VR_I =null
	else select @vl_lib_VR_I = @vl_lib_sens_oppose

	if (@vl_BAU_etroite= 1 or @vl_BAU in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)) select @vl_lib_BAU = "X"
	else if @vl_BAU in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_BAU = "X"
	else select @vl_lib_BAU = "-"
	if @vl_VL in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)   select @vl_lib_VL = "X"
        else if  @vl_VL=XDC_VOIE_SENS_INVERSE select @vl_lib_VL = @vl_lib_sens_oppose
	else if @vl_VL in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VL = null
	else select @vl_lib_VL = @vl_lib_sens
	if @vl_VM1 in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)  select @vl_lib_VM1 = "X"
        else if  @vl_VM1=XDC_VOIE_SENS_INVERSE select @vl_lib_VM1 = @vl_lib_sens_oppose
	else if @vl_VM1 in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM1 =null
	else select @vl_lib_VM1 = @vl_lib_sens
	if @vl_VM2 in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)          select @vl_lib_VM2 = "X"
        else if  @vl_VM2=XDC_VOIE_SENS_INVERSE select @vl_lib_VM2 = @vl_lib_sens_oppose
	else if @vl_VM2 in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM2 = null
	else select @vl_lib_VM2 = @vl_lib_sens
	if @vl_VR in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE)         select @vl_lib_VR = "X"
        else if  @vl_VR=XDC_VOIE_SENS_INVERSE select @vl_lib_VR = @vl_lib_sens_oppose
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

        select @vl_lib_PR1 = rtrim(convert(char(3),@vl_PR1/1000))+"."+right(rtrim(convert(char(6),@vl_PR1)),3)
	select @vl_lib_PR2 = rtrim(convert(char(3),@vl_PR2/1000))+"."+right(rtrim(convert(char(6),@vl_PR2)),3)

        select @va_restriction_out_2 = rtrim(ltrim("{1|" + @vl_lib_PR1 + "|" +  @vl_lib_PR2 + "|"  + @vl_synoptique + "}"))

      /* construction de la localisation */
      select @vl_pt_car=point_caracteristique, @vl_num_pt_car=numero_point_caracteristique, @vl_PR1=PR
         from EXP..FMC_HIS_DER where numero=@va_numero_in and cle=@va_cle_in

      if @vl_pt_car = XDC_POINT_CARACT_AIRE
      begin
	select @va_type_loc_out = XDC_POINT_CARACT_AIRE
	select @va_num_loc_out = @vl_num_pt_car
	select @va_nom_loc_out = nom from CFG..RES_AIR where numero=@vl_num_pt_car
      end
      else if @vl_pt_car = XDC_POINT_CARACT_ECHANGEUR
      begin
	/* les bifurcations sont typees en base comme des echangeurs reste 99 modulo 100 (99,199,299...)*/
        if @vl_num_pt_car%100 = 99 select @va_type_loc_out =XDC_POINT_CARACT_BIFURCATION
        else select @va_type_loc_out =XDC_POINT_CARACT_ECHANGEUR
        select @va_nom_loc_out = nom from CFG..RES_ECH where numero=@vl_num_pt_car
        select @va_num_loc_out = @vl_num_pt_car
      end
      else
      begin
        select @va_type_loc_out = 0
	select @va_nom_loc_out = nom from CFG..RES_AUT where numero=@vl_autoroute
	select @va_num_loc_out = 0
      end
	
	return XDC_OK
go

