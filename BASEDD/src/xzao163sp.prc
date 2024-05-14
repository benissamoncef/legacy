/*E*/
/* Fichier : $Id: xzao163sp.prc,v 1.6 2018/11/06 09:43:25 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/11/06 09:43:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao163sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
* mismer	08 Oct 1996	: Modif si arg Machine = NULL => liste de tous les BRA (DEM/1228) (v1.3)
* mismer	20 Jan 1998	: Ajout FAU et FAC (DEM/) (v1.4)
* JMG	13/11/07 : ajout site de gestion DEM715  1.5
* JPL	17/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.6
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao163sp.h" */

/* static char *version = "@(#)xzao163.c	1.1 23/18/94 : xzao163" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des NAV connectees avec ses modules
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;163
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO163SP
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
* Retourne la liste des NAV connectees avec ses modules
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
         
            select distinct NAV.numero, 
            		    NAV.type, 
         		    GSRV.nom, 
         		    NAV.liaison_serie, 
         		    NAV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
         		    GNAV.type,
			    GNAV.site_gestion
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GNAV, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_NAV NAV, 
         	CFG..EQT_MOD MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and ( GNAV.type = XDC_EQT_NAV or GNAV.type = XDC_EQT_FAU or GNAV.type = XDC_EQT_FAC )
                and GNAV.maitre      = GSRV.numero
                and GNAV.type_maitre = XDC_MAI_NAV
                and GNAV.eqt_supprime = XDC_FAUX
                and NAV.numero       = GNAV.numero
                and GMOD.type        = XDC_EQT_MOD
                and GMOD.maitre      = GNAV.numero
                and GMOD.type_maitre = XDC_MAI_MOD
                and MOD.numero       = GMOD.numero
             
             order by GNAV.type, NAV.numero, NAV.type 

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct NAV.numero, 
            		    NAV.type, 
         		    GSRV.nom, 
         		    NAV.liaison_serie, 
         		    NAV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
         		    GNAV.type,
			    GNAV.site_gestion
         
            from   CFG..EQT_GEN GMAC, 
         	CFG..EQT_GEN GSRV, 
         	CFG..EQT_GEN GNAV, 
         	CFG..EQT_GEN GMOD, 
         	CFG..EQT_NAV NAV, 
         	CFG..EQT_MOD MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and ( GNAV.type = XDC_EQT_NAV or GNAV.type = XDC_EQT_FAU or GNAV.type = XDC_EQT_FAC )
                and GNAV.maitre      = GSRV.numero
                and GNAV.type_maitre = XDC_MAI_NAV
                and GNAV.eqt_supprime = XDC_FAUX
                and NAV.numero       = GNAV.numero
                and GMOD.type        = XDC_EQT_MOD
                and GMOD.maitre      = GNAV.numero
                and GMOD.type_maitre = XDC_MAI_MOD
                and MOD.numero       = GMOD.numero
             
             order by GNAV.type, NAV.numero, NAV.type   

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
         
            select distinct NAV.numero, 
            		    NAV.type, 
         		    GSRV.nom, 
         		    NAV.liaison_serie, 
         		    NAV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GNAV.site_gestion
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GNAV, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_NAV NAV, 
         	CFT..EQT_MOD MOD
                 
              where GMAC.numero      = @vl_Machine
                and GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GNAV.type        = XDC_EQT_NAV
                and GNAV.maitre      = GSRV.numero
                and GNAV.type_maitre = XDC_MAI_NAV
                and GNAV.eqt_supprime = XDC_FAUX
                and NAV.numero       = GNAV.numero
                and GMOD.type        = XDC_EQT_MOD
                and GMOD.maitre      = GNAV.numero
                and GMOD.type_maitre = XDC_MAI_MOD
                and MOD.numero       = GMOD.numero
             
             order by NAV.numero, NAV.type   

         end
         else
         begin

            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
            select distinct NAV.numero, 
            		    NAV.type, 
         		    GSRV.nom, 
         		    NAV.liaison_serie, 
         		    NAV.rgs, 
         		    MOD.numero, 
         		    MOD.code,
			    GNAV.site_gestion
         
            from   CFT..EQT_GEN GMAC, 
         	CFT..EQT_GEN GSRV, 
         	CFT..EQT_GEN GNAV, 
         	CFT..EQT_GEN GMOD, 
         	CFT..EQT_NAV NAV, 
         	CFT..EQT_MOD MOD
                 
              where GMAC.type        = XDC_EQT_MAC
                and GSRV.type        = XDC_EQT_SRV
                and GSRV.maitre      = GMAC.numero
                and GSRV.type_maitre = XDC_MAI_SRV
                and GNAV.type        = XDC_EQT_NAV
                and GNAV.maitre      = GSRV.numero
                and GNAV.type_maitre = XDC_MAI_NAV
                and GNAV.eqt_supprime = XDC_FAUX
                and NAV.numero       = GNAV.numero
                and GMOD.type        = XDC_EQT_MOD
                and GMOD.maitre      = GNAV.numero
                and GMOD.type_maitre = XDC_MAI_MOD
                and MOD.numero       = GMOD.numero
             
             order by NAV.numero, NAV.type   

         end
             
      end

#endif      
      
   end
      
   return XDC_OK
         
go  
