/*E*/
/*Fichier : $Id: xzao204sp.prc,v 1.3 1997/02/10 16:31:04 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1997/02/10 16:31:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao204sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao204sp.h" */

/* static char *version = "" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des FAV d'un troncon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;204
   @va_Basedd_in       	char(3),
   @va_NomTroncon_in	char(25),
   @va_Tous_in		bit,
   @va_Resultat_out	int = null output	
   as 

/* 
*
* SP	XZAO204SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomTroncon_in
* XDY_Booleen	va_Tous_in
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
* Retourne la liste des FAV d'un troncon
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
------------------------------------------------------
* HISTORIQUE :
*
* volcic	10 Oct 1994	: Creation
* niepceron	15 Jan 1997	: 
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Autoroute	tinyint,
   @vl_PRdebut		int,
   @vl_PRfin		int,
   @vl_PR_tete_Toulon_Nord	int,
   @vl_PR_tete_Toulon_Sud 	int
   
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_Tous_in = null
   or @va_NomTroncon_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls  
   */
      
   begin
      
      /*A
      **  Travail dans la base CFG
      */   

	  /* Verrue tunnel de Toulon : les PR de tete ne correspondent pas aux PR des SAV de tete */
	  select @vl_PR_tete_Toulon_Nord = -100
	  select @vl_PR_tete_Toulon_Sud = -100
	  if @va_NomTroncon_in="BANDOL A50"
	  begin
		select @vl_PR_tete_Toulon_Nord = PR from CFG..EQT_GEN where type=XDC_EQT_TUBE and numero=37
		select @vl_PR_tete_Toulon_Sud = PR from CFG..EQT_GEN where type=XDC_EQT_TUBE and numero=38	  
	  end
	  
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche la liste des FAV d'un Troncon
         */
        select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_TRC
              where nom = @va_NomTroncon_in
         if @va_Tous_in = XDC_VRAI
         	select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
         	from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
          	where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie		<> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PRdebut
		     and GTUB.PR       		<= @vl_PRfin 
	             and PAN.numero		= GTUB.numero
             
 	        order by GTUB.numero
 	 else
	         select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
	         from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
	           where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PRdebut
		     and GTUB.PR       		<= @vl_PRfin 
	             and PAN.numero		= GTUB.numero
	             and GTUB.PR in ( select PR from CFG..EQT_GEN where type=XDC_EQT_TUBE and sens=GTUB.sens and autoroute=GTUB.autoroute)
 			 union
 	         select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
	         from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
	           where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PR_tete_Toulon_Nord - 30
		     and GTUB.PR       		<= @vl_PR_tete_Toulon_Nord + 30 
	             and PAN.numero		= GTUB.numero
			 union
	         select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
	         from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
	           where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PR_tete_Toulon_Sud - 30
		     and GTUB.PR       		<= @vl_PR_tete_Toulon_Sud + 30 
	             and PAN.numero		= GTUB.numero

	         order by GTUB.numero
         
      end

#ifdef CI      
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche la liste des FAV d'un Troncon
         */
        select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_TRC
              where nom = @va_NomTroncon_in
         
         if @va_Tous_in = XDC_VRAI
         	select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
         	from CFT..EQT_PAN PAN, CFT..EQT_GEN GTUB
         
          	where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> null
		     and PAN.voie               <> 0
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PRdebut
		     and GTUB.PR       		<= @vl_PRfin 
	             and PAN.numero		= GTUB.numero
             
 	        order by GTUB.numero
 	 else
	         select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
	         from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
	           where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PRdebut
		     and GTUB.PR       		<= @vl_PRfin 
	             and PAN.numero		= GTUB.numero
	             and GTUB.PR in ( select PR from CFG..EQT_GEN where type=XDC_EQT_TUBE and sens=GTUB.sens and autoroute=GTUB.autoroute)
 			 union
 	         select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
	         from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
	           where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PR_tete_Toulon_Nord - 30
		     and GTUB.PR       		<= @vl_PR_tete_Toulon_Nord + 30 
	             and PAN.numero		= GTUB.numero
			 union
	         select distinct GTUB.numero, 
         		GTUB.autoroute, 
         		GTUB.PR, 
         		GTUB.sens, 
         		PAN.voie,
         		PAN.inverse
         		         	
	         from CFG..EQT_PAN PAN, CFG..EQT_GEN GTUB
         
	           where GTUB.type   		= XDC_EQT_PAN
		     and PAN.voie               <> 0
		     and PAN.voie               <> null
		     and GTUB.autoroute 	= @vl_Autoroute
		     and GTUB.PR       		>  @vl_PR_tete_Toulon_Sud - 30
		     and GTUB.PR       		<= @vl_PR_tete_Toulon_Sud + 30 
	             and PAN.numero		= GTUB.numero

	         order by GTUB.numero
         
      end

#endif      
      
   end
      
   return XDC_OK
         
go  
