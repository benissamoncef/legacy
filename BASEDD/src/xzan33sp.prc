/*E*/
/*Fichier : $Id: xzan33sp.prc,v 1.5 2020/11/09 14:13:05 devgtie Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/11/09 14:13:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan33sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	02 dec 1994	: modif (DEM/1306)1.2
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/



/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Affecte une fiche anuaire au calandrier d'une competence pour une periode donnee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;33
   @va_NumCompetence_in		smallint = null,
   @va_DateDebut_in		datetime = null,
   @va_DateFin_in		datetime = null,
   @va_NumFicheAnnuaire_in	smallint = null	
   
   as 

/* 
*
* SP	XZAN33SP
*
* PARAMETRES EN ENTREE :
*
*
* XDY_Mot		va_NumCompetence_in
* XDY_Horodate		va_DateDebut_in
* XDY_Horodate		va_DateFin_in
* XDY_Mot		va_NumFicheAnnuaire_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Affecte une fiche anuaire au calandrier d'une competence pour une periode donnee
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumCompetence inconnu
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

   @vl_Fiche		smallint,
   @vl_Compt		smallint,
   @vl_dateD		datetime,
   @vl_dateF		datetime,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */

   if @va_NumFicheAnnuaire_in 	= 0 select @va_NumFicheAnnuaire_in = null
   if @va_NumCompetence_in = 0 select @va_NumCompetence_in =null
   if @va_DateDebut_in = "1900" select @va_DateDebut_in =null
   if @va_DateFin_in = "1900" select @va_DateFin_in =null
      
   if @va_NumFicheAnnuaire_in 	= null
   or @va_NumCompetence_in	= null
   or @va_DateDebut_in		= null
   or @va_DateFin_in		= null
      
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
           
         /*A
         ** La fiche annuaire et la competence existent
         */
         
         if @vl_Fiche != null and @vl_Compt != null
         begin
            
            /*B
            ** Modification du commentaire de la fiche annuaire
            */
            select @vl_dateD=convert(datetime,convert(char(8),@va_DateDebut_in,3))
            select @vl_dateF=dateadd(day,
                                     1,
                                     convert(datetime,convert(char(8),@va_DateFin_in,3)))
     
            update CFG..ANN_AST_CAL
            
            set interlocuteur = @va_NumFicheAnnuaire_in
            
            where date_debut>= @vl_dateD
              and date_debut< @vl_dateF
              and competence = @va_NumCompetence_in
              
         end
         
         else
         
         begin
         
         /*A
         ** La fiche annuaire ou la competence n'existe pas
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
