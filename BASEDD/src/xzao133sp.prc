/*E*/
/*Fichier : $Id: xzao133sp.prc,v 1.3 1994/11/28 17:48:51 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/28 17:48:51 $
------------------------------------------------------
* GTIEA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao133sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao133sp.h" */

/* static char *version = "@(#)xzao133.c	1.1 23/18/94 : xzao133" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des villes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;133
   @va_Basedd_in       	char(3)
   as 

/* 
*
* SP	XZAO133SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des villes
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
* Retourne la liste des villes
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
   ** Les parametres d'entree sont non nuls 
   */
    
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
	 ** Recherche des villes
	 */
	 
	 select nom from CFG..RES_VIL 
	 		  
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
	 ** Recherche des villes 
	 */
	 
	 select nom from CFT..RES_VIL 
	 		  
      end

#endif

   end
      
   return XDC_OK
         
go  
