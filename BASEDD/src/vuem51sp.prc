/*E*/
/*  Fichier : $Id: vuem51sp.prc,v 1.1 1996/07/01 18:02:37 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/07/01 18:02:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem51sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fichiers metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	01/07/96	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* calcul la surface d'une zone de PR en fonction du nombre de voies circulées
* 
* Sequence d'appel
* SP	VUEM51_Surface_Zone
* 
* Arguments en entree
* XDY_Octet	va_Autoroute_in		
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Sens	va_Sens_in		
* 
* Arguments en sortie
* XDY_Entier	va_Surface_out		
*
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee 
* 
* Conditions d'utilisation
* 
*
-------------------------------------------------------*/

use PRC
go

create proc VUEM51
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PRDebut_in		T_PR	= null,
	@va_PRFin_in		T_PR	= null,
	@va_Sens_in		T_SENS	= null,
	@va_Surface_out		int	= null output
as
	if @va_Autoroute_in is null or @va_PRDebut_in is null or
		@va_PRFin_in is null or @va_Sens_in is null
		return XDC_ARG_INV

	/*A rechercher les segments concernés */
	select 
		PR_fin_por = CFG..RES_POR.PR_fin,
		PR_debut_seg = CFG..RES_SEG.PR_debut,
		nombre_de_voies
	into #SEGMENTS
	from CFG..RES_POR, CFG..RES_SEG
	where autoroute = @va_Autoroute_in and
		CFG..RES_POR.PR_debut < @va_PRFin_in and 
		CFG..RES_POR.PR_fin >= @va_PRDebut_in and
		portion = numero and
		sens = @va_Sens_in
	order by CFG..RES_SEG.PR_debut

	/*A rechercher le PR de fin de chaque segment */
	select
		D.PR_fin_por,
		D.PR_debut_seg,
		PR_fin_seg = F.PR_debut_seg,
		D.nombre_de_voies
	into #SEGMENTS_LONG
	from #SEGMENTS D, #SEGMENTS F
	where F.PR_debut_seg > D.PR_debut_seg
	group by D.PR_debut_seg
	having F.PR_debut_seg > D.PR_debut_seg and
		F.PR_debut_seg = min(F.PR_debut_seg)

	/*A prise en compte des PR fantomes */
	update #SEGMENTS_LONG set PR_fin_seg = PR_fin_por
	where PR_fin_por < PR_fin_seg

	/*A supprimer les segments n'etant sur la zone donnee */
	delete #SEGMENTS_LONG
	where PR_fin_seg < @va_PRDebut_in or
		PR_debut_seg > @va_PRFin_in

	/*A PR intial du premier segment est celui donne en entree */
	update #SEGMENTS_LONG set PR_debut_seg = @va_PRDebut_in
	where PR_debut_seg < @va_PRDebut_in
	update #SEGMENTS_LONG set PR_fin_seg = @va_PRFin_in
	where PR_fin_seg > @va_PRFin_in

	/*A calcul de la surface des voies entre 2 PR */
	select
		@va_Surface_out = sum((PR_fin_seg - PR_debut_seg) * nombre_de_voies)
	from #SEGMENTS_LONG

	return XDC_OK
go
