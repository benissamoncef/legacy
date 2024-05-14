/*E*/
/*Fichier : $Id: xzan40sp.prc,v 1.1 1996/12/05 13:12:24 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/12/05 13:12:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan40sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	15 nov 1996	: Creation (DEM/1306)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan40sp.h" */

/* static char *version = "@(#)xzan32.c	1.1 23/18/94 : xzan32" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
* ajout d'une periode dans le calendrier des astreintes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;40
   @va_NumCompetence_in		smallint = null,
   @va_NumInterlocuteur_in	smallint = null,
   @va_DateDebut_in		datetime = null,
   @va_DateFin_in		datetime = null
   as 

/* 
*
* SP	XZAN40SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeFiche
*
* VALEUR RENDUE :
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumCompetence inconnu
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
    
   @vl_Compt	smallint
       
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumCompetence_in = null or @va_NumCompetence_in=0
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base CFG
      */   
      
      select @vl_Compt = numero from CFG..ANN_AST_CMP
        where numero = @va_NumCompetence_in
      
      /*A
      ** La competence existe
      */
      
      if @vl_Compt != null
      begin
      
	 /*suppression dans le calendrier*/
	 insert into CFG..ANN_AST_CAL
	 values ( @va_NumInterlocuteur_in,@va_NumCompetence_in,@va_DateDebut_in,@va_DateFin_in)

	if @@rowcount=0
	  return (XDC_NOK)
      end
      
      else
      
      /*A
      ** La competence n'existe pas
      ** return XDC_NOK
      */
      
         return ( XDC_NOK )
      
   end
     
   return ( XDC_OK )
         
go  
