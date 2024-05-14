/*E*/
/*Fichier : $Id: xzao501sp.prc,v 1.1 2007/08/21 11:35:16 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/08/21 11:35:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao501sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao501sp.h" */

/* static char *version = "$Id: xzao501sp.prc,v 1.1 2007/08/21 11:35:16 gesconf Exp $ : xzao501" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des portions d'un district
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop proc XZAO501
go

create procedure XZAO501
   @va_Basedd_in       	char(3),
   @va_FiltreDistrict_in 	char(25)	
   as 

/* 
*
* SP	XZAO501SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_FiltreDistrict_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des portions d'un district
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
* Retourne  XZAOT_Portion
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
* volcic	22 Nov 1994	: Modification du type du district (v 1.2)
------------------------------------------------------*/
 
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_FiltreDistrict_in ) = null 	select @va_FiltreDistrict_in = null
   
   
   if @va_Basedd_in = null
   or @va_FiltreDistrict_in = null
    
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
         ** Recherche des portions du district
         */
         
         select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
         
         from CFG..RES_POR POR
         
           where POR.nom      like  @va_FiltreDistrict_in
            
      end
      
            
      else

#ifdef CI
      
      begin
      /*A
      **  Travail dans la base CFT
      */   
      
         if @va_Basedd_in = XDC_BASE_CFT
         
         /*A
         ** Recherche des portions du district
         */
         
         select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
         
         from CFT..RES_POR POR
         
           where POR.nom      like  @va_FiltreDistrict_in
                  
           
      end
      
      return XDC_OK
      
#endif      

       return (XDC_NOK)
   end
   
      
   return XDC_OK
         
go  
