/*E*/
/*Fichier : $Id: xzao84sp.prc,v 1.7 2018/01/09 15:20:37 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/01/09 15:20:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao84sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Modif test param entree (v1.2)
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.3)
* PN		20 Nov 1996	: Suppression du controle sur la portion 1.4
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.5)
* JPL		14/06/2012	: Ajout nom sortie et indicateur de bifurcation, ecrits en table �changeurs  1.6
* LCL		05/07/17	: Modif PRA
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao84sp.h" */

/* static char *version = "$Id: xzao84sp.prc,v 1.7 2018/01/09 15:20:37 pc2dpdy Exp $ : xzao84" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un echangeur dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select name from sysobjects where name = 'XZAO84' and type = 'P')
	drop procedure XZAO84
go


create procedure XZAO84
   @va_Basedd_in        	char(3),
   @va_Numero_in		tinyint,
   @va_NomEchangeur_in		char(25),
   @va_Abreviation_in		char(15),
   @va_NomSortie_in		char(20),
   @va_Bifurcation_in		bit,
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_PRentreeSens1_in		int,
   @va_VoiesEntreeSens1_in	tinyint,
   @va_PeageEntreeSens1_in	bit,
   @va_PRentreeSens2_in		int,
   @va_VoiesEntreeSens2_in	tinyint,
   @va_PeageEntreeSens2_in	bit,
   @va_PRsortieSens1_in		int,
   @va_VoiesSortieSens1_in	tinyint,
   @va_PeageSortieSens1_in	bit,
   @va_PRsortieSens2_in		int,	
   @va_VoiesSortieSens2_in	tinyint,
   @va_PeageSortieSens2_in	bit,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO84SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_Numero_in
* XDY_Nom		va_NomEchangeur_in
* XDY_Abrev		va_Abreviation_in
* XDY_Sortie		va_NomSortie_in
* XDY_Booleen		va_Bifurcation_in
* XDY_NomAuto		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_PR		va_PRentreeSens1_in	
* XDY_Octet		va_VoiesEntreeSens1_in
* XDY_Booleen		va_PeageEntreeSens1_in
* XDY_PR		va_PRentreeSens2_in	
* XDY_Octet		va_VoiesEntreeSens2_in
* XDY_Booleen		va_PeageEntreeSens2_in
* XDY_PR		va_PRsortieSens1_in	
* XDY_Octet		va_VoiesSortieSens1_in
* XDY_Booleen		va_PeageSortieSens1_in
* XDY_PR		va_PRsortieSens2_in	
* XDY_Octet		va_VoiesSortieSens2_in
* XDY_Booleen		va_PeageSortieSens2_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un echangeur en base
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
* Insertion ou modification dans la table RES_ECH (ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Portion		smallint,
   @vl_NumEch		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU

   if (@va_NomSortie_in = XDC_CHAINE_VIDE)
      select @va_NomSortie_in = null
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if (@va_Basedd_in = null  or  @va_Basedd_in = XDC_CHAINE_VIDE)
   or @va_Numero_in = null 
   or (@va_NomEchangeur_in = null  or  @va_NomEchangeur_in = XDC_CHAINE_VIDE)
   or (@va_Abreviation_in = null  or  @va_Abreviation_in = XDC_CHAINE_VIDE)
   or @va_Bifurcation_in = null 
   or (@va_Autoroute_in = null  or  @va_Autoroute_in = XDC_CHAINE_VIDE)
   or @va_PR_in = null 
   or @va_PeageEntreeSens1_in = null 
   or @va_PeageEntreeSens2_in = null 
   or @va_PeageSortieSens1_in = null 
   or @va_PeageSortieSens2_in = null 
   
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
            ** Verification de l'existence et recherche du numero de l'autoroute
            */
         
            select @vl_Portion = POR.numero from CFG..RES_AUT AUT, CFG..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
                and POR.PR_debut <  @va_PR_in
                and POR.PR_fin   >= @va_PR_in
                
          
            if @vl_Portion <> null
            begin
         
               /*A 
               ** Insertion ou Modification de l'echangeur
               **
               ** Verification de l'existence et recherche de l'echangeur dans RES_ECH
               */
         
               select @vl_NumEch = numero from CFG..RES_ECH
                 where numero      = @va_Numero_in
              
            
               /*A
               ** L'echangeur n'existe pas dans RES_ECH
               */
            
               if @vl_NumEch = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de l'echangeur dans la table RES_ECH
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
                     insert into CFG..RES_ECH (
                        numero,
                        nom,
                        abreviation,
                        sortie,
                        bifurcation,
                        portion,
                        PR,
                        PR_entree_sens1,
                        voies_entree_sens1,
                        peage_entree_sens1,
                        PR_sortie_sens1,
                        voies_sortie_sens1,
                        peage_sortie_sens1,
                        PR_entree_sens2,
                        voies_entree_sens2,
                        peage_entree_sens2,
                        PR_sortie_sens2,
                        voies_sortie_sens2,
                        peage_sortie_sens2,
                        gare_de_peage,
                        supprime )
                     values (
                        @va_Numero_in,
                        @va_NomEchangeur_in,
                        @va_Abreviation_in,
                        @va_NomSortie_in,
                        @va_Bifurcation_in,
                        @vl_Portion,
                        @va_PR_in,
                        @va_PRentreeSens1_in,
                        @va_VoiesEntreeSens1_in,
                        @va_PeageEntreeSens1_in,
                        @va_PRsortieSens1_in,
                        @va_VoiesSortieSens1_in,
                        @va_PeageSortieSens1_in,
                        @va_PRentreeSens2_in,
                        @va_VoiesEntreeSens2_in,
                        @va_PeageEntreeSens2_in,
                        @va_PRsortieSens2_in,
                        @va_VoiesSortieSens2_in,
                        @va_PeageSortieSens2_in,
                        null,
                        0 )
              
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
               ** L'echangeur existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de l'echangeur dans la table RES_ECH
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_ECH
    	             set nom                = @va_NomEchangeur_in, 
    	                 abreviation        = @va_Abreviation_in,
    	                 sortie             = @va_NomSortie_in,
    	                 bifurcation        = @va_Bifurcation_in,
    	                 portion            = @vl_Portion,
    	                 PR                 = @va_PR_in,
    	                 PR_entree_sens1    = @va_PRentreeSens1_in, 
    	                 voies_entree_sens1 = @va_VoiesEntreeSens1_in, 
    	                 peage_entree_sens1 = @va_PeageEntreeSens1_in, 
    	                 PR_sortie_sens1    = @va_PRsortieSens1_in, 
    	                 voies_sortie_sens1 = @va_VoiesSortieSens1_in, 
    	                 peage_sortie_sens1 = @va_PeageSortieSens1_in,
    	                 PR_entree_sens2    = @va_PRentreeSens2_in, 
    	                 voies_entree_sens2 = @va_VoiesEntreeSens2_in, 
    	                 peage_entree_sens2 = @va_PeageEntreeSens2_in, 
    	                 PR_sortie_sens2    = @va_PRsortieSens2_in, 
    	                 voies_sortie_sens2 = @va_VoiesSortieSens2_in, 
    	                 peage_sortie_sens2 = @va_PeageSortieSens2_in,
    	                 supprime           = XDC_FAUX
                     where numero   = @vl_NumEch
    
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
                  
                  else
                  
                  /*A
                  ** Creation non positionne =>
                  ** Marquage de la suppression de l'echangeur dans la table RES_ECH
                  */
                     
                  begin
                     
                     update CFG..RES_ECH
    	             set supprime = XDC_VRAI
                     where numero   = @vl_NumEch
                       and portion  = @vl_Portion
                       
                     /*A
                     ** Erreur de suprression
                     */
                     
                     if @@rowcount > 1
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
            ** Verification de l'existence et recherche du numero de l'autoroute
            */
         
            select @vl_Portion = POR.numero from CFT..RES_AUT AUT, CFT..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
                and POR.PR_debut <  @va_PR_in
                and POR.PR_fin   >= @va_PR_in
                
          
            if @vl_Portion <> null
            begin
         
               /*A 
               ** Insertion ou Modification de l'echangeur
               **
               ** Verification de l'existence et recherche de l'echangeur dans RES_ECH
               */
         
               select @vl_NumEch = numero from CFT..RES_ECH
                 where numero      = @va_Numero_in
              
            
               /*A
               ** L'echangeur n'existe pas dans RES_ECH
               */
            
               if @vl_NumEch = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de l'echangeur dans la table RES_ECH
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
                     insert into CFT..RES_ECH (
                        numero,
                        nom,
                        abreviation,
                        sortie,
                        bifurcation,
                        portion,
                        PR,
                        PR_entree_sens1,
                        voies_entree_sens1,
                        peage_entree_sens1,
                        PR_sortie_sens1,
                        voies_sortie_sens1,
                        peage_sortie_sens1,
                        PR_entree_sens2,
                        voies_entree_sens2,
                        peage_entree_sens2,
                        PR_sortie_sens2,
                        voies_sortie_sens2,
                        peage_sortie_sens2,
                        gare_de_peage,
                        supprime )
                     values (
                        @va_Numero_in,
                        @va_NomEchangeur_in,
                        @va_Abreviation_in,
                        @va_NomSortie_in,
                        @va_Bifurcation_in,
                        @vl_Portion,
                        @va_PR_in,
                        @va_PRentreeSens1_in,
                        @va_VoiesEntreeSens1_in,
                        @va_PeageEntreeSens1_in,
                        @va_PRsortieSens1_in,
                        @va_VoiesSortieSens1_in,
                        @va_PeageSortieSens1_in,
                        @va_PRentreeSens2_in,
                        @va_VoiesEntreeSens2_in,
                        @va_PeageEntreeSens2_in,
                        @va_PRsortieSens2_in,
                        @va_VoiesSortieSens2_in,
                        @va_PeageSortieSens2_in,
                        null,
                        0 )
              
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
               ** L'echangeur existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de l'echangeur dans la table RES_ECH
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_ECH
    	             set nom                = @va_NomEchangeur_in, 
    	                 abreviation        = @va_Abreviation_in,
    	                 sortie             = @va_NomSortie_in,
    	                 bifurcation        = @va_Bifurcation_in,
    	                 portion            = @vl_Portion,
    	                 PR                 = @va_PR_in,
    	                 PR_entree_sens1    = @va_PRentreeSens1_in, 
    	                 voies_entree_sens1 = @va_VoiesEntreeSens1_in, 
    	                 peage_entree_sens1 = @va_PeageEntreeSens1_in, 
    	                 PR_sortie_sens1    = @va_PRsortieSens1_in, 
    	                 voies_sortie_sens1 = @va_VoiesSortieSens1_in, 
    	                 peage_sortie_sens1 = @va_PeageSortieSens1_in,
    	                 PR_entree_sens2    = @va_PRentreeSens2_in, 
    	                 voies_entree_sens2 = @va_VoiesEntreeSens2_in, 
    	                 peage_entree_sens2 = @va_PeageEntreeSens2_in, 
    	                 PR_sortie_sens2    = @va_PRsortieSens2_in, 
    	                 voies_sortie_sens2 = @va_VoiesSortieSens2_in, 
    	                 peage_sortie_sens2 = @va_PeageSortieSens2_in,
    	                 supprime           = XDC_FAUX
                     where numero   = @vl_NumEch
    
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
                  
                  else
                  
                  /*A
                  ** Creation non positionne =>
                  ** Marquage de la suppression de l'echangeur dans la table RES_ECH
                  */
                     
                  begin
                     
                     update CFT..RES_ECH
    	             set supprime = XDC_VRAI
                     where numero   = @vl_NumEch
                       and portion  = @vl_Portion
                       
                     /*A
                     ** Erreur de suprression
                     */
                     
                     if @@rowcount > 1
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
