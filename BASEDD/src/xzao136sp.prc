/*E*/
/*Fichier : @(#)xzao136sp.prc	1.1      Release : 1.1        Date : 11/07/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao136sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao136sp.h" */

/* static char *version = "@(#)xzao136.c	1.1 23/18/94 : xzao136" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lire la machine suppleante concernee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go
drop proc XZAO136
go
create procedure XZAO136
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO136SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE :
*
* XDY_Machine	vl_Suppleant_out		
*
* VALEUR RENDUE :
*
* Retourne la machine suppleante concernee 
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
* PNI		26 jan 2009	: Suppression du parametre de sortie v1.3 DEM800
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint,
   @vl_Suppleant_out		char(10)
    
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_NomMachine_in = null
    
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
         ** Recherche de la machine suppleante
         */
         
         select @vl_Suppleant_out = GSP.nom
         
         from CFG..EQT_GEN GMAC, CFG..EQT_GEN GSP, CFG..EQT_MAC MAC
                 
           where GMAC.nom   = @va_NomMachine_in
             and GMAC.type  = XDC_EQT_MAC
             and MAC.numero = GMAC.numero
             and GSP.numero = MAC.suppleant
             and GSP.type   = XDC_EQT_MAC
             
      end
      
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche de la machine suppleante
         */
         
         select @vl_Suppleant_out = GSP.nom
         
         from CFT..EQT_GEN GMAC, CFT..EQT_GEN GSP, CFT..EQT_MAC MAC
                 
           where GMAC.nom   = @va_NomMachine_in
             and GMAC.type  = XDC_EQT_MAC
             and MAC.numero = GMAC.numero
             and GSP.numero = MAC.suppleant
             and GSP.type   = XDC_EQT_MAC
             
      end


#endif      
   end
      
   select @vl_Suppleant_out
   return XDC_OK
         
go  
