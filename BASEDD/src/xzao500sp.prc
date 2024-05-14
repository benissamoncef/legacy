/*E*/
/* Fichier : $Id: xzao500sp.prc,v 1.17 2019/10/01 09:15:47 gesconf Exp $        Release : $Revision: 1.17 $        Date : $Date: 2019/10/01 09:15:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao500sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des equipements impactes par une perturbation
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	27/04/04	creation		1.1
* PCL	17/09/04	on renvoie un select	1.2
* PCL	20/10/04	on recherche en extion les pa allumes en amont du bouchon mais hors z.inf 1.3
* PCL	16/11/04	on distingue 3 cas : pas bouchon ou bouchon ou evt termine 1.4
* PCL	17/11/04	on distingue les curseurs .
			dans la 2eme phase on eteint les pal en aval de l'evt 1.5
* PCL	06/12/04	on n'eteint pas les PAL dans un bouchon a - de 200 m de la tete 1.6
* PCL	07/07/05	traces supplementaires dans hist 1.7
* PCL	24/10/06	maintenant, les pa dans le bouchon restent allum�s si le bouchon est en accordeon 1.8
* PCL	07/02/07	traces supplementaires dans hist 1.9
* PCL	05/02/08	gestion des trous des PR sur A50 REPERE1 1.10
* PCL	27/08/12	EIT ne veut plus allumer de PA pour les evenements en bretelle REPERE2 1.11 et 1.12
* JPL	12/12/16	Proposer les PA meme s'ils sont utilises (autorise un changement de scenario)  1.13
* JPL	12/12/16	Proposer d'eteindre les PA en amont de la zinf puis en aval (evite les doublons)  1.14
* PNI	04/05/17	Traitement des cas pour PAL en entr�e de bretelle 1.15 (DEM1226)
* PNI	27/09/17	Activation des PAL dans les bouchons ne bloquant pas toutes les voies 1.16 DEM1247
* PNI	23/09/17	Zone d'influence pour les bouchons doublée hors zone urbaine 1.17 DEM1353
* PNI	19/09/23	Zone d'influence devient hors zone urbaine, allumage de tous les PAL dans le bouchon SAE-522
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* 0 : pas bouchon.
* 1 : bouchon
* 2 : evt termine
*
* si evt non termine (0 ou 1) on eteint les PA hors zinf et on allume les PA en zinf
* si bouchon (1) on eteint les PA dans le bouchon, sauf de type accordeon
* si evt termine (2) on eteint tous les pa allumes pour cet evt
*
* les PA a eteindre sont reconnaissables en sortie par le fait que la distance est -99
*
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO500_PAL_amont
* 
* Arguments en entree
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Mot		va_Numero_in
* XDY_Horodate		va_Horodate_in
* 
* Arguments en sortie
* XDY_Eqt		va_NumPAL_out
* XDY_District		va_District_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* XDY_Entier		va_Distance_out
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
---------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO500' and type = 'P')
	drop procedure XZAO500
go


create procedure XZAO500
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PR_in		T_PR = null,
	@va_Sens_in		tinyint = null,
	@va_Numero_in		smallint = null,
	@va_distance_in		int = null,
	@va_Horodate_in		datetime = null,
	@va_PR_tete_in          T_PR = null,
	@va_Flag_Bouchon_in     bit,
	@va_numero_fmc_in       int=null,
	@va_cle_fmc_in          tinyint=null
as
declare @vl_PR_queue_artificiel T_PR,
	@vl_type_fmc tinyint, @vl_horodate_fin datetime, @vl_type_bouchon tinyint,
	@vl_numero_pal smallint, @vl_district_pal tinyint , @vl_PR_pal int, @vl_sens_pal tinyint,@vl_distance_pal_evt int,
	@vl_ordre_PR_autoroute smallint, @vl_PR_croissants smallint,
	@vl_sur_bretelle tinyint, @vl_compte int,
	@vl_sens_orig tinyint,@vl_sens_entree tinyint,
	@vl_BAU tinyint,
	@vl_VL tinyint,
	@vl_VM1 tinyint,
	@vl_VM2 tinyint,
	@vl_VR  tinyint,
	@vl_num int

/*A Controle des parametres d'entree obligatoire */

	if	@va_PR_in = null or
		@va_Sens_in = null or
		@va_distance_in = null

	return XDC_ARG_INV


/*A Recherche des caracteristiques de l'autoroute */
	select	@vl_ordre_PR_autoroute = inversee
	from	CFG..RES_AUT_INV
	where	numero = @va_Autoroute_in

	if @@rowcount = 0
	return XDC_ARG_INV

	if	@va_Sens_in=XDC_SENS_SUD 
	   select @vl_sens_entree=XDC_SENS_ENTREE_SUD
	if	@va_Sens_in=XDC_SENS_ENTREE_NORD
	   select @vl_sens_entree=XDC_SENS_ENTREE_NORD

/*A Ramene le sens sur section courante s'il s'agit d'une sortie ou d'une entree */
/* si evt en bretelle on force l'extinction des PA eventuellement allumes */
	select	@vl_sur_bretelle = XDC_FAUX
	if	@va_Sens_in=XDC_SENS_SORTIE_NORD /*or
		@va_Sens_in=XDC_SENS_ENTREE_NORD*/
	/*REPERE2 */
	begin
	  select	@vl_sur_bretelle = XDC_VRAI
	  select	@va_Sens_in=XDC_SENS_NORD
	end  
	
	if	@va_Sens_in=XDC_SENS_SORTIE_SUD /*or
		@va_Sens_in=XDC_SENS_ENTREE_SUD*/
	/*REPERE2 */
	begin
	  select	@vl_sur_bretelle = XDC_VRAI
	  select	@va_Sens_in=XDC_SENS_SUD
	end  

/*	if	@va_Sens_in=XDC_SENS_ENTREE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_NORD

	begin
	  select	@vl_sur_bretelle = XDC_VRAI
	end  */
	



/*A Si le sens est inconnu : retourne null (pas d'equipement amont) */
	
	if	@va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD and @va_Sens_in<>XDC_SENS_ENTREE_NORD and @va_Sens_in<>XDC_SENS_ENTREE_SUD
	  return XDC_OK


	select @vl_PR_croissants = XDC_VRAI
	if ( (@vl_ordre_PR_autoroute = XDC_PR_NOR)  and  (@va_Sens_in = XDC_SENS_NORD) )  or
	   ( (@vl_ordre_PR_autoroute = XDC_PR_INV)  and  (@va_Sens_in = XDC_SENS_SUD) )
		select @vl_PR_croissants = XDC_FAUX


   create table #TMP_PAL
      (  
		numero_pal  smallint null,
		district    tinyint  null, 
		autoroute   tinyint null,
		PR          int     null,
		sens        tinyint null,
		distance int null
      )

	  /** SAE-522 : double la zone pour tous les evt , toute zone */
select @va_distance_in = @va_distance_in * 2
select "distance=",@va_distance_in


/****** SAE-522
si on a un bouchon actif on double la distance hors zone urbaine 
if @va_Numero_in = 1 
begin

	select "regarde urb"
	select @vl_num=numero from CFG..ZON_URB where autoroute=@va_Autoroute_in and sens=@va_Sens_in and @va_PR_in >= debut and @va_PR_in <= fin
	if @@rowcount = 0
	begin 
		select @va_distance_in = @va_distance_in * 2
		select "distance=",@va_distance_in
	end
end
*************/

/* si on a un bouchon actif on calcule un PR de queue artificiel pour que les 1ers PA restent allumes dans les 200 m */
if @va_Numero_in = 1  and  ( abs(@va_PR_in - @va_PR_tete_in) > 200 )
begin
  if @vl_PR_croissants = XDC_VRAI
  begin
    select @vl_PR_queue_artificiel = @va_PR_in +  200
    select @va_distance_in = @va_distance_in + 200
  end
  else
  begin
    select @vl_PR_queue_artificiel = @va_PR_in -  200 
    select @va_distance_in = @va_distance_in + 200
  end
end
else
begin
  select @vl_PR_queue_artificiel = @va_PR_in
end

select @va_Numero_in
select isnull(@vl_sens_orig,@va_Sens_in)

/***************************************************************************************************************************/ 
/**********************Si evt termine on eteint les PA deja allumes pour cet evt    ****************************************/
/***************************************************************************************************************************/ 
if @va_Numero_in = 2  or  @vl_sur_bretelle = XDC_VRAI

begin
		
	declare Equipement1 cursor for
		select 	distinct EQT.numero,
			EQT.PR,
			EQT.sens,
			DSP.sit
		from	CFG..EQT_GEN EQT,
#ifdef HIST
			HIS..EQT_DSP DSP
#else
			EXP..EQT_DSP DSP
#endif
		where	EQT.type=XDC_EQT_PAL and
			EQT.autoroute=@va_Autoroute_in and
			/*(EQT.sens=@va_Sens_in or EQT.sens=@vl_sens_entree) and*/
			(EQT.sens=@va_Sens_in ) and
			DSP.equipement=EQT.numero and
			DSP.type=XDC_EQT_PAL and
			DSP.dernier=1 and 	-- derniere dispo connue
			DSP.inhibe=0 and 	-- non inhibe
			DSP.desactive=0 and 	-- non desactive
			DSP.majeure=0 and 	-- en panne       
			DSP.HS=0 		-- non HS

	open Equipement1

	fetch Equipement1 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal

        while (@@sqlstatus = 0 )
	begin
	  /* le pal est-il en cours d'utilisation */
	  select @vl_compte =0
#ifdef HIST
	  select @vl_compte= count(*) from HIS..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_succes <= @va_Horodate_in and heure_fin >= @va_Horodate_in
          if @vl_compte>0
            select "phase fin evenement destinee a eteindre les pa : on a "+convert(char(2),@vl_compte)+" actions PAL numero "+
				convert(char(2),@vl_numero_pal)+" en cours pour cet evenement"
#else       
	  select @vl_compte= count(*) from EXP..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_lancement < getdate() and heure_succes < getdate() and heure_lancement_fin is null and heure_fin is null
#endif       
	  if @vl_compte > 0
	  begin
            insert #TMP_PAL values (@vl_numero_pal,@vl_district_pal,@va_Autoroute_in,@vl_PR_pal,@vl_sens_pal,-99)
#ifdef HIST
	      select "phase fin evenement destinee a eteindre les PA : on eteint le pal "+convert(char(2),@vl_numero_pal)+" car evt termine" 
#endif      
	  end
         /* on passe a l'enregistrement suivant */
	 fetch Equipement1 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal
	end

        close Equipement1
	
  /*A  on retourne les resultats */

  select * from #TMP_PAL order by PR

  return XDC_OK

end
/***************************************************************************************************************************/ 
/**********************Si evt non termine on allume les PA amont  dans la zinf      ****************************************/
/**********************  et on eteint les PA hors zinf                              ****************************************/
/***************************************************************************************************************************/ 
else 

begin

  /*******************************************************************/
  /* premiere phase : on allume les PA amont s'ils sont dans la zinf */ 
  /*******************************************************************/
    
	declare Equipement2 cursor for
		select 	distinct EQT.numero,
			EQT.PR,
			EQT.sens,
			DSP.sit
		from	CFG..EQT_GEN EQT,
#ifdef HIST
			HIS..EQT_DSP DSP
#else
			EXP..EQT_DSP DSP
#endif
		where	EQT.type=XDC_EQT_PAL and
			EQT.autoroute=@va_Autoroute_in and
			/*(EQT.sens=@va_Sens_in or EQT.sens=@vl_sens_entree) and*/
			(EQT.sens=@va_Sens_in ) and
			DSP.equipement=EQT.numero and
			DSP.type=XDC_EQT_PAL and
			DSP.dernier=1 and 	-- derniere dispo connue
			DSP.inhibe=0 and 	-- non inhibe
			DSP.desactive=0 and 	-- non desactive
			DSP.majeure=0 and 	-- en panne       
			DSP.HS=0 		-- non HS

	open Equipement2

	fetch Equipement2 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal
-- select @va_Sens_in,@vl_sens_entree

select 	distinct EQT.numero,
			EQT.PR,
			EQT.sens,
			DSP.sit
		from	CFG..EQT_GEN EQT,
#ifdef HIST
			HIS..EQT_DSP DSP
#else
			EXP..EQT_DSP DSP
#endif
		where	EQT.type=XDC_EQT_PAL and
			EQT.autoroute=@va_Autoroute_in and
			/*(EQT.sens=@va_Sens_in or EQT.sens=@vl_sens_entree) and*/
			(EQT.sens=@va_Sens_in ) and
			/*(((EQT.sens=@va_Sens_in or (EQT.sens=and @vl_sens_orig=XDC_FAUX) or (EQT.sens=@vl_sens_orig and @vl_sens_orig!=XDC_FAUX)) and*/
			DSP.equipement=EQT.numero and
			DSP.type=XDC_EQT_PAL and
			DSP.dernier=1 /* and 	-- derniere dispo connue
			DSP.inhibe=0 and 	-- non inhibe
			DSP.desactive=0 and 	-- non desactive
			DSP.majeure=0 and 	-- en panne       
			DSP.HS=0 		-- non HS*/

	while (@@sqlstatus = 0 )
	begin

          /* recherche distance situee entre le PAL et l'evenement */
	  /*REPERE1*/
	  if (@va_Autoroute_in =3 and (@va_Sens_in=XDC_SENS_SUD or @va_Sens_in=XDC_SENS_ENTREE_SUD)and  @vl_PR_pal<16698 and @va_PR_in > 25847) 
			 select @vl_distance_pal_evt = @vl_PR_pal - @vl_PR_queue_artificiel + (25847-16698)
	  else
                         select @vl_distance_pal_evt = @vl_PR_pal-@vl_PR_queue_artificiel

	  if abs (@vl_distance_pal_evt) <= @va_distance_in  and
	     ( (@vl_PR_croissants = XDC_VRAI  and  @vl_PR_pal <= @vl_PR_queue_artificiel)  or
	       (@vl_PR_croissants = XDC_FAUX  and  @vl_PR_pal >= @vl_PR_queue_artificiel) )
	  begin
		insert #TMP_PAL 
		values (@vl_numero_pal,@vl_district_pal,@va_Autoroute_in,@vl_PR_pal,@vl_sens_pal,abs(@vl_distance_pal_evt))
	  end

          /* on passe a l'enregistrement suivant */
	  fetch Equipement2 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal
	end
	close Equipement2
  
  /***********************************************************************************/ 
  /* deuxieme phase : on eteint les PA qui sont en amont de la zinf                  */
  /***********************************************************************************/ 
  
	declare Equipement3 cursor for
		select 	distinct EQT.numero,
			EQT.PR,
			EQT.sens,
			DSP.sit
		from	CFG..EQT_GEN EQT,
#ifdef HIST
			HIS..EQT_DSP DSP
#else      
			EXP..EQT_DSP DSP
#endif
		where	EQT.type=XDC_EQT_PAL and
			EQT.autoroute=@va_Autoroute_in and
			/*(EQT.sens=@va_Sens_in or EQT.sens=@vl_sens_entree) and*/
			(EQT.sens=@va_Sens_in) and
			DSP.equipement=EQT.numero and
			DSP.type=XDC_EQT_PAL and
			DSP.dernier=1 and 	-- derniere dispo connue
			DSP.inhibe=0 and 	-- non inhibe
			DSP.desactive=0 and 	-- non desactive
			DSP.majeure=0 and 	-- en panne       
			DSP.HS=0 		-- non HS

	open Equipement3

	fetch Equipement3 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal

        while (@@sqlstatus = 0 )
	begin
	  /* le pal est-il en cours d'utilisation */
	  select @vl_compte =0
#ifdef HIST
	  select @vl_compte= count(*) from HIS..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_succes <= @va_Horodate_in and heure_fin >= @va_Horodate_in
          if @vl_compte>0
            select "phase 2 extinction des pa hors zinf : evt non termine, hors zinf, on a deja "+convert(char(2),@vl_compte)+
		       " actions en cours pour cet equipement numero "+convert(char(2),@vl_numero_pal)
#else
	  select @vl_compte= count(*) from EXP..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_lancement < getdate() and heure_succes < getdate() and heure_lancement_fin is null and heure_fin is null
#endif
	  if @vl_compte > 0
	  begin
            /* recherche distance situee entre le PAL et l'evenement */
            select @vl_distance_pal_evt = @vl_PR_pal-@vl_PR_queue_artificiel

	    /* si le pal est en amont de la zone d'influence alors proposer de l'eteindre */
	    if abs (@vl_distance_pal_evt) > @va_distance_in  and
	       ( (@vl_PR_croissants = XDC_VRAI  and  @vl_PR_pal <= @vl_PR_queue_artificiel)  or
	         (@vl_PR_croissants = XDC_FAUX  and  @vl_PR_pal >= @vl_PR_queue_artificiel) )
            begin
	        insert #TMP_PAL values (@vl_numero_pal,@vl_district_pal,@va_Autoroute_in,@vl_PR_pal,@vl_sens_pal,-99)
#ifdef HIST
          select "phase 2 extinction des pa en amont de la zinf :evt non termine, on eteint le pal " + convert(char(2),@vl_numero_pal)
#endif
              end
	  end
         /* on passe a l'enregistrement suivant */
	 fetch Equipement3 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal
	end

        close Equipement3

  /**********************************************************************************************************/	
  /* 3eme phase : si en plus c'est un bouchon, on eteint les PA dans le bouchon (sauf s'il est en accordeon)*/
  /**********************************************************************************************************/	
  if @va_Numero_in = 1
  begin
#ifdef HIST
    select @vl_type_bouchon=type from HIS..FMC_BOU where numero=@va_numero_fmc_in and cle=@va_cle_fmc_in and horodate<= @va_Horodate_in 
					  group by numero,cle having horodate=max(horodate)
    select @vl_BAU=BAU,@vl_VL=VL,@vl_VM1=VM1,@vl_VM2=VM2,@vl_VR=VR from HIS..FMC_HIS where numero=@va_numero_fmc_in and cle=@va_cle_fmc_in and horodate_validation<= @va_Horodate_in 
					  group by numero,cle having horodate_validation=max(horodate_validation)
#else
    select @vl_type_bouchon=type from EXP..FMC_BOU where numero=@va_numero_fmc_in and cle=@va_cle_fmc_in  group by numero,cle having horodate=max(horodate)
    select @vl_BAU=BAU,@vl_VL=VL,@vl_VM1=VM1,@vl_VM2=VM2,@vl_VR=VR from EXP..FMC_HIS where numero=@va_numero_fmc_in and cle=@va_cle_fmc_in  group by numero,cle having horodate_validation=max(horodate_validation)
#endif
/*** SAE-522 on allume tous les PAL dans un bouchon quelque soit le type ***
    if @vl_type_bouchon = XZAEC_BOU_Accordeon OR (@vl_VL=XDC_VOIE_SENS_NORMAL or @vl_VM1=XDC_VOIE_SENS_NORMAL or @vl_VM2=XDC_VOIE_SENS_NORMAL or @vl_VR=XDC_VOIE_SENS_NORMAL)
    ****/
  
#ifdef HIST
	select "phase 3 : on allume tous les PAL "
#endif

	declare Equipement6 cursor for
		select 	distinct EQT.numero,
			EQT.PR,
			EQT.sens,
			DSP.sit
		from	CFG..EQT_GEN EQT,
#ifdef HIST
			HIS..EQT_DSP DSP
#else
			EXP..EQT_DSP DSP
#endif
		where	EQT.type=XDC_EQT_PAL and
			EQT.autoroute=@va_Autoroute_in and
			/*(EQT.sens=@va_Sens_in or EQT.sens=@vl_sens_entree) and*/
			(EQT.sens=@va_Sens_in) and
			DSP.equipement=EQT.numero and
			DSP.type=XDC_EQT_PAL and
			DSP.dernier=1 and 	-- derniere dispo connue
			DSP.inhibe=0 and 	-- non inhibe
			DSP.desactive=0 and 	-- non desactive
			DSP.majeure=0 and 	-- en panne       
			DSP.HS=0 		-- non HS

	open Equipement6

	fetch Equipement6 into	@vl_numero_pal,
				@vl_PR_pal,
				@vl_sens_pal,
				@vl_district_pal

	while (@@sqlstatus = 0 )
	begin

          /* est-ce que le pa est dans le bouchon ? */
	  if ( ( @vl_PR_pal between @va_PR_tete_in and @vl_PR_queue_artificiel ) or
	       ( @vl_PR_pal between @vl_PR_queue_artificiel and @va_PR_tete_in ) )
	  begin
	    /* s'il existe deja une action pour cet equipement pour cet evt, on ne repropose pas l'evt */
	    select @vl_compte =0
#ifdef HIST
	    select @vl_compte= count(*) from HIS..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_succes < @va_Horodate_in and heure_lancement_fin > @va_Horodate_in
            if @vl_compte>0 
	      select "phase 3 allumage des pa dans bouchon en accordeon : bouchon en accordeon non termine, hors a57, sud , en zinf, on a deja "+
	      convert(char(2),@vl_compte)+ " actions en cours pour cet equipement numero "+convert(char(2),@vl_numero_pal) +
	      " on ne le repropose pas"
#else
	    select @vl_compte= count(*) from EXP..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_lancement < getdate() and heure_succes < getdate() and heure_lancement_fin is null and heure_fin is null
#endif
	    if @vl_compte = 0
	          insert #TMP_PAL 
		       values (@vl_numero_pal,@vl_district_pal,@va_Autoroute_in,@vl_PR_pal,@vl_sens_pal,abs(@vl_PR_pal - @va_PR_in))
	  end

          /* on passe a l'enregistrement suivant */
	  fetch Equipement6 into @vl_numero_pal,
				 @vl_PR_pal,
				@vl_sens_pal,
				 @vl_district_pal
	end
	close Equipement6
 
  end
  /******************************************************************************/	
  /* 3eme phase bis  : si ce n'est pas un bouchon on eteint les PA en aval      */
  /******************************************************************************/	
  else
  begin
	
	declare Equipement5 cursor for
		select 	distinct EQT.numero,
			EQT.PR,
			EQT.sens,
			DSP.sit
		from	CFG..EQT_GEN EQT,
#ifdef HIST
			HIS..EQT_DSP DSP
#else
			EXP..EQT_DSP DSP
#endif
		where	EQT.type=XDC_EQT_PAL and
			EQT.autoroute=@va_Autoroute_in and
			/*(EQT.sens=@va_Sens_in or EQT.sens=@vl_sens_entree) and*/
			(EQT.sens=@va_Sens_in) and
			DSP.equipement=EQT.numero and
			DSP.type=XDC_EQT_PAL and
			DSP.dernier=1 and 	-- derniere dispo connue
			DSP.inhibe=0 and 	-- non inhibe
			DSP.desactive=0 and 	-- non desactive
			DSP.majeure=0 and 	-- en panne       
			DSP.HS=0 		-- non HS

	open Equipement5

	fetch Equipement5 into	@vl_numero_pal,
				@vl_PR_pal,
				 @vl_sens_pal,
				@vl_district_pal

        while (@@sqlstatus = 0 )
	begin
	  /* le pal est-il en cours d'utilisation */
	  select @vl_compte =0
#ifdef HIST
	  select @vl_compte= count(*) from HIS..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_succes <= @va_Horodate_in and heure_fin >= @va_Horodate_in
          if @vl_compte>0 
	    select "phase 3 bis extinction des pa en aval : evt pas de type bouchon, non termine, on a "+convert(char(2),@vl_compte)+
		       " actions en cours pour cet equipement numero "+convert(char(2),@vl_numero_pal)
#else
	  select @vl_compte= count(*) from EXP..ACT_GEN where type=XDC_EQT_PAL and equipement=@vl_numero_pal and 
		     evenement=@va_numero_fmc_in and cle=@va_cle_fmc_in and
		     heure_lancement < getdate() and heure_succes < getdate() and heure_lancement_fin is null and heure_fin is null
#endif
	  if @vl_compte > 0
	  begin
            /* si le PAL est en aval de l'evenement alors proposer de l'eteindre */
            if ( @vl_PR_croissants = XDC_VRAI  and  @vl_PR_pal > @vl_PR_queue_artificiel )  or
               ( @vl_PR_croissants = XDC_FAUX  and  @vl_PR_pal < @vl_PR_queue_artificiel )
            begin
	      /* si oui on memorise le pal en table temporaire */
	      insert #TMP_PAL values (@vl_numero_pal,@vl_district_pal,@va_Autoroute_in,@vl_PR_pal,@va_Sens_in,-99)
#ifdef HIST
		select "phase 3 bis extinction des pa en aval : evt pas de type bouchon, non termine, on eteint le pal numero "+
		       convert(char(2),@vl_numero_pal)+" car en aval p/r a l'evt"
#endif
	    end 
	  end
         /* on passe a l'enregistrement suivant */
	 fetch Equipement5 into	@vl_numero_pal,
				@vl_PR_pal,
				 @vl_sens_pal,
				@vl_district_pal
	end

        close Equipement5
  end

/*A  on retourne les resultats */

select distinct * from #TMP_PAL order by PR

return XDC_OK

end

go
