/*E*/
/*Fichier :  $Id: xzam06sp.prc,v 1.15 2021/03/08 13:46:18 pc2dpdy Exp $      Release : $Revision: 1.15 $        Date : $Date: 2021/03/08 13:46:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Nov 1994	: Creation
* gaborit	04 jan 1995	: modif integration (1.2)
* C.T.   	15 fev 1995	: ajout de demande de travaux 
*                                 en retour (1.3)
* C.T.   	28 fev 1995	: retour de la date de premiere
*                                 apparition tout le temps (1.4)
* C.T.   	29 mar 1995	: Ajout la localisation en sortie (1.5)
* Guilhou	06 mai 1996	: Ajout du nom d'operateur qui a acquitte en sortie (1.6) DEM/919
* Hilmarcher    22 aou 2005     : ajout param tri (par alarme ou designation) DEM498 v1.7
* GTIE    	04 fev 2008     : GTC renovee DEM663 v1.8
* JPL		02/01/2013	: Retour de la date de dernier changement; dates formatees (DEM 1057)  1.9
* JPL		03/01/2013	: Ajout criteres Criticite et Activite; selection prealable des types eqt.  1.10
* JPL		03/01/2013	: Sur serveur HIStorique, execute la procedure au CI pour Fiche Metier (DEM 1057)  1.11
* JMG		29/09/14	: tri dernier changement 1.12
* JMG		29/12/14	: ajout indicateur ALA_IHM 1.13
* PNI		07/09/20	: Ajout site et dégroupage SAE-164 1.14
* CGR		06/01/21	: MOVIS ajout du serveur PRA DEM-SAE93 1.15
* GGY		26/09/23	: Ajout district CA (DEM-483)
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Recherche les alarmes correspondant au critere
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop proc XZAM06
go

create procedure XZAM06
	@va_TypeAlarme_in	smallint	= null,
	@va_Criticite_in	int		= 0,
	@va_Activite_in		char(1)		= XDC_CHAINE_VIDE,
	@va_Eqt_in		T_EQUIPEMENT	= null,
	@va_Tri_in		char(20)	= XDC_CHAINE_VIDE,
        @va_Site_in             tinyint         = null,
	@va_TypeEqt1_in		smallint	= null,
	@va_TypeEqt2_in		smallint	= null,
	@va_TypeEqt3_in		smallint	= null,
	@va_TypeEqt4_in		smallint	= null
as 

/* 
*
* SP	XZAM06SP
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeAlarme	va_TypeAlarme_in
* XDY_Entier            va_Criticite_in		Combinaison des valeurs MINEURE, MAJEURE, CRITIQUE
* char(1)               va_Activite_in		"A"ctives, "I"nactives ou "T" pour "T"ous etats
* XDY_TypeEqt		va_TypeEqt_in		attention s'il vaut XDC_DEBUT_TYPEQT_GTCRENOVEE alors cas "Autre"
* XDY_Eqt		va_Eqt_in
* char(20)              va_Tri_in
* tinyint		va_Site_in : numero du site local
*
* PARAMETRES EN SORTIE :
*
* XZAMT_ListeAla ListeAlarmes
*
* VALEUR RENDUE :
*
* Recherche les alarmes correspondant au critere
*
* CODE RETOUR :
*
*    XDC_OK
*    XDC_ARG_INV :
*       Numero d'equipement demande sans indiquer son type.
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* select from ALM_EQT, EQT_GEN, TYP_ALM
*
* MECANISMES :
*
------------------------------------------------------*/

declare	@vl_Mineure		bit,
		@vl_Majeure		bit,
		@vl_Critique		bit,
		@vl_Active		bit,
		@vl_Tous_Types_Eqt	bit,
		@vl_animation_bouton    bit,
		@vl_type 		smallint,
		@vl_type_equipement 	tinyint,
		@vl_equipement 		smallint,
		@vl_serveur_pra		char(6), 
		@vl_SiteMaitre tinyint

	/* Verification du site */
	select @vl_SiteMaitre = numero
		from CFG..RES_DIS
			where type = XDC_TYPEM_SD

	/*A Transformation des parametres d'entree nuls */

	if  @va_TypeAlarme_in = 0 select @va_TypeAlarme_in = null
	if  @va_Eqt_in = 0 select @va_Eqt_in = null
	if  @va_TypeEqt1_in = 0 select @va_TypeEqt1_in = null
	if  @va_TypeEqt2_in = 0 select @va_TypeEqt2_in = null
	if  @va_TypeEqt3_in = 0 select @va_TypeEqt3_in = null
	if  @va_TypeEqt4_in = 0 select @va_TypeEqt4_in = null


	/*A Verifier la validite des parametres d'entree */
	select @va_Activite_in = upper (@va_Activite_in)
	if @va_Activite_in = "A"
		select @vl_Active = XDC_VRAI
	else if @va_Activite_in = "I"
		select @vl_Active = XDC_FAUX
	else if @va_Activite_in <> "T"
		return ( XDC_ARG_INV )


	select @vl_animation_bouton = XDC_FAUX

	select @va_Tri_in = upper (@va_Tri_in)
	if (@va_Tri_in like "EQUIP%")
		select @va_Tri_in = "EQUIPEMENT"
	else if (@va_Tri_in like "URG%")
		select @va_Tri_in = "URGENCE"
	else
		return ( XDC_ARG_INV )


	if (@va_Eqt_in <> null)  and  (@va_TypeEqt1_in = null)
		return ( XDC_ARG_INV )


#ifdef HIST

	/*A
	** Sur historique, rechercher les alarmes sur le serveur du CI
	** -----------------------------------------------------------
	*/
	
	declare	@vl_Status	int

	/* Init serveur PRA */
	select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=XDC_CI
	
	if @vl_serveur_pra = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAM06
                                     @va_TypeAlarme_in, @va_Criticite_in, @va_Activite_in, @va_Eqt_in, @va_Tri_in,
                                     @va_TypeEqt1_in, @va_TypeEqt2_in, @va_TypeEqt3_in, @va_TypeEqt4_in
	else
		exec @vl_Status = SQL_CI.PRC..XZAM06
	        	             @va_TypeAlarme_in, @va_Criticite_in, @va_Activite_in, @va_Eqt_in, @va_Tri_in,
	                	     @va_TypeEqt1_in, @va_TypeEqt2_in, @va_TypeEqt3_in, @va_TypeEqt4_in

	if @vl_Status = null
		return XDC_PRC_INC
	else return @vl_Status


#else

	/*A Determiner les criteres de criticite */
	select	@vl_Mineure = XDC_FAUX,
		@vl_Majeure = XDC_FAUX,
		@vl_Critique = XDC_FAUX
	if (@va_Criticite_in & XDC_EQT_MINEUR) <> 0	select @vl_Mineure = XDC_VRAI
	if (@va_Criticite_in & XDC_EQT_MAJEUR) <> 0	select @vl_Majeure = XDC_VRAI
	if (@va_Criticite_in & XDC_EQT_CRITIQUE) <> 0	select @vl_Critique = XDC_VRAI



	/*A
	** Construire la liste des types d'equipements demandes
	** ----------------------------------------------------
	*/

	begin
		/*B Initialiser une table des types d'equipements vide */
		select code as "Type_Equipement" into #LST_TYP_EQT_EN_ALARME from CFG..LIB_PRE
		where notyp = XDC_LIB_TYPE_EQUIPEMENT  and  code <> code

		select @vl_Tous_Types_Eqt = XDC_FAUX

		if (@va_TypeEqt1_in = null)
		begin
			select @vl_Tous_Types_Eqt = XDC_VRAI
		end

		else if (@va_TypeEqt1_in = XDC_EQT_NIC)		/*cas de la GTC - on prend les types de la GTC renovee*/
		begin
			insert #LST_TYP_EQT_EN_ALARME select code from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_EQUIPEMENT  and
			      (code in (@va_TypeEqt1_in, @va_TypeEqt2_in, @va_TypeEqt3_in, @va_TypeEqt4_in)  or
			          code >= XDC_DEBUT_TYPEQT_GTCRENOVEE)
		end

		else if (@va_TypeEqt1_in = XDC_DEBUT_TYPEQT_GTCRENOVEE)		/*cas du type Autre*/
		begin
			insert #LST_TYP_EQT_EN_ALARME select code from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_EQUIPEMENT  and
			      code not in
				(XDC_EQT_NAV,XDC_EQT_PMV,XDC_EQT_TUBE,XDC_EQT_DAI,XDC_EQT_ECHANGEUR,XDC_EQT_RAU,XDC_EQT_PAU,
				XDC_EQT_NIC,XDC_EQT_PAN,XDC_EQT_MOD,XDC_EQT_TOR,XDC_EQT_PCT,XDC_EQT_RAD,XDC_EQT_ANA,
				XDC_EQT_PAL,XDC_EQT_FAC,XDC_EQT_FAU)
		end

		else						/*cas general*/
		begin
			insert #LST_TYP_EQT_EN_ALARME select code from CFG..LIB_PRE
			where notyp = XDC_LIB_TYPE_EQUIPEMENT  and
			      code in (@va_TypeEqt1_in, @va_TypeEqt2_in, @va_TypeEqt3_in, @va_TypeEqt4_in)
		end
	end
	--select * from #LST_TYP_EQT_EN_ALARME



	/*A
	** Rechercher la liste des alarmes correspondant aux criteres
	** ----------------------------------------------------------
	*/

	select	type_alarme = ALM.type,
		nom_alarme = TYP.nom,
		TYP.mineure,
		TYP.majeure,
		TYP.critique,
		type_eqt = EQT.type,
		num_eqt = EQT.numero,
		nom_eqt = EQT.nom,
		date_1er_apparition,
		premiere_apparition = convert (char(10), date_1er_apparition, 103) + ' ' +
		                         convert (char(8), date_1er_apparition, 8),
		dernier_changement = convert (char(10), date_dernier_changement, 103) + ' ' +
		                        convert (char(8), date_dernier_changement, 8),
		ALM.nombre_d_apparitions,
		ALM.etat,
		ALM.operateur_acquit,
		ALM.demande_de_travaux,
		EQT.autoroute,
		EQT.PR,
		EQT.sens,
		date_tri = isnull(date_dernier_changement,date_1er_apparition),
		animation_bouton = XDC_FAUX

	into	#LST_ALARMES

	from	CFG..TYP_ALM TYP,
		EXP..ALM_EQT ALM,
		CFG..EQT_GEN EQT

	where	( @va_TypeAlarme_in = null  or  TYP.numero = @va_TypeAlarme_in )
	  and	( (@vl_Critique = XDC_VRAI  and  TYP.critique = XDC_VRAI)  or
	  	  (@vl_Majeure = XDC_VRAI  and  TYP.majeure = XDC_VRAI)  or
	  	  (@vl_Mineure = XDC_VRAI  and  TYP.mineure = XDC_VRAI) )
	  
	  and ( ALM.sit = @va_Site_in  or @va_Site_in = @vl_SiteMaitre )

	  and	ALM.type = TYP.numero
	  and	( (@vl_Tous_Types_Eqt = XDC_VRAI)  or
	     	  ALM.type_equipement in (select Type_Equipement from #LST_TYP_EQT_EN_ALARME) )
	  and	( @va_Eqt_in = null  or  ALM.equipement = @va_Eqt_in )
	  and	( @va_Activite_in = "T"  or  ALM.etat = @vl_Active )

	  and	EQT.type = ALM.type_equipement
	  and	EQT.numero = ALM.equipement


	/* identifier les alarmes critiques qui ont passe le bouton au rouge*/
	declare Pointeur_ihm cursor
	for select type, type_equipement, equipement
	from EXP..ALM_IHM
	open Pointeur_ihm
	fetch Pointeur_ihm into @vl_type, @vl_type_equipement, @vl_equipement


	while (@@sqlstatus=0)
	begin
		update #LST_ALARMES
		set animation_bouton = XDC_VRAI
		where type_eqt = @vl_type_equipement
		and num_eqt = @vl_equipement
		and type_alarme = @vl_type
	fetch Pointeur_ihm into @vl_type, @vl_type_equipement, @vl_equipement
	end
	close Pointeur_ihm

	/*A
	** Retourner les resultats tries dans l'ordre specifie
	** ---------------------------------------------------
	*/

	if (@va_Tri_in = "EQUIPEMENT")				/* tri par designation */
	begin
		select	type_alarme, nom_alarme,
			mineure, majeure, critique,
			type_eqt, num_eqt, nom_eqt,
			premiere_apparition, dernier_changement,
			nombre_d_apparitions, etat,
			demande_de_travaux,
			autoroute, PR, sens,
			operateur_acquit, nom_oper = OPR.nom, animation_bouton

		from	#LST_ALARMES, CFG..OPR_NOM OPR

		where	OPR.numero =* operateur_acquit

		order by autoroute, sens, PR, nom_eqt, type_eqt, date_tri desc
	end
	else if (@va_Tri_in = "URGENCE")			/* tri par urgence */
	begin
		select	type_alarme, nom_alarme,
			mineure, majeure, critique,
			type_eqt, num_eqt, nom_eqt,
			premiere_apparition, dernier_changement,
			nombre_d_apparitions, etat,
			demande_de_travaux,
			autoroute, PR, sens,
			operateur_acquit, nom_oper = OPR.nom, animation_bouton

		from	#LST_ALARMES, CFG..OPR_NOM OPR

		where	OPR.numero =* operateur_acquit

		order by critique desc, majeure desc, mineure desc, date_tri desc
	end

	return ( XDC_OK )
#endif

go  
