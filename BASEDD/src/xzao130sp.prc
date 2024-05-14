/*E*/
/* Fichier : $Id: xzao130sp.prc,v 1.5 2018/10/23 15:40:53 devgfi Exp $        $Revision: 1.5 $        $Date: 2018/10/23 15:40:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao130sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation  1.1 - 1.4
* JPL	15/10/18 : Filtrage des stations RADT marquees comme supprimees (DEM 1305)  1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao130sp.h" */

/* static char *version = "@(#)xzao130.c	1.1 23/18/94 : xzao130" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Zones d'influences d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;130
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint	
   as 

/* 
*
* SP	XZAO130SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	a_NumeroPortion_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des Zones d'influences d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des Zones d'influences de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des Zones d'influences du troncon
*
* Si NumeroPortion est value alors la liste des Zones d'influences de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens, nom du type, nom du type de picto 
* des Zones d'influences a partir des tables EQT_INF, EQT_Zones d'influences, EQT_PCT, TYP_Zones d'influences, TYP_PCT
*
* MECANISMES :
*
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
   or ( @va_NomTroncon_in != null
    and @va_NumeroPortion_in != null )
    
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
         ** Recherche des Zones d'influences de l'ensemble des Autotoutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select distinct INF.portion, 
            		    INF.station, 
            		    INF.sens, 
            		    INF.PR_debut, 
            		    INF.PR_fin,
            		    POR.autoroute
            from CFG..RES_POR POR, CFG..RES_INF INF, CFG..EQT_GEN EQT
	     
	      where INF.portion   = POR.numero
	        and EQT.type      = XDC_EQT_RAD
	        and EQT.numero    = INF.station
	        and EQT.eqt_supprime = XDC_FAUX
	        
	      order by  POR.autoroute, INF.portion 
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des Zones d'influences de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select distinct INF.portion, 
            		    INF.station, 
            		    INF.sens, 
            		    INF.PR_debut, 
            		    INF.PR_fin,
            		    POR.autoroute
            from CFG..RES_POR POR, CFG..RES_INF INF, CFG..EQT_GEN EQT
	     
	      where POR.numero = @va_NumeroPortion_in
	        and INF.portion = POR.numero
	        and EQT.type = XDC_EQT_RAD
	        and EQT.numero = INF.station
	        and EQT.eqt_supprime = XDC_FAUX
	        
	      order by INF.portion 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des Zones d'influences du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
            select distinct INF.portion, 
            		    INF.station, 
            		    INF.sens, 
            		    INF.PR_debut, 
            		    INF.PR_fin,
            		    POR.autoroute
            from CFG..RES_POR POR, CFG..RES_INF INF, CFG..EQT_GEN EQT
	     
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut <= @vl_PRfin
	        and POR.PR_fin   >= @vl_PRdebut
	        and INF.portion   = POR.numero
	        and ((INF.PR_debut >= @vl_PRdebut and INF.PR_debut <= @vl_PRfin)
	          or (INF.PR_fin >= @vl_PRdebut and INF.PR_fin   <= @vl_PRfin))
	        and EQT.type = XDC_EQT_RAD
	        and EQT.numero = INF.station
	        and EQT.eqt_supprime = XDC_FAUX
	        
	      order by INF.portion 
	 
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
         ** Recherche des Zones d'influences de l'ensemble des Autotoutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select distinct INF.portion, 
            		    INF.station, 
            		    INF.sens, 
            		    INF.PR_debut, 
            		    INF.PR_fin,
            		    POR.autoroute
            from CFT..RES_POR POR, CFT..RES_INF INF, CFT..EQT_GEN EQT
	     
	      where INF.portion   = POR.numero
	        and EQT.type      = XDC_EQT_RAD
	        and EQT.numero    = INF.station
	        and EQT.eqt_supprime = XDC_FAUX
	        
	      order by  POR.autoroute, INF.portion 
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des Zones d'influences de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select distinct INF.portion, 
            		    INF.station, 
            		    INF.sens, 
            		    INF.PR_debut, 
            		    INF.PR_fin,
            		    POR.autoroute
            from CFT..RES_POR POR, CFT..RES_INF INF, CFT..EQT_GEN EQT
	     
	      where POR.numero = @va_NumeroPortion_in
	        and INF.portion = POR.numero
	        and EQT.type = XDC_EQT_RAD
	        and EQT.numero = INF.station
	        and EQT.eqt_supprime = XDC_FAUX
	        
	      order by INF.portion 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des Zones d'influences du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
            
            select distinct INF.portion, 
            		    INF.station, 
            		    INF.sens, 
            		    INF.PR_debut, 
            		    INF.PR_fin,
            		    POR.autoroute
            from CFT..RES_POR POR, CFT..RES_INF INF, CFT..EQT_GEN EQT
	     
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut <= @vl_PRfin
	        and POR.PR_fin   >= @vl_PRdebut
	        and INF.portion   = POR.numero
	        and ((INF.PR_debut >= @vl_PRdebut and INF.PR_debut <= @vl_PRfin)
	          or (INF.PR_fin >= @vl_PRdebut and INF.PR_fin   <= @vl_PRfin))
	        and EQT.type = XDC_EQT_RAD
	        and EQT.numero = INF.station
	        and EQT.eqt_supprime = XDC_FAUX
	        
	      order by INF.portion 
	 
	 end   
      end

#endif
      
   end
      
   return XDC_OK
         
go  
