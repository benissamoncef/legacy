/*------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao963sp.prc
------------------------------------------------------
* HISTORIQUE :
*		
* GRO  19/08/2020 : Creation   ! XZAO962 !
*			Calcul TDP_FCD DEM-SAE152 1.0
* LPE  05/10/2020 : Horodate -6 minutes DEM-SAE152 1.2
* LPE  05/10/2020 : Horodate entre -8 minutes et -2 minutes DEM-SAE152 1.3
* LPE  06/10/2020 : Remplacement de XDC_ARG_INV par XDC_OK hors réseau avec TDP, validite et validite_faible a 0 DEM-SAE152 1.4
* GRO  07/10/2020 : Vérification dénominateur différent de zero dans calcul du coef DEM-SAE152 1.5
* GRO  15/10/2020 : Suppression du filtre sur le sens d'origine pour effectuer le trajet DEM-SAE152 1.6
* LPE  17/11/2020 : Recuperation a partir du numero/cle d'une FMC bouchon du sens et de l'autoroute de la tete du bouchon DEM-SAE152 1.7
* ABE  02/12/2020 : Abaissement du seuil de validite faible de 20 vers 15 SEM-SAE152 1.8
* ABE  22/01/2020 : Ajout trajets manquant A50/1 -> A8/1, A50/1 -> A8/2, A57/2 -> A50/2 dem-SAE152 1.9
* ABE  01/02/2020 : Correction trajet A50 (hors trou de pr) vers A8/1 DEM-SAE152 1.10
* ABE  15/11/2021 : Fix calcul trajet A52 Vers Marseille en passant par l'A501 DEM-SAE318 1.11
* PNI      18/07/2022 : modification du seuil de prise en compte des données TDP FCD SAE-394
* LCL  12/12/2022 : Fix probleme tdp hybride vers VINTIMILLE PR 224000/230000 au 230088 SAE-318
* PNI  11/08/2023 : Suppression TDP A51 s2 -> A8 s1 SAE-506
* PNI  04/10/2023 : SAE-506 : Pour Vintimille N'ayant plus les données TDP_FCD au delà du PR 223500, ajustement:  au PR 223500 et + 220 secondes
* JPL	10/10/2023 : Correction PRs (A500/2 ->A50/1, A52/1 ->A501/2); calcul TDP avec PR Sup si PRs decroissants
* JPL	10/10/2023 : Support de segments quelconques en table TDP_FCD (PR_debut et PR_fin non multiples de 500) (SAE-504)
* JPL	14/11/2023 : Correction saut de la premiere portion d'autoroute s'il y en a plusieurs
* JPL	29/11/2023 : Suppression des limitations en debut A8,A50,A501 pour TDP vers Marseille et A7/A9  (SAE-504)
* JPL	04/12/2023 : Definition des trajets vers A7, A54 et A9 depuis A8 sens 2 (SAE-504)
* JPL	23/01/2024 : Creation   ! XZAO963 !                 Retour du TdP brut, les arrondis sont laisses a XZAO962
* JPL	31/01/2024 : Gestion N105 L.Bourgeois; PR debut A54 a 120m; Controle longueur portions vs. somme longueurs sections
* JPL	17/03/2024 : Support du calcul de Temps de parcours a une horodate donnee (SAE-526)
------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calculer le temps de parcours FCD (Floating Car Data) entre un PR d'origine et de destination
* 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO963_Calcul_TDP_FCD_Pur
* 
* Arguments en entree
* XDY_Autoroute		Numero d'autoroute d'origine
* XDY_PR			PR d'origine
* XDY_Sens        	Sens d'origine
* XDY_Autoroute		Numero d'autoroute de destination
* XDY_PR			PR de destination
* XDY_Sens        	Sens de destination
* XDY_Entier        	Numero de la FMC
* XDY_Octet         	Cle de la FMC
* XDY_Horodate		Horodate de calcul du Temps de parcours
* 
* Arguments en sortie
* XDY_TDP			TDP FCD
* XDY_Octet        	Validite du TDP
* XDY_Booleen		teste si validite faible rencontrée (=0 si aucune validité <20 sur le trajet, sinon 1)						
* 
* Code retour								
* XDC_OK
* XDC_NOK
* XDC_ARG_INV		parametre invalide
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
* Fonction
* Découpe le trajet en portions en fonction du nombre d'autoroutes empruntées
* Somme les TDP de chaque portion et calcule la validité correspondante
---------------------------------------------------------*/
use PRC
go
		
if exists (select * from sysobjects where name = 'XZAO963' and type = 'P')
	drop procedure XZAO963
go				

#ifdef HIST
	#define BDD_XPL HIS
#else
	#define BDD_XPL EXP
#endif


create procedure XZAO963
   @va_autoroute_in_o   tinyint, /*Numero d'autoroute*/
   @va_PR_in_o          int,
   @va_sens_in_o        tinyint, 
   @va_autoroute_in_d   tinyint, /*Numero d'autoroute*/
   @va_PR_in_d          int, 
   @va_sens_in_d        tinyint,
   @va_numero_in	int,
   @va_cle_in		tinyint,  
   @va_TDP_FCD_out      int output,
   @va_validite_out     tinyint output,		
   @va_validite_faible_out bit output,
   @va_horodate_in	datetime	= NULL
as 
		
declare 
    	@vl_A8     tinyint ,
	@vl_A500   tinyint ,
	@vl_A50    tinyint ,
	@vl_A501   tinyint ,
	@vl_A51    tinyint ,
	@vl_A52    tinyint ,
	@vl_A520   tinyint ,
	@vl_A57    tinyint ,
	@vl_A7     tinyint ,
	@vl_A9     tinyint ,
	@vl_A54    tinyint ,
	@vl_D35    tinyint ,
	@vl_D535   tinyint ,
	@vl_N105   tinyint,
		
	@vl_compt_segments	float,
	@vl_nb_segments		int,
	@vl_somme_validite 	int,
	@vl_nb_faibles		int,
	@vl_date 		datetime,
	@vl_date_suiv 		datetime,
	@vl_maintenant 		datetime,
	@vl_minutes		int,
	@vl_secondes		int,

    	@vl_PR_portion_o 	int,
    	@vl_PR_portion_d 	int,
    	@vl_autoroute_portion 	tinyint, 
    	@vl_sens_portion 	tinyint,
	@vl_PR_inf 		int, 
	@vl_PR_sup 		int, 
    	@vl_lgr_portion 	int,

    	@vl_lgr_sections 	int,
	@vl_PR_debut 		int, 
	@vl_PR_fin 		int, 
	@vl_distance		int, 
	@t 			int,
	@t_partiel		int,
	@v 			int,
	@vl_coef 		float,

    	@vl_type_fmc        	smallint,
    	@vl_numero_cause    	int,
    	@vl_cle_cause       	tinyint,
    	@vl_Status		int,
	
    /* Ajout manuel de secondes pour cas extremes (A8/1/230000 a A8/1/230088 par exemple - VINTIMILLE hybride)*/
    @ajustement_manuel int


/* Resultats par defaut */
select	@va_TDP_FCD_out=0,
	@va_validite_out=0,
	@va_validite_faible_out=0


/* Verifier l'horodate demandee (date courante par defaut) */
select @vl_maintenant = getdate()

if @va_horodate_in is null
	select @vl_date = @vl_maintenant
else if @va_horodate_in > @vl_maintenant
	return XDC_OK
else
	select @vl_date = @va_horodate_in

#ifdef HIST
if @vl_date <= (select dateadd (month, -12, @vl_maintenant))
	return XDC_OK
#else
if @vl_date <= (select dateadd (day, -1, @vl_maintenant))
begin
	exec @vl_Status = SQL_HIS.PRC..XZAO963 @va_autoroute_in_o, @va_PR_in_o, @va_sens_in_o,
	                                       @va_autoroute_in_d, @va_PR_in_d, @va_sens_in_d,
	                                       @va_numero_in, @va_cle_in,
	                                       @va_TDP_FCD_out output, @va_validite_out output, @va_validite_faible_out output,
	                                       @va_horodate_in
	return XDC_OK
end
#endif


/* Horodate des donnees exploitees (horodate x 6mn precedente) */
select	@vl_minutes = datepart (minute, @vl_date) % 6
select	@vl_secondes = datepart (second, @vl_date) + (@vl_minutes * 60)
select	@vl_date = dateadd (second, - @vl_secondes, @vl_date)
select	@vl_date = convert (datetime, convert (char, @vl_date))

/* Marge de precaution contre les delais d'arrivee des donnees en base */
if datediff (second, @vl_date, @vl_maintenant) < 150
	select @vl_date = dateadd (minute, -6, @vl_date)

select	@vl_date_suiv = dateadd (minute, 1, @vl_date)			-- precaution contre les donnees hors creneaux de 6 mn

#if defined (DEV)
	declare @vl_date_debut_txt	char (50)
	select @vl_date_debut_txt = convert (char, @vl_date, 109) + " - " + convert (char(8), @vl_date_suiv, 8)
	print "XZAO963 Dates debut: %1!", @vl_date_debut_txt
#endif

select      @vl_A8             = numero from CFG..RES_AUT where nom="A8"
select      @vl_A500           = numero from CFG..RES_AUT where nom="A500"
select      @vl_A50            = numero from CFG..RES_AUT where nom="A50"
select      @vl_A501           = numero from CFG..RES_AUT where nom="A501"
select      @vl_A51            = numero from CFG..RES_AUT where nom="A51"
select      @vl_A52            = numero from CFG..RES_AUT where nom="A52"
select      @vl_A520           = numero from CFG..RES_AUT where nom="A520"
select      @vl_A57            = numero from CFG..RES_AUT where nom="A57"
select      @vl_A7             = numero from CFG..RES_AUT where nom="A7"
select      @vl_A9             = numero from CFG..RES_AUT where nom="A9"
select      @vl_A54            = numero from CFG..RES_AUT where nom="A54"
select      @vl_D35            = numero from CFG..RES_AUT where nom="D35"
select      @vl_D535           = numero from CFG..RES_AUT where nom="D535"
select      @vl_N105           = numero from CFG..RES_AUT where nom="N105"

select  @vl_somme_validite=0,
	@vl_compt_segments=0
        
select  @vl_PR_portion_o=-1,
        @vl_PR_portion_d=-1,
	@vl_PR_inf=0, 
	@vl_PR_sup=0

select	@t=0,
	@v=0,
	@vl_coef=0,
	@vl_nb_faibles=0

select @ajustement_manuel=0

/*************************************************************************************************
 Creation d'une table temporaire pour stocker les PR des portions d'autoroutes parcourues 
 (PR d'origine, PR des bifurcations et PR de destination)
*************************************************************************************************/

		create table #Portions (   Numero_portion tinyint, /*id unique*/
				           PR_portion_o int,       /*PR de debut de la portion*/
				           PR_portion_d int,       /*PR de fin de la portion*/
				           Autoroute_portion tinyint,
				           Sens_portion tinyint)	

/*************************************************************************************************
 Creation d'une table temporaire pour stocker les dernieres donnees de TDP_FCD
 comprises entre les PR d'une portion parcourue pour une autoroute et un sens donnes
*************************************************************************************************/

		create table #TDP_Portion ( PR_debut	int,
		                            PR_fin	int,
		                            tdp		int,
		                            validite	tinyint)

/*************************************************************************************************
 Verification de l'existence de la FMC et qu'elle est bien de type bouchon 
 pour recuperer une autoroute et un sens de la tete du bouchon
*************************************************************************************************/

if(@va_numero_in>0 and @va_numero_in<>null)
begin
    /*Récupération du type, cause et cle_cause de la FMC*/
    select  @vl_type_fmc = type,
            @vl_numero_cause = cause,
            @vl_cle_cause = cle_cause
    from BDD_XPL..FMC_GEN
    where numero = @va_numero_in and cle = @va_cle_in
		
    /*Recuperation des informations d'autoroute et sens pour la tete du bouchon pour calculer le parcours*/
    if(@vl_type_fmc=XZAEC_FMC_QueueBouchon)   /*On verifie que le type du select précédent est bien XZAEC_FMC_QueueBouchon*/		
    begin
        select  @va_autoroute_in_d = autoroute,
                @va_sens_in_d = sens
        from BDD_XPL..FMC_HIS
	where numero = @vl_numero_cause and cle = @vl_cle_cause
	having horodate_validation=max(horodate_validation) and numero = @vl_numero_cause and cle = @vl_cle_cause
	/*On vérifie qu'on a bien qu'une FMC pour la tête du bouchon */
	if(@@rowcount <> 1) return XDC_OK
    end		    
end			


/***********************************************************************************************************************/
/*        Pour chaque cas, determination des portions, avec ajout des PR des bifurcations si changement d'autoroute         */
/**********************************************************************************************************************/

if(@va_PR_in_o<0 or @va_PR_in_d<0) begin return XDC_OK end

/***********************************************************************************/
/* Traitement specifique de la portion en bout d'A8 vers VINTIMILLE                */
/* Le reseau ESCOTA s'arrete au PR 224000 mais on a des TDP FCD jusqu'au PR 230000 */
/* La derniere portion va jusqu'au PR 230088 : on ajoute 3 secondes au TDP calcule */
/* uniquement sens 1 vers VINTIMILLE  (16 secondes / 500m --> 3 secondes / 100 m)  */
/* SAE-506 : N'ayant plus les données TDP_FCD au delà du PR 223500, ajustement:     */
/* + 220 seconde                                                                    */
/***********************************************************************************/


if (@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1 and @va_PR_in_d>230000)
begin
  /* on ajoute 2 secondes pour les 88 metres manquants entre le PR 230000 et le PR 230088 */
  select @ajustement_manuel=3+220
  /* on force le PR destination a 23000 : pas de TDP FCD au dela */
  select @va_PR_in_d = 223500
end


/*************************autoroute d'origine = autoroute de destination************************/		
if @va_autoroute_in_o=@va_autoroute_in_d
begin
    /* Verifie si en sens 1, PR origine inferieur a PR destination, si sens 2, superieur (SAUF A57)*/
    if      ((@va_sens_in_d=1 and @va_autoroute_in_d!=@vl_A57) OR  (@va_sens_in_d=2 and @va_autoroute_in_d=@vl_A57))
            AND @va_PR_in_o>@va_PR_in_d 
    begin		 		
	return XDC_OK
    end
        
    else if ((@va_sens_in_d=2 and @va_autoroute_in_d!=@vl_A57) OR (@va_sens_in_d=1 and @va_autoroute_in_d=@vl_A57))
            AND @va_PR_in_o<@va_PR_in_d 
    begin		 
	return XDC_OK				
    end

    else if (@va_autoroute_in_o=@vl_A8)
    begin		
        if(@va_PR_in_o<=224000 and @va_PR_in_d<=230000)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_A500)
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d<=3000)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end

    else if (@va_autoroute_in_o=@vl_A50)
    begin
        /*On vérifie que les PR demandés ne soient ni dans le trou, ni supérieurs au max*/
        if( @va_PR_in_o<=72780 and @va_PR_in_d<=72780 
            and (@va_PR_in_o<=16698 or @va_PR_in_o>=25847)
            and (@va_PR_in_d<=16698 or @va_PR_in_d>=25847))
        
        begin
            /*Si on est en sens 1, de part et d'autre du trou de PR*/
            if(@va_PR_in_o<=16698 and @va_PR_in_d>=25847)
            begin
                insert into #Portions values (0,@va_PR_in_o,16698,@va_autoroute_in_o,1)
                insert into #Portions values (1,25847,@va_PR_in_d,@va_autoroute_in_o,1)
            end

            /*Si on est en sens 2, de part et d'autre du trou de PR*/
            else if(@va_PR_in_o>=25847 and @va_PR_in_d<=16698)
            begin
                insert into #Portions values (0,@va_PR_in_o,25847,@va_autoroute_in_o,2)
                insert into #Portions values (1,16698,@va_PR_in_d,@va_autoroute_in_o,2)
            end

            else insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d)
        end
    end

    else if (@va_autoroute_in_o=@vl_A501)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d<=5375)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_A51)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_d<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d>=16500)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_A52)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d<=26900)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_A520)
    begin
        if(@va_PR_in_o<=2900 and @va_PR_in_d<=2900)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_A57)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=52360)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_D35)
    begin
        if(@va_PR_in_o<=5800 and @va_PR_in_d<=5800)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_D535)
    begin
        if(@va_PR_in_o<=3400 and @va_PR_in_d<=3400)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end
    else if (@va_autoroute_in_o=@vl_N105)
    begin
        if(@va_PR_in_o<=900 and @va_PR_in_d<=900)
        begin insert into #Portions values  (0,@va_PR_in_o,@va_PR_in_d,@va_autoroute_in_o,@va_sens_in_d) end
    end

end

/************************************* A8/1->  ***************************************************/
else if @va_autoroute_in_o=@vl_A8
begin
    /*-------------------------------  A8/1 ->A52/1 ---------------------------------------------*/
    if @va_autoroute_in_d=@vl_A52 and @va_sens_in_d=1
    begin		
        if (@va_PR_in_o<=30700 and @va_PR_in_d<=26900) 				
        begin
            insert into #Portions values (0,@va_PR_in_o,30700,@va_autoroute_in_o,1)
            insert into #Portions values (1,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A8/1 ->A50/1  --------------------------------------------*/
    else if @va_autoroute_in_d=@vl_A50 and @va_sens_in_d=1
    begin
        if (@va_PR_in_o<=30700 and @va_PR_in_d>=25847 and @va_PR_in_d<72780) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,30700,@va_autoroute_in_o,1)
            insert into #Portions values  (1,0,26900,@vl_A52,1)
            insert into #Portions values  (2,25847,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end

    /*-------------------------------- A8/1 ->A57/1  --------------------------------------------*/
    else if @va_autoroute_in_d=@vl_A57 and @va_sens_in_d=1
    begin
        if (@va_PR_in_o<=98800 and @va_PR_in_d<=52360)
        begin
            insert into #Portions values  (0,@va_PR_in_o,98800,@va_autoroute_in_o,1)
            insert into #Portions values  (1,52360,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A8/1 ->A57/2  --------------------------------------------*/
    else if @va_autoroute_in_d=@vl_A57 and @va_sens_in_d=2
    begin
        if (@va_PR_in_o<=30700 and @va_PR_in_d<=52360)
        begin
            insert into #Portions values  (0,@va_PR_in_o,30700,@va_autoroute_in_o,1)
            insert into #Portions values  (1,0,26900,@vl_A52,1)
            insert into #Portions values  (2,25847,72780,@vl_A50,1)
            insert into #Portions values  (3,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A8/1 ->A500/1  --------------------------------------------*/
    else if @va_autoroute_in_d=@vl_A500 and @va_sens_in_d=1
    begin
        if (@va_PR_in_o<=207400 and @va_PR_in_d<=3000)
        begin
            insert into #Portions values  (0,@va_PR_in_o,207400,@va_autoroute_in_o,1)
            insert into #Portions values  (1,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------  A8/1 ->A501/2 ---------------------------------------------*/
    if @va_autoroute_in_d=@vl_A501 and @va_sens_in_d=2
    begin
        if (@va_PR_in_o<=30700 and @va_PR_in_d<=5375) 				
        begin
            insert into #Portions values (0,@va_PR_in_o,30700,@va_autoroute_in_o,1)
            insert into #Portions values (1,0,20900,@vl_A52,1)
            insert into #Portions values (2,5375,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end		
/************************************* A8/2->  ***************************************************/
    /*-------------------------------- A8/2->A50/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o>=30700 and @va_PR_in_o<=224000 and @va_PR_in_d>=25847 and @va_PR_in_d<=72780) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,30700,@va_autoroute_in_o,2)
            insert into #Portions values  (1,0,26900,@vl_A52,1)				
            insert into #Portions values  (2,25847,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end	
    /*-------------------------------- A8/2->A50/2 (trou de PR)--------------------------------------------*/
    else if (@va_autoroute_in_d = @vl_A50 and @va_sens_in_d = 2)
    begin 
        if (@va_PR_in_o >= 98800 and @va_PR_in_o<=224000 and ((@va_PR_in_d<=16698) or (@va_PR_in_d>=25847 and @va_PR_in_d<=72780)))		      
        begin
            if (@va_PR_in_d>=25847)
            begin
                insert into #Portions values  (0,@va_PR_in_o,98800,@va_autoroute_in_o,2)
                insert into #Portions values  (1,52360,0,@vl_A57,1)
                insert into #Portions values  (2,72780,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end	
            else if (@va_PR_in_d<=16698)
            begin
                insert into #Portions values  (0,@va_PR_in_o,98800,@va_autoroute_in_o,2)
                insert into #Portions values  (1,52360,0,@vl_A57,1)
                insert into #Portions values  (2,72780,25847,@va_autoroute_in_d,@va_sens_in_d)
                insert into #Portions values  (3,16698,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end	
        end
    end
    /*-------------------------------- A8/2->A52/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o>=30700 and @va_PR_in_o<=224000 and @va_PR_in_d<=26900) 
        begin
            insert into #Portions values (0,@va_PR_in_o,30700,@va_autoroute_in_o,2)
            insert into #Portions values (1,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end    

    /*-------------------------------- A8/2->A51/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A51 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o>=17900 and @va_PR_in_o<=224000 and @va_PR_in_d>=16500 and @va_PR_in_d<=153000)
        begin
            insert into #Portions values (0,@va_PR_in_o,17900,@va_autoroute_in_o,2)
            insert into #Portions values (1,16500,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end 
    /*-------------------------------- A8/2->A57/1 --------------------------------------------*/
    if (@va_autoroute_in_d = @vl_A57 and @va_sens_in_d = 1)
    begin 
        if (@va_PR_in_o>=98800 and @va_PR_in_o<=224000 and @va_PR_in_d<=52360)
        begin
            insert into #Portions values  (0,@va_PR_in_o,98800,@va_autoroute_in_o,2)
            insert into #Portions values  (1,52360,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end		
    end
        /*-------------------------------- A8/2 ->A500/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A500 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o>=207400 and @va_PR_in_o<=224000 and @va_PR_in_d<=3000) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,207400,@va_autoroute_in_o,2)			
            insert into #Portions values  (1,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end	
        /*-------------------------------- A8/2 ->A501/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A501 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o>=30700 and @va_PR_in_o<=224000 and @va_PR_in_d<=5375) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,30700,@va_autoroute_in_o,2)
            insert into #Portions values  (1,0,20900,@vl_A52,1)				
            insert into #Portions values  (2,5375,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end							
        /*-------------------------------- A8/2 ->A7/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A7 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=224000 and @va_PR_in_d>=6000 and @va_PR_in_d<=247840)
        begin
            insert into #Portions values  (0,@va_PR_in_o,1130,@va_autoroute_in_o,2)
            insert into #Portions values  (1,247840,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
        /*-------------------------------- A8/2 ->A54/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A54 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=224000 and @va_PR_in_d<=72360)
        begin
            insert into #Portions values  (0,@va_PR_in_o,1130,@va_autoroute_in_o,2)
            insert into #Portions values  (1,247840,235380,@vl_A7,2)
            insert into #Portions values  (2,72360,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
        /*-------------------------------- A8/2 ->A9/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A9 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=224000 and @va_PR_in_d >= 54680 and @va_PR_in_d<=280480)
        begin
            insert into #Portions values  (0,@va_PR_in_o,1130,@va_autoroute_in_o,2)
            insert into #Portions values  (1,247840,235380,@vl_A7,2)
            insert into #Portions values  (2,72360,120,@vl_A54,2)
            insert into #Portions values  (3,54680,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* A50/1->  ***************************************************/
else if @va_autoroute_in_o=@vl_A50
begin
    /*-------------------------------- A50/1 ->A57/2 (trou de PR) --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A57 and @va_sens_in_d=2)
    begin
        if(((@va_PR_in_o<=16698) or (@va_PR_in_o>=25847 and @va_PR_in_o<=72780)) and @va_PR_in_d<=52360)
        begin
            if(@va_PR_in_o<=16698)
            begin
                insert into #Portions values (0,@va_PR_in_o,16698,@va_autoroute_in_o,1)
                insert into #Portions values (1,25847,72780,@vl_A50,1)
                insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end
            else if(@va_PR_in_o>=25847)
            begin
                insert into #Portions values (0,@va_PR_in_o,72780,@va_autoroute_in_o,1)
                insert into #Portions values (1,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end
        end
    end
    /*-------------------------------- A50/1 ->A8/1 --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(((@va_PR_in_o<=16698) or (@va_PR_in_o>=25847 and @va_PR_in_o<=72780)) and @va_PR_in_d<=230000)
        begin
            if(@va_PR_in_o<=16698)
            begin
                insert into #Portions values (0,@va_PR_in_o,16698,@va_autoroute_in_o,1)
                insert into #Portions values (1,25847,72780,@vl_A50,1)
		insert into #Portions values (2,0,52360,@vl_A57,2)
                insert into #Portions values (3,98800,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end
            else if(@va_PR_in_o>=25847)
            begin
                insert into #Portions values (0,@va_PR_in_o,72780,@va_autoroute_in_o,1)
		insert into #Portions values (1,0,52360,@vl_A57,2)
                insert into #Portions values (2,98800,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end
        end
    end
    /*-------------------------------- A50/1 ->A8/2 (à partir du trou de PR) --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin								
        if((@va_PR_in_o<=16698) and @va_PR_in_d<=30700)
        begin
	    insert into #Portions values (0,@va_PR_in_o,16698,@va_autoroute_in_o,1)
	    insert into #Portions values (1,26900,0,@vl_A52,2)
	    insert into #Portions values (2,30700,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
			
/************************************* A50/2->  ***************************************************/
    /*-------------------------------- A50/2 ->A52/2  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=72780 and @va_PR_in_o>=25847 and @va_PR_in_d<=26900) 
        begin
            insert into #Portions values (0,@va_PR_in_o,25847,@va_autoroute_in_o,2)
            insert into #Portions values (1,26900,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A50/2 ->A8/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=72780 and @va_PR_in_o>=25847 and @va_PR_in_d<=30700)
        begin
            insert into #Portions values (0,@va_PR_in_o,25847,@va_autoroute_in_o,2)
            insert into #Portions values (1,26900,0,@vl_A52,2)
            insert into #Portions values (2,30700,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A50/2 ->A51/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A51 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=72780 and @va_PR_in_o>=25847 and @va_PR_in_d>=16500 and @va_PR_in_d<=153000) 
        begin
            insert into #Portions values (0,@va_PR_in_o,25847,@va_autoroute_in_o,2)
            insert into #Portions values (1,26900,0,@vl_A52,2)
            insert into #Portions values (2,30700,17900,@vl_A8,2)
            insert into #Portions values (3,16500,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A50/2 ->A520/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A520 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=72780 and @va_PR_in_o>=25847 and @va_PR_in_d<=2900)
        begin
            insert into #Portions values (0,@va_PR_in_o,25847,@va_autoroute_in_o,2)
            insert into #Portions values (1,26900,17000,@vl_A52,2)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end
								
/************************************* A500/2->  ***************************************************/
else if @va_autoroute_in_o = @vl_A500
begin
    /*-------------------------------- A500/2 ->A8/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)	
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d>=207400 and @va_PR_in_d<=230000) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)			
            insert into #Portions values  (1,207400,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A500/2 ->A8/2  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)	
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d<=207400) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)			
            insert into #Portions values  (1,207400,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A500/2 ->A52/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d<=26900) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values  (1,207400,30700,@vl_A8,2)
            insert into #Portions values  (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A500/2 ->A50/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d>=25847 and @va_PR_in_d<=72780) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values  (1,207400,30700,@vl_A8,2)
            insert into #Portions values  (2,0,26900,@vl_A52,1)
            insert into #Portions values  (3,25847,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A500/2 ->A51/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A51 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d>=16500 and @va_PR_in_d<=153000) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values  (1,207400,17900,@vl_A8,2)
            insert into #Portions values  (2,16500,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
        /*-------------------------------- A500/2 ->A57/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A57 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=3000 and @va_PR_in_d<=52360) 
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values  (1,207400,98800,@vl_A8,2)
            insert into #Portions values  (2,52360,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* A501/1->  ***************************************************/
else if @va_autoroute_in_o=@vl_A501
begin
    /*-------------------------------- A501/1->A8/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d>=30700 and @va_PR_in_d<=230000)
        begin
            insert into #Portions values (0,@va_PR_in_o,5375,@va_autoroute_in_o,1)
            insert into #Portions values (1,20900,0,@vl_A52,2)
            insert into #Portions values (2,30700,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A501/1->A8/2  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d<=30700) 
        begin
            insert into #Portions values (0,@va_PR_in_o,5375,@va_autoroute_in_o,1)
            insert into #Portions values (1,20900,0,@vl_A52,2)
            insert into #Portions values (2,30700,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A501/1->A52/2  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d<=20900)
        begin
            insert into #Portions values (0,@va_PR_in_o,5375,@va_autoroute_in_o,1)
            insert into #Portions values (1,20900,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A501/1->A51/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A51 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d>=16500 and @va_PR_in_d<=153000)
        begin
            insert into #Portions values (0,@va_PR_in_o,5375,@va_autoroute_in_o,1)
            insert into #Portions values (1,20900,0,@vl_A52,2)
            insert into #Portions values (2,30700,17900,@vl_A8,2)
            insert into #Portions values (3,16500,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A501/1->A500/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A500 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d<=3000)
        begin
            insert into #Portions values (0,@va_PR_in_o,5375,@va_autoroute_in_o,1)
            insert into #Portions values (1,20900,0,@vl_A52,2)
            insert into #Portions values (2,30700,207400,@vl_A8,1)
            insert into #Portions values (3,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A501/1->A520/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A520 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=5375 and @va_PR_in_d<=2900)
        begin
            insert into #Portions values (0,@va_PR_in_o,5375,@va_autoroute_in_o,1)
            insert into #Portions values (1,20900,17000,@vl_A52,2)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* A51/2->  ***************************************************/
else if @va_autoroute_in_o=@vl_A51
begin
    /*-------------------------------- A51/2 ->A8/1  --------------------------------------------*/
    /* Suppression des données de TPVA A51 pr0 -> pr23 **************** SAE 506
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d>=17900 and @va_PR_in_d<=230000)
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end */
    /*-------------------------------- A51/2 ->A52/1  --------------------------------------------*/
    /*else */
    if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d<=26900)
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,30700,@vl_A8,1)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A51/2 ->A50/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d>=25847 and @va_PR_in_d<=72780)
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,30700,@vl_A8,1)
            insert into #Portions values (2,0,26900,@vl_A52,1)
            insert into #Portions values (3,25847,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A51/2 ->A57/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A57 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d<=52360) 
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,98800,@vl_A8,1)
            insert into #Portions values (2,52360,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A51/2 ->A57/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A57 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d<=52360)
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,30700,@vl_A8,1)
            insert into #Portions values (2,0,26900,@vl_A52,1)
            insert into #Portions values (3,25847,72780,@vl_A50,1)
            insert into #Portions values (4,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A51/2 ->A500/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A500 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d<=3000) 
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,207400,@vl_A8,1)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A51/2 ->A501/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A501 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=153000 and @va_PR_in_o>=16500 and @va_PR_in_d<=5375) 
        begin
            insert into #Portions values (0,@va_PR_in_o,16500,@va_autoroute_in_o,2)
            insert into #Portions values (1,17900,30700,@vl_A8,1)
            insert into #Portions values (2,0,20900,@vl_A52,1)
            insert into #Portions values (3,5375,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* 	A52/1->  ***************************************************/
else if @va_autoroute_in_o=@vl_A52
begin
    /*-------------------------------- A52/1 ->A50/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d>=25847 and @va_PR_in_d<=72780)
        begin
            insert into #Portions values (0,@va_PR_in_o,26900,@va_autoroute_in_o,1)
            insert into #Portions values (1,25847,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A52/1 ->A57/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A57 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d<=52360)
        begin
            insert into #Portions values (0,@va_PR_in_o,26900,@va_autoroute_in_o,1)
            insert into #Portions values (1,25847,72780,@vl_A50,1)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A52/1 ->A501/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A501 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=20900 and @va_PR_in_d<=5375) 
        begin
            insert into #Portions values (0,@va_PR_in_o,20900,@va_autoroute_in_o,1)
            insert into #Portions values (1,5375,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
/************************************* A52/2->  ***************************************************/
    /*-------------------------------- A52/2 ->A8/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d>=30700 and @va_PR_in_d<=230000)
        begin
            insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values (1,30700,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A52/2 ->A8/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d<=30700)
        begin
            insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values (1,30700,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A52/2 ->A51/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A51 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d>=16500 and @va_PR_in_d<=153000) 
        begin
            insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values (1,30700,17900,@vl_A8,2)
            insert into #Portions values (2,16500,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end

    /*-------------------------------- A52/2 ->A57/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A57 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=26900 and @va_PR_in_d<=52360)
        begin
            insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values (1,30700,98800,@vl_A8,1)
            insert into #Portions values (2,52360,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A52/2 ->A520/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A520 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o>=17000 and @va_PR_in_o<=26900 and @va_PR_in_d<=2900)
        begin
            insert into #Portions values (0,@va_PR_in_o,17000,@va_autoroute_in_o,2)
            insert into #Portions values (1,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end			

/************************************* A520/2->  ***************************************************/
else if ( @va_autoroute_in_o = @vl_A520)
begin
    /*-------------------------------- A520/2 ->A52/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=1)	
    begin
        if(@va_PR_in_o<=2900 and @va_PR_in_d>=17000 and @va_PR_in_d<=26900)
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)			
            insert into #Portions values  (1,17000,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A520/2 ->A50/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=2900 and @va_PR_in_d>=25847 and @va_PR_in_d<=72780)
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values  (1,17000,26900,@vl_A52,1)				
            insert into #Portions values  (2,25847,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A520/2 ->A501/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A501 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=2900 and @va_PR_in_d<=5375)
        begin
            insert into #Portions values  (0,@va_PR_in_o,0,@va_autoroute_in_o,2)
            insert into #Portions values  (1,17000,20900,@vl_A52,1)				
            insert into #Portions values  (2,5375,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end	
end								

/************************************* A57/1->  ***************************************************/
else if @va_autoroute_in_o=@vl_A57 and @va_sens_in_o=1
begin			
    /*-------------------------------- A57/1 ->A50/2 (trou de PR) --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=52360 and ((@va_PR_in_d<=16698) or (@va_PR_in_d>=25847 and @va_PR_in_d<=72780)))	
        begin
            if (@va_PR_in_d>=25847)
            begin
                insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,1)
                insert into #Portions values (1,72780,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end
            else if (@va_PR_in_d<=16698)
            begin
                insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,1)
                insert into #Portions values (1,72780,25847,@va_autoroute_in_d,@va_sens_in_d)
                insert into #Portions values (2,16698,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
            end
        end
    end
    /*-------------------------------- A57/1 ->A52/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=26900)
        begin
            insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,1)
            insert into #Portions values (1,72780,25847,@vl_A50,2)
            insert into #Portions values (2,26900,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A57/1 ->A520/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A520 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=2900)
        begin
            insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,1)
            insert into #Portions values (1,72780,25847,@vl_A50,2)												
            insert into #Portions values (2,26900,17000,@vl_A52,2)
            insert into #Portions values (3,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* A57/2->  ***************************************************/
else if @va_autoroute_in_o=@vl_A57 and @va_sens_in_o=2
begin								
    /*-------------------------------- A57/2 ->A8/1  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d>=98800 and @va_PR_in_d<=230000)
        begin
            insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,2)
            insert into #Portions values (1,98800,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A57/2 ->A50/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A50 and @va_sens_in_d=2)
    begin
	if(@va_PR_in_o>=0 and ((@va_PR_in_d<=16698) or (@va_PR_in_d>=25847 and @va_PR_in_d<=72780)))	
	begin
	    if (@va_PR_in_d>=25847)
	    begin
		insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,1)
		insert into #Portions values (1,72780,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
	    end
	    else if (@va_PR_in_d<=16698)
	    begin
		insert into #Portions values (0,@va_PR_in_o,0,@va_autoroute_in_o,1)
		insert into #Portions values (1,72780,25847,@va_autoroute_in_d,@va_sens_in_d)
		insert into #Portions values (2,16698,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
	    end
	end
    end
    /*-------------------------------- A57/2 ->A8/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=98800)
        begin
            insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,2)
            insert into #Portions values (1,98800,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A57/2 ->A52/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A52 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=26900)
        begin
            insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,2)
            insert into #Portions values (1,98800,30700,@vl_A8,2)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A57/2 ->A51/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A51 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d>=16500 and @va_PR_in_d<=153000)
        begin
            insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,2)
            insert into #Portions values (1,98800,17900,@vl_A8,2)
            insert into #Portions values (2,16500,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A57/2 ->A500/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A500 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=3000)
        begin
            insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,2)
            insert into #Portions values (1,98800,207400,@vl_A8,1)
            insert into #Portions values (2,0,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- A57/2 ->A501/2  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A501 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=52360 and @va_PR_in_d<=5375) 
        begin
            insert into #Portions values (0,@va_PR_in_o,52360,@va_autoroute_in_o,2)
            insert into #Portions values (1,98800,30700,@vl_A8,2)
            insert into #Portions values (2,0,20900,@vl_A52,1)
            insert into #Portions values (3,5375,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* D35/2->  ***************************************************/
else if @va_autoroute_in_o=@vl_D35
begin
    /*-------------------------------- D35/2 ->A8/2  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=5800 and @va_PR_in_d<=171040) 
        begin
            insert into #Portions values (0,@va_PR_in_o,5800,@va_autoroute_in_o,2)
            insert into #Portions values (1,171040,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- D35/2 ->A8/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=5400 and @va_PR_in_d>=172100 and @va_PR_in_d<=230000)
        begin
            insert into #Portions values (0,@va_PR_in_o,5400,@va_autoroute_in_o,2)
            insert into #Portions values (1,172100,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end

/************************************* D535/2->  ***************************************************/
else if @va_autoroute_in_o=@vl_D535
begin
    /*-------------------------------- D535/2 ->A8/2  --------------------------------------------*/
    if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=2)
    begin
        if(@va_PR_in_o<=3400 and @va_PR_in_d<=171040)
        begin
            insert into #Portions values (0,@va_PR_in_o,3400,@va_autoroute_in_o,2)
            insert into #Portions values (1,171040,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
    /*-------------------------------- D535/2 ->A8/1  --------------------------------------------*/
    else if(@va_autoroute_in_d=@vl_A8 and @va_sens_in_d=1)
    begin
        if(@va_PR_in_o<=2200 and @va_PR_in_d>=172444 and @va_PR_in_d<=230000)
        begin
            insert into #Portions values (0,@va_PR_in_o,2200,@va_autoroute_in_o,2)
            insert into #Portions values (1,172444,@va_PR_in_d,@va_autoroute_in_d,@va_sens_in_d)
        end
    end
end


#if defined (DEV)
--select * from #Portions
#endif

/**************************************************************************************************/
/**************************************************************************************************/
/*                                   CALCUL DU TDP                                                */
/**************************************************************************************************/
/**************************************************************************************************/
/*On parcourt toutes les portions de la table #Portions*/
declare Pointeur_portion cursor
for select Autoroute_portion, Sens_portion, PR_portion_o, PR_portion_d, abs (PR_portion_d - PR_portion_o)
	from #Portions
	order by Numero_portion

open Pointeur_portion
fetch Pointeur_portion into @vl_autoroute_portion, @vl_sens_portion, @vl_PR_portion_o, @vl_PR_portion_d, @vl_lgr_portion

while ( @@sqlstatus = 0 )
begin

#if defined (DEV)
	print ""
	print "Portion sur autoroute %1! sens %2! PRs %3!-%4! longueur %5!", @vl_autoroute_portion, @vl_sens_portion, @vl_PR_portion_o, @vl_PR_portion_d, @vl_lgr_portion
#endif

	/***************** Calculer les valeurs de TDP et validite sur la portion *****************/

	if @vl_PR_portion_o <= @vl_PR_portion_d
		begin select @vl_PR_inf = @vl_PR_portion_o, @vl_PR_sup = @vl_PR_portion_d end
	else
		begin select @vl_PR_inf = @vl_PR_portion_d, @vl_PR_sup = @vl_PR_portion_o end

	/* Si la portion est contenue dans un segment alors calculer le TDP en proportion */
	select @vl_PR_debut = PR_debut, @vl_PR_fin = PR_fin, @t = tdp, @v = validite
		from BDD_XPL..TDP_FCD
		where horodate >= @vl_date  and  horodate < @vl_date_suiv
		  and autoroute = @vl_autoroute_portion
		  and sens = @vl_sens_portion
		  and ( ( PR_debut <= @vl_PR_inf  and  @vl_PR_sup <= PR_fin )  or
			( PR_fin <= @vl_PR_inf  and  @vl_PR_sup <= PR_debut ) )
	if @@rowcount = 1
	begin
		if ( @v < XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA ) begin select @va_validite_faible_out=1 end
		else if ( @v > 100 ) begin select @v = 100 end

		select @vl_distance = @vl_lgr_portion
		/* ajouter le TDP (arrondi a la seconde la plus proche) et la validite en proportion de la distance parcourue */
		select @vl_coef = round (convert (float, @vl_distance) / convert (float, abs (@vl_PR_fin - @vl_PR_debut)), 2)
		select @t_partiel = convert (int, @t * @vl_coef)
		select @va_TDP_FCD_out = @va_TDP_FCD_out + @t_partiel
		select @vl_somme_validite = @vl_somme_validite + convert (int, @v * @vl_coef)
		select @vl_compt_segments = @vl_compt_segments + (1 * @vl_coef)

#if defined (DEV)
	print "Portion incluse dans segment %1! - %2!, parcouru=%3!, TDP=%4! partiel=%5!, validite=%6!", @vl_PR_debut, @vl_PR_fin, @vl_distance, @t, @t_partiel, @v
#endif	
	end
	else
	begin
		select @vl_lgr_sections = 0

		/*** 1) Extraire les donnees TDP des segments entierement inclus dans la portion ***/
		delete #TDP_Portion
		insert #TDP_Portion ( PR_debut, PR_fin, tdp, validite )
			select PR_debut, PR_fin, tdp, validite
			  from BDD_XPL..TDP_FCD
			 where horodate >= @vl_date  and  horodate < @vl_date_suiv
			   and autoroute = @vl_autoroute_portion
			   and sens = @vl_sens_portion
			   and PR_debut	between @vl_PR_inf and @vl_PR_sup
			   and PR_fin	between @vl_PR_inf and @vl_PR_sup
		select @vl_nb_segments = @@rowcount

		if @vl_nb_segments > 0
		begin
			select	@vl_lgr_sections = sum (abs (PR_fin - PR_debut)),
				@t = sum (tdp),
				@v = sum (case when validite > 100 then 100 else validite end),
				@vl_nb_faibles = sum (case when validite < XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA then 1 else 0 end)
			from #TDP_Portion

			select @vl_compt_segments = @vl_compt_segments + @vl_nb_segments
			select @va_TDP_FCD_out = @va_TDP_FCD_out + @t
			select @vl_somme_validite = @vl_somme_validite + @v
			if @vl_nb_faibles > 0 begin select @va_validite_faible_out = 1 end
		end

#if defined (DEV)
	if @vl_nb_segments > 0
		print "%1! segments inclus dans la portion, %2! de validite faible; longueur=%3!, TDP=%4!, validite=%5!",
			@vl_nb_segments, @vl_nb_faibles, @vl_lgr_sections, @t, @v
	else
		print "Aucun segment inclus dans la portion"
#endif	

		/*** 2) Si le PR de debut de portion n'est pas une borne de segment, calculer le TDP et la validite sur son segment ***/
		select @vl_PR_debut = PR_debut, @vl_PR_fin = PR_fin, @t = tdp, @v = validite
			from BDD_XPL..TDP_FCD
			where horodate >= @vl_date  and  horodate < @vl_date_suiv
			  and autoroute = @vl_autoroute_portion
			  and sens = @vl_sens_portion
			  and ( ( PR_debut < @vl_PR_portion_o  and  @vl_PR_portion_o < PR_fin )  or
				( PR_fin < @vl_PR_portion_o  and  @vl_PR_portion_o < PR_debut ) )

		if @@rowcount = 1
		begin
			if ( (@vl_PR_portion_o < @vl_PR_portion_d)  and  (@vl_PR_debut < @vl_PR_fin) )  or
			   ( (@vl_PR_portion_o > @vl_PR_portion_d)  and  (@vl_PR_debut > @vl_PR_fin) )
				begin select @vl_distance = abs (@vl_PR_fin - @vl_PR_portion_o) end
			else
				begin select @vl_distance = abs (@vl_PR_portion_o - @vl_PR_debut) end

			select @vl_lgr_sections = @vl_lgr_sections + @vl_distance

			if ( @v < XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA ) begin select @va_validite_faible_out=1 end
			else if ( @v > 100 ) begin select @v = 100 end

			/* ajouter le TDP (arrondi a la seconde la plus proche) et la validite en proportion de la distance parcourue */
			select @vl_coef = round (convert (float, @vl_distance) / convert (float, abs (@vl_PR_fin - @vl_PR_debut)), 2)
			select @t_partiel = convert (int, @t * @vl_coef)
			select @va_TDP_FCD_out = @va_TDP_FCD_out + @t_partiel
			select @vl_somme_validite = @vl_somme_validite + convert (int, @v * @vl_coef)
			select @vl_compt_segments = @vl_compt_segments + (1 * @vl_coef)

#if defined (DEV)
	print "Segment debut portion : PRs %1! / %2!, parcouru=%3!, TDP=%4! partiel=%5!, validite=%6!", @vl_PR_debut, @vl_PR_fin, @vl_distance, @t, @t_partiel, @v
#endif	
		end


		/*** 3) Si le PR de fin de portion n'est pas une borne de segment, calculer le TDP et la validite sur son segment ***/
		select @vl_PR_debut = PR_debut, @vl_PR_fin = PR_fin, @t = tdp, @v = validite
			from BDD_XPL..TDP_FCD
			where horodate >= @vl_date  and  horodate < @vl_date_suiv
			  and autoroute = @vl_autoroute_portion
			  and sens = @vl_sens_portion
			  and ( ( PR_debut < @vl_PR_portion_d  and  @vl_PR_portion_d < PR_fin )  or
				( PR_fin < @vl_PR_portion_d  and  @vl_PR_portion_d < PR_debut ) )

		if @@rowcount = 1
		begin
			if ( (@vl_PR_portion_o < @vl_PR_portion_d)  and  (@vl_PR_debut < @vl_PR_fin) )  or
			   ( (@vl_PR_portion_o > @vl_PR_portion_d)  and  (@vl_PR_debut > @vl_PR_fin) )
				begin select @vl_distance = abs (@vl_PR_portion_d - @vl_PR_debut) end
			else
				begin select @vl_distance = abs (@vl_PR_fin - @vl_PR_portion_d) end

			select @vl_lgr_sections = @vl_lgr_sections + @vl_distance

			if ( @v < XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA ) begin select @va_validite_faible_out=1 end
			else if ( @v > 100 ) begin select @v = 100 end

			/* ajouter le TDP (arrondi a la seconde la plus proche) et la validite en proportion de la distance parcourue */
			select @vl_coef = round (convert (float, @vl_distance) / convert (float, abs (@vl_PR_fin - @vl_PR_debut)), 2)
			select @t_partiel = convert (int, @t * @vl_coef)
			select @va_TDP_FCD_out = @va_TDP_FCD_out + @t_partiel
			select @vl_somme_validite = @vl_somme_validite + convert (int, @v * @vl_coef)
			select @vl_compt_segments = @vl_compt_segments + (1 * @vl_coef)

#if defined (DEV)
	print "Segment fin portion : PRs %1! / %2!, parcouru=%3!, TDP=%4! partiel=%5!, validite=%6!", @vl_PR_debut, @vl_PR_fin, @vl_distance, @t, @t_partiel, @v
#endif	
		end

#if defined (DEV)
	print "Longueur sections = %1!", @vl_lgr_sections
#endif	

		if @vl_lgr_sections <> @vl_lgr_portion
		begin
			select @va_TDP_FCD_out = 0
			select @va_validite_out = 0
			return 0
		end
	end

	fetch Pointeur_portion into @vl_autoroute_portion, @vl_sens_portion, @vl_PR_portion_o, @vl_PR_portion_d, @vl_lgr_portion
end
				
close Pointeur_portion

/******************************************************************************/
/*                      Calcul de la validite                           */		
/******************************************************************************/

if @vl_compt_segments!=0		
begin
	select @va_validite_out=@vl_somme_validite / @vl_compt_segments
end									

/******************************************************************************/
/*              Application de l'ajustement manuel                            */
/******************************************************************************/

#if defined (DEV)
print "va_TDP_FCD_out = %1!, Ajustement=%2!", @va_TDP_FCD_out, @ajustement_manuel
#endif

/* ajout de l'ajustement manuel */
select @va_TDP_FCD_out = @va_TDP_FCD_out + @ajustement_manuel

return XDC_OK
go
