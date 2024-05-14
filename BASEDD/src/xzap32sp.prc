/*E*/
/*X  Fichier : $Id: xzap32sp.prc,v 1.2 2014/10/08 12:42:27 gesconf Exp $    Release : $Revision: 1.2 $       Date : $Date: 2014/10/08 12:42:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap32sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	30/08/05	: Creation
* JPL	08/10/14	: Suppression procedure avant compilation  1.2
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition BAD d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP32' and type = 'P')
	drop procedure XZAP32
go


create procedure XZAP32

	@va_NumPA_in		smallint,
	@va_Ordre_in		tinyint
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
*
* ARGUMENTS EN SORTIE :
*
* XZAPT_PropNAV 	va_PropNAV_out
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
* Lit une proposition de positionnement NAV d'un plan d'actions
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
		exec @vl_Status = SQL_CI.PRC..XZAP32
			@va_NumPA_in		,
			@va_Ordre_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP32
			@va_NumPA_in		,
			@va_Ordre_in

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   declare 	@vl_priorite tinyint,
   		@vl_distance int,
   		@vl_evt int,
   		@vl_cle tinyint,
   		@vl_eqt smallint
   
   if @va_NumPA_in = null
   or @va_Ordre_in = null
  
      return XDC_ARG_INV
  
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition NAV d'un plan d'action
   */
   
   begin
      
      
      select	@vl_evt=evenement,
      		@vl_cle=cle
      from	EXP..SAD_PLN
      where 	numero = @va_NumPA_in
      
      select	@vl_eqt=equipement
      from EXP..SAD_BAD
      where numero = @va_NumPA_in and ordre = @va_Ordre_in
      
      select	@vl_priorite=priorite,
      		@vl_distance=distance
      from EXP..EQT_FIL
      where	evenement=@vl_evt
      	and	cle=@vl_cle
      	and	type_equipement=XDC_EQT_BAD
      	and	equipement=@vl_eqt

      select 	remarque, 
   		equipement,
      		explication,
		scenario,
		isnull(@vl_priorite,0),
   		isnull(@vl_distance,0)
   	
      from EXP..SAD_BAD
   
        where numero = @va_NumPA_in and ordre = @va_Ordre_in 
   	
      return XDC_OK
      
   end   
         
#endif
go
