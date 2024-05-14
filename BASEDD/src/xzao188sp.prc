/*E*/
/*Fichier : $Id: xzao188sp.prc,v 1.3 2018/07/11 10:41:49 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 10:41:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao188sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	05 Nov 1996	:	Creation (DEM/1232)
* LCL		11/07/18	:	correction syntaxe from
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23 	:	Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao188sp.h" */

/* static char *version = "@(#)xzao188.c	1.1 23/18/94 : xzao188" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Cree un type d'alerte
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;188
   @va_Basedd_in      		char(3) = null,
   @va_NomTypeAlerte_in         char(25) = null,
   @va_NumAlerte_in		tinyint = null,
   @va_Resultat_out		int = XDC_LU output
   as 

/* 
*
* SP	XZAO188SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomTypeAlerte_in
* XDY_Mot		va_NumAlerte_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Cree un type d'alerte
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
*
* FONCTION :
*
* Insertion ou update dans la table TYP_ALT du nom et du numero
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Numero		tinyint,
   @vl_Site		char(10)


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NomTypeAlerte_in 	= null
   or @va_NumAlerte_in	= null
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
            **  Recherche du type d'alerte
            */   
      
            select @vl_Numero = numero from CFG..TYP_ALT
              where numero = @va_NumAlerte_in
              
            /*B
            ** Le type d'alerte n'existe pas => Creation de celui ci dans TYP_ALT
            */
            
            if @vl_Numero = null
            begin
               
               insert into CFG..TYP_ALT
               values ( @va_NumAlerte_in, @va_NomTypeAlerte_in )
               
               select @va_Resultat_out = XDC_AJOUT
                  
            end

            /*B
            ** Le type d'alerte existe => Update de celui ci dans TYP_ALT
            */
            else
            begin
               
               update CFG..TYP_ALT
               set nom = @va_NomTypeAlerte_in
               where numero = @va_NumAlerte_in
               
               select @va_Resultat_out = XDC_MODIF
                  
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
               **  Recherche du type d'alerte
               */   
      
               select @vl_Numero = numero from CFT..TYP_ALT
                 where numero = @va_NumAlerte_in
              
               /*B
               ** Le type d'alerte n'existe pas => Creation de celui ci dans TYP_ALT
               */
            
               if @vl_Numero = null
               begin
               
                  insert into CFT..TYP_ALT
                  values ( @va_NumAlerte_in, @va_NomTypeAlerte_in )
                  
                  select @va_Resultat_out = XDC_AJOUT
                  
               end
               
               /*B
               ** Le type d'alerte existe => Update de celui ci dans TYP_ALT
               */
               else
               begin
               
                  update CFT..TYP_ALT
                  set nom = @va_NomTypeAlerte_in
                  where numero = @va_NumAlerte_in
               
               select @va_Resultat_out = XDC_MODIF
                     
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
