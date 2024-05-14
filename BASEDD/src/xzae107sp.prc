/*X  Fichier : $Id: xzae107sp.prc,v 1.1 1996/09/20 12:04:28 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/09/20 12:04:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae107sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/96	: Creation
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des depanneurs modifiable par un pc simplifie
*
* Sequence d'appel
* SP    XZAE107_Liste_Depanneurs_FMC
*
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
*
* Arguments en sortie
*
* Code retour
* XDC_ARG_INV
* XDC_OK
* XZAEC_FMC_NON_FIN : fmc non finie 
* XZAEC_FMC_A_TRT : fmc a traietr dans un district
* <0 erreur sybase
*
* Conditions d'utilisation
* au CI uniquement
*
* Fonction
---------------------------------------------------*/

use PRC
go

#ifdef CI

create procedure XZAE;107
	@va_NumEvt_in	int = null,
	@va_CleEvt_in	tinyint = null
as
	declare  @vl_Fin datetime, @vl_SiteMaitre T_SITE

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	/*A controler les paramatres d'entree */
	if @va_NumEvt_in is null or @va_CleEvt_in is null
		return XDC_ARG_INV

	/* Rechercher la fin reelle de la fmc */
	select @vl_Fin = fin
	from EXP..FMC_GEN
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in

	/*A Tester que la fmc est finie et qu'elle est � traiter uniquement au CI */
	if  @vl_Fin is null
		return XZAEC_FMC_NON_FIN
	if exists (	select * from EXP..FMC_GEN
			where	numero =  @va_NumEvt_in and
				cle = @va_CleEvt_in and
				sit != @vl_SiteMaitre)
		return XZAEC_FMC_A_TRT
	

	/*A liste des actions depanneurs liees a cette fmc et saisies au CI */
	select
		numero,
		heure_lancement,
		nom,
		EXP..ACT_GEN.sit,
		resultat,
		renfort,
		patrouille_presente,
		garagiste_doit_contacter_pc,
		annulation_intervention,
		code_type_astreinte,
		interlocuteur
	from EXP..ACT_GEN, EXP..ACT_DEP
	where evenement = @va_NumEvt_in and cle = @va_CleEvt_in and type = XDC_ACT_DEPANNEUR and 
		EXP..ACT_GEN.sit = @vl_SiteMaitre and actio = numero and EXP..ACT_DEP.sit = EXP..ACT_GEN.sit
	order by heure_lancement desc

	return XDC_OK
go
#endif
