/*E*/
/*Fichier : $Id: xzao134sp.prc,v 1.4 1994/11/28 19:09:10 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/28 19:09:10 $
------------------------------------------------------
* GTIEA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao134sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao134sp.h" */

/* static char *version = "@(#)xzao134.c	1.1 23/18/94 : xzao134" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des panneaux de police des troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;134
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumPortion_in	smallint
   as 

/* 
*
* SP	XZAO134SP
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
*  Liste des panneaux de police d'un troncon
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
* Retourne la liste des panneaux de police du troncon
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.2)
* gaborit	28 nov 1994	: ajout portion (1.4)
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
	 ** Recherche des panneaux de police du troncon passe en argument
	 */
	 
	 select distinct POL.PR, 
	 		POL.sens, 
	 		POL.vitesse,
			@vl_Autoroute
	 
	 from CFG..RES_POR POR, CFG..RES_POL POL
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and POL.portion   = POR.numero
	     and ( POL.PR >= @vl_PRdebut and POL.PR <= @vl_PRfin )
	     
	   order by POR.autoroute, POL.sens, POL.PR
        end 
      else
        begin
	 /*A
	 ** Recherche des panneaux de police de la portion passee en argument
	 */
	 
	 select distinct POL.PR, 
	 		POL.sens, 
	 		POL.vitesse,
			POR.autoroute
	 
	 from CFG..RES_POR POR, CFG..RES_POL POL
	 		  
	   where POR.numero = @va_NumPortion_in
	     and POL.portion   = POR.numero
	     
	   order by  POL.sens, POL.PR
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
	 ** Recherche des panneaux de police du troncon passe en argument
	 */
	 
	 select distinct POL.PR, 
	 		POL.sens, 
	 		POL.vitesse,
			@vl_Autoroute
	 
	 from CFT..RES_POR POR, CFT..RES_POL POL
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and POL.portion   = POR.numero
	     and ( POL.PR >= @vl_PRdebut and POL.PR <= @vl_PRfin )
	     
	   order by POR.autoroute, POL.sens, POL.PR
        end 
      else
        begin
	 /*A
	 ** Recherche des panneaux de police de la portion passee en argument
	 */
	 
	 select distinct POL.PR, 
	 		POL.sens, 
	 		POL.vitesse,
			POR.autoroute
	 
	 from CFT..RES_POR POR, CFT..RES_POL POL
	 		  
	   where POR.numero = @va_NumPortion_in
	     and POL.portion   = POR.numero
	     
	   order by  POL.sens, POL.PR
        end		  
	  		  
	 		  
      end

#endif
 
   end
      
   return XDC_OK
         
go  
