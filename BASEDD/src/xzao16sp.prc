/*E*/
/*  Fichier : $Id: xzao16sp.prc,v 1.2 1995/11/02 10:58:18 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/11/02 10:58:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao16sp.prc
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
* B.G.	24/10/95	: gestion du sens inverse A57 (1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche la "numero" aire en amont (>0) ou
* en aval (<0). 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO16_Aire_amont
* 
* Arguments en entree
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Mot		va_Numero_in
* 
* Arguments en sortie
* XDY_Octet		va_NumAire_out
* XDY_Nom		va_NomAire_out
* XDY_Abrev		va_AbrevAire_out
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
* 
---------------------------------------------------------*/

use PRC
go

create procedure XZAO;16
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PR_in		T_PR = null,
	@va_Sens_in		tinyint = null,
	@va_Numero_in		smallint = null,
	@va_NumAire_out		tinyint = null output,
	@va_NomAire_out		char(25) = null output,
	@va_AbrevAire_out	char(15) = null output,
	@va_Autoroute_out	T_AUTOROUTE = null output,
	@va_PR_out		T_PR = null output,
	@va_Sens_out		tinyint = null output,
	@va_Distance_out	int = null output
as

/*A Controle des parametres d'entree obligatoire */

	if	@va_Autoroute_in = null or
		@va_PR_in = null or
		@va_Sens_in = null or
		@va_Numero_in = null or
		@va_Numero_in = 0

	return XDC_ARG_INV

/*A Declaration des variables locales */

	declare	@vl_cr int

/*A Ram…ne le sens sur section courante s'il s'agit d'une sortie ou d'une entree */

	if	@va_Sens_in=XDC_SENS_SORTIE_NORD or
		@va_Sens_in=XDC_SENS_ENTREE_NORD
	select	@va_Sens_in=XDC_SENS_NORD
	
	if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_SUD
	select	@va_Sens_in=XDC_SENS_SUD

/*A Si le sens est inconnu : retourne null (pas d'aire amont) */
	
	if	@va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
	return XDC_OK
	
/*A Si l'autoroute est inversee, change le signe de @va_Numero_in (A57) */

	if	exists (select * from CFG..RES_AUT_INV where numero=@va_Autoroute_in and inversee=XDC_PR_INV)
	select	@va_Numero_in=-@va_Numero_in
	
/*A Suivant le sens et le signe de @va_Numero_in, declare un curseur asc ou desc */

	if	( @va_Sens_in=XDC_SENS_SUD and @va_Numero_in>0 ) or
		( @va_Sens_in=XDC_SENS_NORD and @va_Numero_in<0 )
	begin
		declare Aire cursor for
			select 	AIR.numero,
				AIR.nom,
				AIR.abreviation,
				AIR.PRentree
			from	CFG..RES_AIR AIR,
				CFG..RES_POR POR
			where	AIR.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				AIR.sens=@va_Sens_in and
				AIR.PRentree<@va_PR_in 	-- en amont (SUD) ou aval (NORD)
			order by AIR.PRentree desc 	-- la plus proche en premier
	end
	else
	begin
		declare Aire cursor for
			select 	AIR.numero,
				AIR.nom,
				AIR.abreviation,
				AIR.PRentree
			from	CFG..RES_AIR AIR,
				CFG..RES_POR POR
			where	AIR.portion=POR.numero and
				POR.autoroute=@va_Autoroute_in and
				AIR.sens=@va_Sens_in and
				AIR.PRentree>@va_PR_in 	-- en aval (SUD) ou amont (NORD)
			order by AIR.PRentree asc 	-- la plus proche en premier
	end
	
	select @va_Numero_in=abs(@va_Numero_in)

	open Aire

	fetch Aire into	@va_NumAire_out,
			@va_NomAire_out,
			@va_AbrevAire_out,
			@va_PR_out

	while @@sqlstatus = 0 and @va_Numero_in > 1
	begin
		fetch Aire into	@va_NumAire_out,
				@va_NomAire_out,
				@va_AbrevAire_out,
				@va_PR_out

		select @va_Numero_in=@va_Numero_in-1
	end
	close Aire
	
/*A  si on a trouv≈ une aire : recherche distance, autoroute et sens */

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
	
/*A  sinon on positionne les param…tres retour » null */

	else
	select 	@va_NumAire_out=null,
		@va_NomAire_out=' ',
		@va_AbrevAire_out=' ',
		@va_PR_out=null

	return XDC_OK

go
