/*E*/
/* Fichier : $Id: xzao64sp.prc,v 1.8 2018/10/19 14:57:53 devgfi Exp $        $Revision: 1.8 $        $Date: 2018/10/19 14:57:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao64sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	16 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* volcic	11 Nov 1994	: Inversion insert : 1) MAC 2) EQT (v1.3)
* volcic	30 Nov 1994	: Suppr begin tran / commit (v1.4)
* JMG		13/11/07 	: ajout site de gestion DEM715  1.5
* LCL		05/07/17	: modif PRA  1.6
* LCL	11/07/18        : correction syntaxe from  1.7
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.8
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao64sp.h" */

/* static char *version = "@(#)xzao64.c	1.1 23/18/94 : xzao64" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une machine dans la table EQT_MAC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;64
   @va_Basedd_in		char(3),
   @va_NomMachine_in	char(10),
   @va_NomDistrict_in	char(25),
   @va_Resultat_out		int output
as 

/* 
*
* SP	XZAO64SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_Nom	va_NomMachine_in
* XDY_NomSite	va_NomDistrict_in			
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
* Insertion dans la table EQT_MAC
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_District		tinyint,
   @vl_Machine		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_NomMachine_in = null or @va_NomDistrict_in = null 
      
      return XDC_ARG_INV
   
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
         
         select @vl_District = numero from CFG..RES_DIS
           where nom = @va_NomDistrict_in
         
         /*A
         ** Le district existe
         */
         
         if @vl_District <> null    
         begin  
           
            select @vl_Machine = numero from CFG..EQT_GEN
              where nom  = @va_NomMachine_in
                and type = XDC_EQT_MAC
          
            /*A
            ** La machine existe comme equipement
            */
              
            if @vl_Machine <> null
            begin
         
               /*A
               **  Modification de la table EQT_MAC
	       */
               
               update CFG..EQT_MAC
               set district = @vl_District
               where numero  = @vl_Machine
            
               /*A
               ** Si Erreur de modification
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
         
            else
         
            /*A
            ** La machine n'existe pas comme equipement
            */
         
            begin
         
               select @vl_Numero = max(numero) from CFG..EQT_GEN
                 where type = XDC_EQT_MAC
              
               if  @vl_Numero = null
                 select @vl_Numero = 0  
            
               /*A
               **  Insertion dans la table EQT_MAC
	       */
               
               insert into CFG..EQT_MAC
               values ( @vl_Numero + 1, @vl_District, null )
            
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end
            
               /*A
               **  Insertion dans la table EQT_GEN
	       */
               
               insert into CFG..EQT_GEN (
                             type,
                             numero,
                             maitre,
                             type_maitre,
                             autoroute,
                             PR,
                             sens,
                             nom,
                             site_gestion
                           )
               values ( XDC_EQT_MAC, @vl_Numero + 1, null, null, null, null, null, @va_NomMachine_in , null)
            
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end
            
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_AJOUT
               */
         
               select @va_Resultat_out = XDC_AJOUT
               
            end
         end   
         
         else
         
         /*a
         ** Le district n'existe pas
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
         
         select @vl_District = numero from CFT..RES_DIS
           where nom = @va_NomDistrict_in
         
         /*A
         ** Le district existe
         */
         
         if @vl_District <> null    
         begin  
           
            select @vl_Machine = numero from CFT..EQT_GEN
              where nom  = @va_NomMachine_in
                and type = XDC_EQT_MAC
          
            /*A
            ** La machine existe comme equipement
            */
              
            if @vl_Machine <> null
            begin
         
               /*A
               **  Modification de la table EQT_MAC
	       */
               
               update CFT..EQT_MAC
               set district = @vl_District
               where numero  = @vl_Machine
            
               /*A
               ** Si Erreur de modification
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
         
            else
         
            /*A
            ** La machine n'existe pas comme equipement
            */
         
            begin
         
               /*A
               **  Insertion dans la table EQT_GEN
	       */
               
               select @vl_Numero = max(numero) from CFT..EQT_GEN
                 where type = XDC_EQT_MAC
              
               if  @vl_Numero = null
                 select @vl_Numero = 0  
            
               insert into CFT..EQT_GEN (
                             type,
                             numero,
                             maitre,
                             type_maitre,
                             autoroute,
                             PR,
                             sens,
                             nom,
                             site_gestion
                           )
               values ( XDC_EQT_MAC, @vl_Numero + 1, null, null, null, null, null, @va_NomMachine_in , null)
            
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end
            
               /*A
               **  Insertion dans la table EQT_MAC
	       */
               
               insert into CFT..EQT_MAC
               values ( @vl_Numero + 1, @vl_District, null )
            
               /*A
               ** Erreur d'insertion
               */
                     
               if @@rowcount <> 1
               begin
                     
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                  */
         
                  select @va_Resultat_out = XDC_AJOUT
                        
                  return XDC_NOK
               end
            
               /*A
               ** Affectation du parametre de sortie Resultat a XDC_AJOUT
               */
         
               select @va_Resultat_out = XDC_AJOUT
               
            end
         end   
         
         else
         
         /*a
         ** Le district n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         
            select @va_Resultat_out = XDC_ECHEC
         
      end
      
#endif      
      end
   end
      
   return XDC_OK
         
go  
