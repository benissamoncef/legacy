/*E*/
/*Fichier : $Id: xzao116sp.prc,v 1.2 1995/01/09 13:15:52 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:15:52 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao116sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao116sp.h" */

/* static char *version = "@(#)xzao116.c	1.1 23/18/94 : xzao116" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des MAA pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;116
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10)
   as 

/* 
*
* SP	XZAO116SP
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
* Retourne la liste des MAA de la Machine 
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
* Retourne XZAOT_MAA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
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
   or @va_NomMachine_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls alors XDC_ARG_INV  
   */
      
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
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
         
        select distinct MAA.numero,
        		GSRV.nom, 
        		MAA.liaison_serie, 
        		MAA.rgs, 
        		MAA.machine_operateur,
        		MAA.numero_repondeur 
        from CFG..EQT_GEN GSRV, 
        	CFG..EQT_GEN GMAA, 
        	CFG..EQT_MAA MAA
          
          where GSRV.type         = XDC_EQT_SRV
            and GSRV.maitre       = @vl_Machine
            and GSRV.type_maitre  = XDC_MAI_SRV
            and GMAA.type         = XDC_EQT_MAA
            and GMAA.maitre       = GSRV.numero
            and GMAA.type_maitre  = XDC_MAI_MAA
            and MAA.numero        = GMAA.numero
            
          order by GSRV.nom, MAA.numero   
            
                   
      end
      
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
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
         
        select distinct MAA.numero,
        		GSRV.nom, 
        		MAA.liaison_serie, 
        		MAA.rgs, 
        		MAA.machine_operateur,
        		MAA.numero_repondeur 
        from CFT..EQT_GEN GSRV, 
        	CFT..EQT_GEN GMAA, 
        	CFT..EQT_MAA MAA
          
          where GSRV.type         = XDC_EQT_SRV
            and GSRV.maitre       = @vl_Machine
            and GSRV.type_maitre  = XDC_MAI_SRV
            and GMAA.type         = XDC_EQT_MAA
            and GMAA.maitre       = GSRV.numero
            and GMAA.type_maitre  = XDC_MAI_MAA
            and MAA.numero        = GMAA.numero
            
          order by GSRV.nom, MAA.numero   
            
                   
      end

#endif      
      
      
   end
      
   return XDC_OK
         
go  
