/*E*/
/*Fichier : $Id: xzai02sp.prc,v 1.2 1996/11/29 13:35:49 niepceron Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/11/29 13:35:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* niepceron	18 Oct 1996	: Ajout du niveau (dem/1248) v1.2
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne le message TFM annote par l'operateur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;02
   @va_NumEvt_in		int = null,
   @va_CleEvt_in		tinyint = null,
   @va_Horodate_in		datetime = null
   as 

/* 
*
* SP	XZAI02SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Evt		va_NumEvt_in
* XDY_Horodate		va_Horodate_in
*
* PARAMETRES EN SORTIE :
*
* Char *		va_Message_out
* Char *		va_Annotations_out
* XDY_Booleen		va_confidentiel_out
*
* VALEUR RENDUE :
*
* Retourne le message TFM annote par l'operateur
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
declare @vl_FMC		int
       
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
      
      /*A
      ** Recherche des textes correspondants
      */
      
      select @vl_FMC = numero 
      
      from EXP..FMC_TFM
      
      where numero 	 = @va_NumEvt_in
        and cle		 = @va_CleEvt_in
        
      if @vl_FMC != null
         
         select 	morceau,
      			texte,
      			commentaire,
      			confidentiel,
      			niveau
      		
      	 from EXP..FMC_TFM
      
      	 where horodate = @va_Horodate_in
           and numero	= @va_NumEvt_in
           and cle	= @va_CleEvt_in     		
   
      else
      
         return (XDC_NOK)
   
   end
     
   return ( XDC_OK )
         
go  
