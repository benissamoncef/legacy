/*E*/
/*  Fichier : $Id: xzae982sp.prc,v 1.1 2012/05/29 10:16:28 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2012/05/29 10:16:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae981sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* mise a jour d un evenements communautaire
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU	10/02/12 : Creation	 DEM/1015
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* creation l'evenement communautaire
* 
* Sequence d'appel
* SP	XZAE981_Update_Communautaire
* 
* Arguments en entree
* XDY_Entier          va_NumeroEvt_in : numero FMC
* XDY_Octet           va_CleEvt_in    : cle FMC
* XDY_Tendance_Comm   va_Tendance_in  : tendance
* XDY_Synthese_Commu  va_Synthese_in  : synthese
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: pas de FMC liee
* XDC_ARG_INV		: parametres d'entree null
* 
* Conditions d'utilisation
*
* Fonction
	@va_NumeroEvt_in   int = null ,
	@va_CleEvt_in	tinyint	= null,
	@va_tendance_in    int = null ,	
	@va_synthese_in    int = null 

-------------------------------------------------------*/

use PRC
go
drop procedure XZAE982
go
create procedure XZAE982
	@va_NumeroEvt_in    int = null,
	@va_CleEvt_in	tinyint	= null,
	@va_id_commu_in	int	= null,
	@va_tendance_in     int = null,	
	@va_synthese_in     int = null
as
	
	/*A controle parametres en entree */
	if @va_NumeroEvt_in = null or @va_CleEvt_in = null or @va_tendance_in = null or @va_synthese_in = null or @va_id_commu_in = null
		return XDC_ARG_INV

	/*A recherche l'alerte communautaire */
	insert into EXP..FMC_COMMU (evenement, cle, id_commu, tendance, synthese)
	values(@va_NumeroEvt_in,@va_CleEvt_in,@va_id_commu_in,@va_tendance_in,@va_synthese_in)

	if @@rowcount = 0
		return XDC_NOK
	return XDC_OK
go
