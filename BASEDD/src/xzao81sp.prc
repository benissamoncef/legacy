/*E*/
/* Fichier : $Id: xzao81sp.prc,v 1.8 2018/11/06 19:07:05 devgfi Exp $        $Revision: 1.8 $        $Date: 2018/11/06 19:07:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao81sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	02 Dec 1994	: Suppr begin tran (v 1.3)
* JMG		13/11/07	: ajout site de gestion DEM715  1.3
* LCL		05/07/17	: Modif PRA  1.5
* LCL		11/07/18	: correction syntaxe from  1.6
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.7
* JPL		06/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.8
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao81sp.h" */

/* static char *version = "@(#)xzao81.c	1.1 23/18/94 : xzao81" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un analyseur dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO81' and type = 'P')
	drop procedure XZAO81
go


create procedure XZAO81
   @va_Basedd_in        	char(3),
   @va_NomAnalyseur_in 		char(10),
   @va_NomDAI_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PRdebut_in		int,
   @va_PRfin_in			int,
   @va_Sens_in			tinyint,
   @va_NumeroAnalyseur_in	tinyint,
   @va_VoiesSurveillees_in	tinyint,
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO81SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomAnalyseur_in
* XDY_NomEqt    va_NomDAI_in,
* XDY_Nom       va_Autoroute_in
* XDY_PR        va_PRdebut_in
* XDY_PR        va_PRfin_in
* XDY_Sens      va_Sens_in
* XDY_Octet     va_NumeroAnalyseur_in
* XDY_Octet     va_VoiesSurveillees_in			
* XDY_NomSite   va_NomSite_in
* XDY_Booleen	va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un analyseur en base
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
* Insertion ou modification dans les tables EQT_ANA (ANALYSEUR) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumAna		smallint,
   @vl_DAI			smallint,
   @vl_Numero		smallint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_NomAnalyseur_in = null 
   or @va_Autoroute_in = null 
   or @va_PRdebut_in = null 
   or @va_PRfin_in = null 
   or @va_Sens_in = null 
   or @va_NumeroAnalyseur_in = null 
   or @va_VoiesSurveillees_in = null
   or @va_NomSite_in	= null
   or @va_Supprime_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_SiteNom = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_SiteNom=XDC_SQLCI or @vl_SiteNom=XDC_SQLCA
		begin
      
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence et recherche du numero de l'autoroute
            ** et de la DAI
            */
         
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
          
            select @vl_DAI = numero from CFG..EQT_GEN
              where nom  = @va_NomDAI_in
                and type = XDC_EQT_DAI
          
	  select @vl_Site = numero from CFG..RES_DIS
	  where code = @va_NomSite_in

	      if @vl_Site = NULL
		      return XDC_ARG_INV

            /*A
            ** L'autoroute et la DAI existent
            */
            
            if @vl_Autoroute <> null and @vl_DAI <> null
            begin
         
               /*A 
               ** Insertion ou Modification de l'analyseur
               **
               ** Verification de l'existence et recherche de l'analyseur dans les equipements
               */
         
               select @vl_NumAna = numero from CFG..EQT_GEN
                 where type = XDC_EQT_ANA
                   and nom  = @va_NomAnalyseur_in
              
            
               /*A
               ** L'analyseur n'existe pas comme equipement
               */
            
               if @vl_NumAna = null
               begin
              
                  /*A
                  ** Insertion de l'analyseur dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_ANA
               
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
                  values ( XDC_EQT_ANA, @vl_Numero + 1, @vl_DAI, XDC_MAI_ANA, @vl_Autoroute, @va_PRdebut_in, @va_Sens_in, @va_NomAnalyseur_in , @vl_Site, @va_Supprime_in)
               
                  
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
                  ** Insertion de l'analyseur dans la table EQT_ANA
                  */
               
                  insert into CFG..EQT_ANA
                  values ( @vl_Numero + 1, @va_PRfin_in, @va_NumeroAnalyseur_in, @va_VoiesSurveillees_in )
               
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
               ** La Ana existe deja comme equipement
               */      
            
               begin
           
                  /*A
                  ** Modification de la Ana dans la table EQT_GEN
                  */
                
                  update CFG..EQT_GEN
    	          set maitre      = @vl_DAI,
    	              type_maitre = XDC_MAI_ANA,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PRdebut_in,
                      sens        = @va_Sens_in,
                      nom         = @va_NomAnalyseur_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumAna
                    and type   = XDC_EQT_ANA  
    
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
                  ** Modification de l'analyseur dans la table EQT_ANA
                  */
                
                  update CFG..EQT_ANA
    	          set PR_fin           = @va_PRfin_in,
                      numero_analyseur = @va_NumeroAnalyseur_in,
                      nombre_de_voies  = @va_VoiesSurveillees_in
                  where numero = @vl_NumAna
                 
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
               ** L'autoroute ou la DAI n'existe pas
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
            ** Verification de l'existence et recherche du numero de l'autoroute
            ** et de la DAI
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
          
            select @vl_DAI = numero from CFT..EQT_GEN
              where nom  = @va_NomDAI_in
                and type = XDC_EQT_DAI
          
	    select @vl_Site = numero from CFT..RES_DIS
	    where code = @va_NomSite_in

	if @vl_Site = NULL
		return XDC_ARG_INV

            /*A
            ** L'autoroute et la DAI existent
            */
            
            if @vl_Autoroute <> null and @vl_DAI <> null
            begin
         
               /*A 
               ** Insertion ou Modification de l'analyseur
               **
               ** Verification de l'existence et recherche de l'analyseur dans les equipements
               */
         
               select @vl_NumAna = numero from CFT..EQT_GEN
                 where type = XDC_EQT_ANA
                   and nom  = @va_NomAnalyseur_in
              
            
               /*A
               ** L'analyseur n'existe pas comme equipement
               */
            
               if @vl_NumAna = null
               begin
              
                  /*A
                  ** Insertion de l'analyseur dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_ANA
               
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
                  values ( XDC_EQT_ANA, @vl_Numero + 1, @vl_DAI, XDC_MAI_ANA, @vl_Autoroute, @va_PRdebut_in, @va_Sens_in, @va_NomAnalyseur_in , @vl_Site, @va_Supprime_in)
               
                  
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
                  ** Insertion de l'analyseur dans la table EQT_ANA
                  */
               
                  insert into CFT..EQT_ANA
                  values ( @vl_Numero + 1, @va_PRfin_in, @va_NumeroAnalyseur_in, @va_VoiesSurveillees_in )
               
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
               ** La Ana existe deja comme equipement
               */      
            
               begin
           
                  /*A
                  ** Modification de la Ana dans la table EQT_GEN
                  */
                
                  update CFT..EQT_GEN
    	          set maitre      = @vl_DAI,
    	              type_maitre = XDC_MAI_ANA,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PRdebut_in,
                      sens        = @va_Sens_in,
                      nom         = @va_NomAnalyseur_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumAna
                    and type   = XDC_EQT_ANA  
    
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
                  ** Modification de l'analyseur dans la table EQT_ANA
                  */
                
                  update CFT..EQT_ANA
    	          set PR_fin           = @va_PRfin_in,
                      numero_analyseur = @va_NumeroAnalyseur_in,
                      nombre_de_voies  = @va_VoiesSurveillees_in
                  where numero = @vl_NumAna
                 
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
               ** L'autoroute ou la DAI n'existe pas
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
