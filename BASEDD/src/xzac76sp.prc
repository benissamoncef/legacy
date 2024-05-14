/*E*/
/*  Fichier : $Id: xzac76sp.prc,v 1.2 2010/08/30 15:48:21 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/08/30 15:48:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac76sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Lecture action pmv datex
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	27/10/1998 : creation
* JPL		22/07/2010 : Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC76_Lire_PMV_DATEX
*
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
*
* Arguments en sortie
* XDY_OCtet		va_Gravite_out
* XDY_OCtet		va_Trafic_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture action "exporter pmv"
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC76' and type = 'P')
	drop procedure XZAC76
go

create procedure XZAC76
	@va_NumeroAction_in		int		= null,
	@va_Site_in			tinyint	= null,
	@va_Nom_out			char(15)	output,
	@va_Texte1_out			char(255)	output,
	@va_Texte2_out			char(100)	output,
	@va_HeureDebut_out		datetime=null	output,
	@va_HeureFin_out		datetime=null	output
as
	declare	@vl_Status		int

#ifdef HIST

	select	@va_Texte1_out = texte1,
		@va_Texte2_out = texte2,
		@va_Nom_out = PMV
	from	HIS..ACT_PMV_ADA
	where	actio = @va_NumeroAction_in and
		sit = @va_Site_in

	if @@rowcount != 1
		return XDC_NOK

	select	@va_HeureDebut_out = heure_succes,
		@va_HeureFin_out = heure_fin
	from	HIS..ACT_GEN
	where	numero = @va_NumeroAction_in and
		sit = @va_Site_in

#else

	select	@va_Texte1_out = texte1,
		@va_Texte2_out = texte2,
		@va_Nom_out = PMV
	from	EXP..ACT_PMV_ADA
	where	actio = @va_NumeroAction_in and
		sit = @va_Site_in

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC76
					@va_NumeroAction_in,
					@va_Site_in,
					@va_Nom_out		output,
					@va_Texte1_out		output,
					@va_Texte2_out		output,
					@va_HeureDebut_out	output,
					@va_HeureFin_out	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end


	select	@va_HeureDebut_out = heure_succes,
		@va_HeureFin_out = heure_fin
	from	EXP..ACT_GEN
	where	numero = @va_NumeroAction_in and
		sit = @va_Site_in

#endif

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK
go
