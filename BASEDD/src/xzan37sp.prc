/*E*/
/*Fichier : $Id: xzan37sp.prc,v 1.2 2001/04/19 16:25:27 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2001/04/19 16:25:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan37sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	25 Nov 1994	: Creation
* cluchague	25 Nov 2000	: Les liens ne sont plus bons
------------------------------------------------------*/



/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne le nom de l'interlocuteur associ≈ » une fonction
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;37
   @va_NumTypeAstreinte_in	tinyint = null,
   @va_Nom_out			char(25) = ' ' output
   as 

   select @va_Nom_out=FIC.nom
   from   CFG..ANN_FIC FIC,
          CFG..ANN_AST_CMP CMP
   where  FIC.competence=CMP.numero and
          CMP.type_d_astreinte=@va_NumTypeAstreinte_in
   
   select @va_Nom_out=isnull(@va_Nom_out,' ')
     
   return ( XDC_OK )
         
go  
