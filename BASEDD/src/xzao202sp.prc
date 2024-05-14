/*E*/
/*Fichier : $Id: xzao202sp.prc,v 1.2 1995/01/09 13:18:07 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:18:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao202sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao202sp.h" */

/* static char *version = "@(#)xzao202.c	1.1 23/18/94 : xzao202" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des LT video du district
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;202
   @va_Basedd_in        	char(3),
   @va_District_in		tinyint
   as 

/* 
*
* SP	XZAO202SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in
* XDY_District	va_District_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet	va_ListeNbreCanauxFixes_out
* XDY_Octet	va_ListeNbreCanauxCycles_out
* XDY_Nom	va_ListeNomLTVideo_out
*
* VALEUR RENDUE :
*
* Liste des LT video du district
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
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales  
   */
   
   @vl_Autoroute	tinyint,
   @vl_PRdebut		int,
   @vl_PRfin		int
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_District_in = null
    
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
      
         /*A
         ** Recherche des LT Video du district
         */
         
         select distinct LTV.nombre_liaisons_fixes, 
         		LTV.nombre_liaisons_cycles, 
         		GLTV.nom 
         		
         from CFG..EQT_GEN GLTV, CFG..EQT_LTV LTV, CFG..RES_POR POR
         	
         where POR.district = @va_District_in
           and GLTV.type      = XDC_EQT_LTV
           and GLTV.autoroute = POR.autoroute
           and GLTV.PR       >= POR.PR_debut
           and GLTV.PR       <  POR.PR_fin
           and LTV.numero     = GLTV.numero 	
         
      end

#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche des LT Video du district
         */
         
         select distinct LTV.nombre_liaisons_fixes, 
         		LTV.nombre_liaisons_cycles, 
         		GLTV.nom 
         		
         from CFT..EQT_GEN GLTV, CFT..EQT_LTV LTV, CFT..RES_POR POR
         	
         where POR.district = @va_District_in
           and GLTV.type      = XDC_EQT_LTV
           and GLTV.autoroute = POR.autoroute
           and GLTV.PR       >= POR.PR_debut
           and GLTV.PR       <  POR.PR_fin
           and LTV.numero     = GLTV.numero 	
         
      end
#endif      
      
   end
      
   return XDC_OK
         
go  
