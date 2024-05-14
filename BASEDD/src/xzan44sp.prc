/*E*/
/*Fichier : $Id: xzan44sp.prc,v 1.1 1996/12/13 16:26:49 gesconf Exp $      Release : $Revision: 1.1 $     Date :$Date: 1996/12/13 16:26:49 $ 
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan44sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	12 dec 1996	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan44sp.h" */

/* static char *version = "@(#)xzan44.c	1.1 23/18/94 : xzan44" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des fiches annuaires d'astreinte pour une competence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;44
   @va_NumCompetence_in		smallint = null,
   @va_DateDebut_in		datetime = null,
   @va_DateFin_in		datetime = null,
   @va_SiteResponsable_out	tinyint = null output
   as 

/* 
*
* SP	XZAN44SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeFiche
*
* VALEUR RENDUE :
*
* Retourne la liste (date+numero+nom interlocuteur) des fiches annuaires d'astreinte pour une competence
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumCompetence inconnu
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
    
   @vl_Compt	smallint, @vl_fin tinyint
       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumCompetence_in = null or @va_NumCompetence_in=0
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      select @vl_fin=0
      if @va_DateDebut_in=XDC_DATE_NULLE select @va_DateDebut_in = null
      if @va_DateFin_in=XDC_DATE_NULLE select @vl_fin=1

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
      
         /*B
         ** Recherche de la liste des fiches annuaires d'astreinte
         */
     	 select CAL.date_debut,CAL.date_fin, FIC.numero, FIC.nom
         from 	CFG..ANN_AST_CMP CMP, 
         	CFG..ANN_AST_CAL CAL, 
         	CFG..ANN_FIC FIC
         
         where CMP.numero 	= @va_NumCompetence_in
           and CAL.competence 	= @va_NumCompetence_in
           and FIC.numero	=* CAL.interlocuteur
	   and CAL.date_fin	>= @va_DateDebut_in
	   and (CAL.date_debut   <= @va_DateFin_in or @vl_fin=1)
         order by CAL.date_debut, CAL.date_fin
     
	 select @va_SiteResponsable_out = site_responsable
	 from CFG..ANN_AST_CMP CMP
	 where CMP.numero       = @va_NumCompetence_in

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
