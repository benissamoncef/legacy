/*E*/
/*Fichier : $Id: xzan13sp.prc,v 1.3 2020/11/09 14:07:13 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/09 14:07:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan13sp.prc
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
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan13sp.h" */

/* static char *version = "@(#)xzan13.c	1.1 23/18/94 : xzan13" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une fiche annuaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;13
   @va_NumFicheAnnuaire_in	smallint = null,
   @va_NumCompetence_in		smallint = null
   as 

/* 
*
* SP	XZAN13SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Cree une fiche annuaire en controlant l'homonymie
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
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

   @vl_Fiche		smallint,
   @vl_Compt		smallint,	
   @vl_ComptTab		smallint,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumFicheAnnuaire_in = null
   or @va_NumCompetence_in = null
      
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
         
         select @vl_Fiche = numero from CFG..ANN_FIC
           where numero = @va_NumFicheAnnuaire_in
           
         select @vl_Compt = numero from CFG..ANN_AST_CMP
           where numero = @va_NumCompetence_in
           
         select @vl_Compt = competence from CFG..ANN_CMP
              where competence = @va_NumCompetence_in
            
         /*A
         ** La fiche annuaire et la competence existent
         */
         
         if @vl_Fiche != null and @vl_Compt != null and @vl_ComptTab = null
         begin
            
            insert into CFG..ANN_CMP
            values ( @va_NumFicheAnnuaire_in, @va_NumCompetence_in )
         
         end
         
         else
         
         /*A
         ** La fiche annuaire ou la competence n'existe pas
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
