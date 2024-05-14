/*E*/
/*  Fichier : @(#)xzao429sp.prc	1.1        Release : 1.1        Date : 03/15/05
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao429sp.prc
------------------------------------------------------
*
*/
/*------------------------------------------------------
* HISTORIQUE :
*
* CHI	04/04/05: Creation
* PNI	16/08/05: Tri par ordre alpha DEM495 v1.2
* ---------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste les libelles des bouchons pre-definis
*
------------------------------------------------------
* SEQUENCE D'APPEL :

* SP	XZAO429_Liste_Bouchons_predef
* 
* Arguments en entree
* XDY_Mot	va_Sit_in
* XDY_Mot       va_Libelle_in
*
* 
* Arguments en sortie
* si un seul arg en entree: renvoie la liste des libelles des bouchons en fonction du site passe en arg entree
* sinon                   : renvoie la configuration du bouchon dont le libelle a ete passe en arg entree
*
* Service rendu
*
* Liste les libelles des bouchons pre-definis
* si va_Libelle_in est null: renvoie la liste des libelles des bouchons en fonction du site passe en arg entree
* si va_Libelle_in est non null: renvoie la configuration du bouchon dont le libelle a ete passe en arg entree:
* 		veracite, 
*		type_point_carcat, 
*		point_carac, 
*		autoroute, 
*		PR, 
*		sens, 
*		VR, 
*		VM2, 
*		VM1, 
*		VL, 
*		BAU, 
*		longueur, 
*		niveau
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

if exists (select * from sysobjects where name = 'XZAO429' and type = 'P')
	drop procedure XZAO429
go

create procedure XZAO429
   @va_Sit_in			int,
   @va_Libelle_in               char(50)=null

as
	declare @vl_Site T_SITE

	/* Transfo en parametres nulls bug applix */
	if @va_Sit_in = 0 select va_Sit_in = null

	/*analyse des parametres d'entree */
	if @va_Sit_in = null
		return XDC_ARG_INV

	/*si appel pour retourner la liste des libelles des bouchons*/
	if @va_Libelle_in = null
	begin
  		/* Verification du site */
		select  @va_Sit_in = numero
			from CFG..RES_DIS
			where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
			select ltrim(libelle) from CFG..DEF_BOU order by libelle
		end
		else
		begin
			select ltrim(libelle) from CFG..DEF_BOU where (sit = @va_Sit_in or sit =0) order by libelle
		end
	end
	else
	/*si appel pour retourner les details du bouchon choisi*/
	begin
  		select  confirme_ou_signale,
			point_caracteristique,
			numero_point_caracteristique,
			autoroute,
			PR,
			sens,
			VR,
			VM2,
			VM1,
			VL,
			BAU,
			longueur,
			type

		from CFG..DEF_BOU 
		where libelle like @va_Libelle_in
	end
	return XDC_OK

go
