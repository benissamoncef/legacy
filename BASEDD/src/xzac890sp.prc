/*E*/
/*  Fichier : $Id: xzac890sp.prc,v 1.1 2021/05/03 12:55:43 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2021/05/03 12:55:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac890sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	25/11/20	creation IMU Insertion de Messages d'Urgence DEM-SAE155 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
*
* Sequence d'appel
* SP	XZAC890_Lire_Commande_IMU
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
* XDY_Texte	va_Chaine_out
* Int		va_Intevalle_out
* Int		va_Cycles_out
* Int		va_Duree_out
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
* Select dans les tables ACTION et E_IMU
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC890
go

create procedure XZAC890
	@va_NumAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureEchec_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output,
	@va_Chaine_out			char(1024)	= null		output,
	@va_Intevalle_out		int		= null		output,
	@va_Cycles_out			int		= null		output,
	@va_Duree_out			int		= null		output
as
	declare	@vl_NbEnrg		int,
		@vl_Status		int

	/*! transfo en param�tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST

	select 
		@va_NumEqt_out			= HIS..ACT_GEN.equipement,
		@va_HeureLancement_out		= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out		= HIS..ACT_GEN.heure_fin,
		@va_Chaine_out			= HIS..ACT_IMU.chaine,
		@va_Intevalle_out		= HIS..ACT_IMU.intervalle,
		@va_Cycles_out			= HIS..ACT_IMU.cycles,
		@va_Duree_out			= HIS..ACT_IMU.duree
	from HIS..ACT_IMU, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_IMU and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_IMU.actio = @va_NumAction_in and 
		HIS..ACT_IMU.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select 
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin,
		@va_Chaine_out			= EXP..ACT_IMU.chaine,
		@va_Intevalle_out		= EXP..ACT_IMU.intervalle,
		@va_Cycles_out			= EXP..ACT_IMU.cycles,
		@va_Duree_out			= EXP..ACT_IMU.duree
	from EXP..ACT_IMU, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_IMU and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_IMU.actio = @va_NumAction_in and 
		EXP..ACT_IMU.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC890
					@va_NumAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,				
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output,
					@va_Chaine_out			output,
					@va_Intevalle_out		output,
					@va_Cycles_out			output,
					@va_Duree_out			output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
