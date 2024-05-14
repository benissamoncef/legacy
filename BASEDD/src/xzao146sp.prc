/*E*/
/*Fichier : $Id: xzao146sp.prc,v 1.6 2018/07/11 09:58:57 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 09:58:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao146sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* PN	20 Nov 1996	: Suppression du controle sur la portion 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao146sp.h" */

/* static char *version = "@(#)xzao146.c	1.1 20/09/94 : xzao146" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un ITPC dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;146
   @va_Basedd_in        	char(3),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO146SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomAuto		va_Autoroute_in
* XDY_PR		va_PR_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un ITPC en base
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
* Insertion ou modification dans la table RES_ITPC (RES_ITPC)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumPortion	smallint,
   @vl_NumITPC		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null   
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   
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
            ** Verification de l'existence et recherche de l'autoroute et de la portion
            */
         
            select @vl_NumPortion = numero from CFG..RES_POR
              where autoroute in
                (select numero from CFG..RES_AUT
                   where nom       = @va_Autoroute_in)
              and PR_debut <  @va_PR_in
              and PR_fin   >= @va_PR_in
              
            /*A
            ** Existence de l'autoroute et de la portion
            */
            
            if @vl_NumPortion <> null
            begin
               
               select @vl_NumITPC = portion from CFG..RES_ITP
                 where PR      = @va_PR_in
                   
               /*A
               ** L'ITPC n'existe pas dans RES_ITP
               */
               
               if @vl_NumITPC = null
               begin
                  
                  /*A
                  ** Insertion du ITPC dans la table RES_ITP
                  */
               
                  insert into CFG..RES_ITP
                  values ( @vl_NumPortion, @va_PR_in )
                  
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
               begin
               	update CFG..RES_ITP
               	set PR= @va_PR_in,portion=@vl_NumPortion
               	where PR      = @va_PR_in
               end
                  
            end
             
            else
         
            /*A
            ** L'autoroute ou la portion n'existe pas
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
            ** Verification de l'existence et recherche de l'autoroute et de la portion
            */
         
            select @vl_NumPortion = numero from CFT..RES_POR
              where autoroute in
                (select numero from CFT..RES_AUT
                   where nom       = @va_Autoroute_in)
              and PR_debut <  @va_PR_in
              and PR_fin   >= @va_PR_in
              
            /*A
            ** Existence de l'autoroute et de la portion
            */
            
            if @vl_NumPortion <> null
            begin
               
               select @vl_NumITPC = portion from CFT..RES_ITP
               where
                    PR      = @va_PR_in
                   
               /*A
               ** L'ITPC n'existe pas dans RES_ITP
               */
               
               if @vl_NumITPC = null
               begin
                  
                  /*A
                  ** Insertion du ITPC dans la table RES_ITP
                  */
               
                  insert into CFT..RES_ITP
                  values ( @vl_NumPortion, @va_PR_in )
                  
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
               begin
               	update CFT..RES_ITP
               	set PR= @va_PR_in,portion=@vl_NumPortion
               	where PR      = @va_PR_in
               end
                  
            end
             
            else
         
            /*A
            ** L'autoroute ou la portion n'existe pas
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
