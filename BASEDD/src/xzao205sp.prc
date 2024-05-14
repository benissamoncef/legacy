/*E*/
/* Fichier : $Id: xzao205sp.prc,v 1.3 2018/11/06 09:43:48 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/06 09:43:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao205sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	04 Nov 1996	: Creation (DEM/1232)
* JMG	13/11/07 : ajout site de gestion DEM715  1.2
* JPL	17/10/18 : Filtrage des equipements LCR marques comme supprimes (DEM 1305)  1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao205sp.h" */

/* static char *version = "@(#)xzao205.c	1.1 23/18/94 : xzao205" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des equipements generiques LCR pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;205
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO205SP
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
* Retourne la liste des equipements generiques LCR de la Machine 
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
* Retourne les parametres numero de l'equipements generiques LCR, nom du ST, numero de liaison maitre et esclave , adresse rgs, type
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
            ** Recherche de la liste des equipements generiques LCR associes a plusieurs serveurs
            */
         
           select distinct LCR.numero,
        	   	   LCR.type,
        	   	   LIB.libelle,
        		   LCR.rgs, 
                           LCR.liaison_serie_maitre,
                           LCR.liaison_serie_esclave,
        		   LCR.code, 
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom,
			   GEN3.site_gestion
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_GEN GEN3, 
        	   CFG..EQT_LCR LCR,
        	   CFG..RES_AUT AUT,
        	   CFG..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.maitre      = @vl_Machine
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = LCR.type
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and LCR.numero       = GEN3.numero
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = LCR.type
            
             order by LCR.type, GEN3.sens, GEN3.autoroute, GEN3.PR, LCR.numero   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des equipements generiques LCR associes a plusieurs serveurs
            */
           select distinct LCR.numero,
        	   	   LCR.type,
        	   	   LIB.libelle,
        		   LCR.rgs, 
                           LCR.liaison_serie_maitre,
                           LCR.liaison_serie_esclave,
        		   LCR.code,
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom,
			   GEN3.site_gestion
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_GEN GEN3, 
        	   CFG..EQT_LCR LCR,
        	   CFG..RES_AUT AUT,
        	   CFG..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = LCR.type
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and LCR.numero       = GEN3.numero
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = LCR.type
            
             order by LCR.type, GEN3.sens, GEN3.autoroute, GEN3.PR, LCR.numero   
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
            ** Recherche de la liste des LCR associes a plusieurs serveurs
            */
         
           select distinct LCR.numero,
        	   	   LCR.type,
        	   	   LIB.libelle,
        		   LCR.rgs, 
                           LCR.liaison_serie_maitre,
                           LCR.liaison_serie_esclave,
        		   LCR.code, 
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom,
			   GEN3.site_gestion
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_GEN GEN3, 
        	   CFT..EQT_LCR LCR,
        	   CFT..RES_AUT AUT,
        	   CFT..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.maitre      = @vl_Machine
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = LCR.type
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and LCR.numero       = GEN3.numero
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = LCR.type
            
             order by LCR.type, GEN3.sens, GEN3.autoroute, GEN3.PR, LCR.numero   
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des equipements generiques LCR associes a plusieurs serveurs
            */
         
           select distinct LCR.numero,
        	   	   LCR.type,
        	   	   LIB.libelle,
        		   LCR.rgs, 
                           LCR.liaison_serie_maitre,
                           LCR.liaison_serie_esclave,
        		   LCR.code, 
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom,
			   GEN3.site_gestion
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_GEN GEN3, 
        	   CFT..EQT_LCR LCR,
        	   CFT..RES_AUT AUT,
        	   CFT..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = LCR.type
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and LCR.numero       = GEN3.numero
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = LCR.type
            
             order by LCR.type, GEN3.sens, GEN3.autoroute, GEN3.PR, LCR.numero   
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
