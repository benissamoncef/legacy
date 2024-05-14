/*E*/
/* Fichier : $Id: xzao453sp.prc,v 1.4 2018/11/05 17:38:44 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/05 17:38:44 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao453sp.prc
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
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao453sp.h" */

/* static char *version = "" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des BAD connectees avec ses modules
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO453' and type = 'P')
	drop procedure XZAO453
go


create procedure XZAO453
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO453SP
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
* Retourne la liste des BAD connectees avec ses modules
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
         
            select distinct BAD.numero, 
            		    BAD.type, 
         		    GSRV.nom, 
         		    BAD.liaison_serie, 
         		    BAD.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAD.site_gestion
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GBAD, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_BAD BAD, 
         	CFG..EQT_MODBAD MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAD.type = XDC_EQT_BAD 
                and GBAD.maitre      = GSRV.numero
                and GBAD.type_maitre = XDC_MAI_BAD
                and GBAD.eqt_supprime = XDC_FAUX
                and BAD.numero       = GBAD.numero
                and GMOD.type        = XDC_EQT_MODBAD
                and GMOD.maitre      = GBAD.numero
                and GMOD.type_maitre = XDC_MAI_MODBAD
                and MOD.numero       = GMOD.numero
             
             order by BAD.numero, BAD.type 

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct BAD.numero, 
            		    BAD.type, 
         		    GSRV.nom, 
         		    BAD.liaison_serie, 
         		    BAD.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAD.site_gestion
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GBAD, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_BAD BAD, 
         	CFG..EQT_MODBAD MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAD.type = XDC_EQT_BAD 
                and GBAD.maitre      = GSRV.numero
                and GBAD.type_maitre = XDC_MAI_BAD
                and GBAD.eqt_supprime = XDC_FAUX
                and BAD.numero       = GBAD.numero
                and GMOD.type        = XDC_EQT_MODBAD
                and GMOD.maitre      = GBAD.numero
                and GMOD.type_maitre = XDC_MAI_MODBAD
                and MOD.numero       = GMOD.numero
             
             order by BAD.numero, BAD.type   

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
         
            select distinct BAD.numero, 
            		    BAD.type, 
         		    GSRV.nom, 
         		    BAD.liaison_serie, 
         		    BAD.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAD.site_gestion
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GBAD, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_BAD BAD, 
         	CFT..EQT_MODBAD MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAD.type        = XDC_EQT_BAD
                and GBAD.maitre      = GSRV.numero
                and GBAD.type_maitre = XDC_MAI_BAD
                and GBAD.eqt_supprime = XDC_FAUX
                and BAD.numero       = GBAD.numero
                and GMOD.type        = XDC_EQT_MODBAD
                and GMOD.maitre      = GBAD.numero
                and GMOD.type_maitre = XDC_MAI_MODBAD
                and MOD.numero       = GMOD.numero
             
             order by BAD.numero, BAD.type   

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct BAD.numero, 
            		    BAD.type, 
         		    GSRV.nom, 
         		    BAD.liaison_serie, 
         		    BAD.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GBAD.site_gestion
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GBAD, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_BAD BAD, 
         	CFT..EQT_MODBAD MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GBAD.type        = XDC_EQT_BAD
                and GBAD.maitre      = GSRV.numero
                and GBAD.type_maitre = XDC_MAI_BAD
                and GBAD.eqt_supprime = XDC_FAUX
                and BAD.numero       = GBAD.numero
                and GMOD.type        = XDC_EQT_MODBAD
                and GMOD.maitre      = GBAD.numero
                and GMOD.type_maitre = XDC_MAI_MODBAD
                and MOD.numero       = GMOD.numero
             
             order by BAD.numero, BAD.type   

         end
             
      end

#endif      
      
   end
      
   return XDC_OK
         
go  
