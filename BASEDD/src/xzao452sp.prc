/*E*/
/* Fichier : $Id: xzao452sp.prc,v 1.5 2020/05/19 14:08:25 pc2dpdy Exp $        $Revision: 1.5 $        $Date: 2020/05/19 14:08:25 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao452sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JMG   13/11/07        : ajout site de gestion DEM715  1.2
* JPL	16/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.4
* LCL	07/06/19	: BAF/IP DEM1332
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao452sp.h" */

/* static char *version = "" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des BAF connectees avec ses modules
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO452' and type = 'P')
	drop procedure XZAO452
go


create procedure XZAO452
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO452SP
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
* Retourne la liste des BAF connectees avec ses modules
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
            ** Recherche de la liste des PMV associes a un serveur
            */
         
            select distinct BAF.numero, 
            		    BAF.type, 
         		    GSRV.nom, 
         		    BAF.liaison_serie, 
         		    BAF.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAF.site_gestion,
			    BAF.adresse_ip,
			    BAF.port
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GBAF, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_BAF BAF, 
         	CFG..EQT_MODBAF MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAF.type = XDC_EQT_BAF 
                and GBAF.maitre      = GSRV.numero
                and GBAF.type_maitre = XDC_MAI_BAF
                and GBAF.eqt_supprime = XDC_FAUX
                and BAF.numero       = GBAF.numero
                and GMOD.type        = XDC_EQT_MODBAF
                and GMOD.maitre      = GBAF.numero
                and GMOD.type_maitre = XDC_MAI_MODBAF
                and MOD.numero       = GMOD.numero
             
             order by BAF.numero, BAF.type 

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct BAF.numero, 
            		    BAF.type, 
         		    GSRV.nom, 
         		    BAF.liaison_serie, 
         		    BAF.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAF.site_gestion,
			    BAF.adresse_ip,
			    BAF.port
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GBAF, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_BAF BAF, 
         	CFG..EQT_MODBAF MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAF.type = XDC_EQT_BAF 
                and GBAF.maitre      = GSRV.numero
                and GBAF.type_maitre = XDC_MAI_BAF
                and GBAF.eqt_supprime = XDC_FAUX
                and BAF.numero       = GBAF.numero
                and GMOD.type        = XDC_EQT_MODBAF
                and GMOD.maitre      = GBAF.numero
                and GMOD.type_maitre = XDC_MAI_MODBAF
                and MOD.numero       = GMOD.numero
             
             order by BAF.numero, BAF.type   

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
            ** Recherche de la liste des PMV associes a un serveur
            */
         
            select distinct BAF.numero, 
            		    BAF.type, 
         		    GSRV.nom, 
         		    BAF.liaison_serie, 
         		    BAF.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAF.site_gestion,
			    BAF.adresse_ip,
			    BAF.port
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GBAF, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_BAF BAF, 
         	CFT..EQT_MODBAF MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAF.type        = XDC_EQT_BAF
                and GBAF.maitre      = GSRV.numero
                and GBAF.type_maitre = XDC_MAI_BAF
                and GBAF.eqt_supprime = XDC_FAUX
                and BAF.numero       = GBAF.numero
                and GMOD.type        = XDC_EQT_MODBAF
                and GMOD.maitre      = GBAF.numero
                and GMOD.type_maitre = XDC_MAI_MODBAF
                and MOD.numero       = GMOD.numero
             
             order by BAF.numero, BAF.type   

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct BAF.numero, 
            		    BAF.type, 
         		    GSRV.nom, 
         		    BAF.liaison_serie, 
         		    BAF.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAF.site_gestion,
			    BAF.adresse_ip,
			    BAF.port
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GBAF, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_BAF BAF, 
         	CFT..EQT_MODBAF MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAF.type        = XDC_EQT_BAF
                and GBAF.maitre      = GSRV.numero
                and GBAF.type_maitre = XDC_MAI_BAF
                and GBAF.eqt_supprime = XDC_FAUX
                and BAF.numero       = GBAF.numero
                and GMOD.type        = XDC_EQT_MODBAF
                and GMOD.maitre      = GBAF.numero
                and GMOD.type_maitre = XDC_MAI_MODBAF
                and MOD.numero       = GMOD.numero
             
             order by BAF.numero, BAF.type   

         end
             
      end

#endif      
      
   end
      
   return XDC_OK
         
go  
