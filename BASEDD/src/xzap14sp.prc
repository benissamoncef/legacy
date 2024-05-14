/*E*/
/*Fichier : $Id: xzap14sp.prc,v 1.4 2009/10/22 15:51:47 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2009/10/22 15:51:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* gaborit	16 mar 1995	: modif priorite+distance
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.3
* JMG		22/10/09	: ajout domaine DEM895 1.4
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition de signalisation TUB d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;14

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
* XZAPT_PropTUB 	va_PropTUB_out
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
* Lit une proposition de signalisation TUB d'un plan d'actions
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
		exec @vl_Status = SQL_CI.PRC..XZAP;14
		@va_NumPA_in		,
		@va_Ordre_in		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;14
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
   ** Lecture d'une Proposition TUB d'un plan d'action
   */
   
   begin

      select	@vl_evt=evenement,
      		@vl_cle=cle
      from	EXP..SAD_PLN
      where 	numero = @va_NumPA_in
      
      select	@vl_eqt=equipement
      from EXP..SAD_TUB
      where numero = @va_NumPA_in and ordre = @va_Ordre_in
      
      select	@vl_priorite=priorite,
      		@vl_distance=distance
      from EXP..EQT_FIL
      where	evenement=@vl_evt
      	and	cle=@vl_cle
      	and	type_equipement=XDC_EQT_TUB
      	and	equipement=@vl_eqt
      
      select 	remarque, 
   		equipement,
      		explication,
		scenario,
		isnull(@vl_priorite,0),
   		isnull(@vl_distance,0),
		domaine
   	
      from EXP..SAD_TUB
   
        where numero = @va_NumPA_in and ordre = @va_Ordre_in 
   	
      return XDC_OK
      
   end   
         
#endif
go
