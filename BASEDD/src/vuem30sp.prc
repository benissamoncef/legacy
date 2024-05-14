/*E*/
/*  Fichier : $Id: vuem30sp.prc,v 1.3 2011/12/06 15:35:43 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2011/12/06 15:35:43 $
-------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   procedure stockee sur l'extraction de tous les travaux du reseau entre 2 dates
*	pour calcul CIGARE
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* PN	14/06/1999	: Creation
* PN    29/09/1999	: Modif ordre de la liste de trx 1.2
* VR	05/10/2011	: Traitement vitesse et vitesse_opposee 1.3(DEM/1015)
* GGY	26/09/23	: Ajout district CA (DEM-483)
---------------------------------------------------------------------------- */


use PRC
go

create procedure VUEM30
@va_district	tinyint,
@va_Debut       datetime,
@va_Fin         datetime,
@va_prevu	bit

as

/* declaration des variables */
declare 
  @vl_numero int , @vl_cle tinyint , @vl_numero_conseq int , @vl_cle_conseq tinyint , @vl_debut datetime , @vl_fin datetime , @vl_debut_plan datetime, @vl_fin_plan datetime, @vl_debut_basculement datetime , @vl_fin_basculement datetime , @vl_origine char(25) , 
  @vl_autoroute tinyint ,  @vl_PR int ,@vl_PR_basculement_tete int ,@vl_PR_basculement int , @vl_sens tinyint , @vl_sens_inverse tinyint , @vl_PR_max int, @vl_PR_min int,
  @vl_localisation_tete int , @vl_voies_retrecies bit , @vl_nature bit , @vl_type tinyint , @vl_chantier_mobile bit , @vl_continu bit ,
  @vl_nom_autoroute char(4) ,  @vl_PR_debut int , @vl_PR_fin int , @vl_debut_formate char (19) , @vl_fin_formate char(19) ,
  @vl_duree int , @vl_duree_formate char(7) ,  @vl_travaux_formate char(9) , @vl_nature_formate char(50) , 
  @vl_BAU_I tinyint ,  @vl_BAU_I_formate char(1) , @vl_VL_I tinyint , @vl_VL_I_formate char(1) ,
  @vl_VM1_I tinyint ,  @vl_VM1_I_formate char(1) , @vl_VM2_I tinyint , @vl_VM2_I_formate char(1) ,
  @vl_VR_I tinyint ,  @vl_VR_I_formate char(1) , @vl_VR tinyint , @vl_VR_formate char(1) ,
  @vl_VM1 tinyint ,  @vl_VM1_formate char(1) , @vl_VM2 tinyint , @vl_VM2_formate char(1) ,
  @vl_BAU tinyint ,  @vl_BAU_formate char(1) , @vl_VL tinyint , @vl_VL_formate char(1) ,   
  @vl_basculement bit , @vl_voies_retrecies_formate char(3) , @date_lancement datetime,
  @vl_HxKM real , @vl_chantier_mobile_formate char(3) , @vl_premier_commentaire char(250) , @vl_dernier_commentaire char(250) ,
  @vl_heures int , @vl_minutes int , @vl_nb_voies_neutralisees int , @vl_longueur_travaux int ,
  @vl_VR_X int , @vl_VR_I_X int , @vl_VM2_X int , @vl_VM2_I_X int , @vl_VL_X int , @vl_VL_I_X int , @vl_VM1_X int , @vl_VM1_I_X int,
  @vl_portion int, @vl_nb_voies_init tinyint, @vl_nb_voies_restantes tinyint, @vl_debit int, @vl_station smallint, @vl_inversee smallint, @vl_sit T_SITE,@vl_vitesse tinyint, @vl_vitesse_trv tinyint,
  @vl_SiteCICA T_SITE

	/* Verification du site */
	select  @vl_SiteCICA = numero
		from CFG..RES_DIS
		where type = XDC_TYPEM_SD
  
if @va_district=null	select   @va_district=0


/* declaration de la table temporaire qui va etre generee */
create table #LISTE_TRAVAUX 
  ( numero int NULL, district tinyint NULL , autoroute char(4) NULL , sens tinyint NULL , PR_debut int NULL , PR_fin int NULL , debut datetime NULL , 
    debut_formate char(19) NULL, fin datetime NULL , fin_formate char(19) NULL , duree char(7) NULL , travaux char(9) NULL , nature char(50) NULL , 
    voies_init tinyint NULL, voies_restantes tinyint NULL, longueur int NULL,
    voies_retrecies char(3) NULL , debit int NULL , basculement bit , vitesse tinyint NULL, vitesse_trv tinyint NULL)
  
  
      
--select @date_lancement=getdate()
--select getdate()+
/* declaration du curseur */ 
if @va_district=0  
declare curseur_liste_travaux cursor
for select distinct
      G1.numero ,
      G1.cle ,
      isnull(G1.debut ,debut_prevu),
      isnull(G1.fin ,fin_prevue),
      G1.origine_creation ,
      TRF1.BAU_I ,
      TRF1.VL_I ,
      TRF1.VM1_I ,
      TRF1.VM2_I ,
      TRF1.VR_I ,
      TRF1.VR ,
      TRF1.VM2 ,
      TRF1.VM1 ,
      TRF1.VL ,
      TRF1.BAU ,
      TRF1.nature ,
      TRF1.type ,
      TRF1.mobile,
      TRF1.continu
    from 
         HIS..FMC_GEN G1,
         HIS..FMC_TRF TRF1
    where 
      TRF1.cle = G1.cle and
      TRF1.numero = G1.numero and
      G1.cloture<>"01/01/1950 12:00"
      and
      (
       (( G1.debut <= @va_Fin or (G1.debut=null and G1.debut_prevu<= @va_Fin)) and
        (G1.fin>= @va_Debut or (G1.fin=null and G1.fin_prevue >= @va_Debut) )) or
        (TRF1.continu=XDC_VRAI and TRF1.horodate_debut_planifiee <=  @va_Fin and TRF1.horodate_fin_planifiee >= @va_Debut)
      )
      and datex=XDC_FAUX
   order by debut_prevu,G1.debut
else
declare curseur_liste_travaux cursor
for select distinct
      G1.numero ,
      G1.cle ,
      isnull(G1.debut ,debut_prevu),
      isnull(G1.fin ,fin_prevue),
      G1.origine_creation ,
      TRF1.BAU_I ,
      TRF1.VL_I ,
      TRF1.VM1_I ,
      TRF1.VM2_I ,
      TRF1.VR_I ,
      TRF1.VR ,
      TRF1.VM2 ,
      TRF1.VM1 ,
      TRF1.VL ,
      TRF1.BAU ,
      TRF1.nature ,
      TRF1.type ,
      TRF1.mobile,
      TRF1.continu 
    from 
         HIS..FMC_GEN G1,
         HIS..FMC_TRF TRF1
    where 
      (G1.cle=@va_district or G1.cle=@vl_SiteCICA) and
      TRF1.cle = G1.cle and
      TRF1.numero = G1.numero and
      G1.cloture<>"01/01/1950 12:00"
      and
      (
       (( G1.debut <= @va_Fin or (G1.debut=null and G1.debut_prevu<= @va_Fin)) and
        (G1.fin>= @va_Debut or (G1.fin=null and G1.fin_prevue >= @va_Debut) )) or
        (TRF1.continu=XDC_VRAI and TRF1.horodate_debut_planifiee <=  @va_Fin and TRF1.horodate_fin_planifiee >= @va_Debut)
     )
      and datex=XDC_FAUX
   order by debut_prevu,G1.debut
    

/* initialisation de variables locales */

--   select getdate()
    
/* ouverture du curseur */
open curseur_liste_travaux
fetch curseur_liste_travaux 
  into @vl_numero , @vl_cle , @vl_debut , @vl_fin , @vl_origine , 
       @vl_BAU_I , @vl_VL_I , @vl_VM1_I , @vl_VM2_I , @vl_VR_I , 
       @vl_VR , @vl_VM2 , @vl_VM1 , @vl_VL , @vl_BAU ,
       @vl_nature , @vl_type , @vl_chantier_mobile , @vl_continu

/* tant 'l y a des enregsrements */
while (@@sqlstatus = 0)
begin
-- select @vl_numero,@vl_cle,@vl_debut , @vl_fin

   select 
      @vl_autoroute=autoroute ,
      @vl_PR=PR ,
      @vl_sens=sens ,
      @vl_localisation_tete=localisation_tete ,
      @vl_voies_retrecies=voies_retrecies ,
      @vl_vitesse_trv = vitesse
    from 
         HIS..FMC_HIS H1,
         HIS..FMC_TRH TRH1
    where
      H1.cle = @vl_cle and
      H1.numero =  @vl_numero and
      TRH1.cle = H1.cle and
      TRH1.numero = H1.numero 
     group by 
      H1.numero , H1.cle
     having
      H1.horodate_validation = max (H1.horodate_validation) and
      TRH1.horodate = max(TRH1.horodate) 
 
  /* recuperation du nom d 'autoroute */
 /*-------------------------*/
 select @vl_nom_autoroute = nom
 from CFG..RES_AUT where numero = @vl_autoroute
 select @vl_inversee=inversee
 from CFG..RES_AUT_INV where numero = @vl_autoroute

  /*recuperation dPR de debut et fin */
  /*---------------------------------*/
  select @vl_PR_debut = @vl_PR  ,    @vl_PR_fin = @vl_localisation_tete 
  
  select @vl_sit = district
  from CFG..RES_POR
  where	CFG..RES_POR.autoroute = @vl_autoroute and
	CFG..RES_POR.PR_debut <= @vl_PR and
	CFG..RES_POR.PR_fin > @vl_PR
  
  if @vl_sit<>@va_district and @va_district<>0
  begin
   	/* pointage par le curseur a l'enregistrement suivant */
	fetch curseur_liste_travaux 
  	into @vl_numero , @vl_cle , @vl_debut , @vl_fin , @vl_origine , 
   	    @vl_BAU_I , @vl_VL_I , @vl_VM1_I , @vl_VM2_I , @vl_VR_I , 
  	     @vl_VR , @vl_VM2 , @vl_VM1 , @vl_VL , @vl_BAU ,
  	     @vl_nature , @vl_type , @vl_chantier_mobile , @vl_continu
  	
  	continue
 
  end
  /* recherche consequence */
  select @vl_numero_conseq=null
  
  select @vl_numero_conseq=numero, @vl_cle_conseq=cle,
      @vl_debut_basculement=isnull(debut ,debut_prevu),
      @vl_fin_basculement=isnull(fin ,fin_prevue)
  from HIS..FMC_GEN
  where cause=@vl_numero and cle_cause=@vl_cle and type=XZAEC_FMC_Basculement

  if @vl_numero_conseq <> null
  begin
        select  @vl_PR_basculement=PR
        from HIS..FMC_HIS
        where numero=@vl_numero_conseq and cle=@vl_cle_conseq  
    	group by 
      		numero , cle
    	having
      		horodate_validation = max (horodate_validation) and
      		numero=@vl_numero_conseq and cle=@vl_cle_conseq
      		
  	select @vl_debut=@vl_debut_basculement ,@vl_fin=@vl_fin_basculement
  	select @vl_PR_basculement_tete=localisation_PR_fin
  	from HIS..FMC_BAS
  	where numero=@vl_numero_conseq and cle=@vl_cle_conseq
  end

  /*calcul des horoda formatees */
/*-----------------------------*/
  if @vl_continu=XDC_VRAI and @va_prevu=XDC_VRAI
  	select @vl_debut=horodate_debut_planifiee ,@vl_fin=horodate_fin_planifiee
  	from HIS..FMC_TRF
  	where numero=@vl_numero and cle=@vl_cle
  	
  /* Si pas de date de fin alors on met la fin de la periode demandee */
  if @vl_fin = null
  	select @vl_fin=@va_Fin

-- select @vl_debut ,@vl_fin

  select @vl_debut_formate = convert ( char (10) , @vl_debut , 103 ) + ' ' + convert ( char (8) , @vl_debut , 8 ) ,
         @vl_fin_formate   = convert ( char (10) , @vl_fin   , 103 ) + ' ' + convert ( char (8) , @vl_fin   , 8 ) 
         
  /* calcul de la duree */
  /*--------------------*/
  if @vl_continu<>XDC_VRAI or @va_prevu=XDC_FAUX
  begin
  select @vl_duree = datediff (minute,@vl_debut,@vl_fin)
  if @vl_duree < 0
  	select @vl_duree=0
  select @vl_heures = @vl_duree / 60
  select @vl_minutes = @vl_duree - (@vl_heures * 60 )
  if ( @vl_heures <= 9 )
  select @vl_duree_formate = convert ( char(1)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  if ( @vl_heures > 9 and @vl_heures < 100 )
  select @vl_duree_formate = convert ( char(2)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  if ( @vl_heures > 99 and @vl_heures < 1000 )
  select @vl_duree_formate = convert ( char(3)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  if ( @vl_heures > 999 and @vl_heures < 10000 )
  select @vl_duree_formate = convert ( char(4)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  if ( @vl_heures > 9999 and @vl_heures < 100000 )
  select @vl_duree_formate = convert ( char(5)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  if ( @vl_heures > 99999 and @vl_heures < 1000000 )
  select @vl_duree_formate = convert ( char(6)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  end
  else
  begin
  	select @vl_duree=datediff (day,@vl_debut,@vl_fin)
  	if @vl_fin < dateadd(day,@vl_duree,@vl_debut)
  		select @vl_duree=@vl_duree-1
  	
  	select @vl_duree=datediff (minute,dateadd(day,@vl_duree,@vl_debut),@vl_fin)
  	if @vl_duree < 0
  		select @vl_duree=0
  	select @vl_heures = @vl_duree / 60
 	 select @vl_minutes = @vl_duree - (@vl_heures * 60 )
 	 if ( @vl_heures <= 9 )
 	 select @vl_duree_formate = convert ( char(1)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
 	 if ( @vl_heures > 9 and @vl_heures < 100 )
  	select @vl_duree_formate = convert ( char(2)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
 	 if ( @vl_heures > 99 and @vl_heures < 1000 )
	  select @vl_duree_formate = convert ( char(3)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
	  if ( @vl_heures > 999 and @vl_heures < 10000 )
	  select @vl_duree_formate = convert ( char(4)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
 	 if ( @vl_heures > 9999 and @vl_heures < 100000 )
 	 select @vl_duree_formate = convert ( char(5)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
 	 if ( @vl_heures > 99999 and @vl_heures < 1000000 )
 	 select @vl_duree_formate = convert ( char(6)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  end
  
  /* conversion du type de travaux */
  /*-------------------------------*/
  select @vl_travaux_formate = "entretien"
  if @vl_nature = 1 select @vl_travaux_formate = "tvx neufs"
   
  /* conversion de la nature des travaux */
  /*-------------------------------------*/
  select @vl_nature_formate = libelle from CFG..LIB_PRE where notyp=21 and code=@vl_type
  
  /* comptage des voies */
  /*----------------------*/
  select @vl_nb_voies_restantes=0
  if @vl_VR = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VM1 = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VM2 =XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VL = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_BAU = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VR_I = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VM1_I = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VM2_I =XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_VL_I = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  if @vl_BAU_I = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1

  if @vl_PR > @vl_localisation_tete
  	select @vl_PR_max=@vl_PR,@vl_PR_min=@vl_localisation_tete
  else
  	select @vl_PR_max=@vl_localisation_tete,@vl_PR_min=@vl_PR
  	
  select @vl_portion=numero
  from CFG..RES_POR
  where PR_debut < @vl_PR and PR_fin >= @vl_PR and autoroute=@vl_autoroute
  
  select @vl_nb_voies_init=nombre_de_voies
  from CFG..RES_SEG 
  where portion=@vl_portion and PR_debut < @vl_PR and sens=@vl_sens
  group by nombre_de_voies
  having PR_debut=max(PR_debut)

	/* recherche de la vitesse */
  if (@vl_sens=XDC_SENS_NORD and @vl_inversee=1) or
  	   (@vl_sens=XDC_SENS_SUD and @vl_inversee=-1)
  	select @vl_vitesse=vitesse
  	from CFG..RES_POL,CFG..RES_POR 
 	where portion=numero and autoroute=@vl_autoroute and PR >= @vl_PR and sens=@vl_sens    having PR=min(PR) and sens=@vl_sens
  else
  	if (@vl_sens=XDC_SENS_SUD and @vl_inversee=1) or
  	   (@vl_sens=XDC_SENS_NORD and @vl_inversee=-1)
   	select @vl_vitesse=vitesse
  	from CFG..RES_POL,CFG..RES_POR 
  	where portion=numero and autoroute=@vl_autoroute and PR <= @vl_PR and sens=@vl_sens    having PR=max(PR) and sens=@vl_sens
 else
  	 select @vl_vitesse=null

  
  
  /* conversion du champ voies retrecies */
  /*-------------------------------------*/
  select @vl_voies_retrecies_formate = "non"
  if @vl_voies_retrecies = 1 select @vl_voies_retrecies_formate = "oui"

  /* Calcul du d�bit */
  if (@va_prevu = XDC_FAUX)
  begin
  	select @vl_station=station
  	from CFG..RES_INF, CFG..RES_POR
  	where numero=portion and autoroute=@vl_autoroute and CFG..RES_INF.PR_fin > @vl_PR_debut and CFG..RES_INF.PR_debut <= @vl_PR_debut and sens=@vl_sens
  	
  	select @vl_debit=max(Q)
  	from HIS..MES_TRA
  	where station=@vl_station and sens=@vl_sens and voie=0 and 
  		validite_Q=XDC_RADT_CORRECT and
  		horodate > @vl_debut and horodate < @vl_fin
  		
  	if @vl_debit=null select @vl_debit=0
  end
  else
  begin
  	select @vl_debit=0
  end
  
  if @vl_numero_conseq = null
  	select @vl_basculement=XDC_FAUX
  else
  	select @vl_basculement=XDC_VRAI
  
  /* sauvegarde de l'enregistrement dans la table temporaire      */
  /* si certaines conditions sont respectees                      */
  /*--------------------------------------------------------------*/
  if @vl_continu=XDC_VRAI and @va_prevu=XDC_VRAI
  begin
  	select @vl_heures=datepart(hour,@vl_fin),
  		@vl_minutes=datepart(minute,@vl_fin)
  	if  datepart(hour,@vl_fin) < datepart(hour,@vl_debut)
  		select @vl_fin_plan=dateadd(hour,@vl_heures,dateadd(minute,@vl_minutes,dateadd(day,1,dateadd(hour,-datepart(hour,@vl_debut),dateadd(minute,-datepart(minute,@vl_debut),@vl_debut)))))
  	else
  		select @vl_fin_plan=dateadd(hour,@vl_heures,dateadd(minute,@vl_minutes,dateadd(hour,-datepart(hour,@vl_debut),dateadd(minute,-datepart(minute,@vl_debut),@vl_debut))))
  	
	select @vl_debut_plan=@vl_debut
  	while (@vl_fin_plan <= @vl_fin) or (@vl_debut_plan<=@vl_fin)
  	begin
		  select @vl_debut_formate = convert ( char (10) , @vl_debut_plan , 103 ) + ' ' + convert ( char (8) , @vl_debut_plan , 8 ) ,
       			  @vl_fin_formate   = convert ( char (10) , @vl_fin_plan   , 103 ) + ' ' + convert ( char (8) , @vl_fin_plan   , 8 ) 
  		select @vl_duree = datediff (minute,@vl_debut_plan,@vl_fin_plan)
  		if @vl_duree < 0
  			select @vl_duree=0
		select @vl_heures = @vl_duree / 60
  		select @vl_minutes = @vl_duree - (@vl_heures * 60 )
  		if ( @vl_heures <= 9 )
  			select @vl_duree_formate = convert ( char(1)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  		if ( @vl_heures > 9 and @vl_heures < 100 )
  			select @vl_duree_formate = convert ( char(2)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  		if ( @vl_heures > 99 and @vl_heures < 1000 )
  			select @vl_duree_formate = convert ( char(3)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  		if ( @vl_heures > 999 and @vl_heures < 10000 )
  			select @vl_duree_formate = convert ( char(4)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  		if ( @vl_heures > 9999 and @vl_heures < 100000 )
  			select @vl_duree_formate = convert ( char(5)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  		if ( @vl_heures > 99999 and @vl_heures < 1000000 )
  			select @vl_duree_formate = convert ( char(6)  , @vl_heures ) + ':' + convert ( char(2) , @vl_minutes )
  		
  		if @vl_fin_plan >= @va_Debut
   			insert #LISTE_TRAVAUX
    			( numero, district  , autoroute  , sens  , PR_debut  , PR_fin  , debut  , 
   			 debut_formate , fin , fin_formate , duree , travaux , nature , 
    			voies_init , voies_restantes , longueur, voies_retrecies, debit , basculement, vitesse, vitesse_trv )
 
  			values (@vl_numero, @vl_sit , @vl_nom_autoroute , @vl_sens , @vl_PR_debut , @vl_PR_fin , @vl_debut_plan ,
  			@vl_debut_formate , @vl_fin_plan , @vl_fin_formate , @vl_duree_formate , @vl_travaux_formate , @vl_nature_formate ,
 			 @vl_nb_voies_init,@vl_nb_voies_restantes, abs(@vl_PR_debut - @vl_PR_fin), @vl_voies_retrecies_formate, @vl_debit , @vl_basculement, @vl_vitesse, @vl_vitesse_trv )
 		 
 		 
  		/* S'il y a un basculement consequence */
  		if @vl_numero_conseq <> null
 		begin
  			if @vl_sens=XDC_SENS_NORD
  				select @vl_sens_inverse=XDC_SENS_SUD
  			else
  	 			select @vl_sens_inverse=XDC_SENS_NORD
  			select @vl_nb_voies_restantes=0
  			if @vl_VR_I = XDC_VOIE_SENS_NORMAL
         		       select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VM1_I = XDC_VOIE_SENS_NORMAL
        		        select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VM2_I =XDC_VOIE_SENS_NORMAL
        		        select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VL_I = XDC_VOIE_SENS_NORMAL
            			    select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_BAU_I = XDC_VOIE_SENS_NORMAL
         		       select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VR = XDC_VOIE_SENS_INVERSE
         		       select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VM1 = XDC_VOIE_SENS_INVERSE
        		        select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VM2 =XDC_VOIE_SENS_INVERSE
        		        select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_VL = XDC_VOIE_SENS_INVERSE
       			         select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  			if @vl_BAU = XDC_VOIE_SENS_INVERSE
                		select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1

  	
  			select @vl_portion=numero
  			from CFG..RES_POR
  			where PR_debut > @vl_PR_basculement and PR_fin <= @vl_PR_basculement and autoroute=@vl_autoroute
  
  			select @vl_nb_voies_init=nombre_de_voies
  			from CFG..RES_SEG 
  			where portion=@vl_portion and PR_debut < @vl_PR and sens=@vl_sens_inverse
  			group by nombre_de_voies
  			having PR_debut=max(PR_debut)

			/* recherche de la vitesse */
  			if (@vl_sens=XDC_SENS_NORD and @vl_inversee=1) or
  	   		   (@vl_sens=XDC_SENS_SUD and @vl_inversee=-1)
  				select @vl_vitesse=vitesse
  				from CFG..RES_POL,CFG..RES_POR 
 				where portion=numero and autoroute=@vl_autoroute and PR >= @vl_PR and sens=@vl_sens    having PR=min(PR)
  			else
  			if (@vl_sens=XDC_SENS_SUD and @vl_inversee=1) or
  	   		   (@vl_sens=XDC_SENS_NORD and @vl_inversee=-1)
   				select @vl_vitesse=vitesse
  				from CFG..RES_POL,CFG..RES_POR 
  				where portion=numero and autoroute=@vl_autoroute and PR <= @vl_PR and sens=@vl_sens    having PR=max(PR)
 			else
  	 			select @vl_vitesse=null


  			insert #LISTE_TRAVAUX
    				( numero, district  , autoroute  , sens  , PR_debut  , PR_fin  , debut  , 
    				debut_formate , fin , fin_formate , duree , travaux , nature , 
    				voies_init , voies_restantes , longueur, voies_retrecies , debit , basculement, vitesse , vitesse_trv)
 
  			values (@vl_numero_conseq, @vl_sit , @vl_nom_autoroute , @vl_sens_inverse , @vl_PR_basculement_tete , @vl_PR_basculement , @vl_debut_plan ,
  				@vl_debut_formate , @vl_fin_plan , @vl_fin_formate , @vl_duree_formate , @vl_travaux_formate , @vl_nature_formate ,
  				@vl_nb_voies_init,@vl_nb_voies_restantes, abs(@vl_PR_debut - @vl_PR_fin), @vl_voies_retrecies_formate, @vl_debit , XDC_FAUX, @vl_vitesse, @vl_vitesse_trv )
  
 		 end
 		
 		select @vl_debut_plan=dateadd(day,1,@vl_debut_plan)
 		select @vl_fin_plan=dateadd(day,1,@vl_fin_plan)
  	end
  end
  else
  begin
  insert #LISTE_TRAVAUX
    ( numero, district  , autoroute  , sens  , PR_debut  , PR_fin  , debut  , 
    debut_formate , fin , fin_formate , duree , travaux , nature , 
    voies_init , voies_restantes , longueur, voies_retrecies, debit , basculement, vitesse , vitesse_trv )
 
  values (@vl_numero, @vl_sit , @vl_nom_autoroute , @vl_sens , @vl_PR_debut , @vl_PR_fin , @vl_debut ,
  @vl_debut_formate , @vl_fin , @vl_fin_formate , @vl_duree_formate , @vl_travaux_formate , @vl_nature_formate ,
  @vl_nb_voies_init,@vl_nb_voies_restantes, abs(@vl_PR_debut - @vl_PR_fin), @vl_voies_retrecies_formate, @vl_debit, @vl_basculement, @vl_vitesse, @vl_vitesse_trv  )
  
  /* S'il y a un basculement consequence */
  if @vl_numero_conseq <> null
  begin
  	if @vl_sens=XDC_SENS_NORD
  		select @vl_sens_inverse=XDC_SENS_SUD
  	else
  	 	select @vl_sens_inverse=XDC_SENS_NORD
  	select @vl_nb_voies_restantes=0
  	if @vl_VR_I = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VM1_I = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VM2_I =XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VL_I = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_BAU_I = XDC_VOIE_SENS_NORMAL
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VR = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VM1 = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VM2 =XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_VL = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1
  	if @vl_BAU = XDC_VOIE_SENS_INVERSE
                select @vl_nb_voies_restantes=@vl_nb_voies_restantes+1

  	
  	select @vl_portion=numero
  	from CFG..RES_POR
  	where PR_debut > @vl_PR_basculement and PR_fin <= @vl_PR_basculement and autoroute=@vl_autoroute
  
  	select @vl_nb_voies_init=nombre_de_voies
  	from CFG..RES_SEG 
  	where portion=@vl_portion and PR_debut < @vl_PR and sens=@vl_sens_inverse
  	group by nombre_de_voies
  	having PR_debut=max(PR_debut)

  	insert #LISTE_TRAVAUX
    		( numero, district  , autoroute  , sens  , PR_debut  , PR_fin  , debut  , 
    		debut_formate , fin , fin_formate , duree , travaux , nature , 
    		voies_init , voies_restantes , longueur, voies_retrecies , debit , basculement )
 
  	values (@vl_numero_conseq, @vl_sit , @vl_nom_autoroute , @vl_sens_inverse , @vl_PR_basculement_tete , @vl_PR_basculement , @vl_debut ,
  		@vl_debut_formate , @vl_fin , @vl_fin_formate , @vl_duree_formate , @vl_travaux_formate , @vl_nature_formate ,
  		@vl_nb_voies_init,@vl_nb_voies_restantes, abs(@vl_PR_debut - @vl_PR_fin), @vl_voies_retrecies_formate, @vl_debit , XDC_FAUX )
  
  end
  end

  /* pointage par le curseur a l'enregistrement suivant */
fetch curseur_liste_travaux 
  into @vl_numero , @vl_cle , @vl_debut , @vl_fin , @vl_origine ,  
       @vl_BAU_I , @vl_VL_I , @vl_VM1_I , @vl_VM2_I , @vl_VR_I , 
       @vl_VR , @vl_VM2 , @vl_VM1 , @vl_VL , @vl_BAU ,
       @vl_nature , @vl_type , @vl_chantier_mobile , @vl_continu
end

/* fermeture du curseur */
close curseur_liste_travaux
--select getdate()

/* retourner les donnees */
select
    numero, district  , autoroute  , sens  , PR_debut  , PR_fin  ,  
    debut_formate , fin_formate , duree , travaux , nature , longueur, 
    voies_init , voies_restantes , voies_retrecies , debit , basculement, vitesse, vitesse_trv
from #LISTE_TRAVAUX
/*order by debut*/
order by autoroute  , sens  , PR_debut

return XDC_OK
go
