/*E*/
/*Fichier : $Id: xzan14sp.prc,v 1.5 2020/11/09 14:24:38 devgtie Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/09 14:24:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* B.GABORIT	21 Nov 1994	: modif (1 seule competence)
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/



/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Supprime une competences a un interlocuteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;14
   @va_NumFicheAnnuaire_in	smallint = null,
   @va_NumCompetence_in		smallint = null
   as 

/* 
*
* SP	XZAN14SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* 
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK
* XDC_ARG_INV
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE (AU CI UNIQUEMENT !)
* Ne pas oublier qu'il existe egalement une competence liee a la fonction
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
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumFicheAnnuaire_in = null or @va_NumCompetence_in = null
      
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
           
         /*A
         ** La fiche annuaire existe
         */
         
         if @vl_Fiche != null
         begin
            
            delete CFG..ANN_CMP
            where interlocuteur = @va_NumFicheAnnuaire_in and
                  competence = @va_NumCompetence_in
         
         end
         
         else
         
         begin
         
         /*A
         ** La fiche annuaire n'existe pas
         ** return XDC_NOK
         */
         
            return ( XDC_NOK ) 
         
         end
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
