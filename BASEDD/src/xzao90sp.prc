/*E*/
/*Fichier : $Id: xzao90sp.prc,v 1.7 2018/07/11 09:45:52 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/07/11 09:45:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao90sp.prc
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

/* #include "xzao90sp.h" */

/* static char *version = "@(#)xzao90.c	1.1 23/18/94 : xzao90" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un PS dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;90
   @va_Basedd_in        	char(3),
   @va_NomPS_in			char(25),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_GabaritSens1_in		smallint,
   @va_GabaritSens2_in		smallint,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO90SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomPS_in
* XDY_Nom		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Mot		va_GabaritSens1_in	
* XDY_Mot		va_GabaritSens2_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une PS en base
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
* Insertion ou modification dans la table RES_SUP 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.4)
* GGY		26/09/23 	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumPS		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomPS_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   or @va_GabaritSens1_in = null 
   or @va_GabaritSens2_in = null 
   
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
            ** Verification de leexistence et recherche du numero de leautoroute
            */
         
            select @vl_Autoroute = POR.numero from CFG..RES_AUT AUT, CFG..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
                and POR.PR_debut <  @va_PR_in
                and POR.PR_fin   >= @va_PR_in
                
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de le PS
               **
               ** Verification de leexistence et recherche de le PS dans RES_SUP
               */
         
               select @vl_NumPS = numero from CFG..RES_SUP
                 where nom            = @va_NomPS_in
               
               /*A
               ** Le PS n'existe pas dans RES_SUP
               */
            
               if @vl_NumPS = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de lePS dans la table RES_SUP
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     select @vl_Numero = max(numero) from CFG..RES_SUP
               
                     if @vl_Numero = null
                        select @vl_Numero = 0 
                     
                     insert into CFG..RES_SUP
                     values ( @vl_Numero + 1, @va_NomPS_in, @vl_Autoroute, @va_PR_in, @va_GabaritSens1_in, @va_GabaritSens2_in, NULL )
                  
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
               ** Le PS existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le PS dans la table RES_SUP
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_SUP
    	             set nom           = @va_NomPS_in,
    	                 PR            = @va_PR_in,
    	                 gabarit_sens1 = @va_GabaritSens1_in,
    	                 gabarit_sens2 = @va_GabaritSens2_in,
                        portion  = @vl_Autoroute  
    	             where numero   = @vl_NumPS
    
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
                  ** Marquage de suppression de le PS dans la table RES_SUP
                  */
                  
                  begin
                  
                     delete CFG..RES_SUP   
    	             where numero   = @vl_NumPS
                       and portion  = @vl_Autoroute
                     
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
                
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de le PS
               **
               ** Verification de l'existence et recherche de le PS dans RES_SUP
               */
         
               select @vl_NumPS = numero from CFT..RES_SUP
                 where nom            = @va_NomPS_in
                   
               /*A
               ** Le PS n'existe pas dans RES_SUP
               */
            
               if @vl_NumPS = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de lePS dans la table RES_SUP
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     select @vl_Numero = max ( numero ) from CFT..RES_SUP
               
                     if @vl_Numero = null
                        select @vl_Numero = 0 
                     
                     insert into CFT..RES_SUP
                     values ( @vl_Numero + 1, @va_NomPS_in, @vl_Autoroute, @va_PR_in, @va_GabaritSens1_in, @va_GabaritSens2_in, NULL )
                  
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
               ** Le PS existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le PS dans la table RES_SUP
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_SUP
    	             set nom           = @va_NomPS_in,
    	                 PR            = @va_PR_in,
    	                 gabarit_sens1 = @va_GabaritSens1_in,
    	                 gabarit_sens2 = @va_GabaritSens2_in,
                        portion  = @vl_Autoroute  
    	             where numero   = @vl_NumPS
    
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
                  ** Marquage de suppression de le PS dans la table RES_SUP
                  */
                  
                  begin
                  
                     delete CFT..RES_SUP   
    	             where numero   = @vl_NumPS
                       and portion  = @vl_Autoroute
                     
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
