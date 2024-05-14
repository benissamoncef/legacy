/*E*/
/*Fichier : $Id: xzao780sp.prc,v 1.5 2018/11/05 17:48:01 devgfi Exp $     Release : $Revision: 1.5 $        Date : $Date: 2018/11/05 17:48:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao780sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	02/10/18	: ajout IP suppression SAGA DEM1306 1.3
* JPL	16/10/18	: Filtrage des equipements marques comme supprimes (DEM 1305)  1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao780sp.h" */

/* static char *version = "$Id: xzao780sp.prc,v 1.5 2018/11/05 17:48:01 devgfi Exp $ $Revision: 1.5 $ $Date: 2018/11/05 17:48:01 $: xzao780" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMVA pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO780' and type = 'P')
	drop procedure XZAO780
go

create procedure XZAO780
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO780SP
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
* Retourne les parametres numero de PMVA, nom du ST, numero de liaison, adresse rgs
* numero de type de PMVA
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
            ** Recherche de la liste des PMVA associes a plusieurs serveurs
            */
         
           select distinct PMVA.numero,
        	   	   GEN.nom, 
        		   PMVA.liaison_serie, 
        		   PMVA.rgs, 
        	   	   PMVA.type ,
			   GEN2.site_gestion,
			   PMVA.adresse_ip,
			   PMVA.port,
			   PMVA.id_lcr
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PMVA PMVA
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMVA
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMVA
               and GEN2.eqt_supprime = XDC_FAUX
               and PMVA.numero       = GEN2.numero
             
             order by PMVA.numero, GEN.nom, PMVA.type   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMVA associes a plusieurs serveurs
            */
           select distinct PMVA.numero,
        	   	   GEN.nom, 
        		   PMVA.liaison_serie, 
        		   PMVA.rgs, 
        	   	   PMVA.type ,
			   GEN2.site_gestion,
			   PMVA.adresse_ip,
			   PMVA.port,
			   PMVA.id_lcr

        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PMVA PMVA
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMVA
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMVA
               and GEN2.eqt_supprime = XDC_FAUX
               and PMVA.numero       = GEN2.numero
             
             order by PMVA.numero, GEN.nom, PMVA.type   
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
            ** Recherche de la liste des PMVA associes a plusieurs serveurs
            */
         
           select distinct PMVA.numero,
        		   GEN.nom, 
        		   PMVA.liaison_serie, 
        		   PMVA.rgs, 
        		   PMVA.type ,
			   GEN2.site_gestion,
			   PMVA.adresse_ip,
			   PMVA.port,
			   PMVA.id_lcr
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PMVA PMVA
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMVA
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMVA
               and GEN2.eqt_supprime = XDC_FAUX
               and PMVA.numero       = GEN2.numero
            
             order by GEN.nom, PMVA.numero, PMVA.type   
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des PMV associes a plusieurs serveurs
            */
         
           select distinct PMVA.numero,
        		   GEN.nom, 
        		   PMVA.liaison_serie, 
        		   PMVA.rgs, 
        		   PMVA.type ,
			   GEN2.site_gestion,
			   PMVA.adresse_ip,
			   PMVA.port,
			   PMVA.id_lcr
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PMVA PMVA
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PMVA
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PMVA
               and GEN2.eqt_supprime = XDC_FAUX
               and PMVA.numero       = GEN2.numero
            
             order by GEN.nom, PMVA.numero, PMVA.type   
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
