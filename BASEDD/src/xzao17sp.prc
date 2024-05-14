/*E*/
/*  Fichier : $Id: xzao17sp.prc,v 1.8 2012/07/26 11:48:42 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2012/07/26 11:48:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau 
* d'autoroute 
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  07/11/94        : Creation (v1.1)
* C.T.  29/06/95        : Modif numero de sortie echangeur (v1.3)
* B.G.  19/07/95	: si pas de correspondance echangeur numero, on retourne echangeur (1.4)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.5)
* C.T.	04/04/96	: Rechercher d'un echangeur complet si le sens est inconnu (1.6) (795)
* PNI	16/11/10	: Recherche le premier échangeur en sens 2 pour un sens inconnu (à la place d'un échangeur complet) DEM962 1.7
* JPL	25/06/12	: Degroupee; retour numero echangeur brut + nom sortie usager, indic. bifurcation (DEM 1033)  1.8
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche la "numero" sortie en amont (>0) ou
* en aval (<0). 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO17_Sortie_amont
* 
* Arguments en entree
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Mot		va_Numero_in
* 
* Arguments en sortie
* XDY_Octet		va_NumSortie_out
* XDY_Nom		va_NomSortie_out
* XDY_Abrev		va_AbrevSortie_out
* XDY_Sortie		va_SortieUsager_out
* XDY_Booleen		va_Bifurcation_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* XDY_Entier		va_Distance_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* le test se fait par rapport au PR de sortie
* recherche le numero de sortie Escota 
---------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO17' and type = 'P')
	drop procedure XZAO17
go


create procedure XZAO17
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PR_in		T_PR = null,
	@va_Sens_in		tinyint = null,
	@va_Numero_in		smallint = null,
	@va_NumSortie_out	tinyint = null output,
	@va_NomSortie_out	char(25) = XDC_CHAINE_VIDE output,
	@va_AbrevSortie_out	char(15) = XDC_CHAINE_VIDE output,
	@va_SortieUsager_out	char(20) = XDC_CHAINE_VIDE output,
	@va_Bifurcation_out	tinyint = XDC_FAUX output,
	@va_Autoroute_out	T_AUTOROUTE = null output,
	@va_PR_out		T_PR = null output,
	@va_Sens_out		T_SENS = null output,
	@va_Distance_out	int = null output
as


/*A Definition des valeurs de sortie par defaut */
	select 	@va_NumSortie_out	= null,
		@va_NomSortie_out	= XDC_CHAINE_VIDE,
		@va_AbrevSortie_out	= XDC_CHAINE_VIDE,
		@va_SortieUsager_out	= XDC_CHAINE_VIDE,
		@va_Bifurcation_out	= XDC_FAUX,
		@va_Autoroute_out	= null,
		@va_PR_out		= null,
		@va_Sens_out		= null,
		@va_Distance_out	= null

/*A Controle des parametres d'entree obligatoire */

	if	@va_Autoroute_in = null or
		@va_PR_in = null or
		@va_Sens_in = null or
		@va_Numero_in = null or
		@va_Numero_in = 0

	return XDC_ARG_INV


/*A Declaration des variables locales */

	declare	@vl_cr int

/*A Ramene le sens sur section courante s'il s'agit d'une sortie ou d'une entree */

	if	@va_Sens_in=XDC_SENS_SORTIE_NORD or
		@va_Sens_in=XDC_SENS_ENTREE_NORD
	select	@va_Sens_in=XDC_SENS_NORD
	
	if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_SUD
	select	@va_Sens_in=XDC_SENS_SUD

/*A Si le sens est inconnu : retourne null (pas de sortie amont) */
	
	if	@va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD and
		@va_Sens_in<>XDC_SENS_INCONNU
	return XDC_OK
	
/*A Si l'autoroute est inversee, change le signe de @va_Numero_in (A57) */

	if	exists (select * from CFG..RES_AUT_INV where numero=@va_Autoroute_in and inversee=XDC_PR_INV)
	select	@va_Numero_in=-@va_Numero_in
	
/*A Suivant le sens et le signe de @va_Numero_in, declare un curseur asc ou desc */

	if	( @va_Sens_in=XDC_SENS_SUD and @va_Numero_in>0 )
	begin
		declare Sortie cursor for
			select 	ECH.numero,
				ECH.nom,
				ECH.abreviation,
				isnull (ECH.sortie, XDC_CHAINE_VIDE),
				ECH.bifurcation,
				ECH.PR_sortie_sens1
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens1<@va_PR_in and	-- en amont (SUD)
				ECH.supprime = 0 and
				ECH.voies_sortie_sens1<>0
			order by ECH.PR desc 		-- le plus proche en premier
	end
	else
	if	( @va_Sens_in=XDC_SENS_NORD and @va_Numero_in<0 )
	begin
		declare Sortie cursor for
			select 	ECH.numero,
				ECH.nom,
				ECH.abreviation,
				isnull (ECH.sortie, XDC_CHAINE_VIDE),
				ECH.bifurcation,
				ECH.PR_sortie_sens2
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens2<@va_PR_in and	-- aval (NORD)
				ECH.supprime = 0 and
				ECH.voies_sortie_sens2<>0
			order by ECH.PR desc 		-- le plus proche en premier
	end
	else
	if	( @va_Sens_in=XDC_SENS_SUD and @va_Numero_in<0 )
	begin
		declare Sortie cursor for
			select 	ECH.numero,
				ECH.nom,
				ECH.abreviation,
				isnull (ECH.sortie, XDC_CHAINE_VIDE),
				ECH.bifurcation,
				ECH.PR_sortie_sens1
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens1>@va_PR_in and	-- en aval (SUD)
				ECH.supprime = 0 and
				ECH.voies_sortie_sens1<>0
			order by ECH.PR asc 		-- le plus proche en premier
	end
	else
	if	( @va_Sens_in=XDC_SENS_NORD and @va_Numero_in>0 )
	begin
		declare Sortie cursor for
			select 	ECH.numero,
				ECH.nom,
				ECH.abreviation,
				isnull (ECH.sortie, XDC_CHAINE_VIDE),
				ECH.bifurcation,
				ECH.PR_sortie_sens2
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens2>@va_PR_in and	-- en amont (NORD)
				ECH.supprime = 0 and
				ECH.voies_sortie_sens2<>0
			order by ECH.PR asc 		-- le plus proche en premier
	end
	else
	if	( @va_Sens_in=XDC_SENS_INCONNU and @va_Numero_in>0 )
	begin
		declare Sortie cursor for
			select 	ECH.numero,
				ECH.nom,
				ECH.abreviation,
				isnull (ECH.sortie, XDC_CHAINE_VIDE),
				ECH.bifurcation,
				ECH.PR_sortie_sens1
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens2>@va_PR_in and	-- en amont (NORD)
				/*ECH.PR_sortie_sens1 > 0 and		-- echangeur complet
				ECH.PR_entree_sens1 > 0 and
				ECH.PR_entree_sens2 > 0 and */
				ECH.supprime = 0 and
				ECH.voies_sortie_sens2<>0
			order by ECH.PR asc 		-- le plus proche en premier
	end
	else
	if	( @va_Sens_in=XDC_SENS_INCONNU and @va_Numero_in<0 )
	begin
		declare Sortie cursor for
			select 	ECH.numero,
				ECH.nom,
				ECH.abreviation,
				isnull (ECH.sortie, XDC_CHAINE_VIDE),
				ECH.bifurcation,
				ECH.PR_sortie_sens1
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens2<@va_PR_in and	-- en aval (NORD)
				ECH.PR_sortie_sens1 > 0 and		-- echangeur complet
				ECH.PR_entree_sens1 > 0 and
				ECH.PR_entree_sens2 > 0 and
				ECH.supprime = 0 and
				ECH.voies_sortie_sens1<>0
			order by ECH.PR desc 		-- le plus proche en premier
	end
	
	select @va_Numero_in=abs(@va_Numero_in)

	open Sortie

	fetch Sortie into	@va_NumSortie_out,
				@va_NomSortie_out,
				@va_AbrevSortie_out,
				@va_SortieUsager_out,
				@va_Bifurcation_out,
				@va_PR_out

	while @@sqlstatus = 0 and @va_Numero_in > 1
	begin
		fetch Sortie into	@va_NumSortie_out,
					@va_NomSortie_out,
					@va_AbrevSortie_out,
					@va_SortieUsager_out,
					@va_Bifurcation_out,
					@va_PR_out

		select @va_Numero_in=@va_Numero_in-1
	end
	close Sortie
	
/*A  si on a trouve une sortie : recherche distance, autoroute et sens */

	if @@sqlstatus = 0
	begin
		select 	@va_Autoroute_out=@va_Autoroute_in,
			@va_Sens_out=@va_Sens_in

		exec @vl_cr=XZAO;22 	@va_Autoroute_out,
					@va_PR_out,
					@va_Autoroute_in,
					@va_PR_in,
					@va_Distance_out output

		if @vl_cr<>XDC_OK return XDC_NOK
	end
	
/*A  sinon on positionne les parametres retour a null */

	else
	select 	@va_NumSortie_out	= null,
		@va_NomSortie_out	= XDC_CHAINE_VIDE,
		@va_AbrevSortie_out	= XDC_CHAINE_VIDE,
		@va_SortieUsager_out	= XDC_CHAINE_VIDE,
		@va_Bifurcation_out	= XDC_FAUX,
		@va_PR_out=null

	return XDC_OK

go
