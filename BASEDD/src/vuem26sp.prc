/*E*/
/*  Fichier : $Id: vuem26sp.prc,v 1.4 2010/09/29 17:05:22 gesconf Exp $        Release : $Revision: 1.4 $        Date : $Date: 2010/09/29 17:05:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem26_AnalysePAUouDAIsp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fiche metier dont l'objet est de connaitre la liste
* des evenements lies a une alerte PAU ou DAI
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	14/03/1997	: Creation V1.0
* P.N.	05/02/99	: Modif interg. dem/1724 v1.3
* JPL	29/09/10	: Adaptation compilateur C (# en debut de ligne) 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM26
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* int		va_PR_debut_in
* int		va_PR_fin_in
* tinyint	va_Autoroute_in
* tinyint	va_Sens_in
* int		va_Num_evt_in
* tinyint	va_Type_alerte_in (XDC_EQT_PAU = 14 et XDC_EQT_DAI = 4)
*
* Arguments en sortie
* int	Nb vehicules en panne
* int	Nb accidents
* int	Nb vehicules arretes
* int	Nb bouchons
* int	Nb autre evt
* + Liste des evtsqui ont genere l'alerte 
*
* Service rendu
* Liste les evts qui ont genere une alerte PAU ou DAI pour 
* une zone et une periode donnees
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

create procedure VUEM26
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_PR_debut_in		int		= null,
	@va_PR_fin_in		int		= null,
	@va_Autoroute_in	tinyint		= null,
	@va_Sens_in		tinyint		= null,
	@va_Num_evt_in		int		= null,
	@va_PAU_ou_DAI_in	tinyint		= null,
	@va_NbPanne_out		int		output,
	@va_NbAccident_out	int		output,
	@va_NbArrete_out	int		output,
	@va_NbBouchon_out	int		output,
	@va_NbAutre_out		int		output
as
	declare
		@vl_NbEvts	int,
		@vl_PR_eqt	int



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
	
if @va_Num_evt_in is null
	return XDC_ARG_INV

if @va_PAU_ou_DAI_in is null
	return XDC_ARG_INV


/*A Est-ce une requete sur tous les evenements ? */
if @va_Num_evt_in = 0 select @va_Num_evt_in = null

/* initialisation du PR des eqts a null */
select @vl_PR_eqt = null	
					 
/*A Creation de la table temporaire contenant la liste de tous les */
/*A evenements  (sauf les bouchons !!) pour la periode et la zone donnees. */
/*A De plus l'origine de l'evt doit etre une alerte du type demande (PAU ou DAI) */
	
select
	numero = HIS..FMC_GEN.numero,
	cle = HIS..FMC_GEN.cle,
	num_type_evt = HIS..FMC_GEN.type,
	type_evt = CFG..TYP_FMC.nom,
	debut = HIS..FMC_GEN.debut,
	fin = HIS..FMC_GEN.fin,
	type_equipement = HIS..ALT_EXP.type_equipement,
	num_eqt = HIS..ALT_EXP.equipement,
	PR_eqt = @vl_PR_eqt,
	PR_evt = HIS..FMC_HIS.PR,
	sens = HIS..FMC_HIS.sens,
	autoroute = CFG..RES_AUT.nom
into #LISTE_EVTS
from HIS..FMC_GEN, HIS..FMC_HIS, CFG..TYP_FMC, CFG..RES_AUT,HIS..ALT_EXP
where
	/* condition sur le type des evts */
	(HIS..FMC_GEN.type = @va_Num_evt_in
	or @va_Num_evt_in is null) and
	HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
	HIS..FMC_GEN.type != XZAEC_FMC_QueueBouchon and
	HIS..FMC_GEN.datex<>XDC_VRAI and
	/* condition sur les horodates */
	HIS..FMC_GEN.debut >= @va_Debut_in and
	HIS..FMC_GEN.debut <= @va_Fin_in and
	/* condition sur les alertes */
	HIS..FMC_GEN.fausse_alerte = XDC_FAUX and
	/* jointure entre FMC_HIS et TYP_FMC */
	HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
	/* jointure entre FMC_GEN et ALT_EXP */
	HIS..FMC_GEN.alerte = HIS..ALT_EXP.numero and
	HIS..FMC_GEN.cle = HIS..ALT_EXP.sit and
	/* condition sur le type d'eqt generant l'alerte */
	HIS..ALT_EXP.type_equipement = @va_PAU_ou_DAI_in and	
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
	HIS..FMC_HIS.autoroute = CFG..RES_AUT.numero and
	HIS..FMC_GEN.alerte = HIS..ALT_EXP.numero and
	HIS..FMC_GEN.cle = HIS..ALT_EXP.sit
	
	
/*A Si on veut aussi les evts de type bouchons => les inserer dans la table tempo */
/*A #LISTE_EVTS. Car pour un bouchon on prend son PR a la 1ere validation de la tete */

if (@va_Num_evt_in is null or @va_Num_evt_in = XZAEC_FMC_TeteBouchon or @va_Num_evt_in = XZAEC_FMC_QueueBouchon)
begin
	
	insert #LISTE_EVTS(numero,cle,num_type_evt,type_evt,debut,fin,type_equipement,num_eqt,PR_eqt,PR_evt,sens,autoroute)
	select
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		HIS..FMC_GEN.type,
		CFG..TYP_FMC.nom,
		HIS..FMC_GEN.debut,
		HIS..FMC_GEN.fin,
		HIS..ALT_EXP.type_equipement,
		HIS..ALT_EXP.equipement,
		@vl_PR_eqt,
		HIS..FMC_HIS.PR,
		HIS..FMC_HIS.sens,
		CFG..RES_AUT.nom
	from HIS..FMC_GEN, HIS..FMC_HIS, CFG..TYP_FMC, CFG..RES_AUT,HIS..ALT_EXP
	where
		/* condition sur le type des evts */
		HIS..FMC_GEN.type = XZAEC_FMC_TeteBouchon and
		HIS..FMC_GEN.datex<>XDC_VRAI and
		/* condition sur les horodates */
		HIS..FMC_GEN.debut >= @va_Debut_in and
		HIS..FMC_GEN.debut <= @va_Fin_in and
		/* condition sur les alertes */
		HIS..FMC_GEN.fausse_alerte = XDC_FAUX and
		/* jointure entre FMC_HIS et TYP_FMC */
		HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
		/* jointure entre FMC_GEN et ALT_EXP */
		HIS..FMC_GEN.alerte = HIS..ALT_EXP.numero and
		HIS..FMC_GEN.cle = HIS..ALT_EXP.sit and
		/* condition sur le type d'eqt generant l'alerte */
		HIS..ALT_EXP.type_equipement = @va_PAU_ou_DAI_in and	
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
		horodate_validation = min(horodate_validation) and
		HIS..FMC_HIS.autoroute = @va_Autoroute_in and
		HIS..FMC_HIS.PR >= @va_PR_debut_in and
		HIS..FMC_HIS.PR <= @va_PR_fin_in and
		HIS..FMC_HIS.sens = @va_Sens_in and
		HIS..FMC_GEN.type = CFG..TYP_FMC.numero and
		HIS..FMC_GEN.datex<>XDC_VRAI and
		HIS..FMC_HIS.autoroute = CFG..RES_AUT.numero and
		HIS..FMC_GEN.alerte = HIS..ALT_EXP.numero and
		HIS..FMC_GEN.cle = HIS..ALT_EXP.sit
		
			
end	/* fin d'insertion des evts bouchons */	



/*A Mise a jour du PR de l'equipement */

update #LISTE_EVTS
set #LISTE_EVTS.PR_eqt = CFG..EQT_GEN.PR
from #LISTE_EVTS, CFG..EQT_GEN
where
	type_equipement = CFG..EQT_GEN.type and
	num_eqt = CFG..EQT_GEN.numero


/*A calcul du nombre d'evts VEH PANNE, VEH ARRETE,ACCIDENTS,BOUCHONS et AUTRE */

select @va_NbPanne_out = count(*)
from #LISTE_EVTS
where
	num_type_evt = XZAEC_FMC_VehPanne
	

select @va_NbAccident_out = count(*)
from #LISTE_EVTS
where
	num_type_evt = XZAEC_FMC_Accident
	
select @va_NbArrete_out = count(*)
from #LISTE_EVTS
where
	num_type_evt = XZAEC_FMC_VehArrete

select @va_NbBouchon_out = count(*)
from #LISTE_EVTS
where
	num_type_evt = XZAEC_FMC_TeteBouchon


select @vl_NbEvts = count(*) from #LISTE_EVTS

select @va_NbAutre_out = @vl_NbEvts-@va_NbArrete_out-@va_NbAccident_out-@va_NbPanne_out-@va_NbBouchon_out


/*A retourner la liste */
	
select
	debut=convert(char(10),debut,103)+' '+convert(char(8),debut,8),
	fin=convert(char(10),fin,103)+' '+convert(char(8),fin,8),
	numero,
	type_evt,
	PR_evt,
	sens,
	autoroute,
	num_eqt,
	PR_eqt
from #LISTE_EVTS

go
