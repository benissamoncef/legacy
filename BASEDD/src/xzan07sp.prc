/*E*/
/*Fichier : $Id: xzan07sp.prc,v 1.3 2016/09/21 10:39:52 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2016/09/21 10:39:52 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* gaborit	03 dec 1994	: modif bug applix null
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan07sp.h" */

/* static char *version = "@(#)xzan07.c	1.1 23/18/94 : xzan07" ; */

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

create procedure XZAN;07
   @va_NumAstreinte_in	tinyint,
   @va_Autoroute_in	tinyint,
   @va_PR_in		int
   as 

/* 
*
* SP	XZAN07SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumAstreinte_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeComp
*
* VALEUR RENDUE :
*
* Liste des competences (numero) verifiantles criteres de recherche. (si Autoroute = null,
* toutes les competences ayant NumAstreinte)
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* NumAstreinte inconnu
* Autoroute inconnue 
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
   
   @vl_Auto	tinyint,
   @vl_Compt	smallint,
   @vl_Zone	int 
     
   /*A bug applix */
   
   if @va_NumAstreinte_in=0 select @va_NumAstreinte_in=null
   if @va_PR_in = XDC_PR_INCONNU select @va_PR_in=null
   if @va_Autoroute_in = 0 select @va_Autoroute_in=null
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumAstreinte_in	= null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base CFG
      */   
      
      select @vl_Compt = numero from CFG..TYP_AST
        where numero = @va_NumAstreinte_in
        
      /*A
      ** Le type d'astreinte existe
      */
      
      if @vl_Compt != null
      begin
      
         /*B
         ** L'autoroute est null
         */
         
         if @va_Autoroute_in = null
         begin
            
            select numero from CFG..ANN_AST_CMP
              where type_d_astreinte = @va_NumAstreinte_in
         
         end
         
         else
         
         begin
         
         /*B
         ** L'autoroute est non null
         */
         
            select @vl_Auto = numero from CFG..RES_AUT
              where numero = @va_Autoroute_in
     
            /*B
            ** L'autoroute existe
            */
         
            if @vl_Auto != null
            begin
            
               (select CMP.numero from CFG..ANN_AST_CMP CMP, CFG..ANN_GEO GEO
                 where GEO.autoroute 		= @va_Autoroute_in
                   and (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_debut else GEO.PR_fin end) <= @va_PR_in
                   and (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_fin else GEO.PR_debut end) >= @va_PR_in
                   and GEO.competence		= CMP.numero
                   and CMP.type_d_astreinte 	= @va_NumAstreinte_in
                   
               union
               
               select numero from CFG..ANN_AST_CMP
                 where numero not in
                    ( select competence from CFG..ANN_GEO )
                   and type_d_astreinte = @va_NumAstreinte_in  )
               
               order by numero
               
            end
         
            else
         
            /*B
            ** L'autoroute n'existe pas
            */
   
               return ( XDC_NOK )   
         end
      end
      
      else
      
      /*A
      ** Le type d'astreinte n'existe pas
      */ 
   
         return ( XDC_NOK )
   
   end
     
   return ( XDC_OK )
         
go  
