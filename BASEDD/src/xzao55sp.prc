/*E*/
/*Fichier : $Id: xzao55sp.prc,v 1.4 1998/02/19 09:54:27 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/02/19 09:54:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao55sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao55sp.h" */

/* static char *version = "$Id: xzao55sp.prc,v 1.4 1998/02/19 09:54:27 gaborit Exp $ $Revision: 1.4 $ $Date: 1998/02/19 09:54:27 $ : xzao55" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Portion dans la table PORTION
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;55
   @va_Basedd_in        		char(3),
   @va_NomPortion_in			char(5),
   @va_NomAutoroute_in			char(4),
   @va_CodeDistrict_in			char(2),
   @va_CodeDistrictRecouvre_in		char(2),
   @va_PRdebut_in			int,
   @va_PRfin_in				int,
   @va_Resultat_out			int output
   as 

/* 
*
* SP	XZAO55SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	   	va_Basedd_in        		
* XDY_Nom	   	va_NomPortion_in			
* XDY_Nom	   	va_NomAutoroute_in			
* XDY_NomSite	   	va_CodeDistrict_in			
* XDY_NomSite	   	va_CodeDistrictRecouvre_in	
* XDY_PR	   	va_PRdebut_in				
* XDY_PR	   	va_PRfin_in				
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf		va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une portion d'autoroute a une base
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
* Insertion dans la table RES_POR (PORTION)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
* volcic	18 Oct 1994	: Modif pour tests (v1.2)
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.3)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumAutoroute	tinyint,
   @vl_NumPortion	smallint,
   @vl_NumDistrict	tinyint,
   @vl_NumDistrictRec	tinyint,
   @vl_Numero		smallint

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_NomPortion_in = null 
   or @va_NomAutoroute_in = null or @va_CodeDistrict_in	= null or @va_CodeDistrictRecouvre_in = null
   or @va_PRdebut_in = null or @va_PRfin_in = null 
      return XDC_ARG_INV
   else   
   begin
      
      /*A
      **  Insertion du nom de l'autoroute argument dans la table RES_POR de la base
      **  passee en argument, si celui ci n'existe pas
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin
         select @vl_NumAutoroute = numero from CFG..RES_AUT
            where nom = @va_NomAutoroute_in
            
         select @vl_NumDistrict = numero from CFG..RES_DIS 
             where code = @va_CodeDistrict_in
             
         select @vl_NumDistrictRec = numero from CFG..RES_DIS 
             where code = @va_CodeDistrictRecouvre_in
              
         select @vl_NumPortion = numero from CFG..RES_POR
            where nom       = @va_NomPortion_in
      end       

#ifdef CI

      else
      begin
         select @vl_NumAutoroute = numero from CFT..RES_AUT
            where nom = @va_NomAutoroute_in
            
         select @vl_NumDistrict = numero from CFT..RES_DIS 
             where code = @va_CodeDistrict_in
             
         select @vl_NumDistrictRec = numero from CFT..RES_DIS 
             where code = @va_CodeDistrictRecouvre_in
              
         select @vl_NumPortion = numero from CFT..RES_POR
            where nom = @va_NomPortion_in
      end        
              
#endif

      /*A
      ** L'autoroute et le district existent
      */
      
      if @vl_NumAutoroute <> null and @vl_NumDistrict <> null
      begin
      
      /*A
      ** La Portion n'existe pas
      */
      
      if @vl_NumPortion = null
      begin
         
         if @va_Basedd_in = XDC_BASE_CFG
         begin 
            select @vl_Numero = max(numero) from CFG..RES_POR
              
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFG..RES_POR
            values ( @vl_Numero + 1, @va_NomPortion_in, @vl_NumAutoroute, @vl_NumDistrict, @vl_NumDistrictRec, @va_PRdebut_in, @va_PRfin_in)
           
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

#ifdef CI

         else
         begin   
            select @vl_Numero = max(numero) from CFT..RES_POR
               
            if @vl_Numero = null
               select @vl_Numero = 0
            
            insert into CFT..RES_POR
            values ( @vl_Numero + 1, @va_NomPortion_in, @vl_NumAutoroute, @vl_NumDistrict, @vl_NumDistrictRec, @va_PRdebut_in, @va_PRfin_in)
            
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

#endif

      end
      else
         
         /*A
         ** Modification de la portion
         */
         
         if @va_Basedd_in = XDC_BASE_CFG
         begin 
            update CFG..RES_POR
            set nom = @va_NomPortion_in, 
                autoroute = @vl_NumAutoroute, 
                district = @vl_NumDistrict, 
                district_recouvrement = @vl_NumDistrictRec, 
                PR_debut = @va_PRdebut_in, 
                PR_fin = @va_PRfin_in
            where numero = @vl_NumPortion
            
            /*A
            ** Erreur de modification
            */
                     
            if @@rowcount > 1
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

#ifdef CI

         else
         begin   
            
            update CFT..RES_POR
            set nom = @va_NomPortion_in, 
                autoroute = @vl_NumAutoroute, 
                district = @vl_NumDistrict, 
                district_recouvrement = @vl_NumDistrictRec, 
                PR_debut = @va_PRdebut_in, 
                PR_fin = @va_PRfin_in
            where numero = @vl_NumPortion
            
            /*A
            ** Erreur de modification
            */
                     
            if @@rowcount > 1
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

#endif

      end
      
      /*A
      ** L'autoroute ou le district n'existe pas
      */
      
      else
         
         /*A
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         
         select @va_Resultat_out = XDC_ECHEC
                  
   end
      
   return XDC_OK
         
go
