#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaoc.h"
/*E*/
/*  Fichier : $Id: xzao802sp.prc,v 1.1 2021/06/09 11:09:17 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2021/06/09 11:09:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao802sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* retourne les pr du tunnel opposé à partir d'un pr, autoroute, sens. Retourne XZAEC_FMC_LOC_INC si les param_in ne sont pas un tunnel
*
------------------------------------------------------
* HISTORIQUE :
* ABE	10/05/2021	Creation DEM-SAE244 1.1
-----------------------------------------------------*/				

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne les pr du tunnel opposé à partir d'un pr, autoroute, sens. Retourne XZAEC_FMC_LOC_INC si les param_in ne sont pas un tunnel
* 
* Sequence d'appel
* SP	XZAO802_Tunnel_Oppose
* 
* Arguments en entree
* XDY_Autoroute		Autoroute_In
* XDY_PR		PR_In
* XDY_Sens		Sens_In
*
* Arguments en sortie
* XDY_PR		PR_Debut_Out
* XDY_PR		PR_Fin_Out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
* 
* Fonction
* Select from EQT_GEN et EQT_TUB
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO802' and type = 'P')
	drop procedure XZAO802
go

create procedure XZAO802				
	@va_Autoroute_in	tinyint = null,
   	@va_PR_in		int = null,
   	@va_Sens_in		tinyint = null,
	@va_PR_Debut_out	int = null output,
	@va_PR_Fin_out		int = null output

as
	declare
		@vl_PR_Debut int,
		@vl_PR_Fin int,
		@vl_A57 tinyint

	/* récupération valeur A57*/
	select @vl_A57 = numero from CFG..RES_AUT where nom="A57" 

	if @va_Autoroute_in = null or @va_PR_in = null or @va_Sens_in = null 
		return XDC_ARG_INV

	/* valeur initiales */
	select @va_PR_Debut_out = -1
	select @va_PR_Fin_out = -1

	if (@va_Autoroute_in!=@vl_A57) /* On cherche le premier equipement amont de la circulation qui correspond au param */
	begin
		if (@va_Sens_in = XDC_SENS_SUD or @va_Sens_in = XDC_SENS_ENTREE_SUD)
		begin
			/* on cherche si le PR en param est dans un tub */
			if exists(select * from CFG..EQT_GEN G, CFG..EQT_TUB T
				where G.type = XDC_EQT_TUB
				and G.autoroute = @va_Autoroute_in
				and G.sens = @va_Sens_in
				and T.numero = G.numero
				and @va_PR_in between G.PR and T.PR_Influence)
			begin
				/* On cherche si le PR sur la route opposé fait partie d'un tunnel */
				select @vl_PR_Debut = T.PR_Influence, @vl_PR_Fin = G.PR from CFG..EQT_GEN G, CFG..EQT_TUB T
					where G.type = XDC_EQT_TUB
					and G.autoroute = @va_Autoroute_in
					and G.sens != @va_Sens_in
					and T.numero = G.numero
					and @va_PR_in between  T.PR_Influence and G.PR

				/* PR oppose pas dans tunnel */												
				if @@rowcount = 0
				return XDC_OK							

				select @va_PR_Debut_out = @vl_PR_Debut
				select @va_PR_Fin_out = @vl_PR_Fin
				return XDC_OK
			end				
		end
		else
		begin
			/* on cherche si le PR en param est dans un tub */
			if exists(select * from CFG..EQT_GEN G, CFG..EQT_TUB T
				where G.type = XDC_EQT_TUB
				and G.autoroute = @va_Autoroute_in
				and G.sens = @va_Sens_in
				and T.numero = G.numero
				and @va_PR_in between T.PR_Influence and G.PR)
			begin
				/* On cherche si le PR sur la route opposé fait partie d'un tunnel */
				select @vl_PR_Debut = G.PR, @vl_PR_Fin = T.PR_Influence from CFG..EQT_GEN G, CFG..EQT_TUB T
					where G.type = XDC_EQT_TUB
					and G.autoroute = @va_Autoroute_in
					and G.sens != @va_Sens_in
					and T.numero = G.numero
					and @va_PR_in between G.PR and T.PR_Influence

				/* PR oppose pas dans tunnel */
				if @@rowcount = 0
				return XDC_OK							

				select @va_PR_Debut_out = @vl_PR_Debut
				select @va_PR_Fin_out = @vl_PR_Fin
				return XDC_OK
			end	
		end
	end
	else /* Pour l'A57 on prend le premier equipement amont dans le sens de la circulation*/
	begin
		if(@va_Sens_in = XDC_SENS_NORD or @va_Sens_in = XDC_SENS_ENTREE_NORD)
		begin
			/* on cherche si le PR en param est dans un tub */
			if exists(select * from CFG..EQT_GEN G, CFG..EQT_TUB T
				where G.type = XDC_EQT_TUB
				and G.autoroute = @va_Autoroute_in
				and G.sens = @va_Sens_in
				and T.numero = G.numero
				and @va_PR_in between G.PR and T.PR_Influence)
			begin
				/* On cherche si le PR sur la route opposé fait partie d'un tunnel */
				select @vl_PR_Debut = T.PR_Influence, @vl_PR_Fin = G.PR from CFG..EQT_GEN G, CFG..EQT_TUB T
					where G.type = XDC_EQT_TUB
					and G.autoroute = @va_Autoroute_in
					and G.sens != @va_Sens_in
					and T.numero = G.numero
					and @va_PR_in between  T.PR_Influence and G.PR

				/* PR oppose pas dans tunnel */												
				if @@rowcount = 0
				return XDC_OK							

				select @va_PR_Debut_out = @vl_PR_Debut
				select @va_PR_Fin_out = @vl_PR_Fin
				return XDC_OK
			end				
		end
		else
		begin
			/* on cherche si le PR en param est dans un tub */
			if exists(select * from CFG..EQT_GEN G, CFG..EQT_TUB T
				where G.type = XDC_EQT_TUB
				and G.autoroute = @va_Autoroute_in
				and G.sens = @va_Sens_in
				and T.numero = G.numero
				and @va_PR_in between T.PR_Influence and G.PR)
			begin
				/* On cherche si le PR sur la route opposé fait partie d'un tunnel */
				select @vl_PR_Debut = G.PR, @vl_PR_Fin = T.PR_Influence from CFG..EQT_GEN G, CFG..EQT_TUB T
					where G.type = XDC_EQT_TUB
					and G.autoroute = @va_Autoroute_in
					and G.sens != @va_Sens_in
					and T.numero = G.numero
					and @va_PR_in between G.PR and T.PR_Influence

				/* PR oppose pas dans tunnel */
				if @@rowcount = 0
				return XDC_OK							

				select @va_PR_Debut_out = @vl_PR_Debut
				select @va_PR_Fin_out = @vl_PR_Fin
				return XDC_OK
			end	
		end
	end

	return XDC_NOK
go
