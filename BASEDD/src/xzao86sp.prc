/*E*/
/*Fichier : $Id: xzao86sp.prc,v 1.9 2018/07/11 09:44:19 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2018/07/11 09:44:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao86sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao86sp.h" */

/* static char *version = "@(#)xzao86.c	1.1 23/18/94 : xzao86" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une aire dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;86
   @va_Basedd_in        	char(3),
   @va_Numero_in		tinyint,
   @va_NomAire_in		char(25),
   @va_Abreviation_in		char(15),
   @va_Autoroute_in		char(4),
   @va_Sens_in			tinyint,
   @va_PRentree_in		int,
   @va_PRsortie_in		int,
   @va_Creation_in		bit,
   @va_CapaciteVL_in		smallint,
   @va_CapacitePL_in		smallint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO86SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_Numero_in
* XDY_Nom		va_NomAire_in
* XDY_Abrev		va_Abreviation_in
* XDY_Nom		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRentree
* XDY_PR		va_PRsortie	
* XDY_Booleen		va_Creation_in
* XDY_Mot		va_CapaciteVL_in
* XDY_Mot		va_CapacitePL_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une aire en base
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
* Insertion ou modification dans la table RES_AIR (AIRE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	15 Jan 1995	: Modif Resultat_out (v1.3)
* C.T.		10/03/95	: Ajout capacite VL et PL (V 1.5)
* C.T.		21/03/95	: Correction ligne dupliquee (V 1.6)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.7)
* LCL		05/07/17	: Modif PRA
* LCL 		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumAutoroute	tinyint,
   @vl_Autoroute	smallint,
   @vl_NumAire		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_Numero_in = null 
   or @va_NomAire_in = null or @va_Abreviation_in = null or @va_Autoroute_in = null 
   or @va_PRentree_in = null or @va_PRsortie_in = null
   
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
         
            select @vl_NumAutoroute = numero from CFG..RES_AUT 
              where nom = @va_Autoroute_in
            
            select @vl_Autoroute = numero from  CFG..RES_POR    
              where autoroute = @vl_NumAutoroute
                and (PR_debut <  @va_PRentree_in
                  and PR_fin   >= @va_PRentree_in )
                
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de l'aire
               **
               ** Verification de l'existence et recherche de l'aire dans RES_AIR
               */
         
               select @vl_NumAire = numero from CFG..RES_AIR
                 where numero      = @va_Numero_in
               
               /*A
               ** L'aire n'existe pas dans RES_AIR
               */
            
               if @vl_NumAire = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de l'aire dans la table RES_AIR
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFG..RES_AIR
                     values ( @va_Numero_in, @va_NomAire_in, @va_Abreviation_in, 
				@vl_Autoroute, @va_PRentree_in, @va_PRsortie_in, 
				@va_Sens_in, @va_CapaciteVL_in, @va_CapacitePL_in, NULL, XDC_FAUX )
                  
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
               ** L'aire existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de l'aire dans la table RES_AIR
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_AIR
    	             set nom         = @va_NomAire_in, 
    	                 abreviation = @va_Abreviation_in,
    	                 PRentree    = @va_PRentree_in, 
		         PRsortie    = @va_PRsortie_in,
		         sens        = @va_Sens_in,
		         capacite_VL = @va_CapaciteVL_in,
		         capacite_PL = @va_CapacitePL_in,
		         supprime    = XDC_FAUX   
    	             where numero   = @vl_NumAire  
    
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
                  ** Marquage de suppression de l'aire dans la table RES_AIR
                  */
                  
                  begin
                  
                     update CFG..RES_AIR
    	             set supprime    = XDC_VRAI   
    	             where numero   = @vl_NumAire
                     
                     /*A
                     ** Erreur de suppression
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
         
            select @vl_NumAutoroute = numero from CFT..RES_AUT 
              where nom = @va_Autoroute_in
            
            select @vl_Autoroute = numero from  CFT..RES_POR    
              where autoroute = @vl_NumAutoroute
                and (PR_debut <  @va_PRentree_in
                  and PR_fin   >= @va_PRentree_in )
                
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de l'aire
               **
               ** Verification de l'existence et recherche de l'aire dans RES_AIR
               */
         
               select @vl_NumAire = numero from CFT..RES_AIR
                 where numero      = @va_Numero_in
               
               /*A
               ** L'aire n'existe pas dans RES_AIR
               */
            
               if @vl_NumAire = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de l'aire dans la table RES_AIR
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFT..RES_AIR
                     values ( @va_Numero_in, @va_NomAire_in, @va_Abreviation_in,
			@vl_Autoroute, @va_PRentree_in, @va_PRsortie_in, 
			@va_Sens_in, @va_CapaciteVL_in, @va_CapacitePL_in, NULL, XDC_FAUX )
                  
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
               ** L'aire existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de l'aire dans la table RES_AIR
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_AIR
    	             set nom         = @va_NomAire_in, 
    	                 abreviation = @va_Abreviation_in,
    	                 PRentree   = @va_PRentree_in, 
		         PRsortie   = @va_PRsortie_in,
		         sens        = @va_Sens_in,
		         capacite_VL = @va_CapaciteVL_in,
		         capacite_PL = @va_CapacitePL_in,
		         supprime    = XDC_FAUX   
    	             where numero   = @vl_NumAire 
    
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
                  ** Marquage de suppression de l'aire dans la table RES_AIR
                  */
                  
                  begin
                  
                     update CFT..RES_AIR
    	             set supprime    = XDC_VRAI   
    	             where numero   = @vl_NumAire
                     
                     /*A
                     ** Erreur de suppression
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
