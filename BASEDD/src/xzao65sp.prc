/*E*/
/*Fichier : $Id: xzao65sp.prc,v 1.5 2018/07/11 09:35:13 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 09:35:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao65sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao65sp.h" */

/* static char *version = "@(#)xzao65.c	1.1 23/18/94 : xzao65" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Suppleant dans la table MACHINE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;65
   @va_Basedd_in        	char(3),
   @va_NomMachine_in		char(10),
   @va_NomSuppleant_in		char(10),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO65SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_Nom	va_NomMachine_in
* XDY_NomEqt	va_NomSuppleant_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un suppleant a une machine en base
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
* Insertion dans la table RES_PEN (PENMENT HOMOGENE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	16 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* LCL		05/07/17	: Modif PRA
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Machine1		smallint,
   @vl_Machine2		smallint,
   @Numero			tinyint,
   @vl_Site			char(10)

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = NULL
   or @va_NomMachine_in = NULL
      
      return XDC_NOK
   
   else   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin 
         
         select @vl_Machine1 = numero from CFG..EQT_GEN
            where nom  = @va_NomMachine_in
              and type = XDC_EQT_MAC
          
         select @vl_Machine2 = numero from CFG..EQT_GEN
            where nom = @va_NomSuppleant_in
              and type = XDC_EQT_MAC 
            
         
         /*A
         ** La machine existe
         */
              
         if @vl_Machine1 <> NULL
         begin
         
            /*A
            **  Modification de la table EQT_MAC
	    */
               
            update CFG..EQT_MAC
            set suppleant = @vl_Machine2
            where numero  = @vl_Machine1
            
            /*A
            ** Erreur de modification
            */
                     
            if @@rowcount <> 1
            begin
                     
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_MODIF
               */
         
               select @va_Resultat_out = XDC_MODIF
                        
               return XDC_NOK
            end
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_MODIF
            */
         
            select @va_Resultat_out = XDC_MODIF
               
         end
         
         /*A
         ** La machine n'existe pas
         */
         
         else
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */ 
      
      begin 
         
         select @vl_Machine1 = numero from CFT..EQT_GEN
            where nom = @va_NomMachine_in
          
         select @vl_Machine2 = numero from CFT..EQT_GEN
            where nom = @va_NomSuppleant_in 
            
         
         /*A
         ** La machine existe
         */
              
         if @vl_Machine1 <> NULL
         begin
         
            /*A
            **  Modification de la table EQT_MAC
	    */
               
            update CFT..EQT_MAC
            set suppleant = @vl_Machine2
            where numero  = @vl_Machine1
            
            /*A
            ** Erreur de modification
            */
                     
            if @@rowcount <> 1
            begin
                     
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_MODIF
               */
         
               select @va_Resultat_out = XDC_MODIF
                        
               return XDC_NOK
            end
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_MODIF
            */
         
            select @va_Resultat_out = XDC_MODIF
               
         end
         
         /*A
         ** La machine n'existe pas
         */
         
         else
            
            /*A
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
      end

#endif
      
      end          
   end
      
   return XDC_OK
         
go  
