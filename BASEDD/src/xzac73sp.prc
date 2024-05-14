/*E*/
/*  Fichier : $Id: xzac73sp.prc,v 1.4 2020/10/09 14:17:47 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/10/09 14:17:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac73sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* Mise � jour etat de l' action "exporter intergestionnaire"
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	27/10/1998 : creation
* JMG	04/06/04 : plus de gestion d action d export v1.2
* ama	09/10/20 : ajout du drop
* LCL	28/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC73_Ecrire_Exporter
*
* Arguments en entree
* XDY_Entier		va_NumeroFMC_in
* XDY_Entier		va_CleFMC_in
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
* Creation action "exporter intergestionnaire"
-------------------------------------------------------*/

use PRC
go
if exists (select * from sysobjects where name = 'XZAC73' and type = 'P')
        drop procedure XZAC73
go
create procedure XZAC73
	@va_NumeroFMC_in		int		= null,
	@va_CleFMC_in			tinyint		= null,
	@va_Heure_in			datetime	= null,
	@va_TypeHorodate_in		tinyint		= null,
	@vl_SiteCI  			tinyint
	
as
declare @vl_NumeroAction int,
	@vl_HeureSucces datetime,
	@vl_Fin	datetime
return XDC_OK 
	if @va_Heure_in = null or @va_NumeroFMC_in=null or @va_CleFMC_in=null or
	   ( @va_TypeHorodate_in <> XDC_ACT_ETAT_SUCCES and
	     @va_TypeHorodate_in <> XDC_ACT_ETAT_FIN and
	     @va_TypeHorodate_in <> XDC_ACT_ETAT_ECHEC )
	  return XDC_ARG_INV

	select @vl_SiteCI=numero from CFG..RES_DIS where type=XDC_TYPEM_SD
	  
	select @vl_NumeroAction=numero,@vl_HeureSucces=heure_succes from EXP..ACT_GEN 
	where sit=@vl_SiteCI and evenement=@va_NumeroFMC_in and cle=@va_CleFMC_in and type=XDC_ACT_EXP
	order by heure_lancement
	
	if @vl_NumeroAction<>null
	begin
		if ( @va_TypeHorodate_in = XDC_ACT_ETAT_SUCCES) 
		begin
			update EXP..ACT_GEN
			set heure_echec=null
			where numero=@vl_NumeroAction and sit=@vl_SiteCI
			
			if @vl_HeureSucces=null
				update EXP..ACT_GEN
				set heure_succes=@va_Heure_in
				where numero=@vl_NumeroAction and sit=@vl_SiteCI
			
		end
		else
		if ( @va_TypeHorodate_in = XDC_ACT_ETAT_FIN)
		begin 
			update EXP..ACT_GEN
			set heure_fin=@va_Heure_in
			where numero=@vl_NumeroAction and sit=@vl_SiteCI
			
			/* S'il existe un export TFM en cours et evt non fin  alors la terminer */
			select @vl_NumeroAction=numero from EXP..ACT_GEN 
			where sit=@vl_SiteCI and evenement=@va_NumeroFMC_in and cle=@va_CleFMC_in and type=XDC_ACT_EXPTFM and heure_fin=null
			order by heure_lancement
			if @vl_NumeroAction<>null
			begin
				/* Si l'evt n'est pas fini, forcer la fin de l'esport TFM */
				select @vl_Fin=fin from EXP..FMC_GEN
				where numero=@va_NumeroFMC_in and cle=@va_CleFMC_in
				if @vl_Fin=null
				begin
					update EXP..ACT_GEN
					set heure_fin=@va_Heure_in
					where numero=@vl_NumeroAction and sit=@vl_SiteCI
				end
			end
		end
		else
		if ( @va_TypeHorodate_in = XDC_ACT_ETAT_ECHEC) 
			update EXP..ACT_GEN
			set heure_echec=@va_Heure_in
			where numero=@vl_NumeroAction and sit=@vl_SiteCI
		
	end
	else
		return XDC_NOK
		
	return XDC_OK
go

