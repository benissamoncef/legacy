/*E*/
/*Fichier : $Id: xzan22sp.prc,v 1.3 2014/11/12 11:28:01 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2014/11/12 11:28:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* guilhou	14 nov 1996	: categorie devient district de validite (DEM/1306) 1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan22sp.h" */

/* static char *version = "@(#)xzan22.c	1.1 23/18/94 : xzan22" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit competence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;22
   @va_NumCompetence_in		smallint = null,
   @va_NumAstreinte_out		smallint = null output,
   @va_NumDistrict_out		smallint = null output,
   @va_SousType_out		char(25) = null output,
   @va_SiteResponsable_out	tinyint  = null output
   as 

/* 
*
* SP	XZAN22SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumAstreinte_out
* XDY_Mot		va_NumDistrict_out
* XZANT_ListeGeo
*
* VALEUR RENDUE :
*
* Retourne les valeurs liees a une competence
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_Compt 	smallint
       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumCompetence_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base CFG
      */   
      
      select @vl_Compt = numero from CFG..ANN_AST_CMP
        where numero = @va_NumCompetence_in
        
      /*A
      ** La competence existe
      */
      
      if @vl_Compt != null
      begin
      
         /*A
         ** Affectation des parametres de sortie
         */
         
         select @va_NumAstreinte_out 	= type_d_astreinte,
         	@va_NumDistrict_out	= district_validite,
		@va_SousType_out	= sous_type_d_astreinte,
		@va_SiteResponsable_out = site_responsable
         	
         from 	CFG..ANN_AST_CMP
         
         where numero = @va_NumCompetence_in
         	
         /*A
         ** Recherche des zones geographiques correspondantes
         */
         
         select numero,
         	autoroute,
         	PR_debut,
         	PR_fin,
         	sens
         
         from 	CFG..ANN_GEO
         
         where competence = @va_NumCompetence_in
      	 order by autoroute   
         
      end
      
      else
      
      /*A
      ** La competence n'existe pas
      ** return XDC_NOK
      */
      
         return ( XDC_NOK )
      
   end
     
   return ( XDC_OK )
         
go  
