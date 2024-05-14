/*E*/
/*Fichier : $Id: xzan39sp.prc,v 1.1 1996/12/05 13:12:22 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/12/05 13:12:22 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan39sp.prc
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

/* #include "xzan39sp.h" */

/* static char *version = "@(#)xzan32.c	1.1 23/18/94 : xzan32" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
* suppression d'une periode dans le calendrier des astreintes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;39
   @va_Interlocuteur_in		smallint = null,
   @va_NumCompetence_in		smallint = null,
   @va_DateDebut_in		datetime = null,
   @va_DateFin_in		datetime = null
   as 

/* 
*
* SP	XZAN39SP
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
      
   if @va_NumCompetence_in = null or @va_NumCompetence_in=0 or @va_Interlocuteur_in=0
      
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
	 delete from CFG..ANN_AST_CAL
	 where competence = @va_NumCompetence_in
	 and date_debut = @va_DateDebut_in 
	 and date_fin = @va_DateFin_in
	 and interlocuteur = @va_Interlocuteur_in

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
