/*E*/
/*Fichier : $Id: xzan11sp.prc,v 1.9 2020/11/09 14:05:59 devgtie Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/09 14:05:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic		26 Oct 1994	: Creation
* gaborit		03 dec 1994	: modif (homonyme+competence)
* torregrossa   14 Fev 1995	: ajout test date nulle applix (V1.3)
* Guilhou		21 nov 1996	: ajout radio(DEM/1306)			1.4
* Cluchague     02 Fev 1999	: Champs supplementaires site_responsable et site_concerne 1.5
* Guilhou		31/10/2007	: ajout adresse_email 1.6
* JPL			03/02/2009	: Ajout de l'indicateur d'inhibition (DEM 834) 1.7
* LCL			04/07/17	: Modif PRA
* RGR			09/11/2020	: Correction Pb de compilation
* GGY			26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan11sp.h" */

/* static char *version = "@(#)xzan11.c	1.1 23/18/94 : xzan11" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Modifie une fiche annuaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go


create procedure XZAN;11
   @va_NumFicheAnnuaire_in	smallint = null,
   @va_NomInterlocuteur_in	char(25) = null,
   @va_Fonction_in		char(25) = null,
   @va_NumCompetence_in		smallint = null,
   @va_Adresse_in		char(50) = null,
   @va_ExplicationTel1_in	char(25) = null,
   @va_Telephone1_in		char(15) = null,
   @va_ExplicationTel2_in	char(25) = null,
   @va_Telephone2_in		char(15) = null,
   @va_ExplicationTel3_in	char(25) = null,
   @va_Telephone3_in		char(15) = null,
   @va_ExplicationFax_in	char(25) = null,
   @va_Fax_in			char(15) = null,
   @va_ExplicationBip_in	char(25) = null,
   @va_Bip_in			char(15) = null,
   @va_TypeBip_in		tinyint = null,
   @va_Commentaire_in		char(250) = null,
   @va_ValiditeCommentaire_in	datetime = null,
   @va_TelephoneCommentaire_in	char(15) = null,
   @va_Radio_in			char(15) = null,
   @va_ExplicationRadio_in	char(25) = null,
   @va_Adresse_Email_in		char(100) = null,
   @va_Site_responsable_in      tinyint  = null,
   @va_Site_concerne_in         tinyint  = null,
   @va_Resultat_out		int = XDC_OK output,
   @va_Inhibe_in		tinyint = null
   as 

/* 
*
* SP	XZAN11SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_Nom		va_NomInterlocuteur_in
* XDY_Nom		va_Fonction_in
* XDY_Mot		va_NumCompetence_in
* XDY_Phrase		va_Adresse_in
* XDY_Nom		va_ExplicationTel1_in
* XDY_Tel		va_Telephone1_in
* XDY_Nom		va_ExplicationTel2_in
* XDY_Tel		va_Telephone2_in
* XDY_Nom		va_ExplicationTel3_in
* XDY_Tel		va_Telephone3_in
* XDY_Nom		va_ExplicationFax_in
* XDY_Tel		va_Fax_in
* XDY_Nom		va_ExplicationBip_in
* XDY_Tel		va_Bip_in
* XDY_Octet		va_TypeBip_in
* XDY_TexteAnn		va_Commentaire_in
* XDY_Horodate		va_ValiditeCommentaire_in
* XDY_Tel		va_TelephoneCommentaire_in
* XDY_Tel		va_Radio_in
* XDY_Nom		va_ExplicationRadio_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_resultat_out (OK fiche ajouter
*					 NOK homonymie - non ajouter)
*
* VALEUR RENDUE :
*
* Modifie une fiche annuaire en controlant l'homonymie
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
   @vl_Type		tinyint,
   @vl_Num		smallint,
   @vl_Site		tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumFicheAnnuaire_in	= null
   or @va_NomInterlocuteur_in	= null
      
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
      
	if @va_ValiditeCommentaire_in = XDC_DATE_NULLE	select @va_ValiditeCommentaire_in = null
	if @va_Fonction_in = XDC_CHAINE_VIDE	select @va_Fonction_in = null
   	if @va_TelephoneCommentaire_in	= XDC_CHAINE_VIDE select @va_TelephoneCommentaire_in = null
         
         /*A
         ** Le premier caractere du nom de l'interlocuteur n'est pas une majuscule, ou il existe un homonyme
         ** Resultat = XDC_NOK
         ** Return XDC_OK
         */
         select @vl_Num=numero from CFG..ANN_FIC where numero<>@va_NumFicheAnnuaire_in and nom=@va_NomInterlocuteur_in
         
         if ( substring ( @va_NomInterlocuteur_in , 1, 1 ) != upper ( substring ( @va_NomInterlocuteur_in , 1, 1 ) ) )
            or @vl_Num<>null
         begin
         
            select @va_Resultat_out = XDC_NOK
            return ( XDC_OK )
            
         end    
            
         else
            
         begin
         
         /*A
         ** Le premier caractere du nom de l'interlocuteur est une majuscule
         */
         
            select @vl_Fiche = numero from CFG..ANN_FIC
              where numero = @va_NumFicheAnnuaire_in
              
            select @vl_Compt = numero from CFG..ANN_AST_CMP 
              where numero  = @va_NumCompetence_in
                
              
            if @vl_Fiche != null and ( @vl_Compt != null or @va_NumCompetence_in=0 or @va_NumCompetence_in=null)
            begin
            
            /*B
            ** La fiche annuaire ,la competence et le type de bip exitent
            ** Update de la fiche annuaire
            */
            
               if @va_ExplicationTel1_in = null
               	  select @va_ExplicationTel1_in = ' '
               	  
               if @va_Telephone1_in = null
               	  select @va_Telephone1_in = ' '
               	  
               if @va_ExplicationTel2_in = null
               	  select @va_ExplicationTel2_in = ' '
               	  
               if @va_Telephone2_in = null
               	  select @va_Telephone2_in = ' '
               	  
               if @va_ExplicationTel3_in = null
               	  select @va_ExplicationTel3_in = ' '
               	  
               if @va_Telephone1_in = null
               	  select @va_Telephone1_in = ' '
               	  
               if @va_ExplicationFax_in = null
               	  select @va_ExplicationFax_in = ' '
               	  
               if @va_Fax_in = null
               	  select @va_Fax_in = ' '
               	  
               if @va_ExplicationBip_in = null
               	  select @va_ExplicationBip_in = ' '
               	  
               if @va_Bip_in = null
               	  select @va_Bip_in = ' '
               	  
         	if @va_Radio_in = null
			select @va_Radio_in = ' '

		if @va_ExplicationRadio_in = null
			select @va_ExplicationRadio_in = ' '
               	 
		 if @va_Adresse_Email_in = null
			select @va_Adresse_Email_in = ' ' 

               update CFG..ANN_FIC
            
               set	nom 			= @va_NomInterlocuteur_in,
                	fonction 		= @va_Fonction_in,
            		competence		= @va_NumCompetence_in,
            		adresse			= @va_Adresse_in,
            		explication_tel1	= @va_ExplicationTel1_in,
            		tel1			= @va_Telephone1_in,
            		explication_tel2	= @va_ExplicationTel2_in,
            		tel2			= @va_Telephone2_in,
            		explication_tel3	= @va_ExplicationTel3_in,
            		tel3			= @va_Telephone3_in,
            		explication_fax		= @va_ExplicationFax_in,
            		fax			= @va_Fax_in,
            		explication_bip		= @va_ExplicationBip_in,
            		bip			= @va_Bip_in,
            		type_de_bip		= @va_TypeBip_in,
            		commentaire		= @va_Commentaire_in,
            		validite_commentaire	= @va_ValiditeCommentaire_in,
            		tel_commentaire	= @va_TelephoneCommentaire_in,
			radio			= @va_Radio_in,
			explication_radio	= @va_ExplicationRadio_in,
			site_responsable        = @va_Site_responsable_in,
			site_concerne           = @va_Site_concerne_in,
			adresse_email		= @va_Adresse_Email_in,
			inhibition		= @va_Inhibe_in
            	
                where numero = @va_NumFicheAnnuaire_in
  
             end
         
             else
      
             /*A
             ** La fiche annuaire, la competence ou le type de bip n'existe pas
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
