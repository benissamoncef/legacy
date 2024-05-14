/*E*/
/*  Fichier : $Id: xzac79sp.prc,v 1.1 2012/10/29 19:28:07 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/10/29 19:28:07 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzac79sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture des actions
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	25/10/12 : Creation (DEM 1032)  1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Action d'envoi des donnees de Conditions de Conduite : donnees additionnelles.
*
* SEQUENCE D'APPEL
*
* SP	XZAC79_Ajouter_Conditions_CCH
*
* Arguments en entree :
*  XDY_Entier           va_NumAction_in
*  XDY_Octet            va_NumSite_in
*           n fois :
*     XDY_Mot         va_Troncon{n}_in
*     XDY_Octet       va_CCH{n}_in
*     char(200)       va_Commentaire{n}_in
*
* Arguments en sortie : Aucun
*
* Code retour :
*  XDC_OK
*  XDC_NOK            : action inexistante; troncon inexistant; erreur d'ecriture
*  XDC_ARG_INV        : parametre d'entree obligatoire manquant (Null) ou invalide
*  <0                 : erreur sybase
*
* Conditions d'utilisation :
*  La procedure stockee n'existe qu'au CI.
*
* Fonction :
*  Verifier que l'action existe et est de type "Envoi VH".
*  Inserer les donnees additionnelles dans la table ACT_CCH, des lors que
*  la condition de conduite est renseignee (<> NR) ou le commentaire l'est.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC79' and type = 'P')
	drop procedure XZAC79
go


#ifdef CI

create procedure XZAC79
	@va_NumAction_in		int		= null,
	@va_NumSite_in			T_SITE		= null,

	@va_Troncon1_in			smallint	= null,
	@va_CCH1_in			tinyint		= null,
	@va_Commentaire1_in		char(200)	= null,
	@va_Troncon2_in			smallint	= null,
	@va_CCH2_in			tinyint		= null,
	@va_Commentaire2_in		char(200)	= null,
	@va_Troncon3_in			smallint	= null,
	@va_CCH3_in			tinyint		= null,
	@va_Commentaire3_in		char(200)	= null,
	@va_Troncon4_in			smallint	= null,
	@va_CCH4_in			tinyint		= null,
	@va_Commentaire4_in		char(200)	= null,
	@va_Troncon5_in			smallint	= null,
	@va_CCH5_in			tinyint		= null,
	@va_Commentaire5_in		char(200)	= null,
	@va_Troncon6_in			smallint	= null,
	@va_CCH6_in			tinyint		= null,
	@va_Commentaire6_in		char(200)	= null,
	@va_Troncon7_in			smallint	= null,
	@va_CCH7_in			tinyint		= null,
	@va_Commentaire7_in		char(200)	= null,
	@va_Troncon8_in			smallint	= null,
	@va_CCH8_in			tinyint		= null,
	@va_Commentaire8_in		char(200)	= null,
	@va_Troncon9_in			smallint	= null,
	@va_CCH9_in			tinyint		= null,
	@va_Commentaire9_in		char(200)	= null,
	@va_Troncon10_in		smallint	= null,
	@va_CCH10_in			tinyint		= null,
	@va_Commentaire10_in		char(200)	= null
as
	declare	@vl_Site		T_SITE,
		@vl_troncon		smallint,
		@vl_cch			tinyint,
		@vl_commentaire		varchar(200),
		@vl_cr			int

	/*B Compte-rendu par defaut */
	select @vl_cr = XDC_OK


	/*A Transformation des parametres d'entree nuls */
	if @va_NumAction_in = 0	select @va_NumAction_in	= null
	if @va_NumSite_in = 0	select @va_NumSite_in	= null

	/*A Verifier la validite des parametres d'entree */
	if @va_NumAction_in = null or @va_NumSite_in = null
		return XDC_ARG_INV


	/*A Verifier l'existance de l'action */
	if not exists (select * from EXP..ACT_GEN
	                  where numero = @va_NumAction_in  and  sit = @va_NumSite_in
	                    and type = XDC_ACT_CCH)
		return XDC_NOK


	/*A Creer une table temporaire pour la liste des parametres */
	create table #LISTE_CCH (
		troncon		smallint	null,
		cch		tinyint		null,
		commentaire	varchar(200)	null
	)


	/*A Stocker la liste des parametres */
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon1_in, @va_CCH1_in, rtrim (@va_Commentaire1_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon2_in, @va_CCH2_in, rtrim (@va_Commentaire2_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon3_in, @va_CCH3_in, rtrim (@va_Commentaire3_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon4_in, @va_CCH4_in, rtrim (@va_Commentaire4_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon5_in, @va_CCH5_in, rtrim (@va_Commentaire5_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon6_in, @va_CCH6_in, rtrim (@va_Commentaire6_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon7_in, @va_CCH7_in, rtrim (@va_Commentaire7_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon8_in, @va_CCH8_in, rtrim (@va_Commentaire8_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon9_in, @va_CCH9_in, rtrim (@va_Commentaire9_in))
	insert #LISTE_CCH (troncon, cch, commentaire)	values (@va_Troncon10_in, @va_CCH10_in, rtrim (@va_Commentaire10_in))

	/*A Purger les parametres non renseignes */
	delete #LISTE_CCH
	where cch = XZAEC_CCH_NR  and  commentaire is null


	/*A Supprimer les troncons invalides (inconnus) */
	delete #LISTE_CCH
	where troncon not in (select numero from CFG..CCH_TRO)
	if @@rowcount > 0
		select @vl_cr = XDC_NOK


	/*A Supprimer les valeurs de conditions de conduite invalides */
	delete #LISTE_CCH
	where cch not in (XZAEC_CCH_NR, XZAEC_CCH_C1, XZAEC_CCH_C2, XZAEC_CCH_C3, XZAEC_CCH_C4)
	if @@rowcount > 0
		select @vl_cr = XDC_ARG_INV


	/*A Pour chaque declaration de conditions de conduite renseignee */
	declare Pointeur_CCH cursor for
	select troncon, cch, commentaire
	  from #LISTE_CCH
	open Pointeur_CCH
	fetch Pointeur_CCH into @vl_troncon, @vl_cch, @vl_commentaire

	while (@@sqlstatus = 0)
	begin
		/*A Si des conditions existent pour cette action et ce troncon */
		/*A alors les remplacer sinon ajouter les conditions indiquees */
		if exists (select * from EXP..ACT_CCH
		              where actio = @va_NumAction_in  and  sit = @va_NumSite_in
		                and troncon = @vl_troncon)
		begin
			update EXP..ACT_CCH	set cch = @vl_cch, commentaire = @vl_commentaire
			where actio = @va_NumAction_in  and  sit = @va_NumSite_in
			  and  troncon = @vl_troncon
		end
		else
		begin
			insert EXP..ACT_CCH (actio, sit, troncon, cch, commentaire)
			values (@va_NumAction_in, @va_NumSite_in, @vl_troncon, @vl_cch, @vl_commentaire)
		end

		if @@rowcount <> 1
		begin
			if @vl_cr = XDC_OK
				select @vl_cr = XDC_NOK
		end

		fetch Pointeur_CCH into @vl_troncon, @vl_cch, @vl_commentaire
	end
	close Pointeur_CCH

	return @vl_cr

#endif

go
