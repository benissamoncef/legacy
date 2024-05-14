/*E*/
/*  Fichier : $Id: xzac85sp.prc,v 1.3 2010/09/02 09:47:29 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2010/09/02 09:47:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac85sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.2
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC85_Lire_Commande_PMVA
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

drop procedure XZAC85
go

create procedure XZAC85
	@va_NumAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_Ligne1_out			char(15)	= null		output,
	@va_Ligne2_out			char(15)	= null		output,
	@va_Ligne3_out			char(15)	= null		output,
	@va_Ligne4_out			char(15)	= null		output,
	@va_Ligne5_out			char(15)	= null		output,
	@va_Alternat1_out		char(15)	= null		output,
	@va_Alternat2_out		char(15)	= null		output,
	@va_Alternat3_out		char(15)	= null		output,
	@va_Alternat4_out		char(15)	= null		output,
	@va_Alternat5_out		char(15)	= null		output,
	@va_Clignotement1_out		bit		= XDC_NON	output,
	@va_Clignotement2_out		bit		= XDC_NON	output,
	@va_Clignotement3_out		bit		= XDC_NON	output,
	@va_Clignotement4_out		bit		= XDC_NON	output,
	@va_Clignotement5_out		bit		= XDC_NON	output,
	@va_Flash_out			tinyint         = null          output,
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
		@va_Ligne1_out			= HIS..ACT_PMA.premiere_ligne,
		@va_Ligne2_out			= HIS..ACT_PMA.deuxieme_ligne,
		@va_Ligne3_out			= HIS..ACT_PMA.troisieme_ligne,
		@va_Ligne4_out			= HIS..ACT_PMA.quatrieme_ligne,
		@va_Ligne5_out			= HIS..ACT_PMA.cinquieme_ligne,
		@va_Alternat1_out		= HIS..ACT_PMA.premiere_ligne_alternat,
		@va_Alternat2_out		= HIS..ACT_PMA.deuxieme_ligne_alternat,
		@va_Alternat3_out		= HIS..ACT_PMA.troisieme_ligne_alternat,
		@va_Alternat4_out		= HIS..ACT_PMA.quatrieme_ligne_alternat,
		@va_Alternat5_out		= HIS..ACT_PMA.cinquieme_ligne_alternat,
		@va_Clignotement1_out		= HIS..ACT_PMA.clignotement1,
		@va_Clignotement2_out		= HIS..ACT_PMA.clignotement2,
		@va_Clignotement3_out		= HIS..ACT_PMA.clignotement3,
		@va_Clignotement4_out		= HIS..ACT_PMA.clignotement4,
		@va_Clignotement5_out		= HIS..ACT_PMA.clignotement5,
		@va_Flash_out			= HIS..ACT_PMA.flash,
		@va_HeureLancement_out		= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out		= HIS..ACT_GEN.heure_fin
	from HIS..ACT_PMA, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_PMVA and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_PMA.actio = @va_NumAction_in and 
		HIS..ACT_PMA.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select 
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_Ligne1_out			= EXP..ACT_PMA.premiere_ligne,
		@va_Ligne2_out			= EXP..ACT_PMA.deuxieme_ligne,
		@va_Ligne3_out			= EXP..ACT_PMA.troisieme_ligne,
		@va_Ligne4_out			= EXP..ACT_PMA.quatrieme_ligne,
		@va_Ligne5_out			= EXP..ACT_PMA.cinquieme_ligne,
		@va_Alternat1_out		= EXP..ACT_PMA.premiere_ligne_alternat,
		@va_Alternat2_out		= EXP..ACT_PMA.deuxieme_ligne_alternat,
		@va_Alternat3_out		= EXP..ACT_PMA.troisieme_ligne_alternat,
		@va_Alternat4_out		= EXP..ACT_PMA.quatrieme_ligne_alternat,
		@va_Alternat5_out		= EXP..ACT_PMA.cinquieme_ligne_alternat,
		@va_Clignotement1_out		= EXP..ACT_PMA.clignotement1,
		@va_Clignotement2_out		= EXP..ACT_PMA.clignotement2,
		@va_Clignotement3_out		= EXP..ACT_PMA.clignotement3,
		@va_Clignotement4_out		= EXP..ACT_PMA.clignotement4,
		@va_Clignotement5_out		= EXP..ACT_PMA.clignotement5,
		@va_Flash_out			= EXP..ACT_PMA.flash,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin
	from EXP..ACT_PMA, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_PMVA and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_PMA.actio = @va_NumAction_in and 
		EXP..ACT_PMA.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC85
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
					@va_Clignotement1_out		output,
					@va_Clignotement2_out		output,
					@va_Clignotement3_out		output,
					@va_Clignotement4_out		output,
					@va_Clignotement5_out		output,
					@va_Flash_out			output,
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
