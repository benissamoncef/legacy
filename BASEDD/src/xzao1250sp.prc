/*E*/
/* Fichier : $Id: xzao1250sp.prc,v 1.2 2018/10/23 14:33:30 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/10/23 14:33:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1250sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	 	29 Novembre 2011	: Creation
* JPL	15/10/18 : Filtrage des equipements marques comme supprimes  1.2
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao1250sp.h" */

/* static char *version = "@(#)xzao1250.c	1.1 29/11/11 : xzao1250" ; */

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

if exists (select 1 from sysobjects where name = 'XZAO1250' and type = 'P')
	drop procedure XZAO1250
go


create procedure XZAO1250
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_NumeroPortion_in smallint
   as 

/* 
*
* SP	XZAO1250SP
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
* Ecriture specifique pour les cameras / SAE V2 a partir de xzao125sp.prc 
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
   
   if @va_Basedd_in = null
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
            
            select EQTGEN.numero, EQTGEN.nom, EQTGEN.autoroute, EQTGEN.PR, EQTGEN.sens, EQTCAM.IP, EQTCAM.protocole, EQTCAM.port, EQTCAM.encoder  
	    from CFG..EQT_GEN EQTGEN, CFG..EQT_CAM EQTCAM
	      where EQTGEN.type = XDC_EQT_CAM and
		    EQTGEN.eqt_supprime = XDC_FAUX and
		    EQTGEN.numero = EQTCAM.numero
	      order by sens, PR
	 
	 end                        
         
         /*A
         ** NomTroncon est nul 
         ** Recherche des equipements de la Portion
         */
         
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
              where numero = @va_NumeroPortion_in
            
            
            select EQTGEN.numero, EQTGEN.nom, EQTGEN.autoroute, EQTGEN.PR, EQTGEN.sens, EQTCAM.IP, EQTCAM.protocole, EQTCAM.port, EQTCAM.encoder
	    from CFG..EQT_GEN EQTGEN, CFG..EQT_CAM EQTCAM
	    where EQTGEN.type = XDC_EQT_CAM and
	    EQTGEN.eqt_supprime = XDC_FAUX and
	    EQTGEN.numero = EQTCAM.numero and
	    EQTGEN.autoroute = @vl_Autoroute and
	    EQTGEN.PR > @vl_PRdebut and
	    EQTGEN.PR <= @vl_PRfin
	    order by sens, PR

	select numero, nom, autoroute, PR, sens from CFG..EQT_GEN
	      where type = XDC_EQT_CAM
	 	and eqt_supprime = XDC_FAUX
	 	and autoroute = @vl_Autoroute
	        and PR       >  @vl_PRdebut
	        and PR       <= @vl_PRfin 
	      order by sens, PR
	        
	 
	 end   
      
         /*A
         ** NumeroPortion est nul 
         ** Recherche des equipements du Troncon
         */
         
         if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         begin
            
            select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
            
            select EQTGEN.numero, EQTGEN.nom, EQTGEN.autoroute, EQTGEN.PR, EQTGEN.sens, EQTCAM.IP, EQTCAM.protocole, EQTCAM.port, EQTCAM.encoder
            from CFG..EQT_GEN EQTGEN, CFG..EQT_CAM EQTCAM
	    where EQTGEN.type = XDC_EQT_CAM and
	    EQTGEN.eqt_supprime = XDC_FAUX and
	    EQTGEN.numero = EQTCAM.numero and
 	    EQTGEN.autoroute = @vl_Autoroute and
            EQTGEN.PR > @vl_PRdebut and
	    EQTGEN.PR <= @vl_PRfin
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
            
            select EQTGEN.numero, EQTGEN.nom, EQTGEN.autoroute, EQTGEN.PR, EQTGEN.sens, EQTCAM.IP, EQTCAM.protocole, EQTCAM.port, EQTCAM.encoder
	    from CFT..EQT_GEN EQTGEN, CFT..EQT_CAM EQTCAM
            where EQTGEN.type = XDC_EQT_CAM and
	    EQTGEN.eqt_supprime = XDC_FAUX and
	    EQTGEN.numero = EQTCAM.numero
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
            
            
            select EQTGEN.numero, EQTGEN.nom, EQTGEN.autoroute, EQTGEN.PR, EQTGEN.sens, EQTCAM.IP, EQTCAM.protocole, EQTCAM.port, EQTCAM.encoder
	    from CFT..EQT_GEN EQTGEN, CFT..EQT_CAM EQTCAM
            where EQTGEN.type = XDC_EQT_CAM and
	    EQTGEN.numero = EQTCAM.numero and
            EQTGEN.eqt_supprime = XDC_FAUX and
            EQTGEN.autoroute = @vl_Autoroute and
   	    EQTGEN.PR > @vl_PRdebut and
	    EQTGEN.PR <= @vl_PRfin
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
            
            select EQTGEN.numero, EQTGEN.nom, EQTGEN.autoroute, EQTGEN.PR, EQTGEN.sens, EQTCAM.IP, EQTCAM.protocole, EQTCAM.port, EQTCAM.encoder
	    from CFT..EQT_GEN EQTGEN, CFT..EQT_CAM EQTCAM
	    where EQTGEN.type = XDC_EQT_CAM and
	    EQTGEN.eqt_supprime = XDC_FAUX and
	    EQTGEN.numero = EQTCAM.numero and
	    EQTGEN.autoroute = @vl_Autoroute and
	    EQTGEN.PR > @vl_PRdebut and
	    EQTGEN.PR <= @vl_PRfin
	    order by sens, PR

	 end   
      end
#endif
      
   end
      
   return XDC_OK
         
go  
