/*E*/
/*  Fichier : $Id: xzag11sp.prc,v 1.21 2021/05/04 13:29:13 pc2dpdy Exp $      Release : $Revision: 1.21 $        Date : $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/10/94	: Creation	(V 1.1)
* C.T.	16/11/94	: Correction erreur sur complement action
*                         (rajout le sit dans la clause) (V 1.2)
*                         non teste
* C.T.	15/06/95	: Ajout des alertes liees a un evt (V 1.3)
* C.T.	10/08/95	: Modif interface (V 1.4)
* C.T.	02/02/98	: Ajouter la purge sur les actions PAL, FAC, FAU (1538) (V1.5)
* PN	04/09/98	: Separartion EXP/HIS dem / 1696 1.7
* P.N.	27/01/99	: Ajouter les actions sur ACT_EXP ACT_EXPTFM ACT_PMVDAT  et FMC_DAT dem/1724 (V1.8)
* CHI   19/07/05        : Ajout purge table FMC_NAT (Nature de l'Obstacle) DEM487 v1.9
* JPL	12/04/07	: Purge FMC_ACC ET FMC_VEH pour Accident et Vehicule au cas ou FMC retypee (DEM 637) v1.10
* JPL	08/06/07	: Purge FMC_TRF, FMC_TRH et FMC_BAS pour Basculements pour travaux (DEM 647) v1.11
* AAZ   28/08/2007  : Purge action domaine DEM 663
* JBL   Mai 2008	: DEM 663 Ajout GTC_ECH
* JMG	22/09/11	: ajout ALT_LOC DEM 979
* VR	12/12/11	: Ajout PRV (DEM/1016)
* JPL	13/09/12	: Prise en compte table FMC_INC pour la classe Incendie (DEM 1037)  1.16
* JPL	07/11/12	: Purge FMC_MET_CCH pour classe Meteo et ACT_CCH pour classe Operateur (DEM 1032)  1.17
* JMG	22/08/13	: purge FMC_REG 1.18
* LCL	02/03/18	: ajout controleurs de feux CTRL_FEUX 1.19 DEM1284
* ABE	17/12/20	: ajout IMU DEM-SAE155 1.20
* ABE	07/04/20	: Ajout PAU SONO DEM-SAE244 1.21
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge les donnees situees la base historique locales relatives 
* a un evenement
* 
* Sequence d'appel
* SP	XZAG11_Purge_Evenement_historise
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in	: numero d'evenement
* XDY_Octet		va_CleEvt_in	: cle d'evenement
* XDY_Octet		vl_ClasseEvt	: classe de l'evenement
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* Prodecure appelee en interne uniquement et n'existant qu'au CI
* 
* Fonction
* Suppression les actions, l'alerte relatif a l'evenement. 
* et l'evenement 
-------------------------------------------------------*/

use PRC
go

#ifdef HIST

create procedure XZAG;11
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Resultat_out	bit 	= XDC_NON output
as
	declare @vl_Action int, @vl_Site T_SITE, @vl_NumEvt int,
		@vl_CleEvt tinyint, @vl_TypeAction tinyint,
		@vl_ClasseEvt tinyint

	/*A controle du parametre en entree */
	if @va_CleEvt_in = null or @va_NumEvt_in = null
		return XDC_ARG_INV

	/*A rechercher la classe et le site de l'evenement */
	select 
		@vl_ClasseEvt = classe
	from HIS..FMC_GEN, CFG..TYP_FMC
	where	HIS..FMC_GEN.numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type

	/*A l'evenement a deja ete purge */
	if @@rowcount = 0
	begin
		/* indiquer que l'evenement n'a pas ete efface */
		select @va_Resultat_out = XDC_NON
		return XDC_OK
	end

	/*A rechercher la liste des actions liees a l'evenement */
	select
		numero,
		type,
		sit
	into #ACT_SUPP
	from HIS..ACT_GEN 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	declare Pointeur_actions cursor
	for select numero, type, sit
	from #ACT_SUPP

	/*A Effacer toutes les actions liees a l'evenement */
	open Pointeur_actions
	fetch Pointeur_actions into @vl_Action, @vl_TypeAction, @vl_Site
	while @@sqlstatus = 0
	begin
		/* supprimer le complement de l'action selectionnee */
		if @vl_TypeAction = XDC_ACT_APPEL or 
		   @vl_TypeAction = XDC_ACT_FAX or
		   @vl_TypeAction = XDC_ACT_BIP 
		begin
			delete HIS..ACT_APL 
			where	actio = @vl_Action and sit = @vl_Site

			/* supprimer le texte du fax */
			if @vl_TypeAction = XDC_ACT_FAX
				delete HIS..ACT_FAX
				where actio = @vl_Action and sit = @vl_Site
		end
		else if @vl_TypeAction = XDC_ACT_CCH
			delete HIS..ACT_CCH 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_NAV or @vl_TypeAction = XDC_ACT_FAU or @vl_TypeAction = XDC_ACT_FAC
			delete HIS..ACT_NAV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_ECHANGEUR
			delete HIS..ACT_ECH 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_TUNNEL
			delete HIS..ACT_TUB 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_DEPANNEUR
			delete HIS..ACT_DEP 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_AFF_GARE
			delete HIS..ACT_GAR 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PAT_SIS
			delete HIS..ACT_SIS 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PAL
			delete HIS..ACT_PAL 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_EXP
			delete HIS..ACT_EXP_ADA 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_EXPTFM
			delete HIS..ACT_TFM_EXP 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMVDAT
			delete HIS..ACT_PMV_ADA 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV
			delete HIS..ACT_PMV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PRV
			delete HIS..ACT_PRV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PIC
			delete HIS..ACT_PIC
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_CFE
			delete HIS..ACT_CFE 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_IMU 
			delete HIS..ACT_IMU 
			where actio = @vl_Action and sit = @vl_Site 				
		else if @vl_TypeAction = XDC_ACT_PAU
			delete HIS..ACT_PAU
			where actio = @vl_Action and sit = @vl_Site 	
		else if @vl_TypeAction = XDC_ACT_PMV_REMORQUE
			delete HIS..ACT_RMQ 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV_FOURGON
			delete HIS..ACT_FRG 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_INTERVENTION
			delete HIS..ACT_INT 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_TFM
		begin
			delete HIS..ACT_TFM 
			where actio = @vl_Action and sit = @vl_Site

			/* supprimer le texte de TFM */
			delete HIS..ACT_TXT_TFM
			where actio = @vl_Action and sit = @vl_Site
		end
		else if @vl_TypeAction = XDC_ACT_GTC_SIG
			delete HIS..ACT_GTC_SIG
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_GTC_SIG_ECH
			delete HIS..ACT_GTC_SIG
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_GTC_EXP
			delete HIS..ACT_GTC_EXP
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_GTC_ECL
			delete HIS..ACT_GTC_ECL
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_GTC_VEN
			delete HIS..ACT_GTC_VEN
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_GTC_ENR
			delete HIS..ACT_GTC_ENR
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_GTC_INC
			delete HIS..ACT_GTC_INC
			where actio = @vl_Action and sit = @vl_Site
	
		/* passage a l'action suivante selectionnee */
		fetch Pointeur_actions into @vl_Action, @vl_TypeAction, @vl_Site
	end
	/* fermer le curseur */
	close Pointeur_actions

	/* suppression de la partie generique de toutes les actions */
	delete HIS..ACT_GEN 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	/*A Effacer le complement de l'evenement et le complement */
	if (@vl_ClasseEvt = XZAEC_ClasseAccident)  or  (@vl_ClasseEvt = XZAEC_ClasseVehicule)
	begin
		delete HIS..FMC_ACC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 

		delete HIS..FMC_VEH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end
	else if @vl_ClasseEvt = XZAEC_ClasseDelestage
		delete HIS..FMC_DEL 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseEchangeur
		delete HIS..FMC_ECH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseSousConcess
		delete HIS..FMC_CNC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseManif
		delete HIS..FMC_MNF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseTravaux
	begin
		delete HIS..FMC_TRF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
		delete HIS..FMC_TRH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end
	else if @vl_ClasseEvt = XZAEC_ClasseBasculement
		delete HIS..FMC_BAS 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	else if @vl_ClasseEvt = XZAEC_ClasseBasculTrav
	begin
		delete HIS..FMC_TRF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
		delete HIS..FMC_TRH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
		delete HIS..FMC_BAS 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	end
	else if @vl_ClasseEvt = XZAEC_ClasseVehLent
		delete HIS..FMC_VLN 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseMeteo
	begin
		delete HIS..FMC_MET 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
		delete HIS..FMC_MET_CCH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end
	else if @vl_ClasseEvt = XZAEC_ClasseRegulation
		delete HIS..FMC_REG
		where   numero = @va_NumEvt_in and
			 cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseNature
		delete HIS..FMC_NAT
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseIncendie
		delete HIS..FMC_INC
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseBouchon
		delete HIS..FMC_BOU 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClassePosteOper
		delete HIS..FMC_OPR 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

	/*A Effacer l'historique de l'evenement */
	delete HIS..FMC_HIS 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in

	/*A Effacer les commentaires de l'evenement */
	delete HIS..FMC_COM 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in

	/*A Effacer les attribut datex de l'evenement */
	delete HIS..FMC_DAT 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in

	/*A Effacer l'alerte pointant sur l'evenement */
	delete HIS..ALT_EXP 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in 

	/*A Effacer l'alerte pointee par l'evenement */
	delete HIS..ALT_EXP 
	where	numero in (select alerte from HIS..FMC_GEN
			   where numero = @va_NumEvt_in and
				cle = @va_CleEvt_in  ) and
		sit = @va_CleEvt_in 

	delete HIS..ALT_LOC
	where numero not in (select numero from HIS..ALT_EXP)

	/*A Effacement de la partie generique de l'evenement */
	delete HIS..FMC_GEN
	where	numero = @va_NumEvt_in and cle = @va_CleEvt_in

	/* indiquer que l'evenement a ete efface */
	select @va_Resultat_out = XDC_OUI

	return XDC_OK
go

#endif
