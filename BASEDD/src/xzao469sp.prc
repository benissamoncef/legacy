/*E*/
/*Fichier : $Id: xzao469sp.prc,v 1.1 2013/01/25 10:00:13 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/01/25 10:00:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao469sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao469sp.h" */

/* static char *version = "@(#)xzao469.c	1.1 23/18/94 : xzao469" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des destinations Temps De Parcours  par pmva
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO469
go

create procedure XZAO469
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO469SP
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
* VR   13/08/12        : creation (DEM/1014 PMA)
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
         
           select numero_PMA, numero_dest, type_dest, numero_ZDP
           from    CFG..DTP_PMA
               where inhibition   =  0
             order by numero_PMA, numero_ZDP desc
            
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
         
           select numero_PMA, numero_dest, type_dest, numero_ZDP
           from    CFT..DTP_PMA
               where inhibition   =  0
             order by numero_PMA, numero_ZDP
      
            
      end

#endif      
   end
      
   return XDC_OK
         
go  
