/*E*/
/*Fichier : @(#)xzao127sp.prc	1.3      Release : 1.3        Date : 01/09/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao127sp.h" */

/* static char *version = "@(#)xzao127.c	1.1 23/18/94 : xzao127" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO1270' and type = 'P')
	drop procedure XZAO1270
go

create procedure XZAO1270
   @va_Basedd_in       	char(3)
   as 

/* 
*
* SP	XZAO127SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_Mot	va_NumeroPortion_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des portions d'un troncon
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
* Retourne nom des troncons
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	29 Nov 1994	: Ajout elements liste (v1.2)
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   
      return XDC_ARG_INV
   
   else   
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
	 ** Recherche des nom de troncons de la base
	 */
	 
	 select nom, PR_debut, PR_fin, autoroute from CFG..RES_TRC
	    
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      if @va_Basedd_in = XDC_BASE_CFT
      begin   
      
         /*A
	 ** Recherche des nom de troncons de la base
	 */
	 
	 select nom PR_debut, PR_fin, autoroute from CFT..RES_TRC
	    
      end

#endif

   end
      
   return XDC_OK
         
go  
