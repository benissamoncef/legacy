/*E*/
/*Fichier : $Id: xzan09sp.prc,v 1.5 2020/11/09 14:04:54 devgtie Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/09 14:04:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic		26 Oct 1994	: Creation
* gaborit		03 dec 1994	: modif insertion valeurs non nulles
* cluchague     02 Fev 1999	: mise a jour champ site_responsable 1.3
* LCL			04/07/17	: Modif PRA
* RGR			09/11/2020	: Correction Pb de compilation
* GGY			26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan09sp.h" */

/* static char *version = "@(#)xzan09.c	1.1 23/18/94 : xzan09" ; */

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

create procedure XZAN;09
   @va_NomInterlocuteur_in	char(25),
   @va_NumFicheAnnuaire_out	smallint = null output,
   @va_Resultat_out		tinyint = XDC_OK output
   as 

/* 
*
* SP	XZAN09SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_NomInterlocuteur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_Octet		va_resultat_out (OK fiche ajouter
*					 NOK homonymie - non ajouter)
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

   @vl_Fiche		smallint	,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NomInterlocuteur_in = null
      
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
         ** Le premier caractere du nom de l'interlocuteur n'est pas une majuscule
         ** Resultat = XDC_NOK
         ** Return XDC_NOK
         */
         
         if ( substring ( @va_NomInterlocuteur_in , 1, 1 ) != upper ( substring ( @va_NomInterlocuteur_in , 1, 1 ) ) )
         begin
         
            select @va_Resultat_out = XDC_NOK
            return ( XDC_OK )
            
         end    
            
         else
            
         begin
         
         /*A
         ** Le premier caractere du nom de l'interlocuteur est une majuscule
         */
         
            /*B
            ** L'interlocuteur existe deja dans une fiche annuaire
            */
            
		if ( ( select count(*) from CFG..ANN_FIC
                    where nom = @va_NomInterlocuteur_in ) != 0 )
                     
                select @va_Resultat_out = XDC_NOK    
            
            else
            
            /*B
            ** L'interlocuteur n'existe pas dans une fiche annuaire
            ** Creation de la fiche annuaire pour l'interlocuteur
            */
            
            begin
            
               select @vl_Fiche = max ( numero ) from CFG..ANN_FIC
               
               if @vl_Fiche = null
                  select @vl_Fiche = 0
                  
               insert into CFG..ANN_FIC (
                       numero,
                       competence,
                       nom,
                       adresse,
                       explication_tel1,
                       tel1,
                       explication_tel2,
                       tel2,
                       explication_tel3,
                       tel3,
                       explication_fax,
                       fax,
                       explication_bip,
                       type_de_bip,
                       bip,
                       commentaire,
                       validite_commentaire,
                       tel_commentaire,
                       fonction,
		       site_responsable)
               values (
                      @vl_Fiche + 1,
                      0,
                      @va_NomInterlocuteur_in,
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      ' ',
                      0,
                      ' ',
                      ' ',
                      '1900',
                      ' ',
                      ' ',
		                @vl_Site
                      )
               
               select @va_NumFicheAnnuaire_out = @vl_Fiche + 1 
                
            end   
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
