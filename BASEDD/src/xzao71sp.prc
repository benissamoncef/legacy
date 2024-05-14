/*E*/
/*Fichier : $Id: xzao71sp.prc,v 1.4 2018/07/11 09:40:00 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 09:40:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao71sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao71sp.h" */

/* static char *version = "@(#)xzao71.c	1.1 23/18/94 : xzao71" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un type picto dans la table TYP_PCT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;71
   @va_Basedd_in           	char(3),
   @va_NomType_in          	char(25),
   @va_NombreCaracteres_in 	tinyint,
   @va_FichierPanneau_in	char(10),
   @va_TypePnc_in			tinyint,
   @va_FichierPnc_in		char (10),
   @va_Resultat_out        	int output
   as 

/* 
*
* SP	XZAO71SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt	va_NomType_in
* XDY_Octet		va_NombreCaracteres_in
* XDY_NomFich	va_FichierPanneau_in	
* XDY_Octet		va_TypePnc_in	
* XDY_NomFich	va_FichierPnc_in	
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un type picto en base
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
* Insertion ou modification dans la table TYP_PCT (TYPE PICTO)
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
* GGY   	22/06/23	: Ajout parametre va_TypePnc_in et va_FichierPnc_in (DEM-473)
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumPicto		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomType_in = null 
   or @va_NombreCaracteres_in = null 
   or @va_FichierPanneau_in = null  
   or @va_TypePnc_in = null 
   
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
      
      if @va_Basedd_in = "CFG"
      begin
         
         select @vl_NumPicto = numero from CFG..TYP_PCT
           where nom         = @va_NomType_in
             
         /*A
         ** Le type de picto n'existe pas
         */
         
         if @vl_NumPicto = NULL
         begin
            
            /*A
            ** Insertion dans la table du nouveau type picto
            */
            
            select @vl_Numero = max(numero) from CFG..TYP_PCT
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFG..TYP_PCT
            values ( @vl_Numero + 1, @va_NomType_in, @va_FichierPanneau_in, @va_NombreCaracteres_in, @va_TypePnc_in, @va_FichierPnc_in ) 
            
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
            
            update CFG..TYP_PCT
    	    set nom				= @va_NomType_in,
                fichier			= @va_FichierPanneau_in,
                caracteres		= @va_NombreCaracteres_in,
				type_pnc		= @va_TypePnc_in,
				fichier_pnc		= @va_FichierPnc_in 

            where numero = @vl_NumPicto  
    
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
         
         select @vl_NumPicto = numero from CFT..TYP_PCT
           where nom         = @va_NomType_in
             
         /*A
         ** Le type de picto n'existe pas
         */
         
         if @vl_NumPicto = NULL
         begin
            
            /*A
            ** Insertion dans la table du nouveau type picto
            */
            
            select @vl_Numero = max(numero) from CFT..TYP_PCT
            
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFT..TYP_PCT
            values ( @vl_Numero + 1, @va_NomType_in, @va_FichierPanneau_in, @va_NombreCaracteres_in, @va_TypePnc_in, @va_FichierPnc_in ) 
            
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
            
            update CFT..TYP_PCT
    	    set nom				= @va_NomType_in,
                fichier			= @va_FichierPanneau_in,
                caracteres		= @va_NombreCaracteres_in,
				type_pnc		= @va_TypePnc_in,
				fichier_pnc		= @va_FichierPnc_in 

            where numero = @vl_NumPicto  
    
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
