/*E*/
/*  Fichier : $Id: xzac892sp.prc,v 1.2 2021/06/10 12:33:15 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2021/06/10 12:33:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac892sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture des états terrain PAU SONO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	07/04/21	Création SONO DEM-SAE244 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
*
* Sequence d'appel
* SP	XZAC892_Lire_Commande_PAU_SONO				
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* XDY_Texte	va_Message_out
* Int		va_Autoroute_out
* Int		va_Sens_out
* Int		va_PR_debut_out
* Int		va_PR_fin_out
* Int		va_PAUErreur_out
* Int		va_PAUPilotes_out
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
* Select dans les tables ACTION et E_PAU
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC892' and type = 'P')
	drop procedure XZAC892
go


create procedure XZAC892 
	@va_NumAction_in int= null,
	@va_Site_in T_SITE= null,
	@va_NumEqt_out T_EQUIPEMENT= null output,
	@va_HeureLancement_out datetime= null output,
	@va_HeureLancementFin_out datetime= null output,
	@va_HeureSucces_out datetime= null output,
	@va_HeureEchec_out datetime= null output,
	@va_HeureFin_out datetime= null output,
	@va_Message_out char(31)= null output,
	@va_Autoroute_out int= null output,
	@va_Sens_out tinyint= null output,
	@va_PR_debut_out int= null output,
	@va_PR_fin_out int= null output,
	@va_PAUErreur_out int= null output, 
	@va_PAUPilotes_out int= null output 							
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
		@va_HeureLancement_out		= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out		= HIS..ACT_GEN.heure_fin,
		@va_Message_out			= HIS..ACT_PAU.message,
		@va_Autoroute_out		= HIS..ACT_PAU.autoroute,
		@va_Sens_out			= HIS..ACT_PAU.sens,
		@va_PR_debut_out		= HIS..ACT_PAU.PR_debut,
		@va_PR_fin_out			= HIS..ACT_PAU.PR_fin,
		@va_PAUErreur_out		= HIS..ACT_PAU.PAUErreur,
		@va_PAUPilotes_out		= HIS..ACT_PAU.PAUPilotes
	from HIS..ACT_PAU, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_PAU and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_PAU.actio = @va_NumAction_in and 
		HIS..ACT_PAU.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select 
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin,
		@va_Message_out			= EXP..ACT_PAU.message,
		@va_Autoroute_out		= EXP..ACT_PAU.autoroute,
		@va_Sens_out			= EXP..ACT_PAU.sens,
		@va_PR_debut_out		= EXP..ACT_PAU.PR_debut,
		@va_PR_fin_out			= EXP..ACT_PAU.PR_fin,
		@va_PAUErreur_out		= EXP..ACT_PAU.PAUErreur,
		@va_PAUPilotes_out		= EXP..ACT_PAU.PAUPilotes
	from EXP..ACT_PAU, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_PAU and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_PAU.actio = @va_NumAction_in and 
		EXP..ACT_PAU.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC892
					@va_NumAction_in,
					@va_Site_in,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,				
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output,
					@va_Message_out			output,
					@va_Autoroute_out		output,
					@va_Sens_out			output,
					@va_PR_debut_out		output,
					@va_PR_fin_out			output,
					@va_PAUErreur_out		output,
					@va_PAUPilotes_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
