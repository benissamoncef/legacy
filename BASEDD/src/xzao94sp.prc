/*E*/
/*Fichier : $Id: xzao94sp.prc,v 1.5 2018/07/11 09:49:14 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 09:49:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao94sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao94sp.h" */

/* static char *version = "@(#)xzao94.c	1.1 23/08/94 : xzao94" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une Ville dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;94
   @va_Basedd_in        	char(3),
   @va_NomVille_in		char(25),
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO94SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomVille_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une Ville en base
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
* Insertion ou modification dans la table RES_VIL 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* LCL		05/07/17	: Modif PRA
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NomVille		char(25),
   @vl_Site			char(10)
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomVille_in = null 
   
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
            ** Insertion ou Modification de le Ville
            **
            ** Verification de leexistence et recherche de le Ville dans RES_VIL
            */
         
            select @vl_NomVille = nom from CFG..RES_VIL
              where nom     = @va_NomVille_in
               
            /*A
            ** Le Ville n'existe pas dans RES_VIL
            */
            
            if @vl_NomVille = null
            begin
              
               /*A
               ** Creation non positionne =>
               ** Ajout de la Ville dans la table RES_VIL
               */
                  
               if @va_Creation_in = XDC_VRAI
               begin
                  
                  insert into CFG..RES_VIL
                  values ( @va_NomVille_in, null )
                     
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
               ** Le Ville existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation non positionne =>
                  ** Suppression de le Ville dans la table RES_VIL
                  */
                  
                  delete CFG..RES_VIL
                  where nom = @va_NomVille_in
                     
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

#ifdef CI
         
         else
         
         /*A
         **  Travail dans la base CFT
         */   
      
         if @va_Basedd_in = XDC_BASE_CFT
         begin
         
            /*A 
            ** Insertion ou Modification de le Ville
            **
            ** Verification de leexistence et recherche de le Ville dans RES_VIL
            */
         
            select @vl_NomVille = nom from CFT..RES_VIL
              where nom     = @va_NomVille_in
               
            /*A
            ** Le Ville n'existe pas dans RES_VIL
            */
            
            if @vl_NomVille = null
            begin
              
               /*A
               ** Creation non positionne =>
               ** Ajout de la Ville dans la table RES_VIL
               */
                  
               if @va_Creation_in = XDC_VRAI
               begin
                  
                  insert into CFT..RES_VIL
                  values ( @va_NomVille_in, null )
                     
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
               ** Le Ville existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation non positionne =>
                  ** Suppression de le Ville dans la table RES_VIL
                  */
                  
                  delete CFT..RES_VIL
                  where nom = @va_NomVille_in
                     
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
      
#endif      
      
      end
   end         
     
   return XDC_OK
         
go  
