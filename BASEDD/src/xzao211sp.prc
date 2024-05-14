/*E*/
/*Fichier : $Id: xzao211sp.prc,v 1.2 1995/01/09 13:18:27 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:18:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao211sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao211sp.h" */

/* static char *version = "@(#)xzao211.c	1.1 23/18/94 : xzao211" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Echangeurs d'un Troncon ou d'une portion
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;211
   @va_Basedd_in       	char(3),
   @va_NumeroPortion_in smallint,	
   @va_NomTroncon_in	char(25)
   as 

/* 
*
* SP	XZAO211SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Mot	va_NumeroPortion_in
* XDY_Nom	va_NomTroncon_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des Echangeurs d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon est non values alors la liste des Echangeurs du troncon
*
* Si NumeroPortion est value alors la liste des Echangeurs de la portion
*
* FONCTION :
*
* Retourne XZAOT_Echangeur
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	30 Nov 1994	: Ajout #ifdef CI (v1.2)
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
         ** NomTroncon est nul et NumeroPortion est nul
         ** Recherche des Echangeurs de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         
            select distinct ECH.nom, 
            		    POR.autoroute, 
            		    ECH.PR, 
            		    ECH.peage_entree_sens1, 
            		    ECH.PR_entree_sens1, 
            		    ECH.peage_entree_sens2, 
            		    ECH.PR_entree_sens2, 
            		    ECH.peage_sortie_sens1, 
            		    ECH.PR_sortie_sens1, 
            		    ECH.peage_sortie_sens2, 
            		    ECH.PR_sortie_sens2
            
            from CFG..RES_ECH ECH, CFG..RES_POR POR
	     
	      where ECH.portion = POR.numero and ECH.supprime = XDC_FAUX
	        
	      order by POR.autoroute, ECH.PR 
	 
	 
         /*A
         ** NomTroncon est nul 
         ** Recherche des Echangeurs de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         
            select distinct ECH.nom, 
            		    POR.autoroute, 
            		    ECH.PR, 
            		    ECH.peage_entree_sens1, 
            		    ECH.PR_entree_sens1, 
            		    ECH.peage_entree_sens2, 
            		    ECH.PR_entree_sens2, 
            		    ECH.peage_sortie_sens1, 
            		    ECH.PR_sortie_sens1, 
            		    ECH.peage_sortie_sens2, 
            		    ECH.PR_sortie_sens2
            
            from CFG..RES_ECH ECH, CFG..RES_POR POR
	     
	      where ECH.portion = @va_NumeroPortion_in and ECH.supprime = XDC_FAUX
	        
	      order by POR.autoroute, ECH.PR 
	 
	 
         /*A
         ** NumeroPortion est nul 
         ** Recherche des Echangeurs du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
            select distinct ECH.nom, 
            		    POR.autoroute, 
            		    ECH.PR, 
            		    ECH.peage_entree_sens1, 
            		    ECH.PR_entree_sens1, 
            		    ECH.peage_entree_sens2, 
            		    ECH.PR_entree_sens2, 
            		    ECH.peage_sortie_sens1, 
            		    ECH.PR_sortie_sens1, 
            		    ECH.peage_sortie_sens2, 
            		    ECH.PR_sortie_sens2
            
            from CFG..RES_ECH ECH, CFG..RES_POR POR
	     
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut  >  @vl_PRdebut
	        and POR.PR_fin    <= @vl_PRfin 
	        and ECH.portion   = POR.numero
	        and ECH.supprime = XDC_FAUX
	      order by POR.autoroute, ECH.PR 
	        
	 end   
      end
      
#ifdef CI      
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** NomTroncon est nul et NumeroPortion est nul
         ** Recherche des Echangeurs de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         
            select distinct ECH.nom, 
            		    POR.autoroute, 
            		    ECH.PR, 
            		    ECH.peage_entree_sens1, 
            		    ECH.PR_entree_sens1, 
            		    ECH.peage_entree_sens2, 
            		    ECH.PR_entree_sens2, 
            		    ECH.peage_sortie_sens1, 
            		    ECH.PR_sortie_sens1, 
            		    ECH.peage_sortie_sens2, 
            		    ECH.PR_sortie_sens2
            
            from CFT..RES_ECH ECH, CFT..RES_POR POR
	     
	      where ECH.portion = POR.numero and ECH.supprime = XDC_FAUX
	        
	      order by POR.autoroute, ECH.PR 
	 
	 
         /*A
         ** NomTroncon est nul 
         ** Recherche des Echangeurs de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         
            select distinct ECH.nom, 
            		    POR.autoroute, 
            		    ECH.PR, 
            		    ECH.peage_entree_sens1, 
            		    ECH.PR_entree_sens1, 
            		    ECH.peage_entree_sens2, 
            		    ECH.PR_entree_sens2, 
            		    ECH.peage_sortie_sens1, 
            		    ECH.PR_sortie_sens1, 
            		    ECH.peage_sortie_sens2, 
            		    ECH.PR_sortie_sens2
            
            from CFT..RES_ECH ECH, CFT..RES_POR POR
	     
	      where ECH.portion = @va_NumeroPortion_in and ECH.supprime = XDC_FAUX
	        
	      order by POR.autoroute, ECH.PR 
	 
	 
         /*A
         ** NumeroPortion est nul 
         ** Recherche des Echangeurs du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
            
            select distinct ECH.nom, 
            		    POR.autoroute, 
            		    ECH.PR, 
            		    ECH.peage_entree_sens1, 
            		    ECH.PR_entree_sens1, 
            		    ECH.peage_entree_sens2, 
            		    ECH.PR_entree_sens2, 
            		    ECH.peage_sortie_sens1, 
            		    ECH.PR_sortie_sens1, 
            		    ECH.peage_sortie_sens2, 
            		    ECH.PR_sortie_sens2
            
            from CFT..RES_ECH ECH, CFT..RES_POR POR
	     
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut  >  @vl_PRdebut
	        and POR.PR_fin    <= @vl_PRfin 
	        and ECH.portion   = POR.numero
	        and ECH.supprime = XDC_FAUX
	      order by POR.autoroute, ECH.PR 
	        
	 end   
      end
#endif      
      
   end
      
   return XDC_OK
         
go  
