/*E*/
/*Fichier : $Id: xzap25sp.prc,v 1.1 1999/02/26 11:25:18 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:25:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	25 Nov 1998	: Creation
* GGY		26/09/23 	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Fournit le niveau de trafic pour une localisation et une horodate donnee 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;25

	@va_Autoroute_in		smallint,
	@va_PR_in		T_PR,
	@va_Sens_in		tinyint,
	@va_Horodate_in		datetime,
	@va_Trafic_out		tinyint	output
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_Autoroute_in
* XDY_PR 		va_PR_in
* XDY_Sens 		va_Sens_in
* XDY_Horodate 		va_horodate_in
*
* ARGUMENTS EN SORTIE :
*
* XDC_Octet va_Trafic_out
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Fournit le niveau de trafic pour une localisation et une horodate donnee 
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap25sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap25sp.prc	1.1 08/25/94 : xzap25sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
declare @vl_Status int,
		@vl_portion smallint,
		@vl_station smallint,
		@vl_Site tinyint

if @va_Horodate_in = XDC_DATE_NULLE
	select @va_Horodate_in = null

#ifdef HIST

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;25
			@va_Autoroute_in	,
			@va_PR_in		,
			@va_Sens_in		,
			@va_Horodate_in		,
			@va_Trafic_out		output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;25
			@va_Autoroute_in	,
			@va_PR_in		,
			@va_Sens_in		,
			@va_Horodate_in		,
			@va_Trafic_out		output

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   
   if @va_Autoroute_in = null
      or @va_PR_in = null
      or @va_Sens_in = null
   begin  
	return XDC_ARG_INV
   end
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition "export intergestionnaire" d'un plan d'action
   */
   
   begin
      
   
 	select @vl_station= station       
 	from CFG..RES_INF INF, CFG..RES_POR POR
 	where portion=numero and
 		sens=@va_Sens_in and
 		INF.PR_debut <= @va_PR_in and INF.PR_fin > @va_PR_in and
 		autoroute=@va_Autoroute_in and
 		POR.PR_debut <= @va_PR_in and POR.PR_fin > @va_PR_in
       
       if @va_Horodate_in != null
       begin
		select @va_Trafic_out=niveau 
		from EXP..MES_NIV where station=@vl_station and sens=@va_Sens_in and horodate > dateadd(minute, -6, @va_Horodate_in) 
		group by station,sens 
		having horodate=max(horodate)
	
		if @va_Trafic_out=null
			select @va_Trafic_out=XDC_RADT_NIV_INCONNU
	end
	else
	begin
		select @va_Trafic_out=niveau
		from EXP..MES_NIV_DER where station=@vl_station and sens=@va_Sens_in

		if @va_Trafic_out=null
			select @va_Trafic_out=XDC_RADT_NIV_INCONNU
	end

        
      return XDC_OK
      
   end   
         
#endif
go
