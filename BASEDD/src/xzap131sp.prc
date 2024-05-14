/*E*/
/*Fichier : @(#)xzap131sp.prc	@(#)xzap131sp.prc	1.2      Release : 1.2        Date : 05/06/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap131sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher	29/10/04	Creation v1.1
* GGY			26/09/23 :	Ajout district CA (DEM483)
*
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP131' and type = 'P')
	drop procedure XZAP131
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*  renvoit le numero PAL et son district a partir du numero PA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP131

	@va_NumPA_in		smallint
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
*
* ARGUMENTS EN SORTIE :
*  numero equipement smallint,
*  distict du PAL    smallint
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
*
* MECANISMES :
*
------------------------------------------------------*/


as 
  
#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP131
			@va_NumPA_in		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP131
			@va_NumPA_in

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   declare
		@vl_eqt smallint
   
   if @va_NumPA_in = null
  
      return XDC_ARG_INV
  
   else
   begin
      
      
      select	
      		EXP..SAD_PAL.equipement
      from	EXP..SAD_PLN,EXP..SAD_PAL
      where 	EXP..SAD_PLN.numero = EXP..SAD_PAL.numero and
		EXP..SAD_PLN.numero = @va_NumPA_in
      
      return XDC_OK
      
   end   
         
#endif
go
