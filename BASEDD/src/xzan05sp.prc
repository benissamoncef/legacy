/*E*/
/*Fichier : $Id: xzan05sp.prc,v 1.5 2020/11/09 13:59:35 devgtie Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/09 13:59:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	03 dec 1994	: Correction heure debut
* Guilhou	14 nov 1996	: refonte complete de la gestion des astreintes (DEM/1306) 1.3
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan05sp.h" */

/* static char *version = "$Id: xzan05sp.prc,v 1.5 2020/11/09 13:59:35 devgtie Exp $ $Revision: 1.5 $ $Date: 2020/11/09 13:59:35 $: xzan05" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Cree une fiche competence (et son calandrier vide)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;05
   @va_NumAstreinte_in		tinyint = null,
   @va_SousType_in		char(25) = null,
   @va_District_in		tinyint = null,
   @va_SiteResponsable_in	tinyint = null,
   @va_NumCompetence_out	smallint = null output
   as 

/* 
*
* SP	XZAN05SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumAstreinte_in
* XDY_Octet		va_SousTypeAstreinte
* XDY_Octet		va_SiteResponsable
* XDY_Mot		va_DistrictAstreinte_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumCompetence_out
*
* VALEUR RENDUE :
*
* Cree une fiche competence (et son calandrier vide)
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumAstreinte inconnu
* NumCategorie inconnu
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
* 
* penser a appeler XZAN16 pour ajouter les zones geographiques a la competence
*
* FONCTION :
*
* Insertion into COMPETENCE parametres
* POUR jour=aujourd'hui JUSQU'Aaujourd'hui + 400
*   insert into CALENDRIER_D_ASTREINTE
*     interlocuteur = null
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Numero		smallint,
   @vl_Type		tinyint,
   @vl_Categ		tinyint,
   @vl_Cpt		int,
   @vl_DateDeb		datetime,
   @vl_DateAuj		datetime,
   @vl_Site		tinyint

   select 	@vl_Cpt = 0,
   		@vl_DateAuj = getdate()
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumAstreinte_in	= null
   or @va_SousType_in	= null
   or @va_District_in	= null
      
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
      
         /*A
         ** Verification de l'existence du type et de la categorie d'astreinte
         */
         
         select @vl_Type = numero from CFG..TYP_AST
           where numero = @va_NumAstreinte_in
           
         /*select @vl_Categ = numero from CFG..ANN_AST_CAT
           where numero = @va_District_in  */
         
         
         /*A
         ** Le type et la categorie d'astreinte existent
         */
        
         if ( @vl_Type != null )
         begin
         
            /*B
            **  Mise de la date sous forme jj/mm/aa hh:00
            */   
            
            /*select @vl_DateDeb = convert ( char(2) , @va_HeureDebut_in, 8 ) + ':00'*/
      
            /*B
            **  Insertion des parametres dans la table ANN_AST_CMP
            */   
      
            select @vl_Numero = max ( numero ) from CFG..ANN_AST_CMP
            
            if @vl_Numero = null
               select @vl_Numero = 0
               
            select @va_NumCompetence_out = @vl_Numero + 1
            
            insert into CFG..ANN_AST_CMP (numero,type_d_astreinte,sous_type_d_astreinte,district_validite,site_responsable)
            values ( @va_NumCompetence_out, @va_NumAstreinte_in, @va_SousType_in, @va_District_in,@va_SiteResponsable_in)
          
            /*B
            **  Creation du calendrier dans la table ANN_AST_CAL
            */
        
/*	    insert into CFG..ANN_AST_CAL
	    values (null,@va_NumCompetence_out,null,null)*/

/*            while ( @vl_Cpt < 400 )
            begin
              
               insert into CFG..ANN_AST_CAL
               values ( null, @va_NumCompetence_out, dateadd ( day, @vl_Cpt, @vl_DateAuj ) )
               
               select @vl_Cpt = @vl_Cpt + 1
            end     */
         end
         
         else
         
         /*A
         ** Le type ou la categorie d'astreinte n'existe pas
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
