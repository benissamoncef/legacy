/*E*/
/*Fichier : $Id: xzan17sp.prc,v 1.3 2020/11/09 14:10:21 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/09 14:10:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23 	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan17sp.h" */

/* static char *version = "@(#)xzan17.c	1.1 23/18/94 : xzan17" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Supprime une zone geographique
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;17
   @va_NumZone_in	int = null
   as 

/* 
*
* SP	XZAN17SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_NumZone_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Supprime une zone geographique
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumZone inconnu
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
* 
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Zone		int	,
   @vl_Site		tinyint


   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumZone_in 	= null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
         
         select @vl_Zone = numero from CFG..ANN_GEO
           where numero = @va_NumZone_in
           
         /*A
         ** La zone geographique existe
         ** Suppression de celle ci
         */
         
         if @vl_Zone != null
         begin
            
            delete CFG..ANN_GEO
            where numero = @va_NumZone_in
            
         end
         
         else
         
         begin
         
         /*A
         ** La zone geographique n'existe pas
         ** return XDC_NOK
         */
         
            return ( XDC_NOK ) 
         
         end
      end
      
      else
      
      /*A
      ** Le serveur n'est pas le CI
      ** return XDC_NOK
      */
         
         return ( XDC_NOK )
      
   end 
     
   return ( XDC_OK )
         
go  
