/*E*/
/*Fichier : $Id: xzao190sp.prc,v 1.6 2018/07/11 10:43:20 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 10:43:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao190sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* description des equipement du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa 	19 jan 1996 : Creation (V1.2)
* Torregrossa 	22 avr 1996 : Correction numero maitre a null (V1.3-1.4)
* LCL           11/07/18	: correction syntaxe from
* GGY			26/09/23	: Ajout district CA (DEM483)
* GGY			17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao190sp.h" */

/* static char *version = "$Id: xzao190sp.prc,v 1.6 2018/07/11 10:43:20 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 10:43:20 $"; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un equipement fax dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;190
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_NomDistrict_in		char(25),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO190SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une MAA en base
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
* Insertion ou modification dans la table EQT_GEN 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* CT	01 dec 1995	: Creation
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Numero		smallint,
   @vl_District		tinyint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null   
   or @va_Nom_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin      
      
         select @vl_District = numero from CFG..RES_DIS
         where nom = @va_NomDistrict_in
          
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin

            /*A
            ** Verification de l'existence de l'equipement
            */
            select @vl_Numero = numero from CFG..EQT_GEN
            where type        = XDC_EQT_FAX
              	  and nom     = @va_Nom_in
              	    
             /*A
              ** L'equipement n'existe pas 
              */
               
             if @vl_Numero is null and @vl_District is not null    
             begin
                  
                  /*A
                  ** Insertion de l'equipement dans la table EQT_GEN
                  */
               
                  select @vl_Numero = isnull(max(numero),0)+1 from CFG..EQT_GEN
                  where type = XDC_EQT_FAX
                        
                  insert CFG..EQT_FAX (numero, district)
                  values ( @vl_Numero, @vl_District)
                       
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
              
                  insert CFG..EQT_GEN (type, numero, maitre, type_maitre, autoroute, PR, sens, nom)
                  values ( XDC_EQT_FAX, @vl_Numero, null, null, null, null, null, @va_Nom_in )
                       
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
	    else if @vl_Numero is not null and @vl_District is not null    
            begin
         
               /*A
               **  Modification de la table EQT_FAX
	       */
               
               update CFG..EQT_FAX
               set district = @vl_District
               where numero  = @vl_Numero
            
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
            ** L'equipement existe deja
            **
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
         
            /*A
            ** Verification de l'existance de l'equipement
            */

            select @vl_Numero = numero from CFT..EQT_GEN
            where type        = XDC_EQT_FAX
              	  and nom     = @va_Nom_in
              	    
             /*A
              ** L'equipement n'existe pas 
              */
               
	     if @vl_Numero is null and @vl_District is not null    
             begin
                  
                  /*A
                  ** Insertion de la MAA dans la table EQT_GEN
                  */
               
                  select @vl_Numero = isnull(max(numero),0)+1 from CFT..EQT_GEN
                   where type = XDC_EQT_FAX
              
                  insert CFT..EQT_GEN (type, numero, maitre, type_maitre, autoroute, PR, sens, nom)
                  values ( XDC_EQT_FAX, @vl_Numero , null, null, null,null, null, @va_Nom_in )
                       
                  /*A
                  ** Erreur d'insertion
                  */
                     
                  if @@rowcount != 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end   
                        
                  insert CFT..EQT_FAX (numero, district)
                  values ( @vl_Numero , @vl_District)
                       
                  /*A
                  ** Erreur d'insertion
                  */
                     
                  if @@rowcount != 1
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
	    else if @vl_Numero is not null and @vl_District is not null    
            begin
         
               /*A
               **  Modification de la table EQT_FAX
	       */
               
               update CFT..EQT_FAX
               set district = @vl_District
               where numero  = @vl_Numero
            
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
            ** L'equipement existe deja
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         end
         
#endif         
            
      end
   end         
     
   return XDC_OK
         
go  
