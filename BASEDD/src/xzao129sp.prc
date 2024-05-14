/*E*/
/*Fichier : $Id: xzao129sp.prc,v 1.4 1994/12/05 16:03:23 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/12/05 16:03:23 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao129sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao129sp.h" */

/* static char *version = "@(#)xzao129.c	1.1 23/18/94 : xzao129" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des Peages d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;129
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in	smallint	
   as 

/* 
*
* SP	XZAO129SP
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
*  Liste des Peages d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des Peages de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des Peages du troncon
*
* Si NumeroPortion est value alors la liste des Peages de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens, nom du type, nom du type de picto 
* des Peages a partir des tables EQT_PEA, EQT_Peages, EQT_PCT, TYP_Peages, TYP_PCT
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* gaborit	28 nov 1994	: modif entete (v1.3)
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
   or ( @va_NomTroncon_in <> null
    and @va_NumeroPortion_in <> null )
    
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
         ** Recherche des Peages de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select distinct PEA.numero, 
            		    PEA.nom, 
            		    PEA.PRsens1, 
            		    PEA.longueur_sens1, 
            		    PEA.PRsens2, 
            		    PEA.PRsens2, 
            		    PEA.longueur_sens2,
            		    AUT.numero
            from CFG..RES_PEA PEA, CFG..RES_AUT AUT, CFG..RES_POR POR
	     
	      where POR.autoroute = AUT.numero
	        and PEA.portion   = POR.numero
	        
	      order by  AUT.numero, PEA.numero 
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des Peages de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select distinct PEA.numero, 
            		    PEA.nom, 
            		    PEA.PRsens1, 
            		    PEA.longueur_sens1, 
            		    PEA.PRsens2, 
            		    PEA.PRsens2, 
            		    PEA.longueur_sens2,
            		    POR.autoroute
            from CFG..RES_PEA PEA, CFG..RES_POR POR
	     
	      where POR.numero  = @va_NumeroPortion_in
	        and PEA.portion = POR.numero
	        
	      order by PEA.numero 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des Peages du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, 
                   @vl_PRdebut = PR_debut, 
                   @vl_PRfin = PR_fin 
                   
            from CFG..RES_TRC
              
              where nom = @va_NomTroncon_in
            
            
            
            select distinct PEA.numero, 
                            PEA.nom, 
            		    PEA.PRsens1, 
            		    PEA.longueur_sens1, 
            		    PEA.PRsens2, 
            		    PEA.longueur_sens2,
            		    @vl_Autoroute
            		    
            from CFG..RES_POR POR, CFG..RES_PEA PEA
	     
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut <= @vl_PRfin
	        and POR.PR_fin   >= @vl_PRdebut
	        and (( PEA.PRsens1 >=  @vl_PRdebut and PEA.PRsens1<= @vl_PRfin )
	        or   ( PEA.PRsens2 >=  @vl_PRdebut and PEA.PRsens2<= @vl_PRfin ))
	        and PEA.portion   = POR.numero
	        
	      order by PEA.numero 
	        
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
         ** Recherche des Peages de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select distinct PEA.numero, 
            		    PEA.nom, 
            		    PEA.PRsens1, 
            		    PEA.longueur_sens1, 
            		    PEA.PRsens2, 
            		    PEA.PRsens2, 
            		    PEA.longueur_sens2,
            		    AUT.numero
            from CFT..RES_PEA PEA, CFT..RES_AUT AUT, CFT..RES_POR POR
	     
	      where POR.autoroute = AUT.numero
	        and PEA.portion   = POR.numero
	        
	      order by  AUT.numero, PEA.numero 
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des Peages de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select distinct PEA.numero, 
            		    PEA.nom, 
            		    PEA.PRsens1, 
            		    PEA.longueur_sens1, 
            		    PEA.PRsens2, 
            		    PEA.PRsens2, 
            		    PEA.longueur_sens2,
            		    POR.autoroute
            from CFT..RES_PEA PEA, CFT..RES_POR POR
	     
	      where POR.numero  = @va_NumeroPortion_in
	        and PEA.portion = POR.numero
	        
	      order by PEA.numero 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des Peages du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, 
                   @vl_PRdebut = PR_debut, 
                   @vl_PRfin = PR_fin 
                   
            from CFT..RES_TRC
              
              where nom = @va_NomTroncon_in
            
            
            
            select distinct PEA.numero, 
                            PEA.nom, 
            		    PEA.PRsens1, 
            		    PEA.longueur_sens1, 
            		    PEA.PRsens2, 
            		    PEA.longueur_sens2,
            		    @vl_Autoroute
            		    
            from CFT..RES_POR POR, CFT..RES_PEA PEA
	     
	      where POR.autoroute = @vl_Autoroute
	        and POR.PR_debut <= @vl_PRfin
	        and POR.PR_fin   >= @vl_PRdebut
	        and (( PEA.PRsens1 >=  @vl_PRdebut and PEA.PRsens1<= @vl_PRfin )
	        or   ( PEA.PRsens2 >=  @vl_PRdebut and PEA.PRsens2<= @vl_PRfin ))
	        and PEA.portion   = POR.numero
	        
	      order by PEA.numero 
	        
	 end   
      end

#endif

   end
      
   return XDC_OK
         
go  
