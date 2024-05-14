/*E*/
/*Fichier : $Id: xzao132sp.prc,v 1.5 1995/01/09 13:16:18 volcic Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/01/09 13:16:18 $
------------------------------------------------------
* GTIEA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao132sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao132sp.h" */

/* static char *version = "@(#)xzao132.c	1.1 23/18/94 : xzao132" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des reseaux des troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;132
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumPortion_in	smallint
   as 

/* 
*
* SP	XZAO132SP
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
*  Liste des reseaux d'un troncon
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
* Retourne la liste des reseaux du troncon
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.2)
* volcic	29 Nov 1994	: Modi clause where (v1.3)
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
	 ** Recherche des reseaux du troncon passe en argument
	 */
	 
	 select distinct TRN.numero, 
	 		TRN.nom, 
	 		TRN.PR,
			@vl_Autoroute,
	 		TRN.type
	 
	 from CFG..RES_POR POR, CFG..RES_TRN TRN
	 		  
	   where POR.autoroute 	 = @vl_Autoroute
	     and POR.PR_debut 	<= @vl_PRfin
	     and POR.PR_fin   	>= @vl_PRdebut
	     and TRN.portion   	 = POR.numero
	     and ( TRN.PR	>= @vl_PRdebut
	       and TRN.PR	<= @vl_PRfin )
	     
	   order by POR.autoroute, TRN.PR, TRN.numero
        end 
      else
        begin
	 /*A
	 ** Recherche des reseaux de la portion passee en argument
	 */
	 
	 select distinct TRN.numero, 
	 		TRN.nom, 
	 		TRN.PR,
			POR.autoroute,
	 		TRN.type
	 from CFG..RES_POR POR, CFG..RES_TRN TRN
	 		  
	   where POR.numero = @va_NumPortion_in
	     and TRN.portion   = POR.numero
	     
	   order by TRN.PR, TRN.numero
	 		  
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
	 ** Recherche des reseaux du troncon passe en argument
	 */
	 
	 select distinct TRN.numero, 
	 		TRN.nom, 
	 		TRN.PR,
			POR.autoroute,
	 		TRN.type
	 from CFT..RES_POR POR, CFT..RES_TRN TRN
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and TRN.portion   = POR.numero
	     and TRN.PR		<= @vl_PRfin
	     and TRN.PR		>= @vl_PRdebut
	    
	   order by POR.autoroute, TRN.PR, TRN.numero
        end 
      else
        begin
	 /*A
	 ** Recherche des reseaux de la portion passee en argument
	 */
	 
	 select distinct TRN.numero, 
	 		TRN.nom, 
	 		TRN.PR,
			POR.autoroute,
	 		TRN.type
	 from CFT..RES_POR POR, CFT..RES_TRN TRN
	 		  
	   where POR.numero = @va_NumPortion_in
	     and TRN.portion   = POR.numero
	     
	   order by TRN.PR, TRN.numero
	 		  
        end		  
	 		  
      end

#endif

   end
      
   return XDC_OK
         
go  
