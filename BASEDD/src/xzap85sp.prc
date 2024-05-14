/*E*/
/* Fichier : $Id: xzap85sp.prc,v 1.2 2021/07/20 11:14:03 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2021/07/20 11:14:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap85sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	19/10/20 	: Creation pour IMU 1.1 DEM-SAE155
* ABE	15/07/21	: Modif pour tdrol IM DEM-SAE155 1.2
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAP85' and type = 'P')
	drop procedure XZAP85
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'affichage IMU d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP85

	@va_NumPA_in		smallint,
	@va_Ordre_in		smallint								
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in								
*
* ARGUMENTS EN SORTIE :
*
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
* Lit une proposition d'affichage IMU d'un plan d'actions
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
		exec @vl_Status = SQL_CI.PRC..XZAP85
			@va_NumPA_in,
			@va_Ordre_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP85
			@va_NumPA_in,
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
   ** Lecture d'une Proposition IMU d'un plan d'action
   */
   
   begin
      
      select	numero,
		ordre,
		district,
		equipement,
		priorite,
		remarque,
		explication,
		realisable,
		chaine,
		intervalle,
		cycles,
		sit,
		duree,
		libelle	 							
      from EXP..SAD_IMU
      where numero = @va_NumPA_in and ordre = @va_Ordre_in
   	
      return XDC_OK
      
   end   
         
#endif
go
