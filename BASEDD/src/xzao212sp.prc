/*E*/
/*Fichier : $Id: xzao212sp.prc,v 1.3 1995/01/09 13:18:29 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/01/09 13:18:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao212sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao212sp.h" */

/* static char *version = "@(#)xzao212.c	1.1 23/18/94 : xzao212" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Troncons contenant PR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;212
   @va_Basedd_in       	char(3),
   @va_Autoroute_in 	tinyint,	
   @va_PR_in		int
   as 

/* 
*
* SP	XZAO212SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Mot	va_Autoroute_in
* XDY_Nom	va_PR_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Nom 	va_ListeTroncons_out
*
* VALEUR RENDUE :
*
*  Liste des Troncons contenant PR
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
* Retourne la liste des Troncons contenant PR
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
   or @va_Autoroute_in = null
   or @va_PR_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG   
      
         /*A
         ** Recherche des Troncons contenant PR
         */
         
         select nom from CFG..RES_TRC
	   
	   where autoroute = @va_Autoroute_in
	     and PR_debut <= @va_PR_in
	     and PR_fin   > @va_PR_in
	     
#ifdef CI

      else
     
     
      /*A
      **  Travail dans la base CFT
      */
         
         /*A
         ** Recherche des Troncons contenant PR
         */
         
         select nom from CFT..RES_TRC
	   
	   where autoroute = @va_Autoroute_in
	     and PR_debut <= @va_PR_in
	     and PR_fin   > @va_PR_in
	  	     
#endif
	
   end
      
   return XDC_OK
         
go  
