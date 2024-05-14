/*E*/
/*  Fichier : $Id: xzat26sp.prc,v 1.4 2007/09/28 13:33:18 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2007/09/28 13:33:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat26sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/02/95	: Creation
* C.T.	28/02/95	: Modif test sur local technique (V 1.2)
* C.T.	08/01/98	: Correction erreur de compil. suite a ajout de colonne dans RES_DIS (V1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Initialiser les etats des equipements du synoptique ou
* d'Equest
*
* Sequence d'appel
* SP	XZAT26_Init_Etats_Eqts
* 
* Arguments en entree
* XDY_Octet		va_Type_in
* XDY_Entier		va_Etats_in
* XDY_NomMachine	va_LocalTechnique_in
* 
* Arguments en sortie
* 
* Liste retournee : 
* XDY_Octet		Type,
* XDY_Mot		Equipement,
* XDY_NomMachine	Nom,
* XDY_Entier		Etat,
* char(2)		CodeSite
* 
* Code retour
* XDC_OK
* XDC_NOK	: machine maitre du local technique non trouvee 
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* va_Type_in prend pour valeur XDC_EQT_xxx
* Utiliser les constantes XDC_EQT_HS, XDC_EQT_MINEUR,
* XDC_EQT_CRITIQUE, XDC_EQT_DESACTIVE, XDC_EQT_INHIBE,
* XDC_EQT_MAJEUR pour choisir les etats desires pour
* les equipements (ou non exclusif est utilise pour les etats)
* Les parametres en entree ne sont pas obligatoires
* (0 est equivalent a null en entree)
* 
* Fonction
------------------------------------------------------*/

use PRC
go

create procedure XZAT;26
	@va_Type_in		tinyint = null,
	@va_Etats_in		int = null,
	@va_LocalTechnique_in	T_NOM_MACHINE = null
as
	declare @vl_MachineTrouvee bit, @vl_Eqt T_EQUIPEMENT, 
		@vl_NomEqt T_NOM_MACHINE, @vl_Type tinyint, 
		@vl_NumEqt T_EQUIPEMENT, @vl_TypeEqt tinyint,
		@vl_Maitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_NomMaitre T_NOM_MACHINE, @vl_Site T_SITE,
		@vl_Inhibe bit, @vl_Majeure bit, @vl_Mineure bit,
		@vl_Critique bit, @vl_HS bit, @vl_Desactive bit

	if @va_Type_in = 0 select @va_Type_in = null 
	if @va_Etats_in = 0 select @va_Etats_in = null 
	if @va_LocalTechnique_in = XDC_CHAINE_VIDE select @va_LocalTechnique_in = null 

	/*A Positionner les variables locales des etats des equipements */
	/*A servant de criteres de selection                            */
	if @va_Etats_in is not null
	begin
		select	@vl_Inhibe = @va_Etats_in & XDC_EQT_INHIBE,
			@vl_Majeure = @va_Etats_in & XDC_EQT_MAJEUR,
			@vl_Critique = @va_Etats_in & XDC_EQT_CRITIQUE,
			@vl_Desactive = @va_Etats_in & XDC_EQT_DESACTIVE,
			@vl_Mineure = @va_Etats_in & XDC_EQT_MINEUR
	end

	/*A Rechercher les equipements respectant le critere */
	/*A de selection d'etat d'equipement */
	select
		CFG..EQT_GEN.type,
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.maitre,
		CFG..EQT_GEN.type_maitre,
		EXP..EQT_DSP.critique,
		EXP..EQT_DSP.majeure,
		EXP..EQT_DSP.mineure,
		EXP..EQT_DSP.HS,
		EXP..EQT_DSP.inhibe,
		EXP..EQT_DSP.desactive,
		EXP..EQT_DSP.sit
	into #LISTE_EQT
	from CFG..EQT_GEN, EXP..EQT_DSP
	where	(@va_Type_in is null or CFG..EQT_GEN.type = @va_Type_in) and
		EXP..EQT_DSP.dernier = 1 and
		EXP..EQT_DSP.equipement = CFG..EQT_GEN.numero and
		EXP..EQT_DSP.type = CFG..EQT_GEN.type and
		(@va_Etats_in is null or 
		 (EXP..EQT_DSP.critique = @vl_Critique or
		  EXP..EQT_DSP.majeure = @vl_Majeure or
		  EXP..EQT_DSP.mineure = @vl_Mineure or
		  EXP..EQT_DSP.HS = @vl_HS or
		  EXP..EQT_DSP.inhibe = @vl_Inhibe or
		  EXP..EQT_DSP.desactive = @vl_Desactive))

	/*A Rechercher les equipements appartenant au local technique */
	/*A dans la liste des equipements respectant le critere de    */
	/*A selection precedent                                       */
	if @va_LocalTechnique_in is not null
	begin
		/* table de stockage */
		create table #EQT_LT (	type tinyint, numero smallint, 
					nom char(10), critique bit,
					majeure bit, mineure bit, 
					HS bit, inhibe bit, 
					desactive bit, sit tinyint)

		declare Pointeur_Eqt cursor
		for select
			type,
			numero,
			nom,
			maitre,
			type_maitre,
			critique,
			majeure,
			mineure,
			HS,
			inhibe,
			desactive,
			sit
		from #LISTE_EQT
		where type != XDC_EQT_MAC and maitre is not null

		open Pointeur_Eqt
		fetch Pointeur_Eqt into @vl_Type, @vl_Eqt, @vl_NomEqt,
				@vl_Maitre, @vl_TypeMaitre, @vl_Critique,
				@vl_Majeure, @vl_Mineure, @vl_HS,
				@vl_Inhibe, @vl_Desactive, @vl_Site

		while @@sqlstatus = 0
		begin
			/*A initialisation des variables pour la recherche */
			/*A machine pilotant l'equipement                  */
			select  @vl_NumEqt = @vl_Maitre,
				@vl_TypeEqt = @vl_TypeMaitre,
				@vl_MachineTrouvee = XDC_NON

			/*B Rechercher le local technique auquel appartient */
			/*B l'equipement                                    */
			while @vl_MachineTrouvee = XDC_NON
			begin
				if @vl_TypeMaitre != XDC_EQT_MAC
				begin
					select
						@vl_Maitre = maitre,
						@vl_TypeMaitre = type_maitre
					from CFG..EQT_GEN
					where numero = @vl_NumEqt and type = @vl_TypeEqt
					if @@rowcount != 1
					begin	
						return XDC_NOK
					end
				end

				/* nom de la machine maitre */
				select @vl_NomMaitre = nom
				from CFG..EQT_GEN
				where numero = @vl_Maitre and type = @vl_TypeMaitre
				if @@rowcount != 1
				begin
					return XDC_NOK
				end

				/*B test si l'equipement n'a plus de maitre */
				if @vl_TypeMaitre = XDC_EQT_MAC
					select @vl_MachineTrouvee = XDC_OUI
				else select	@vl_NumEqt = @vl_Maitre,
						@vl_TypeEqt = @vl_TypeMaitre
			end

			if @vl_NomMaitre = @va_LocalTechnique_in
			begin
				insert #EQT_LT (type, numero, nom, critique,
				majeure, mineure, HS, inhibe, desactive, sit)
				values (@vl_Type, @vl_Eqt, @vl_NomEqt,
				@vl_Critique, @vl_Majeure, @vl_Mineure,
				@vl_HS, @vl_Inhibe, @vl_Desactive, @vl_Site)
			end

			/*B passage a l'equipement suivant */
			fetch Pointeur_Eqt into @vl_Type, @vl_Eqt, 
				@vl_NomEqt, @vl_Maitre, @vl_TypeMaitre, 
				@vl_Critique, @vl_Majeure, @vl_Mineure, 
				@vl_HS, @vl_Inhibe, @vl_Desactive, @vl_Site
		end

		/*A Etat de l'equipement pilotant tous ces equipements */
		if @va_Type_in is null or @va_Type_in = XDC_EQT_MAC
		begin
			insert #EQT_LT (type, numero, nom, critique,
			majeure, mineure, HS, inhibe, desactive, sit)
			select
				type,
				numero,
				nom,
				critique,
				majeure,
				mineure,
				HS,
				inhibe,
				desactive,
				sit
			from #LISTE_EQT
			where nom = @va_LocalTechnique_in and type = XDC_EQT_MAC
		end

		/*A Retour de la liste des equipements avec leur etats */
		select #EQT_LT.type, #EQT_LT.numero, #EQT_LT.nom,
			majeure * XDC_EQT_MAJEUR + mineure * XDC_EQT_MINEUR +
				HS * XDC_EQT_HS + inhibe * XDC_EQT_INHIBE +
				desactive * XDC_EQT_DESACTIVE + 
				critique * XDC_EQT_CRITIQUE,
			code
		from #EQT_LT, CFG..RES_DIS
		where CFG..RES_DIS.numero = #EQT_LT.sit
	end
	else
		/*A Retour de la liste des equipements avec leur etats */
		select #LISTE_EQT.type, #LISTE_EQT.numero, #LISTE_EQT.nom,
			majeure * XDC_EQT_MAJEUR + mineure * XDC_EQT_MINEUR +
				HS * XDC_EQT_HS + inhibe * XDC_EQT_INHIBE +
				desactive * XDC_EQT_DESACTIVE + 
				critique * XDC_EQT_CRITIQUE,
			code
		from #LISTE_EQT, CFG..RES_DIS
		where CFG..RES_DIS.numero = #LISTE_EQT.sit

	return XDC_OK

go
