/*E*/
/*  Fichier : $Id: xzac96sp.prc,v 1.1 2018/01/09 15:36:03 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2018/01/09 15:36:03 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac96.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	27/11/17	: Creation
*
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture  des infos FMC  associees a un pilotage
* 
* Sequence d'appel
* SP	XZAC96_Lire_FMC_Data
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC96
go

create procedure XZAC96
	@va_NumeroAction_in		int		= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_numero_out			int	= null		output,
	@va_cle_out		tinyint	= null		output,
	@va_pr_queue_out	T_PR	= null		output,
	@va_pr_tete_out		T_PR	= null		output
as
	declare	@vl_NbEnrg	int,
		@vl_Status	int,
		@vl_numero_cause int, 
		@vl_cle_cause tinyint

	/*A controle des parametres obligatoires */
	if @va_NumeroAction_in = null or @va_NumEqt_in = null
		return XDC_ARG_INV

	select
		@va_numero_out		= EXP..ACT_GEN.evenement,
		@va_cle_out		= EXP..ACT_GEN.cle
	from EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
		EXP..ACT_GEN.equipement = @va_NumEqt_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		return XDC_NOK
	end

	/*recherche PRs de la FMC*/
	select @va_pr_queue_out = PR
	from EXP..FMC_HIS_DER
	where numero = @va_numero_out and cle = @va_cle_out

	/*bouchon donc on recherche le PR de la cause i.e. de la tete*/
	select @vl_numero_cause = cause,
		@vl_cle_cause = cle_cause
	from EXP..FMC_GEN
	where numero = @va_numero_out and cle = @va_cle_out

	select @va_pr_tete_out = PR
	from EXP..FMC_HIS_DER
	where numero = @vl_numero_cause and cle = @vl_cle_cause

	return XDC_OK

go
