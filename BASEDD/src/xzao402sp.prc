/*E*/
/* Fichier : $Id: xzao402sp.prc,v 1.5 2018/11/05 17:35:13 devgfi Exp $        $Revision: 1.5 $        $Date: 2018/11/05 17:35:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao402sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	05 Fev 1996	: Creation (DEM/1395)
* JMG		22/06/11	: remontee si num LS > = 0  1.3
* JPL		16/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao402sp.h" */

/* static char *version = "@(#)xzao402.c	1.1 23/18/94 : xzao402" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des equipements Temps De Parcours pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO402' and type = 'P')
	drop procedure XZAO402
go


create procedure XZAO402
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO402SP
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
* Retourne la liste des equipements Temps De Parcours de la Machine 
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
* Retourne les parametres numero de l'equipements Temps De Parcours, nom du ST, numero de liaison , adresse rgs, type
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
            ** Recherche de la liste des equipements Temps De Parcours associes a plusieurs serveurs
            */
         
           select distinct TDP.numero,
        	   	   LIB.libelle,
        		   TDP.rgs, 
                           TDP.liaison_serie,
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_GEN GEN3, 
        	   CFG..EQT_TDP TDP,
        	   CFG..RES_AUT AUT,
        	   CFG..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.maitre      = @vl_Machine
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = XDC_EQT_TDP
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and TDP.numero       = GEN3.numero
               and TDP.liaison_serie>= 0
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = XDC_EQT_TDP
            
             order by GEN3.sens, GEN3.autoroute, GEN3.PR, TDP.numero   
            
         end
         else
         begin
            /*A
            ** Recherche de la liste des equipements Temps De Parcours associes a plusieurs serveurs
            */
           select distinct TDP.numero,
        	   	   LIB.libelle,
        		   TDP.rgs, 
                           TDP.liaison_serie,
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom
        
           from    CFG..EQT_GEN GEN, 
        	   CFG..EQT_GEN GEN2, 
        	   CFG..EQT_GEN GEN3, 
        	   CFG..EQT_TDP TDP,
        	   CFG..RES_AUT AUT,
        	   CFG..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = XDC_EQT_TDP
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and TDP.numero       = GEN3.numero
               and TDP.liaison_serie>= 0
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = XDC_EQT_TDP
            
             order by GEN3.sens, GEN3.autoroute, GEN3.PR, TDP.numero   
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
            ** Recherche de la liste des TDP associes a plusieurs serveurs
            */
         
           select distinct TDP.numero,
        	   	   LIB.libelle,
        		   TDP.rgs, 
                           TDP.liaison_serie,
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_GEN GEN3, 
        	   CFT..EQT_TDP TDP,
        	   CFT..RES_AUT AUT,
        	   CFT..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.maitre      = @vl_Machine
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = XDC_EQT_TDP
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.maitre      = GEN2.numero
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and TDP.numero       = GEN3.numero
               and TDP.liaison_serie>= 0
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = XDC_EQT_TDP
            
             order by GEN3.sens, GEN3.autoroute, GEN3.PR, TDP.numero   
                   
         end
         else
         begin
            /*A
            ** Recherche de la liste des equipements Temps De Parcours associes a plusieurs serveurs
            */
         
           select distinct TDP.numero,
        	   	   LIB.libelle,
        		   TDP.rgs, 
                           TDP.liaison_serie,
        	   	   GEN3.autoroute, 
        	   	   AUT.nom, 
        	   	   GEN3.PR, 
        	   	   GEN3.sens,
        	   	   GEN2.nom, 
        	   	   GEN.nom
        
           from    CFT..EQT_GEN GEN, 
        	   CFT..EQT_GEN GEN2, 
        	   CFT..EQT_GEN GEN3, 
        	   CFT..EQT_TDP TDP,
        	   CFT..RES_AUT AUT,
        	   CFT..LIB_PRE LIB
          
             where GEN.type         = XDC_EQT_MAC
               and GEN2.type        = XDC_EQT_SRV
               and GEN2.type_maitre = XDC_MAI_SRV
               and GEN3.type        = XDC_EQT_TDP
               and GEN3.type_maitre = XDC_EQT_SRV
               and GEN3.eqt_supprime = XDC_FAUX
               and GEN3.maitre      = GEN2.numero
               and GEN.numero       = GEN2.maitre
               and GEN2.numero      = GEN3.maitre
               and TDP.numero       = GEN3.numero
               and TDP.liaison_serie>= 0
               and AUT.numero       = GEN3.autoroute
               and LIB.notyp        = TYP_EQT
               and LIB.code         = XDC_EQT_TDP
            
             order by GEN3.sens, GEN3.autoroute, GEN3.PR, TDP.numero   
                   
         end
      end

#endif      
   end
      
   return XDC_OK
         
go  
