/*X  Fichier : $Id: xzae30sp.prc,v 1.1 1997/05/21 11:26:27 verdier Exp $   Release : $Revision: 1.1 $        Date : $Date: 1997/05/21 11:26:27 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae30sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/96	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* recherche la presence d'un basculement consequence
* d'un travaux
*
* Sequence d'appel
* SP    XZAE30_Cherche_Basculement_Consequence
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* au CI uniquement
*
* Fonction
---------------------------------------------------*/

use PRC
go

create procedure XZAE;30
	@va_NumEvt_in	int = null,
	@va_CleEvt_in	tinyint = null
as
	declare @vl_Type smallint, @vl_Cause int

	if (exists (	select * from EXP..FMC_GEN
			where type = XZAEC_FMC_Basculement
			and   cause= @va_NumEvt_in 
			and   cle_cause = @va_CleEvt_in
			and   cloture is null))
	begin
		return XZAEC_FMC_NON_FIN
	end

	return XDC_OK
go
