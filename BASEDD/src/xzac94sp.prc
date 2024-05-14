/*E*/
/*  Fichier : $Id: xzac94sp.prc,v 1.3 2010/09/02 09:47:06 gesconf Exp $     Release : $Revision: 1.3 $        Date : $Date: 2010/09/02 09:47:06 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzac94.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.2
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.3

------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio de deployement de BAF 
* 
* Sequence d'appel
* SP	XZAC94_Lire_Commande_BAF
* 
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* 
* Arguments en sortie
* XDY_Eqt		va_NumEqt_in
* XDY_Horodate		va_HeureLancement_in
* XDY_Horodate		va_HeureLancementFin_in
* XDY_Horodate		va_HeureSucces_in
* XDY_Horodate		va_HeureEchec_in
* XDY_Horodate		va_HeureFin_in
* XDY_Booleen		va_Manuelle_in
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
* Select des tables ACTION et E_BAF
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC94
go

create procedure XZAC94
	@va_NumeroAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureEchec_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output,
	@va_Manuelle_out		bit		= XDC_NON	output,
	@va_Module11_out		tinyint		= null		output,
	@va_Module12_out		tinyint		= null		output,
	@va_Module15_out		tinyint		= null		output,
	@va_Module21_out		tinyint		= null		output,
	@va_Module22_out		tinyint		= null		output,
	@va_Module25_out		tinyint		= null		output
as
	declare	@vl_NbEnrg		int,
		@vl_Status		int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumeroAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

#ifdef HIST

 	/*A alors les rechercher dans la base HIS */
	select
		@va_NumEqt_out		= HIS..ACT_GEN.equipement,
		@va_HeureLancement_out	= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out	= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out	= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out	= HIS..ACT_GEN.heure_fin,
		@va_Manuelle_out	= HIS..ACT_BAF.manuelle,
		@va_Module11_out	= HIS..ACT_BAF.module11,
		@va_Module12_out	= HIS..ACT_BAF.module12,
		@va_Module15_out	= HIS..ACT_BAF.module15,
		@va_Module21_out	= HIS..ACT_BAF.module21,
		@va_Module22_out	= HIS..ACT_BAF.module22,
		@va_Module25_out	= HIS..ACT_BAF.module25
	from HIS..ACT_BAF, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumeroAction_in and 
			HIS..ACT_GEN.type = XDC_ACT_BAF and
			HIS..ACT_GEN.sit = @va_Site_in and
			HIS..ACT_BAF.actio = @va_NumeroAction_in and 
			HIS..ACT_BAF.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin,
		@va_Manuelle_out		= EXP..ACT_BAF.manuelle,
		@va_Module11_out	= EXP..ACT_BAF.module11,
		@va_Module12_out	= EXP..ACT_BAF.module12,
		@va_Module15_out	= EXP..ACT_BAF.module15,
		@va_Module21_out	= EXP..ACT_BAF.module21,
		@va_Module22_out	= EXP..ACT_BAF.module22,
		@va_Module25_out	= EXP..ACT_BAF.module25
	from EXP..ACT_BAF, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_BAF and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_BAF.actio = @va_NumeroAction_in and 
		EXP..ACT_BAF.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC94
					@va_NumeroAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output,
					@va_Manuelle_out		output,
					@va_Module11_out		output,
					@va_Module12_out		output,
					@va_Module15_out		output,
					@va_Module21_out		output,
					@va_Module22_out		output,
					@va_Module25_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
