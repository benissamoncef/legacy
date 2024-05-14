/*E*/
/*  Fichier : $Id: xzao801sp.prc,v 1.2 2013/09/30 11:14:26 devgtie Exp $     Release : $Revision: 1.2 $        Date : $Date: 2013/09/30 11:14:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao801sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* verifie si une sequence est autorisee pour un tunnel
*
------------------------------------------------------
* HISTORIQUE :
* JMG	15/03/09	: creation DEM895
* ABE   26/03/13	: gestion du retour � la normale avec ajout de sc�narios pour le domaine exploitation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des gares de peages
* 
* Sequence d'appel
* SP	XZAO801_Verif_Sequence_Autorisee
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_PEA
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO801' and type = 'P')
	drop procedure XZAO801
go

create procedure XZAO801
	@va_tunnel_in		int = null,
	@va_sequence_in		int = null,
	@va_domaine_in	int= null 
as
	declare @va_autorise_out        int,
		@vl_tub tinyint

	/*si tunnel non renove*/
	if @va_domaine_in = XDC_ACT_TUNNEL
	begin
		select @vl_tub = tube
		from CFG..EQT_SQA_TUB
		where tube = @va_tunnel_in
		and sequence = @va_sequence_in

		if @@rowcount = 0
		begin
			select @va_autorise_out=XDC_NOK
		end
		else
		begin
			select @va_autorise_out=XDC_OK
		end
	end
	else
	begin
		if @va_domaine_in = XDC_ACT_GTC_ECL
		begin
			select @vl_tub = equipement
			from CFG..EQT_SQA_GTC_ECL
			where equipement = @va_tunnel_in
			and sequence = @va_sequence_in
			and type = XDC_EQT_TUB
		end
		else if @va_domaine_in = XDC_ACT_GTC_EXP
		begin
			select @vl_tub = equipement
			from CFG..EQT_SQA_GTC_EXP
			where equipement = @va_tunnel_in
			and sequence = @va_sequence_in
			and type = XDC_EQT_TUB
		end
		else if @va_domaine_in = XDC_ACT_GTC_VEN
		begin
			select @vl_tub = equipement
			from CFG..EQT_SQA_GTC_VEN
			where equipement = @va_tunnel_in
			and sequence = @va_sequence_in
			and type = XDC_EQT_TUB
		end
		else
		begin
			select @vl_tub = equipement
			from CFG..EQT_SQA_GTC_SIG
			where equipement = @va_tunnel_in
			and sequence = @va_sequence_in
			and type = XDC_EQT_TUB
		end
			

		if @@rowcount = 0
		begin
			select @va_autorise_out=XDC_NOK
		end
		else
		begin
			select @va_autorise_out=XDC_OK
		end
	end
	select @va_autorise_out, @va_tunnel_in, @va_sequence_in,@va_domaine_in
	return XDC_OK
go
