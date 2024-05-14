/*E*/
/*  Fichier : $Id: xzac87sp.prc,v 1.1 2018/04/19 12:13:27 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2018/04/19 12:13:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac87sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18	creation controleurs de feux CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage CFE.
* 
* Sequence d'appel
* SP	XZAC87_Lire_Commande_CFE
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_Octet	va_PlanDeFeux_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si les informations n'existent pas dans
* EXP tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche d'infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans les tables ACTION et E_CFE
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC87
go

create procedure XZAC87
	@va_NumAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_PlanDeFeux_out		tinyint		= null		output,
	@va_LibellePlanDeFeux_out		char(150)		= null		output,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureEchec_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output
as
	declare	@vl_NbEnrg		int,
		@vl_Status		int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST

	select 
		@va_NumEqt_out			= HIS..ACT_GEN.equipement,
		@va_PlanDeFeux_out		= HIS..ACT_CFE.plan_de_feux,
		@va_LibellePlanDeFeux_out		= HIS..ACT_CFE.libelle_plan,
		@va_HeureLancement_out		= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out		= HIS..ACT_GEN.heure_fin
	from HIS..ACT_CFE, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_CFE and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_CFE.actio = @va_NumAction_in and 
		HIS..ACT_CFE.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select 
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_PlanDeFeux_out		= EXP..ACT_CFE.plan_de_feux,
		@va_LibellePlanDeFeux_out		= EXP..ACT_CFE.libelle_plan,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin
	from EXP..ACT_CFE, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_CFE and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_CFE.actio = @va_NumAction_in and 
		EXP..ACT_CFE.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC87
					@va_NumAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_PlanDeFeux_out		output,
					@va_LibellePlanDeFeux_out		output,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
