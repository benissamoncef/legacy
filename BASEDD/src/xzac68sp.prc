/*E*/
/*  Fichier : $Id: xzac68sp.prc,v 1.5 2010/09/02 09:46:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/09/02 09:46:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac68sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  19/01/98        : Creation
* PN    29/06/98        : separation EXP/HIS 1.2
* JMG	27/04/04	: pilotage PAL par scenario 1.3
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.4
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio de deployement de PAL par scenario
* 
* Sequence d'appel
* SP	XZAC68_Lire_Commande_PAL
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
* char(6)		va_Scenario_in
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
* Select des tables ACTION et E_PAL
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAC;68
	@va_NumeroAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureEchec_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output,
	@va_Manuelle_out		bit		= XDC_NON	output,
	@va_Scenario_out		char(6)		= null		output
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
		@va_NumEqt_out			= HIS..ACT_GEN.equipement,
		@va_HeureLancement_out		= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out		= HIS..ACT_GEN.heure_fin,
		@va_Manuelle_out		= HIS..ACT_PAL.manuelle,
		@va_Scenario_out		= HIS..ACT_PAL.scenario
	from HIS..ACT_PAL, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumeroAction_in and 
			HIS..ACT_GEN.type = XDC_ACT_PAL and
			HIS..ACT_GEN.sit = @va_Site_in and
			HIS..ACT_PAL.actio = @va_NumeroAction_in and 
			HIS..ACT_PAL.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin,
		@va_Manuelle_out		= EXP..ACT_PAL.manuelle,
		@va_Scenario_out		= EXP..ACT_PAL.scenario
	from EXP..ACT_PAL, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_PAL  and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_PAL.actio = @va_NumeroAction_in and 
		EXP..ACT_PAL.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;68
					@va_NumeroAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output,
					@va_Manuelle_out		output,
					@va_Scenario_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
