/*E*/
/*Fichier : $Id: xzan12sp.prc,v 1.1 1994/11/15 12:17:50 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 12:17:50 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan12sp.h" */

/* static char *version = "@(#)xzan12.c	1.1 23/18/94 : xzan12" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des competences (numero)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;12
   @va_NumFicheAnnuaire_in	smallint
   as 

/* 
*
* SP	XZAN12SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeComp
*
* VALEUR RENDUE :
*
* Retourne la liste des competences (numero) d'un inerlocuteur
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
        ** Recherche de la liste des competences d'un interlocuteur
        */
        
        select competence from CFG..ANN_CMP
          where interlocuteur = @va_NumFicheAnnuaire_in
      
      
      end
         
      else
      
      /*A
      ** La fiche annuaire n'existe pas
      ** return XDC_NOK
      */ 
   
         return ( XDC_NOK )
   
   end
     
   return ( XDC_OK )
         
go  
