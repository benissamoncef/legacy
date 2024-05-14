/*E*/
/*Fichier : $Id: xzao122sp.prc,v 1.2 1995/01/09 13:16:01 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:16:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao122sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao122sp.h" */

/* static char *version = "@(#)xzao122.c	1.1 23/18/94 : xzao122" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des districts 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;122
   @va_Basedd_in        	char(3)
  as 

/* 
*
* SP	XZAO122SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Liste des districts (nom) 
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
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
------------------------------------------------------*/
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **  Les parametres d'entree ne sont pas nus   
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche des districts
         */
         
         select nom from CFG..RES_DIS
                   
            
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche des districts
         */
         
         select * from CFT..RES_DIS
                   
            
      end      

#endif

   end
      
   return XDC_OK
         
go  
