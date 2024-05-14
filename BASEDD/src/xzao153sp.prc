/*E*/
/* Fichier : $Id: xzao153sp.prc,v 1.2 2020/11/03 17:02:30 pc2dpdy Exp $        $Revision: 1.2 $        $Date: 2020/11/03 17:02:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao125sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	07/05.19	Creation DEM1334 1.1
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao125sp.h" */

/* static char *version = "@(#)xzao125.c	1.1 23/18/94 : xzao125" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des equipements d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO153
go

create procedure XZAO153
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint,
   @va_TypeEqt_in	tinyint	
   as 

/* 
*
* SP	XZAO125SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des equipements de type choisi d'un troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des equipements de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des equipements du troncon
*
* Si NumeroPortion est value alors la liste des equipements de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens, numero du type de l'equipement
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
    
   /*! transfo en param…tres nulls bug applix */
   if rtrim ( @va_Basedd_in ) = null		select @va_Basedd_in = null
   if rtrim ( @va_NomTroncon_in ) = null 	select @va_NomTroncon_in = null
   if @va_NumeroPortion_in = 0 			select @va_NumeroPortion_in = null
   if @va_TypeEqt_in = 0 			select @va_TypeEqt_in = null
   
   if @va_Basedd_in = null
   or @va_TypeEqt_in = null
   or (@va_NomTroncon_in != null and @va_NumeroPortion_in != null)
    
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
         ** Recherche des equipements de l'ensemble des Autotoutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFG..EQT_GEN e, CFG..EQT_PAU p
	      where e.type = @va_TypeEqt_in
	        and e.eqt_supprime = XDC_FAUX
		and e.numero = p.numero
		and p.type_PAU != 1
	      order by e.sens, e.PR
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des equipements de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
            
            
	    if @va_NumeroPortion_in=73
            begin
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFG..EQT_GEN e,  CFG..EQT_PAU p
	      where e.type = @va_TypeEqt_in
		and e.eqt_supprime = XDC_FAUX
	 	and e.autoroute = @vl_Autoroute
	        and e.PR       >  @vl_PRdebut
	        and e.PR       <= @vl_PRfin 
		and e.numero = p.numero
		and p.type_PAU != 1
	      
	    union
            select G.numero, G.nom, 1, 172254,G.numero%2+2  from CFG..EQT_GEN G, CFG..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D35"
	    union
            select G.numero, G.nom, 1,172444,2 from CFG..EQT_GEN G, CFG..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D535"
		order by sens, PR
	    end
	    else
            select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFG..EQT_GEN e, CFG..EQT_PAU p
	      where e.type = @va_TypeEqt_in
		and e.eqt_supprime = XDC_FAUX
	 	and e.autoroute = @vl_Autoroute
	        and e.PR       >  @vl_PRdebut
	        and e.PR       <= @vl_PRfin 
		and e.numero = p.numero
		and p.type_PAU != 1
	      order by e.sens, e.PR
	        
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des equipements du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
            if ( @va_NomTroncon_in="BREGUIERES A8")
            begin
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFG..EQT_GEN e, CFG..EQT_PAU p
	      where e.type = @va_TypeEqt_in
		and e.eqt_supprime = XDC_FAUX
	 	and e.autoroute = @vl_Autoroute
	        and e.PR       >  @vl_PRdebut
	        and e.PR       <= @vl_PRfin 
		and e.numero = p.numero
		and p.type_PAU != 1
	      
	    union
            select G.numero, G.nom, 1, 172254,G.numero%2+2  from CFG..EQT_GEN G, CFG..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D35"
	    union
            select G.numero, G.nom, 1,172444,2 from CFG..EQT_GEN G, CFG..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D535"
		order by sens, PR
	    end
	    else
            select numero, nom, autoroute, PR, sens from CFG..EQT_GEN
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = @vl_Autoroute
	        and PR       >  @vl_PRdebut
	        and PR       <= @vl_PRfin 
		order by sens, PR
	 
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
         ** Recherche des equipements de l'ensemble des Autotoutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFT..EQT_GEN e, CFT..EQT_PAU p
	      where type = @va_TypeEqt_in
	        and eqt_supprime = XDC_FAUX
		and e.numero =p.numero
		and p.type_PAU != 1
	      order by sens, PR
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des equipements de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in

	    if @va_NumeroPortion_in=73
            begin
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFT..EQT_GEN e, CFT..EQT_PAU p
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = @vl_Autoroute
	        and PR       >  @vl_PRdebut
	        and PR       <= @vl_PRfin 
		and e.numero =p.numero
		and p.type_PAU != 1
	    union
            select G.numero, G.nom, 1, 172254,G.numero%2+2  from CFT..EQT_GEN G, CFT..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D35"
	    union
            select G.numero, G.nom, 1,172444,2 from CFT..EQT_GEN G, CFT..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D535"
		order by sens, PR
	    end
	    else
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFT..EQT_GEN e, CFT..EQT_PAU p
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = @vl_Autoroute
	        and PR       >  @vl_PRdebut
	        and PR       <= @vl_PRfin 
		and e.numero =p.numero
		and p.type_PAU != 1
	      order by sens, PR
	        
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des equipements du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in

            if ( @va_NomTroncon_in="BREGUIERES A8")
            begin
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFT..EQT_GEN e, CFT..EQT_PAU p
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = @vl_Autoroute
	        and PR       >  @vl_PRdebut
	        and PR       <= @vl_PRfin 
		and e.numero =p.numero
		and p.type_PAU != 1
	      
	    union
            select G.numero, G.nom, 1, 172254,G.numero%2+2  from CFT..EQT_GEN G, CFT..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D35"
	    union
            select G.numero, G.nom, 1,172444,2 from CFT..EQT_GEN G, CFT..RES_AUT A
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = A.numero
		and A.nom="D535"
		order by sens, PR
	    end
	    else
	    select e.numero, e.nom, e.autoroute, e.PR, e.sens from CFT..EQT_GEN e, CFT..EQT_PAU p
	      where type = @va_TypeEqt_in
		and eqt_supprime = XDC_FAUX
	 	and autoroute = @vl_Autoroute
	        and PR       >  @vl_PRdebut
	        and PR       <= @vl_PRfin  
		and e.numero =p.numero
		and p.type_PAU != 1
	      order by sens, PR
	 
	 end   
      end
#endif
      
   end
      
   return XDC_OK
         
go  
