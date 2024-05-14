/*E*/
/*Fichier : $Id: xzan01sp.prc,v 1.3 2020/11/09 13:57:47 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/09 13:57:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan01sp.h" */

/* static char *version = "@(#)xzan01.c	1.1 23/18/94 : xzan01" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Cree un type d'astreinte
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;01
   @va_Basedd_in      		char(3) = null,
   @va_NomTypeAstreinte_in      char(25) = null,
   @va_NumAstreinte_in		tinyint = null,
   @va_Resultat_out		int = XDC_LU output
   as 

/* 
*
* SP	XZAN01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomTypeAstreinte_in
* XDY_Mot		va_NumAstreinte_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Cree un type d'astreinte
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* CONFIG OFF LINE
* 
* ( ne peut pas reutiliser un numero deja utilise, meme dans le passe, pour conserver la 
*   coherence de l'historique )
*
* FONCTION :
*
* Insertion ou update dans la table TYP_AST du nom et du numero
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Numero		tinyint,
   @vl_Nom		char(25),
   @vl_Site		char(10)


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NomTypeAstreinte_in 	= null
   or @va_NumAstreinte_in	= null
   or @va_Basedd_in		= null 
      
      return ( XDC_ARG_INV )
   
   else   
   begin
      
	   /* Verification que le site est bien celui du CI ou CA */
	   select  @vl_Site = @@servername
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
         /*A
         ** Travail dans la base CFG
         */
         
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*B
            **  Recherche du type d'astreinte
            */   
      
            select @vl_Numero = numero from CFG..TYP_AST
              where numero = @va_NumAstreinte_in
            
            select @vl_Nom = nom from CFG..TYP_AST
              where nom = @va_NomTypeAstreinte_in
              
            /*B
            ** Le type d'astreinte n'existe pas => Creation de celui ci dans TYP_AST
            */
            
            if @vl_Numero = null and @vl_Nom = null
            begin
               
               insert into CFG..TYP_AST
               values ( @va_NumAstreinte_in, @va_NomTypeAstreinte_in )
               
               select @va_Resultat_out = XDC_AJOUT
                  
            end
         
         end
        
#ifdef CI        
        
         else
	 
	 /*A
	 ** Base differente de CFG
	 */
	 
	      begin
            
            /*A
            ** Travail dans la base CFT
            */
         
            if @va_Basedd_in = XDC_BASE_CFT
         
            begin
            
               /*B
               **  Recherche du type d'astreinte
               */   
      
               select @vl_Numero = numero from CFT..TYP_AST
                 where numero = @va_NumAstreinte_in
              
               select @vl_Nom = nom from CFT..TYP_AST
                 where nom = @va_NomTypeAstreinte_in
              
               /*B
               ** Le type d'astreinte n'existe pas => Creation de celui ci dans TYP_AST
               */
            
               if @vl_Numero = null and @vl_Nom = null
               begin
               
                  insert into CFT..TYP_AST
                  values ( @va_NumAstreinte_in, @va_NomTypeAstreinte_in )
                  
                  select @va_Resultat_out = XDC_AJOUT
                  
               end
            end
            
#endif             
            
            else
            
            /*A
            ** Erreur sur le nom de base
            */
            
               return ( XDC_NOK )

#ifdef CI
         
         end
         
#endif      
         
      end
      
      else
      
      /*A
      ** Le serveur n'est pas le CI
      */
         
         return ( XDC_NOK )
      
   end 
     
   return ( XDC_OK )
         
go  
