/*E*/
/*Fichier : $Id: xzal03sp.prc,v 1.1 1994/12/19 16:15:18 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/19 16:15:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAL * FICHIER xzal03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Appel >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	03 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Archive le motif d'un appel usager
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAL;03
   @va_Horodate_in	datetime = null,
   @va_NumeroAppele_in	char(15)
   as 

/* 
*
* SP	XZAL03SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_Tel		va_NumeroAppele_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Archive le motif d'un appel usager
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Quelquesoit le motif du FAX
*
* FONCTION :
*
* Ajoute une ligne a la table APL_FAX
*
* MECANISMES :
*
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Horodate_in = null
   or @va_NumeroAppele_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   ** Archivage d'un appel fax
   */
         
      insert into EXP..APL_FAX
      values ( @va_Horodate_in, @va_NumeroAppele_in )
      
   return ( XDC_OK )
         
go  
