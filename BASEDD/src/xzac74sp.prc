/*E*/
/*  Fichier : $Id: xzac74sp.prc,v 1.2 2010/08/30 15:48:19 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/08/30 15:48:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac74sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Lecture action "exporter intergestionnaire"
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	27/10/1998 : creation
* JPL		21/07/2010 : Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC74_Lire_Exporter
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
* Lecture action "exporter intergestionnaire"
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC74' and type = 'P')
	drop procedure XZAC74
go

create procedure XZAC74
	@va_NumeroAction_in		int		= null,
	@va_Site_in			tinyint	= null,
	@va_Gravite_out			tinyint= null	output,
	@va_Trafic_out			tinyint= null	output,
	@va_HeureFin_out		datetime=null	output
as
	declare	@vl_Status		int

#ifdef HIST

	select	@va_Gravite_out = gravite,
		@va_Trafic_out = trafic
	from	HIS..ACT_EXP_ADA
	where	actio = @va_NumeroAction_in and
		sit = @va_Site_in

	if @@rowcount != 1
		return XDC_NOK

	select	@va_HeureFin_out = heure_fin
	from	HIS..ACT_GEN
	where	numero = @va_NumeroAction_in and
		sit = @va_Site_in

#else

	select	@va_Gravite_out = gravite,
		@va_Trafic_out = trafic
	from	EXP..ACT_EXP_ADA
	where	actio = @va_NumeroAction_in and
		sit = @va_Site_in

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @@rowcount = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC74
					@va_NumeroAction_in,
					@va_Site_in,
					@va_Gravite_out		output,
					@va_Trafic_out		output,
					@va_HeureFin_out	output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end


	select	@va_HeureFin_out = heure_fin
	from	EXP..ACT_GEN
	where	numero = @va_NumeroAction_in and
		sit = @va_Site_in

#endif		

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
