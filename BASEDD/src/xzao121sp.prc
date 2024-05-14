/*E*/
/*Fichier : $Id: xzao121sp.prc,v 1.7 2018/07/11 09:50:52 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/07/11 09:50:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao121sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* PN	20 Nov 1996	: Suppression du controle sur la portion 1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao121sp.h" */

/* static char *version = "@(#)xzao121.c	1.1 20/09/94 : xzao121" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Panneau de Police dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;121
   @va_Basedd_in        	char(3),
   @va_NomPortion_in		char(25),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_Vitesse_in		tinyint,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO121SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomPortion_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Vitesse_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Panneau Police en base
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
* Insertion ou modification dans la table RES_POL (PANNEAU_POLICE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.4)
* PNI		26 mar 2012	: ajout controle sur portion 1.5
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumPortion	smallint,
   @vl_NumPnPol		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomPortion_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null
   or @va_Vitesse_in = null
   
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
            ** Verification de l'existence et recherche du numero de la portion
            */
         
            select @vl_NumPortion = numero from CFG..RES_POR 
              where nom = @va_NomPortion_in
                
          
            if @vl_NumPortion <> null
            begin
         
               /*A 
               ** Insertion ou Modification du PnPol
               **
               ** Verification de l'existence et recherche du PnPol dans RES_POL
               */
         
               select @vl_NumPnPol = portion from CFG..RES_POL
                 where PR      = @va_PR_in
                   and sens    = @va_Sens_in and portion=@vl_NumPortion
               
               /*A
               ** Le PnPol n'existe pas dans RES_POL
               */
            
               if @vl_NumPnPol = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion du PnPol dans la table RES_POL
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFG..RES_POL
                     values ( @vl_NumPortion, @va_PR_in, @va_Sens_in, @va_Vitesse_in )
                  
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
               ** Le PnPol existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le PnPol dans la table RES_POL
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_POL
    	             set vitesse = @va_Vitesse_in,portion = @vl_NumPortion
    	             where PR      = @va_PR_in
                       and sens    = @va_Sens_in
                       
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
                  ** Marquage de suppression de le PnPol dans la table RES_POL
                  */
                  
                  begin
                  
                     delete CFG..RES_POL   
    	             where portion = @vl_NumPortion  
                       and PR      = @va_PR_in
                       and sens    = @va_Sens_in
                  
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
            ** Verification de l'existence et recherche du numero de la portion
            */
         
            select @vl_NumPortion = numero from CFT..RES_POR 
              where nom = @va_NomPortion_in
                
          
            if @vl_NumPortion <> null
            begin
         
               /*A 
               ** Insertion ou Modification du PnPol
               **
               ** Verification de l'existence et recherche du PnPol dans RES_POL
               */
         
               select @vl_NumPnPol = portion from CFT..RES_POL
                 where PR      = @va_PR_in
                   and sens    = @va_Sens_in and portion=@vl_NumPortion
               
               /*A
               ** Le PnPol n'existe pas dans RES_POL
               */
            
               if @vl_NumPnPol = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion du PnPol dans la table RES_POL
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFT..RES_POL
                     values ( @vl_NumPortion, @va_PR_in, @va_Sens_in, @va_Vitesse_in )
                  
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
               ** Le PnPol existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le PnPol dans la table RES_POL
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_POL
    	             set vitesse = @va_Vitesse_in, portion = @vl_NumPortion
    	             where PR      = @va_PR_in
                       and sens    = @va_Sens_in
                       
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
                  ** Marquage de suppression de le PnPol dans la table RES_POL
                  */
                  
                  begin
                  
                     delete CFT..RES_POL   
    	             where portion = @vl_NumPortion  
                       and PR      = @va_PR_in
                       and sens    = @va_Sens_in
                  
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
