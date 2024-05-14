/*E*/
/*Fichier : $Id: xzai01sp.prc,v 1.1 1994/11/08 17:44:28 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/08 17:44:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Archive le message TFM propose par le SAD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;01
   @va_NumPA_in			smallint = null,
   @va_Message_in		char(250)
   as 

/* 
*
* SP	XZAI01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumPA_in
* XDY_Commentaire	va_Message_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Archive le message TFM propose par le SAD
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
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_PA 		smallint,
   @vl_Ordre 		smallint,
   @vl_Numero		smallint
       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumPA_in = null
   or @va_Message_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base EXP
      */   
      
      select @vl_PA = numero, @vl_Ordre = ordre from EXP..SAD_PLN
      
      /*A
      ** Le plan d'action existe
      ** insertion du message dans SAD_TXT_TFM
      */
      
      if @vl_PA != null
      begin   
         
         select @vl_Numero = max ( morceau ) from EXP..SAD_TXT_TFM
           where numero = @va_NumPA_in
         
         if @vl_Numero = null
            select @vl_Numero = 0
            
         insert into EXP..SAD_TXT_TFM
         values ( @va_NumPA_in, @vl_Ordre, @vl_Numero + 1, @va_Message_in )
      
      end  
   end
     
   return ( XDC_OK )
         
go  
