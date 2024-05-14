/*E*/
/*Fichier : @(#)xzao464sp.prc	1.3      Release : 1.3        Date : 10/11/11
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao464sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
* JMG	10/11/11	Création  1.0 (DEM/1015)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao464sp.h" */

/* static char *version = "@(#)xzao464.c	1.0 10/11/11 : xzao464" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* recherche vitesse amont pour un PR donne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO464
go

create procedure XZAO464
	@va_Autoroute_in		T_AUTOROUTE,
	@va_PR_in			T_PR,
	@va_PR_tete_in			T_PR,
	@va_Sens_in			T_SENS,
	@va_Vitesse_out		tinyint output,
	@va_Tunnel_out		bit output
as

/* 
*
* SP	XZAO464SP
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	06 Fev 1996	: Creation (DEM/1395)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_sens		tinyint,
   @vl_PR_deb		integer,
   @vl_PR_fin		integer
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */

	if @va_Autoroute_in = null 
		or @va_PR_in = null 
		or @va_Sens_in = null 
   
		return XDC_ARG_INV
   
	else   
	begin
  
		select @va_Vitesse_out = 0

		if @va_Sens_in = XDC_SENS_SORTIE_NORD or
			@va_Sens_in = XDC_SENS_ENTREE_NORD or
			@va_Sens_in = XDC_SENS_NORD
			select @vl_sens = XDC_SENS_NORD
	
		 if @va_Sens_in = XDC_SENS_SORTIE_SUD or
			 @va_Sens_in = XDC_SENS_ENTREE_SUD or
			 @va_Sens_in = XDC_SENS_SUD
			 select @vl_sens = XDC_SENS_SUD

               	/*recherche portion*/
		set rowcount 1 

		if @vl_sens = XDC_SENS_SUD 
		begin

			select @va_Vitesse_out = POL.vitesse	
			from CFG..RES_POR POR, CFG..RES_POL POL
			where POR.autoroute = @va_Autoroute_in
			and POL.sens = @vl_sens
			and POL.portion = POR.numero
			and POL.PR <= @va_PR_in
			order by POL.PR desc

		end
		else
		begin

			select @va_Vitesse_out = POL.vitesse	
			from CFG..RES_POR POR, CFG..RES_POL POL
			where POR.autoroute = @va_Autoroute_in
			and POL.sens = @vl_sens
			and POL.portion = POR.numero
			and POL.PR >= @va_PR_in
			order by POL.PR 

		end

		if @va_PR_tete_in = 0
			select @vl_PR_fin = @va_PR_in
		else
			select @vl_PR_fin = @va_PR_tete_in

		if @vl_PR_fin < @va_PR_in
		begin
			select @vl_PR_deb = @vl_PR_fin
			select @vl_PR_fin = @va_PR_in
		end
		else
			select @vl_PR_deb = @va_PR_in
			
		
		select @va_Tunnel_out = 0
		select SEG.tunnel
		from CFG..RES_POR POR, CFG..RES_SEG SEG
		where POR.autoroute = @va_Autoroute_in
		and SEG.sens = @vl_sens
		and SEG.tunnel = 1
		and SEG.portion = POR.numero
		and 
		(
		(SEG.PR_debut <= @vl_PR_deb
		and SEG.PR_fin >= @vl_PR_deb)
		OR
		(SEG.PR_debut >= @vl_PR_deb
		and SEG.PR_fin <= @vl_PR_fin)
		OR
		(SEG.PR_debut <= @vl_PR_fin
		and SEG.PR_fin >= @vl_PR_fin)
		)

		if @@rowcount > 0
			select @va_Tunnel_out = 1
		else
			select @va_Tunnel_out = 0

	end

	return XDC_OK
go  
