/*E*/
/* Fichier : $Id: xzai600sp.prc,v 1.3 2017/07/19 12:58:52 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2017/07/19 12:58:52 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzai600sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Tracage des messages echanges entre operateurs par la messagerie SAE.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	04/07/17 : Creation  1.1
* JPL	18/07/17 : Changement du type du destinataire en texte  1.2
* JPL	19/07/17 : Prise en compte du poste NOMADE  1.3
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Enregistre dans la table dediee une portion de texte d'un message echange
* entre operateurs avec l'horodate et les postes emetteur et destinataire.
*
* RESTRICTIONS :
*
*  - Destinee a etre executee en base HIStorique uniquement.
*
* -----------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAI600_Tracer_Message_SAE
*
* Arguments en entree
* XDY_Horodate		va_Horodate_in
* XDY_Eqt		va_Expediteur_in
* XDY_NomMachine	va_Destinataire_in
* XDY_Mot		va_Morceau_in
* XDY_Texte		va_Texte_in
*
* Arguments en sortie
*  Aucun
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase
*
* Conditions d'utilisation
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAI600' and type = 'P')
	drop procedure XZAI600
go


create procedure XZAI600
	@va_Horodate_in		datetime,
	@va_Expediteur_in	T_EQUIPEMENT,
	@va_Destinataire_in	T_NOM_MACHINE,
	@va_Morceau_in		smallint,
	@va_Texte_in		T_TEXTE
as
	declare	@vl_Num_Destinataire	T_EQUIPEMENT

	/*A Transformation des parametres d'entree nulls */
	if @va_Horodate_in = XDC_DATE_NULLE		select @va_Horodate_in = null
	if @va_Destinataire_in = XDC_CHAINE_VIDE	select @va_Destinataire_in = null


	/*A Controle des parametres d'entree obligatoire */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	if ( @va_Morceau_in = null )  or  ( @va_Morceau_in <= 0 )
		return XDC_ARG_INV

	if not exists (select 1 from CFG..EQT_GEN where type = XDC_EQT_MAC  and  numero = @va_Expediteur_in)
		return XDC_ARG_INV

	/* Determiner le poste destinataire du message */
	if @va_Destinataire_in = XDC_NOMADE
		select @vl_Num_Destinataire = XDC_NUM_NOMADE
	else
	begin
		select	@vl_Num_Destinataire = numero
		from	CFG..EQT_GEN
		where	type = XDC_EQT_MAC  and  nom = @va_Destinataire_in
		if @@rowcount = 0
			return XDC_ARG_INV
	end


	/*A Si une portion de message existe entre ces postes a cette date alors mettre a jour le texte */
	if exists (select 1 from HIS..MSG_TRA where horodate = @va_Horodate_in and
	                                            expediteur = @va_Expediteur_in  and  destinataire = @vl_Num_Destinataire and
	                                            morceau = @va_Morceau_in)
	begin
		update	HIS..MSG_TRA
		set	texte = @va_Texte_in
		where	horodate = @va_Horodate_in
		  and	expediteur = @va_Expediteur_in
		  and	destinataire = @vl_Num_Destinataire
		  and	morceau = @va_Morceau_in
	end
	else
	begin
		insert	HIS..MSG_TRA (horodate, expediteur, destinataire, morceau, texte)
		values	(@va_Horodate_in, @va_Expediteur_in, @vl_Num_Destinataire, @va_Morceau_in, @va_Texte_in)
	end


	if @@rowcount <> 1
		return XDC_NOK

	return XDC_OK
go
