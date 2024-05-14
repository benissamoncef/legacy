/*E*/
/*Fichier : $Id: xzao159sp.prc,v 1.5 2018/07/11 10:02:13 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 10:02:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao159sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao159sp.h" */

/* static char *version = "@(#)xzao159.c	1.1 20/09/94 : xzao159" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un scenario connu dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;159
   @va_Basedd_in        	char(3),
   @va_Scenario_in		char(4),
   @va_NomType_in		T_NOM,
   @va_AutoFichier_in		bit,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO159SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Scenario		va_Scenario_in		
* XDY_Nom		va_NomType_in
* XDY_Booleen		va_AutoFichier_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un scenario connu en base
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
* Insertion ou modification dans la table EQT_SCN_CNN (TYPE_NAV)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	25 Nov 1994	: Modif pour config BASE (v1.3)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumScen		tinyint,
   @vl_NumNAV		tinyint,
   @vl_Numero		tinyint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_Scenario_in = null 
   or @va_NomType_in = null 
   or @va_AutoFichier_in = null
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
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence du type NAV et du scenario
            */
         
            select @vl_NumNAV = numero from CFG..TYP_NAV
              where nom = @va_NomType_in
                
            /*A
            ** Le type NAV existe 
            */
               
            if @vl_NumNAV <> null
            begin
             
               select @vl_NumScen = numero from CFG..EQT_SCN_NAV
                 where scenario = @va_Scenario_in
                
               /*A
               ** Le scenario existe 
               */
               
               if @vl_NumScen <> null
               begin     
                  
                  select @vl_Numero = scenario from CFG..EQT_SCN_CNN
                    where scenario = @vl_NumScen
                      and type_NAV = @vl_NumNAV
               
               
                  /*A
                  ** Le scenario connu n'existe pas dans EQT_SCN_CNN 
                  */
               
                  if @vl_Numero = null
                  begin
                  
                     /*A
                     ** Creation positionne =>
                     ** Insertion du scenario connu dans la table EQT_SCN_CNN
                     */
                     
                     if @va_Creation_in = XDC_VRAI
                     begin
                     
                        insert into CFG..EQT_SCN_CNN
                        values ( @vl_NumScen, @vl_NumNAV, @va_AutoFichier_in )
                  
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
           
                  /*A
                  ** Le scenario connu existe deja dans EQT_SCN_CNN
                  */
                     
                  begin
                     
                     /*A
                     ** Creation non positionne =>
                     ** Suppression du scenario connu dans la table EQT_SCN_CNN
                     */
                     
                     if @va_Creation_in = XDC_FAUX
                     begin
                     
                        delete CFG..EQT_SCN_CNN
                        where scenario = @vl_NumScen
                           and type_NAV = @vl_NumNAV
                  
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
               
               end
                  
               else
                     
               /*A
               ** le scenario n'existe pas =>
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
                     
                 select @va_Resultat_out = XDC_ECHEC
            end          
               
            else
               
            /*A
            ** le type NAV n'existe pas =>
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
                     
               select @va_Resultat_out = XDC_ECHEC  
            
         end
      
#ifdef CI      
      
         else
         
         /*A
         **  Travail dans la base CFT
         */   
      
         if @va_Basedd_in = XDC_BASE_CFT
         begin
         
            /*A
            ** Verification de l'existence du type NAV et du scenario
            */
         
            select @vl_NumNAV = numero from CFT..TYP_NAV
              where nom = @va_NomType_in
                
            /*A
            ** Le type NAV existe 
            */
               
            if @vl_NumNAV <> null
            begin
             
               select @vl_NumScen = numero from CFT..EQT_SCN_NAV
                 where scenario = @va_Scenario_in
                
               /*A
               ** Le scenario existe 
               */
               
               if @vl_NumScen <> null
               begin     
                  
                  select @vl_Numero = scenario from CFT..EQT_SCN_CNN
                    where scenario = @vl_NumScen
                      and type_NAV = @vl_NumNAV
               
               
                  /*A
                  ** Le scenario connu n'existe pas dans EQT_SCN_CNN 
                  */
               
                  if @vl_Numero = null
                  begin
                  
                     /*A
                     ** Creation positionne =>
                     ** Insertion du scenario connu dans la table EQT_SCN_CNN
                     */
                     
                     if @va_Creation_in = XDC_VRAI
                     begin
                     
                        insert into CFT..EQT_SCN_CNN
                        values ( @vl_NumScen, @vl_NumNAV, @va_AutoFichier_in )
                  
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
           
                  /*A
                  ** Le scenario connu existe deja dans EQT_SCN_CNN
                  */
                     
                  begin
                     
                     /*A
                     ** Creation non positionne =>
                     ** Suppression du scenario connu dans la table EQT_SCN_CNN
                     */
                     
                     if @va_Creation_in = XDC_FAUX
                     begin
                     
                        delete CFT..EQT_SCN_CNN
                        where scenario = @vl_NumScen
                           and type_NAV = @vl_NumNAV
                  
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
               
               end
                  
               else
                     
               /*A
               ** le scenario n'existe pas =>
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
                     
                 select @va_Resultat_out = XDC_ECHEC
            end          
               
            else
               
            /*A
            ** le type NAV n'existe pas =>
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
                     
               select @va_Resultat_out = XDC_ECHEC  
            
         end
      
#endif
      
      end
   end         
     
   return XDC_OK
         
go  
