/*E*/
/*  Fichier : $Id: xzae213sp.prc,v 1.1 2020/11/03 16:37:44 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/11/03 16:37:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae213.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG 14/09/18 creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante fermeture echangeur
* 
* Sequence d'appel
* SP	XZAE213_Rechercher_Fiche_Fermeture_Echangeur
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Octet		va_ech_in
* 
* Arguments en sortie
* XDY_Booleen	va_Entree1_out
* XDY_Booleen	va_Entree2_out
* XDY_Booleen	va_Sortie1_out
* XDY_Booleen	va_Sortie2_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* < 0 erreur sybase
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans la table FERMETURE_ECHANGEUR
* where numero evenement and horodate
----------------------------------------------------*/

use PRC
go

drop procedure XZAE213
go

create procedure XZAE213
	@va_ech_in		tinyint = null,
	@va_Entree1_out		bit = XDC_NON	output,
	@va_Entree2_out		bit = XDC_NON	output,
	@va_Sortie1_out		bit = XDC_NON	output,
	@va_Sortie2_out		bit = XDC_NON	output
as
	declare @vl_num int, @vl_cle tinyint,
	@vl_nb int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_ech_in = 0 select @va_ech_in = null

	/*A test la presence des parametres obligatoires */
	if @va_ech_in = null 
		return XDC_ARG_INV

	/*recherche FMC fermeture echangeur en cours*/
	select  @vl_num = G.numero, @vl_cle = G.cle
	from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
	where G.debut is not null
	and G.fin is NULL
	and G.type = XZAEC_FMC_FermetureEch
	and G.numero = H.numero
	and G.cle = H.cle
	and H.numero_point_caracteristique = @va_ech_in
	and H.point_caracteristique = XDC_POINT_CARACT_ECHANGEUR

	if @@rowcount != 1
	begin
		select 
		@va_Entree1_out = XDC_NON,
		@va_Entree2_out = XDC_NON,
		@va_Sortie1_out = XDC_NON,
		@va_Sortie2_out = XDC_NON

		/*recherche des evts autres que fermeture echangeur*/
		/*d abord sur la bretelle de sortie sens 1*/
		select @vl_nb = count(*)
		from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
		where G.debut is not null
		and G.fin is NULL
		and G.numero = H.numero
		and G.cle = H.cle
		and H.numero_point_caracteristique = @va_ech_in
		and H.point_caracteristique = XDC_POINT_CARACT_ECHANGEUR
		and H.sens = XDC_SENS_SORTIE_SUD
		and G.type in (XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil,
					XZAEC_FMC_Escargot,XZAEC_FMC_VehArrete, XZAEC_FMC_Travaux,
					XZAEC_FMC_VehFeu, XZAEC_FMC_VehPanne, XZAEC_FMC_VehLent)

		if @vl_nb!=0
		begin
			select va_Sortie1_out = XDC_OUI
		end
		else
		begin
			select va_Sortie1_out = XDC_NON
		end

		/*bretelle entree sens 1*/
		select @vl_nb = count(*)
		from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
		where G.debut is not null
		and G.fin is NULL
		and G.numero = H.numero
		and G.cle = H.cle
		and H.numero_point_caracteristique = @va_ech_in
		and H.point_caracteristique = XDC_POINT_CARACT_ECHANGEUR
		and H.sens = XDC_SENS_ENTREE_SUD
		and G.type in (XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil,
					XZAEC_FMC_Escargot,XZAEC_FMC_VehArrete, XZAEC_FMC_Travaux,
					XZAEC_FMC_VehFeu, XZAEC_FMC_VehPanne, XZAEC_FMC_VehLent)

		if @vl_nb!=0
		begin
			select va_Entree1_out = XDC_OUI
		end
		else
		begin
			select va_Entree1_out = XDC_NON
		end


		select @vl_nb = count(*)
		from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
		where G.debut is not null
		and G.fin is NULL
		and G.numero = H.numero
		and G.cle = H.cle
		and H.numero_point_caracteristique = @va_ech_in
		and H.point_caracteristique = XDC_POINT_CARACT_ECHANGEUR
		and H.sens = XDC_SENS_SORTIE_NORD
		and G.type in (XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil,
					XZAEC_FMC_Escargot,XZAEC_FMC_VehArrete,XZAEC_FMC_Travaux,
					XZAEC_FMC_VehFeu, XZAEC_FMC_VehPanne, XZAEC_FMC_VehLent)

		if @vl_nb!=0
		begin
			select va_Sortie2_out = XDC_OUI
		end
		else
		begin
			select va_Sortie2_out = XDC_NON
		end

		select @vl_nb = count(*)
		from EXP..FMC_GEN G, EXP..FMC_HIS_DER H
		where G.debut is not null
		and G.fin is NULL
		and G.numero = H.numero
		and G.cle = H.cle
		and H.numero_point_caracteristique = @va_ech_in
		and H.point_caracteristique = XDC_POINT_CARACT_ECHANGEUR
		and H.sens = XDC_SENS_ENTREE_NORD
		and G.type in (XZAEC_FMC_Cirque, XZAEC_FMC_ConvoiExc, XZAEC_FMC_ConvoiMil,
					XZAEC_FMC_Escargot,XZAEC_FMC_VehArrete,XZAEC_FMC_Travaux,
					XZAEC_FMC_VehFeu, XZAEC_FMC_VehPanne, XZAEC_FMC_VehLent)

		if @vl_nb!=0
		begin
			select va_Entree2_out = XDC_OUI
		end
		else
		begin
			select va_Entree2_out = XDC_NON
		end
	end
	else /*j ai une fermeture echangeur*/
	begin
		select
			@va_Entree1_out		= entree1,
			@va_Entree2_out		= entree2,
			@va_Sortie1_out		= sortie1,
			@va_Sortie2_out		= sortie2
		from	EXP..FMC_ECH
		where	numero = @vl_num and cle = @vl_cle 

		/*A test si les infos ont ete trouvees */
		if @@rowcount != 1
		begin
			select
			@va_Entree1_out		= XDC_NON,
			@va_Entree2_out		= XDC_NON,
			@va_Sortie1_out		= XDC_NON,
			@va_Sortie2_out		= XDC_NON
		end
	end

	return XDC_OK

go
