/*E*/
/*Fichier : $Id: xzan06sp.prc,v 1.4 2020/11/09 14:01:25 devgtie Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/09 14:01:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* guilhou	14 nov 1996	: categorie devient district de validite (DEM/1306) 1.2
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan06sp.h" */

/* static char *version = "@(#)xzan06.c	1.1 23/18/94 : xzan06" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Modifie une fiche competence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;06
   @va_NumCompetence_in		smallint = null,
   @va_SousType_in		char(25) = null,
   @va_District_in		tinyint = null,
   @va_SiteResponsable_in	tinyint = null
as
/* 
*
* SP	XZAN06SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
* char(25)		va_SousType_in
* XDY_Mot		va_District_in
* XDY_Mot		va_SiteResponsable_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Modifie une fiche competence (sauf type d'astreinte, pour lequel il faut supprimer
* la competence)
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumCompetence inconnu
* NumCategorie inconnu
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
* 
* penser a appeler XZAN16 et XZAN17 pour ajouter/supprimer les zones geographiques
*
* FONCTION :
*
* update COMPETENCE
*   set colonne = nouvelles valeurs
*
* MECANISMES :
*
*------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Compt		smallint,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumCompetence_in	= null
   or @va_SousType_in	= null
   or @va_District_in	= null
   or @va_SiteResponsable_in = null
      return ( XDC_ARG_INV )
   
   else   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
      
         /*
         ** Verification de l'existence de la competence 
         */
         
         select @vl_Compt = numero from CFG..ANN_AST_CMP
           where numero = @va_NumCompetence_in
           
         /*A
         ** La competence d'astreinte existe
         */
        
         if ( @vl_Compt != null )
         begin
         
            /*B
            **  modification de la table ANN_AST_CMP
            */   
      
            update CFG..ANN_AST_CMP
            set district_validite	= @va_District_in, 
                sous_type_d_astreinte 	= @va_SousType_in,
		site_responsable 	= @va_SiteResponsable_in
            where numero = @va_NumCompetence_in  
          
         end
         
         else
         
         /*A
         ** La competence ou la categorie d'astreinte n'existe pas
         */
         
            return ( XDC_NOK )    
      end
      
      else
      
      /*A
      ** Le serveur n'est pas le CI
      */
         
         return ( XDC_NOK )
      
   end 
     
   return ( XDC_OK )
         
go  
