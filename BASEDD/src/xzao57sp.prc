/*E*/
/*Fichier : $Id: xzao57sp.prc,v 1.5 2018/07/11 09:27:30 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 09:27:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao57sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao57sp.h" */

/* static char *version = "@(#)xzao57.c	1.1 23/18/94 : xzao57" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Noeud dans la table NOEUD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;57
   @va_Basedd_in        		char(3),
   @va_NomPortion1_in			char(5),
   @va_NomPortion2_in			char(5),
   @va_ExtremitePortion1_in		bit,
   @va_ExtremitePortion2_in		bit,
   @va_Creation_in			bit,
   @va_Resultat_out			int output
   as 

/* 
*
* SP	XZAO57SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	   va_Basedd_in        		
* XDY_Octet	   va_NomPortion1_in			
* XDY_Octet	   va_NomPortion2_in			
* XDY_Booleen	   va_ExtremitePortion1_in			
* XDY_Booleen	   va_ExtremitePortion2_in	
* XDY_Booleen	   va_Creation_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
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
* Insertion dans la table RES_NEU (NOEUD)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC (v1.2)
* niepceron	4  fev 1999	: Correection de CFG en CFT v1.3
* LCL		05/07/17	: modif PRA
* LCL		11/07/18 	: correction syntaxe from
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumNoeud		smallint,
   @vl_Portion1		smallint,
   @vl_Portion2		smallint,
   @vl_Numero		tinyint,
   @vl_OK			smallint,
   @vl_Site			char(10)


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_NomPortion1_in = null 
   or @va_NomPortion2_in = null or @va_ExtremitePortion1_in = null or @va_ExtremitePortion2_in = null
   or @va_Creation_in = null 
      return XDC_ARG_INV
   else   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
         /*A
         **  Insertion du nom de l'autoroute argument dans la table RES_NEU de la base
         **  passee en argument, si celui ci n'existe pas
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin   
            
            select @vl_Portion1 = numero from CFG..RES_POR
               where nom = @va_NomPortion1_in
         
            select @vl_Portion2 = numero from CFG..RES_POR
               where nom = @va_NomPortion2_in  
       
                
            /*B
            ** Les portions existent
            */
       
              
            if @vl_Portion1 <> null and @vl_Portion2 <> null
            begin
               
               select @vl_OK = portion1 from CFG..RES_NEU
                 where portion1 = @vl_Portion1
                   and portion2 = @vl_Portion2
               
               /*B
               ** Creation positionne et enregistrement non existant
               */
               
               if @vl_OK = null and @va_Creation_in = XDC_VRAI
               begin
                  
                  insert into CFG..RES_NEU
                  values ( @vl_Portion1, @vl_Portion2, @va_ExtremitePortion1_in, @va_ExtremitePortion2_in)
              
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
               
               
               /*B
               ** Creation non positionne et enregistrement existant
               */
               
               if @vl_OK != null and @va_Creation_in = XDC_FAUX
               begin
                  
                  delete CFG..RES_NEU
                  where portion1 = @vl_Portion1
                    and portion2 = @vl_Portion2
               
                  /*A
                  ** Erreur de suppression
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
               end     
            end
            
            else
            
            /*A
            ** Une des portions n'existe pas
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC 
         end 
      
#ifdef CI

         else
      
         /*A
         **  Insertion du nom de l'autoroute argument dans la table RES_NEU de la base
         **  passee en argument, si celui ci n'existe pas
         */   
      
         if @va_Basedd_in = XDC_BASE_CFT
         begin   
            
            select @vl_Portion1 = numero from CFT..RES_POR
               where nom = @va_NomPortion1_in
         
            select @vl_Portion2 = numero from CFT..RES_POR
               where nom = @va_NomPortion2_in  
       
                
            /*B
            ** Les portions existent
            */
       
              
            if @vl_Portion1 <> null and @vl_Portion2 <> null
            begin
               
               select @vl_OK = portion1 from CFT..RES_NEU
                 where portion1 = @vl_Portion1
                   and portion2 = @vl_Portion2
               
               /*B
               ** Creation positionne et enregistrement non existant
               */
               
               if @vl_OK = null and @va_Creation_in = XDC_VRAI
               begin
                  
                  insert into CFT..RES_NEU
                  values ( @vl_Portion1, @vl_Portion2, @va_ExtremitePortion1_in, @va_ExtremitePortion2_in)
              
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
               
               
               /*B
               ** Creation non positionne et enregistrement existant
               */
               
               if @vl_OK != null and @va_Creation_in = XDC_FAUX
               begin
                  
                  delete CFT..RES_NEU
                  where portion1 = @vl_Portion1
                    and portion2 = @vl_Portion2
               
                  /*A
                  ** Erreur de suppression
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
                        
                     return XDC_NOK
                  end
                  
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
               end     
            end
            
            /*!else*/
            
            /*A
            ** Une des portions n'existe pas
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               /*!select @va_Resultat_out = XDC_ECHEC*/ 
         end

#endif

      end
   end
      
   return XDC_OK
         
go  
