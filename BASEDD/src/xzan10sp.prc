/*E*/
/*Fichier : $Id: xzan10sp.prc,v 1.5 2007/12/13 10:50:11 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2007/12/13 10:50:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	03 dec 1994	: modif tel1 en tel3
* guilhou	20 nov 1996	: ajout radio (DEM/1306) 1.3
* cluchague     02 fev 1998     : gestion des champs site_responsable et site_concerne
* JMG	31/10/07	: ajout adresse_email 1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan10sp.h" */

/* static char *version = "@(#)xzan10.c	1.1 23/18/94 : xzan10" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une fiche annuaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;10
   @va_NumFicheAnnuaire_in	smallint = null,
   @va_NomInterlocuteur_out	char(25) = null output,
   @va_Fonction_out		char(25) = null output,
   @va_NumCompetence_out	smallint = null output,
   @va_Adresse_out		char(50) = null output,
   @va_ExplicationTel1_out	char(25) = null output,
   @va_Telephone1_out		char(15) = null output,
   @va_ExplicationTel2_out	char(25) = null output,
   @va_Telephone2_out		char(15) = null output,
   @va_ExplicationTel3_out	char(25) = null output,
   @va_Telephone3_out		char(15) = null output,
   @va_ExplicationFax_out	char(25) = null output,
   @va_Fax_out			char(15) = null output,
   @va_ExplicationBip_out	char(25) = null output,
   @va_Bip_out			char(15) = null output,
   @va_TypeBip_out		tinyint = null output,
   @va_Commentaire_out		char(250) = null output,
   @va_ValiditeCommentaire_out	datetime = null output,
   @va_TelephoneCommentaire_out	char(15) = null output,
   @va_Radio_out		char(15) = null output,
   @va_ExplicationRadio_out	char(25) = null output,
   @va_Site_responsable_out     tinyint  = null output,
   @va_Site_concerne_out        tinyint  = null output,
   @va_Adresse_Email_out        char(100) = null output
   as 

/* 
*
* SP	XZAN10SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Nom		va_NomInterlocuteur_out
* XDY_Nom		va_Fonction_out
* XDY_Mot		va_NumCompetence_out
* XDY_Phrase		va_Adresse_out
* XDY_Nom		va_ExplicationTel1_out
* XDY_Tel		va_Telephone1_out
* XDY_Nom		va_ExplicationTel2_out
* XDY_Tel		va_Telephone2_out
* XDY_Nom		va_ExplicationTel3_out
* XDY_Tel		va_Telephone3_out
* XDY_Nom		va_ExplicationFax_out
* XDY_Tel		va_Fax_out
* XDY_Nom		va_ExplicationBip_out
* XDY_Tel		va_Bip_out
* XDY_Octet		va_TypeBip_out
* XDY_TexteAnn		va_Commentaire_out
* XDY_Horodate		va_ValiditeCommentaire_out
* XDY_Tel		va_TelephoneCommentaire_out
* XDY_Tel		va_Radio_out
* XDY_Nom		va_ExplicationRadio_out
* XDY_Octet             va_Site_responsable_out
* XDY_Octet             va_Site_concerne_out
*
* VALEUR RENDUE :
*
* Lit une fiche annuaire
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumFicheAnnuaire inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** declaration des variables locales
   */
   
   @vl_Fiche	smallint
     
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumFicheAnnuaire_in	= null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base CFG
      */   
      
      select @vl_Fiche = numero from CFG..ANN_FIC
        where numero = @va_NumFicheAnnuaire_in
        
      /*A
      ** La fiche annuaire existe
      */
      
      if @vl_Fiche != null
      begin
      
         /*B
         ** Recherche de la fiche annuaire
         */
         
         select	@va_NomInterlocuteur_out 	= nom,
         	@va_Fonction_out		= fonction,
         	@va_NumCompetence_out		= competence,
         	@va_Adresse_out			= adresse,
         	@va_ExplicationTel1_out		= explication_tel1,
         	@va_Telephone1_out		= tel1,
       		@va_ExplicationTel2_out		= explication_tel2,
         	@va_Telephone2_out		= tel2,
                @va_ExplicationTel3_out		= explication_tel3,
         	@va_Telephone3_out		= tel3,
                @va_ExplicationFax_out		= explication_fax,
         	@va_Fax_out			= fax,
                @va_ExplicationBip_out		= explication_bip,
         	@va_Bip_out			= bip,
                @va_TypeBip_out			= type_de_bip,
                @va_Commentaire_out		= commentaire,
                @va_ValiditeCommentaire_out	= validite_commentaire,
                @va_TelephoneCommentaire_out	= tel_commentaire,
		@va_Radio_out			= radio,
		@va_ExplicationRadio_out	= explication_radio,
		@va_Site_responsable_out        = site_responsable,
		@va_Site_concerne_out           = site_concerne,
		@va_Adresse_Email_out		= adresse_email
                
         from CFG..ANN_FIC
         
         where numero = @va_NumFicheAnnuaire_in
         
         /*B
         ** Si la validite du commentaire est permimee 
         ** update validite
         */
         
         if @va_ValiditeCommentaire_out < getdate()
         begin
           
            update CFG..ANN_FIC
            set validite_commentaire	= null,
            	commentaire		= null,
            	tel_commentaire		= null
              
            where numero = @va_NumFicheAnnuaire_in
         
         end
      end
      
      else
      
      /*A
      ** La fiche annuaire n'existe pas
      ** retuen XDC_NOK
      */ 
   
         return ( XDC_NOK )
   
   end
     
   return ( XDC_OK )
         
go  
