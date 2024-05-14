/*E*/
/*  Fichier : $Id: xzac45sp.prc,v 1.1 2012/10/29 20:26:52 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/10/29 20:26:52 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzac45sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des actions
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	26/10/12 : Creation (DEM 1032)  1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Consultation d'action d'envoi des donnees de Conditions de Conduite.
*
* SEQUENCE D'APPEL
*
* SP	XZAC45_Lire_Action_Envoi_CCH
*
* Arguments en entree :
*  XDY_Entier           va_NumEvt_in
*  XDY_Octet            va_CleEvt_in
*
* Arguments en sortie :
*  XDY_Horodate         va_HeureLancement_out
*  XDY_Horodate         va_HeureSucces_out
*  XDY_Horodate         va_HeureEchec_out
*  XDY_Horodate         va_HeureFin_out
*
* Code retour :
*  XDC_OK
*  XDC_NOK            : action inexistante
*  XDC_ARG_INV        : parametre d'entree obligatoire manquant (Null) ou invalide
*  <0                 : erreur sybase
*
* Conditions d'utilisation :
*  La procedure stockee n'existe qu'au CI et sur historique.
*  Au CI, la procedure recherche les informations dans la base HIS
*  si elles n'existent pas dans la base EXP.
*
* Fonction :
*  Verifier que l'action existe et est de type "Envoi VH".
*  Recherche dans les tables ACT_GEN et ACT_CCH.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC45' and type = 'P')
	drop procedure XZAC45
go


create procedure XZAC45
	@va_NumAction_in		int		= null,
	@va_Site_in			tinyint		= null,
	@va_HeureLancement_out		datetime	= null	output,
	@va_HeureSucces_out		datetime	= null	output,
	@va_HeureEchec_out		datetime	= null	output,
	@va_HeureFin_out 		datetime	= null	output
as
	declare	@vl_Status	int


	/*A Transformation des parametres d'entree nuls */
	if @va_NumAction_in = 0	select @va_NumAction_in	= null
	if @va_Site_in = 0	select @va_Site_in	= null

	/*A Verifier la validite des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV


	/*A Lecture des infos sur une action d'envoi de Conditions de Conduite */

#ifdef HIST

	select	@va_HeureLancement_out	= heure_lancement,
		@va_HeureSucces_out	= heure_succes,
		@va_HeureEchec_out	= heure_echec,
		@va_HeureFin_out	= heure_fin
	from	HIS..ACT_GEN
	where	numero = @va_NumAction_in  and  sit = @va_Site_in  and
		type = XDC_ACT_CCH

	/*A Si les informations ne sont pas trouvees */
	/*A alors les rechercher dans l'historique   */
	if @@rowcount = 0
		return XDC_NOK


	/*A Lecture des valeurs de Conditions de Conduite et des commentaires */
	select	troncon, cch, commentaire
	from	HIS..ACT_CCH
	where	actio = @va_NumAction_in  and  sit = @va_Site_in
	order by troncon

#else
#ifdef CI

	select	@va_HeureLancement_out	= heure_lancement,
		@va_HeureSucces_out	= heure_succes,
		@va_HeureEchec_out	= heure_echec,
		@va_HeureFin_out	= heure_fin
	from	EXP..ACT_GEN
	where	numero = @va_NumAction_in  and  sit = @va_Site_in  and
		type = XDC_ACT_CCH

	/*A Si les informations ne sont pas trouvees */
	/*A alors les rechercher dans l'historique   */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC45
					@va_NumAction_in,
					@va_Site_in,
					@va_HeureLancement_out	output,
					@va_HeureSucces_out	output,
					@va_HeureEchec_out	output,
					@va_HeureFin_out 	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end


	/*A Lecture des valeurs de Conditions de Conduite et des commentaires */
	select	troncon, cch, commentaire
	from	EXP..ACT_CCH
	where	actio = @va_NumAction_in  and  sit = @va_Site_in
	order by troncon

#endif
#endif

	return XDC_OK

go
