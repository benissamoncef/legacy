/*E*/
/* Fichier : $Id: xzao79sp.prc,v 1.8 2020/11/03 17:04:37 pc2dpdy Exp $        $Revision: 1.8 $        $Date: 2020/11/03 17:04:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao79sp.prc
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
* volcic	19 Sep 1995	: Modif si arg Machine = NULL => liste de tous les pictos (v1.3)
* gaborit 	25 sep 1995	: modif nom XZAO79 en XZAO;79 (1.4)
* JMG   	13/11/07	: ajout site de gestion DEM715  1.5
* JPL		16/10/18	: Filtrage des equipements marques comme supprimes (DEM 1305)  1.6
* LCL		19/01/19	: PMV IP SUPPRESSION SAGA DEM1306 1.7
* GGY		10/10/23	: passage en procedure non groupee (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao79sp.h" */

/* static char *version = "@(#)xzao79.c	1.1 23/18/94 : xzao79" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PCT pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO79' and type = 'P')
	drop procedure XZAO79
go

create procedure XZAO79
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO79SP
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
* Retourne la liste des PCT de la Machine 
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
* Retourne les parametres numero de PCT, nom du ST, numero de liaison, adresse rgs
* numero de type de PCT
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
   
   /*A
   ** Les parametres d'entree ne sont pas nuls   
   */
      
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
            ** Recherche de la liste des Pictos associes a plusieurs serveurs
            */
         
           select distinct PCT.numero,
        		   GEN.nom, 
        		   PCT.liaison_serie, 
        		   PCT.rgs, 
        		   PCT.type ,
			   GEN2.site_gestion,
			   PCT.adresse_ip,
			   PCT.port,
			   PCT.type_lcr,
			   PCT.id_lcr
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PCT PCT
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PCT
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PCT
               and GEN2.eqt_supprime = XDC_FAUX
               and PCT.numero       = GEN2.numero
             
             order by PCT.numero, GEN.nom, PCT.type   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des Pictos associes a plusieurs serveurs
            */
         
           select distinct PCT.numero,
        		   GEN.nom, 
        		   PCT.liaison_serie, 
        		   PCT.rgs, 
        		   PCT.type ,
			   GEN2.site_gestion,
			   PCT.adresse_ip,
			   PCT.port,
			   PCT.type_lcr,
			   PCT.id_lcr
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_PCT PCT
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PCT
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PCT
               and GEN2.eqt_supprime = XDC_FAUX
               and PCT.numero       = GEN2.numero
             
             order by PCT.numero, GEN.nom, PCT.type   
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
            ** Recherche de la liste des Pictos associes a plusieurs serveurs
            */
         
           select distinct PCT.numero,
        		   GEN.nom, 
        		   PCT.liaison_serie, 
        		   PCT.rgs, 
        		   PCT.type ,
			   GEN2.site_gestion,
			   PCT.adresse_ip,
			   PCT.port,
			   PCT.type_lcr,
			   PCT.id_lcr
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PCT PCT
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.maitre       = @vl_Machine
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PCT
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PCT
               and GEN2.eqt_supprime = XDC_FAUX
               and PCT.numero       = GEN2.numero
             
             order by PCT.numero, GEN.nom, PCT.type   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des Pictos associes a plusieurs serveurs
            */
         
           select distinct PCT.numero,
        		   GEN.nom, 
        		   PCT.liaison_serie, 
        		   PCT.rgs, 
        		   PCT.type ,
			   GEN2.site_gestion,
			   PCT.adresse_ip,
			   PCT.port,
			   PCT.type_lcr,
			   PCT.id_lcr
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_PCT PCT
          
             where GEN.type         = XDC_EQT_SRV
               and GEN.type_maitre  = XDC_MAI_SRV
               and GEN2.type        = XDC_EQT_PCT
               and GEN2.maitre      = GEN.numero
               and GEN2.type_maitre = XDC_MAI_PCT
               and GEN2.eqt_supprime = XDC_FAUX
               and PCT.numero       = GEN2.numero
             
             order by PCT.numero, GEN.nom, PCT.type   
         end          
      end
            
#endif      
   end
      
   return XDC_OK
         
go  
