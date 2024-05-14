/*E*/
/*  Fichier : $Id: xzac126sp.prc,v 1.2 2019/01/17 18:05:23 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2019/01/17 18:05:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac72sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Creation action "exporter intergestionnaire"
------------------------------------------------------
* HISTORIQUE :
*
* JMG	30/07/18 creation LOT25 RVA DEM1303 1.1
* LCL	08/01/19 : activation fonction 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC126_Premier_RVA
*
* Arguments en entree
* XDY_Entier		va_NumeroEvt_in
* XDY_Octet		va_CleEvt_in
*
* Arguments en sortie
* XDY_Entier		va_premier_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Creation action "exporter intergestionnaire"
-------------------------------------------------------*/

use PRC
go

drop procedure XZAC126
go

create procedure XZAC126
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_premier_out		int			output
	
as
		select  @va_premier_out = 0

	/*A verifier l'existance de l'action TFM */
	if not exists ( select * from EXP..ACT_GEN 
			where evenement = @va_NumEvt_in and cle = @va_CleEvt_in
			and type = XDC_ACT_TFM)
	begin
		select  @va_premier_out = 1
	end

	return XDC_OK
go
