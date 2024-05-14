/*E*/
/*Fichier : $Id: xzao152sp.prc,v 1.5 1995/01/09 13:16:44 volcic Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/01/09 13:16:44 $
------------------------------------------------------
* GTIEA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao152sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao152sp.h" */

/* static char *version = "@(#)xzao152.c	1.1 23/18/94 : xzao152" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des portails des troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;152
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumPortion_in 	smallint
   as 

/* 
*
* SP	XZAO152SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_Nom	va_NomTroncon_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des portails d'un troncon
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
* Retourne la liste des portails du troncon
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.2)
* gaborit	28 nov 1994	: ajout portion (1.4)
* volcic	29 nov 1994	: ajout test restriction troncon (1.5)
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Autoroute	tinyint,
   @vl_PRdebut		int,
   @vl_PRfin		int
      
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_NomTroncon_in ) = null 	select @va_NomTroncon_in = null
   if @va_NumPortion_in = 0		 	select @va_NumPortion_in = null
   
   
   if @va_Basedd_in = null
   
      return XDC_ARG_INV
      
   if @va_NomTroncon_in = null and  @va_NumPortion_in = null   
   
      return XDC_ARG_INV
   
   if @va_NomTroncon_in <> null and  @va_NumPortion_in <> null   
   
      return XDC_ARG_INV
   
   else   
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      if @va_NomTroncon_in <> null
        begin
       
         /*A
	 ** Recherche du numero de l'autoroute, du PRdebut et PRfin du Troncon
	 */
	 
	 select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
	   where nom = @va_NomTroncon_in
	   
	 
	 /*A
	 ** Recherche des portails du troncon passe en argument
	 */
	 
	 select distinct POR.autoroute,
	 		PTL.PR,
	 		PTL.sens
	 		
	 from CFG..RES_POR POR, CFG..RES_PTL PTL
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and PTL.portion   = POR.numero
	     and ( PTL.PR >= @vl_PRdebut and PTL.PR <= @vl_PRfin )
	   
	   order by POR.autoroute, PTL.sens, PTL.PR
        end 
      else
        begin
	 
	 /*A
	 ** Recherche des portails de la portion passe en argument
	 */
	 
	 select distinct POR.autoroute,
	 		PTL.PR,
	 		PTL.sens
	 		
	 from CFG..RES_POR POR, CFG..RES_PTL PTL
	 		  
	   where POR.numero = @va_NumPortion_in
	     and PTL.portion   = POR.numero
	     
	   order by  PTL.sens, PTL.PR
        end 
	 		  
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
      if @va_NomTroncon_in <> null
        begin
       
         /*A
	 ** Recherche du numero de l'autoroute, du PRdebut et PRfin du Troncon
	 */
	 
	 select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
	   where nom = @va_NomTroncon_in
	   
	 
	 /*A
	 ** Recherche des portails du troncon passe en argument
	 */
	 
	 select distinct POR.autoroute,
	 		PTL.PR,
	 		PTL.sens
	 		
	 from CFT..RES_POR POR, CFT..RES_PTL PTL
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and PTL.portion   = POR.numero
	     and ( PTL.PR >= @vl_PRdebut and PTL.PR <= @vl_PRfin )
	   
	   order by POR.autoroute, PTL.sens, PTL.PR
        end 
      else
        begin
	 
	 /*A
	 ** Recherche des portails de la portion passe en argument
	 */
	 
	 select distinct POR.autoroute,
	 		PTL.PR,
	 		PTL.sens
	 		
	 from CFT..RES_POR POR, CFT..RES_PTL PTL
	 		  
	   where POR.numero = @va_NumPortion_in
	     and PTL.portion   = POR.numero
	     
	   order by  PTL.sens, PTL.PR
        end 
	
	 		  
      end

#endif

   end
      
   return XDC_OK
         
go  
