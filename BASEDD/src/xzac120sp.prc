/*E*/
/*Fichier :  $Id: xzac120sp.prc,v 1.4 2010/09/02 09:49:19 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2010/09/02 09:49:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac120sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JBL	10/12/07	: Creation DEM 663
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.3
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage tunnel
* 
* Sequence d'appel
* SP	XZAC120_Lire_Commande_Signalisation
* 
* Arguments en entree
* XDY_Entier	va_NumeroAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* XDY_Booleen	va_Manuelle_out
* XDY_Octet	va_Sequence_out
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
* Select des tables ACTION et AFFICHAGE_TUNNEL
-----------------------------------------------------*/

use PRC
go

drop procedure XZAC120
go

create procedure XZAC120
	@va_NumeroAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureEchec_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output,
	@va_Manuelle_out		bit		= XDC_NON	output,
	@va_Sequence_out		tinyint		= null		output
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
		@va_Manuelle_out		= HIS..ACT_GTC_SIG.manuelle,
		@va_Sequence_out		= HIS..ACT_GTC_SIG.sequence
	from HIS..ACT_GTC_SIG, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumeroAction_in and 
		(HIS..ACT_GEN.type = XDC_ACT_GTC_SIG or HIS..ACT_GEN.type = XDC_ACT_GTC_SIG_ECH) and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_GTC_SIG.actio = @va_NumeroAction_in and 
		HIS..ACT_GTC_SIG.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount
	
#else

	select
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin,
		@va_Manuelle_out		= EXP..ACT_GTC_SIG.manuelle,
		@va_Sequence_out		= EXP..ACT_GTC_SIG.sequence
	from EXP..ACT_GTC_SIG, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		(EXP..ACT_GEN.type = XDC_ACT_GTC_SIG or EXP..ACT_GEN.type = XDC_ACT_GTC_SIG_ECH) and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_GTC_SIG.actio = @va_NumeroAction_in and 
		EXP..ACT_GTC_SIG.sit = @va_Site_in
	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC120
					@va_NumeroAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output,
					@va_Manuelle_out		output,
					@va_Sequence_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
	begin
		/* evite une erreur sybase */
		select @va_Manuelle_out = XDC_NON

		return XDC_NOK
	end

	return XDC_OK
go
