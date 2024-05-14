/*E*/
/*  Fichier : $Id: xzae88sp.prc,v 1.6 2015/09/16 14:39:09 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2015/09/16 14:39:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae88sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/08/95	: Creation
* C.T.	03/10/95	: Modif sens pour l'autoroute A57 (1.2)
* B.G.	02/11/95	: Retour a version 1.1 suite a gestion A57 (1.3)
* C.T.	06/11/95	: Modif format du libelle (1.4)
* C.T.	17/01/96	: correction longueur du libelle des travaux pour bretelle (1.5)
* NDE	20/08/15	: nouveaux intitules pour evt causes par travaux DEM1136 (1.6)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Construire l'intitule pour la synthese des travaux 
* 
* Sequence d'appel
* SP	XZAE88_Intitule_Synthes_Trv
* 
* Arguments en entree
* XDY_Booleen	va_Prevision_in
* XDY_Octet	va_Type_in
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PRTete_in
* XDC_PR	va_PRQueue_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* char(63)	va_Intitule_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE88' and type = 'P')
	drop procedure XZAE88
go

create procedure XZAE88
	@va_Prevision_in	bit,
	@va_Type_in 		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PRTete_in		T_PR		= null,
	@va_PRQueue_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Intitule_out	char(68)	= null output
as
	declare @vl_InitIntitule char(29), @vl_Localisation char(50),
		@vl_Autoroute char(4), @vl_PR1 float, @vl_PR2 float, 
		@vl_NomSens char(21)


	/*A debut de l'intitule */ 

	if @va_Prevision_in = XDC_VRAI
		select @vl_InitIntitule = case @va_Type_in
			when XZAEC_FMC_Travaux		then XZAEC_SYN_CHANTIER_PREVU
			when XZAEC_FMC_Deviation 	then XZAEC_SYN_DEVIATION_PREVUE
			when XZAEC_FMC_FermetureEch	then XZAEC_SYN_FERMETURE_ECH_PREVUE
			when XZAEC_FMC_AireFermee	then XZAEC_SYN_AIRE_FERMEE_PREVUE
			when XZAEC_FMC_BasculTravaux	then XZAEC_SYN_BASCUL_TRAVAUX_PREVU
		end
	else	
		select @vl_InitIntitule = case @va_Type_in
			when XZAEC_FMC_Travaux		then XZAEC_SYN_CHANTIER_EN_COURS
			when XZAEC_FMC_Deviation 	then XZAEC_SYN_DEVIATION_EN_COURS
			when XZAEC_FMC_FermetureEch	then XZAEC_SYN_FERMETURE_ECH_EN_COURS
			when XZAEC_FMC_AireFermee	then XZAEC_SYN_AIRE_FERMEE_EN_COURS
			when XZAEC_FMC_BasculTravaux	then XZAEC_SYN_BASCUL_TRAVAUX_EN_COURS
		end

	/*A dans le cas ou les travaux sont localises sur un point caracteristique : pas de PR de tete */
	/*A donc PRtete = PRQueue                                                                      */
	if  @va_PRTete_in is null
		select @va_PRTete_in = @va_PRQueue_in

	/*A construire la localisation */
	if @va_Autoroute_in is not null and @va_PRTete_in is not null and @va_PRQueue_in is not null
	begin
		/* recherche le nom de l'autoroute */
		select @vl_Autoroute = nom
		from CFG..RES_AUT
		where numero = @va_Autoroute_in

		/* libelle du sens : nord ou sud */
		if @va_Sens_in = XDC_SENS_SORTIE_NORD
			select @vl_NomSens = XDC_LIB_SENS_SORTIE_NORD
		else if @va_Sens_in = XDC_SENS_SORTIE_SUD
			select @vl_NomSens = XDC_LIB_SENS_SORTIE_SUD
		else if @va_Sens_in = XDC_SENS_ENTREE_NORD
			select @vl_NomSens = XDC_LIB_SENS_ENTREE_NORD
		else if @va_Sens_in = XDC_SENS_ENTREE_SUD
			select @vl_NomSens = XDC_LIB_SENS_ENTREE_SUD
		else if @va_Sens_in = XDC_SENS_NORD
			select @vl_NomSens = XDC_LIB_SENS_NORD
		else if @va_Sens_in = XDC_SENS_SUD
			select @vl_NomSens = XDC_LIB_SENS_SUD
		else	select @vl_NomSens = XDC_LIB_SENS_INCONNU

		/* zone de travaux */
		select @vl_PR1 = @va_PRTete_in
		select @vl_PR2 = @va_PRQueue_in
		select @vl_Localisation = @vl_Autoroute + ' pr(Q/T) ' + str(@vl_PR2/1000, 6, 3) + '/' + 
					str(@vl_PR1/1000, 6, 3) + ', ' + @vl_NomSens
	end
	else	select @vl_Localisation = XDC_CHAINE_VIDE

	/*A construction de l'intitule */
	select @va_Intitule_out = @vl_InitIntitule + @vl_Localisation

	return XDC_OK
go
