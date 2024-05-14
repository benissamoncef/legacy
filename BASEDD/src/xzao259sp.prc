/*E*/
/*Fichier : $Id: xzao259sp.prc,v 1.1 2018/04/19 14:06:42 pc2dpdy Exp $  Release : $Revision: 1.1 $ Date : $Date: 2018/04/19 14:06:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao259sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
*
* Historique
* LCL	16/03/18	: Creation CTRL_FEUX 1.1 DEM1284
*
*
*------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao259sp.h" */

/* static char *version = "$Id: xzao259sp.prc,v 1.1 2018/04/19 14:06:42 pc2dpdy Exp $ $Revision: 1.1 $ $Date: 2018/04/19 14:06:42 $ : xzao259" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des CFE d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO259' and type = 'P')
	drop procedure XZAO259
go

create procedure XZAO259
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint,
   @va_Resultat_out	int = null output	
   as 

/* 
*
* SP	XZAO259SP
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
*  Liste des CFE d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des CFE de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des CFE du troncon
*
* Si NumeroPortion est value alors la liste des CFE de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens, nom du type, nom du type de picto 
* des CFE a partir des tables EQT_GEN, EQT_CFE, EQT_PCT, TYP_CFE, TYP_PCT
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18	creation CTRL_FEUX 1.1
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
         ** Recherche des CFE de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
                   
            from CFG..EQT_GEN GEN, 
                 CFG..EQT_CFE CFE
	      
	      where GEN.type = XDC_EQT_CFE
	        and CFE.numero = GEN.numero
	  
	    
	 end                    
                
         /*A
         ** NomTroncon est nul 
         ** Recherche des CFE de la Portion
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
            
            from CFG..EQT_GEN GEN, CFG..EQT_CFE CFE
	      where GEN.type = XDC_EQT_CFE
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       <= @vl_PRfin
	        and GEN.PR       >= @vl_PRdebut 
	        and CFE.numero    = GEN.numero
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des CFE du Troncon
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
                   
            from CFG..EQT_GEN GEN, CFG..EQT_CFE CFE
	      where GEN.type = XDC_EQT_CFE
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and CFE.numero = GEN.numero
	        
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
         ** Recherche des CFE de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute
                   
            from CFT..EQT_GEN GEN, 
                 CFT..EQT_CFE CFE
	      
	      where GEN.type = XDC_EQT_CFE
	        and CFE.numero = GEN.numero
	  
	    
	 end                    
                
         /*A
         ** NomTroncon est nul 
         ** Recherche des CFE de la Portion
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
            
            from CFT..EQT_GEN GEN, CFT..EQT_CFE CFE
	      where GEN.type = XDC_EQT_CFE
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and CFE.numero    = GEN.numero 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des CFE du Troncon
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
                   
            from CFT..EQT_GEN GEN, CFT..EQT_CFE CFE
	      where GEN.type = XDC_EQT_CFE
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and CFE.numero = GEN.numero
	        
	 if @va_NomTroncon_in != null and  @va_NumeroPortion_in != null
            select @va_Resultat_out = XDC_ECHEC
            
                   
	 
	 end   
      end

#endif
      
   end
      
   return XDC_OK
         
go  
