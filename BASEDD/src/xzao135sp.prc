/*E*/
/*Fichier : $Id: xzao135sp.prc,v 1.5 2011/04/04 15:43:24 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2011/04/04 15:43:24 $
------------------------------------------------------
* GTIEA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao135sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao135sp.h" */

/* static char *version = "@(#)xzao135.c	1.1 23/18/94 : xzao135" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des passages superieurs des troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;135
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumPortion_in	smallint
   as 

/* 
*
* SP	XZAO135SP
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
*  Liste des passages superieurs d'un troncon
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
* Retourne la liste des passages superieurs du troncon
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* gaborit	28 nov 1994	: ajout portion (1.4)
* JMG		20/12/10	: correction autoroute cas 2 (1.5)à
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
	   
	 select @vl_Autoroute , @vl_PRdebut , @vl_PRfin
	 /*A
	 ** Recherche des passages superieurs du troncon passe en argument
	 */
	 
	 select distinct SUP.numero,
	 		SUP.nom,
	 		SUP.PR, 
	 		@vl_Autoroute, 
	 		SUP.gabarit_sens1, 
	 		SUP.gabarit_sens2
			
	 from CFG..RES_POR POR, CFG..RES_SUP SUP
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and SUP.portion   = POR.numero
	     and ( SUP.PR >= @vl_PRdebut and SUP.PR <= @vl_PRfin )
	     
	   order by SUP.PR, SUP.numero
        end 
      else
        begin
	 /*A
	 ** Recherche des passages superieurs de la portion passee en argument
	 */
	 
	 select distinct SUP.numero,
	 		SUP.nom,
	 		SUP.PR, 
	 		POR.autoroute,
	 		SUP.gabarit_sens1, 
	 		SUP.gabarit_sens2
			
	 from CFG..RES_POR POR, CFG..RES_SUP SUP
	 		  
	   where POR.numero = @va_NumPortion_in
	     and SUP.portion   = POR.numero
	     
	   order by SUP.PR, SUP.numero
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
	   
	 select @vl_Autoroute , @vl_PRdebut , @vl_PRfin
	 /*A
	 ** Recherche des passages superieurs du troncon passe en argument
	 */
	 
	 select distinct SUP.numero,
	 		SUP.nom,
	 		SUP.PR, 
	 		@vl_Autoroute, 
	 		SUP.gabarit_sens1, 
	 		SUP.gabarit_sens2
			
	 from CFT..RES_POR POR, CFT..RES_SUP SUP
	 		  
	   where POR.autoroute = @vl_Autoroute
	     and POR.PR_debut <= @vl_PRfin
	     and POR.PR_fin   >= @vl_PRdebut
	     and SUP.portion   = POR.numero
	     and ( SUP.PR >= @vl_PRdebut and SUP.PR <= @vl_PRfin )
	     
	   order by SUP.PR, SUP.numero
        end 
      else
        begin
	 /*A
	 ** Recherche des passages superieurs de la portion passee en argument
	 */
	 
	 select distinct SUP.numero,
	 		SUP.nom,
	 		SUP.PR, 
	 		@vl_Autoroute, 
	 		SUP.gabarit_sens1, 
	 		SUP.gabarit_sens2
			
	 from CFT..RES_POR POR, CFT..RES_SUP SUP
	 		  
	   where POR.numero = @va_NumPortion_in
	     and SUP.portion   = POR.numero
	     
	   order by SUP.PR, SUP.numero
        end 
	 		  
	 		  
      end

#endif

   end
      
   return XDC_OK
         
go  
