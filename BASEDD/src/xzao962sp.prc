/*------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao962sp.prc
------------------------------------------------------
* HISTORIQUE :
*		
* GRO  19/08/2020 : Creation TDP_FCD DEM-SAE152 1.0
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
*
* JPL	23/01/2024 : Deport du calcul pur dans XZAO963 qui est appelee; application des arrondis selon seuils
------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calculer le temps de parcours FCD (Floating Car Data) entre un PR d'origine et de destination
* 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO962_Calcul_TDP_FCD
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
* Lit en base de donnees la configuration des valeurs de seuils et d'arrondis.
* Execute XZAO963 pour obtenir les Temps de parcours et validites,
* puis applique les arrondis selon les seuils configures et retourne les resultats.
---------------------------------------------------------*/
use PRC
go
		
if exists (select * from sysobjects where name = 'XZAO962' and type = 'P')
	drop procedure XZAO962
go				

create procedure XZAO962
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
   @va_validite_faible_out bit output
as 
		
declare 
	/* Valeur de seuil pour les arrondis */
	@vl_seuil_TP_court smallint,
	@vl_seuil_TP_moyen smallint,

	/* Valeurs d'arrondi */
	@vl_arrondi_inf smallint,
	@vl_arrondi_moy smallint,
	@vl_arrondi_sup smallint,
	@vl_arrondi smallint,

	@vl_Status int


select	@va_TDP_FCD_out=0,
	@va_validite_out=0,
	@va_validite_faible_out=0

if(@va_PR_in_o<0 or @va_PR_in_d<0) begin return XDC_OK end


select @vl_seuil_TP_court = seuil_TP_court from CFG..TDP_ARR_FCD
select @vl_seuil_TP_moyen = seuil_TP_moyen from CFG..TDP_ARR_FCD
select @vl_arrondi_inf   = arrondi_inf from CFG..TDP_ARR_FCD
select @vl_arrondi_moy   = arrondi_moy from CFG..TDP_ARR_FCD
select @vl_arrondi_sup   = arrondi_sup from CFG..TDP_ARR_FCD

/* Conversion vers seconde pour arrondi */
select @vl_seuil_TP_court = @vl_seuil_TP_court * 60
select @vl_seuil_TP_moyen = @vl_seuil_TP_moyen * 60
select @vl_arrondi_inf   = @vl_arrondi_inf * 60
select @vl_arrondi_moy   = @vl_arrondi_moy * 60
select @vl_arrondi_sup   = @vl_arrondi_sup * 60

/* Calculer les temps de parcours bruts */
exec @vl_Status = PRC..XZAO963	@va_autoroute_in_o, @va_PR_in_o, @va_sens_in_o,
				@va_autoroute_in_d, @va_PR_in_d, @va_sens_in_d,
				@va_numero_in, @va_cle_in,
				@va_TDP_FCD_out output,
				@va_validite_out output,
				@va_validite_faible_out output
if @vl_Status != XDC_OK
	return @vl_Status


/* Application des arrondis en fonction des seuils */
if (@va_TDP_FCD_out < @vl_seuil_TP_court )
		select @vl_arrondi = @vl_arrondi_inf
else if  (@va_TDP_FCD_out > @vl_seuil_TP_court AND @va_TDP_FCD_out < @vl_seuil_TP_moyen)
		select @vl_arrondi = @vl_arrondi_moy
else /*@va_TDP_FCD_out > @vl_seuil_TP_moyen*/
		select @vl_arrondi = @vl_arrondi_sup
	
/* Application de l'arrondi, sauf dans le cas ou on est pile sur une valeur modulo de l'arrondi */
if ((@va_TDP_FCD_out % @vl_arrondi) != 0)
	select @va_TDP_FCD_out = ((@va_TDP_FCD_out / @vl_arrondi)*@vl_arrondi) + @vl_arrondi /* Arondi à  @vl_arrondi min supérieur */

return XDC_OK
go
