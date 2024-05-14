/*E*/
/*  Fichier : $Id: asql_t17_act_dep.trg,v 1.1 1995/01/25 10:26:26 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/25 10:26:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t17_act_dep.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table ACT_DEP
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  25/01/95        : Creation
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

drop trigger act_dep_iu_trg
go

create trigger act_dep_iu_trg
on ACT_DEP
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
		@vl_PR int,
		@vl_Sens tinyint,
		@vl_TypPtCar tinyint,
		@vl_PtCar tinyint,
		@vl_PosPtCar tinyint,
		@vl_Suppr bit

/*A Recuperation des valeurs */

	select @vl_action=actio,@vl_cleact=sit
	from inserted
	
	select  @vl_typesyn=XZAHC_Depanneur
	
	
	select  @vl_evt=evenement,
		@vl_cleevt=cle,
		@vl_horodeb=heure_succes,
		@vl_horofin=heure_fin
	from	EXP..ACT_GEN
	where	numero=@vl_action and
		sit=@vl_cleact
	
	select	@vl_autoroute=autoroute,
		@vl_PR=PR,
		@vl_Sens=sens,
		@vl_TypPtCar=point_caracteristique,
		@vl_PtCar=numero_point_caracteristique,
		@vl_PosPtCar=position_sur_aire_ou_echangeur
	from	EXP..FMC_HIS
	where	numero=@vl_evt and
		cle=@vl_cleevt
	having	numero=@vl_evt and
		cle=@vl_cleevt and
		horodate_validation=max(horodate_validation)
		
	select  @vl_Suppr=XDC_VRAI
	
	if @vl_horodeb<>null and @vl_horofin=null
	select @vl_Suppr=XDC_FAUX
	
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
