/*E*/
/*Fichier : $Id: xzan08sp.prc,v 1.5 2020/11/09 14:21:47 devgtie Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/09 14:21:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan08sp.h" */

/* static char *version = "@(#)xzan08.c	1.1 23/18/94 : xzan08" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Supprime une competence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;08
   @va_NumCompetence_in		smallint = null,
   @va_Resultat_out		int = XDC_OK output
   as 

/* 
*
* SP	XZAN08SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Supprime une competence,apres controle que la calendrier associe est vide
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumCompetence inconnu
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
* 
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Numero		smallint,
   @vl_Fiche		smallint,
   @vl_Interlocuteur	smallint,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumCompetence_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
      
         /*A
         ** Verification de l'existence de la competence
         */
         
         select @vl_Numero = numero from CFG..ANN_AST_CMP
           where numero = @va_NumCompetence_in
           
         /*A
         ** La competence existe
         */
        
         if @vl_Numero != null
         begin
         
            /*B
            ** Le calendrier est non vide dans la table ANN_AST_CAL
            ** Resultat = XZANC_CAL_NON_VIDE
            */   
      
            if ( ( select count(*) from CFG..ANN_AST_CAL
                    where competence = @va_NumCompetence_in and interlocuteur<>null) != 0 )
                     
                select @va_Resultat_out = XZANC_CAL_NON_VIDE    
            
            else
            
            begin
            
            /*B
            **  Le calendrier est vide dans la table ANN_AST_CAL
            **  delete des tables ANN_AST_CMP, ANN_GEO, ANN_CMP et update ANN_FIC
            */   
      
               declare COMP cursor
               for select numero from CFG..ANN_FIC
                     where competence = @va_NumCompetence_in
               
               open COMP
               
               delete CFG..ANN_AST_CMP
               where numero = @va_NumCompetence_in
                
               delete CFG..ANN_GEO
               where competence = @va_NumCompetence_in
                
               delete CFG..ANN_CMP
               where competence = @va_NumCompetence_in
                     
               fetch COMP into @vl_Interlocuteur
               
               while ( @@sqlstatus = 0 )
               begin
               
                  update CFG..RES_ECH
                  set gare_de_peage = null 
                  where gare_de_peage = @vl_Interlocuteur 
               
                  update CFG..RES_PEA
                  set gare_de_peage = null 
                  where gare_de_peage = @vl_Interlocuteur 
               
                  update CFG..RES_AIR
                  set sous_concessionnaire = null 
                  where sous_concessionnaire = @vl_Interlocuteur 
               
                  update CFG..RES_SUP
                  set interlocuteur = null 
                  where interlocuteur = @vl_Interlocuteur 
               
                  update CFG..RES_TRN
                  set interlocuteur = null 
                  where interlocuteur = @vl_Interlocuteur 
               
                  update CFG..RES_VIL
                  set interlocuteur = null 
                  where interlocuteur = @vl_Interlocuteur 
               
                  fetch COMP into @vl_Interlocuteur
               
               end
               
               close COMP
               
               update CFG..ANN_FIC
               set competence = null
               where competence = @va_NumCompetence_in
               
               select @va_Resultat_out = XDC_OK
               
            end   
         end
         
         else
         
         /*A
         ** La competence n'existe pas
         ** return XDC_NOK
         */
         
             return ( XDC_NOK )    
       end
      
       else
      
       /*A
       ** Le serveur n'est pas le CI
       ** return XDC_NOK
       */
         
         return ( XDC_NOK )
      
   end 
     
   return ( XDC_OK )
         
go  
