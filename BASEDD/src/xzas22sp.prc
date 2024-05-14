/*E*/
/*  Fichier : $Id: xzas22sp.prc,v 1.7 1999/02/26 11:45:40 gaborit Exp $      Release : $Revision: 1.7 $        Date : $Date: 1999/02/26 11:45:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  29/11/94        : Creation      (V 1.1)
* B.G.  12/09/95	: Correction calcul proportion (1.5)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.6
* PN	05/02/99	: Modif intergest dem/1724 1.7
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Utilisation des ressources (PMV et NAV).
*
* Sequence d'appel
* SP    XZAS22_Utilisation_Ressources
*
* Arguments en entree :
* @va_EqtType_in	: Type d'equipement (PMV : XDC_EQT_PMV = 2
*					  ou NAV : XDC_EQT_NAV = 1) 
*
* Arguments en sortie :
*
* Liste retournee :
* XDY_Nom	nom_fmc		: Nom de l'evenement
* XDY_Entier	nb_util		: Nombre d'utilisations
* XDY_Entier	duree_moy	: Duree moyenne d'utilisation
* XDY_Entier	duree_tot	: Duree totale d'utilisation
* XDY_Entier	proportion	: Proportion par rapport au total des types
* (s'il n'a pas d'evenement, la liste retournee est :
*	XDC_CHAINE_VIDE,0,0,0,0	)
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN           : pas de resultat.
*
* Conditions d'utilisation
* - Avoir des evenements finis pour les equipements concernes
* - Les durees moyenne et totales sont exprimees en minutes 
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;22
	@va_EqtType_in	tinyint		= null
as
	declare @vl_Site	 T_SITE,/* Site concerne */ 
		@vl_NbEnrg	 int,	/* Nombre d'enregistrements */
		@vl_Count        int,   /* Compteur interne */
		@vl_NumEvenement int,	/* Evenemenent main courante */
		@vl_Total_types	 int,	/* Total des durees d'utilisation */
		@vl_NumCle	 tinyint,/* Numero de cle */
		@vl_typeSite	tinyint

	/* Test Argument APPLIX (il n'existe pas d'equipement de type zero) */
	if @va_EqtType_in = 0 select @va_EqtType_in = null

	/*A 
	** Controle des parametres en entree :
	*/
	if @va_EqtType_in = null
		return XDC_ARG_INV

	/*A
	** Test du type d'equipement 
	*/
	if ( @va_EqtType_in != XDC_EQT_PMV )		/* Type PMV */
		and ( @va_EqtType_in != XDC_EQT_NAV )	/* Type NAV */
		/* ------- Type d'equipement non traite -------*/
		return XDC_ARG_INV

	/*A
	** Recherche du site local 
	*/
	select @vl_Site = numero, @vl_typeSite = type
	from CFG..RES_DIS
	where serveur = @@servername

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK

	/* Creation d'une table temporaire utilisee pour le stockage    */
	/* des informations qui seront retournees comme resultat:       */
	/*      - informations concernant le type d'evenement           */
	/*      - les donnees calculees (le nombre d'utilisation,       */
	/*	la duree total, la duree moyenne et la proportion 	*/
	create table #UTIL_RES_RET (	nom_fmc         char(40)	null,
					type_fmc	smallint 	null,
					nb_util		int		null,
					duree_moy	int		null,
					duree_tot	int		null,
					proportion	float		null)


	/* creation d'une table temporaire utilisee pour le stockage    */
	/* des temps par type d'evenement */
	create table #INFOS_TEMP (	evenement 		int	null,
					cle			tinyint	null,
					heure_succes		datetime null,
					heure_fin		datetime null,
					type_fmc		smallint null)

	/*A
	** Recherche des actions lies au site et au type d'equipement 
	*/
	insert #INFOS_TEMP (	evenement,
				cle,
				heure_succes,
				heure_fin)
		select 	evenement,
			cle,
			heure_succes,
			heure_fin
		from EXP..ACT_GEN
		where	sit =	@vl_Site 
		and 	type = 	@va_EqtType_in
		and 	heure_lancement_fin IS NOT NULL
		and	heure_lancement IS NOT NULL
		and     heure_succes IS NOT NULL
		and     heure_fin IS NOT NULL

	/* Nombre d'actions trouves */
	select @vl_NbEnrg = @@rowcount

	/* S'il n'y a pas d'action,  il n'est pas necessaire de continuer */
	/* le traitement.					          */
	if @vl_NbEnrg = 0
	begin
		select XDC_CHAINE_VIDE,0,0,0,0
		return XDC_OK
	end 

	/* Initialisation du compteur */
	select @vl_Count = 0

	/* Traitement enregistrement par enregistrement */
	set rowcount 1

	/*A
	** Recherche du type d'evenement main courante pour chaque action 
	*/
	while (@vl_Count < @vl_NbEnrg)
	begin
		/* Premier enregistrement satisfaisant la condition */
		select @vl_NumEvenement = evenement,
		       @vl_NumCle 	= cle
		from #INFOS_TEMP
		where type_fmc = null

		update #INFOS_TEMP
		set type_fmc = (select type 
				from EXP..FMC_GEN
				where numero  = @vl_NumEvenement
				and cle = @vl_NumCle and
				datex = XDC_FAUX)
		where evenement = @vl_NumEvenement
		and   cle	= @vl_NumCle
		and   type_fmc  = null

		select @vl_Count = @vl_Count + 1
	end	/* Fin recherche du type d'evenement main courante */

	/* Retour en mode normal */
	set rowcount 0

	/*A
	** Calcul du nombre d'utilisation et du temps total
	** d'utilisation pour chaque evenement.
	*/
	insert #UTIL_RES_RET (	type_fmc,
				nb_util,
				duree_tot) 
	   select distinct type_fmc,
		  count(*),
		  sum(datediff(minute,heure_succes,heure_fin))
	   from #INFOS_TEMP
	   group by type_fmc

	/*A
	** Calcul de la duree globale d'utilisation (tout type confondus) 
	*/
	select @vl_Total_types = sum(duree_tot)
	from #UTIL_RES_RET

	/* Mise a jour de la moyenne, de la proportion et du nom de */ 
	/* de l'evenement.                                          */
	/* La proportion n'est pas calculee ni pour des NAV ni pour */
	/* une duree total d'utilisation nulle.			    */
	if (@vl_Total_types > 0) 
		update #UTIL_RES_RET
		set nom_fmc = (select nom from CFG..TYP_FMC
		  		where numero = #UTIL_RES_RET.type_fmc),
		    duree_moy = (#UTIL_RES_RET.duree_tot/#UTIL_RES_RET.nb_util),
		    proportion = ((10000.0*#UTIL_RES_RET.duree_tot)/@vl_Total_types)
	else
		update #UTIL_RES_RET
		set nom_fmc = (select nom from CFG..TYP_FMC
				where numero = #UTIL_RES_RET.type_fmc),
		    duree_moy = (#UTIL_RES_RET.duree_tot/#UTIL_RES_RET.nb_util),
		    proportion = 0

	/*A
	** Envoi du resultat final 
	*/
	select 	nom_fmc,		/* Nom de l'evenement 		*/
		nb_util,		/* Nombre d'utilisations	*/
		duree_moy,		/* Duree moyenne en minutes	*/
		duree_tot,		/* Duree totale	en minutes	*/
		proportion/100.0		/* Proportion    */
	from #UTIL_RES_RET

	/* Destruction de la table temporaire */
	drop table #INFOS_TEMP
	drop table #UTIL_RES_RET

 	return XDC_OK
go
