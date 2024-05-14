/*E*/
/*Fichier : $Id: xzan31sp.prc,v 1.3 1996/12/13 16:26:44 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/12/13 16:26:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan31sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	03 dec 1994	: modif (ann_ast_cmp) 1.2
* guilhou	12 dec 1996	: select distinct (DEM/1335) 1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan31sp.h" */

/* static char *version = "@(#)xzan31.c	1.1 23/18/94 : xzan31" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des fiches annuaires possedant la competence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;31
   @va_NumCompetence_in		smallint = null
   as 

/* 
*
* SP	XZAN31SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeAnn
*
* VALEUR RENDUE :
*
* Retourne la liste (numero+nom interlocuteur) des fiches annuaires possedant la competence
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
      
         create table #RES (numero int,nom char(25))
         
         insert #RES
         select distinct FIC.numero, 
          		 FIC.nom 
         from CFG..ANN_FIC FIC, CFG..ANN_CMP CMP 
	 where CMP.competence	= @va_NumCompetence_in
	   and FIC.numero	= CMP.interlocuteur
	   
         insert #RES
         select distinct numero, 
          		 nom 
         from CFG..ANN_FIC 
	 where competence = @va_NumCompetence_in
	 
	 select distinct numero,nom  from #RES order by nom
	  
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
