/*E*/
/*Fichier : $Id: xzao164sp.prc,v 1.2 1995/01/09 13:16:57 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:16:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao164sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao164sp.h" */

/* static char *version = "@(#)xzao164.c	1.1 23/18/94 : xzao164" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des types NAV connectees avec les scenarios associes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;164
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO164SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des types de NAV avec les scenarios associes
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
         
         select distinct NAV.numero, 
         		NAV.presence_flash_4, 
         		NAV.nb_barrieres, 
         		SCN.scenario, 
         		SCN.auto_fichier,
         		SCNAV.scenario
         
         from   CFG..TYP_NAV NAV, 
         	CFG..EQT_SCN_CNN SCN,
                CFG..EQT_SCN_NAV SCNAV 
         	 
           where SCN.type_NAV = NAV.numero
             and SCNAV.numero = SCN.scenario
           
           order by NAV.numero
             

             
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
         
         select distinct NAV.numero, 
         		NAV.presence_flash_4, 
         		NAV.nb_barrieres, 
         		SCN.scenario, 
         		SCN.auto_fichier,
         		SCNAV.scenario
         
         from   CFT..TYP_NAV NAV, 
         	CFT..EQT_SCN_CNN SCN,
                CFT..EQT_SCN_NAV SCNAV 
         	 
           where SCN.type_NAV = NAV.numero
             and SCNAV.numero = SCN.scenario
           
           order by NAV.numero
             

             
      end
#endif
      
   end
      
   return XDC_OK
         
go  
