/*E*/
/* Fichier : $Id: xzao144sp.prc,v 1.9 2019/05/21 11:07:29 pc2dpdy Exp $        $Revision: 1.9 $        $Date: 2019/05/21 11:07:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao144sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* JMG		13/11/07	: ajout site de gestion DEM715  1.3
* PNI		29/05/08	: Ajout mise a jour autoroute,pr,sens v1.4
* LCL		11/07/18	: correction syntaxe from  1.6
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.7
* JPL		06/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.8
* LCL		07/05/19	: Ajout type PAU DEM1334 1.9
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao144sp.h" */

/* static char *version = "@(#)xzao144.c	1.1 20/09/94 : xzao144" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un PAU dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO144' and type = 'P')
	drop procedure XZAO144
go


create procedure XZAO144
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_NomPIRAU_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_CodeAntenne_in		char(3),
   @va_CodePAU_in		char(3),
   @va_Supprime_in		bit,
   @va_Type_in			tinyint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO144SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomEqt		va_NomPIRAU_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_CodeEqt		va_CodeAntenne_in
* XDY_CodeEqt		va_CodePAU_in
* XDY_Booleen		va_Supprime_in
* XDY_Octet		va_Type_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PAU en base
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
* Insertion ou modification dans la table EQT_PAU (EQT_PAU)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumPIRAU		smallint,
   @vl_NumPAU		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_Nom_in = null 
   or @va_NomPIRAU_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_CodeAntenne_in = null 
   or @va_CodePAU_in = null
   or @va_Supprime_in = null
   or @va_Type_in = null
   
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
         
            /*A
            ** Verification de l'existence et recherche de l'autoroute et du PAU comme equipement
            */
         
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
              
            /*
            ** Existence de l'autoroute
            */
            
            if @vl_Autoroute <> null
            begin
               
               select @vl_NumPIRAU = numero from CFG..EQT_GEN
                 where type      = XDC_EQT_RAU
                   and nom       = @va_NomPIRAU_in
               
               /*
               ** Existence du PIRAU
               */
            
               if @vl_NumPIRAU <> null
               begin
               
                  select @vl_NumPAU = numero from CFG..EQT_GEN
                    where type      = XDC_EQT_PAU
                      and nom       = @va_Nom_in    
            
            
                  /*A
                  ** Le PAU n'existe pas comme equipement
                  */
               
                  if @vl_NumPAU = null
                  begin
                     
                     /*A
                     ** Insertion du PAU dans la table EQT_GEN
                     */
               
                     select @vl_Numero = max(numero) from CFG..EQT_GEN
                       where type = XDC_EQT_PAU
              
                     if @vl_Numero = null
                        select @vl_Numero = 0  
            
                     insert into CFG..EQT_GEN (
                                   type,
                                   numero,
                                   maitre,
                                   type_maitre,
                                   autoroute,
                                   PR,
                                   sens,
                                   nom,
                                   site_gestion,
                                   eqt_supprime
                                 )
                     values ( XDC_EQT_PAU, @vl_Numero + 1, @vl_NumPIRAU, XDC_MAI_PAU, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_Nom_in, null, @va_Supprime_in)
                  
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
                     ** Insertion du PAU dans la table EQT_PAU
                     */
               
                     insert into CFG..EQT_PAU
                     values ( @vl_Numero + 1, @va_CodeAntenne_in, @va_CodePAU_in, @va_Type_in )
                  
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
                  ** Le PAU existe comme equipement
                  */
                  begin
                     
                     /*A
                     ** Modification du PAU dans la table EQT_GEN
                     */
                
                     update CFG..EQT_GEN
    	             set maitre      = @vl_NumPIRAU,
    	                 type_maitre = XDC_MAI_PAU,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        eqt_supprime = @va_Supprime_in
    	             where numero = @vl_NumPAU
    	               and type   = XDC_EQT_PAU
    	               and nom    = @va_Nom_in
                       
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
                     ** Modification du PAU dans la table EQT_PAU
                     */
                
                     update CFG..EQT_PAU
    	             set code_antenne = @va_CodeAntenne_in,
    	                 code_PAU     = @va_CodePAU_in,
			 type_PAU     = @va_Type_in
    	             where numero = @vl_NumPAU
                       
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
               
               else
          
               /*A
               ** Le PIRAU n'existe pas comme equipement
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
                     select @va_Resultat_out = XDC_ECHEC      
            end
         
            else
         
            /*A
            ** L'autoroute n'existe pas
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
            ** Verification de l'existence et recherche de l'autoroute et du PAU comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
              
            /*
            ** Existence de l'autoroute
            */
            
            if @vl_Autoroute <> null
            begin
               
               select @vl_NumPIRAU = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_RAU
                   and nom       = @va_NomPIRAU_in
               
               /*
               ** Existence du PIRAU
               */
            
               if @vl_NumPIRAU <> null
               begin
               
                  select @vl_NumPAU = numero from CFT..EQT_GEN
                    where type      = XDC_EQT_PAU
                      and nom       = @va_Nom_in    
            
            
                  /*A
                  ** Le PAU n'existe pas comme equipement
                  */
               
                  if @vl_NumPAU = null
                  begin
                     
                     /*A
                     ** Insertion du PAU dans la table EQT_GEN
                     */
               
                     select @vl_Numero = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_PAU
              
                     if @vl_Numero = null
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
                                   site_gestion,
                                   eqt_supprime
                                 )
                     values ( XDC_EQT_PAU, @vl_Numero + 1, @vl_NumPIRAU, XDC_MAI_PAU, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_Nom_in, null, @va_Supprime_in)
                  
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
                     ** Insertion du PAU dans la table EQT_PAU
                     */
               
                     insert into CFT..EQT_PAU
                     values ( @vl_Numero + 1, @va_CodeAntenne_in, @va_CodePAU_in, @va_Type_in )
                  
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
                  ** Le PAU existe comme equipement
                  */
                  begin
                     
                     /*A
                     ** Modification du PAU dans la table EQT_GEN
                     */
                
                     update CFT..EQT_GEN
    	             set maitre      = @vl_NumPIRAU,
    	                 type_maitre = XDC_MAI_PAU,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        eqt_supprime = @va_Supprime_in
    	             where numero = @vl_NumPAU
    	               and type   = XDC_EQT_PAU
    	               and nom    = @va_Nom_in
                       
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
                     ** Modification du PAU dans la table EQT_PAU
                     */
                
                     update CFT..EQT_PAU
    	             set code_antenne = @va_CodeAntenne_in,
    	                 code_PAU     = @va_CodePAU_in,
			 type_PAU     = @va_Type_in
    	             where numero = @vl_NumPAU
                       
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
               
               else
          
               /*A
               ** Le PIRAU n'existe pas comme equipement
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
                     select @va_Resultat_out = XDC_ECHEC      
            end
         
            else
         
            /*A
            ** L'autoroute n'existe pas
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
