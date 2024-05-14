/*X  Fichier : $Id: xzae101sp.prc,v 1.9 2007/03/26 15:06:48 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2007/03/26 15:06:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae101sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	02/09/96	: Creation
* JMG	14/10/96	: plusde filtrage sur les PR (1.2)
* C.T.	04/12/96	: ajout en sortie dates de debut,
*                         fin,operation et chgt le tri (680) (1.3-1.4)
* C.T.	09/12/96	: Correction ordre de tri (680) (1.5-1.6)
* PN	10/03/99	: Modif intergest 1.8
* Hilmarcher 23/02/05   : modif ecriture TAB + # car pb a la compil DEM423 v1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des fmc locales a un site
*
* Sequence d'appel
* SP    XZAE101_Liste_FMC_Locales
*
* Arguments en entree
* XDY_NomSite	va_District_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Entier	numero
* XDY_Octet	cle
* XDY_Mot	type
* XDY_Horodate	debut
* char()	nom_autoroute-PR-sens
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* a executer au CI uniquement et pour les PC simplifies
*
* Fonction
---------------------------------------------------*/

use PRC
go

#ifdef CI
create procedure XZAE;101
	@va_District_in	char(2) = null
as
	declare @vl_Site T_SITE,
		@vl_datex tinyint

	if @va_District_in is null
		return XDC_NOK

	/*A rechercher le code du district */
	select @vl_Site = numero from CFG..RES_DIS
	where code = @va_District_in

	if @@rowcount = 0
		return XDC_NOK

	/*A rechercher la zone du district */
	select
		autoroute,
		PR_debut,
		PR_fin
	into #ZONE
	from CFG..RES_POR
	where district = @vl_Site or 
		district_recouvrement = @vl_Site

	/*A rechercher les fmc en cours avec a la derniere validation */
	select  distinct
		EXP..FMC_GEN.numero,
		EXP..FMC_GEN.cle,
		type,
		date = isnull(debut,debut_prevu),
		autoroute,
		PR,
		sens,
		debut,
		fin,
		evt_en_cours = XDC_VRAI,
		datex
	into #LISTE
	from EXP..FMC_GEN,EXP..FMC_HIS, EXP..FMC_TRT
	where 	cloture is null and
		type != XZAEC_FMC_PosteOperateur and
		type != XZAEC_FMC_TeteBouchon and
		EXP..FMC_TRT.evenement = EXP..FMC_GEN.numero and
		EXP..FMC_TRT.cle = EXP..FMC_GEN.cle and
		EXP..FMC_TRT.sit = @vl_Site and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle
	group by EXP..FMC_GEN.numero, EXP..FMC_GEN.cle,
		EXP..FMC_TRT.evenement, EXP..FMC_TRT.cle,
		EXP..FMC_HIS.numero, EXP..FMC_HIS.cle
	having 	cloture is null and
		type != XZAEC_FMC_PosteOperateur and
		type != XZAEC_FMC_TeteBouchon and
		EXP..FMC_TRT.evenement = EXP..FMC_GEN.numero and
		EXP..FMC_TRT.cle = EXP..FMC_GEN.cle and
		EXP..FMC_TRT.sit = @vl_Site and
		EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
		EXP..FMC_HIS.cle = EXP..FMC_GEN.cle and
		horodate_validation = max(horodate_validation)

	/*A marquer les evenements en prevision */
	update #LISTE set evt_en_cours = XDC_FAUX 
	where debut is null
	
	/*A construire les fmc locales au site */
	create table #LISTE_ORDONNEE (numero int, cle tinyint, type smallint, date datetime null, localisation varchar(14) null, 
					debut datetime null, fin datetime null, operation bit, datex tinyint)
	insert #LISTE_ORDONNEE
	select #LISTE.numero, #LISTE.cle,
		type,
		date,
		convert(varchar(4),CFG..RES_AUT.nom) + "-" +
			convert(char(1),sens) + "-" +
			str(convert(float,#LISTE.PR)/1000,7,3),
		debut,
		fin,
		XDC_FAUX,
		datex
	from #LISTE, CFG..RES_AUT, CFG..TYP_FMC
	where	CFG..RES_AUT.numero = #LISTE.autoroute and
		CFG..TYP_FMC.numero = type and evt_en_cours = XDC_VRAI and (operation=XDC_FAUX or datex=XDC_VRAI)
	order by evt_en_cours desc,  datex asc, date desc

	insert #LISTE_ORDONNEE
	select #LISTE.numero, #LISTE.cle,
		type,
		date,
		convert(varchar(4),CFG..RES_AUT.nom) + "-" +
			convert(char(1),sens) + "-" +
			str(convert(float,#LISTE.PR)/1000,7,3),
		debut,
		fin,
		operation,
		datex
	from #LISTE, CFG..RES_AUT, CFG..TYP_FMC
	where	CFG..RES_AUT.numero = #LISTE.autoroute and
		CFG..TYP_FMC.numero = type and evt_en_cours = XDC_VRAI and operation=XDC_VRAI and datex=XDC_FAUX
	order by date asc


	insert #LISTE_ORDONNEE
	select #LISTE.numero, #LISTE.cle,
		type,
		date,
		convert(varchar(4),CFG..RES_AUT.nom) + "-" +
			convert(char(1),sens) + "-" +
			str(convert(float,#LISTE.PR)/1000,7,3),
		debut,
		fin,
		operation,
		datex
	from #LISTE, CFG..RES_AUT, CFG..TYP_FMC
	where	CFG..RES_AUT.numero = #LISTE.autoroute and
		CFG..TYP_FMC.numero = type and evt_en_cours = XDC_FAUX
	order by  date asc

	/*A Retourner les fmc locales au site */
	select * from #LISTE_ORDONNEE

/*	from #LISTE, #ZONE, CFG..RES_AUT
	where 	#LISTE.autoroute = #ZONE.autoroute and
		PR >= PR_debut and PR < PR_fin and
		CFG..RES_AUT.numero = #LISTE.autoroute*/

	return XDC_OK
go
#endif
