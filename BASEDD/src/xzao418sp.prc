/*E*/
/* Fichier : $Id: xzao418sp.prc,v 1.3 2020/05/19 14:29:49 pc2dpdy Exp $        $Revision: 1.3 $        $Date: 2020/05/19 14:29:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao418sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer	21 Jan 1998	: Creation (DEM/1539)
* JPL	15/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.2
* LCL	11/06/19 : PAL/IP DEM1337	1.3
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao418sp.h" */

/* static char *version = "@(#)xzao418.c	1.1 23/18/94 : xzao418" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* Service rendu : 
* Retourne la liste des panneaux d'alerte sur un troncon ou une portion ou sur
* toutes les autoroutes
*
* Sequence d'appel :
* SP	XZAO418_Liste_Panneaux_Alerte
*
* Arguments en sortie
*
* ------------------------------------------------------
*/

use PRC
go

create procedure XZAO;418
   @va_Basedd_in        char(3),
   @va_NomTroncon_in    char(25),
   @va_NumeroPortion_in smallint,
   @va_Resultat_out     int = null output
   as 

/* 
*
* SP	XZAO418SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Nom       va_NomTroncon_in
* XDY_Mot       va_NumeroPortion_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier    va_Resultat_out
*
*
* VALEUR RENDUE :
*
* Retourne la liste des panneaux d'alerte d'un Troncon ou d'une portion ou de l'ensemble des autoroutes
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Si NomTroncon et NumeroPortion sont non values alors la liste des PAL de l'ensemble
* des autoroutes est retournee
*
* Si NomTroncon est non values alors la liste des PAL du troncon
*
* Si NumeroPortion est value alors la liste des PAL de la portion
*
* FONCTION :
*
* Retourne numero, nom, numero autoroute, PR, sens
* des PAL a partir des tables EQT_GEN, EQT_PAL
*
* MECANISMES :
*
------------------------------------------------------*/
    
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV 
   */
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
      
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
         
         /*A
         ** Recherche de la liste des PAL pour tous les site
         */
         
              select 
		GEN.nom,
		GEN.numero,
        	GEN.autoroute, 
        	GEN.PR, 
        	GEN.sens,
		PAL.type,
		PAL.adresse_ip,
		PAL.port
              from    CFG..EQT_GEN GEN, 
        	      CFG..EQT_PAL PAL
               where GEN.type         = XDC_EQT_PAL
                  and GEN.eqt_supprime = XDC_FAUX
                  and PAL.numero       = GEN.numero
            
         end
         
         else if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         
         /*A
         ** Recherche de la liste des PAL sur une portion
         */
         
         begin
         
              select 
		GEN.nom,
		GEN.numero,
        	GEN.autoroute, 
        	GEN.PR, 
        	GEN.sens,
		PAL.type,
		PAL.adresse_ip,
		PAL.port
              from    CFG..EQT_GEN GEN, 
        	      CFG..EQT_PAL PAL,
		      CFG..RES_POR POR
               where GEN.type = XDC_EQT_PAL
		  and GEN.eqt_supprime = XDC_FAUX
		  and POR.numero = @va_NumeroPortion_in
		  and GEN.autoroute = POR.autoroute
		  and GEN.PR <= PR_fin
		  and GEN.PR >= PR_debut
                  and PAL.numero = GEN.numero 
            
         end
         else if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         
         /*A
         ** Recherche de la liste des PAL sur un troncon
         */
         
         begin
         
              select 
		GEN.nom,
		GEN.numero,
        	GEN.autoroute, 
        	GEN.PR, 
        	GEN.sens,
		PAL.type,
		PAL.adresse_ip,
		PAL.port
              from    CFG..EQT_GEN GEN, 
        	      CFG..EQT_PAL PAL,
		      CFG..RES_TRC TRC
               where GEN.type = XDC_EQT_PAL
		  and GEN.eqt_supprime = XDC_FAUX
		  and TRC.nom = @va_NomTroncon_in
		  and GEN.autoroute = TRC.autoroute
		  and GEN.PR <= PR_fin
		  and GEN.PR > PR_debut
                  and PAL.numero = GEN.numero 
            
         end
         else
		select @va_Resultat_out = XDC_ECHEC
            
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         if @va_NomTroncon_in = null and  @va_NumeroPortion_in = null
         begin
         /*A
         ** Recherche de la liste des PAL pour tous les site
         */
         
              select 
		GEN.nom,
		GEN.numero,
        	GEN.autoroute, 
        	GEN.PR, 
        	GEN.sens,
		PAL.type,
		PAL.adresse_ip,
		PAL.port
        
              from    CFT..EQT_GEN GEN, 
        	      CFT..EQT_PAL PAL
               where GEN.type         = XDC_EQT_PAL
                  and GEN.eqt_supprime = XDC_FAUX
                  and PAL.numero       = GEN.numero
            
         end
         
         else if @va_NomTroncon_in = null and  @va_NumeroPortion_in <> null
         
         /*A
         ** Recherche de la liste des PAL sur une portion
         */
         
         begin
         
              select 
		GEN.nom,
		GEN.numero,
        	GEN.autoroute, 
        	GEN.PR, 
        	GEN.sens,
		PAL.type,
		PAL.adresse_ip,
		PAL.port
        
              from    CFT..EQT_GEN GEN, 
        	      CFT..EQT_PAL PAL,
		      CFT..RES_POR POR
               where GEN.type = XDC_EQT_PAL
		  and GEN.eqt_supprime = XDC_FAUX
		  and POR.numero = @va_NumeroPortion_in
		  and GEN.autoroute = POR.autoroute
		  and GEN.PR <= PR_fin
		  and GEN.PR >= PR_debut
                  and PAL.numero = GEN.numero 
            
         end
         else if @va_NomTroncon_in <> null and  @va_NumeroPortion_in = null
         
         /*A
         ** Recherche de la liste des PAL sur un troncon
         */
         
         begin
         
              select 
		GEN.nom,
		GEN.numero,
        	GEN.autoroute, 
        	GEN.PR, 
        	GEN.sens,
		PAL.type,
		PAL.adresse_ip,
		PAL.port
              from    CFT..EQT_GEN GEN, 
        	      CFT..EQT_PAL PAL,
		      CFT..RES_TRC TRC
               where GEN.type = XDC_EQT_PAL
		  and GEN.eqt_supprime = XDC_FAUX
		  and TRC.nom = @va_NomTroncon_in
		  and GEN.autoroute = TRC.autoroute
		  and GEN.PR <= PR_fin
		  and GEN.PR > PR_debut
                  and PAL.numero = GEN.numero 
            
         end
         else
		select @va_Resultat_out = XDC_ECHEC
            
         
            
      end

#endif      
   end
      
   return XDC_OK
         
go  
