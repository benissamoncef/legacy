/*E*/
/*Fichier : $Id: xzao185sp.prc,v 1.2 1995/01/09 13:17:42 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:17:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao185sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao185sp.h" */

/* static char *version = "@(#)xzao185.c	1.1 23/18/94 : xzao185" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des sequences tunnel connues
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;185
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO185SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des sequences tunnel connues
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
* volcic	07 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
------------------------------------------------------*/

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls  
   */
      
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche des types de NAV et des scenarios associes
         */
         
         select numero, nom from CFG..EQT_SEQ_TUB
           
           order by numero
             

             
      end
   
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche des types de NAV et des scenarios associes
         */
         
         select numero, nom from CFT..EQT_SEQ_TUB
           
           order by numero
             

             
      end

#endif   
   
   end
      
   return XDC_OK
         
go  
