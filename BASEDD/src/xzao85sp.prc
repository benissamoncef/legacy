/*E*/
/* Fichier : $Id: xzao85sp.prc,v 1.2 2018/10/23 14:49:35 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/10/23 14:49:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao85sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* XXX	xx/xx/94 : Creation  1.1
* JPL	15/10/18 : Filtrage des equipements marques comme supprimes  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao85sp.h" */

/* static char *version = "@(#)xzao85.c	1.1 23/18/94 : xzao85" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des equipements SAGA d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO85' and type = 'P')
	drop procedure XZAO85
go


create procedure XZAO85
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint,
   @va_TypeEqt_in	tinyint	
   as 

/* 
*
* SP	XZAO85SP
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
            
            select gen.numero, gen.nom, gen.autoroute, gen.PR, gen.sens 
		from CFG..EQT_GEN gen, CFG..EQT_SAGA saga
	      where gen.type = XDC_EQT_SAGA
	      and gen.eqt_supprime = XDC_FAUX
	      and gen.numero = saga.numero
	      and saga.type_saga = @va_TypeEqt_in
	      order by gen.sens, gen.PR
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des equipements de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select gen.numero, gen.nom, gen.autoroute, gen.PR, gen.sens 
		from CFG..EQT_GEN gen, CFG..EQT_SAGA saga
	      where gen.type = XDC_EQT_SAGA
		and gen.eqt_supprime = XDC_FAUX
		 and gen.numero = saga.numero
		 and saga.type_saga = @va_TypeEqt_in
	 	and gen.autoroute = @vl_Autoroute
	        and gen.PR       >  @vl_PRdebut
	        and gen.PR       <= @vl_PRfin 
	      order by gen.sens, gen.PR
	        
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des equipements du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
             select gen.numero, gen.nom, gen.autoroute, gen.PR, gen.sens 
	     from CFG..EQT_GEN gen, CFG..EQT_SAGA saga
	      where gen.type      = XDC_EQT_SAGA
	       and gen.eqt_supprime = XDC_FAUX
	       and gen.numero = saga.numero
		and saga.type_saga = @va_TypeEqt_in
	 	and gen.autoroute = @vl_Autoroute
	        and gen.PR       >  @vl_PRdebut
	        and gen.PR       <= @vl_PRfin
	      order by gen.sens, gen.PR
	 
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
            
            select gen.numero, gen.nom, gen.autoroute, gen.PR, gen.sens 
		from CFT..EQT_GEN gen, CFT..EQT_SAGA saga
	      where gen.type = XDC_EQT_SAGA
	     and gen.eqt_supprime = XDC_FAUX
	     and gen.numero = saga.numero
	     and saga.type_saga = @va_TypeEqt_in
	      order by gen.sens, gen.PR
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des equipements de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select gen.numero, gen.nom, gen.autoroute, gen.PR, gen.sens 
		from CFT..EQT_GEN gen, CFT..EQT_SAGA saga
	      where gen.type = XDC_EQT_SAGA
		   and gen.numero = saga.numero
		and gen.eqt_supprime = XDC_FAUX
		and saga.type_saga =@va_TypeEqt_in
	 	and gen.autoroute = @vl_Autoroute
	        and gen.PR       >  @vl_PRdebut
	        and gen.PR       <= @vl_PRfin 
	      order by gen.sens, gen.PR
	        
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des equipements du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
            
            select gen.numero, gen.nom, gen.autoroute, gen.PR, gen.sens 
		from CFT..EQT_GEN gen, CFT..EQT_SAGA saga
	      where type      = XDC_EQT_SAGA
		 and gen.eqt_supprime = XDC_FAUX
		 and gen.numero = saga.numero
		 and saga.type_saga =@va_TypeEqt_in
	 	and gen.autoroute = @vl_Autoroute
	        and gen.PR       >  @vl_PRdebut
	        and gen.PR       <= @vl_PRfin  
	      order by gen.sens, gen.PR
	 
	 end   
      end
#endif
      
   end
      
   return XDC_OK
         
go  
