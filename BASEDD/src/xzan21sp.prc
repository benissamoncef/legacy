/*E*/
/*Fichier : $Id: xzan21sp.prc,v 1.4 2020/11/09 14:12:09 devgtie Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/09 14:12:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* guilhou	24 dec 1996 : permettre que les champs modifies soient nuls (DEM/1367) 1.2
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23 	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan21sp.h" */

/* static char *version = "@(#)xzan21.c	1.1 27/10/94 : xzan21" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Commente une fiche anuaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;21
   @va_NumFicheAnnuaire_in		smallint = null,
   @va_NouveauCommentaire_in		char(250),
   @va_NouvelleValiditeComm_in		datetime = null,
   @va_NouveauTelephoneComm_in		char(15)
   
   as 

/* 
*
* SP	XZAN21SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_TexteAnn		va_NouveauCommentaire_in
* XDY_Horodate		va_NouvelleValiditeCommentaire_in
* XDY_Tel		va_NouveauTelephoneCommentaire_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Modifie un commentaire d'une fiche annuaire
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumFicheAnnuaire inconnu
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

   @vl_Fiche		smallint	,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumFicheAnnuaire_in 			= null
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
            
            /*B
            ** Modification du commentaire de la fiche annuaire
            */
            
            update CFG..ANN_FIC
            
            set commentaire 		= @va_NouveauCommentaire_in,
            	validite_commentaire 	= @va_NouvelleValiditeComm_in,
            	tel_commentaire		= @va_NouveauTelephoneComm_in
            
            where numero = @va_NumFicheAnnuaire_in
            
         end
         
         else
         
         begin
         
         /*A
         ** La zone geographique n'existe pas
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
