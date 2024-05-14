/*E*/
/*  Fichier : $Id: xzag10sp.prc,v 1.36 2021/05/04 13:29:13 pc2dpdy Exp $      Release : $Revision: 1.36 $        Date : $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/10/94	: Creation	(V 1.1)
* C.T.	16/11/94	: La derniere action creee en local
*                         n'est pas purgee (V 1.2)
*                         non teste
* C.T.	20/06/95	: Modif interface (V 1.3)
* C.T.	10/08/95	: Ajout init code retour (V 1.4)
* C.T.	23/08/96	: passage par var intermediaire pour eviter deadlock (V 1.5)
* C.T.	03/09/96	: Ajout cas de purge sur pc simplifie (V 1.6)
* C.T.	06/01/98	: Ajout cas de purge dans FMC_TRS (1531) (V 1.7)
* C.T.	12/01/98	: Ajout cas de purge dans ALT_EXP pour pc3 (1532) (V 1.8)
* C.T.	02/02/98	: Ajouter la purge sur les actions PAL, FAC, FAU (1538) (V1.9)
* P.N.	27/01/99	: Ajouter les actions sur ACT_EXP ACT_EXPTFM ACT_PMVDAT et FMC_DAT 
				+ ADA_SIT +  ADA_TXT_SYN dem/1724 (V1.10)
* P.N.  16/06/99	: Correction de la purge de FMC_TFM v1.12
* P.N.	02/09/99	: delete complet FMC_REV a chaque purge v1.13
* JMG	17/01/01	: delete de FMC_HIS_DER 1.14
* PCL	06/02/03	: FMC_HIS_DER doit etre purgee sur tous les sites (sauf PC N3) 1.15
* C.H.  18/03/03        : insertion des numeros d'actions a supprimer dans table EXP..PURGE_10
*                         puis update de la valeur=2 avant le delete ds table ACT_GEN
*                         puis update de la valeur=3 avant le delete ds table FMC_GEN
* C.H.  25/03/03        : Suppression des traces et insert dans table PURGE_10 v1.16
* CHI   19/07/05        : Ajout purge table FMC_NAT (Nature de l'Obstacle) v1.17
* JPL	12/04/07	: Purge FMC_ACC ET FMC_VEH pour Accident et Vehicule au cas ou FMC retypee (DEM 637) v1.18
* JPL	08/06/07	: Correction bug pour classe travaux : recherche complement en table FMC_TRH
*			  Purge table basculement et tables travaux pour Basculement pour Travaux (DEM 647) v1.19
* JPL	26/07/07	: Correction pour Basculement pour Travaux : table basculement non purgee  1.20
* AAZ   28/08/2007  : Ajout action domaine DEM 663
* JBM	Mai 2008	: DEM 663 ajout GTC_ECH
* LCL	20/07/11	: Ajout troncature num telephone client 4 digits 1.23
 * JMG	22/09/11	: ajout ALT_LOC 1.24
* PNI	01/12/11	: correction dans la troncature num telephone client 4 digits (zz=zz+1) 1.25
* JMG	05/04/12	 : ajout FMC_COMMU 1.26
* VR	12/12/11	: Ajout PRV (DEM/1016)
* JPL	13/09/12	: Prise en compte table FMC_INC pour la classe Incendie (DEM 1037)  1.28
* PNI	26/09/12	: delete de FMC_HIS_DER (supprimer de l'xzag15) DEM1052 1.29
* JPL	07/11/12	: Purge FMC_MET_CCH pour classe Meteo et ACT_CCH pour classe Operateur (DEM 1032)  1.30
* JMG	20/08/13 	: ajout FMC_REG 1.31
* PNI	01/04/2015	: ajout ACT_PMA ACT_BAF ACT_BAD DEM1122 1.32
* PNI	14/12/2017	: tronquer l'immat apres un mois DEM 1260 1.33
* LCL	02/03/18	: ajout controleurs de feux CTRL_FEUX 1.34 DEM1284
* ABE	17/12/20	: ajout IMU DEM-SAE155 1.35
* ABE	07/04/21	: ajout PAU SONO DEM-SAE 244 1.36
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge les donnees situees sur le site local relatives 
* a un evenement
* 
* Sequence d'appel
* SP	XZAG10_Purge_Evenement
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in	: numero d'evenement
* XDY_Octet		va_CleEvt_in	: cle d'evenement
* 
* Arguments en sortie
* XDY_Booleen		va_Resultat_out : XDC_OUI si evenement supprime
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* sur PC non simplifie :
* Suppression les actions, les SAD, l'alerte situes sur le site
* local relatif a l'evenement.
* Suppression de l'evenement si aucun lien persiste avec
* l'evenement et s'il situe sur le site local
*
* sur PC simplifie :
* Suppression les actions liees a une fmc operateur
* local relatif a l'evenement. 
* Suppression de l'evenement operateur ou travaux sur le site local
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;10
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Resultat_out	bit 	= XDC_NON output
as
	declare @vl_Site T_SITE, @vl_NbActions int,
		@vl_NbHisto int, @vl_NbAlerte int,
		@vl_NbComplement int, @vl_Action int,
		@vl_NbCommentaires int,
		@vl_Alerte int,
		@vl_Plan smallint, @vl_NumEvt int,
		@vl_CleEvt tinyint, @vl_TypeAction tinyint,
		@vl_SiteEvt T_SITE, @vl_ClasseEvt tinyint

	/*A controle du parametre en entree */
	if @va_CleEvt_in = null or @va_NumEvt_in = null 
		return XDC_ARG_INV
	
	/* init a l'evenement n'a pas ete efface */
	select @va_Resultat_out = XDC_NON

	/* recherche le site local */
	select @vl_Site = numero 
	from CFG..RES_DIS
	where serveur = @@servername

	if @vl_Site is null
		return XDC_NOK

	/*A rechercher la classe et le site de l'evenement */
	select 
		@vl_Alerte = alerte,
		@vl_ClasseEvt = classe,
		@vl_SiteEvt = sit
	from EXP..FMC_GEN, CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvt_in and cle = @va_CleEvt_in and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	/*A l'evenement a deja ete purge */
	if @@rowcount = 0
#ifdef PC_SIMPL
		select @vl_ClasseEvt = null
#else
	begin
		return XDC_OK
	end
#endif

	/*A initialisation des variables internes */
	select @vl_NbComplement = 0

	/*A rechercher la liste des actions sur le site local liees a */
	/*A l'evenement sauf la derniere creee sur le site            */
	select
		numero,
		type
	into #ACT_SUPP
	from EXP..ACT_GEN 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in and
		sit = @vl_Site and
		numero not in ( select max(numero) from EXP..ACT_GEN
				where sit = @vl_Site)

	declare Pointeur_actions_sup cursor
	for select numero, type
	from #ACT_SUPP

	/*A Effacer toutes les actions sur le site local liees a */
	/*A l'evenement                                          */
	open Pointeur_actions_sup
	fetch Pointeur_actions_sup into @vl_Action, @vl_TypeAction
	while @@sqlstatus = 0
	begin
		/* supprimer le complement de l'action selectionnee */
		if @vl_TypeAction = XDC_ACT_NAV or @vl_TypeAction = XDC_ACT_FAC or @vl_TypeAction = XDC_ACT_FAU
			delete EXP..ACT_NAV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV
			delete EXP..ACT_PMV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PRV
			delete EXP..ACT_PRV 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PIC
			delete EXP..ACT_PIC 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PAL
			delete EXP..ACT_PAL 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_BAF
			delete EXP..ACT_BAF
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_BAD
			delete EXP..ACT_BAD
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMVA
			delete EXP..ACT_PMA
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_CFE
			delete EXP..ACT_CFE
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_IMU 
			delete EXP..ACT_IMU 
			where actio = @vl_Action and sit = @vl_Site 	
		else if @vl_TypeAction = XDC_ACT_PAU 
			delete EXP..ACT_PAU
			where actio = @vl_Action and sit = @vl_Site 					

#ifndef PC_SIMPL
		else if @vl_TypeAction = XDC_ACT_APPEL or 
		   @vl_TypeAction = XDC_ACT_FAX or
		   @vl_TypeAction = XDC_ACT_BIP 
		begin
			if @vl_TypeAction = XDC_ACT_APPEL 
			begin
				update EXP..ACT_APL
				set numero_telephonique = substring(numero_telephonique,1,4) 
				where actio = @vl_Action and sit = @vl_Site and type_d_astreinte = XDC_AST_CLIENT
			end
			
			delete EXP..ACT_APL 
			where	actio = @vl_Action and sit = @vl_Site

			/* supprimer le texte du fax */
			if @vl_TypeAction = XDC_ACT_FAX
				delete EXP..ACT_FAX
				where actio = @vl_Action and sit = @vl_Site
		end
		else if @vl_TypeAction = XDC_ACT_CCH
			delete EXP..ACT_CCH 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_ECHANGEUR
			delete EXP..ACT_ECH 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_TUNNEL
			delete EXP..ACT_TUB 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_DEPANNEUR
			delete EXP..ACT_DEP 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_AFF_GARE
			delete EXP..ACT_GAR 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PAT_SIS
			delete EXP..ACT_SIS 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV_REMORQUE
			delete EXP..ACT_RMQ 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMV_FOURGON
			delete EXP..ACT_FRG 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_INTERVENTION
			delete EXP..ACT_INT 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_EXP
			delete EXP..ACT_EXP_ADA
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_EXPTFM
			delete EXP..ACT_TFM_EXP 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_PMVDAT
			delete EXP..ACT_PMV_ADA 
			where actio = @vl_Action and sit = @vl_Site
		else if @vl_TypeAction = XDC_ACT_TFM
		begin
			delete EXP..ACT_TFM 
			where actio = @vl_Action and sit = @vl_Site

			/* supprimer le texte de TFM */
			delete EXP..ACT_TXT_TFM
			where actio = @vl_Action and sit = @vl_Site
		end
		/* AAZ 28/08/2007 suppression des action domaine */
		else if  @vl_TypeAction = XDC_ACT_GTC_SIG
			delete EXP..ACT_GTC_SIG 
			where actio = @vl_Action and sit = @vl_Site
		else if  @vl_TypeAction = XDC_ACT_GTC_SIG_ECH
			delete EXP..ACT_GTC_SIG 
			where actio = @vl_Action and sit = @vl_Site
		else if  @vl_TypeAction = XDC_ACT_GTC_EXP
			delete EXP..ACT_GTC_EXP
			where actio = @vl_Action and sit = @vl_Site
		else if  @vl_TypeAction = XDC_ACT_GTC_ECL
			delete EXP..ACT_GTC_ECL 
			where actio = @vl_Action and sit = @vl_Site
		else if  @vl_TypeAction = XDC_ACT_GTC_VEN
			delete EXP..ACT_GTC_VEN 
			where actio = @vl_Action and sit = @vl_Site
		else if  @vl_TypeAction = XDC_ACT_GTC_ENR
			delete EXP..ACT_GTC_ENR 
			where actio = @vl_Action and sit = @vl_Site
		else if  @vl_TypeAction = XDC_ACT_GTC_INC
			delete EXP..ACT_GTC_INC 
			where actio = @vl_Action and sit = @vl_Site
		/* AAZ */
#endif
		/* suppression de la partie generique de toutes les actions */
		delete EXP..ACT_GEN 
		where	numero = @vl_Action and 
			sit = @vl_Site

		/* passage a l'action suivante selectionnee */
		fetch Pointeur_actions_sup into @vl_Action, @vl_TypeAction
	end
	/* fermer le curseur */
	close Pointeur_actions_sup

#ifndef PC_SIMPL
	/*A Effacer tous les reveils lies a l'evenement */
	delete EXP..FMC_REV  
#endif

	/*A Effacer le complement de l'evenement et le complement */
	/*A historise sur le site local                           */
	if @vl_ClasseEvt = XZAEC_ClassePosteOper
		delete EXP..FMC_OPR 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if (@vl_ClasseEvt = XZAEC_ClasseTravaux) or (@vl_ClasseEvt = XZAEC_ClasseBasculTrav)
	begin
		delete EXP..FMC_TRF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
		delete EXP..FMC_TRH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site

		if @vl_ClasseEvt = XZAEC_ClasseBasculTrav
		begin
			delete EXP..FMC_BAS 
			where	numero = @va_NumEvt_in and
				cle = @va_CleEvt_in and
				sit = @vl_Site
		end
	end

#ifndef PC_SIMPL
	else if @vl_ClasseEvt = XZAEC_ClasseBasculement
		delete EXP..FMC_BAS 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if (@vl_ClasseEvt = XZAEC_ClasseAccident)  or  (@vl_ClasseEvt = XZAEC_ClasseVehicule)
	begin
		delete EXP..FMC_ACC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site

		update EXP..FMC_VEH
		set complement_rappel = substring(complement_rappel,1,4),zz=zz+1
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site and
			type_rappel = XZAEC_TYP_RAP_NUM_CLIENT
		update EXP..FMC_VEH set immatriculation=substring(immatriculation,1,2),zz=zz+1
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site and
			len(rtrim(immatriculation))<5 and immatriculation!=null
		update EXP..FMC_VEH set immatriculation=substring(immatriculation,1,4),zz=zz+1
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site and
			len(rtrim(immatriculation))>4

		delete EXP..FMC_VEH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	end
	else if @vl_ClasseEvt = XZAEC_ClasseDelestage
		delete EXP..FMC_DEL 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseEchangeur
		delete EXP..FMC_ECH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseSousConcess
		delete EXP..FMC_CNC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseManif
		delete EXP..FMC_MNF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseVehLent
		delete EXP..FMC_VLN 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseMeteo
	begin
		delete EXP..FMC_MET 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
		delete EXP..FMC_MET_CCH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	end
	else if @vl_ClasseEvt = XZAEC_ClasseNature
		delete EXP..FMC_NAT
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseIncendie
		delete EXP..FMC_INC
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseBouchon
		delete EXP..FMC_BOU 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site
	else if @vl_ClasseEvt = XZAEC_ClasseRegulation
		delete EXP..FMC_REG
		where   numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site

	/*A Effacer le TFM associe a l'historique situe sur */
	/*A le site local                                   */
	delete EXP..FMC_TFM 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in 

	delete EXP..FMC_COMMU
	where evenement = @va_NumEvt_in 
	and cle = @va_CleEvt_in
#endif
	/*A Effacer l'historique de l'evenement situe sur le site local */
	delete EXP..FMC_HIS 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in and
		sit = @vl_Site

#ifndef PC_SIMPL
	/*effacer la ligne correspondante dans FMC_HIS_DER*/
	delete EXP..FMC_HIS_DER 
		where numero = @va_NumEvt_in and
		cle = @va_CleEvt_in and
		sit = @vl_Site

#endif


	/*A Effacer les commentaires de l'evenement situe sur le site local */
	delete EXP..FMC_COM 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in and
		sit = @vl_Site

		
	delete EXP..ALT_LOC
	where sit = @vl_Site and numero in (select numero from EXP..ALT_EXP
			 where   evenement = @va_NumEvt_in and
			 cle = @va_CleEvt_in and
			 sit = @vl_Site)

	/*A Effacer l'alerte pointant sur l'evenement sur le site local */
	delete EXP..ALT_EXP 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in and
		sit = @vl_Site

#ifdef CI
	/*A Effacer les attribut datex  pointant sur l'evenement sur le site local */
	delete EXP..ADA_TXT_SYN 
	where	reference in (select reference
				from EXP..ADA_SIT
				where evenement = @va_NumEvt_in and
				cle = @va_CleEvt_in )

	delete EXP..ADA_SIT
	where evenement = @va_NumEvt_in and
	cle = @va_CleEvt_in
#endif

#ifndef PC_SIMPL
	/*A Effacer les attribut datex de l'evenement situe sur le site local */
	delete EXP..FMC_DAT 
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in 
	
	/*A Effacer l'alerte pointee par l'evenement sur le site local */
	delete EXP..ALT_EXP 
	where	numero = @vl_Alerte and
		sit = @va_CleEvt_in

	/*A Effacer tous les enregistrements de la file d'attente */
	/*A lies a cet evenement                                  */
	delete EXP..EQT_FIL 
	where	evenement = @va_NumEvt_in and
		cle = @va_CleEvt_in

	/*A Tester sur tous les sites s'il reste des actions */
	/*A liees a cet evenement                            */
	select @vl_NbActions = count(*)
	from EXP..ACT_GEN 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	/*A Tester sur tous les sites s'il reste un historique */
	select @vl_NbHisto = count(*)
	from EXP..FMC_HIS 
	where	numero = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	/*A Tester sur tous les sites s'il reste des commentaires */
	select @vl_NbCommentaires = count(*)
	from EXP..FMC_COM 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in
		
	/*A Tester sur tous les sites s'il reste une alerte */
	select @vl_NbAlerte = count(*)
	from EXP..ALT_EXP 
	where	evenement = @va_NumEvt_in and 
		cle = @va_CleEvt_in

	/*A Tester sur tous les sites s'il reste un complement */
	if @vl_ClasseEvt = XZAEC_ClassePosteOper
		select @vl_NbComplement = count(*)
		from EXP..FMC_OPR 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseTravaux
	begin
		select @vl_NbComplement = count(*)
		from EXP..FMC_TRF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 

		select @vl_NbComplement = @vl_NbComplement + count(*)
		from EXP..FMC_TRH
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end
	else if @vl_ClasseEvt = XZAEC_ClasseBasculTrav
	begin
		select @vl_NbComplement = count(*)
		from EXP..FMC_TRF
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

		select @vl_NbComplement = @vl_NbComplement + count(*)
		from EXP..FMC_TRH
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

		select @vl_NbComplement = @vl_NbComplement + count(*)
		from EXP..FMC_BAS
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end
	else if @vl_ClasseEvt = XZAEC_ClasseAccident
		select @vl_NbComplement = count(*)
		from EXP..FMC_ACC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseVehicule
		select @vl_NbComplement = count(*)
		from EXP..FMC_VEH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseDelestage
		select @vl_NbComplement = count(*)
		from EXP..FMC_DEL 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseEchangeur
		select @vl_NbComplement = count(*)
		from EXP..FMC_ECH 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseSousConcess
		select @vl_NbComplement = count(*)
		from EXP..FMC_CNC 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	else if @vl_ClasseEvt = XZAEC_ClasseManif
		select @vl_NbComplement = count(*)
		from EXP..FMC_MNF 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	else if @vl_ClasseEvt = XZAEC_ClasseBasculement
		select @vl_NbComplement = count(*)
		from EXP..FMC_BAS 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in 
	else if @vl_ClasseEvt = XZAEC_ClasseVehLent
		select @vl_NbComplement = count(*)
		from EXP..FMC_VLN 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseMeteo
	begin
		select @vl_NbComplement = count(*)
		from EXP..FMC_MET 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

		select @vl_NbComplement = @vl_NbComplement + count(*)
		from EXP..FMC_MET_CCH
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	end
	else if @vl_ClasseEvt = XZAEC_ClasseNature
		select @vl_NbComplement = count(*)
		from EXP..FMC_NAT
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseIncendie
		select @vl_NbComplement = count(*)
		from EXP..FMC_INC
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseBouchon
		select @vl_NbComplement = count(*)
		from EXP..FMC_BOU 
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in
	else if @vl_ClasseEvt = XZAEC_ClasseRegulation
		select @vl_NbComplement = count(*)
		from EXP..FMC_REG
		where   numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

	/*A Effacer l'evenement generique si aucun lien persiste */
	/*A sur cet evenement et s'il est sur le site local      */
	if @vl_NbComplement = 0 and @vl_NbActions = 0 and
	   @vl_NbHisto = 0 and @vl_NbAlerte = 0 and 
	   @vl_NbCommentaires = 0 and @vl_SiteEvt = @vl_Site
	begin
		/* effacement de l'evenement */
		delete EXP..FMC_GEN
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in and
			sit = @vl_Site

		/* effacement dans la table de transfert de fmc si elle existe */
		delete EXP..FMC_TRS
		where	numero = @va_NumEvt_in and
			cle = @va_CleEvt_in

		/* indiquer que l'evenement a ete efface */
		select @va_Resultat_out = XDC_OUI
	end
	else	select @va_Resultat_out = XDC_NON
#else
	/* effacement de l'evenement */
	delete EXP..FMC_GEN
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in and
		sit = @vl_Site

	select @va_Resultat_out = XDC_OUI
#endif

	return XDC_OK
go
