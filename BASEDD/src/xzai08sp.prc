/*E*/
/*Fichier : $Id: xzai08sp.prc,v 1.3 1996/11/29 13:35:51 niepceron Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/11/29 13:35:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* gaborit	02 dec 1994	: modif non conformite 1.2
* niepceron	18 Oct 1996	: Ajout du niveau (dem/1248) 1.3
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Archive dans FMC_TFM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;08
   @va_NumEvt_in		int = null,
   @va_CleEvt_in		tinyint = null,
   @va_Horodate_in		datetime = null,
   @va_MessageOuAnnotation_in	bit,
   @va_Texte_in			char(250),
   @va_Niveau_in		tinyint = null
   as 

/* 
*
* SP	XZAI08SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Evt		va_NumEvt_in
* XDY_Horodate		va_Horodate_in
* XDY_Booleen		va_MessageOuAnnotation_in (XDC_TFM_MESSAGE ou XDC_TFM_ANNOTATION)
* XDY_Booleen		va_FinTexte_in
* XDY_Commentaire	va_Texte_in
* XDY_Octet		va_Niveau_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Archive dans FMC_TFM
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
* Si fin texte => reveil au CI "Message TFM pret" avec XZAI03
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Evt	int,
   @vl_Morceau	tinyint
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumEvt_in = null
   or @va_CleEvt_in = null
   or @va_Horodate_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      /*A
      ** Travail dans la base EXP
      */   
      
      if @va_Texte_in = null
         
         select @va_Texte_in = ' '
      
      select @vl_Evt = numero from EXP..FMC_GEN
        where numero 	= @va_NumEvt_in
          and cle 	= @va_CleEvt_in
   
      /*A
      ** L'evenement existe
      */
      
      if @vl_Evt != null
      begin
      
         /*A
         ** Le site est le CI
         */
      
            
            /*B
            ** Insertion dans la table FMC_TFM
            */
      
            select @vl_Morceau = max ( morceau ) from EXP..FMC_TFM
              where numero 	= @va_NumEvt_in
                and cle 	= @va_CleEvt_in
                and horodate    = @va_Horodate_in
            
            
            if @vl_Morceau = null
               select @vl_Morceau = 0 
            
            insert into EXP..FMC_TFM ( numero, cle, horodate, morceau, texte, commentaire, confidentiel , niveau)
            
            values ( @va_NumEvt_in, @va_CleEvt_in, @va_Horodate_in, @vl_Morceau + 1, @va_Texte_in, @va_MessageOuAnnotation_in, XDC_FAUX , @va_Niveau_in )        

            return ( XDC_OK )
      end
              
      else
   
      /*A
      ** L'evenement n'existe pas
      ** return ( XDC_NOK )
      */
         return ( XDC_NOK )
   
   
   return ( XDC_OK )
   
   end
         
go  
