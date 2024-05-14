/*E*/
/*Fichier : $Id: xzao401sp.prc,v 1.6 2018/10/11 11:44:03 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/10/11 11:44:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao401sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao401sp.h" */

/* static char *version = "$Id: xzao401sp.prc,v 1.6 2018/10/11 11:44:03 devgfi Exp $" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des destinations Temps De Parcours 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO401' and type = 'P')
	drop procedure XZAO401
go

create procedure XZAO401
   @va_Basedd_in        	char(3)
   as 

/* 
*
* SP	XZAO401SP
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
* mismer	05 Fev 1996	: Creation (DEM/1395)
* LCL		07 Fev 2012	: TdP etendu (DEM/1014) v1.2
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
         
           select distinct DTP.numero,
        	   	   rtrim(DTP.libelle_TFM),
        	   	   DTP.libelle_PMV,
        		   DTP.autoroute, 
        	   	   DTP.PR, 
        	   	   DTP.sens,
           		   DTP.type_localisation
        
           from    CFG..RES_DTP DTP
          
             where DTP.inhibition   =  0
            
             order by DTP.sens, DTP.autoroute, DTP.PR, DTP.numero   
            
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
         
           select distinct DTP.numero,
        	   	   DTP.libelle_TFM,
        	   	   DTP.libelle_PMV,
        		   DTP.autoroute, 
        	   	   DTP.PR, 
        	   	   DTP.sens,
           		   DTP.type_localisation
        
           from    CFT..RES_DTP DTP
          
             where DTP.inhibition   =  0
            
             order by DTP.sens, DTP.autoroute, DTP.PR, DTP.numero   
      
            
      end

#endif      
   end
      
   return XDC_OK
         
go  
