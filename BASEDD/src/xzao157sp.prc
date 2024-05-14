/*E*/
/*Fichier : $Id: xzao157sp.prc,v 1.4 2018/07/11 10:01:23 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 10:01:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao157sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao157sp.h" */

/* static char *version = "@(#)xzao157.c	1.1 20/09/94 : xzao157" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Type NAV dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;157
   @va_Basedd_in        	char(3),
   @va_NomType_in		T_NOM,
   @va_FlashMod4_in		bit,
   @va_NbBarrieres_in		tinyint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO157SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomType_in
* XDY_Booleen		va_FlashMod4_in
* XDY_Octet		va_NbBarrieres_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Type NAV en base
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
* Insertion ou modification dans la table TYP_NAV (TYPE_NAV)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumNAV	tinyint,
   @vl_Numero	tinyint,
   @vl_Site 	char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomType_in = null 
   or @va_FlashMod4_in = null 
   or @va_NbBarrieres_in = null 
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin tran
      
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence du type NAV
            */
         
            select @vl_NumNAV = numero from CFG..TYP_NAV
              where nom              = @va_NomType_in
              
            /*A
            ** Le type NAV n'existe pas dans TYP_NAV
            */
               
            if @vl_NumNAV = null
            begin
                  
               /*A
               ** Insertion du type NAV dans la table TYP_NAV
               */
               
               select @vl_Numero = max(numero) from CFG..TYP_NAV
               
               if @vl_Numero = null
                  select @vl_Numero = 0
                     
               insert into CFG..TYP_NAV
               values ( @vl_Numero + 1, @va_NomType_in, @va_FlashMod4_in, @va_NbBarrieres_in  )
                  
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
         
#ifdef CI

         /*A
         **  Travail dans la base CFT
         */   
      
         begin
         
            /*A
            ** Verification de l'existence du type NAV
            */
         
            select @vl_NumNAV = numero from CFT..TYP_NAV
              where nom              = @va_NomType_in
              
            /*A
            ** Le type NAV n'existe pas dans TYP_NAV
            */
               
            if @vl_NumNAV = null
            begin
                  
               /*A
               ** Insertion du type NAV dans la table TYP_NAV
               */
               
               select @vl_Numero = max(numero) from CFT..TYP_NAV
               
               if @vl_Numero = null
                  select @vl_Numero = 0
                     
               insert into CFT..TYP_NAV
               values ( @vl_Numero + 1, @va_NomType_in, @va_FlashMod4_in, @va_NbBarrieres_in  )
                  
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
         
#endif         

      commit tran
   end         
     
   return XDC_OK
         
go  
