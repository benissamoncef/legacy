#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaoc.h"
/*E*/
/* Fichier : $Id: xzao870sp.prc,v 1.2 2018/11/05 17:40:33 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/11/05 17:40:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao870sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	21/01/12        : Creation (DEM/1016)
* JPL	16/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao870sp.h" */

/* static char *version = "@(#)xzao870sp.prc	1.2 1.2 11/19/07: xzao870" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PRV pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO870' and type = 'P')
	drop procedure XZAO870
go


create procedure XZAO870
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO870SP
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
* Retourne les parametres numero de PRV, nom du ST, numero de liaison, adresse rgs
* numero de type de PRV
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
            ** Recherche de la liste des PRV associes a plusieurs serveurs
            */
         
           select distinct PRV.numero,
				   PRV.liaison_serie,
				   PRV.rgs,
        	   	   GEN.nom, 
        		   PRV.type, 
        		   PRV.vitesse_nominale, 
        	   	   PRV.zone,
        	   	   PRV.critique,
				   GEN2.site_gestion
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PRV PRV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PRV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PRV
               and GEN2.eqt_supprime = XDC_FAUX
               and PRV.numero       = GEN2.numero
             
             order by PRV.numero, GEN.nom, PRV.type   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des PRV associes a plusieurs serveurs
            */
           select distinct PRV.numero,
				   PRV.liaison_serie,
				   PRV.rgs,
        	   	   GEN.nom, 
        		   PRV.type, 
        		   PRV.vitesse_nominale, 
        	   	   PRV.zone,
        	   	   PRV.critique,
				   GEN2.site_gestion
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PRV PRV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PRV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PRV
               and GEN2.eqt_supprime = XDC_FAUX
               and PRV.numero       = GEN2.numero
             
             order by PRV.numero, GEN.nom, PRV.type   
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
            ** Recherche de la liste des PRV associes a plusieurs serveurs
            */
         
           select distinct PRV.numero,
		   		   PRV.liaison_serie,
				   PRV.rgs,
        	   	   GEN.nom, 
        		   PRV.type, 
        		   PRV.vitesse_nominale, 
        	   	   PRV.zone,
        	   	   PRV.critique,
				   GEN2.site_gestion
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PRV PRV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PRV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PRV
               and GEN2.eqt_supprime = XDC_FAUX
               and PRV.numero       = GEN2.numero
            
             order by GEN.nom, PRV.numero, PRV.type   
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct PRV.numero,
		   		   PRV.liaison_serie,
				   PRV.rgs,
        	   	   GEN.nom, 
        		   PRV.type, 
        		   PRV.vitesse_nominale, 
        	   	   PRV.zone,
        	   	   PRV.critique,
				   GEN2.site_gestion 
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PRV PRV
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PRV
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PRV
               and GEN2.eqt_supprime = XDC_FAUX
               and PRV.numero       = GEN2.numero
            
             order by GEN.nom, PRV.numero, PRV.type   
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
