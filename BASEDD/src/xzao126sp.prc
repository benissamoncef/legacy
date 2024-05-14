/*E*/
/*Fichier : $Id: xzao126sp.prc,v 1.2 1995/01/09 13:16:07 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/09 13:16:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao126sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao126sp.h" */

/* static char *version = "@(#)xzao126.c	1.1 23/18/94 : xzao126" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des portions d'un troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;126
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25)
   as 

/* 
*
* SP	XZAO126SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des portions d'un troncon
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
* Retourne numero segment, numero autoroute, coef_trans debut, coef_trans fin, 
* sens, nombre de voies, largeur BAU, tunnel
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Autoroute	tinyint,
   @vl_AutoTrc		tinyint,
   @vl_NumPortion	smallint,
   @vl_Por1		smallint,
   @vl_Por2		smallint,
   @vl_Ext1		bit,
   @vl_Ext2		bit,
   @vl_PRdebut		int,
   @vl_PRfin		int,
   @vl_Portion		smallint,
   @vl_PRd		int,
   @vl_PRf		int,
   @vl_PRn1		int,
   @vl_PRn2		int,
   @vl_Distr		tinyint,
   @vl_Bifurcation	bit,
   @vl_PRBif		int
   
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV   
   */
      
   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_NomTroncon_in ) = null 	select @va_NomTroncon_in = null
   
   
   if @va_Basedd_in = null
   or @va_NomTroncon_in = null
    
      return XDC_ARG_INV
   
   else   
   
   begin
      
      /*A
      ** Creation d'une table temporaire TMP_POR
      */
         
      create table #TMP_POR
      ( numero		smallint not null,
        autoroute	tinyint not null,
        PRdebut		int not null,
        PRfin		int not null,
        DistrictR	bit,
        Bif		bit,
        PRBif		int null )
           
      /*A
      ** Creation d'une table temporaire TMP_NEU
      */
         
      create table #TMP_NEU
      ( p1 smallint not null,
        p2 smallint not null )
           
         
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
	 ** Recherche du numero de l'autoroute sur lequel se trouve la portion
	 */
	 
	 select @vl_AutoTrc = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
           where nom = @va_NomTroncon_in
         
         /*A 
	 ** Recherche des noeuds appartenant au troncon
	 */
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFG..RES_NEU NEU, CFG..RES_POR POR
	                      where NEU.portion1 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_1 = 0
	                        and POR.PR_debut >= @vl_PRdebut
	 			and POR.PR_debut <= @vl_PRfin
	                       
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFG..RES_NEU NEU, CFG..RES_POR POR
	                      where NEU.portion1 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_1 = 1
	                        and POR.PR_fin >= @vl_PRdebut
	 			and POR.PR_fin <= @vl_PRfin
	                       
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFG..RES_NEU NEU, CFG..RES_POR POR
	                      where NEU.portion2 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_2 = 0
	                        and POR.PR_debut >= @vl_PRdebut
	 			and POR.PR_debut <= @vl_PRfin
	                       
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFG..RES_NEU NEU, CFG..RES_POR POR
	                      where NEU.portion2 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_2 = 1
	                        and POR.PR_fin >= @vl_PRdebut
	 			and POR.PR_fin <= @vl_PRfin
	                       
	 
	 declare Noeud cursor
	 for select distinct 	POR.numero,
	 			POR.autoroute,
	 			POR.PR_debut,
	 			POR.PR_fin
              
              from #TMP_NEU NEU, CFG..RES_POR POR
              
              where NEU.p1 = POR.numero
                 or NEU.p2 = POR.numero	 			
	 			
	 		
	  open Noeud
	  
	  fetch Noeud into @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf
	  
	  while ( @@sqlstatus = 0 )
	  begin
	  
	     if @vl_Autoroute != @vl_AutoTrc
	     begin
	        
	        select @vl_Por1 = portion1,
	               @vl_Ext1 = extremite_portion_1
		
		from CFG..RES_NEU
		
		where portion2 = @vl_NumPortion
		
		if @vl_Por1 = null
		   
		   select @vl_Por1 = portion2,
	                  @vl_Ext1 = extremite_portion_2
		
		   from CFG..RES_NEU
		
		   where portion1 = @vl_NumPortion
		   
		
		if @vl_Ext1 = XDC_VRAI  -- Fin de portion
		   
		   select @vl_PRBif = PR_fin 
		   
		   from CFG..RES_POR
		   
		   where numero = @vl_Por1
		   
		else  -- Debut de portion
		
		   select @vl_PRBif = PR_debut 
		   
		   from CFG..RES_POR
		   
		   where numero = @vl_Por1
		    	               
	        
	        insert into #TMP_POR
	        values ( @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf, XDC_FAUX, XDC_VRAI, @vl_PRBif )
	        
	     end
	     
	     else
	     
	     /*A
	     ** L'autoroute est celui du troncon
	     */
	     
	        insert into #TMP_POR
	        values ( @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf, XDC_FAUX, XDC_FAUX, null )
	        
	     
	     
	     fetch Noeud into @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf
	  
	  end
	  
	  select * from #TMP_POR
	    order by Bif, PRdebut
	    
	  if @@rowcount = 0
	  
	     select numero, autoroute, PR_debut, PR_fin, XDC_FAUX, XDC_FAUX, null
	     from CFG..RES_POR
	     where autoroute = @vl_AutoTrc
	       and PR_debut <= @vl_PRfin
	       and PR_fin >= @vl_PRdebut  
	  
      end
   
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
	 ** Recherche du numero de l'autoroute sur lequel se trouve la portion
	 */
	 
	 select @vl_AutoTrc = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
           where nom = @va_NomTroncon_in
         
         /*A 
	 ** Recherche des noeuds appartenant au troncon
	 */
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFT..RES_NEU NEU, CFT..RES_POR POR
	                      where NEU.portion1 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_1 = 0
	                        and POR.PR_debut >= @vl_PRdebut
	 			and POR.PR_debut <= @vl_PRfin
	                       
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFT..RES_NEU NEU, CFT..RES_POR POR
	                      where NEU.portion1 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_1 = 1
	                        and POR.PR_fin >= @vl_PRdebut
	 			and POR.PR_fin <= @vl_PRfin
	                       
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFT..RES_NEU NEU, CFT..RES_POR POR
	                      where NEU.portion2 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_2 = 0
	                        and POR.PR_debut >= @vl_PRdebut
	 			and POR.PR_debut <= @vl_PRfin
	                       
	 
	 insert into #TMP_NEU select portion1, portion2
	                      from CFT..RES_NEU NEU, CFT..RES_POR POR
	                      where NEU.portion2 = POR.numero
	                        and POR.autoroute = @vl_AutoTrc
	                        and NEU.extremite_portion_2 = 1
	                        and POR.PR_fin >= @vl_PRdebut
	 			and POR.PR_fin <= @vl_PRfin
	                       
	 
	 declare Noeud cursor
	 for select distinct 	POR.numero,
	 			POR.autoroute,
	 			POR.PR_debut,
	 			POR.PR_fin
              
              from #TMP_NEU NEU, CFT..RES_POR POR
              
              where NEU.p1 = POR.numero
                 or NEU.p2 = POR.numero	 			
	 			
	 		
	  open Noeud
	  
	  fetch Noeud into @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf
	  
	  while ( @@sqlstatus = 0 )
	  begin
	  
	     if @vl_Autoroute != @vl_AutoTrc
	     begin
	        
	        select @vl_Por1 = portion1,
	               @vl_Ext1 = extremite_portion_1
		
		from CFT..RES_NEU
		
		where portion2 = @vl_NumPortion
		
		if @vl_Por1 = null
		   
		   select @vl_Por1 = portion2,
	                  @vl_Ext1 = extremite_portion_2
		
		   from CFT..RES_NEU
		
		   where portion1 = @vl_NumPortion
		   
		
		if @vl_Ext1 = XDC_VRAI  -- Fin de portion
		   
		   select @vl_PRBif = PR_fin 
		   
		   from CFT..RES_POR
		   
		   where numero = @vl_Por1
		   
		else  -- Debut de portion
		
		   select @vl_PRBif = PR_debut 
		   
		   from CFT..RES_POR
		   
		   where numero = @vl_Por1
		    	               
	        
	        insert into #TMP_POR
	        values ( @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf, XDC_FAUX, XDC_VRAI, @vl_PRBif )
	        
	     end
	     
	     else
	     
	     /*A
	     ** L'autoroute est celui du troncon
	     */
	     
	        insert into #TMP_POR
	        values ( @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf, XDC_FAUX, XDC_FAUX, null )
	        
	     
	     
	     fetch Noeud into @vl_NumPortion, @vl_Autoroute, @vl_PRd, @vl_PRf
	  
	  end
	  
	  select * from #TMP_POR
	    order by Bif, PRdebut
	  
	  
	  if @@rowcount = 0
	  
	     select numero, autoroute, PR_debut, PR_fin, XDC_FAUX, XDC_FAUX, null
	     from CFT..RES_POR
	     where autoroute = @vl_AutoTrc
	       and PR_debut <= @vl_PRfin
	       and PR_fin >= @vl_PRdebut
	  
      end

#endif   
   
   end
      
   return XDC_OK
         
go  
 
	 
