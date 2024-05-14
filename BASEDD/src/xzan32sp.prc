/*E*/
/*Fichier : $Id: xzan32sp.prc,v 1.4 1996/12/10 17:58:10 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/12/10 17:58:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan32sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	02 dec 1994	: Jointure externe
* guilhou	14 nov 1996	: refonte de la gestion des astreintes (DEM/1306) 1.3
* guilhou	10 dec 1996	: site responsable du calendrier en sortie (DEM/1306) 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan32sp.h" */

/* static char *version = "@(#)xzan32.c	1.1 23/18/94 : xzan32" ; */

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

create procedure XZAN;32
   @va_NumCompetence_in		smallint = null,
   @va_SiteResponsable_out	tinyint = null output
   as 

/* 
*
* SP	XZAN32SP
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
    
   @vl_Compt	smallint
       
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
