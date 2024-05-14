/*E*/
/*  Fichier : @(#)xzao806sp.prc	1.2     Release : 1.2        Date : 14/12/11
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat451sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	14/12/11 	: Creation (DEM/1016)

------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao806sp.h" */

/* static char *version = "@(#)xzao806sp.prc	1.2 1.2 11/19/07 : xzao806" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PRV connectees avec ses modules
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO806
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO806SP
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
* Retourne la liste des PRV connectees avec ses modules
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
         
            select distinct PRV.numero, 
            		    PRV.type, 
         		    GSRV.nom, 
         		    PRV.liaison_serie, 
         		    PRV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GPRV.site_gestion
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GPRV, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_PRV PRV, 
         	CFG..EQT_MODPRV MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GPRV.type = XDC_EQT_PRV 
                and GPRV.maitre      = GSRV.numero
                and GPRV.type_maitre = XDC_MAI_PRV
                and PRV.numero       = GPRV.numero
                and GMOD.type        = XDC_EQT_MODPRV
                and GMOD.maitre      = GPRV.numero
                and GMOD.type_maitre = XDC_MAI_MODPRV
                and MOD.numero       = GMOD.numero
             
             order by PRV.numero, PRV.type 

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct PRV.numero, 
            		    PRV.type, 
         		    GSRV.nom, 
         		    PRV.liaison_serie, 
         		    PRV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GPRV.site_gestion
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GPRV, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_PRV PRV, 
         	CFG..EQT_MODPRV MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GPRV.type = XDC_EQT_PRV 
                and GPRV.maitre      = GSRV.numero
                and GPRV.type_maitre = XDC_MAI_PRV
                and PRV.numero       = GPRV.numero
                and GMOD.type        = XDC_EQT_MODPRV
                and GMOD.maitre      = GPRV.numero
                and GMOD.type_maitre = XDC_MAI_MODPRV
                and MOD.numero       = GMOD.numero
             
             order by PRV.numero, PRV.type   

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
         
            select distinct PRV.numero, 
            		    PRV.type, 
         		    GSRV.nom, 
         		    PRV.liaison_serie, 
         		    PRV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GPRV.site_gestion
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GPRV, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_PRV PRV, 
         	CFT..EQT_MODPRV MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GPRV.type        = XDC_EQT_PRV
                and GPRV.maitre      = GSRV.numero
                and GPRV.type_maitre = XDC_MAI_PRV
                and PRV.numero       = GPRV.numero
                and GMOD.type        = XDC_EQT_MODPRV
                and GMOD.maitre      = GPRV.numero
                and GMOD.type_maitre = XDC_MAI_MODPRV
                and MOD.numero       = GMOD.numero
             
             order by PRV.numero, PRV.type   

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct PRV.numero, 
            		    PRV.type, 
         		    GSRV.nom, 
         		    PRV.liaison_serie, 
         		    PRV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GPRV.site_gestion
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GPRV, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_PRV PRV, 
         	CFT..EQT_MODPRV MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GPRV.type        = XDC_EQT_PRV
                and GPRV.maitre      = GSRV.numero
                and GPRV.type_maitre = XDC_MAI_PRV
                and PRV.numero       = GPRV.numero
                and GMOD.type        = XDC_EQT_MODPRV
                and GMOD.maitre      = GPRV.numero
                and GMOD.type_maitre = XDC_MAI_MODPRV
                and MOD.numero       = GMOD.numero
             
             order by PRV.numero, PRV.type   

         end
             
      end

#endif      
      
   end
      
   return XDC_OK
         
go  
