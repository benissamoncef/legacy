/*E*/
/*Fichier : $Id: xzao70sp.prc,v 1.5 2018/10/08 14:35:42 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/10/08 14:35:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao70sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao70sp.h" */

/* static char *version = "@(#)xzao70.c	1.1 23/18/94 : xzao70" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un type PMV dans la table TYP_PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO70
go

create procedure XZAO70
   @va_Basedd_in        	char(3),
   @va_NomType_in		char(25),
   @va_NombreLignes_in		tinyint,
   @va_NombreCaracteres_in	tinyint,
   @va_Flash_in			bit,
   @va_Temperature_in		bit,
   @va_Resultat_out		int output,
   @va_Picto_in			tinyint
   as 

/* 
*
* SP	XZAO70SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomType_in
* XDY_Octet	va_NombreLignes_in	
* XDY_Octet	va_NombreCaracteres_in
* XDY_Booleen	va_Flash_in
* XDY_Booleen	va_Temperature_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un type de PMV en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table TYP_PMV (TYPE PMV)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* LCL		05/07/17	: Modif PRA
* LCL		11/07/18	: correction syntaxe from
* LCL		02/10/18	: suppression SAGA DEM1306
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumPMV		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomType_in = null 
   or @va_NombreLignes_in = null 
   or @va_NombreCaracteres_in = null  
   or @va_Picto_in = null  
      
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
         
         select @vl_NumPMV = numero from CFG..TYP_PMV
           where nom  = @va_NomType_in
             
         /*A
         ** Le type de PMV n'existe pas
         */
         
         if @vl_NumPMV = NULL
         begin
            
            /*A
            ** Insertion dans la table du nouveau type PMV
            */
            
            select @vl_Numero = max(numero) from CFG..TYP_PMV
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFG..TYP_PMV (numero, nom, lignes, caracteres, flash, temperature, picto)
            values ( @vl_Numero + 1, @va_NomType_in, @va_NombreLignes_in, @va_NombreCaracteres_in, @va_Flash_in, @va_Temperature_in , @va_Picto_in) 
            
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
                  
         else
         
         /*A
         ** Le type de PMV existe deja
         */ 
         begin
           
            /*A
            ** Modification dans la table du type de PMV
            */
            
            update CFG..TYP_PMV
    	    set nom         = @va_NomType_in,
                lignes      = @va_NombreLignes_in,
                caracteres  = @va_NombreCaracteres_in,
                flash       = @va_Flash_in,
                temperature = @va_Temperature_in,
		picto 		= @va_Picto_in
            where numero = @vl_NumPMV  
    
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
         
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin
         
         select @vl_NumPMV = numero from CFT..TYP_PMV
           where nom         = @va_NomType_in
             
         /*A
         ** Le type de PMV n'existe pas
         */
         
         if @vl_NumPMV = NULL
         begin
            
            /*A
            ** Insertion dans la table du nouveau type PMV
            */
            
            select @vl_Numero = max(numero) from CFT..TYP_PMV
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFT..TYP_PMV (numero, nom, lignes, caracteres, flash, temperature, picto)
            values ( @vl_Numero + 1, @va_NomType_in, @va_NombreLignes_in, @va_NombreCaracteres_in, @va_Flash_in, @va_Temperature_in , @va_Picto_in) 
            
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
                  
         else
         
         /*A
         ** Le type de PMV existe deja
         */ 
         begin
           
            /*A
            ** Modification dans la table du type de PMV
            */
            
            update CFT..TYP_PMV
    	    set nom         = @va_NomType_in,
                lignes      = @va_NombreLignes_in,
                caracteres  = @va_NombreCaracteres_in,
                flash       = @va_Flash_in,
                temperature = @va_Temperature_in,
		picto	    = @va_Picto_in
            where numero = @vl_NumPMV  
    
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
         
      end
      
#endif

      end
   end     
      
   return XDC_OK
         
go  
