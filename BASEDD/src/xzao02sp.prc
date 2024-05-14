/*E*/
/*Fichier :  $Id: xzao02sp.prc,v 1.3 2021/02/16 11:37:14 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2021/02/16 11:37:14 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao02sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture (exploitation) de la configuration du reseau d'autoroutes
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	29/10/08 : Creation 1.1
* JPL	05/11/08 : Gestion mode memorisation et spid pour acces concurrents 1.2
* PNI	16/02/21 : Suppression de la creation de la table RES_JON 1.3
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Construit l'itineraire d'un point d'origine donne sur une autoroute
*  a un point de destination pouvant etre situe sur une autre autoroute.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO02_Itineraire
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute	va_NumAutoOrig_in
* XDY_PR	va_PrOrig_in
* XDY_Autoroute	va_NumAutoDest_in
* XDY_PR	va_PrDest_in
* bit		va_Memorisation_in
*
* PARAMETRES EN SORTIE :
*
* smallint	va_Nb_Etapes_out	nombre d'etapes si destination trouvee
*					0 si destination hors graphe
*
* VALEUR RENDUE :
*
*  Liste des etapes de l'itineraire defini par l'origine et la destination ;
*  Cette liste peut etre conservee dans une table interne et non retournee
*  selon l'indicateur de memorisation fourni en parametre.
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK     : parametre(s) incorrect(s) (localisation hors reseau)
* XDC_ARG_INV : parametre d'entree obligatoire manquant (Null) ou invalide
* <0          : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
*  Cette procedure exploite une table des jonctions d'autoroutes
*  de la configuration du reseau (situee dans la base CFG).
*
*  Cette table est construite lors de la phase de compilation
*  de cette procedure.
*
* FONCTION :
*
*  Si les autoroutes d'origine et de destination sont identiques
*  alors l'itineraire resultat est constitue uniquement de ce trajet ;
*  sinon il est decompose en une liste d'etapes sur une meme autoroute.
*
*  Si le Pr d'origine ou de destination est hors reseau alors
*  une erreur est retournee.
*
-----------------------------------------------------------------------------*/

/*A
** Creer la table des intersections (jonctions) d'autoroutes
** dans la base de configuration
*/

use CFG
go

if exists (select * from sysobjects where name = 'RES_JON' and type = 'U')
	delete RES_JON
go

/**
create table RES_JON (
	autoroute1	T_AUTOROUTE	not null,
	autoroute2	T_AUTOROUTE	not null,
	pr1		T_PR		not null,
	pr2		T_PR		not null,
	portion1	smallint	not null,
	portion2	smallint	not null
)
grant all on RES_JON to public

execute sp_primarykey RES_JON, autoroute1, autoroute2

create unique index RES_JON_PK on RES_JON (autoroute1, autoroute2)
**/



/*A
** Construire l'ensemble des intersections (jonctions) d'autoroutes
** a partir de l'ensemble des noeuds de la configuration du reseau
*/

insert RES_JON (autoroute1, autoroute2, pr1, pr2, portion1, portion2)
select P1.autoroute AUTOROUTE1,
       P2.autoroute AUTOROUTE2,
       PR1 = (select (1 - N.extremite_portion_1) * PR_debut + (N.extremite_portion_1 * PR_fin)
                from RES_POR
               where numero = N.portion1),
       PR2 = (select (1 - N.extremite_portion_2) * PR_debut + (N.extremite_portion_2 * PR_fin)
                from RES_POR
               where numero = N.portion2),
       N.portion1 PORTION1,
       N.portion2 PORTION2
  from RES_NEU N, RES_POR P1, RES_POR P2
 where N.portion1 = P1.numero
   and N.portion2 = P2.numero
   and (P1.autoroute != P2.autoroute)
 group by P1.autoroute, P2.autoroute
 having N.portion1 = max (N.portion1)
    and N.portion2 = max (N.portion2)
 order by P1.autoroute, P2.autoroute

go



/*A
** Creer la table interne des etapes constituant un itineraire
** (dans la base des procedures)
*/

use PRC
go

if exists (select * from sysobjects where name = 'LISTE_ETAPES' and type = 'U')
	drop table LISTE_ETAPES
go

create table LISTE_ETAPES (
	etape			smallint	not null,
	autoroute		T_AUTOROUTE	not null,
	pr_debut		T_PR		not null,
	pr_fin			T_PR		not null,
	vers_autoroute		T_AUTOROUTE	null,
	vers_pr			T_PR		null,
	spid			int		not null
)

grant all on LISTE_ETAPES to public
go



/*A
** Creer la procedure de construction d'un itineraire
*/

if exists (select * from sysobjects where name = 'XZAO02' and type = 'P')
	drop procedure XZAO02
go


create procedure XZAO02
	@va_NumAutoOrig_in	T_AUTOROUTE	= null,
	@va_PrOrig_in		T_PR		= null,
	@va_NumAutoDest_in	T_AUTOROUTE	= null,
	@va_PrDest_in		T_PR		= null,
	@va_Memorisation_in	bit		= 0,
	@va_Nb_Etapes_out	smallint	= 0	output
as

declare
	@vl_Num_Etape		smallint,
	@vl_Autoroute_Debut	T_AUTOROUTE,
	@vl_Autoroute_Fin	T_AUTOROUTE,
	@vl_PR_Debut		T_PR,
	@vl_PR_Jonction_Dest	T_PR,
	@vl_portion		smallint


	/* Valeur par defaut du nombre d'etapes resultat */
	select @va_Nb_Etapes_out = 0


	/*A
	** Supprimer les resultats de toute execution precedente
	** PAR LE MEME PROCESSUS
	*/

	delete LISTE_ETAPES
	 where spid = @@spid


	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumAutoOrig_in = 0	select @va_NumAutoOrig_in = null
	if @va_NumAutoDest_in = 0	select @va_NumAutoDest_in = null


	/*A
	** Verifier la validite des parametres d'entree obligatoires
	*/

	if (@va_NumAutoOrig_in is null  or  @va_NumAutoOrig_in = XDC_AUT_INCONNUE)
	   or (@va_PrOrig_in is null  or  @va_PrOrig_in = XDC_PR_INCONNU)
	   or (@va_NumAutoDest_in is null  or  @va_NumAutoDest_in = XDC_AUT_INCONNUE)
	   or (@va_PrDest_in is null  or  @va_PrDest_in = XDC_PR_INCONNU)
		return XDC_ARG_INV


	/*A
	** Verifier que les localisations d'origine et de destination
	** sont valides (c'est a dire ne sont pas hors reseau)
	*/

	select @vl_portion = numero
	  from CFG..RES_POR
	 where autoroute = @va_NumAutoOrig_in
	   and (PR_debut <= @va_PrOrig_in  and  @va_PrOrig_in <= PR_fin)
	if @@rowcount = 0
		return XDC_NOK

	select @vl_portion = numero
	  from CFG..RES_POR
	 where autoroute = @va_NumAutoDest_in
	   and (PR_debut <= @va_PrDest_in  and  @va_PrDest_in <= PR_fin)
	if @@rowcount = 0
		return XDC_NOK


	/*A
	** Si les autoroutes d'origine et de destination sont identiques
	** alors le parcours de ce trajet constitue l'itineraire complet
	*/

	if (@va_NumAutoOrig_in = @va_NumAutoDest_in)
	begin
		insert LISTE_ETAPES (
			etape,
			autoroute,
			pr_debut,
			pr_fin,
			vers_autoroute,
			vers_pr,
			spid
		)
		values (
			1,
			@va_NumAutoOrig_in,
			@va_PrOrig_in,
			@va_PrDest_in,
			null,
			null,
			@@spid
		)

		select @va_Nb_Etapes_out = 1

		return XDC_OK
	end


	/*A
	** Sinon (l'itineraire doit emprunter plusieurs autoroutes)
	** commencer par chercher tous les trajets possibles en tant que
	** premiere etape sur l'autoroute d'origine (jusqu'a une jonction)
	*/

	insert LISTE_ETAPES (
		etape,
		autoroute,
		pr_debut,
		pr_fin,
		vers_autoroute,
		vers_pr,
		spid
	)
	select	1,
		@va_NumAutoOrig_in,
		@va_PrOrig_in,
		pr1,
		autoroute2,
		pr2,
		@@spid
	  from CFG..RES_JON
	 where autoroute1 = @va_NumAutoOrig_in

	/*B
	** Si l'autoroute d'origine a une jonction vers celle de destination
	** alors l'itineraire est trouve et le nombre d'etapes est connu
	*/

	select @vl_PR_Jonction_Dest = vers_pr from LISTE_ETAPES
	 where vers_autoroute = @va_NumAutoDest_in
	   and spid = @@spid

	if @vl_PR_Jonction_Dest is not null
		select @va_Nb_Etapes_out = 1



	/*A
	** Tant qu'aucune jonction vers l'autoroute de destination n'a ete trouvee,
	** chercher a l'atteindre par jonction a partir d'une etape connue
	*/

	declare Pointeur_Etape cursor for
		select	etape,
			vers_autoroute,
			vers_pr
		  from LISTE_ETAPES
		 where spid = @@spid

	open Pointeur_Etape
	fetch Pointeur_Etape into @vl_Num_Etape,
	                          @vl_Autoroute_Debut,
	                          @vl_PR_Debut

	while (@@sqlstatus = 0) and (@va_Nb_Etapes_out = 0)
	begin
		/*B les etapes suivant celle-ci ont le numero suivant */
		select @vl_Num_Etape = @vl_Num_Etape + 1

		/*B
		** Ajouter a la liste toutes les etapes possibles
		** sur l'autoroute de jonction de cette etape-ci
		** qui ne menent pas a une autoroute debut d'etape
		*/

		insert LISTE_ETAPES (
			etape,
			autoroute,
			pr_debut,
			pr_fin,
			vers_autoroute,
			vers_pr,
			spid
		)
		select	@vl_Num_Etape,
			@vl_Autoroute_Debut,
			@vl_PR_Debut,
			pr1,
			autoroute2,
			pr2,
			@@spid
		  from CFG..RES_JON J
		 where autoroute1 = @vl_Autoroute_Debut
		   and not exists (select * from LISTE_ETAPES
		                    where autoroute = J.autoroute2
		                      and spid = @@spid)

		/*B si l'autoroute de destination est atteinte alors l'itineraire est trouve */
		select @vl_PR_Jonction_Dest = vers_pr from LISTE_ETAPES
		 where vers_autoroute = @va_NumAutoDest_in
		   and spid = @@spid

		if @vl_PR_Jonction_Dest is not null
			select @va_Nb_Etapes_out = @vl_Num_Etape
		else
			fetch Pointeur_Etape into @vl_Num_Etape,
			                          @vl_Autoroute_Debut,
			                          @vl_PR_Debut
	end
	close Pointeur_Etape



	/*A
	** Si l'autoroute de destination n'a pas ete atteinte
	** alors elle n'est pas dans le graphe (dans ce sous-graphe)
	*/

	if (@va_Nb_Etapes_out = 0)
		return XDC_NOK


	/*A
	** sinon supprimer de la liste les etapes qui n'ont pas abouti
	*/

	select @vl_Autoroute_Fin = @va_NumAutoDest_in
	select @vl_Num_Etape = @va_Nb_Etapes_out

	while (@vl_Num_Etape > 0)
	begin
		delete LISTE_ETAPES
		 where etape = @vl_Num_Etape
		   and vers_autoroute <> @vl_Autoroute_Fin
		   and spid = @@spid

		if (@vl_Num_Etape > 1)
			select @vl_Autoroute_Fin = autoroute
			  from LISTE_ETAPES
			 where etape = @vl_Num_Etape
			   and spid = @@spid

		select @vl_Num_Etape = @vl_Num_Etape - 1
	end


	/*A
	** et enfin ajouter la derniere etape sur l'autoroute de destination
	** necessaire pour atteindre le Pr de destination (meme si extremite)
	*/

	select @va_Nb_Etapes_out = @va_Nb_Etapes_out + 1

	insert LISTE_ETAPES (
		etape,
		autoroute,
		pr_debut,
		pr_fin,
		vers_autoroute,
		vers_pr,
		spid
	)
	values (
		@va_Nb_Etapes_out,
		@va_NumAutoDest_in,
		@vl_PR_Jonction_Dest,
		@va_PrDest_in,
		null,
		null,
		@@spid
	)



	/*A
	** Supprimer les resultats de la table interne et les retourner,
	** sauf si le mode memorisation est specifie
	*/

	if @va_Memorisation_in = XDC_FAUX
	begin
		select autoroute, pr_debut, pr_fin
		  from LISTE_ETAPES
		 where spid = @@spid
		 order by etape

		delete LISTE_ETAPES
		 where spid = @@spid
	end

	return XDC_OK
go
