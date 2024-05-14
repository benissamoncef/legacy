/*E*/
/*  Fichier : $Id: asql_t24_mes_niv.trg,v 1.1 1999/02/26 11:29:32 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:29:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  VEHEDD
------------------------------------------------------
* FICHIER asql_t24_mes_niv.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table MES_NIV
*
------------------------------------------------------
* HISTORIQUE :
*
* PN	01/12/98        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* met a jour la table MES_NIV_DER
* 
*
* Code retour
* 
* n≈ant
*
* Conditions d'utilisation
* base EXP 
---------------------------------------------------*/

use EXP
go

#ifdef CI
drop trigger mes_niv_iu_trg
go

create trigger mes_niv_iu_trg
on MES_NIV
for insert,update
as

/*A D≈claration des variables locales */

	declare @vl_station smallint, @vl_sens tinyint, @vl_niveau tinyint

/*A Recuperation des valeurs precedente et mise a jour */
	select @vl_station=station, 
		@vl_sens=sens,
		@vl_niveau=niveau
	from inserted
	
	if @@rowcount !=0
	begin
		if exists ( select * from MES_NIV_DER where station=@vl_station and sens=@vl_sens)
		begin
			update MES_NIV_DER 
			set niveau=@vl_niveau
			where station=@vl_station and sens=@vl_sens
		end
		else
		begin
			insert MES_NIV_DER (station,sens,niveau)
			values (@vl_station,@vl_sens,@vl_niveau)
		end
	end
go

#endif
