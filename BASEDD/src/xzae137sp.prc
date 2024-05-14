/*E*/
/*  Fichier : $Id: xzae137sp.prc,v 1.9 2021/03/15 17:43:03 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2021/03/15 17:43:03 $
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
* Niepceron	11 Jan 1999 : modif intergest : !!! la longueur renvoir le pr tete !!! v1.2 dem/1624
* Niepceron     16 Mar 1999 : recuperation des lib type v1.3 dem/1741
* Cluchague     12 Jan 2007 : pour METACOR etat des voies et localisation  DEM605 v1.4
* Cluchague     30 Jan 2007 : On double le parametre restriction car > 255 car v1.5  
* Cluchague     15 Fev 2007 : on bricole pour que quand VR="X" et toutes les autres voies neutral on met VR_I=sens inverse v1.6
* VR			05 Oct 2011 : Ajout vitesse v1.7 (DEM/1015)
* JMG		08/03/18	ajout personnel 1.8
* PNI		08/03/18	@va_personnelout mis en output SAE_247 1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE137_Infos_Travaux_Pour_Strada
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

drop proc XZAE137
go

create procedure XZAE137
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_nature_out		bit output,
	@va_type_out		char(100) output,
	@va_mobile_out		bit output,
	@va_longueur_out	int output,
	@va_type_loc_out    	tinyint output,
	@va_nom_loc_out    	char(25)  output,
	@va_num_loc_out    	int output,
	@va_restriction_out_1	char(255) output,
	@va_restriction_out_2	char(255) output,
	@va_vitesse_out		tinyint output,
	@va_personnel_out 	tinyint output
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
	
	select 	@va_nature_out = TRF.nature,
		@vl_no_type = TRF.type,
		@va_mobile_out = mobile
	from EXP..FMC_TRF TRF
	where 
		TRF.numero = @va_numero_in and
		TRF.cle = @va_cle_in


	select @va_longueur_out=TRH.localisation_tete,
	@va_vitesse_out = TRH.vitesse,
	@va_personnel_out = TRH.personnel
	from EXP..FMC_TRH TRH 
	where TRH.numero = @va_numero_in and
	      TRH.cle = @va_cle_in 

	select @va_type_out=libelle
	from CFG..LIB_PRE
	where notyp= TYP_TRAVAUX and
		code=@vl_no_type

	/* METACOR */
	/* construction de la restriction */
        select @vl_BAU_etroite=BAU_etroite, @vl_BAU_I_etroite=BAU_I_etroite,
	       @vl_sens=sens, @vl_autoroute =autoroute
	from EXP..FMC_HIS_DER where numero=@va_numero_in and cle=@va_cle_in
        select 
	       @vl_VR=VR, @vl_VM2=VM2, @vl_VM1=VM1, @vl_VL=VL, @vl_BAU=BAU,
	       @vl_VR_I=VR_I, @vl_VM2_I=VM2_I, @vl_VM1_I=VM1_I, @vl_VL_I=VL_I, @vl_BAU_I=BAU_I
	from EXP..FMC_TRF where numero=@va_numero_in and cle=@va_cle_in

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

	create table #PR (PR int null)
	select @vl_PR_debut_BAU=PR_debut_BAU, @vl_PR_fin_BAU=PR_fin_BAU,@vl_PR_debut_BAU_I=PR_debut_BAU_I, @vl_PR_fin_BAU_I=PR_fin_BAU_I,
	       @vl_PR_debut_VL=PR_debut_VL,@vl_PR_fin_VL=PR_fin_VL, @vl_PR_debut_VL_I=PR_debut_VL_I, @vl_PR_fin_VL_I=PR_fin_VL_I,
	       @vl_PR_debut_VM1=PR_debut_VM1,@vl_PR_fin_VM1=PR_fin_VM1, @vl_PR_debut_VM1_I=PR_debut_VM1_I, @vl_PR_fin_VM1_I=PR_fin_VM1_I,
	       @vl_PR_debut_VM2=PR_debut_VM2,@vl_PR_fin_VM2=PR_fin_VM2, @vl_PR_debut_VM2_I=PR_debut_VM2_I, @vl_PR_fin_VM2_I=PR_fin_VM2_I,
	       @vl_PR_debut_VR=PR_debut_VR,@vl_PR_fin_VR=PR_fin_VR, @vl_PR_debut_VR_I=PR_debut_VR_I, @vl_PR_fin_VR_I=PR_fin_VR_I
	from EXP..FMC_TRF where numero=@va_numero_in and cle=@va_cle_in
	
	if @vl_PR_debut_BAU is not null insert #PR values (@vl_PR_debut_BAU)
	if @vl_PR_fin_BAU is not null insert #PR values (@vl_PR_fin_BAU)
	if @vl_PR_debut_BAU_I is not null insert #PR values (@vl_PR_debut_BAU_I)
	if @vl_PR_fin_BAU_I is not null insert #PR values (@vl_PR_fin_BAU_I)
	if @vl_PR_debut_VL is not null insert #PR values (@vl_PR_debut_VL)
	if @vl_PR_fin_VL is not null insert #PR values (@vl_PR_fin_VL)
	if @vl_PR_debut_VL_I is not null insert #PR values (@vl_PR_debut_VL_I)
	if @vl_PR_fin_VL_I is not null insert #PR values (@vl_PR_fin_VL_I)
	if @vl_PR_debut_VM1 is not null insert #PR values (@vl_PR_debut_VM1)
	if @vl_PR_fin_VM1 is not null insert #PR values (@vl_PR_fin_VM1)
	if @vl_PR_debut_VM1_I is not null insert #PR values (@vl_PR_debut_VM1_I)
	if @vl_PR_fin_VM1_I is not null insert #PR values (@vl_PR_fin_VM1_I)
	if @vl_PR_debut_VM2 is not null insert #PR values (@vl_PR_debut_VM2)
	if @vl_PR_fin_VM2 is not null insert #PR values (@vl_PR_fin_VM2)
	if @vl_PR_debut_VM2_I is not null insert #PR values (@vl_PR_debut_VM2_I)
	if @vl_PR_fin_VM2_I is not null insert #PR values (@vl_PR_fin_VM2_I)
	if @vl_PR_debut_VR is not null insert #PR values (@vl_PR_debut_VR)
	if @vl_PR_fin_VR is not null insert #PR values (@vl_PR_fin_VR)
	if @vl_PR_debut_VR_I is not null insert #PR values (@vl_PR_debut_VR_I)
	if @vl_PR_fin_VR_I is not null insert #PR values (@vl_PR_fin_VR_I)

	select @vl_compte = 1, @va_restriction_out_1 = "", @va_restriction_out_2 = ""

	declare PRs cursor for select distinct PR from #PR order by PR
	open PRs

	fetch PRs into @vl_PR1
        fetch PRs into @vl_PR
	while @@sqlstatus = 0
	begin

	  select @vl_PR2 = @vl_PR1 , @vl_PR1=@vl_PR
	  if (@vl_BAU_I_etroite= 1 or @vl_PR1 between @vl_PR_debut_BAU_I and @vl_PR_fin_BAU_I) select @vl_lib_BAU_I = "X"
	  else if @vl_BAU_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_BAU_I = "X"
	  else select @vl_lib_BAU_I = "-"
	  
	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VL_I and @vl_PR_fin_VL_I ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VL_I and @vl_PR_debut_VL_I )
	  begin
	    if  @vl_VL_I=XDC_VOIE_SENS_NORMAL select @vl_lib_VL_I = @vl_lib_sens_oppose
	    else if  @vl_VL_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VL_I = "X"
	    else if  @vl_VL_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VL_I = "X"
	    else select @vl_lib_VL_I = "X"
	  end
	  else if @vl_VL_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VL_I = null
	  else  select @vl_lib_VL_I =  @vl_lib_sens_oppose

	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VM1_I and @vl_PR_fin_VM1_I ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VM1_I and @vl_PR_debut_VM1_I )
	  begin
	    if  @vl_VM1_I=XDC_VOIE_SENS_NORMAL select @vl_lib_VM1_I = @vl_lib_sens_oppose
	    else if  @vl_VM1_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VM1_I = "X"
	    else if  @vl_VM1_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VM1_I = "X"
	    else select @vl_lib_VM1_I = "X"
	  end
	  else if @vl_VM1_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM1_I = null
	  else  select @vl_lib_VM1_I =  @vl_lib_sens_oppose

	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VM2_I and @vl_PR_fin_VM2_I ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VM2_I and @vl_PR_debut_VM2_I )
	  begin
	    if  @vl_VM2_I=XDC_VOIE_SENS_NORMAL select @vl_lib_VM2_I = @vl_lib_sens_oppose
	    else if  @vl_VM2_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VM2_I = "X"
	    else if  @vl_VM2_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VM2_I = "X"
	    else select @vl_lib_VM2_I = "X"
	  end
	  else if @vl_VM2_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM2_I = null
	  else  select @vl_lib_VM2_I =  @vl_lib_sens_oppose

	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VR_I and @vl_PR_fin_VR_I ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VR_I and @vl_PR_debut_VR_I )
	  begin
	    if  @vl_VR_I=XDC_VOIE_SENS_NORMAL select @vl_lib_VR_I = @vl_lib_sens_oppose
	    else if  @vl_VR_I=XDC_VOIE_SENS_INVERSE select @vl_lib_VR_I = "X"
	    else if  @vl_VR_I in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VR_I = "X"
	    else select @vl_lib_VR_I = "X"
	  end
	  else if @vl_VR_I in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VR_I = null
	  else  select @vl_lib_VR_I =  @vl_lib_sens_oppose

	  /* VR fait l'objet d'un traitement particulier a la fin du traitement des voies */

	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VM2 and @vl_PR_fin_VM2 ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VM2 and @vl_PR_debut_VM2 )
	  begin
	    if  @vl_VM2=XDC_VOIE_SENS_NORMAL select @vl_lib_VM2 = @vl_lib_sens
	    else if  @vl_VM2=XDC_VOIE_SENS_INVERSE select @vl_lib_VM2 = "X"
	    else if  @vl_VM2 in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VM2 = "X"
	    else select @vl_lib_VM2 = "X"
	  end
	  else if @vl_VM2 in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM2 = null
	  else  select @vl_lib_VM2 =  @vl_lib_sens

	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VM1 and @vl_PR_fin_VM1 ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VM1 and @vl_PR_debut_VM1 )
	  begin
	    if  @vl_VM1=XDC_VOIE_SENS_NORMAL select @vl_lib_VM1 = @vl_lib_sens
	    else if  @vl_VM1=XDC_VOIE_SENS_INVERSE select @vl_lib_VM1 = "X"
	    else if  @vl_VM1 in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VM1 = "X"
	    else select @vl_lib_VM1 = "X"
	  end
	  else if @vl_VM1 in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VM1 = null
	  else  select @vl_lib_VM1 =  @vl_lib_sens

	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VL and @vl_PR_fin_VL ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VL and @vl_PR_debut_VL )
	  begin
	    if  @vl_VL=XDC_VOIE_SENS_NORMAL select @vl_lib_VL = @vl_lib_sens
	    else if  @vl_VL=XDC_VOIE_SENS_INVERSE select @vl_lib_VL = "X"
	    else if  @vl_VL in (XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) select @vl_lib_VL = "X"
	    else select @vl_lib_VL = "X"
	  end
	  else if @vl_VL in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VL = null
	  else  select @vl_lib_VL =  @vl_lib_sens

	  if (@vl_BAU_etroite= 1 or @vl_PR1 between @vl_PR_debut_BAU and @vl_PR_fin_BAU) select @vl_lib_BAU = "X"
	  else if @vl_BAU in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_BAU = "X"
	  else select @vl_lib_BAU = "-"
	  
	  if ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_debut_VR and @vl_PR_fin_VR ) or
	     ((@vl_PR1+@vl_PR2)/2 between  @vl_PR_fin_VR and @vl_PR_debut_VR )
	  begin
	    if  @vl_VR=XDC_VOIE_SENS_NORMAL select @vl_lib_VR = @vl_lib_sens
	    else if  @vl_VR in (XDC_VOIE_SENS_INVERSE,XDC_VOIE_BLOQUEE,XDC_VOIE_NEUTRALISEE,XDC_VOIE_BLOQUEE_NEUTRALISEE) 
	    /* si la VR est neutralisee et si toutes les autres voies du meme sens le sont aussi, on passe VR_I en circulation sens inverse */
	    begin
	      if (@vl_lib_VL in (null,"X") and @vl_lib_VM1 in (null,"X") and @vl_lib_VM2 in (null,"X"))
	      begin
	        select @vl_lib_VR_I = @vl_lib_sens
              end
	      select @vl_lib_VR = "X"
            end
	    else select @vl_lib_VR_I = "X"
	  end
	  else if @vl_VR in (XDC_VOIE_INCONNUE,XDC_VOIE_INEXISTANTE) select @vl_lib_VR = null
	  else  select @vl_lib_VR =  @vl_lib_sens

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
          
	  if @vl_lib_sens = "1"
	      select @va_restriction_out_2 =  rtrim(ltrim(@va_restriction_out_2)) + "{" + convert (char(1),@vl_compte) + "|" + @vl_lib_PR2 + "|" + 
				       @vl_lib_PR1 + "|" + @vl_synoptique + "}"
	  else
	      select @va_restriction_out_2 =  rtrim(ltrim(@va_restriction_out_2)) + "{" + convert (char(1),@vl_compte) + "|" + @vl_lib_PR1 + "|" +
				       @vl_lib_PR2 + "|" + @vl_synoptique + "}"
	  
	  select @vl_compte = @vl_compte + 1
	  if char_length(rtrim(ltrim(@va_restriction_out_2))) > 200 
	  begin
	    select @va_restriction_out_1 =  rtrim(ltrim(@va_restriction_out_2))
	    select @va_restriction_out_2 = ""
	  end
	  fetch PRs into @vl_PR
	end
	select @va_restriction_out_1 =  rtrim(ltrim(@va_restriction_out_1))
	select @va_restriction_out_2 =  rtrim(ltrim(@va_restriction_out_2))
	close PRs
	drop table #PR
	/* si la restriction est vide (cas de travaux sur lit d'arret par exemple car lg des travaux = 0 , on met un petit blanc dans la restriction */
	if char_length(@va_restriction_out_1) = 0 select @va_restriction_out_1 = "   "
	if char_length(@va_restriction_out_2) = 0 select @va_restriction_out_2 = "   "

	/* construction de la localisation */
	select @vl_pt_car=point_caracteristique, @vl_num_pt_car=numero_point_caracteristique
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

