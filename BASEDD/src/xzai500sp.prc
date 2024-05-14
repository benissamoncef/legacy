/*E*/
/*Fichier : @(#)xzai500sp.prc	1.1      Release : 1.1        Date : 11/08/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai500sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      12 Fev 2004     : Creation (SAGA)
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Memorise le message TFM recu de DATEX dans la table TFM_DTX.
*  Si ce message n'est lie a aucune FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAI500' and type = 'P')
	drop procedure XZAI500
go

create procedure XZAI500
   @va_code_partenaire_in   char(10)=null, 
   @va_reference_datex_in   char(50)=null, 
   @va_lnk_in               char(35)=null,
   @va_HeureLancement_in    datetime,
   @va_texte_message_1_in   char(250)=null,
   @va_texte_message_2_in   char(250)=null
   as 

/* 
*
* SP	XZAI500_Enregistre_Message_TFM
*
* PARAMETRES EN ENTREE :
*
* va_code_partenaire_in   code du partenaire qui a emis le message
* va_reference_datex_in   reference datex associee        
* va_lnk_in               reference datex FMC associee        
* va_HeureLancement_in    heure de lancement
* va_texte_message_1_in   texte a diffuser (debut du message)
* va_texte_message_2_in   texte a diffuser (fin du message)
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

   /*A
   ** Declaration des variables locales
   */
   declare @vl_evt int,
	   @vl_cle tinyint,
	   @vl_NumeroAction int,
	   @vl_Status int

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV   
   */
 
   if @va_code_partenaire_in = null
   or @va_reference_datex_in = null
   or @va_lnk_in = null
   or @va_texte_message_1_in = null
      return ( XDC_ARG_INV )

   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /* Recherche de la FMC concernee par le message */
      select @vl_evt=evenement,@vl_cle=cle from EXP..ADA_SIT
      where emetteur=@va_code_partenaire_in and reference=@va_lnk_in and evenement<>null and cle<>null
      if @vl_evt<>null and @vl_cle<>null
      begin
	 /* La FMC existe, on ne fait rien et on retourne XDC_NOK. */
	 select @vl_Status = XDC_NOK
      end
      else
      begin
	 /* La FMC n'existe pas, le message est enregistre dans la table TFM_DTX */
	 if not exists (select * from EXP..TFM_DTX where code_partenaire=@va_code_partenaire_in and reference_datex=@va_reference_datex_in)
            insert into EXP..TFM_DTX (code_partenaire,reference_datex,texte_message_1, texte_message_2,nombre_diffusions,supprime)
            values ( @va_code_partenaire_in, @va_reference_datex_in, @va_texte_message_1_in, @va_texte_message_2_in, 0, 0 )
         else
	    update EXP..TFM_DTX 
	    set texte_message_1=@va_texte_message_1_in, 
		texte_message_2=@va_texte_message_2_in, 
		nombre_diffusions=0,supprime=0 
	    where code_partenaire=@va_code_partenaire_in and reference_datex=@va_reference_datex_in
         select @vl_Status = XDC_OK
      end
   end
     
   return ( @vl_Status )
         
go  
