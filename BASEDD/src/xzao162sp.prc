/*E*/
/*Fichier : $Id: xzao162sp.prc,v 1.2 1994/11/09 11:42:50 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/11/09 11:42:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao162sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao162sp.h" */

/* static char *version = "@(#)xzao162.c	1.1 62/18/94 : xzao162" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Scenarios
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;162
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO162SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des Scenarios
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
   **  Les parametres d'entree ne sont pas nuls   
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche des scenarios
         */
         
         select numero, scenario, nom from CFG..EQT_SCN_NAV
                   
            
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche des scenarios
         */
         
         select numero, scenario, nom from CFT..EQT_SCN_NAV
         
                   
            
      end      

#endif
 
   end
      
   return XDC_OK
         
go  
