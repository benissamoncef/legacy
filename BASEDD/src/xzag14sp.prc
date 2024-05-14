/*E*/
/*Fichier :  $Id: xzag14sp.prc,v 1.19 2021/05/04 13:29:13 pc2dpdy Exp $      Release : $Revision: 1.19 $        Date : $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	15/11/94	: Creation (V 1.1)
* C.T.	03/05/96	: Supprimer la purge sur MES_NIV (V 1.3)
* P.N.	09/10/96	: Ajouter la purge sur MES_KCS QPC NEC	(dem/1227) (V 1.4)
* P.N.	10/10/96	: Ajouter la purge sur MES_MET	(dem/1232) (V 1.5)
* P.N.  23/02/97	: Ajouter la purge sur ZDP_HEU (dem/1396) (V 1.6)
* C.T.	02/02/98	: Ajouter la purge sur les actions PAL, FAC, FAU (1538) (V1.7)
* P.N.	27/01/99	: Ajouter les actions sur ACT_EXP ACT_EXPTFM ACT_PMVDAT  et FMC_DAT dem/1724 (V1.11)
* P.C.	06/03/02	: correction pour HIS V1.12
* CHI   20/07/05        : Ajout purge table FMC_NAT DEM487 v1.5
* AAZ 	Nov 2007 : DEM 663
* JMG	22/09/11 : purge ALT_LOC DEM 980
* VR	14/12/11 	: Ajout PRV (DEM/1016)
* JPL	13/09/12	: Prise en compte table FMC_INC pour la classe Incendie (DEM 1037)  1.14
* JPL	07/11/12	: Purge FMC_MET_CCH pour classe Meteo et ACT_CCH pour classe Operateur (DEM 1032)  1.15
* JMG	20/08/13	: purge FMC_REG 1.16
* LCL	02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.17 DEM1284
* ABE	17/12/20	: Ajout IMU DEM-SAE155 1.18
* ABE	07/04/21	: Ajout PAU SONO DEM-SAE244 1.19
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'historique datant
* d'au moins la date precisee situees sur le site local
* sans aucun autre controle
* 
* Sequence d'appel
* SP	XZAG14_Purge_Base_Historique_Rapide
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* XDY_Octet	va_Mode_in	: XZAGC_IMP_ENR/XZAGC_IMP_NBR
* 
* Arguments en sortie
* XDC_Booleen 	va_Resultat_out : XDC_OUI si suppression realisee
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* Supprimer tout jusqu'a la date "va_Horodate_in" en
* imprimant le nombre de suppression par table (XZAGC_IMP_NBR)
* ou l'enregistrement supprime (XZAGC_IMP_ENR)
* (par defaut c'est le nombre de suppression par table qui est
* affiche)
* 
* Fonction
* Suppression des lignes datant d'au moins la date precisee
* au niveau des evenements clos, des actions finies, 
* des etats d'equipement, des alertes, des mesures situes
* sur tous les sites.
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;14
	@va_Horodate_in		datetime = null,
	@va_Mode_in		tinyint = XZAGC_IMP_NBR,
	@va_Resultat_out	bit 	= XDC_NON output
as
#ifdef HIST
	declare @vl_Nombre int

	/*A initialisation du resultat a pas de suppression */
	select @va_Resultat_out = XDC_NON

	/*A controle du parametre en entree */
	if @va_Horodate_in = null or (@va_Mode_in != XZAGC_IMP_NBR and
	   @va_Mode_in != XZAGC_IMP_ENR)
		return XDC_ARG_INV

	/*A Rechercher la liste des evenements dont       */
	/*A la cloture datant de plus de la date precisee */
	select * into #EVT_SUP
	from HIS..FMC_GEN
	where	cloture <= @va_Horodate_in

	select @vl_Nombre = @@rowcount

	/*A suppression des evenements de la liste */
	if @vl_Nombre != 0
	begin
		/*B trace et suppression des enregistrements de FMC_GEN a supprimer */
		print "FMC_GEN : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from #EVT_SUP
		delete	HIS..FMC_GEN
		from HIS..FMC_GEN, #EVT_SUP
		where	HIS..FMC_GEN.numero = #EVT_SUP.numero and
			HIS..FMC_GEN.cle = #EVT_SUP.cle 
		print ' '

		/*B trace et suppression des enregistrements de FMC_ACC a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_ACC, #EVT_SUP
		where	HIS..FMC_ACC.numero = #EVT_SUP.numero and
			HIS..FMC_ACC.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_ACC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_ACC, #EVT_SUP
				where   HIS..FMC_ACC.numero = #EVT_SUP.numero and
					HIS..FMC_ACC.cle = #EVT_SUP.cle 
			delete	HIS..FMC_ACC
			from HIS..FMC_ACC, #EVT_SUP
			where	HIS..FMC_ACC.numero = #EVT_SUP.numero and
				HIS..FMC_ACC.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_VEH a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_VEH, #EVT_SUP
		where	HIS..FMC_VEH.numero = #EVT_SUP.numero and
			HIS..FMC_VEH.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_VEH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_VEH, #EVT_SUP
				where   HIS..FMC_VEH.numero = #EVT_SUP.numero and
					HIS..FMC_VEH.cle = #EVT_SUP.cle 
			delete	HIS..FMC_VEH
			from HIS..FMC_VEH, #EVT_SUP
			where	HIS..FMC_VEH.numero = #EVT_SUP.numero and
				HIS..FMC_VEH.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_VLN a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_VLN, #EVT_SUP
		where	HIS..FMC_VLN.numero = #EVT_SUP.numero and
			HIS..FMC_VLN.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_VLN : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_VLN, #EVT_SUP
				where   HIS..FMC_VLN.numero = #EVT_SUP.numero and
					HIS..FMC_VLN.cle = #EVT_SUP.cle 
			delete	HIS..FMC_VLN
			from HIS..FMC_VLN, #EVT_SUP
			where	HIS..FMC_VLN.numero = #EVT_SUP.numero and
				HIS..FMC_VLN.cle = #EVT_SUP.cle 
			print ' '
		end

		/*DEM487*/
		/*B trace et suppression des enregistrements de FMC_NAT a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_NAT, #EVT_SUP
		where	HIS..FMC_NAT.numero = #EVT_SUP.numero and
			HIS..FMC_NAT.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_NAT : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_NAT, #EVT_SUP
				where   HIS..FMC_NAT.numero = #EVT_SUP.numero and
					HIS..FMC_NAT.cle = #EVT_SUP.cle 
			delete	HIS..FMC_NAT
			from HIS..FMC_NAT, #EVT_SUP
			where	HIS..FMC_NAT.numero = #EVT_SUP.numero and
				HIS..FMC_NAT.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_INC a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_INC, #EVT_SUP
		where	HIS..FMC_INC.numero = #EVT_SUP.numero and
			HIS..FMC_INC.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_INC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_INC, #EVT_SUP
				where   HIS..FMC_INC.numero = #EVT_SUP.numero and
					HIS..FMC_INC.cle = #EVT_SUP.cle 
			delete	HIS..FMC_INC
			from HIS..FMC_INC, #EVT_SUP
			where	HIS..FMC_INC.numero = #EVT_SUP.numero and
				HIS..FMC_INC.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_MET a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_MET, #EVT_SUP
		where	HIS..FMC_MET.numero = #EVT_SUP.numero and
			HIS..FMC_MET.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_MET : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_MET, #EVT_SUP
				where   HIS..FMC_MET.numero = #EVT_SUP.numero and
					HIS..FMC_MET.cle = #EVT_SUP.cle 
			delete	HIS..FMC_MET
			from HIS..FMC_MET, #EVT_SUP
			where	HIS..FMC_MET.numero = #EVT_SUP.numero and
				HIS..FMC_MET.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_MET_CCH a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_MET_CCH, #EVT_SUP
		where	HIS..FMC_MET_CCH.numero = #EVT_SUP.numero and
			HIS..FMC_MET_CCH.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_MET_CCH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_MET_CCH, #EVT_SUP
				where   HIS..FMC_MET_CCH.numero = #EVT_SUP.numero and
					HIS..FMC_MET_CCH.cle = #EVT_SUP.cle 
			delete	HIS..FMC_MET_CCH
			from HIS..FMC_MET_CCH, #EVT_SUP
			where	HIS..FMC_MET_CCH.numero = #EVT_SUP.numero and
				HIS..FMC_MET_CCH.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_MET a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_REG, #EVT_SUP
		where	HIS..FMC_REG.numero = #EVT_SUP.numero and
			HIS..FMC_REG.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_REG : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_REG, #EVT_SUP
				where   HIS..FMC_REG.numero = #EVT_SUP.numero and
					HIS..FMC_REG.cle = #EVT_SUP.cle 
			delete	HIS..FMC_REG
			from HIS..FMC_REG, #EVT_SUP
			where	HIS..FMC_REG.numero = #EVT_SUP.numero and
				HIS..FMC_REG.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_MNF a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_MNF, #EVT_SUP
		where	HIS..FMC_MNF.numero = #EVT_SUP.numero and
			HIS..FMC_MNF.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_MNF : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_MNF, #EVT_SUP
				where   HIS..FMC_MNF.numero = #EVT_SUP.numero and
					HIS..FMC_MNF.cle = #EVT_SUP.cle 
			delete	HIS..FMC_MNF
			from HIS..FMC_MNF, #EVT_SUP
			where	HIS..FMC_MNF.numero = #EVT_SUP.numero and
				HIS..FMC_MNF.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_BOU a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_BOU, #EVT_SUP
		where	HIS..FMC_BOU.numero = #EVT_SUP.numero and
			HIS..FMC_BOU.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_BOU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_BOU, #EVT_SUP
				where   HIS..FMC_BOU.numero = #EVT_SUP.numero and
					HIS..FMC_BOU.cle = #EVT_SUP.cle 
			delete	HIS..FMC_BOU
			from HIS..FMC_BOU, #EVT_SUP
			where	HIS..FMC_BOU.numero = #EVT_SUP.numero and
				HIS..FMC_BOU.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_ECH a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_ECH, #EVT_SUP
		where	HIS..FMC_ECH.numero = #EVT_SUP.numero and
			HIS..FMC_ECH.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_ECH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_ECH, #EVT_SUP
				where   HIS..FMC_ECH.numero = #EVT_SUP.numero and
					HIS..FMC_ECH.cle = #EVT_SUP.cle 
			delete	HIS..FMC_ECH
			from HIS..FMC_ECH, #EVT_SUP
			where	HIS..FMC_ECH.numero = #EVT_SUP.numero and
				HIS..FMC_ECH.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_CNC a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_CNC, #EVT_SUP
		where	HIS..FMC_CNC.numero = #EVT_SUP.numero and
			HIS..FMC_CNC.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_CNC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_CNC, #EVT_SUP
				where   HIS..FMC_CNC.numero = #EVT_SUP.numero and
					HIS..FMC_CNC.cle = #EVT_SUP.cle 
			delete	HIS..FMC_CNC
			from HIS..FMC_CNC, #EVT_SUP
			where	HIS..FMC_CNC.numero = #EVT_SUP.numero and
				HIS..FMC_CNC.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_TRF a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_TRF, #EVT_SUP
		where	HIS..FMC_TRF.numero = #EVT_SUP.numero and
			HIS..FMC_TRF.cle = #EVT_SUP.cle
		if @vl_Nombre != 0
		begin
			print "FMC_TRF : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_TRF, #EVT_SUP
				where   HIS..FMC_TRF.numero = #EVT_SUP.numero and
					HIS..FMC_TRF.cle = #EVT_SUP.cle 
			delete	HIS..FMC_TRF
			from HIS..FMC_TRF, #EVT_SUP
			where	HIS..FMC_TRF.numero = #EVT_SUP.numero and
				HIS..FMC_TRF.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_TRH a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_TRH, #EVT_SUP
		where	HIS..FMC_TRH.numero = #EVT_SUP.numero and
			HIS..FMC_TRH.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_TRH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_TRH, #EVT_SUP
				where   HIS..FMC_TRH.numero = #EVT_SUP.numero and
					HIS..FMC_TRH.cle = #EVT_SUP.cle 
			delete	HIS..FMC_TRH
			from HIS..FMC_TRH, #EVT_SUP
			where	HIS..FMC_TRH.numero = #EVT_SUP.numero and
				HIS..FMC_TRH.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_BAS a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_BAS, #EVT_SUP
		where	HIS..FMC_BAS.numero = #EVT_SUP.numero and
			HIS..FMC_BAS.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_BAS : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_BAS, #EVT_SUP
				where   HIS..FMC_BAS.numero = #EVT_SUP.numero and
					HIS..FMC_BAS.cle = #EVT_SUP.cle 
			delete	HIS..FMC_BAS
			from HIS..FMC_BAS, #EVT_SUP
			where	HIS..FMC_BAS.numero = #EVT_SUP.numero and
				HIS..FMC_BAS.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_OPR a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_OPR, #EVT_SUP
		where	HIS..FMC_OPR.numero = #EVT_SUP.numero and
			HIS..FMC_OPR.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_OPR : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_OPR, #EVT_SUP
				where   HIS..FMC_OPR.numero = #EVT_SUP.numero and
					HIS..FMC_OPR.cle = #EVT_SUP.cle 
			delete	HIS..FMC_OPR
			from HIS..FMC_OPR, #EVT_SUP
			where	HIS..FMC_OPR.numero = #EVT_SUP.numero and
				HIS..FMC_OPR.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_DEL a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_DEL, #EVT_SUP
		where	HIS..FMC_DEL.numero = #EVT_SUP.numero and
			HIS..FMC_DEL.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_DEL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_DEL, #EVT_SUP
				where   HIS..FMC_DEL.numero = #EVT_SUP.numero and
					HIS..FMC_DEL.cle = #EVT_SUP.cle 
			delete	HIS..FMC_DEL
			from HIS..FMC_DEL, #EVT_SUP
			where	HIS..FMC_DEL.numero = #EVT_SUP.numero and
				HIS..FMC_DEL.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_HIS a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_HIS, #EVT_SUP
		where	HIS..FMC_HIS.numero = #EVT_SUP.numero and
			HIS..FMC_HIS.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_HIS : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_HIS, #EVT_SUP
				where   HIS..FMC_HIS.numero = #EVT_SUP.numero and
					HIS..FMC_HIS.cle = #EVT_SUP.cle 
			delete	HIS..FMC_HIS
			from HIS..FMC_HIS, #EVT_SUP
			where	HIS..FMC_HIS.numero = #EVT_SUP.numero and
				HIS..FMC_HIS.cle = #EVT_SUP.cle 
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_COM a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_COM, #EVT_SUP
		where	HIS..FMC_COM.evenement = #EVT_SUP.numero and
			HIS..FMC_COM.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_COM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_COM, #EVT_SUP
				where   HIS..FMC_COM.evenement = #EVT_SUP.numero and
					HIS..FMC_COM.cle = #EVT_SUP.cle 
			delete	HIS..FMC_COM
			from HIS..FMC_COM, #EVT_SUP
			where	HIS..FMC_COM.evenement = #EVT_SUP.numero and
				HIS..FMC_COM.cle = #EVT_SUP.cle 
			print ' '
		end


		/*B trace et suppression des enregistrements de FMC_DAT a supprimer */
		select 	@vl_Nombre = count(*) from HIS..FMC_DAT, #EVT_SUP
		where	HIS..FMC_DAT.numero = #EVT_SUP.numero and
			HIS..FMC_DAT.cle = #EVT_SUP.cle 
		if @vl_Nombre != 0
		begin
			print "FMC_DAT : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..FMC_DAT, #EVT_SUP
				where   HIS..FMC_DAT.numero = #EVT_SUP.numero and
					HIS..FMC_DAT.cle = #EVT_SUP.cle 
			delete	HIS..FMC_DAT
			from HIS..FMC_DAT, #EVT_SUP
			where	HIS..FMC_DAT.numero = #EVT_SUP.numero and
				HIS..FMC_DAT.cle = #EVT_SUP.cle 
			print ' '
		end

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	/*A Rechercher la liste de toutes les actions finies datant d'au */
	/*A moins la date donnee                                         */
	select * into #ACT_SUP
	from HIS..ACT_GEN 
	where	heure_fin <= @va_Horodate_in 

	select @vl_Nombre = @@rowcount

	/*A Effacer la partie generique des actions et */
	/*A le complement associe lies a cette action  */
	if @vl_Nombre != 0
	begin
		/*B trace et suppression des enregistrements de ACT_GEN a supprimer */
		print "ACT_GEN : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from #ACT_SUP
		delete	HIS..ACT_GEN
		where	numero in (select numero from #ACT_SUP) 
		print ' '

		/*B trace et suppression des enregistrements de ACT_PMV a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_PMV, #ACT_SUP
		where HIS..ACT_PMV.actio = #ACT_SUP.numero and 
			HIS..ACT_PMV.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_PMV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_PMV, #ACT_SUP
				where   HIS..ACT_PMV.actio = #ACT_SUP.numero and
					HIS..ACT_PMV.sit = #ACT_SUP.sit
			delete	HIS..ACT_PMV
			from HIS..ACT_PMV, #ACT_SUP
			where	HIS..ACT_PMV.actio = #ACT_SUP.numero and
				HIS..ACT_PMV.sit = #ACT_SUP.sit
			print ' '
		end
		
		/*B trace et suppression des enregistrements de ACT_PRV a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_PRV, #ACT_SUP
		where HIS..ACT_PRV.actio = #ACT_SUP.numero and 
			HIS..ACT_PRV.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_PRV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_PRV, #ACT_SUP
				where   HIS..ACT_PRV.actio = #ACT_SUP.numero and
					HIS..ACT_PRV.sit = #ACT_SUP.sit
			delete	HIS..ACT_PRV
			from HIS..ACT_PRV, #ACT_SUP
			where	HIS..ACT_PRV.actio = #ACT_SUP.numero and
				HIS..ACT_PRV.sit = #ACT_SUP.sit
			print ' '
		end

		/* trace et suppression des enregistrements de ACT_PIC a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_PIC, #ACT_SUP
		where HIS..ACT_PIC.actio = #ACT_SUP.numero and 
			HIS..ACT_PIC.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_PRV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_PIC, #ACT_SUP
				where   HIS..ACT_PIC.actio = #ACT_SUP.numero and
					HIS..ACT_PIC.sit = #ACT_SUP.sit
			delete	HIS..ACT_PIC
			from HIS..ACT_PIC, #ACT_SUP
			where	HIS..ACT_PIC.actio = #ACT_SUP.numero and
				HIS..ACT_PIC.sit = #ACT_SUP.sit
			print ' '
		end
		
		/*B trace et suppression des enregistrements de ACT_CFE a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_CFE, #ACT_SUP
		where HIS..ACT_CFE.actio = #ACT_SUP.numero and 
			HIS..ACT_CFE.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_CFE : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_CFE, #ACT_SUP
				where   HIS..ACT_CFE.actio = #ACT_SUP.numero and
					HIS..ACT_CFE.sit = #ACT_SUP.sit
			delete	HIS..ACT_CFE
			from HIS..ACT_CFE, #ACT_SUP
			where	HIS..ACT_CFE.actio = #ACT_SUP.numero and
				HIS..ACT_CFE.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_IMU a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_IMU, #ACT_SUP
		where HIS..ACT_IMU.actio = #ACT_SUP.numero and 
			HIS..ACT_IMU.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_IMU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_IMU, #ACT_SUP
				where   HIS..ACT_IMU.actio = #ACT_SUP.numero and
					HIS..ACT_IMU.sit = #ACT_SUP.sit
			delete	HIS..ACT_IMU
			from HIS..ACT_IMU, #ACT_SUP
			where	HIS..ACT_IMU.actio = #ACT_SUP.numero and
				HIS..ACT_IMU.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_PAU a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_PAU, #ACT_SUP
		where HIS..ACT_PAU.actio = #ACT_SUP.numero and 
			HIS..ACT_PAU.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_PAU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_PAU, #ACT_SUP
				where   HIS..ACT_PAU.actio = #ACT_SUP.numero and
					HIS..ACT_PAU.sit = #ACT_SUP.sit
			delete	HIS..ACT_PAU
			from HIS..ACT_PAU, #ACT_SUP
			where	HIS..ACT_PAU.actio = #ACT_SUP.numero and
				HIS..ACT_PAU.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_PAL a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_PAL, #ACT_SUP
		where HIS..ACT_PAL.actio = #ACT_SUP.numero and 
			HIS..ACT_PAL.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_PAL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_PAL, #ACT_SUP
				where   HIS..ACT_PAL.actio = #ACT_SUP.numero and
					HIS..ACT_PAL.sit = #ACT_SUP.sit
			delete	HIS..ACT_PAL
			from HIS..ACT_PAL, #ACT_SUP
			where	HIS..ACT_PAL.actio = #ACT_SUP.numero and
				HIS..ACT_PAL.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_PMV_ADA a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_PMV_ADA, #ACT_SUP
		where HIS..ACT_PMV_ADA.actio = #ACT_SUP.numero and 
			HIS..ACT_PMV_ADA.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_PMV_ADA : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_PMV_ADA, #ACT_SUP
				where   HIS..ACT_PMV_ADA.actio = #ACT_SUP.numero and
					HIS..ACT_PMV_ADA.sit = #ACT_SUP.sit
			delete	HIS..ACT_PMV_ADA
			from HIS..ACT_PMV_ADA, #ACT_SUP
			where	HIS..ACT_PMV_ADA.actio = #ACT_SUP.numero and
				HIS..ACT_PMV_ADA.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_EXP_ADA a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_EXP_ADA, #ACT_SUP
		where HIS..ACT_EXP_ADA.actio = #ACT_SUP.numero and 
			HIS..ACT_EXP_ADA.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_EXP_ADA : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_EXP_ADA, #ACT_SUP
				where   HIS..ACT_EXP_ADA.actio = #ACT_SUP.numero and
					HIS..ACT_EXP_ADA.sit = #ACT_SUP.sit
			delete	HIS..ACT_EXP_ADA
			from HIS..ACT_EXP_ADA, #ACT_SUP
			where	HIS..ACT_EXP_ADA.actio = #ACT_SUP.numero and
				HIS..ACT_EXP_ADA.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TFM_EXP a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_TFM_EXP, #ACT_SUP
		where HIS..ACT_TFM_EXP.actio = #ACT_SUP.numero and 
			HIS..ACT_TFM_EXP.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_TFM_EXP : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_TFM_EXP, #ACT_SUP
				where   HIS..ACT_TFM_EXP.actio = #ACT_SUP.numero and
					HIS..ACT_TFM_EXP.sit = #ACT_SUP.sit
			delete	HIS..ACT_TFM_EXP
			from HIS..ACT_TFM_EXP, #ACT_SUP
			where	HIS..ACT_TFM_EXP.actio = #ACT_SUP.numero and
				HIS..ACT_TFM_EXP.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_NAV a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_NAV, #ACT_SUP
		where	HIS..ACT_NAV.actio = #ACT_SUP.numero and
			HIS..ACT_NAV.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_NAV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_NAV, #ACT_SUP
				where   HIS..ACT_NAV.actio = #ACT_SUP.numero and
					HIS..ACT_NAV.sit = #ACT_SUP.sit
			delete	HIS..ACT_NAV
			from HIS..ACT_NAV, #ACT_SUP
			where	HIS..ACT_NAV.actio = #ACT_SUP.numero and
				HIS..ACT_NAV.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TUB a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_TUB, #ACT_SUP
		where	HIS..ACT_TUB.actio = #ACT_SUP.numero and
			HIS..ACT_TUB.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_TUB : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_TUB, #ACT_SUP
				where   HIS..ACT_TUB.actio = #ACT_SUP.numero and
					HIS..ACT_TUB.sit = #ACT_SUP.sit
			delete	HIS..ACT_TUB
			from HIS..ACT_TUB, #ACT_SUP
			where	HIS..ACT_TUB.actio = #ACT_SUP.numero and
				HIS..ACT_TUB.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_ECH a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_ECH, #ACT_SUP
		where	HIS..ACT_ECH.actio = #ACT_SUP.numero and
			HIS..ACT_ECH.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_ECH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_ECH, #ACT_SUP
				where   HIS..ACT_ECH.actio = #ACT_SUP.numero and
					HIS..ACT_ECH.sit = #ACT_SUP.sit
			delete	HIS..ACT_ECH
			from HIS..ACT_ECH, #ACT_SUP
			where	HIS..ACT_ECH.actio = #ACT_SUP.numero and
				HIS..ACT_ECH.sit = #ACT_SUP.sit
			print ' '
		end

        /* AAZ 29/08/2007 */

		/*B trace et suppression des enregistrements de ACT_GTC_SIG a supprimer */
		select 	@vl_Nombre = count(*)
		from    HIS..ACT_GTC_SIG, #ACT_SUP
		where	HIS..ACT_GTC_SIG.actio = #ACT_SUP.numero and
			    HIS..ACT_GTC_SIG.sit   = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_SIG : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   HIS..ACT_GTC_SIG, #ACT_SUP
				where  HIS..ACT_GTC_SIG.actio = #ACT_SUP.numero and
					   HIS..ACT_GTC_SIG.sit   = #ACT_SUP.sit
			delete	HIS..ACT_GTC_SIG
			from    HIS..ACT_GTC_SIG, #ACT_SUP
			where	HIS..ACT_GTC_SIG.actio = #ACT_SUP.numero and
				    HIS..ACT_GTC_SIG.sit   = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_EXP a supprimer */
		select 	@vl_Nombre = count(*)
		from    HIS..ACT_GTC_EXP, #ACT_SUP
		where	HIS..ACT_GTC_EXP.actio = #ACT_SUP.numero and
			    HIS..ACT_GTC_EXP.sit   = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_EXP : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   HIS..ACT_GTC_EXP, #ACT_SUP
				where  HIS..ACT_GTC_EXP.actio = #ACT_SUP.numero and
					   HIS..ACT_GTC_EXP.sit   = #ACT_SUP.sit
			delete	HIS..ACT_GTC_EXP
			from    HIS..ACT_GTC_EXP, #ACT_SUP
			where	HIS..ACT_GTC_EXP.actio = #ACT_SUP.numero and
				    HIS..ACT_GTC_EXP.sit   = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_ECL a supprimer */
		select 	@vl_Nombre = count(*)
		from    HIS..ACT_GTC_ECL, #ACT_SUP
		where	HIS..ACT_GTC_ECL.actio = #ACT_SUP.numero and
			    HIS..ACT_GTC_ECL.sit   = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_ECL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   HIS..ACT_GTC_ECL, #ACT_SUP
				where  HIS..ACT_GTC_ECL.actio = #ACT_SUP.numero and
					   HIS..ACT_GTC_ECL.sit   = #ACT_SUP.sit
			delete	HIS..ACT_GTC_ECL
			from    HIS..ACT_GTC_ECL, #ACT_SUP
			where	HIS..ACT_GTC_ECL.actio = #ACT_SUP.numero and
				    HIS..ACT_GTC_ECL.sit   = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_VEN a supprimer */
		select 	@vl_Nombre = count(*)
		from    HIS..ACT_GTC_VEN, #ACT_SUP
		where	HIS..ACT_GTC_VEN.actio = #ACT_SUP.numero and
			    HIS..ACT_GTC_VEN.sit   = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_VEN : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   HIS..ACT_GTC_VEN, #ACT_SUP
				where  HIS..ACT_GTC_VEN.actio = #ACT_SUP.numero and
					   HIS..ACT_GTC_VEN.sit   = #ACT_SUP.sit
			delete	HIS..ACT_GTC_VEN
			from    HIS..ACT_GTC_VEN, #ACT_SUP
			where	HIS..ACT_GTC_VEN.actio = #ACT_SUP.numero and
				    HIS..ACT_GTC_VEN.sit   = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_ENR a supprimer */
		select 	@vl_Nombre = count(*)
		from    HIS..ACT_GTC_ENR, #ACT_SUP
		where	HIS..ACT_GTC_ENR.actio = #ACT_SUP.numero and
			    HIS..ACT_GTC_ENR.sit   = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_ENR : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   HIS..ACT_GTC_ENR, #ACT_SUP
				where  HIS..ACT_GTC_ENR.actio = #ACT_SUP.numero and
					   HIS..ACT_GTC_ENR.sit   = #ACT_SUP.sit
			delete	HIS..ACT_GTC_ENR
			from    HIS..ACT_GTC_ENR, #ACT_SUP
			where	HIS..ACT_GTC_ENR.actio = #ACT_SUP.numero and
				    HIS..ACT_GTC_ENR.sit   = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_INC a supprimer */
		select 	@vl_Nombre = count(*)
		from    HIS..ACT_GTC_INC, #ACT_SUP
		where	HIS..ACT_GTC_INC.actio = #ACT_SUP.numero and
			    HIS..ACT_GTC_INC.sit   = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_INC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   HIS..ACT_GTC_INC, #ACT_SUP
				where  HIS..ACT_GTC_INC.actio = #ACT_SUP.numero and
					   HIS..ACT_GTC_INC.sit   = #ACT_SUP.sit
			delete	HIS..ACT_GTC_INC
			from    HIS..ACT_GTC_INC, #ACT_SUP
			where	HIS..ACT_GTC_INC.actio = #ACT_SUP.numero and
				    HIS..ACT_GTC_INC.sit   = #ACT_SUP.sit
			print ' '
		end

        /* AAZ */

		/*B trace et suppression des enregistrements de ACT_FRG a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_FRG, #ACT_SUP
		where	HIS..ACT_FRG.actio = #ACT_SUP.numero and
			HIS..ACT_FRG.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_FRG : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_FRG, #ACT_SUP
				where   HIS..ACT_FRG.actio = #ACT_SUP.numero and
					HIS..ACT_FRG.sit = #ACT_SUP.sit
			delete	HIS..ACT_FRG
			from HIS..ACT_FRG, #ACT_SUP
			where	HIS..ACT_FRG.actio = #ACT_SUP.numero and
				HIS..ACT_FRG.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_RMQ a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_RMQ, #ACT_SUP
		where	HIS..ACT_RMQ.actio = #ACT_SUP.numero and
			HIS..ACT_RMQ.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_RMQ : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_RMQ, #ACT_SUP
				where   HIS..ACT_RMQ.actio = #ACT_SUP.numero and
					HIS..ACT_RMQ.sit = #ACT_SUP.sit
			delete	HIS..ACT_RMQ
			from HIS..ACT_RMQ, #ACT_SUP
			where	HIS..ACT_RMQ.actio = #ACT_SUP.numero and
				HIS..ACT_RMQ.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_INT a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_INT, #ACT_SUP
		where	HIS..ACT_INT.actio = #ACT_SUP.numero and
			HIS..ACT_INT.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_INT : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_INT, #ACT_SUP
				where   HIS..ACT_INT.actio = #ACT_SUP.numero and
					HIS..ACT_INT.sit = #ACT_SUP.sit
			delete	HIS..ACT_INT
			from HIS..ACT_INT, #ACT_SUP
			where	HIS..ACT_INT.actio = #ACT_SUP.numero and
				HIS..ACT_INT.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_SIS a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_SIS, #ACT_SUP
		where	HIS..ACT_SIS.actio = #ACT_SUP.numero and
			HIS..ACT_SIS.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_SIS : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_SIS, #ACT_SUP
				where   HIS..ACT_SIS.actio = #ACT_SUP.numero and
					HIS..ACT_SIS.sit = #ACT_SUP.sit
			delete	HIS..ACT_SIS
			from HIS..ACT_SIS, #ACT_SUP
			where	HIS..ACT_SIS.actio = #ACT_SUP.numero and
				HIS..ACT_SIS.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_DEP a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_DEP, #ACT_SUP
		where	HIS..ACT_DEP.actio = #ACT_SUP.numero and
			HIS..ACT_DEP.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_DEP : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_DEP, #ACT_SUP
				where   HIS..ACT_DEP.actio = #ACT_SUP.numero and
					HIS..ACT_DEP.sit = #ACT_SUP.sit
			delete	HIS..ACT_DEP
			from HIS..ACT_DEP, #ACT_SUP
			where	HIS..ACT_DEP.actio = #ACT_SUP.numero and
				HIS..ACT_DEP.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GAR a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_GAR, #ACT_SUP
		where	HIS..ACT_GAR.actio = #ACT_SUP.numero and
			HIS..ACT_GAR.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_GAR : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_GAR, #ACT_SUP
				where   HIS..ACT_GAR.actio = #ACT_SUP.numero and
					HIS..ACT_GAR.sit = #ACT_SUP.sit
			delete	HIS..ACT_GAR
			from HIS..ACT_GAR, #ACT_SUP
			where	HIS..ACT_GAR.actio = #ACT_SUP.numero and
				HIS..ACT_GAR.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TFM a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_TFM, #ACT_SUP
		where	HIS..ACT_TFM.actio = #ACT_SUP.numero and
			HIS..ACT_TFM.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_TFM, #ACT_SUP
				where   HIS..ACT_TFM.actio = #ACT_SUP.numero and
					HIS..ACT_TFM.sit = #ACT_SUP.sit
			delete	HIS..ACT_TFM
			from HIS..ACT_TFM, #ACT_SUP
			where	HIS..ACT_TFM.actio = #ACT_SUP.numero and
				HIS..ACT_TFM.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_FAX a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_FAX, #ACT_SUP
		where	HIS..ACT_FAX.actio = #ACT_SUP.numero and
			HIS..ACT_FAX.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_FAX : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_FAX, #ACT_SUP
				where   HIS..ACT_FAX.actio = #ACT_SUP.numero and
					HIS..ACT_FAX.sit = #ACT_SUP.sit
			delete	HIS..ACT_FAX
			from HIS..ACT_FAX, #ACT_SUP
			where	HIS..ACT_FAX.actio = #ACT_SUP.numero and
				HIS..ACT_FAX.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TXT_TFM a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_TXT_TFM, #ACT_SUP
		where	HIS..ACT_TXT_TFM.actio = #ACT_SUP.numero and
			HIS..ACT_TXT_TFM.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_TXT_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_TXT_TFM, #ACT_SUP
				where   HIS..ACT_TXT_TFM.actio = #ACT_SUP.numero and
					HIS..ACT_TXT_TFM.sit = #ACT_SUP.sit
			delete	HIS..ACT_TXT_TFM
			from HIS..ACT_TXT_TFM, #ACT_SUP
			where	HIS..ACT_TXT_TFM.actio = #ACT_SUP.numero and
				HIS..ACT_TXT_TFM.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_APL a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_APL, #ACT_SUP
		where	HIS..ACT_APL.actio = #ACT_SUP.numero and
			HIS..ACT_APL.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_APL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_APL, #ACT_SUP
				where   HIS..ACT_APL.actio = #ACT_SUP.numero and
					HIS..ACT_APL.sit = #ACT_SUP.sit
			delete	HIS..ACT_APL
			from HIS..ACT_APL, #ACT_SUP
			where	HIS..ACT_APL.actio = #ACT_SUP.numero and
				HIS..ACT_APL.sit = #ACT_SUP.sit
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_CCH a supprimer */
		select 	@vl_Nombre = count(*) from HIS..ACT_CCH, #ACT_SUP
		where	HIS..ACT_CCH.actio = #ACT_SUP.numero and
			HIS..ACT_CCH.sit = #ACT_SUP.sit
		if @vl_Nombre != 0
		begin
			print "ACT_CCH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from HIS..ACT_CCH, #ACT_SUP
				where   HIS..ACT_CCH.actio = #ACT_SUP.numero and
					HIS..ACT_CCH.sit = #ACT_SUP.sit
			delete	HIS..ACT_CCH
			from HIS..ACT_CCH, #ACT_SUP
			where	HIS..ACT_CCH.actio = #ACT_SUP.numero and
				HIS..ACT_CCH.sit = #ACT_SUP.sit
			print ' '
		end

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end


	/*A Effacer toutes les alertes datant de plus que la  */
	/*A date donnee en generant une trace                 */
	select @vl_Nombre = count(*) from HIS..ALT_EXP
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "ALT_EXP : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..ALT_EXP
			where horodate <= @va_Horodate_in
		delete	HIS..ALT_EXP
		where horodate <= @va_Horodate_in
		delete HIS..ALT_LOC where numero not in (select numero from HIS..ALT_EXP)
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	/*A Effacer tous les etats de disponibilite des equipements   */
	/*A anterieurs a la date donnee et ne representant le dernier */
	/*A etat d'un equipement en generant une trace                */
	select @vl_Nombre = count(*) from HIS..EQT_DSP
	where horodate <= @va_Horodate_in and dernier = XDC_NON
	if @vl_Nombre != 0
	begin
		print "EQT_DSP : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..EQT_DSP
			where horodate <= @va_Horodate_in and 
				dernier = XDC_NON
		delete	HIS..EQT_DSP
		where horodate <= @va_Horodate_in and dernier = XDC_NON
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	/*A Effacer toutes les mesures de trafic anterieures a la date */
	/*A donnee en generant une trace                               */
	select @vl_Nombre = count(*) from HIS..MES_TRA
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "MES_TRA : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..MES_TRA
			where horodate <= @va_Horodate_in
		delete	HIS..MES_TRA
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from HIS..MES_POI
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "MES_POI : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..MES_POI
			where horodate <= @va_Horodate_in 
		delete	HIS..MES_POI
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from HIS..MES_KCS
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "MES_KCS : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..MES_KCS
			where horodate <= @va_Horodate_in 
		delete	HIS..MES_KCS
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from HIS..MES_NEC
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "MES_NEC : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..MES_NEC
			where horodate <= @va_Horodate_in 
		delete	HIS..MES_NEC
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from HIS..MES_QPC
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "MES_QPC : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..MES_QPC
			where horodate <= @va_Horodate_in 
		delete	HIS..MES_QPC
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from HIS..MES_MET
	where jour <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "MES_MET : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..MES_MET
			where jour <= @va_Horodate_in 
		delete	HIS..MES_MET
		where jour <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
        select @vl_Nombre = count(*) from HIS..ZDP_HEU
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "ZDP_HEU : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..ZDP_HEU
			where horodate <= @va_Horodate_in
		delete  HIS..ZDP_HEU
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end


	/*A Effacer tous les appels horaire anterieurs a la date specifiee */
	select @vl_Nombre = count(*) from HIS..APL_HOR
	where horodate <= @va_Horodate_in 
	if @vl_Nombre != 0
	begin
		print "APL_HOR : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from HIS..APL_HOR
			where horodate <= @va_Horodate_in 
		delete	HIS..APL_HOR
		where horodate <= @va_Horodate_in 
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	return XDC_OK
#else
	return XDC_NOK
#endif
go
