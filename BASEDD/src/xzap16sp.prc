/*E*/
/*Fichier : $Id: xzap16sp.prc,v 1.4 1998/09/22 13:38:21 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/09/22 13:38:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* C.T.		17 Mar 1995	: Ajout parametre de retour confidentiel
*                                 (V 1.2)
* P.NT.		18 Oct 1996	: Ajout parametre de retour niveau 
				(dem/1248) v1.3
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.4
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

create procedure XZAP;16

	@va_NumPA_in		smallint,
	@va_Ordre_in		tinyint,
	@va_Remarque_out	char(50) output,
	@va_Confidentiel_out	bit	output,
	@va_Niveau_out	tinyint	output
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
*
* ARGUMENTS EN SORTIE :
*
* XZAPT_PropTFM 	va_PropTFM_out
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
* Lit une proposition d'un envoi de message a TFM d'un plan d'actions
*
* MECANISMES :
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzap16sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "@(#)xzap16sp.prc	1.1 08/25/94 : xzap16sp" ; */

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
		exec @vl_Status = SQL_CI.PRC..XZAP;16
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Remarque_out	output,
			@va_Confidentiel_out	output,
			@va_Niveau_out		output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;16
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Remarque_out	output,
			@va_Confidentiel_out	output,
			@va_Niveau_out		output

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
   	select @va_Confidentiel_out = XDC_NON
	return XDC_ARG_INV
   end
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition TFM d'un plan d'action
   */
   
   begin
      
   	select 
		@va_Remarque_out	= remarque, 
		@va_Confidentiel_out	= confidentiel,
		@va_Niveau_out	= niveau
   		
   	from EXP..SAD_TFM TFM
   
        where numero = @va_NumPA_in 
          and ordre  = @va_Ordre_in 
        
     
     
      select 	texte,
      		morceau
   	
      from EXP..SAD_TXT_TFM
   
        where numero = @va_NumPA_in 
          and ordre  = @va_Ordre_in 
        
        order by morceau
        
        
      return XDC_OK
      
   end   
         
#endif
go
