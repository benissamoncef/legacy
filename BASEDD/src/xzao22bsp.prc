/*E*/
/*  Fichier : $Id: xzao22sp.prc,v 1.10 2000/07/21 16:56:47 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2000/07/21 16:56:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao22bsp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau 
* d'autoroute 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  31/08/94        : Creation 
* B.G.  07/11/94        : Creation procedure (v1.5)
* C.T.  06/02/98        : Correction erreur dans le cas
*                         de dM-Ebut de concession pour va_PRDebut_in (v 1.6)
* C.T.	23/02/98	: Reecriture de la proc en ajoutant le calcul sur 3 autoroutes (dem/1534)
* PN	08/03/99	: Retourne -1 si calcul impossible v1.7
* PN	08/03/2023	: Conversion XZAO;22 en XZAO22 pour compatibilite CreationBouchon
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calculer la distance entre le PR de debut et de fin
* sur 3 autoroutes maximum
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO22_Distance
* 
* Arguments en entree
* XDY_Entier		va_NumeroAutorouteDeb_in
* XDY_PR		va_PRDebut_in
* XDY_Entier		va_NumeroAutorouteFin_in
* XDY_PR		va_PRFin_in
* 
* Arguments en sortie
* XDY_Entier		va_Distance_out
* 
* Code retour
* XDC_OK
* XDC_NOK			: 
* XDC_ARG_INV	: parametre obligatoire manquant
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
* Fonction
* Recherche les portions correspondant a PR debut et PR fin
* Somme la longueur de chaque portion intermediaire
---------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO22' and type = 'P')
	drop procedure XZAO22
go

create procedure XZAO22
	@va_NumeroAutorouteDeb_in	T_AUTOROUTE = null,
	@va_PRDebut_in		T_PR = null,
	@va_NumeroAutorouteFin_in	T_AUTOROUTE = null,
	@va_PRFin_in		T_PR = null,
	@va_Distance_out	int  = -1  output
as
declare @vl_PRdebut1 T_PR, @vl_PRFin1 T_PR, @vl_PRInter1 T_PR, @vl_PRInter2 T_PR,
	@vl_PRdebut T_PR, @vl_PRFin T_PR, @vl_Autoroute T_AUTOROUTE, 
	@vl_PR T_PR, @vl_Nb int, @vl_Delta int,
	@vl_portion1 smallint, @vl_portion2 smallint,
	@vl_Longueur1 int, @vl_Longueur2 int, @vl_LongueurJonction int,
	@vl_ExtremitePortion1 bit, @vl_ExtremitePortion2 bit, @vl_Status int

	select @va_Distance_out=-1

	/* controle des parametres d'entree */
	if @va_NumeroAutorouteDeb_in is null or @va_PRDebut_in is null or
		@va_NumeroAutorouteFin_in is null or @va_PRFin_in is null 
		return XDC_ARG_INV

	/*A Recherche de l'existence des localisations en entree */
	select	@vl_portion1 = numero
	from	CFG..RES_POR
	where	autoroute=@va_NumeroAutorouteDeb_in and
		PR_debut<=@va_PRDebut_in and
		PR_fin>=@va_PRDebut_in
	if @vl_portion1 is null
		return XDC_NOK

	select	@vl_portion2 = numero
	from	CFG..RES_POR
	where	autoroute=@va_NumeroAutorouteFin_in and
		PR_debut<=@va_PRFin_in and
		PR_fin>=@va_PRFin_in
	if @vl_portion2 is null
		return XDC_NOK


	/* cas d'une seule autoroute */
	if @va_NumeroAutorouteDeb_in = @va_NumeroAutorouteFin_in
	begin
		/* distance incluse sur une seule portion */
		if @vl_portion1 = @vl_portion2
			select @va_Distance_out = abs(@va_PRDebut_in - @va_PRFin_in)
		else
		begin
			/* longueur sur les portions incluses completement */
			select  @va_Distance_out = sum(PR_fin - PR_debut)
			from CFG..RES_POR
			where autoroute = @va_NumeroAutorouteDeb_in and 
				((@va_PRDebut_in <= @va_PRFin_in and PR_debut >= @va_PRDebut_in and PR_fin <= @va_PRFin_in) or
				(@va_PRFin_in < @va_PRDebut_in and PR_debut >= @va_PRFin_in and PR_fin <= @va_PRDebut_in))
	
			/* calcul du delta entre PR fin 1ere portion et PR min. demande */
			/* et calcul du delta entre PR debut derniere portion et PR max demande */
			if @va_PRDebut_in <= @va_PRFin_in
			begin
				select  @vl_Delta = PR_fin - @va_PRDebut_in
				from CFG..RES_POR
				where autoroute = @va_NumeroAutorouteDeb_in and PR_debut < @va_PRDebut_in and PR_fin > @va_PRDebut_in
	
				select @va_Distance_out = isnull(@va_Distance_out, 0) + isnull(@vl_Delta, 0)
				select @vl_Delta = 0
		
				select  @vl_Delta = @va_PRFin_in - PR_debut
				from CFG..RES_POR
				where autoroute = @va_NumeroAutorouteDeb_in and PR_debut < @va_PRFin_in and PR_fin > @va_PRFin_in
			end
			else 
			begin
				select  @vl_Delta = PR_fin - @va_PRFin_in
				from CFG..RES_POR
				where autoroute = @va_NumeroAutorouteDeb_in and PR_debut < @va_PRFin_in and PR_fin > @va_PRFin_in
	
				select @va_Distance_out = isnull(@va_Distance_out, 0) + isnull(@vl_Delta, 0)
				select @vl_Delta = 0
	
				select  @vl_Delta = @va_PRDebut_in - PR_debut
				from CFG..RES_POR
				where autoroute = @va_NumeroAutorouteDeb_in and PR_debut < @va_PRDebut_in and PR_fin > @va_PRDebut_in
			end
			/* longueur totale */
			select @va_Distance_out = @va_Distance_out + isnull(@vl_Delta, 0)
		end


		select @va_Distance_out =  isnull(@va_Distance_out,-1)

		return XDC_OK
	end

	/* recherche des portions de jonction sur la 2eme intersection d'autoroute */
	select distinct
		portion1 = P1.numero, 
		PR_debut1 = P1.PR_debut,
		PR_fin1 = P1.PR_fin, 
		portion2 = P2.numero,
		autoroute = P2.autoroute,
		PR_debut2 = P2.PR_debut,
		PR_fin2 = P2.PR_fin,
		N1.extremite_portion_1,
		N1.extremite_portion_2
	into #POR2
	from CFG..RES_POR P1, CFG..RES_POR P2, CFG..RES_NEU N1
	where   P1.autoroute = @va_NumeroAutorouteFin_in and N1.portion1 = P1.numero and 
		P2.numero = N1.portion2  and 
		P2.autoroute != P1.autoroute 
	
	/* cas de deux autoroutes */
	if exists(select * from #POR2 where autoroute = @va_NumeroAutorouteDeb_in)
	begin
		/* recherche du PR d'intersection */
		select
			@vl_PRdebut1 = P1.PR_debut,
			@vl_PRFin1 = P1.PR_fin,
			@vl_PRdebut = P2.PR_debut,
			@vl_PRFin = P2.PR_fin,
			@vl_ExtremitePortion1 = N1.extremite_portion_1,
			@vl_ExtremitePortion2 = N1.extremite_portion_2
		from CFG..RES_POR P1, CFG..RES_POR P2, CFG..RES_NEU N1
		where   P1.autoroute = @va_NumeroAutorouteFin_in and N1.portion1 = P1.numero and 
			P2.numero = N1.portion2 and 
			P2.autoroute = @va_NumeroAutorouteDeb_in

		if @@rowcount = 0 
			return XDC_NOK

		if @vl_ExtremitePortion2 = 0
			select @vl_PRInter1 = @vl_PRdebut
		else
			select @vl_PRInter1 = @vl_PRFin
		
	
		/* PR d'intersection sur l'autoroute 2 */
		if @vl_ExtremitePortion1 = 0
			select @vl_PR = @vl_PRdebut1
		else	select @vl_PR = @vl_PRFin1

		exec @vl_Status = PRC..XZAO22 @va_NumeroAutorouteDeb_in, @va_PRDebut_in, @va_NumeroAutorouteDeb_in, @vl_PRInter1, @vl_Longueur1 output

		
		if @vl_Status != XDC_OK
			return @vl_Status

		exec @vl_Status = PRC..XZAO22 @va_NumeroAutorouteFin_in, @va_PRFin_in, @va_NumeroAutorouteFin_in, @vl_PR, @vl_Longueur2 output

		/* longueur totale */
		select @va_Distance_out = @vl_Longueur1 + @vl_Longueur2 

		select @va_Distance_out = isnull(@va_Distance_out,-1)
		return @vl_Status
	end

	/* recherche des portions de jonction sur la 1ere intersection d'autoroute */
	select distinct
		portion1 = P1.numero, 
		PR_debut1 = P1.PR_debut, 
		PR_fin1 = P1.PR_fin, 
		portion2 =P2.numero,
		P2.autoroute,
		PR_debut2 = P2.PR_debut,
		PR_fin2 = P2.PR_fin,
		N1.extremite_portion_1,
		N1.extremite_portion_2
	into #POR1
	from CFG..RES_POR P1, CFG..RES_POR P2, CFG..RES_NEU N1
	where P1.autoroute = @va_NumeroAutorouteDeb_in and 
		N1.portion1 = P1.numero and P2.numero = N1.portion2 and
		P2.autoroute in ( select distinct autoroute from #POR2 )
	
 -- select * from #POR1
 -- select * from #POR2
	/* tester si on a trouve des portions sinon distance sur plus de 3 autoroutes : erreur */
	select @vl_Nb = count(*) from #POR1

	if @vl_Nb = 0 
		return XDC_NOK

	/* supprimer les portions n'�tant pas sur l'autoroute d'intersection */
	delete #POR2 
	from #POR1, #POR2
	where #POR2.autoroute != #POR1.autoroute
	
	/* tester si on a trouve des portions sinon distance sur plus de 3 autoroutes : erreur */
	select @vl_Nb = count(*) from #POR2

	if @vl_Nb = 0 
		return XDC_NOK
	
	set rowcount 1

	/* recherche du PR d'intersection 2 */
	select 
		@vl_PRdebut1 = PR_debut1,
		@vl_PRFin1 = PR_fin1,
		@vl_Autoroute = autoroute,
		@vl_PRdebut = PR_debut2,
		@vl_PRFin = PR_fin2,
		@vl_ExtremitePortion1 = extremite_portion_1,
		@vl_ExtremitePortion2 = extremite_portion_2
	from #POR2

	set rowcount 0

	if @vl_ExtremitePortion2 = 0
		select @vl_PRInter1 = @vl_PRdebut
	else
		select @vl_PRInter1 = @vl_PRFin
	

	/* PR d'intersection sur l'autoroute 2 */
	if @vl_ExtremitePortion1 = 0
		select @vl_PR = @vl_PRdebut1
	else	select @vl_PR = @vl_PRFin1

-- select @va_NumeroAutorouteFin_in, @va_PRFin_in, @va_NumeroAutorouteFin_in, @vl_PR

	/* calcul longueur sur 2 eme autoroute */
	exec @vl_Status = PRC..XZAO22 @va_NumeroAutorouteFin_in, @va_PRFin_in, 
			@va_NumeroAutorouteFin_in, @vl_PR, @vl_Longueur2 output
	
	if @vl_Status != XDC_OK
		return @vl_Status
	
	set rowcount 1

	/* rechercher le PR avec l'intersection 1 sur l'autoroute de jonction */
	select
		@vl_PRdebut1 = PR_debut1,
		@vl_PRFin1 = PR_fin1,
		@vl_PRdebut = PR_debut2,
		@vl_PRFin = PR_fin2,
		@vl_ExtremitePortion1 = extremite_portion_1,
		@vl_ExtremitePortion2 = extremite_portion_2
	from #POR1
	
	set rowcount 0
	
	if @vl_ExtremitePortion2 = 0
		select @vl_PRInter2 = @vl_PRdebut
	else
		select @vl_PRInter2 = @vl_PRFin

	/* PR d'intersection sur l'autoroute 1 */
	if @vl_ExtremitePortion1 = 0
		select @vl_PR = @vl_PRdebut1
	else	select @vl_PR = @vl_PRFin1
	
 -- select @va_NumeroAutorouteDeb_in, @va_PRDebut_in, @va_NumeroAutorouteDeb_in, @vl_PR
	/* calcul longueur sur 1 ere autoroute */
	exec @vl_Status = PRC..XZAO22 @va_NumeroAutorouteDeb_in, @va_PRDebut_in, 
					@va_NumeroAutorouteDeb_in, @vl_PR , @vl_Longueur1 output
	
	if @vl_Status != XDC_OK
		return @vl_Status
	
 -- select "Jonction = A:",@vl_Autoroute, @vl_PRInter1,@vl_Autoroute, @vl_PRInter2

	/* calcul la distance sur l'autoroute de jonction (portions entieres) */
	select  @vl_LongueurJonction = sum(PR_fin - PR_debut)
	from CFG..RES_POR
	where autoroute = @vl_Autoroute and ((@vl_PRInter1 <= @vl_PRInter2 and PR_debut >= @vl_PRInter1 and PR_fin <= @vl_PRInter2) or
		(@vl_PRInter2 < @vl_PRInter1 and PR_debut >= @vl_PRInter2 and PR_fin <= @vl_PRInter1))
	
	/* longueur totale */
	select @va_Distance_out = @vl_Longueur1 + @vl_Longueur2 + @vl_LongueurJonction

--  select @vl_Longueur1 , @vl_Longueur2 , @vl_LongueurJonction
	select @va_Distance_out = isnull(@va_Distance_out, -1)
return XDC_OK
go
