/*E*/
/*  Fichier : 	$Id: xzat050sp.prc,v 1.4 2018/05/29 12:18:38 devgfi Exp $ Release : $Revision: 1.4 $ Date : $Date: 2018/05/29 12:18:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asrv_t050sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG	02/01/13	correction client sybase 1.2
* PNI	11/10/13	: SAPIENS AU CI : on consid�re l'utilisation de l'eqt � partir de l'heure_lancement ou heure_succes DEM 1074 1.3
* JPL	29/05/18	: Suppression prealable de la procedure stockee si elle existe  1.4
* LCL	13/07/23	: Correction SAE-502 : Au CI, état transitoire de commande fait que 2 lignes remontent : la proc sort en erreur
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un PMV a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT050_Utilisation_PMVA
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPMV_out
* XDY_Octet  	va_TypePMV_out
* XDY_LignePMV	va_Ligne1_out
* XDY_LignePMV	va_Ligne2_out
* XDY_LignePMV	va_Ligne3_out
* XDY_LignePMV	va_Alternat1_out
* XDY_LignePMV	va_Alternat2_out
* XDY_LignePMV	va_Alternat3_out
* XDY_Booleen	va_Clignotement1_out
* XDY_Booleen	va_Clignotement2_out
* XDY_Booleen	va_Clignotement3_out
* XDY_Booleen	va_Flash_out
* char[4]	va_TypePicto_out
* char[6]	va_TextePicto_out
* XDY_Octet	va_NumTypePicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_DistanceEvt_out
* XDY_Entier	va_Action_out
* XDY_Site	va_SiteAction_out
* XDY_LignePMV	va_LigneDir_out
* XDY_LignePMV	va_AlternatDir_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* Fonction
* select dans les tables EQUIPEMENT, E_PMV, ACTION et 
* INDISPONIBILITE
------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT050' and type = 'P')
	drop procedure XZAT050
go

create procedure XZAT050
	@va_Horodate_in			datetime	= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_DispoPMV_out		tinyint		= null		output,
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
	@va_Flash_out			bit		= XDC_NON	output,
	@va_NumEvt_out			int		= null		output,
	@va_CleEvt_out			tinyint		= null		output,
	@va_Priorite_out		tinyint		= null		output,
	@va_DistanceEvt_out		int		= null		output,
	@va_Action_out			int		= null		output,
	@va_SiteAction_out		T_SITE		= null		output
as
	declare @vl_HS bit, @vl_Critique bit, @vl_Majeure bit, @vl_Inhibe bit,
		@vl_Desactive bit,  @vl_Mineure bit, @vl_NbEnregistrements int,
		@vl_Status int
	/*! transformations bugs applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEqt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

#ifdef HIST
		/*B lecture des infos sur le PMV */
		select * into #ACT_GEN 
		from HIS..ACT_GEN
		where HIS..ACT_GEN.equipement = @va_NumEqt_in and
	              HIS..ACT_GEN.type = XDC_ACT_PMVA and
		      HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
		      ( HIS..ACT_GEN.heure_fin > @va_Horodate_in or
			HIS..ACT_GEN.heure_fin is null)
		
		select 
			@va_Ligne1_out		= HIS..ACT_PMA.premiere_ligne,
			@va_Ligne2_out		= HIS..ACT_PMA.deuxieme_ligne,
			@va_Ligne3_out		= HIS..ACT_PMA.troisieme_ligne,
			@va_Ligne4_out		= HIS..ACT_PMA.quatrieme_ligne,
			@va_Ligne5_out		= HIS..ACT_PMA.cinquieme_ligne,
			@va_Alternat1_out	= HIS..ACT_PMA.premiere_ligne_alternat,
			@va_Alternat2_out	= HIS..ACT_PMA.deuxieme_ligne_alternat,
			@va_Alternat3_out	= HIS..ACT_PMA.troisieme_ligne_alternat,
			@va_Alternat4_out	= HIS..ACT_PMA.quatrieme_ligne_alternat,
			@va_Alternat5_out	= HIS..ACT_PMA.cinquieme_ligne_alternat,
			@va_Clignotement1_out	= HIS..ACT_PMA.clignotement1,
			@va_Clignotement2_out	= HIS..ACT_PMA.clignotement2,
			@va_Clignotement3_out	= HIS..ACT_PMA.clignotement3,
			@va_Clignotement4_out	= HIS..ACT_PMA.clignotement4,
			@va_Clignotement5_out	= HIS..ACT_PMA.clignotement5,
			@va_Flash_out		= HIS..ACT_PMA.flash,
			@va_NumEvt_out		= #ACT_GEN.evenement,
			@va_CleEvt_out		= #ACT_GEN.cle,
			@va_Priorite_out	= #ACT_GEN.priorite,
			@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
			@va_Action_out		= #ACT_GEN.numero,
			@va_SiteAction_out	= #ACT_GEN.sit
		from HIS..ACT_PMA, #ACT_GEN
		where   HIS..ACT_PMA.actio = #ACT_GEN.numero and 
			HIS..ACT_PMA.sit = #ACT_GEN.sit and #ACT_GEN.heure_fin is not null
	
		select @vl_NbEnregistrements = @@rowcount
		if @vl_NbEnregistrements = 0
		begin
		select 
			@va_Ligne1_out		= HIS..ACT_PMA.premiere_ligne,
			@va_Ligne2_out		= HIS..ACT_PMA.deuxieme_ligne,
			@va_Ligne3_out		= HIS..ACT_PMA.troisieme_ligne,
			@va_Ligne4_out		= HIS..ACT_PMA.quatrieme_ligne,
			@va_Ligne5_out		= HIS..ACT_PMA.cinquieme_ligne,
			@va_Alternat1_out	= HIS..ACT_PMA.premiere_ligne_alternat,
			@va_Alternat2_out	= HIS..ACT_PMA.deuxieme_ligne_alternat,
			@va_Alternat3_out	= HIS..ACT_PMA.troisieme_ligne_alternat,
			@va_Alternat4_out	= HIS..ACT_PMA.quatrieme_ligne_alternat,
			@va_Alternat5_out	= HIS..ACT_PMA.cinquieme_ligne_alternat,
			@va_Clignotement1_out	= HIS..ACT_PMA.clignotement1,
			@va_Clignotement2_out	= HIS..ACT_PMA.clignotement2,
			@va_Clignotement3_out	= HIS..ACT_PMA.clignotement3,
			@va_Clignotement4_out	= HIS..ACT_PMA.clignotement4,
			@va_Clignotement5_out	= HIS..ACT_PMA.clignotement5,
			@va_Flash_out		= HIS..ACT_PMA.flash,
			@va_NumEvt_out		= #ACT_GEN.evenement,
			@va_CleEvt_out		= #ACT_GEN.cle,
			@va_Priorite_out	= #ACT_GEN.priorite,
			@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
			@va_Action_out		= #ACT_GEN.numero,
			@va_SiteAction_out	= #ACT_GEN.sit
		from HIS..ACT_PMA, #ACT_GEN
		where	HIS..ACT_PMA.actio = #ACT_GEN.numero and 
			HIS..ACT_PMA.sit = #ACT_GEN.sit and #ACT_GEN.heure_fin is null
		
		end
	
		if @vl_NbEnregistrements > 1
			/*! erreur dans la base */
			return XDC_NOK
	
		/*B rechercher la disponibilite de l'equipement et le type de picto */
		select
			@vl_Desactive	= HIS..EQT_DSP.desactive,
			@vl_HS		= HIS..EQT_DSP.HS,
			@vl_Majeure	= HIS..EQT_DSP.majeure,
			@vl_Critique	= HIS..EQT_DSP.critique,
			@vl_Mineure	= HIS..EQT_DSP.mineure,
			@vl_Inhibe 	= HIS..EQT_DSP.inhibe
		from HIS..EQT_DSP
		where	HIS..EQT_DSP.equipement = @va_NumEqt_in and 
			HIS..EQT_DSP.type = XDC_EQT_PMVA and
			HIS..EQT_DSP.horodate in (select max(horodate) from HIS..EQT_DSP
						where equipement = @va_NumEqt_in and 
						type = XDC_EQT_PMVA and
						horodate <= @va_Horodate_in)
	
	
	        if @@rowcount != 1
			/*! erreur dans la base */
			return XDC_NOK
	
		/*B calcul de l'etat de l'equipement */
		select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
					@vl_HS * XDC_EQT_HS +
	   				@vl_Majeure * XDC_EQT_MAJEUR + 
					@vl_Critique * XDC_EQT_CRITIQUE +
	   				@vl_Inhibe * XDC_EQT_INHIBE + 
					@vl_Mineure * XDC_EQT_MINEUR

#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAT050
			@va_Horodate_in, @va_NumEqt_in,
			@va_DispoPMV_out	output, 
			@va_Ligne1_out		output,
			@va_Ligne2_out		output, @va_Ligne3_out		output,
			@va_Ligne4_out		output, @va_Ligne5_out		output,
			@va_Alternat1_out	output, @va_Alternat2_out	output,
			@va_Alternat3_out	output, @va_Alternat4_out	output,
			@va_Alternat5_out	output, @va_Clignotement1_out	output,
			@va_Clignotement2_out	output, @va_Clignotement3_out	output,
			@va_Clignotement4_out	output, @va_Clignotement5_out	output,
			@va_Flash_out		output, @va_NumEvt_out	output,
			@va_CleEvt_out		output, @va_Priorite_out	output,
			@va_DistanceEvt_out	output, @va_Action_out		output,
			@va_SiteAction_out	output
       		if @vl_Status = null
                     	return XDC_PRC_INC
           	else return @vl_Status
	end
	else
	begin 
		/*B lecture des infos sur le PMV */
		select * into #ACT_GEN 
		from EXP..ACT_GEN
		where equipement = @va_NumEqt_in and
	              type = XDC_ACT_PMVA and
		      (heure_succes <= @va_Horodate_in OR EXP..ACT_GEN.heure_lancement <= @va_Horodate_in) and
		      ( heure_fin > @va_Horodate_in or
			heure_fin is null)
		
		/* Correction JIRA SAE-502 */
		set rowcount 1

		select 
			@va_Ligne1_out		= EXP..ACT_PMA.premiere_ligne,
			@va_Ligne2_out		= EXP..ACT_PMA.deuxieme_ligne,
			@va_Ligne3_out		= EXP..ACT_PMA.troisieme_ligne,
			@va_Ligne4_out		= EXP..ACT_PMA.quatrieme_ligne,
			@va_Ligne5_out		= EXP..ACT_PMA.cinquieme_ligne,
			@va_Alternat1_out	= EXP..ACT_PMA.premiere_ligne_alternat,
			@va_Alternat2_out	= EXP..ACT_PMA.deuxieme_ligne_alternat,
			@va_Alternat3_out	= EXP..ACT_PMA.troisieme_ligne_alternat,
			@va_Alternat4_out	= EXP..ACT_PMA.quatrieme_ligne_alternat,
			@va_Alternat5_out	= EXP..ACT_PMA.cinquieme_ligne_alternat,
			@va_Clignotement1_out	= EXP..ACT_PMA.clignotement1,
			@va_Clignotement2_out	= EXP..ACT_PMA.clignotement2,
			@va_Clignotement3_out	= EXP..ACT_PMA.clignotement3,
			@va_Clignotement4_out	= EXP..ACT_PMA.clignotement4,
			@va_Clignotement5_out	= EXP..ACT_PMA.clignotement5,
			@va_Flash_out		= EXP..ACT_PMA.flash,
			@va_NumEvt_out		= #ACT_GEN.evenement,
			@va_CleEvt_out		= #ACT_GEN.cle,
			@va_Priorite_out	= #ACT_GEN.priorite,
			@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
			@va_Action_out		= #ACT_GEN.numero,
			@va_SiteAction_out	= #ACT_GEN.sit
		from EXP..ACT_PMA, #ACT_GEN
		where	#ACT_GEN.equipement = @va_NumEqt_in and #ACT_GEN.type = XDC_ACT_PMVA and
			EXP..ACT_PMA.actio = #ACT_GEN.numero and 
			EXP..ACT_PMA.sit = #ACT_GEN.sit and (#ACT_GEN.heure_succes <= @va_Horodate_in OR #ACT_GEN.heure_lancement <= @va_Horodate_in) and (#ACT_GEN.heure_fin is null or #ACT_GEN.heure_fin > @va_Horodate_in) 
		order by EXP..ACT_PMA.actio desc

		select @vl_NbEnregistrements = @@rowcount
	
		/* Correction JIRA SAE-502 */
		set rowcount 0

		if @vl_NbEnregistrements > 1
			/*! erreur dans la base */
			return XDC_NOK
	
		/*B rechercher la disponibilite de l'equipement et le type de picto */
		select
			@vl_Desactive	= EXP..EQT_DSP.desactive,
			@vl_HS		= EXP..EQT_DSP.HS,
			@vl_Majeure	= EXP..EQT_DSP.majeure,
			@vl_Critique	= EXP..EQT_DSP.critique,
			@vl_Mineure	= EXP..EQT_DSP.mineure,
			@vl_Inhibe 	= EXP..EQT_DSP.inhibe
		from EXP..EQT_DSP
		where	EXP..EQT_DSP.equipement = @va_NumEqt_in and 
			EXP..EQT_DSP.type = XDC_EQT_PMVA and
			EXP..EQT_DSP.horodate in (select max(horodate) from EXP..EQT_DSP
						where equipement = @va_NumEqt_in and 
						type = XDC_EQT_PMVA and
						horodate <= @va_Horodate_in)
	
	
	        if @@rowcount != 1
			/*! erreur dans la base */
			return XDC_NOK
	
		/*B calcul de l'etat de l'equipement */
		select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
					@vl_HS * XDC_EQT_HS +
	   				@vl_Majeure * XDC_EQT_MAJEUR + 
					@vl_Critique * XDC_EQT_CRITIQUE +
	   				@vl_Inhibe * XDC_EQT_INHIBE + 
					@vl_Mineure * XDC_EQT_MINEUR

	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des infos sur le PMV */
	select * into #ACT_GEN 
	from EXP..ACT_GEN
	where equipement = @va_NumEqt_in and
	              type = XDC_ACT_PMVA and
		      heure_succes <= @va_Horodate_in and
		      ( heure_fin > @va_Horodate_in or
			heure_fin is null)
		
	select 
			@va_Ligne1_out		= EXP..ACT_PMA.premiere_ligne,
			@va_Ligne2_out		= EXP..ACT_PMA.deuxieme_ligne,
			@va_Ligne3_out		= EXP..ACT_PMA.troisieme_ligne,
			@va_Ligne4_out		= EXP..ACT_PMA.quatrieme_ligne,
			@va_Ligne5_out		= EXP..ACT_PMA.cinquieme_ligne,
			@va_Alternat1_out	= EXP..ACT_PMA.premiere_ligne_alternat,
			@va_Alternat2_out	= EXP..ACT_PMA.deuxieme_ligne_alternat,
			@va_Alternat3_out	= EXP..ACT_PMA.troisieme_ligne_alternat,
			@va_Alternat4_out	= EXP..ACT_PMA.quatrieme_ligne_alternat,
			@va_Alternat5_out	= EXP..ACT_PMA.cinquieme_ligne_alternat,
			@va_Clignotement1_out	= EXP..ACT_PMA.clignotement1,
			@va_Clignotement2_out	= EXP..ACT_PMA.clignotement2,
			@va_Clignotement3_out	= EXP..ACT_PMA.clignotement3,
			@va_Clignotement4_out	= EXP..ACT_PMA.clignotement4,
			@va_Clignotement5_out	= EXP..ACT_PMA.clignotement5,
			@va_Flash_out		= EXP..ACT_PMA.flash,
		@va_NumEvt_out		= #ACT_GEN.evenement,
		@va_CleEvt_out		= #ACT_GEN.cle,
		@va_Priorite_out	= #ACT_GEN.priorite,
		@va_DistanceEvt_out	= #ACT_GEN.distance_evt,
		@va_Action_out		= #ACT_GEN.numero,
		@va_SiteAction_out	= #ACT_GEN.sit
	from EXP..ACT_PMA, #ACT_GEN
	where	#ACT_GEN.equipement = @va_NumEqt_in and #ACT_GEN.type = XDC_ACT_PMVA and
		EXP..ACT_PMA.actio = #ACT_GEN.numero and 
		EXP..ACT_PMA.sit = #ACT_GEN.sit and #ACT_GEN.heure_succes <= @va_Horodate_in and (#ACT_GEN.heure_fin is null or #ACT_GEN.heure_fin > @va_Horodate_in) 

	select @vl_NbEnregistrements = @@rowcount

	if @vl_NbEnregistrements > 1
		/*! erreur dans la base */
		return XDC_NOK

	/*A lecture de la disponibilite de l'equipement et type de picto */
	select
		@vl_Desactive	= EXP..EQT_DSP.desactive,
		@vl_HS		= EXP..EQT_DSP.HS,
		@vl_Majeure	= EXP..EQT_DSP.majeure,
		@vl_Critique	= EXP..EQT_DSP.critique,
		@vl_Mineure	= EXP..EQT_DSP.mineure,
		@vl_Inhibe 	= EXP..EQT_DSP.inhibe
	from EXP..EQT_DSP
	where	EXP..EQT_DSP.equipement = @va_NumEqt_in and 
		EXP..EQT_DSP.type = XDC_EQT_PMVA and
		EXP..EQT_DSP.horodate in (select max(horodate) from EXP..EQT_DSP
					where equipement = @va_NumEqt_in and 
					type = XDC_EQT_PMVA and
					horodate <= @va_Horodate_in)


        if @@rowcount != 1
		/*! erreur dans la base */
		return XDC_NOK

	/*A calcul de l'etat de l'equipement */
	select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
				@vl_HS * XDC_EQT_HS +
	   			@vl_Majeure * XDC_EQT_MAJEUR + 
				@vl_Critique * XDC_EQT_CRITIQUE +
	   			@vl_Inhibe * XDC_EQT_INHIBE + 
				@vl_Mineure * XDC_EQT_MINEUR

#endif
#endif

	if @@rowcount != 1
		return XDC_NOK

	/*A si le PMV n'est pas utilise alors positionner toutes */
	/*A les chaines de caracteres retournees a " " pour eviter */
	/*A les problemes de retour de null, sauf le type du picto */
	/*A qui doit etre positionne a picto vide                  */
	/*! attention conserve la valuation a null pour evt sinon  */
	/*! conservation de la valeur du parametre au 2eme appel   */
	if @vl_NbEnregistrements = 0
		select	@va_Ligne1_out		= XDC_CHAINE_VIDE,
			@va_Ligne2_out		= XDC_CHAINE_VIDE,
			@va_Ligne3_out		= XDC_CHAINE_VIDE,
			@va_Ligne4_out		= XDC_CHAINE_VIDE,
			@va_Ligne5_out		= XDC_CHAINE_VIDE,
			@va_Alternat1_out	= XDC_CHAINE_VIDE,
			@va_Alternat2_out	= XDC_CHAINE_VIDE,
			@va_Alternat3_out	= XDC_CHAINE_VIDE,
			@va_Alternat4_out	= XDC_CHAINE_VIDE,
			@va_Alternat5_out	= XDC_CHAINE_VIDE,
			@va_Clignotement1_out	= XDC_NON,
			@va_Clignotement2_out	= XDC_NON,
			@va_Clignotement3_out	= XDC_NON,
			@va_Clignotement4_out	= XDC_NON,
			@va_Clignotement5_out	= XDC_NON,
			@va_Flash_out		= XDC_NON,
			@va_NumEvt_out 		= null, 
			@va_CleEvt_out 		= null

	/*A calcul de l'etat de l'equipement */
	select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
				@vl_HS * XDC_EQT_HS +
	   			@vl_Majeure * XDC_EQT_MAJEUR + 
				@vl_Critique * XDC_EQT_CRITIQUE +
	   			@vl_Inhibe * XDC_EQT_INHIBE + 
				@vl_Mineure * XDC_EQT_MINEUR


	select @va_Ligne1_out, @va_Ligne2_out, @va_Ligne3_out, @va_Ligne4_out, @va_Ligne5_out,
		@va_Alternat1_out, @va_Alternat2_out, @va_Alternat3_out, @va_Alternat4_out, @va_Alternat5_out

	return XDC_OK
go
