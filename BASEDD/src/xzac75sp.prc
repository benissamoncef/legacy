/*E*/
/*  Fichier : $Id: xzac75sp.prc,v 1.2 2020/10/09 14:15:13 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/10/09 14:15:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac75sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Mise � jour etat de l' action "exporter TFM"
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	27/10/1998 : creation
* RGR		09/10/2020 : Ajout drop
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC75_Ecrire_ExporterTFM
*
* Arguments en entree
* XDY_Entier		va_NumeroAction_in
* XDY_Horodate		va_Heure_in
* XDY_Octet		va_TypeHorodate_in = (XDC_ACT_ETAT_SUCCES, XDC_ACT_ETAT_ECHEC, XDC_ACT_ETAT_FIN)
*
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Creation action "exporter TFM"
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC75' and type = 'P')
  drop procedure XZAC75
go

create procedure XZAC75
	@va_NumeroAction_in		int		= null,
	@va_Heure_in			datetime	= null,
	@va_TypeHorodate_in		tinyint		= null
	
as
declare @vl_action int,
		@vl_SiteCI  tinyint
 
	if @va_Heure_in = null or @va_NumeroAction_in=null or
	   ( @va_TypeHorodate_in <> XDC_ACT_ETAT_SUCCES and
	     @va_TypeHorodate_in <> XDC_ACT_ETAT_FIN and
	     @va_TypeHorodate_in <> XDC_ACT_ETAT_ECHEC )
	  return XDC_ARG_INV
	
	select @vl_SiteCI=numero from CFG..RES_DIS where type=XDC_TYPEM_SD
	
	select   @vl_action=numero 
	from EXP..ACT_GEN , EXP..ACT_TFM_EXP
	where actio_tfm=@va_NumeroAction_in and
		numero=actio and
		EXP..ACT_GEN.sit=EXP..ACT_TFM_EXP.sit and
		EXP..ACT_GEN.sit=@vl_SiteCI and 
		type=XDC_ACT_EXPTFM
		
	if @@rowcount!=0
	begin
		if ( @va_TypeHorodate_in = XDC_ACT_ETAT_SUCCES) 
			update EXP..ACT_GEN
			set heure_succes=@va_Heure_in,heure_echec=null
			where numero=@vl_action and sit=@vl_SiteCI and type=XDC_ACT_EXPTFM
		else
		if ( @va_TypeHorodate_in = XDC_ACT_ETAT_FIN) 
			update EXP..ACT_GEN
			set heure_fin=@va_Heure_in,heure_echec=null
			where numero=@vl_action and sit=@vl_SiteCI and type=XDC_ACT_EXPTFM
		else
		if ( @va_TypeHorodate_in = XDC_ACT_ETAT_ECHEC) 
			update EXP..ACT_GEN
			set heure_echec=@va_Heure_in
			where numero=@vl_action and sit=@vl_SiteCI and type=XDC_ACT_EXPTFM
		
	end
	else
		return XDC_NOK
		
	return XDC_OK
go

