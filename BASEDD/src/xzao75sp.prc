/*E*/
/*Fichier : $Id: xzao75sp.prc,v 1.2 1994/11/09 11:43:48 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/11/09 11:43:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao75sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao75sp.h" */

/* static char *version = "@(#)xzao75.c	1.1 23/18/94 : xzao75" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des types Picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;75
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO75SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Retourne la liste des types de Picto
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
* Retourne les parametres numero de type, nombre de caracteres, nom fichier
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* GGY    	22/06/23	: Ajout parametre va_TypePnc_in et va_FichierPnc_in (DEM-473)
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree ne sont pas nuls
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         select numero, caracteres, fichier, type_pnc, fichier_pnc	 from CFG..TYP_PCT   
      
      end
      
#ifdef CI

      else
      
      /*A
      ** Travail dans la base CFT
      */
      
      begin   
         
         select numero, caracteres, fichier, type_pnc, fichier_pnc from CFT..TYP_PCT   
      
      end      

#endif

   end
      
   return XDC_OK
         
go  
