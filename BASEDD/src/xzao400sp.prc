/*E*/
/*Fichier : $Id: xzao400sp.prc,v 1.5 2016/09/29 15:42:00 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2016/09/29 15:42:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao400sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao400sp.h" */

/* static char *version = "@(#)xzao400.c	1.1 23/18/94 : xzao400" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones Temps De Parcours
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO400' and type = 'P')
	drop procedure XZAO400
go

create procedure XZAO400
   @va_Basedd_in        	char(3),
   @va_Site_in			T_SITE  = null
   as 

/* 
*
* SP	XZAO400SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des Zone De Parcours 
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
* Retourne les parametres numero de la Zone De Parcours, numero , libelle, site etc..
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	05 Fev 1996	: Creation (DEM/1395)
* mismer	14 Fev 1996	: Ajout code zone (DEM/1395)
* niepceron	09 Juin 2005	: Ajout code zone != 0 pour ne plus avoir les zone virtuelles (DEM/471) v1.4
* JMG		05/04/16: ajout nom eqt en sortie TDP ASF 1.5
------------------------------------------------------*/
    
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin
      if @va_Site_in = 0 select @va_Site_in = null
   
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         if ( @va_Site_in = NULL ) 
         begin
         
         /*A
         ** Recherche de la liste des Zone De Parcours pour tous les site
         */
         
              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute, 
        	   	      AUT.nom, 
        	   	      GEN.maitre,
        	   	      GEN.PR, 
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)
        
              from    CFG..EQT_GEN GEN, 
        	      CFG..RES_AUT AUT,
        	      CFG..EQT_ZDP ZDP
          
                where GEN.type         = XDC_EQT_ZDP
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone    != 0
            
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero 
         end
         
         else
         
         /*A
         ** Recherche de la liste des Zone De Parcours pour un site
         */
         
         begin
         
              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute, 
        	   	      AUT.nom, 
        	   	      GEN.maitre,
        	   	      GEN.PR, 
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)
        
              from    CFG..EQT_GEN GEN, 
        	      CFG..RES_AUT AUT,
        	      CFG..EQT_ZDP ZDP
          
                where GEN.type         = XDC_EQT_ZDP
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.sit          = @va_Site_in
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone   != 0
            
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero 
         end
           
            
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         if ( @va_Site_in = NULL ) 
         begin
         
            /*A
            ** Recherche de la liste des Zone De Parcours pour tous les site
            */
         
              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute, 
        	   	      AUT.nom, 
        	   	      GEN.maitre,
        	   	      GEN.PR, 
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)
        
              from    CFT..EQT_GEN GEN, 
        	      CFT..RES_AUT AUT,
        	      CFT..EQT_ZDP ZDP
          
                where GEN.type         = XDC_EQT_ZDP
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone   != 0
            
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero 
         end
         
         else
         
         /*A
         ** Recherche de la liste des Zone De Parcours pour un site
         */
         
         begin
         
              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute, 
        	   	      AUT.nom, 
        	   	      GEN.maitre,
        	   	      GEN.PR, 
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)
        
              from    CFT..EQT_GEN GEN, 
        	      CFT..RES_AUT AUT,
        	      CFT..EQT_ZDP ZDP
          
                where GEN.type         = XDC_EQT_ZDP
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.sit          = @va_Site_in
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone   != 0
            
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero 
         end
           
            
            
      end

#endif      
   end
      
   return XDC_OK
         
go  
