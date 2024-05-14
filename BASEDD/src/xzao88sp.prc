/*E*/
/*Fichier : $Id: xzao88sp.prc,v 1.7 2018/07/11 09:45:12 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/07/11 09:45:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao88sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* PN	20 Nov 1996	: Suppression du controle sur la portion 1.3
* LCL	05/07/17	: Modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao88sp.h" */

/* static char *version = "@(#)xzao88.c	1.1 23/18/94 : xzao88" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une peage dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;88
   @va_Basedd_in        	char(3),
   @va_Numero_in		tinyint,
   @va_NomPeage_in		char(25),
   @va_Abreviation_in		char(15),
   @va_Autoroute_in		char(4),
   @va_PRSens1_in		int,
   @va_PRSens2_in		int,
   @va_LongueurSens1_in		smallint,
   @va_LongueurSens2_in		smallint,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO88SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Octet		va_Numero_in
* XDY_Nom		va_NomPeage_in
* XDY_Abrev		va_Abreviation_in
* XDY_Nom		va_Autoroute_in
* XDY_PR		va_PRSens1_in
* XDY_PR		va_PRSens2_in
* XDY_Mot		va_LongueurSens1_in	
* XDY_Mot		va_LongueurSens2_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une peage en base
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
* Insertion ou modification dans la table RES_PEA (AIRE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC (v1.2)
* volcic	30 Dec 1994	: Modif pour tcgcd dans CFT (v1.3)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.5)
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumPeage		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_Numero_in = null 
   or @va_NomPeage_in = null or @va_Abreviation_in = null or @va_Autoroute_in = null 
   or @va_PRSens1_in = null or @va_PRSens2_in = null or @va_LongueurSens1_in = null or @va_LongueurSens2_in = null 
   
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
      
         if @va_Basedd_in = "CFG"
         begin
         
            /*A
            ** Verification de l'existence et recherche du numero de l'autoroute
            */
         
            select @vl_Autoroute = POR.numero from CFG..RES_AUT AUT, CFG..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
                and ((POR.PR_debut <  @va_PRSens1_in
                  and POR.PR_fin   >= @va_PRSens1_in )
                or  ( POR.PR_debut <  @va_PRSens2_in
                  and POR.PR_fin   >= @va_PRSens2_in ))
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de le peage
               **
               ** Verification de l'existence et recherche de le peage dans RES_PEA
               */
         
               select @vl_NumPeage = numero from CFG..RES_PEA
                 where numero         = @va_Numero_in
               
               /*A
               ** Le peage n'existe pas dans RES_PEA
               */
            
               if @vl_NumPeage = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de l'peage dans la table RES_PEA
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     /*
                     select @vl_Numero = max(numero) from CFG..RES_PEA
               
                     if @vl_Numero = null
                        select @vl_Numero = 0
                     */
                        
                     insert into CFG..RES_PEA
                     values ( @va_Numero_in, @va_NomPeage_in, @va_Abreviation_in, @vl_Autoroute, @va_PRSens1_in, @va_LongueurSens1_in, @va_PRSens2_in, @va_LongueurSens2_in, NULL, XDC_FAUX )
                  
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
               ** Le peage existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le peage dans la table RES_PEA
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_PEA
    	             set nom            = @va_NomPeage_in, 
    	                 abreviation    = @va_Abreviation_in,
    	                 PRsens1        = @va_PRSens1_in,
    	                 longueur_sens1 = @va_LongueurSens1_in,
    	                 PRsens2        = @va_PRSens2_in,
    	                 longueur_sens2 = @va_LongueurSens2_in,
                         portion  = @vl_Autoroute  
   	             where numero   = @vl_NumPeage
    
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
                  ** Marquage de suppression de l'peage dans la table RES_PEA
                  */
                  
                  begin
                  
                     update CFG..RES_PEA
    	             set supprime    = XDC_VRAI   
    	             where numero   = @vl_NumPeage
                       and portion  = @vl_Autoroute
                     
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
         
            select @vl_Autoroute = POR.numero from CFT..RES_AUT AUT, CFT..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
                and ((POR.PR_debut <  @va_PRSens1_in
                  and POR.PR_fin   >= @va_PRSens1_in )
                or  ( POR.PR_debut <  @va_PRSens2_in
                  and POR.PR_fin   >= @va_PRSens2_in ))
          
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de le peage
               **
               ** Verification de l'existence et recherche de le peage dans RES_PEA
               */
         
               select @vl_NumPeage = numero from CFT..RES_PEA
                 where numero         = @va_Numero_in
               
               /*A
               ** Le peage n'existe pas dans RES_PEA
               */
            
               if @vl_NumPeage = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de l'peage dans la table RES_PEA
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     /*select @vl_Numero = max(numero) from CFT..RES_PEA*/
               
                     insert into CFT..RES_PEA
                     values ( @va_Numero_in, @va_NomPeage_in, @va_Abreviation_in, @vl_Autoroute, @va_PRSens1_in, @va_LongueurSens1_in, @va_PRSens2_in, @va_LongueurSens2_in, NULL, XDC_FAUX )
                  
                     
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
               ** Le peage existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le peage dans la table RES_PEA
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_PEA
    	             set nom            = @va_NomPeage_in, 
    	                 abreviation    = @va_Abreviation_in,
    	                 PRsens1        = @va_PRSens1_in,
    	                 longueur_sens1 = @va_LongueurSens1_in,
    	                 PRsens2        = @va_PRSens2_in,
    	                 longueur_sens2 = @va_LongueurSens2_in,
                         portion  = @vl_Autoroute  
    	             where numero   = @vl_NumPeage
    
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
                  ** Marquage de suppression de l'peage dans la table RES_PEA
                  */
                  
                  begin
                  
                     update CFT..RES_PEA
    	             set supprime    = XDC_VRAI   
    	             where numero   = @vl_NumPeage
                       and portion  = @vl_Autoroute
                     
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
