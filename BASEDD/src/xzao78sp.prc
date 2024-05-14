/*E*/
/*Fichier : $Id: xzao78sp.prc,v 1.9 2020/11/03 17:04:25 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:04:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao78sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* mismer	27 Aou 1996	: Modif si arg Machine = NULL => liste de tous les pictos (ANA30) (v1.3)
* mismer	27 Aou 1996	: remplacement XZAO78 par XZAO;78 (ANA30) (v1.4)
* JMG   13/11/07        : ajout site de gestion DEM715  1.5
* LCL		02/10/18	: suppression SAGA DEM1306 1.6
* LCL	22/10/18	: ajout type et id LCR DEM1306 1.7
* JPL	23/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.8
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao78sp.h" */

/* static char *version = "@(#)xzao78.c	1.1 23/18/94 : xzao78" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMV pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO78' and type = 'P')
	drop procedure XZAO78
go

create procedure XZAO78
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO78SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des PMV de la Machine 
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
* Retourne les parametres numero de PMV, nom du ST, numero de liaison, adresse rgs
* numero de type de PMV, adresse IP, port IP
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint
   
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
      
   if @va_Basedd_in = null 
      return XDC_ARG_INV
   
   else   
   
   begin

      if rtrim (@va_NomMachine_in) = NULL select @va_NomMachine_in=NULL
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         if ( @va_NomMachine_in != NULL )
         begin
 
            /*A
            ** Recherche du numero de la machine argument
            */
         
            select @vl_Machine = numero from CFG..EQT_GEN
              where nom  = @va_NomMachine_in
                and type = XDC_EQT_MAC
                   
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct PMV.numero,
        	   	   GEN.nom, 
        		   PMV.liaison_serie, 
        		   PMV.rgs, 
        	   	   PMV.type ,
			   GEN2.site_gestion,
			   PMV.adresse_ip,
			   PMV.port,
			   PMV.type_lcr,
			   PMV.id_lcr
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PMV PMV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMV
               and GEN2.eqt_supprime = XDC_FAUX
               and PMV.numero       = GEN2.numero
             
             order by PMV.numero, GEN.nom, PMV.type   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
           select distinct PMV.numero,
        	   	   GEN.nom, 
        		   PMV.liaison_serie, 
        		   PMV.rgs, 
        	   	   PMV.type ,
			   GEN2.site_gestion,
			   PMV.adresse_ip,
			   PMV.port,
			   PMV.type_lcr,
			   PMV.id_lcr
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PMV PMV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMV
               and GEN2.eqt_supprime = XDC_FAUX
               and PMV.numero       = GEN2.numero
             
             order by PMV.numero, GEN.nom, PMV.type   
         end
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         if ( @va_NomMachine_in != NULL )
         begin
            /*A
            ** Recherche du numero de la machine argument
            */
         
            select @vl_Machine = numero from CFT..EQT_GEN
                 where nom  = @va_NomMachine_in
                   and type = XDC_EQT_MAC
                   
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct PMV.numero,
        		   GEN.nom, 
        		   PMV.liaison_serie, 
        		   PMV.rgs, 
        		   PMV.type ,
			   GEN2.site_gestion,
			   PMV.adresse_ip,
			   PMV.port,
			   PMV.type_lcr,
			   PMV.id_lcr
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PMV PMV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMV
               and GEN2.eqt_supprime = XDC_FAUX
               and PMV.numero       = GEN2.numero
            
             order by GEN.nom, PMV.numero, PMV.type   
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct PMV.numero,
        		   GEN.nom, 
        		   PMV.liaison_serie, 
        		   PMV.rgs, 
        		   PMV.type ,
			   GEN2.site_gestion,
			   PMV.adresse_ip,
			   PMV.port,
			   PMV.type_lcr,
			   PMV.id_lcr
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PMV PMV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMV
               and GEN2.eqt_supprime = XDC_FAUX
               and PMV.numero       = GEN2.numero
            
             order by GEN.nom, PMV.numero, PMV.type   
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
