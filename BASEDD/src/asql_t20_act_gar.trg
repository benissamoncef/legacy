/*E*/
/*  Fichier : $Id: asql_t20_act_gar.trg,v 1.3 1996/04/19 10:31:02 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/04/19 10:31:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t20_act_gar.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table ACT_INT
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  27/03/95        : Creation
* C.T.  19/04/96        : test des demi barrieres sur 
*                         PRsens = 0 au lieu de null (1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* 
* anime l'ihm
*
* Code retour
* 
* n≈ant
*
* Conditions d'utilisation
* base EXP 
---------------------------------------------------*/

use EXP
go

drop trigger act_gar_iu_trg
go

create trigger act_gar_iu_trg
on ACT_GAR
for insert,update
as

/*A D≈claration des variables locales */

	declare @vl_action int,
		@vl_cleact tinyint,
		@vl_typesyn tinyint,
		@vl_evt int,
		@vl_cleevt tinyint,
		@vl_horodeb datetime,
		@vl_horofin datetime,
		@vl_autoroute tinyint,
		@vl_PR T_PR,
		@vl_PRsens1 T_PR,
		@vl_PRsens2 T_PR,
		@vl_Sens tinyint,
		@vl_TypPtCar tinyint,
		@vl_PtCar tinyint,
		@vl_PosPtCar tinyint,
		@vl_NomGare T_NOM,
		@vl_EntSudPeage bit,
		@vl_Suppr bit

	/*A Recuperation des valeurs */
	select @vl_action=actio,@vl_cleact=sit,@vl_NomGare=nom_gare
	from inserted
	
	/* ne rien faire si la gare n'est pas renseignee */
	if @vl_NomGare is null
		return 

	select  @vl_typesyn=XZAHC_Gare, @vl_Suppr=XDC_VRAI
	
	select  @vl_evt=evenement,
		@vl_cleevt=cle,
		@vl_horodeb=heure_succes,
		@vl_horofin=heure_fin
	from	EXP..ACT_GEN
	where	numero=@vl_action and
		sit=@vl_cleact
	
	/*A rechercher la localisation de la gare de peage */
	select	@vl_autoroute=CFG..RES_POR.autoroute,
		@vl_PRsens1=CFG..RES_PEA.PRsens1,
		@vl_PRsens2=CFG..RES_PEA.PRsens2
	from	CFG..RES_PEA, CFG..RES_POR
	where	CFG..RES_PEA.nom=@vl_NomGare and 
		CFG..RES_POR.numero = CFG..RES_PEA.portion
		
	if @@rowcount = 1
	begin
		/* dans une gare de peage sur sectioncourante */

		select  @vl_Sens = XDC_SENS_INCONNU,
			@vl_TypPtCar=null,
			@vl_PtCar=null,
			@vl_PosPtCar=null

		if @vl_PRsens2 = 0
			select @vl_PR = @vl_PRsens1
		else if @vl_PRsens1 = 0
			select @vl_PR = @vl_PRsens2
		else if @vl_PRsens1 < @vl_PRsens2
			select @vl_PR = @vl_PRsens1 + (@vl_PRsens2 - @vl_PRsens1) / 2
		else select @vl_PR = @vl_PRsens2 + (@vl_PRsens1 - @vl_PRsens2) / 2
	end
	else
	begin
		/* sur un echangeur */

		select  @vl_autoroute = CFG..RES_POR.autoroute,
			@vl_TypPtCar = XDC_POINT_CARACT_ECHANGEUR,
			@vl_EntSudPeage = CFG..RES_ECH.peage_entree_sens1,
			@vl_PtCar = CFG..RES_ECH.numero,
			@vl_PosPtCar = XDC_POINT_CARACT_POS_DEF,
			@vl_Sens = XDC_SENS_ENTREE_SUD
		from	CFG..RES_ECH, CFG..RES_POR
		where	CFG..RES_ECH.nom=@vl_NomGare and 
			CFG..RES_POR.numero = CFG..RES_ECH.portion

		if @@rowcount = 0
			return 

		/* si pas de peage sur l'entree sud alors positionner sur entree nord */
		if @vl_EntSudPeage = XDC_NON
			select @vl_Sens = XDC_SENS_ENTREE_NORD
	end
	
	if @vl_horodeb<>null and @vl_horofin=null
		select @vl_Suppr=XDC_FAUX
	
	/*A Ajout de l'objet mobile */
	exec PRC..XZAH;12	@vl_evt,
				@vl_cleevt,
				@vl_action,
				@vl_cleact,
				@vl_typesyn,
				@vl_autoroute,
				@vl_PR,
				@vl_Sens,
				XZAHC_VoieInconnue,
				@vl_TypPtCar,
				@vl_PtCar,
				@vl_PosPtCar,
				@vl_Suppr
go
