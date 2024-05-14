/*E*/
/* Fichier : $Id: xzap63sp.prc,v 1.1 2012/07/04 11:36:59 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:36:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap63sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	22/12/11 	: Creation (DEM/1016)
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP63' and type = 'P')
	drop procedure XZAP63
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'affichage PRV d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP63

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
* XZAPT_PropPMV 	va_PropPMV_out
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
* Lit une proposition d'affichage PMV d'un plan d'actions
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
		exec @vl_Status = SQL_CI.PRC..XZAP63
			@va_NumPA_in		,
			@va_Ordre_in		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP63
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
   ** Lecture d'une Proposition PMV d'un plan d'action
   */
   
   begin
      
      select	@vl_evt=evenement,
      		@vl_cle=cle
      from	EXP..SAD_PLN
      where 	numero = @va_NumPA_in
      
      select	@vl_eqt=equipement
      from EXP..SAD_PRV
      where numero = @va_NumPA_in and ordre = @va_Ordre_in
      
      select	@vl_priorite=priorite,
      		@vl_distance=distance
      from EXP..EQT_FIL
      where	evenement=@vl_evt
      	and	cle=@vl_cle
      	and	type_equipement=XDC_EQT_PRV
      	and	equipement=@vl_eqt
     
      select remarque, 
			equipement,
      		explication,
			bandeau,
			vitesse,
   		isnull(priorite,0),
   		isnull(@vl_distance,0)  
   	
      from EXP..SAD_PRV
   
        where numero = @va_NumPA_in and ordre = @va_Ordre_in 
   	
      return XDC_OK
      
   end   
         
#endif
go
