/*E*/
/*Fichier : $Id: xzap24sp.prc,v 1.1 1999/02/26 11:24:35 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:24:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap24sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	25 Nov 1998	: Creation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'un envoi de message a TFM d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;24

	@va_NumPA_in		smallint,
	@va_Ordre_in		tinyint,
	@va_Gravite_out	tinyint output,
	@va_Trafic_out	tinyint	output
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
*
* ARGUMENTS EN SORTIE :
*
* XDC_Octet va_Gravite_out
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
* Lit une proposition d'un export intergestionnaire d'un plan d'actions
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap24sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap24sp.prc	1.1 08/25/94 : xzap24sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

as 
  
#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;24
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Gravite_out	 output,
			@va_Trafic_out		output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;24
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Gravite_out	 output,
			@va_Trafic_out		output

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   
   if @va_NumPA_in = null
      or @va_Ordre_in = null
   begin  
	return XDC_ARG_INV
   end
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition "export intergestionnaire" d'un plan d'action
   */
   
   begin
      
   	select 
		@va_Gravite_out	= gravite,
		@va_Trafic_out	= trafic
   		
   	from EXP..SAD_EXP
   
        where numero = @va_NumPA_in 
          and ordre  = @va_Ordre_in 
        
     
     
        
      return XDC_OK
      
   end   
         
#endif
go
