/*E*/
/* Fichier : $Id: xzae89sp.prc,v 1.1 2011/10/05 15:44:57 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 2011/10/05 15:44:57 $
--------------------------------------------------------------------------------
* ESCOTA * PROJET PASTRE
--------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
--------------------------------------------------------------------------------
* MODULE XZAE * FICHIER xzae89sp.prc
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Retourne les elements de localisation d'une entite quelconque
*  sous forme lisible (libelles) a partir de leurs identifiants numeriques.
*
*  Retourne egalement un libelle descriptif de la localisation basee sur
*  le Pr et le sens de circulation, dans le format utilise par les
*  syntheses d'evenements.
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		21/09/11 : Creation  (DEM 994)  1.1
----------------------------------------------------------------------------- */

/*X*/
/*------------------------------------------------------------------------------
*
* Service rendu
*
* Retourne les elements textuels decrivant une localisation
* quelconque, y compris sur point caracteristique,
* a partir de valeurs numeriques fournies.
*
* Sequence d'appel
* SP    XZAE89_Localisation
*
* Arguments en entree
*    XDY_Mot		va_Type_Fmc_in
*    XDY_Autoroute	va_Autoroute_in
*    XDC_PR		va_PR_in
*    XDC_Sens		va_Sens_in
*    XDC_Octet		va_Type_Point_Car_in
*    XDC_Octet		va_Num_Point_Car_in
*
* Arguments en sortie
*    char(4)		va_Nom_Autoroute_out
*    char(7)		va_PR_out
*    XDY_Nom		va_Nom_Sens_out
*    XDY_Nom		va_Nom_Type_Pt_Car_out
*    char(30)		va_Nom_Pt_Car_out
*    char(60)		va_Local_Pr_Sens_out
*
* Code de retour
*    XDC_OK
*    XDC_NOK	autoroute inconnue ou sens de circulation inconnu.
*
* Conditions d'utilisation
*   Cette procedure est une sous-procedure utilisablee par toute autre
*   procedure. Elle est interne a la base de donnees.
*
* Fonction
*   Si l'autoroute est fournie, construit un libelle de localisation
*   de format	%4s pr %3s.%3s, %s     (Autoroute pr Kilometres, Sens)
*
*   Si un point caracteristique est indique, retourne les noms
*   du point caracteristique et de son type.
*
------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE89' and type = 'P')
	drop procedure XZAE89
go


create procedure XZAE89
	@va_Type_Fmc_in		smallint	= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Type_Point_Car_in	tinyint		= null,
	@va_Num_Point_Car_in	tinyint		= null,
	@va_Nom_Autoroute_out	char(4)		= null	output,
	@va_PR_out		char(7)		= null	output,
	@va_Nom_Sens_out	T_NOM		= null	output,
	@va_Nom_Type_Pt_Car_out	T_NOM		= null	output,
	@va_Nom_Pt_Car_out	char(30)	= null	output,
	@va_Local_Pr_Sens_out	char(60)	= null	output
as
	declare	@vl_PR_float	float


	/*A Construire le libelle de localisation par PR et sens de circulation */
	if @va_Autoroute_in != null
	begin
		/* rechercher le nom de l'autoroute */
		select @va_Nom_Autoroute_out = nom
		from CFG..RES_AUT
		where numero = @va_Autoroute_in

		if @@rowcount != 1
			return XDC_NOK


		/* libelle du PR en km */
		if @va_PR_in != null
		begin
			select @vl_PR_float = @va_PR_in
			select @va_PR_out = str (@vl_PR_float / 1000, 7, 3)
		end


		/* libelle du sens incluant le cas "2 sens" */
		if @va_Sens_in = XDC_SENS_NORD
			select @va_Nom_Sens_out = XDC_LIB_SENS_NORD
		else if @va_Sens_in = XDC_SENS_SUD
			select @va_Nom_Sens_out = XDC_LIB_SENS_SUD
		else if @va_Sens_in = XDC_SENS_SORTIE_NORD
			select @va_Nom_Sens_out = XDC_LIB_SENS_SORTIE_NORD
		else if @va_Sens_in = XDC_SENS_SORTIE_SUD
			select @va_Nom_Sens_out = XDC_LIB_SENS_SORTIE_SUD
		else if @va_Sens_in = XDC_SENS_ENTREE_NORD
			select @va_Nom_Sens_out = XDC_LIB_SENS_ENTREE_NORD
		else if @va_Sens_in = XDC_SENS_ENTREE_SUD
			select @va_Nom_Sens_out = XDC_LIB_SENS_ENTREE_SUD
		else if @va_Sens_in = XDC_SENS_INCONNU
		begin
			if (@va_Type_Fmc_in = XZAEC_FMC_Meteo  or
			    @va_Type_Fmc_in = XZAEC_FMC_Inondation  or
			    @va_Type_Fmc_in = XZAEC_FMC_TraitChaussees)
				select @va_Nom_Sens_out = XDC_LIB_2_SENS
			else
				select @va_Nom_Sens_out = XDC_LIB_SENS_NON_PREC
		end
		else
			return XDC_NOK


		/*B libelle "standard" base sur le PR et le sens de circulation */
		select @va_Local_Pr_Sens_out = @va_Nom_Autoroute_out + ' pr ' +
		                                  @va_PR_out + ', ' + @va_Nom_Sens_out
	end


	/*A Determiner les elements de localisation sur point caracteristique */
	if @va_Type_Point_Car_in != null  and  @va_Num_Point_Car_in != null
	begin
		if @va_Type_Point_Car_in = XDC_POINT_CARACT_ECHANGEUR
		begin
			select @va_Nom_Type_Pt_Car_out = XDC_NOM_TYP_PTCAR_ECH

			select @va_Nom_Pt_Car_out = nom
			from CFG..RES_ECH
			where numero = @va_Num_Point_Car_in
		end
		else if @va_Type_Point_Car_in = XDC_POINT_CARACT_PEAGE
		begin
			select @va_Nom_Type_Pt_Car_out = XDC_NOM_TYP_PTCAR_PEA

			select @va_Nom_Pt_Car_out = nom
			from CFG..RES_PEA
			where numero = @va_Num_Point_Car_in
		end
		else if @va_Type_Point_Car_in = XDC_POINT_CARACT_AIRE
		begin
			select @va_Nom_Type_Pt_Car_out = XDC_NOM_TYP_PTCAR_AIR

			select @va_Nom_Pt_Car_out = nom
			from CFG..RES_AIR
			where numero = @va_Num_Point_Car_in
		end
		else if @va_Type_Point_Car_in = XDC_POINT_CARACT_LIT_ARRET
		begin
			select @va_Nom_Type_Pt_Car_out = XDC_NOM_TYP_PTCAR_LIT

			select @va_Nom_Pt_Car_out = nom
			from CFG..EQT_GEN
			where type = XDC_CAPT_LIT_ARRET
			  and numero = @va_Num_Point_Car_in
		end
		else
			return XDC_NOK

		if @@rowcount != 1
			return XDC_NOK
	end

	return XDC_OK
go
