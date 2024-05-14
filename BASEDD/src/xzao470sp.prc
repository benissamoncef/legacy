/*E*/
/*Fichier : $Id: xzao470sp.prc,v 1.1 2012/02/14 14:32:35 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/02/14 14:32:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao470sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao470sp.h" */

/* static char *version = "@(#)xzao470.c	1.1 23/18/94 : xzao470" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des destinations Temps De Parcours  par pmv
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO470
go

create procedure XZAO470
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO470SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des destinations Temps De Parcours 
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
* Retourne les parametres numero de destination temps De Parcours, libellees, autoroute, PR, Sens, Type
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VRE		07/02/2012	: Creation (DEM/1014)
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
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
         ** Recherche de la liste des destination Temps De Parcours 
         */
         
           select numero_PMV, numero_dest, type_dest, numero_ZDP
           from    CFG..DTP_PMV
               where inhibition   =  0
             order by numero_PMV
            
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche de la liste des destination Temps De Parcours 
         */
         
           select numero_PMV, numero_dest, type_dest, numero_ZDP
           from    CFT..DTP_PMV
               where inhibition   =  0
             order by numero_PMV
      
            
      end

#endif      
   end
      
   return XDC_OK
         
go  
