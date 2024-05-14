/*E*/
/*Fichier : $Id: xzao128sp.prc,v 1.4 1995/01/09 13:16:11 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/01/09 13:16:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao128sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao128sp.h" */

/* static char *version = "@(#)xzao128.c	1.1 23/18/94 : xzao128" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;128
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumPortion_in 	smallint
   as 

/* 
*
* SP	XZAO128SP
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
*  Liste des aires d'un troncon
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
* Retourne la liste des aires du troncon
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	28 Nov 1994	: Modif retour select pour un troncon (v1.4)
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
   
   else   
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** NomTroncon, NumeroPortion sont nuls
         ** Recherche des aires de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumPortion_in = null
         
           select distinct AIR.nom, 
	 		AIR.numero,
	 		POR.autoroute,
	 		AIR.PRentree,
	 		AIR.sens,
	 		AIR.capacite_VL,
	 		AIR.capacite_PL 
	   from CFG..RES_POR POR, CFG..RES_AIR AIR, CFG..RES_AUT AUT
	 		  
	   where POR.autoroute = AUT.numero
	     and AIR.portion = POR.numero
	     
	   order by POR.autoroute, AIR.numero, AIR.PRentree
	   
	   
	 /*A
         ** NomTroncon est nul 
         ** Recherche des aires de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumPortion_in <> null
           
            select distinct AIR.nom, 
	 		AIR.numero,
	 		POR.autoroute,
	 		AIR.PRentree,
	 		AIR.sens,
	 		AIR.capacite_VL,
	 		AIR.capacite_PL 
	 from CFG..RES_POR POR, CFG..RES_AIR AIR
	 		  
	   where POR.numero  = @va_NumPortion_in
	     and AIR.portion = POR.numero
	     
	   order by POR.autoroute, AIR.numero, AIR.PRentree
	 		    
	
	 /*A
	 ** NumeroPortion est nul
	 ** Recherche du numero de l'autoroute, du PRdebut et PRfin du Troncon
	 */
	 
	 if @va_NomTroncon_in != null and  @va_NumPortion_in = null
         begin
         
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
	      where nom = @va_NomTroncon_in
	   
	 
	    /*A
	    ** Recherche des aires du troncon passe en argument
	    */
	 
	    select distinct 	AIR.nom, 
	 	      		AIR.numero,
	 			POR.autoroute,
	 			AIR.PRentree,
	 			AIR.sens,
	 			AIR.capacite_VL,
	 			AIR.capacite_PL 
	 			
	    from CFG..RES_POR POR, CFG..RES_AIR AIR
	 		  
	     where POR.autoroute = @vl_Autoroute
	       and POR.PR_debut <= @vl_PRfin 
	       and POR.PR_fin   >= @vl_PRdebut
	       and AIR.portion   = POR.numero
	     
	   order by POR.autoroute, AIR.numero, AIR.PRentree
	 end
	 		  
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** NomTroncon, NumeroPortion sont nuls
         ** Recherche des aires de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumPortion_in = null
         
           select distinct AIR.nom, 
	 		AIR.numero,
	 		POR.autoroute,
	 		AIR.PRentree,
	 		AIR.sens,
	 		AIR.capacite_VL,
	 		AIR.capacite_PL 
	   from CFT..RES_POR POR, CFT..RES_AIR AIR, CFG..RES_AUT AUT
	 		  
	   where POR.autoroute = AUT.numero
	     and AIR.portion = POR.numero
	     
	   order by POR.autoroute, AIR.numero, AIR.PRentree
	   
	   
	 /*A
         ** NomTroncon est nul 
         ** Recherche des aires de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumPortion_in <> null
           
            select distinct AIR.nom, 
	 		AIR.numero,
	 		POR.autoroute,
	 		AIR.PRentree,
	 		AIR.sens,
	 		AIR.capacite_VL,
	 		AIR.capacite_PL 
	 from CFT..RES_POR POR, CFT..RES_AIR AIR
	 		  
	   where POR.numero  = @va_NumPortion_in
	     and AIR.portion = POR.numero
	     	  
	     
	   order by POR.autoroute, AIR.numero, AIR.PRentree
	 		    
	
	 /*A
	 ** NumeroPortion est nul
	 ** Recherche du numero de l'autoroute, du PRdebut et PRfin du Troncon
	 */
	 
	 if @va_NomTroncon_in != null and  @va_NumPortion_in = null
         begin
         
           select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
	      where nom = @va_NomTroncon_in
	   
	 
	    /*A
	    ** Recherche des aires du troncon passe en argument
	    */
	 
	    select distinct 	AIR.nom, 
	 			AIR.numero,
	 			POR.autoroute,
	 			AIR.PRentree,
	 			AIR.sens,
	 			AIR.capacite_VL,
	 			AIR.capacite_PL 
	    
	    from CFT..RES_POR POR, CFT..RES_AIR AIR
	 		  
	     where POR.autoroute = @vl_Autoroute
	       and POR.PR_debut <= @vl_PRfin 
	       and POR.PR_fin   >= @vl_PRdebut
	       and AIR.portion   = POR.numero
	     
	     order by POR.autoroute, AIR.numero, AIR.PRentree
	 end
      end

#endif

   end
      
   return XDC_OK
         
go  
