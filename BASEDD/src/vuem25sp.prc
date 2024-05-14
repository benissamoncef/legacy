/*E*/
/*  Fichier : $Id: vuem25sp.prc,v 1.6 2009/06/25 17:38:12 gesconf Exp $        Release : $Revision: 1.6 $        Date : $Date: 2009/06/25 17:38:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fiche metier dont l'objet est de connaitre la liste
* des evenements qui ont ete generes par des alertes.
* La proc renvoit aussi le nombre d'alertes de type :
* PAU, DAI, VIDEO, RADIO et AUTRES
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	12/03/1997	: Creation		V1.0
* L.V.	14/04/1997	: Correction Horodate=heure de 
*			   debut de l'evt 
*			+ ajout d'une colonne type 
*			   final de l'alerte	V1.1
* L.V.	23/05/1997	: Correction remplacement XDC_EQT_DAI
*			par XDC_EQT_ANA		V1.2
* P.N.	05/02/99	: Modif interg. dem/1724 v1.5
* JPL	22/06/09	: Extraire les donnees bouchons des fiches tete et queue (DEM 723) 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM25
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* int		va_PR_debut_in
* int		va_PR_fin_in
* tinyint	va_Autoroute_in
* tinyint	va_Sens_in
* int		va_Type_evt_in
*
* Arguments en sortie
* int	Nb alertes PAU
* int	Nb alertes DAI
* int	Nb alertes VIDEO
* int	Nb alertes RADIO
* int	Nb alertes AUTRE
* 
* Service rendu
* Liste les evts qui ont genere une alerte pou une zone et une periode donnees
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree incorrects
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'VUEM25' and type = 'P')
	drop procedure VUEM25
go


create procedure VUEM25
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_PR_debut_in		int		= null,
	@va_PR_fin_in		int		= null,
	@va_Autoroute_in	tinyint		= null,
	@va_Sens_in		tinyint		= null,
	@va_Type_evt_in		int		= null,
	@va_NbPAU_out		int		output,
	@va_NbDAI_out		int		output,
	@va_NbVIDEO_out		int		output,
	@va_NbRADIO_out		int		output,
	@va_NbAUTRE_out		int		output
as
	declare
		@vl_NbAlertes	int,
		@vl_type_final	char(5)



/*A controle des parametres d'entree */
if @va_Debut_in is null
	return XDC_ARG_INV
		
if @va_Fin_in is null
	return XDC_ARG_INV
		
if @va_PR_debut_in is null
	return XDC_ARG_INV
		
if @va_PR_fin_in is null
	return XDC_ARG_INV	
	
if @va_Autoroute_in is null
	return XDC_ARG_INV
		
if @va_Sens_in is null
	return XDC_ARG_INV
	
if @va_Type_evt_in is null
	return XDC_ARG_INV

/*A Est-ce une requete sur tous les evenements ? */
if @va_Type_evt_in = 0 select @va_Type_evt_in = null


select @vl_type_final = null
	
					 
/*A Creation de la table temporaire contenant la liste de tous les */
/*A evenements  (sauf les bouchons !!) pour la periode et la zone donnees. */
/*A De plus l'origine de l'evt doit etre une alerte */
	
select
	numero = HIS..FMC_GEN.numero,
	cle = HIS..FMC_GEN.cle,
	type_evt = CFG..TYP_FMC.nom,
	horodate = HIS..FMC_GEN.debut,
	alerte = HIS..FMC_GEN.alerte,
	origine = HIS..FMC_GEN.origine_creation,
	PR = HIS..FMC_HIS.PR,
	sens = HIS..FMC_HIS.sens,
	autoroute = CFG..RES_AUT.nom,
	final = @vl_type_final
into #LISTE_EVTS
from HIS..FMC_GEN, HIS..FMC_HIS, CFG..TYP_FMC, CFG..RES_AUT
where
	/* condition sur le type des evts */
	(HIS..FMC_GEN.type = @va_Type_evt_in
	or @va_Type_evt_in is null) and
	HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
	HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
	HIS..FMC_GEN.datex<>XDC_VRAI and
	/* condition sur les horodates */
	HIS..FMC_GEN.debut >= @va_Debut_in and
	HIS..FMC_GEN.debut <= @va_Fin_in and
	/* condition sur les alertes */
	HIS..FMC_GEN.fausse_alerte = XDC_FAUX and
	(HIS..FMC_GEN.alerte is not null or ltrim(HIS..FMC_GEN.origine_creation) is not null) and
	/* jointure obtenant le nom du type FMC */
	HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
	/* jointure entre FMC_GEN et FMC_HIS */
	HIS..FMC_HIS.numero=HIS..FMC_GEN.numero and
	HIS..FMC_HIS.cle=HIS..FMC_GEN.cle and
	/* condition sur les PR, le sens et l'autoroute */
	HIS..FMC_HIS.autoroute = @va_Autoroute_in and
	HIS..FMC_HIS.PR >=  @va_PR_debut_in and
	HIS..FMC_HIS.PR <= @va_PR_fin_in and
	HIS..FMC_HIS.sens = @va_Sens_in and
	/* jointure entre RES_AUT et FMC_HIS */
	HIS..FMC_HIS.autoroute = CFG..RES_AUT.numero
group by
	HIS..FMC_HIS.numero,
	HIS..FMC_HIS.cle,
	HIS..FMC_GEN.numero,
	HIS..FMC_GEN.cle
having
	horodate_validation = max(horodate_validation) and
	HIS..FMC_HIS.autoroute = @va_Autoroute_in and
	HIS..FMC_HIS.PR >= @va_PR_debut_in and
	HIS..FMC_HIS.PR <= @va_PR_fin_in and
	HIS..FMC_HIS.sens = @va_Sens_in and
	HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
	HIS..FMC_GEN.datex<>XDC_VRAI and
	HIS..FMC_HIS.autoroute = CFG..RES_AUT.numero 


/*A Si les evenements de type Bouchon sont recherches, les inserer dans la table temporaire */
/*A Pour un bouchon le PR considere est celui correspondant a la 1ere validation de la tete */

if (@va_Type_evt_in is null or @va_Type_evt_in = XZAEC_FMC_TeteBouchon or @va_Type_evt_in = XZAEC_FMC_QueueBouchon)
begin
	
	insert #LISTE_EVTS(numero,cle,type_evt,horodate,alerte,origine,PR,sens,autoroute,final)
	select
		FMC_Q.numero,
		FMC_Q.cle,
		CFG..TYP_FMC.nom,
		FMC_Q.debut,
		FMC_Q.alerte,
		FMC_Q.origine_creation,
		HIS..FMC_HIS.PR,
		HIS..FMC_HIS.sens,
		CFG..RES_AUT.nom,
		@vl_type_final
	from HIS..FMC_GEN FMC_Q, /*HIS..FMC_GEN FMC_T,*/ HIS..FMC_HIS, CFG..TYP_FMC, CFG..RES_AUT
	where
		/* condition sur le type des evts */
		FMC_Q.type = XZAEC_FMC_QueueBouchon and
		FMC_Q.datex <> XDC_VRAI and
		/* condition sur les horodates */
		FMC_Q.debut >= @va_Debut_in and
		FMC_Q.debut <= @va_Fin_in and
		/* condition sur les alertes */
		FMC_Q.fausse_alerte = XDC_FAUX and
		(FMC_Q.alerte is not null or ltrim(FMC_Q.origine_creation) is not null) and
		/* jointure obtenant le nom du type FMC */
		FMC_Q.type = CFG..TYP_FMC.numero and
		/* jointure entre FMC_GEN et FMC_HIS */
		FMC_Q.cause = HIS..FMC_HIS.numero and
		FMC_Q.cle_cause = HIS..FMC_HIS.cle and
		/* condition sur les PR, le sens et l'autoroute */
		HIS..FMC_HIS.autoroute = @va_Autoroute_in and
		HIS..FMC_HIS.PR >=  @va_PR_debut_in and
		HIS..FMC_HIS.PR <= @va_PR_fin_in and
		HIS..FMC_HIS.sens = @va_Sens_in and 
		/* jointure entre RES_AUT et FMC_HIS */
		HIS..FMC_HIS.autoroute = CFG..RES_AUT.numero
	group by
		HIS..FMC_HIS.numero,
		HIS..FMC_HIS.cle,
		FMC_Q.numero,
		FMC_Q.cle
	having
		horodate_validation = min(horodate_validation) and
		HIS..FMC_HIS.autoroute = @va_Autoroute_in and
		HIS..FMC_HIS.PR >= @va_PR_debut_in and
		HIS..FMC_HIS.PR <= @va_PR_fin_in and
		HIS..FMC_HIS.sens = @va_Sens_in and
		FMC_Q.type = CFG..TYP_FMC.numero and
		FMC_Q.datex <> XDC_VRAI and
		HIS..FMC_HIS.autoroute = CFG..RES_AUT.numero 

end	/* fin d'insertion des evts bouchons */	



/*A On retourne le libelle de l'eqt si l'alerte n'est nulle dans le champ origine */
/*A Ainsi le contenu du champ origine de la table #LISTE_EVTS est le suivant :		*/
/*A Si l'alerte est generee par un eqt c'est le libelle du type d'eqt 			*/
/*A Sinon si l'alerte est renseignee par le champ origine_creation c'est ce champ	*/

update #LISTE_EVTS 
set #LISTE_EVTS.origine = CFG..LIB_PRE.libelle
from #LISTE_EVTS, CFG..LIB_PRE, HIS..ALT_EXP
where #LISTE_EVTS.alerte is not null and
	CFG..LIB_PRE.notyp = TYP_EQT and
	HIS..ALT_EXP.numero = #LISTE_EVTS.alerte and
	HIS..ALT_EXP.sit = #LISTE_EVTS.cle and
	CFG..LIB_PRE.code = HIS..ALT_EXP.type_equipement


/*A On update le type final de l'alerte */

update #LISTE_EVTS
set #LISTE_EVTS.final = "PAU"
from #LISTE_EVTS, HIS..ALT_EXP
where
	HIS..ALT_EXP.numero = #LISTE_EVTS.alerte and
	HIS..ALT_EXP.sit = #LISTE_EVTS.cle and
	HIS..ALT_EXP.type_equipement = XDC_EQT_PAU
	
update #LISTE_EVTS
set #LISTE_EVTS.final = "DAI"
from #LISTE_EVTS, HIS..ALT_EXP
where
	HIS..ALT_EXP.numero = #LISTE_EVTS.alerte and
	HIS..ALT_EXP.sit = #LISTE_EVTS.cle and
	HIS..ALT_EXP.type_equipement = XDC_EQT_ANA

update #LISTE_EVTS
set #LISTE_EVTS.final = "VIDEO"
where ltrim(#LISTE_EVTS.origine) = "VIDEO"

update #LISTE_EVTS
set #LISTE_EVTS.final = "RADIO"
where
	ascii(#LISTE_EVTS.origine) >= ascii("0") and
	ascii(#LISTE_EVTS.origine) <= ascii("9")

update #LISTE_EVTS
set #LISTE_EVTS.final = "AUTRE"
where #LISTE_EVTS.final = null
	

/*A calcul du nombre d'alertes PAU, DAI, VIDEO, RADIO, AUTRES */
/*A Une alerte est PAU si l'eqt qui l'a generee est un PAU */
/*A Une alerte est DAI si l'eqt qui l'a generee est une DAI */
/*A Une alerte est VIDEO si le champ origine est exactement la chaine VIDEO */
/*A Une alerte est RADIO si le champ origine est numerique */
/*A Une alerte est AUTRE sinon */ 

select @va_NbPAU_out = count(*)
from #LISTE_EVTS, HIS..ALT_EXP
where
	HIS..ALT_EXP.numero = #LISTE_EVTS.alerte and
	HIS..ALT_EXP.sit = #LISTE_EVTS.cle and
	HIS..ALT_EXP.type_equipement = XDC_EQT_PAU
	

select @va_NbDAI_out = count(*)
from #LISTE_EVTS, HIS..ALT_EXP
where
	HIS..ALT_EXP.numero = #LISTE_EVTS.alerte and
	HIS..ALT_EXP.sit = #LISTE_EVTS.cle and
	HIS..ALT_EXP.type_equipement = XDC_EQT_ANA
	

select @va_NbVIDEO_out = count(*)
from #LISTE_EVTS
where ltrim(#LISTE_EVTS.origine) = "VIDEO"


select @va_NbRADIO_out = count(*)
from #LISTE_EVTS
where 
	ascii(#LISTE_EVTS.origine) >= ascii("0") and
	ascii(#LISTE_EVTS.origine) <= ascii("9")

select @vl_NbAlertes = count(*) from #LISTE_EVTS

select @va_NbAUTRE_out = @vl_NbAlertes-@va_NbRADIO_out-@va_NbVIDEO_out-@va_NbDAI_out-@va_NbPAU_out


/*A retourner la liste */
	
select
	horodate=convert(char(10),horodate,103)+' '+convert(char(8),horodate,8),
	numero,
	cle,
	type_evt,
	origine = rtrim(origine),
	PR,
	sens,
	autoroute,
	final
from #LISTE_EVTS

go
