/*E*/
/*  Fichier : @(#)xzat05sp.prc	1.4      Release : 1.4        Date : 11/29/94
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asrv_t05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/09/94	: Creation
* C.T.	30/09/94	: Modif interface fonction
*                         ajout de NumTypePicto et TypePMV
* C.T.	10/11/94	: Modif bugs applix V 1.3
* C.T.	15/11/94	: Modif recherche des infos au CI (V 1.4)
*                         et ajout en sortie du numero d'action
*                         Changement du nom du fichier (V 1.1)
* C.T.	24/11/94	: Ajout code de dispo PMV supprime (V 1.3)
* C.T.	29/11/94	: Modif interface fonction ajout site action (V 1.4)
*                         non teste
* B.G.	01/12/94	: ajout date courante si null (V 1.5)
* C.T.	07/02/95	: Modif clause select sur heure_fin exclue (V 1.5)
* P.V.  14/08/96	: ajout lecture ligne direction (V1.6) (ANA30)
* C.T.	19/01/98	: Correction etat dispo du PMV au CI (1.7)
* P.N.	19/06/98	: Correction etat dispo du PMV au CI (1.8)
* P.N.	03/07/98	: separation EXP/HIS (&.9)
* JMG	13/08/12	: correction param de sortie texte pour client V15 linux  1.10
*				suppression ";"
* PNI	11/10/13	: SAPIENS AU CI : on consid�re l'utilisation de l'eqt � partir de l'heure_lancement ou heure_succes DEM 1074 1.14
* JMG	08/08/18 	: ligne PMV 18 car. suppression SAGA DEM1306 1.15
* LCL	20/01/21	: Correction appel XZAT05 sur HIS depuis le CI : XZAT;05 --> XZAT05 1.16
* LCL	13/07/23	: Correction SAE-502 : Au CI, état transitoire de commande fait que 2 lignes remontent : la proc sort en erreur
* GGY	28/04/23 : Augmentation taille va_TextePicto_out a char(XDC_LGR_TXT_PICTO) et va_TypePicto_out (XDC_LGR_NOM_PICTO) (DEM-473)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne l'utilisation reelle d'un PMV a une horodate donnee.
* 
* Sequence d'appel
* CL	XZAT05_Utilisation_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot	va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet  	va_DispoPMV_out
* XDY_Octet  	va_DispoPicto_out
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
* char[XDC_LGR_NOM_PICTO]	va_TypePicto_out
* char[XDC_LGR_TXT_PICTO]	va_TextePicto_out
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

drop procedure XZAT05
go

create procedure XZAT05
	@va_Horodate_in			datetime	= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_DispoPMV_out		tinyint		= null		output,
	@va_DispoPicto_out		tinyint		= null		output,
	@va_TypePMV_out			tinyint		= null		output,
	@va_Ligne1_out			char(18)	= null		output,
	@va_Ligne2_out			char(18)	= null		output,
	@va_Ligne3_out			char(18)	= null		output,
	@va_Alternat1_out		char(18)	= null		output,
	@va_Alternat2_out		char(18)	= null		output,
	@va_Alternat3_out		char(18)	= null		output,
	@va_Clignotement1_out		bit		= XDC_NON	output,
	@va_Clignotement2_out		bit		= XDC_NON	output,
	@va_Clignotement3_out		bit		= XDC_NON	output,
	@va_Flash_out			bit		= XDC_NON	output,
	@va_TypePicto_out		char(XDC_LGR_NOM_PICTO)		= null		output,
	@va_TextePicto_out		char(XDC_LGR_TXT_PICTO)		= null		output,
	@va_NumTypePicto_out		tinyint		= null		output,
	@va_ClignotementPicto_out	bit		= XDC_NON	output,
	@va_NumEvt_out			int		= null		output,
	@va_CleEvt_out			tinyint		= null		output,
	@va_Priorite_out		tinyint		= null		output,
	@va_DistanceEvt_out		int		= null		output,
	@va_Action_out			int		= null		output,
	@va_SiteAction_out		T_SITE		= null		output,
	@va_LigneDir_out		char(18)	= null		output,
	@va_AlternatDir_out		char(18)	= null		output
as
	declare @vl_HS bit, @vl_Critique bit, @vl_Majeure bit, @vl_Inhibe bit,
		@vl_Desactive bit,  @vl_Mineure bit, @vl_NbEnregistrements int,
		@vl_Status int, @vl_presence_picto tinyint
	/*! transformations bugs applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEqt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */

	if @va_Horodate_in=null select @va_Horodate_in=getdate()

	/*A recherche infos sur le flash PMV  et le type de PMV et la presence picto*/
	select 
		@va_Flash_out = flash,
		@va_TypePMV_out = type,
		@vl_presence_picto = picto
	from CFG..TYP_PMV, CFG..EQT_PMV
	where	CFG..EQT_PMV.numero = @va_NumEqt_in and
		CFG..TYP_PMV.numero = CFG..EQT_PMV.type

	if @@rowcount != 1
		return XDC_NOK
#ifdef HIST
		/*B lecture des infos sur le PMV */
		select 
			@va_Ligne1_out		= HIS..ACT_PMV.premiere_ligne,
			@va_Ligne2_out		= HIS..ACT_PMV.deuxieme_ligne,
			@va_Ligne3_out		= HIS..ACT_PMV.troisieme_ligne,
			@va_Alternat1_out	= HIS..ACT_PMV.premiere_ligne_alternat,
			@va_Alternat2_out	= HIS..ACT_PMV.deuxieme_ligne_alternat,
			@va_Alternat3_out	= HIS..ACT_PMV.troisieme_ligne_alternat,
			@va_TypePicto_out	= HIS..ACT_PMV.pictogramme,
			@va_TextePicto_out	= HIS..ACT_PMV.bandeau,
			@va_Clignotement1_out	= HIS..ACT_PMV.clignotement1,
			@va_Clignotement2_out	= HIS..ACT_PMV.clignotement2,
			@va_Clignotement3_out	= HIS..ACT_PMV.clignotement3,
			@va_ClignotementPicto_out	= HIS..ACT_PMV.clignotement_picto,
			@va_Flash_out		= HIS..ACT_PMV.flash,
			@va_NumEvt_out		= HIS..ACT_GEN.evenement,
			@va_CleEvt_out		= HIS..ACT_GEN.cle,
			@va_Priorite_out	= HIS..ACT_GEN.priorite,
			@va_DistanceEvt_out	= HIS..ACT_GEN.distance_evt,
			@va_Action_out		= HIS..ACT_GEN.numero,
			@va_SiteAction_out	= HIS..ACT_GEN.sit,
			@va_LigneDir_out	= HIS..ACT_PMV.direction_ligne,
			@va_AlternatDir_out	= HIS..ACT_PMV.direction_ligne_alternat
		from HIS..ACT_PMV, HIS..ACT_GEN
		where	HIS..ACT_GEN.equipement = @va_NumEqt_in and 
			HIS..ACT_GEN.type = XDC_ACT_PMV and
			HIS..ACT_PMV.actio = HIS..ACT_GEN.numero and 
			HIS..ACT_PMV.sit = HIS..ACT_GEN.sit and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
		        HIS..ACT_GEN.heure_fin > @va_Horodate_in
	
		select @vl_NbEnregistrements = @@rowcount
		if @vl_NbEnregistrements = 0
		begin
		select 
			@va_Ligne1_out		= HIS..ACT_PMV.premiere_ligne,
			@va_Ligne2_out		= HIS..ACT_PMV.deuxieme_ligne,
			@va_Ligne3_out		= HIS..ACT_PMV.troisieme_ligne,
			@va_Alternat1_out	= HIS..ACT_PMV.premiere_ligne_alternat,
			@va_Alternat2_out	= HIS..ACT_PMV.deuxieme_ligne_alternat,
			@va_Alternat3_out	= HIS..ACT_PMV.troisieme_ligne_alternat,
			@va_TypePicto_out	= HIS..ACT_PMV.pictogramme,
			@va_TextePicto_out	= HIS..ACT_PMV.bandeau,
			@va_Clignotement1_out	= HIS..ACT_PMV.clignotement1,
			@va_Clignotement2_out	= HIS..ACT_PMV.clignotement2,
			@va_Clignotement3_out	= HIS..ACT_PMV.clignotement3,
			@va_ClignotementPicto_out	= HIS..ACT_PMV.clignotement_picto,
			@va_Flash_out		= HIS..ACT_PMV.flash,
			@va_NumEvt_out		= HIS..ACT_GEN.evenement,
			@va_CleEvt_out		= HIS..ACT_GEN.cle,
			@va_Priorite_out	= HIS..ACT_GEN.priorite,
			@va_DistanceEvt_out	= HIS..ACT_GEN.distance_evt,
			@va_Action_out		= HIS..ACT_GEN.numero,
			@va_SiteAction_out	= HIS..ACT_GEN.sit,
			@va_LigneDir_out	= HIS..ACT_PMV.direction_ligne,
			@va_AlternatDir_out	= HIS..ACT_PMV.direction_ligne_alternat
		from HIS..ACT_PMV, HIS..ACT_GEN
		where	HIS..ACT_GEN.equipement = @va_NumEqt_in and 
			HIS..ACT_GEN.type = XDC_ACT_PMV and
			HIS..ACT_PMV.actio = HIS..ACT_GEN.numero and 
			HIS..ACT_PMV.sit = HIS..ACT_GEN.sit and
			HIS..ACT_GEN.heure_succes <= @va_Horodate_in and
		        HIS..ACT_GEN.heure_fin is null
		
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
			@vl_Inhibe 	= HIS..EQT_DSP.inhibe,
			@va_NumTypePicto_out	= CFG..EQT_PCT.type
		from HIS..EQT_DSP, CFG..EQT_PCT
		where	HIS..EQT_DSP.equipement = @va_NumEqt_in and 
			HIS..EQT_DSP.type = XDC_EQT_PMV and
			HIS..EQT_DSP.horodate in (select max(horodate) from HIS..EQT_DSP
						where equipement = @va_NumEqt_in and 
						type = XDC_EQT_PMV and
						horodate <= @va_Horodate_in) and
			CFG..EQT_PCT.numero = @va_NumEqt_in
	
	
	        if @@rowcount != 1
		begin
			select
			@vl_Desactive	= HIS..EQT_DSP.desactive,
			@vl_HS		= HIS..EQT_DSP.HS,
			@vl_Majeure	= HIS..EQT_DSP.majeure,
			@vl_Critique	= HIS..EQT_DSP.critique,
			@vl_Mineure	= HIS..EQT_DSP.mineure,
			@vl_Inhibe 	= HIS..EQT_DSP.inhibe,
			@va_NumTypePicto_out	= 0
		from HIS..EQT_DSP
		where	HIS..EQT_DSP.equipement = @va_NumEqt_in and 
			HIS..EQT_DSP.type = XDC_EQT_PMV and
			HIS..EQT_DSP.horodate in (select max(horodate) from HIS..EQT_DSP
						where equipement = @va_NumEqt_in and 
						type = XDC_EQT_PMV and
						horodate <= @va_Horodate_in) 
	
			/*! erreur dans la base */
			if @@rowcount != 1
			return XDC_NOK
		end

		/*B calcul de l'etat de l'equipement */
		select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
					@vl_HS * XDC_EQT_HS +
	   				@vl_Majeure * XDC_EQT_MAJEUR + 
					@vl_Critique * XDC_EQT_CRITIQUE +
	   				@vl_Inhibe * XDC_EQT_INHIBE + 
					@vl_Mineure * XDC_EQT_MINEUR

		/*B lecture de la disponibilite du picto */
		if @vl_presence_picto = 1
		begin
		select
			@vl_Desactive = desactive,
			@vl_HS = HS,
			@vl_Majeure = majeure,
			@vl_Critique = critique,
			@vl_Mineure = mineure,
			@vl_Inhibe = inhibe
		from HIS..EQT_DSP
		where	equipement = @va_NumEqt_in and type = XDC_EQT_PCT and
			horodate in (	select max(horodate) from HIS..EQT_DSP
					where	equipement = @va_NumEqt_in and 
						type = XDC_EQT_PCT and
						horodate <= @va_Horodate_in)
			if @@rowcount != 1
		                return XDC_NOK
		end
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */

	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	/*A la base EXP (au niveau du CI)                       */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAT05
			@va_Horodate_in, @va_NumEqt_in,
			@va_DispoPMV_out	output, @va_DispoPicto_out	output,
			@va_TypePMV_out		output, @va_Ligne1_out		output,
			@va_Ligne2_out		output, @va_Ligne3_out		output,
			@va_Alternat1_out	output, @va_Alternat2_out	output,
			@va_Alternat3_out	output, @va_Clignotement1_out	output,
			@va_Clignotement2_out	output, @va_Clignotement3_out	output,
			@va_Flash_out		output, @va_TypePicto_out	output,
			@va_TextePicto_out	output, @va_NumTypePicto_out	output,
			@va_ClignotementPicto_out	output,	@va_NumEvt_out	output,
			@va_CleEvt_out		output, @va_Priorite_out	output,
			@va_DistanceEvt_out	output, @va_Action_out		output,
			@va_SiteAction_out	output, @va_LigneDir_out	output,
			@va_AlternatDir_out	output
       		if @vl_Status = null
                     	return XDC_PRC_INC
           	else return @vl_Status
	end
	else
	begin 
		/* Correction JIRA SAE-502 */
		set rowcount 1
		/*B lecture des infos sur le PMV */
		select 
			@va_Ligne1_out		= EXP..ACT_PMV.premiere_ligne,
			@va_Ligne2_out		= EXP..ACT_PMV.deuxieme_ligne,
			@va_Ligne3_out		= EXP..ACT_PMV.troisieme_ligne,
			@va_Alternat1_out	= EXP..ACT_PMV.premiere_ligne_alternat,
			@va_Alternat2_out	= EXP..ACT_PMV.deuxieme_ligne_alternat,
			@va_Alternat3_out	= EXP..ACT_PMV.troisieme_ligne_alternat,
			@va_TypePicto_out	= EXP..ACT_PMV.pictogramme,
			@va_TextePicto_out	= EXP..ACT_PMV.bandeau,
			@va_Clignotement1_out	= EXP..ACT_PMV.clignotement1,
			@va_Clignotement2_out	= EXP..ACT_PMV.clignotement2,
			@va_Clignotement3_out	= EXP..ACT_PMV.clignotement3,
			@va_ClignotementPicto_out	= EXP..ACT_PMV.clignotement_picto,
			@va_Flash_out		= EXP..ACT_PMV.flash,
			@va_NumEvt_out		= EXP..ACT_GEN.evenement,
			@va_CleEvt_out		= EXP..ACT_GEN.cle,
			@va_Priorite_out	= EXP..ACT_GEN.priorite,
			@va_DistanceEvt_out	= EXP..ACT_GEN.distance_evt,
			@va_Action_out		= EXP..ACT_GEN.numero,
			@va_SiteAction_out	= EXP..ACT_GEN.sit,
			@va_LigneDir_out	= EXP..ACT_PMV.direction_ligne,
			@va_AlternatDir_out	= EXP..ACT_PMV.direction_ligne_alternat
		from EXP..ACT_PMV, EXP..ACT_GEN
		where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
			EXP..ACT_GEN.type = XDC_ACT_PMV and
			EXP..ACT_PMV.actio = EXP..ACT_GEN.numero and 
			EXP..ACT_PMV.sit = EXP..ACT_GEN.sit and
			(EXP..ACT_GEN.heure_succes <= @va_Horodate_in OR EXP..ACT_GEN.heure_lancement <= @va_Horodate_in) and
			(EXP..ACT_GEN.heure_fin is null or 
			 EXP..ACT_GEN.heure_fin > @va_Horodate_in) 
		order by EXP..ACT_PMV.actio desc
	
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
			@vl_Inhibe 	= EXP..EQT_DSP.inhibe,
			@va_NumTypePicto_out	= CFG..EQT_PCT.type
		from EXP..EQT_DSP, CFG..EQT_PCT
		where	EXP..EQT_DSP.equipement = @va_NumEqt_in and 
			EXP..EQT_DSP.type = XDC_EQT_PMV and
			EXP..EQT_DSP.horodate in (select max(horodate) from EXP..EQT_DSP
						where equipement = @va_NumEqt_in and 
						type = XDC_EQT_PMV and
						horodate <= @va_Horodate_in) and
			CFG..EQT_PCT.numero = @va_NumEqt_in
	
	
	        if @@rowcount != 1
		begin
			select
			@vl_Desactive	= EXP..EQT_DSP.desactive,
			@vl_HS		= EXP..EQT_DSP.HS,
			@vl_Majeure	= EXP..EQT_DSP.majeure,
			@vl_Critique	= EXP..EQT_DSP.critique,
			@vl_Mineure	= EXP..EQT_DSP.mineure,
			@vl_Inhibe 	= EXP..EQT_DSP.inhibe,
			@va_NumTypePicto_out	= 0
		from EXP..EQT_DSP
		where	EXP..EQT_DSP.equipement = @va_NumEqt_in and 
			EXP..EQT_DSP.type = XDC_EQT_PMV and
			EXP..EQT_DSP.horodate in (select max(horodate) from EXP..EQT_DSP
						where equipement = @va_NumEqt_in and 
						type = XDC_EQT_PMV and
						horodate <= @va_Horodate_in) 
	
			/*! erreur dans la base */
			if @@rowcount != 1
				return XDC_NOK
		end

		/*B calcul de l'etat de l'equipement */
		select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
					@vl_HS * XDC_EQT_HS +
	   				@vl_Majeure * XDC_EQT_MAJEUR + 
					@vl_Critique * XDC_EQT_CRITIQUE +
	   				@vl_Inhibe * XDC_EQT_INHIBE + 
					@vl_Mineure * XDC_EQT_MINEUR

		/*B lecture de la disponibilite du picto */
		if @vl_presence_picto = 1
		begin
		select
			@vl_Desactive = desactive,
			@vl_HS = HS,
			@vl_Majeure = majeure,
			@vl_Critique = critique,
			@vl_Mineure = mineure,
			@vl_Inhibe = inhibe
		from EXP..EQT_DSP
		where	equipement = @va_NumEqt_in and type = XDC_EQT_PCT and
			horodate in (	select max(horodate) from EXP..EQT_DSP
					where	equipement = @va_NumEqt_in and 
						type = XDC_EQT_PCT and
						horodate <= @va_Horodate_in)
			if @@rowcount != 1
			                return XDC_NOK
		end
	end
#else
	/*! cas ou la procedure est creee sur le serveur d'un district */

	/*A lecture des infos sur le PMV */
	select 
		@va_Ligne1_out		= EXP..ACT_PMV.premiere_ligne,
		@va_Ligne2_out		= EXP..ACT_PMV.deuxieme_ligne,
		@va_Ligne3_out		= EXP..ACT_PMV.troisieme_ligne,
		@va_Alternat1_out	= EXP..ACT_PMV.premiere_ligne_alternat,
		@va_Alternat2_out	= EXP..ACT_PMV.deuxieme_ligne_alternat,
		@va_Alternat3_out	= EXP..ACT_PMV.troisieme_ligne_alternat,
		@va_TypePicto_out	= EXP..ACT_PMV.pictogramme,
		@va_TextePicto_out	= EXP..ACT_PMV.bandeau,
		@va_Clignotement1_out	= EXP..ACT_PMV.clignotement1,
		@va_Clignotement2_out	= EXP..ACT_PMV.clignotement2,
		@va_Clignotement3_out	= EXP..ACT_PMV.clignotement3,
		@va_ClignotementPicto_out	= EXP..ACT_PMV.clignotement_picto,
		@va_Flash_out		= EXP..ACT_PMV.flash,
		@va_NumEvt_out		= EXP..ACT_GEN.evenement,
		@va_CleEvt_out		= EXP..ACT_GEN.cle,
		@va_Priorite_out	= EXP..ACT_GEN.priorite,
		@va_DistanceEvt_out	= EXP..ACT_GEN.distance_evt,
		@va_Action_out		= EXP..ACT_GEN.numero,
		@va_SiteAction_out	= EXP..ACT_GEN.sit,
		@va_LigneDir_out	= EXP..ACT_PMV.direction_ligne,
		@va_AlternatDir_out	= EXP..ACT_PMV.direction_ligne_alternat
	from EXP..ACT_PMV, EXP..ACT_GEN
	where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
		EXP..ACT_GEN.type = XDC_ACT_PMV and
		EXP..ACT_PMV.actio = EXP..ACT_GEN.numero and 
		EXP..ACT_PMV.sit = EXP..ACT_GEN.sit and
		EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
		(EXP..ACT_GEN.heure_fin is null or 
		 EXP..ACT_GEN.heure_fin > @va_Horodate_in) 

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
		@vl_Inhibe 	= EXP..EQT_DSP.inhibe,
		@va_NumTypePicto_out	= CFG..EQT_PCT.type
	from EXP..EQT_DSP, CFG..EQT_PCT
	where	EXP..EQT_DSP.equipement = @va_NumEqt_in and 
		EXP..EQT_DSP.type = XDC_EQT_PMV and
		EXP..EQT_DSP.horodate in (select max(horodate) from EXP..EQT_DSP
					where equipement = @va_NumEqt_in and 
					type = XDC_EQT_PMV and
					horodate <= @va_Horodate_in) and
		CFG..EQT_PCT.numero = @va_NumEqt_in

        if @@rowcount != 1
	begin
	select
		@vl_Desactive	= EXP..EQT_DSP.desactive,
		@vl_HS		= EXP..EQT_DSP.HS,
		@vl_Majeure	= EXP..EQT_DSP.majeure,
		@vl_Critique	= EXP..EQT_DSP.critique,
		@vl_Mineure	= EXP..EQT_DSP.mineure,
		@vl_Inhibe 	= EXP..EQT_DSP.inhibe,
		@va_NumTypePicto_out	= 0
	from EXP..EQT_DSP
	where	EXP..EQT_DSP.equipement = @va_NumEqt_in and 
		EXP..EQT_DSP.type = XDC_EQT_PMV and
		EXP..EQT_DSP.horodate in (select max(horodate) from EXP..EQT_DSP
					where equipement = @va_NumEqt_in and 
					type = XDC_EQT_PMV and
					horodate <= @va_Horodate_in) 
		/*! erreur dans la base */
		if @@rowcount != 1
			return XDC_NOK
	end

	/*A calcul de l'etat de l'equipement */
	select @va_DispoPMV_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
				@vl_HS * XDC_EQT_HS +
	   			@vl_Majeure * XDC_EQT_MAJEUR + 
				@vl_Critique * XDC_EQT_CRITIQUE +
	   			@vl_Inhibe * XDC_EQT_INHIBE + 
				@vl_Mineure * XDC_EQT_MINEUR

	/*A lecture de la disponibilite du picto */
	if @vl_presence_picto = 1
	begin
	select
		@vl_Desactive = desactive,
		@vl_HS = HS,
		@vl_Majeure = majeure,
		@vl_Critique = critique,
		@vl_Mineure = mineure,
		@vl_Inhibe = inhibe
	from EXP..EQT_DSP
	where	equipement = @va_NumEqt_in and type = XDC_EQT_PCT and
		horodate in (	select max(horodate) from EXP..EQT_DSP
				where	equipement = @va_NumEqt_in and 
					type = XDC_EQT_PCT and
					horodate <= @va_Horodate_in)
	if @@rowcount != 1
		return XDC_NOK
	end
#endif
#endif

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
			@va_Alternat1_out	= XDC_CHAINE_VIDE,
			@va_Alternat2_out	= XDC_CHAINE_VIDE,
			@va_Alternat3_out	= XDC_CHAINE_VIDE,
			@va_TypePicto_out	= XDC_PICTO_PASPICTO,
			@va_TextePicto_out	= XDC_CHAINE_VIDE,
			@va_Clignotement1_out	= XDC_NON,
			@va_Clignotement2_out	= XDC_NON,
			@va_Clignotement3_out	= XDC_NON,
			@va_ClignotementPicto_out = XDC_NON,
			@va_Flash_out		= XDC_NON,
			@va_NumEvt_out 		= null, 
			@va_CleEvt_out 		= null,
			@va_LigneDir_out	= XDC_CHAINE_VIDE,
			@va_AlternatDir_out	= XDC_CHAINE_VIDE

	/*A calcul de l'etat de l'equipement */
	if @vl_presence_picto = 1
	begin
	select @va_DispoPicto_out = @vl_Desactive * XDC_EQT_DESACTIVE + 
				@vl_HS * XDC_EQT_HS +
	   			@vl_Majeure * XDC_EQT_MAJEUR + 
				@vl_Critique * XDC_EQT_CRITIQUE +
	   			@vl_Inhibe * XDC_EQT_INHIBE + 
				@vl_Mineure * XDC_EQT_MINEUR
	end

	select @va_Ligne1_out, @va_Ligne2_out, @va_Ligne3_out, 
		@va_Alternat1_out, @va_Alternat2_out, @va_Alternat3_out, 
		@va_TypePicto_out, @va_TextePicto_out, 
		@va_LigneDir_out, @va_AlternatDir_out
	return XDC_OK
go
