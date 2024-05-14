/*E*/
/* Fichier : $Id: xzao250sp.prc,v 1.2 2018/10/23 13:29:25 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/10/23 13:29:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao250sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* XXX	xx/03/07	creation
* JPL	15/10/18 : Filtrage des equipements marques comme supprimes  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao250sp.h" */

/* static char *version = "$Id: xzao250sp.prc,v 1.2 2018/10/23 13:29:25 devgfi Exp $ $Revision: 1.2 $ $Date: 2018/10/23 13:29:25 $" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMVA d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO250' and type = 'P')
	drop procedure XZAO250
go


create procedure XZAO250
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint,
   @va_Resultat_out	int = null output	
   as 

/* 
*
* SP	XZAO250SP
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
*  Liste des PMVA d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des PMVA de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des PMVA du troncon
*
* Si NumeroPortion est value alors la liste des PMVA de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens, nom du type, nom du type de picto 
* des PMVA a partir des tables EQT_GEN, EQT_PMVA, EQT_PCT, TYP_PMVA, TYP_PCT
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
      
   select @va_Resultat_out = XDC_LU
   
   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_NomTroncon_in ) = null 	select @va_NomTroncon_in = null
   if @va_NumeroPortion_in = 0		 	select @va_NumeroPortion_in = null
   
   
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
         ** Recherche des PMVA de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
                   
            from CFG..EQT_GEN GEN, 
                 CFG..EQT_PMVA PMVA
	      
	      where GEN.type = XDC_EQT_PMVA
	        and GEN.eqt_supprime = XDC_FAUX
	        and PMVA.numero = GEN.numero
	  
	    
	 end                    
                
         /*A
         ** NomTroncon est nul 
         ** Recherche des PMVA de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
            
            from CFG..EQT_GEN GEN, CFG..EQT_PMVA PMVA
	      where GEN.type = XDC_EQT_PMVA
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       <= @vl_PRfin
	        and GEN.PR       >= @vl_PRdebut 
	        and PMVA.numero    = GEN.numero
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des PMVA du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute 
                   
            from CFG..EQT_GEN GEN, CFG..EQT_PMVA PMVA
	      where GEN.type = XDC_EQT_PMVA
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and PMVA.numero = GEN.numero
	        
	 if @va_NomTroncon_in != null and  @va_NumeroPortion_in != null
            select @va_Resultat_out = XDC_ECHEC
            
                   
	 
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
         ** Recherche des PMVA de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
                   
            from CFT..EQT_GEN GEN, 
                 CFT..EQT_PMVA PMVA
	      
	      where GEN.type = XDC_EQT_PMVA
	        and GEN.eqt_supprime = XDC_FAUX
	        and PMVA.numero = GEN.numero
	  
	    
	 end                    
                
         /*A
         ** NomTroncon est nul 
         ** Recherche des PMVA de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
            
            from CFT..EQT_GEN GEN, CFT..EQT_PMVA PMVA
	      where GEN.type = XDC_EQT_PMVA
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and PMVA.numero    = GEN.numero 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des PMVA du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
                   
            from CFT..EQT_GEN GEN, CFT..EQT_PMVA PMVA
	      where GEN.type = XDC_EQT_PMVA
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and PMVA.numero = GEN.numero
	        
	 if @va_NomTroncon_in != null and  @va_NumeroPortion_in != null
            select @va_Resultat_out = XDC_ECHEC
            
                   
	 
	 end   
      end

#endif
      
   end
      
   return XDC_OK
         
go  
