/*E*/
/*  Fichier : $Id: xzae154sp.prc,v 1.1 1999/02/26 14:22:20 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:22:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae152sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos d'une situation strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE154_Lire_FMCDATEX
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos obligatoires pour generer
* un fichier STRADA
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE154
go

create procedure XZAE154
	@va_NumEvt_in 		int=null,
	@va_Gravite_out		tinyint=null output,
	@va_Trafic_out		tinyint=null output,
	@va_Bloquant_out	bit=XDC_FAUX output
	
as

#ifdef HIST
	select 
		@va_Gravite_out=gravite,
		@va_Trafic_out=trafic,
		@va_Bloquant_out=bloquant
	from HIS..FMC_DAT
	where numero=@va_NumEvt_in and cle = (select numero from CFG..RES_DIS where type = XDC_TYPEM_SD)
#else
	select 
		@va_Gravite_out=gravite,
		@va_Trafic_out=trafic,
		@va_Bloquant_out=bloquant
	from EXP..FMC_DAT
	where numero=@va_NumEvt_in and cle = (select numero from CFG..RES_DIS where type = XDC_TYPEM_SD)
#endif
	
	return XDC_OK
go

