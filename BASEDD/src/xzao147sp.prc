/*E*/
/*Fichier : $Id: xzao147sp.prc,v 1.2 1995/01/09 13:16:33 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:16:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao147sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao147sp.h" */

/* static char *version = "@(#)xzao147.c	1.1 23/18/94 : xzao147" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des ITPC d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;147
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint	
   as 

/* 
*
* SP	XZAO147SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	va_NumeroPortion_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des ITPC d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des ITPC de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des ITPC du troncon
*
* Si NumeroPortion est value alors la liste des ITPC de la portion
*
* FONCTION :
*
* Retourne  XZAOT_ITPC
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
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
   if @va_NumeroPortion_in = 0		 	select @va_NumeroPortion_in = null
   
   
   if @va_Basedd_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** NomTroncon, NumeroPortion sont nuls
         ** Recherche des ITPC de l'ensemble des Autotoutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         
            select POR.autoroute, ITP.PR from CFG..RES_ITP ITP, CFG..RES_POR POR
              where ITP.portion = POR.numero
              order by POR.autoroute, ITP.PR
            
         /*A
         ** NomTroncon est nul 
         ** Recherche des ITPC de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
           
            select POR.autoroute, ITP.PR
            
            from CFG..RES_ITP ITP, CFG..RES_POR POR
            
	      where POR.numero  = @va_NumeroPortion_in
	        and ITP.portion = POR.numero
	        
	      order by POR.autoroute, ITP.PR  
	        
	 
	 /*A
         ** NumeroPortion est nul 
         ** Recherche des PMV du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
            select POR.autoroute, ITP.PR
            
            from CFG..RES_ITP ITP, CFG..RES_POR POR 
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut <= @vl_PRfin
	        and POR.PR_fin   >=  @vl_PRdebut 
	        and ITP.portion   = POR.numero
	        
	      order by POR.autoroute, ITP.PR  
	        
	        
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
         ** Recherche des ITPC de l'ensemble des Autotoutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         
            select POR.autoroute, ITP.PR from CFT..RES_ITP ITP, CFT..RES_POR POR
              order by POR.autoroute, ITP.PR
            
         /*A
         ** NomTroncon est nul 
         ** Recherche des ITPC de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
           
            select POR.autoroute, ITP.PR
            
            from CFT..RES_ITP ITP, CFT..RES_POR POR
            
	      where POR.numero  = @va_NumeroPortion_in
	        and ITP.portion = POR.numero
	        
	      order by POR.autoroute, ITP.PR  
	        
	 
	 /*A
         ** NumeroPortion est nul 
         ** Recherche des PMV du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
            
            select POR.autoroute, ITP.PR
            
            from CFT..RES_ITP ITP, CFT..RES_POR POR 
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut <= @vl_PRfin
	        and POR.PR_fin   >=  @vl_PRdebut 
	        and ITP.portion   = POR.numero
	        
	      order by POR.autoroute, ITP.PR  
	        
	        
	 end   
      end
#endif         
   
   end
      
   return XDC_OK
         
go  
