/*E*/
/*  Fichier : $Id: xzac185sp.prc,v 1.1 2017/04/02 16:31:28 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2017/04/02 16:31:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac185sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC185_Lire_Commande_Eqt_SAGA
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_LignePMV	va_Ligne1_out
* XDY_LignePMV	va_Ligne2_out
* XDY_LignePMV	va_Ligne3_out
* XDY_LignePMV	va_Ligne4_out
* XDY_LignePMV	va_Ligne5_out
* XDY_LignePMV	va_Alternat1_out
* XDY_LignePMV	va_Alternat2_out
* XDY_LignePMV	va_Alternat3_out
* XDY_LignePMV	va_Alternat4_out
* XDY_LignePMV	va_Alternat5_out
* XDY_Booleen	va_Clignotement1_out
* XDY_Booleen	va_Clignotement2_out
* XDY_Booleen	va_Clignotement3_out
* XDY_Booleen	va_Clignotement4_out
* XDY_Booleen	va_Clignotement5_out
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
* Select dans les tables ACTION et E_PMV
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC185
go

create procedure XZAC185
	@va_NumAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_Ligne1_out			char(18)	= null		output,
	@va_Ligne2_out			char(18)	= null		output,
	@va_Ligne3_out			char(18)	= null		output,
	@va_Ligne4_out			char(18)	= null		output,
	@va_Ligne5_out			char(18)	= null		output,
	@va_Alternat1_out		char(18)	= null		output,
	@va_Alternat2_out		char(18)	= null		output,
	@va_Alternat3_out		char(18)	= null		output,
	@va_Alternat4_out		char(18)	= null		output,
	@va_Alternat5_out		char(18)	= null		output,
	@va_Clignotement_out		bit		= XDC_NON	output,
	@va_Picto_out		        char(10)	= null		output,
	@va_Luminosite_out		smallint	= null		output,
	@va_Bandeau_out			char(10)	= null		output,
	@va_Barriere_out		smallint	= null		output,
	@va_Scenario_out		smallint	= null		output,
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
		@va_Ligne1_out			= HIS..ACT_SAGA.premiere_ligne,
		@va_Ligne2_out			= HIS..ACT_SAGA.deuxieme_ligne,
		@va_Ligne3_out			= HIS..ACT_SAGA.troisieme_ligne,
		@va_Ligne4_out			= HIS..ACT_SAGA.quatrieme_ligne,
		@va_Ligne5_out			= HIS..ACT_SAGA.cinquieme_ligne,
		@va_Alternat1_out		= HIS..ACT_SAGA.premiere_ligne_alternat,
		@va_Alternat2_out		= HIS..ACT_SAGA.deuxieme_ligne_alternat,
		@va_Alternat3_out		= HIS..ACT_SAGA.troisieme_ligne_alternat,
		@va_Alternat4_out		= HIS..ACT_SAGA.quatrieme_ligne_alternat,
		@va_Alternat5_out		= HIS..ACT_SAGA.cinquieme_ligne_alternat,
		@va_Clignotement_out		= HIS..ACT_SAGA.clignotement,
		@va_Picto_out			=  HIS..ACT_SAGA.picto,
		@va_Luminosite_out		= HIS..ACT_SAGA.luminosite,
		@va_Bandeau_out			= HIS..ACT_SAGA.bandeau,
		@va_Barriere_out		= HIS..ACT_SAGA.barriere,
		@va_Scenario_out		= HIS..ACT_SAGA.scenario,
		@va_HeureLancement_out		= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out		= HIS..ACT_GEN.heure_fin
	from HIS..ACT_SAGA, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_SAGA and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_SAGA.actio = @va_NumAction_in and 
		HIS..ACT_SAGA.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select 
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_Ligne1_out			= EXP..ACT_SAGA.premiere_ligne,
		@va_Ligne2_out			= EXP..ACT_SAGA.deuxieme_ligne,
		@va_Ligne3_out			= EXP..ACT_SAGA.troisieme_ligne,
		@va_Ligne4_out			= EXP..ACT_SAGA.quatrieme_ligne,
		@va_Ligne5_out			= EXP..ACT_SAGA.cinquieme_ligne,
		@va_Alternat1_out		= EXP..ACT_SAGA.premiere_ligne_alternat,
		@va_Alternat2_out		= EXP..ACT_SAGA.deuxieme_ligne_alternat,
		@va_Alternat3_out		= EXP..ACT_SAGA.troisieme_ligne_alternat,
		@va_Alternat4_out		= EXP..ACT_SAGA.quatrieme_ligne_alternat,
		@va_Alternat5_out		= EXP..ACT_SAGA.cinquieme_ligne_alternat,
		@va_Clignotement_out            = EXP..ACT_SAGA.clignotement,
                @va_Picto_out                   =  EXP..ACT_SAGA.picto,
                @va_Luminosite_out              = EXP..ACT_SAGA.luminosite,
                @va_Bandeau_out                 = EXP..ACT_SAGA.bandeau,
                @va_Barriere_out                = EXP..ACT_SAGA.barriere,
                @va_Scenario_out                = EXP..ACT_SAGA.scenario,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin
	from EXP..ACT_SAGA, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_SAGA and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_SAGA.actio = @va_NumAction_in and 
		EXP..ACT_SAGA.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC185
					@va_NumAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_Ligne1_out			output,
					@va_Ligne2_out			output,
					@va_Ligne3_out			output,
					@va_Ligne4_out			output,
					@va_Ligne5_out			output,
					@va_Alternat1_out		output,
					@va_Alternat2_out		output,
					@va_Alternat3_out		output,
					@va_Alternat4_out		output,
					@va_Alternat5_out		output,
					@va_Clignotement_out		output,
					@va_Picto_out			output,
					@va_Luminosite_out		output,
					@va_Bandeau_out			output,
					@va_Barriere_out		output,
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
