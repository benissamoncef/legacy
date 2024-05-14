/*E*/
/* Fichier : $Id: xzao409sp.prc,v 1.9 2018/10/23 15:28:15 devgfi Exp $        $Revision: 1.9 $        $Date: 2018/10/23 15:28:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao409sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	10 Fev 1997	: Creation (DEM/1395)
* niepceron	05 Mar 1997	: Prise en cpte du PR fin (DEM/1395)  v1.2
* mismer	05 Jun 1997	: Ajout filtre zone statique (DEM/1441) v1.3
* niepceron	11 Fev 1999	: Animation zdp (dem/1730) v1.4
* claudel	08 Fev 2012	: TdP etendus (DEM/1014) v1.5
* JMG		09/10/2012 : renommage seuil 1.6, 1.7
* JPL		10/10/17 : Remplacement du champ 'code' par 'code_zone' (pour DEM 1236)  1.8
* JPL	15/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.9
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao409sp.h" */

/* static char *version = "" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des ZDP d'un troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO409' and type = 'P')
	drop procedure XZAO409
go


create procedure XZAO409
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint,
   @va_Resultat_out	int = null output	
   as 

/* 
*
* SP	XZAO409SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomTroncon_in
* XDY_Mot       va_NumeroPortion_in
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
* Retourne la liste des ZDP d'un troncon
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
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
    
   select @va_Resultat_out = XDC_LU

   if rtrim ( @va_Basedd_in ) = null            select @va_Basedd_in = null
   if rtrim ( @va_NomTroncon_in ) = null        select @va_NomTroncon_in = null
   if @va_NumeroPortion_in = 0                  select @va_NumeroPortion_in = null


   if @va_Basedd_in = null

      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls  
   */
      
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
         	select distinct GEN.nom,
         	        GEN.numero, 
         		GEN.autoroute, 
         		GEN.PR, 
         		GEN.sens, 
         		ZDP.PR_fin,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100) seuil_bas,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100) seuil_haut,
			TDP_max
         		         	
         	from CFG..EQT_ZDP ZDP, CFG..EQT_GEN GEN
         
          	where GEN.type   		= XDC_EQT_ZDP
          	     and GEN.eqt_supprime	= XDC_FAUX
          	     and GEN.numero		= ZDP.numero
          	     and ZDP.code_zone		!= 0
          	     and ZDP.inhibition		= 0
		     and ZDP.type_zone		= 1
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero

         end

         /*A
         ** NomTroncon est nul
         ** Recherche des ZDP de la Portion
         */

         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin

            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in

         	select distinct GEN.nom,
         	        GEN.numero, 
         		GEN.autoroute, 
         		GEN.PR, 
         		GEN.sens, 
         		ZDP.PR_fin,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100) seuil_bas,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100) seuil_haut,
			TDP_max

         		         	
         	from CFG..EQT_ZDP ZDP, CFG..EQT_GEN GEN
         
          	where GEN.type   		= XDC_EQT_ZDP
          	     and GEN.eqt_supprime	= XDC_FAUX
          	     and GEN.numero		= ZDP.numero
                     and GEN.autoroute 		= @vl_Autoroute
                     and (  (GEN.PR      	<= @vl_PRfin and
                             GEN.PR    		>= @vl_PRdebut) 
			 or (ZDP.PR_fin         <= @vl_PRfin and
		             ZDP.PR_fin         <= @vl_PRfin))
          	     and ZDP.code_zone		!= 0
          	     and ZDP.inhibition		= 0
		     and ZDP.type_zone		= 1
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero


         end

         /*A
         ** NumeroPortion est nul
         ** Recherche des ZDP du Troncon
         */

         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin

            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in

         	select distinct GEN.nom,
         	        GEN.numero, 
         		GEN.autoroute, 
         		GEN.PR, 
         		GEN.sens, 
         		ZDP.PR_fin,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100) seuil_bas,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100) seuil_haut,
			TDP_max
         		         	
         	from CFG..EQT_ZDP ZDP, CFG..EQT_GEN GEN
         
          	where GEN.type   		= XDC_EQT_ZDP
                     and GEN.eqt_supprime	= XDC_FAUX
                     and GEN.autoroute 		= @vl_Autoroute
          	     and GEN.numero		= ZDP.numero
                     and (  (GEN.PR      	<= @vl_PRfin and
                             GEN.PR    		>= @vl_PRdebut) 
			 or (ZDP.PR_fin         <= @vl_PRfin and
		             ZDP.PR_fin         <= @vl_PRfin))
          	     and ZDP.code_zone		!= 0
          	     and ZDP.inhibition		= 0
		     and ZDP.type_zone		= 1
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero

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
         	select distinct GEN.nom,
         	        GEN.numero, 
         		GEN.autoroute, 
         		GEN.PR, 
         		GEN.sens, 
         		ZDP.PR_fin,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100) seuil_bas,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100) seuil_haut,
			TDP_max
         		         	
         	from CFT..EQT_ZDP ZDP, CFT..EQT_GEN GEN
         
          	where GEN.type   		= XDC_EQT_ZDP
          	     and GEN.eqt_supprime	= XDC_FAUX
          	     and GEN.numero		= ZDP.numero
          	     and ZDP.code_zone		!= 0
          	     and ZDP.inhibition		= 0
		     and ZDP.type_zone		= 1
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero

         end

         /*A
         ** NomTroncon est nul
         ** Recherche des ZDP de la Portion
         */

         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin

            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
              where numero = @va_NumeroPortion_in

         	select distinct GEN.nom,
         	        GEN.numero, 
         		GEN.autoroute, 
         		GEN.PR, 
         		GEN.sens, 
         		ZDP.PR_fin,
         		ZDP.TDP_habituel,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100) seuil_bas,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100) seuil_haut,
			TDP_max
         		         	
         	from CFT..EQT_ZDP ZDP, CFT..EQT_GEN GEN
         
          	where GEN.type   		= XDC_EQT_ZDP
          	     and GEN.eqt_supprime	= XDC_FAUX
          	     and GEN.numero		= ZDP.numero
                     and GEN.autoroute 		= @vl_Autoroute
                     and (  (GEN.PR      	<= @vl_PRfin and
                             GEN.PR    		>= @vl_PRdebut) 
			 or (ZDP.PR_fin         <= @vl_PRfin and
		             ZDP.PR_fin         <= @vl_PRfin))
          	     and ZDP.code_zone		!= 0
          	     and ZDP.inhibition		= 0
		     and ZDP.type_zone		= 1
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero


         end

         /*A
         ** NumeroPortion est nul
         ** Recherche des ZDP du Troncon
         */

         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin

            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in

         	select distinct GEN.nom,
         	        GEN.numero, 
         		GEN.autoroute, 
         		GEN.PR, 
         		GEN.sens, 
         		ZDP.PR_fin,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100) seuil_bas,
			convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100) seuil_haut,
			TDP_max
         		         	
         	from CFT..EQT_ZDP ZDP, CFT..EQT_GEN GEN
         
          	where GEN.type   		= XDC_EQT_ZDP
          	     and GEN.eqt_supprime	= XDC_FAUX
          	     and GEN.numero		= ZDP.numero
                     and GEN.autoroute 		= @vl_Autoroute
                     and (  (GEN.PR      	<= @vl_PRfin and
                             GEN.PR    		>= @vl_PRdebut) 
			 or (ZDP.PR_fin         <= @vl_PRfin and
		             ZDP.PR_fin         <= @vl_PRfin))
          	     and ZDP.code_zone		!= 0
          	     and ZDP.inhibition		= 0
		     and ZDP.type_zone		= 1
                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero

         end
         
      end
      

#endif      
      
   end
      
   return XDC_OK
         
go  
