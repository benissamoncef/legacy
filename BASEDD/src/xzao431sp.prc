/*E*/
/*  Fichier : @(#)xzao431sp.prc	1.1        Release : 1.1        Date : 03/15/05
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao431sp.prc
------------------------------------------------------
*
*/
/*------------------------------------------------------
* HISTORIQUE :
*
* CHI	04/04/05: Creation
* ---------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
* Configuration de la table DEF_BOU de gestion des bouchons predefinis
*
------------------------------------------------------
* SEQUENCE D'APPEL :

* SP	XZAO431_...
* 
* Arguments en entree: (16 args)
* char(50) va_Libelle_in
* int va_veracite_in
* int va_aut_in
* int va_PR_in
* int va_sens_in
* int va_point_car_in
* int va_num_point_car_in
* int va_VR_in
* int va_VM2_in
* int va_VM1_in
* int va_VL_in
* int va_BAU_in
* int va_type_in
* int va_longueur_in
* int va_sit_in
* int va_appelant_in
*
* Arguments en sortie: aucun
*
* Service rendu
* si va_appelant_in=0: suppression du champ dans la table DEF_BOU
* si va_appelant_in=1: ajout du champ dans la table DEF_BOU
* si va_appelant_in=2: modif du champ dans la table DEF_BOU
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
-------------------------------------------------------*/
use PRC
go

if exists (select * from sysobjects where name = 'XZAO431' and type = 'P')
	drop procedure XZAO431
go

create procedure XZAO431
   @va_Libelle_in               char(50)=null,
   @va_veracite_in		int,
   @va_aut_in			int,
   @va_PR_in			int,
   @va_sens_in			int,
   @va_point_car_in		int,
   @va_num_point_car_in		int,
   @va_VR_in			int,
   @va_VM2_in			int,
   @va_VM1_in			int,
   @va_VL_in			int,
   @va_BAU_in			int,
   @va_type_in			int,
   @va_longueur_in		int,
   @va_sit_in			int,
   @va_appelant_in		int

as
	declare @vl_numero int

	/* Transfo en parametres nulls bug applix */
	if @va_sit_in = 0 select va_sit_in = null
	if @va_veracite_in= 0 select va_veracite_in = null
	if @va_aut_in= 0 select va_aut_in = null
	if @va_PR_in= 0 select va_PR_in = null
	if @va_sens_in= 0 select va_sens_in = null
	if @va_point_car_in= 0 select va_point_car_in = null
	if @va_num_point_car_in= 0 select va_num_point_car_in=null
	if @va_VR_in= 0 select va_VR_in=null
	if @va_VM2_in=0 select va_VM2_in=null
	if @va_VM1_in=0 select va_VM1_in= null
	if @va_VL_in= 0 select va_VL_in= null
	if @va_BAU_in= 0 select va_BAU_in= null
	if @va_type_in= 0 select va_type_in= null
	if @va_longueur_in = 0 select va_longueur_in= null
	if @va_sit_in= 0 select va_sit_in=null
	if @va_appelant_in=0 select va_appelant_in=null

	/*analyse des parametres d'entree */
	if (@va_sens_in = null or @va_VR_in= null or @va_VM2_in= null or @va_VM1_in= null or @va_VL_in=null
		or @va_BAU_in=null or @va_type_in=null or @va_Libelle_in=null)
		return XDC_ARG_INV

	/*suppression*/
	if @va_appelant_in = 1
	begin
		delete from CFG..DEF_BOU where (libelle like  @va_Libelle_in and autoroute= @va_aut_in and sens = @va_sens_in and PR = @va_PR_in)
	end

	/*ajout*/
	if @va_appelant_in = 2
	begin
		select @vl_numero = max(numero) from CFG..DEF_BOU
		insert CFG..DEF_BOU values (@vl_numero+1,@va_Libelle_in,@va_veracite_in,@va_aut_in,@va_PR_in,@va_sens_in,
						 @va_point_car_in,@va_num_point_car_in,@va_VR_in,@va_VM2_in,@va_VM1_in,@va_VL_in,@va_BAU_in,
						 @va_type_in,@va_longueur_in,@va_sit_in)
	end

	/*modif*/
	if @va_appelant_in = 3
	begin
		update CFG..DEF_BOU set 
				libelle = @va_Libelle_in,
				confirme_ou_signale = @va_veracite_in,
				autoroute = @va_aut_in,
				PR = @va_PR_in,
				sens = @va_sens_in,
				point_caracteristique = @va_point_car_in,
				numero_point_caracteristique = @va_num_point_car_in,
				VR = @va_VR_in,
				VM2 = @va_VM2_in,
				VM1 = @va_VM1_in,
				VL = @va_VL_in,
				BAU = @va_BAU_in,
				type = @va_type_in,
				longueur = @va_longueur_in,
				sit = @va_sit_in

				where (libelle like  @va_Libelle_in and autoroute= @va_aut_in and sens = @va_sens_in and PR = @va_PR_in)
	end
	
	
	return XDC_OK

go
