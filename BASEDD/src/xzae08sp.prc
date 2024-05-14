/*E*/
/* Fichier : $Id: xzae08sp.prc,v 1.32 2020/11/03 16:11:26 pc2dpdy Exp $        $Revision: 1.32 $        $Date: 2020/11/03 16:11:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/09/94	: Creation	(V 1.1)
* C.T.	24/10/94	: Modif codage	(V 1.2)
*                         au niveau des valeurs possibles
*                         pour la machine operateur
*                         (cf conditions d'utilisation)
*                         pas reteste
* C.T.	10/11/94	: Modif bugs applix	(V 1.4)
*                         non teste
* C.T.	16/11/94        : Modif creation numero avec clause
*                         sur la cle et non le site (V 1.5)
*                         non teste
* C.T.	09/02/95        : Modif creation : zz value (V 1.6)
* C.T.	28/02/95        : Modif interface : date en plus (V 1.7)
* P.V.  29/07/96	: maj numero fiche vehicule (DEM76) (V1.8)
* P.V.  12/09/96	: FMC vehicule => suppr num_veh=0 (V1.9)
* C.T.  02/09/96	: modif procedure pour PC simpl (V1.10)
* C.T.  25/09/96	: Ajout degats domaine dans fmc_gen (FMC10) (V1.11)
* C.T.  04/11/96	: Ajout fausse_alerte dans fmc_gen (FMC18) (V1.12)
* JMG	04/12/96	: ne plus donner automatiquement de numero d'appel (DEM/76) 1.13
* JMG	11/02/97	: neutralisations de voies dans la FMC travaux (DEM/1394) 1.14
* CT	09/12/97	: gestion fmc de type degrade (PCniveau2) 1.15
* PN	05/02/99	: modif intergest dem/1724 1.16
* CHI   28/09/05        : rajout ClasseNAture DEM487 v1.17
* JPL	15/05/07	: Ajout fmc Vehicule en marche arriere (DEM 643) 1.18
* JPL	05/06/07	: Renommmage; annul. modif. v1.18; ajout fmc Basculement pour travaux (DEM 647) 1.19
* LCL   09/07/07        : Suppression DY 1.20
* JPL	08/04/08	: Degats domaine values pour FMC Contresens et Marche Arriere (pour retypage) (DEM 770) 1.21
* JMG	03/12/08	: ajout validation dans FMC_ACC 1.22 DEM/852
* JMG           02/10/09        : secto DEM887 1.23
* VR	05/10/11	: Ajout vitesse et vitesse_opposee 1.24(DEM/1015)
* JMG	15/04/12 	: regul dem 1016 1.25
* JPL	03/09/12	: Insertion dans la table FMC_INC pour la classe Incendie (DEM 1037)  1.26
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.28
* JMG	13/06/18	: ajout numero alerte 1.29
* JPL	15/03/19	: Classe Manifestation historis�e: plus de ligne �crite dans FMC_MNF (DEM 1327) 1.30
* JPL	05/09/19	: Ajout du champ code_nature dans la table FMC_INC (DEM 1352)  1.31
* LCL	11/03/20	: R�cup�ration du sitepass� en parametre MOVIS DEM-SAE93 1.32
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cree une fiche main courante
* 
* Sequence d'appel
* SP	XZAE08_Creer_Fiche_MC
* 
* Arguments en entree
* XDY_Octet	va_TypeEvenement_in
* XDY_Machine	va_MachineOperateur_in
* XDY_Booleen	va_Prevision_ATraiter_in
* XDY_Horodate	va_HoroDebut_in
* XDY_Horodate	va_HoroDebutPrevu_in
* XDY_Mot	va_Site_creation_in
* XDY_Mot	va_Degrade_in
* 
* Arguments en sortie
* XDY_Entier	va_NumEvenement_out
* XDY_Octet	va_CleEvenement_out
* XDY_Entier	va_NumFicheAppel_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: site local incorrect, insertion impossible, 
*                 type inconnu
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'evenement est a traiter, la machine operateur doit
* etre valuee 
* Si l'evenement n'est a traiter, la machine operateur doit
* etre a null (etat en prevision) 
* 
* Fonction
* Inserer dans la table EVENEMENT en indiquant le type 
* d'evenement et le site deduit local.
* Inserer dans la table de complement d'evenement en adequation 
* avec le type d'evenement.
* La fonction retourne le numero et la cle de l'evenement.
* Si l'evenement est a traiter, Insert dans la table a traiter
------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE08' and type = 'P')
	drop procedure XZAE08
go


create procedure XZAE08
			@va_TypeEvenement_in		smallint = null,
			@va_MachineOperateur_in		T_EQUIPEMENT = null,
			@va_Prevision_ATraiter_in	bit,
			@va_HoroDebut_in		datetime = null,
			@va_HoroDebutPrevu_in		datetime = null,
			@va_Site_creation_in		tinyint = null,
			@va_Degrade_in			tinyint = null,
			@va_NumEvenement_out		int 	= null	output,
			@va_CleEvenement_out		tinyint = null	output,
			@va_NumFicheAppel_out		int = null	output,
			@va_NumAlerte_in		int = null,
			@va_SiteAlerte_in		tinyint = null
as
	declare @vl_Site T_SITE, @vl_Classe tinyint, @vl_Erreur bit, 
		@vl_code_appel int, @vl_date_raz datetime, @vl_Degats tinyint

	/*! transfo en param�tres nulls bug applix */
	if @va_Site_creation_in = 0 select @va_Site_creation_in = null
	if @va_TypeEvenement_in = 0 select @va_TypeEvenement_in = null
	if @va_MachineOperateur_in = 0 select @va_MachineOperateur_in = null
	if @va_NumAlerte_in = 0 select @va_NumAlerte_in = null
	if @va_SiteAlerte_in = 0 select @va_SiteAlerte_in = null
	if @va_HoroDebut_in = XDC_DATE_NULLE select @va_HoroDebut_in = null
	if @va_HoroDebutPrevu_in = XDC_DATE_NULLE select @va_HoroDebutPrevu_in = null

	/*A initialisation de la variable d'erreur */
	select @vl_Erreur = XDC_NON

	/*A Controle des parametres obligatoires en entree */
	if @va_TypeEvenement_in = null or 
	   (@va_HoroDebut_in = null and @va_HoroDebutPrevu_in = null) or
	   (@va_HoroDebut_in is not null and @va_HoroDebutPrevu_in is not null) or
	 /*  (@va_Prevision_ATraiter_in = XDC_OUI and @va_MachineOperateur_in = null) or*/
	   (@va_Prevision_ATraiter_in = XDC_NON and @va_MachineOperateur_in != null)
		return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
/* 	Modif suite a pb de creation de FMC sur base CI */	
	if @va_Site_creation_in = null 
           begin 		
	      select @vl_Site = numero 
	      from CFG..RES_DIS
	      where serveur = @@servername
           end           
        else
	
 	   select @vl_Site = @va_Site_creation_in

	if @vl_Site is null
		return XDC_NOK

	/*A valuation de la cle de l'evenement a creer */
	select @va_CleEvenement_out = @vl_Site

	/*A recherche la classe de la fiche main courante a creer */
	select @vl_Classe = classe from CFG..TYP_FMC
	where numero = @va_TypeEvenement_in

	if @@rowcount != 1
		return XDC_NOK

	/*A calcul du numero de la fiche main courante a creer */
	select @va_NumEvenement_out = isnull(max(numero), 0) + 1 from EXP..FMC_GEN
	where cle = @vl_Site

	/*A creation de la fiche main courante et de son complement */
	begin tran
		/*B Valuer les degats a 0 si fmc concernee pas ce champ sinon null */
		if	@va_TypeEvenement_in = XZAEC_FMC_Accident or
			@va_TypeEvenement_in = XZAEC_FMC_Agression or
			@va_TypeEvenement_in = XZAEC_FMC_AltercationUsager or
			@va_TypeEvenement_in = XZAEC_FMC_AnimalErrant or
			@va_TypeEvenement_in = XZAEC_FMC_Autre or
			@va_TypeEvenement_in = XZAEC_FMC_ChausseDeter or
			@va_TypeEvenement_in = XZAEC_FMC_ConvoiExc or
			@va_TypeEvenement_in = XZAEC_FMC_Eboulement or
			@va_TypeEvenement_in = XZAEC_FMC_FlaqueHuile or
			@va_TypeEvenement_in = XZAEC_FMC_GreveESCOTA or
			@va_TypeEvenement_in = XZAEC_FMC_HoldUpVol or
			@va_TypeEvenement_in = XZAEC_FMC_Incendie or
			@va_TypeEvenement_in = XZAEC_FMC_IncSsConces or
			@va_TypeEvenement_in = XZAEC_FMC_Inondation or
			@va_TypeEvenement_in = XZAEC_FMC_Manifestation or
			@va_TypeEvenement_in = XZAEC_FMC_Obstacle or
			@va_TypeEvenement_in = XZAEC_FMC_Meteo or
			@va_TypeEvenement_in = XZAEC_FMC_Contresens or
			@va_TypeEvenement_in = XZAEC_FMC_VehArrete or
			@va_TypeEvenement_in = XZAEC_FMC_VehPanne or
			@va_TypeEvenement_in = XZAEC_FMC_VehFeu or
			@va_TypeEvenement_in = XZAEC_FMC_VehMarcheArr
			select @vl_Degats = 0
		else	select @vl_Degats = null

		/*B insertion de la partie generique */
		/*! ne pas inserer de chaine null    */
		insert EXP..FMC_GEN (numero, cle, type, sit, poste_enrichisseur,
			origine_creation, debut, debut_prevu, degats_domaine, fausse_alerte, zz, degrade,datex, alerte, sit_alerte)
		values ( @va_NumEvenement_out, @va_CleEvenement_out, 
			@va_TypeEvenement_in, @vl_Site,
			@va_MachineOperateur_in, XDC_CHAINE_VIDE, 
			@va_HoroDebut_in, @va_HoroDebutPrevu_in, @vl_Degats, XDC_FAUX, 1, @va_Degrade_in,XDC_FAUX,  @va_NumAlerte_in, @va_SiteAlerte_in)

		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI

#ifdef PC_SIMPL
		if @vl_Classe in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav)
			insert EXP..FMC_TRF (numero, cle, sit, nature, continu, mobile, zz )
			values ( @va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, 0, 0, 0, 1)
		else if @vl_Classe = XZAEC_ClassePosteOper
			insert EXP..FMC_OPR (numero, cle, sit)
			values (@va_NumEvenement_out, @va_CleEvenement_out, @vl_Site)

		/*A test de l'insertion */
		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
	commit tran

	if @vl_Erreur = XDC_OUI
		return XDC_NOK

#else
		/*B insertion de la partie specifique */
		/*! ne pas inserer de chaine null    */
		if @vl_Classe = XZAEC_ClasseAccident
			insert EXP..FMC_ACC (numero, cle, sit, personnalites, 
					personnel_ESCOTA, degats_domaine, 
					vehicule_ESCOTA, matieres_dangereuses, zz,validation)
			values (@va_NumEvenement_out, @va_CleEvenement_out, 
				@vl_Site, XDC_NON, XDC_NON,
				XDC_NON, XDC_NON, XDC_NON, 1, getdate())
		else if @vl_Classe = XZAEC_ClasseRegulation
			insert EXP..FMC_REG (numero, cle, sit, horodate,indicateur_horaire)
			values (@va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, dateadd(minute, -1,@va_HoroDebut_in), XDC_NON)
			
		else if @vl_Classe = XZAEC_ClasseDelestage
			insert EXP..FMC_DEL (numero, cle, sit, responsable, zz)
			values (@va_NumEvenement_out, @va_CleEvenement_out, 
				@vl_Site, XDC_CHAINE_VIDE, 1)
		else if @vl_Classe = XZAEC_ClasseVehicule
		    begin


			/*A mise a jour du numero de fiche d'appel */
/*			update EXP..LST_CPT set valeur=@va_NumFicheAppel_out 
			where code=@vl_code_appel
*/
			select @va_NumFicheAppel_out=null

			insert EXP..FMC_VEH (numero, cle, sit, numero_vehicule, 
					bebe, personne_agee, animaux_vivants, 
					vehicule_ESCOTA, matieres_dangereuses,
					commentaire, zz,numero_fiche_appel,site_creation)
			values ( @va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, 1,
				XDC_NON, XDC_NON, XDC_NON, XDC_NON,
				XDC_CHAINE_VIDE, XDC_CHAINE_VIDE, 1,@va_NumFicheAppel_out,
				@va_Site_creation_in )
		    end
		else if @vl_Classe = XZAEC_ClasseEchangeur
			insert EXP..FMC_ECH (numero, cle, sit, entree1, entree2,
					sortie1, sortie2, zz)
			 values( @va_NumEvenement_out, @va_CleEvenement_out, 
				@vl_Site, 0, 0, 0, 0, 1)
		else if @vl_Classe = XZAEC_ClasseSousConcess
			insert EXP..FMC_CNC (numero, cle, sit, nom_ss_concessionnaire,
					ferme, penurie, zz)
			values ( @va_NumEvenement_out, @va_CleEvenement_out, 
				@vl_Site, XDC_CHAINE_VIDE,  0, 0, 1)
		else if @vl_Classe = XZAEC_ClasseTravaux
			insert EXP..FMC_TRF (numero, cle, sit, nature, continu, mobile, 
						VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, zz)
			values ( @va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1)
		else if @vl_Classe = XZAEC_ClasseBasculement
			insert EXP..FMC_BAS (numero, cle, sit, VR, VM2, VM1, VL, BAU,
					VR_I, VM2_I, VM1_I, VL_I, BAU_I, zz, vitesse, vitesse_opposee)
			values (@va_NumEvenement_out, @va_CleEvenement_out, 
				@vl_Site, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, null, null)
		else if @vl_Classe = XZAEC_ClasseBasculTrav
		    begin
			insert EXP..FMC_TRF (numero, cle, sit, nature, continu, mobile, 
						VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I, zz)
			values ( @va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, 0, 0, 0,
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1)
			if @@rowcount != 1
				select @vl_Erreur = XDC_OUI
			insert EXP..FMC_BAS (numero, cle, sit, VR, VM2, VM1, VL, BAU,
					VR_I, VM2_I, VM1_I, VL_I, BAU_I, zz)
			values (@va_NumEvenement_out, @va_CleEvenement_out, 
				@vl_Site, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1)
		    end
		else if @vl_Classe = XZAEC_ClassePosteOper
			insert EXP..FMC_OPR (numero, cle, sit)
			values (@va_NumEvenement_out, @va_CleEvenement_out, @vl_Site)
		else if @vl_Classe = XZAEC_ClasseIncendie
			insert EXP..FMC_INC (numero, cle, sit, zz,
			                     feu_maitrise, horodate_maitrise, code_nature)
			values (@va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, 1,
				XDC_FAUX, null, null)
		else if @vl_Classe != XZAEC_ClasseBouchon and
			@vl_Classe != XZAEC_ClasseVehLent and
			@vl_Classe != XZAEC_ClasseMeteo and
			@vl_Classe != XZAEC_ClasseNature and
			@vl_Classe != XZAEC_ClasseManif and
			@vl_Classe != XZAEC_ClasseGenerique
			select @vl_Erreur = XDC_OUI
		else	select @vl_Erreur = XDC_NON

		/*A test de l'insertion */
		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
	commit tran

	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	/*A si evenement est a traiter inserer dans la table a traiter */
	if @va_Prevision_ATraiter_in = XDC_OUI
	begin
		insert EXP..FMC_TRT (evenement, cle, sit, zz)
		values (@va_NumEvenement_out, @va_CleEvenement_out, @vl_Site, 1)

		if @@rowcount != 1
			return XDC_NOK
	end
#endif

	return XDC_OK
go

