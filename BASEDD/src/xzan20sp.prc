/*E*/
/*Fichier : $Id: xzan20sp.prc,v 1.4 2020/11/09 14:11:17 devgtie Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/09 14:11:17 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan20sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* LCL		04/07/17	: Modif PRA
* RGR		09/11/2020	: Correction Pb de compilation
* GGY		26/09/23 	: Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan20sp.h" */

/* static char *version = "@(#)xzan20.c	1.1 23/18/94 : xzan20" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Supprime une fiche anuaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;20
   @va_NumFicheAnnuaire_in	smallint = null,
   @va_Resultat_out		int = null output
   as 

/* 
*
* SP	XZAN20SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_Resultat_out
*
* VALEUR RENDUE :
*
* Supprime une fiche annuaire
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* NumFicheAnnuaire inconnu
*
* CONDITIONS D'UTILISATION :
*
* Configuration ON LINE
* 
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

declare
   
   /*A
   ** Definition des variables locales
   */

   @vl_Fiche		smallint	,
   @vl_Site 		tinyint


   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumFicheAnnuaire_in 	= null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
	/* Verification que le site est bien celui du CI ou CA */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin
         
         select @vl_Fiche = numero from CFG..ANN_FIC
           where numero = @va_NumFicheAnnuaire_in
           
         /*A
         ** La fiche annuaire existe
         ** Suppression de celle ci
         */
         
         if @vl_Fiche != null
         begin
            
            /*B
            ** Recherche de l'interlocuteur dans le calendrier d'astreinte
            */
            
            if ( select count(*) from CFG..ANN_AST_CAL where interlocuteur = @vl_Fiche ) != 0
            
               select @va_Resultat_out = XDC_NOK
               
            else
            begin tran
               
            /*B
            ** Suppression de la fiche annuaire
            */
            
            delete CFG..ANN_FIC
            where numero = @va_NumFicheAnnuaire_in
            
            /*B
            ** Mise a jour des interlocuteur du reseau autoroutier
            */
            
            update CFG..RES_ECH
            set gare_de_peage = null 
            where gare_de_peage = @vl_Fiche 
               
            update CFG..RES_PEA
            set gare_de_peage = null 
            where gare_de_peage = @vl_Fiche 
               
            update CFG..RES_AIR
            set sous_concessionnaire = null 
            where sous_concessionnaire = @vl_Fiche 
               
            update CFG..RES_SUP
            set interlocuteur = null 
            where interlocuteur = @vl_Fiche 
               
            update CFG..RES_TRN
            set interlocuteur = null 
            where interlocuteur = @vl_Fiche 
               
            update CFG..RES_VIL
            set interlocuteur = null 
            where interlocuteur = @vl_Fiche 
            
            commit
         end
         
         else
         
         begin
         
         /*A
         ** La fiche annuaire n'existe pas
         ** return XDC_NOK
         */
         
            return ( XDC_NOK ) 
         
         end
      end
      
      else
      
      /*A
      ** Le serveur n'est pas le CI
      ** return XDC_NOK
      */
         
         return ( XDC_NOK )
      
   end 
     
   return ( XDC_OK )
         
go  
