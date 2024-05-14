/*E*/
/* Fichier : $Id: xzao124sp.prc,v 1.5 2018/10/23 15:50:33 devgfi Exp $        $Revision: 1.5 $        $Date: 2018/10/23 15:50:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao124sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	23 Nov 1994	: Modif param null en entree (v1.2)
* PNI		14 Nov 2012	: Prise en compte du PMVA hors DPAC sur A501 v1.4
* JPL		15/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.5
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao124sp.h" */

/* static char *version = "@(#)xzao124.c	1.1 23/18/94 : xzao124" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMV d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;124
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint,
   @va_Resultat_out	int = null output	
   as 

/* 
*
* SP	XZAO124SP
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
*  Liste des PMV d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des PMV de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des PMV du troncon
*
* Si NumeroPortion est value alors la liste des PMV de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens, nom du type, nom du type de picto 
* des PMV a partir des tables EQT_GEN, EQT_PMV, EQT_PCT, TYP_PMV, TYP_PCT
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
         ** Recherche des PMV de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute, 
                   TPMV.lignes,
                   TPCT.nom
                   
            from CFG..EQT_GEN GEN, 
                 CFG..EQT_PMV PMV,
                 CFG..TYP_PMV TPMV,
	         CFG..EQT_PCT PCT,
                 CFG..TYP_PCT TPCT
	      
	      where GEN.type = XDC_EQT_PMV
	        and GEN.eqt_supprime = XDC_FAUX
	        and PMV.numero = GEN.numero
	        and TPMV.numero = PMV.type
	        and PCT.numero = GEN.numero
	        and TPCT.numero = PCT.type
	  
	    
	 end                    
                
         /*A
         ** NomTroncon est nul 
         ** Recherche des PMV de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
	    
	    if (@vl_Autoroute = 4) 
		select  @vl_PRdebut =0
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute, 
                   TPMV.lignes, 
                   TPCT.nom 
            
            from CFG..EQT_GEN GEN, CFG..EQT_PMV PMV, CFG..EQT_PCT PCT, CFG..TYP_PMV TPMV, CFG..TYP_PCT TPCT
	      where GEN.type = XDC_EQT_PMV
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       <= @vl_PRfin
	        and GEN.PR       >= @vl_PRdebut 
	        and PMV.numero    = GEN.numero
	        and PCT.numero    = GEN.numero
	        and TPMV.numero   = PMV.type
	        and TPCT.numero   = PCT.type 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des PMV du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
	    if (@vl_Autoroute = 4) 
		select  @vl_PRdebut =0
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute, 
                   TPMV.lignes, 
                   TPCT.nom 
                   
            from CFG..EQT_GEN GEN, CFG..EQT_PMV PMV, CFG..EQT_PCT PCT, CFG..TYP_PMV TPMV, CFG..TYP_PCT TPCT
	      where GEN.type = XDC_EQT_PMV
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and PMV.numero = GEN.numero
	        and PCT.numero = GEN.numero
	        and TPMV.numero = PMV.type
	        and TPCT.numero = PCT.type
	        
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
         ** Recherche des PMV de l'ensemble des Autoroutes
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute, 
                   TPMV.lignes,
                   TPCT.nom
                   
            from CFT..EQT_GEN GEN, 
                 CFT..EQT_PMV PMV,
                 CFT..TYP_PMV TPMV,
	         CFT..EQT_PCT PCT,
                 CFT..TYP_PCT TPCT
	      
	      where GEN.type = XDC_EQT_PMV
	        and GEN.eqt_supprime = XDC_FAUX
	        and PMV.numero = GEN.numero
	        and TPMV.numero = PMV.type
	        and PCT.numero = GEN.numero
	        and TPCT.numero = PCT.type
	  
	    
	 end                    
                
         /*A
         ** NomTroncon est nul 
         ** Recherche des PMV de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute, 
                   TPMV.lignes, 
                   TPCT.nom 
            
            from CFT..EQT_GEN GEN, CFT..EQT_PMV PMV, CFT..EQT_PCT PCT, CFT..TYP_PMV TPMV, CFT..TYP_PCT TPCT
	      where GEN.type = XDC_EQT_PMV
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and PMV.numero    = GEN.numero
	        and PCT.numero    = GEN.numero
	        and TPMV.numero   = PMV.type
	        and TPCT.numero   = PCT.type 
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des PMV du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
            
            select GEN.nom, 
                   GEN.numero, 
                   GEN.PR, 
                   GEN.sens,
                   GEN.autoroute, 
                   TPMV.lignes, 
                   TPCT.nom 
                   
            from CFT..EQT_GEN GEN, CFT..EQT_PMV PMV, CFT..EQT_PCT PCT, CFT..TYP_PMV TPMV, CFT..TYP_PCT TPCT
	      where GEN.type = XDC_EQT_PMV
	        and GEN.eqt_supprime = XDC_FAUX
	        and GEN.autoroute = @vl_Autoroute
	        and GEN.PR       >  @vl_PRdebut
	        and GEN.PR       <= @vl_PRfin 
	        and PMV.numero = GEN.numero
	        and PCT.numero = GEN.numero
	        and TPMV.numero = PMV.type
	        and TPCT.numero = PCT.type
	        
	 if @va_NomTroncon_in != null and  @va_NumeroPortion_in != null
            select @va_Resultat_out = XDC_ECHEC
            
                   
	 
	 end   
      end

#endif
      
   end
      
   return XDC_OK
         
go  
