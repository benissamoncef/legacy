/*E*/
/*Fichier :  @(#)xzag1300sp.prc	1.2      Release : 1.2        Date : 02/13/08
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	15/11/94	: Creation	(V 1.1)
* C.T.	16/11/94	: Modif clause sur dernier evenement
*                         cree en local (sit remplace par cle)
*                         non teste     (V 1.2)
* P.N.	09/10/96	: Ajout de MES_QPC NEC et KCS (dem/1227) (v1.3-1.4)
* P.N.  23/02/97	: Ajout de ZDP_6MN (1.5)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.6) (dem / 1536)
* C.T.	02/02/98	: Ajouter la purge sur les actions PAL, FAC, FAU (1538) (V1.7)
* AAZ 	Nov 2007	: DEM 663
* LCL	02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.9 DEM1284
* ABE	17/12/20	: Ajout IMU DEM-SAE155	1.10
* ABE	07/04/21	: Ajout PAU SONO DEM-SAE244 1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des donnees de la base d'exploitation datant
* d'au moins la date precisee situees sur le site local
* sans aucun autre controle
* 
* Sequence d'appel
* SP	XZAG13_Purge_Base_Exploitation_Rapide
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
* La derniere action et le dernier evenement sur le site local
* crees sont pas supprimes (pour eviter un doublon dans la base HIS
* lors d'une nouvelle creation)
* 
* Fonction
* Suppression des lignes datant d'au moins la date precisee
* au niveau des evenements clos, des actions finies, des SAD,
* des etats d'equipement, des alertes, des mesures situes
* sur le site local.
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAG1300' and type = 'P')
	drop procedure XZAG1300
go

create procedure XZAG1300
	@va_Horodate_in		datetime = null,
	@va_Mode_in		tinyint = XZAGC_IMP_NBR,
	@va_Resultat_out	bit 	= XDC_NON output
as
	declare @vl_Nombre int, @vl_Site T_SITE

	/*A initialisation du resultat a pas de suppression */
	select @va_Resultat_out = XDC_NON

	/*A controle du parametre en entree */
	if @va_Horodate_in = null or (@va_Mode_in != XZAGC_IMP_NBR and
	   @va_Mode_in != XZAGC_IMP_ENR)
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
           	return XDC_NOK

	/*A Rechercher la liste des evenements sur le site local dont */
	/*A la cloture datant de plus de la date precisee             */
	select * into #EVT_SUP
	from EXP..FMC_GEN
	where	cloture <= @va_Horodate_in and sit = @vl_Site and
		numero not in ( select max(numero) from EXP..FMC_GEN
				where cle = @vl_Site)

	select @vl_Nombre = @@rowcount

	/*A suppression des evenements de la liste */
	if @vl_Nombre != 0
	begin
		/*B trace et suppression des enregistrements de FMC_GEN a supprimer */
		print "FMC_GEN : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from #EVT_SUP
		delete	EXP..FMC_GEN
		from EXP..FMC_GEN, #EVT_SUP
		where	EXP..FMC_GEN.numero = #EVT_SUP.numero and
			EXP..FMC_GEN.cle = #EVT_SUP.cle and
			EXP..FMC_GEN.sit = @vl_Site
		print ' '

		/*B trace et suppression des enregistrements de FMC_ACC a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_ACC, #EVT_SUP
		where	EXP..FMC_ACC.numero = #EVT_SUP.numero and
			EXP..FMC_ACC.cle = #EVT_SUP.cle and
			EXP..FMC_ACC.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_ACC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_ACC, #EVT_SUP
				where   EXP..FMC_ACC.numero = #EVT_SUP.numero and
					EXP..FMC_ACC.cle = #EVT_SUP.cle and
					EXP..FMC_ACC.sit = @vl_Site
			delete	EXP..FMC_ACC
			from EXP..FMC_ACC, #EVT_SUP
			where	EXP..FMC_ACC.numero = #EVT_SUP.numero and
				EXP..FMC_ACC.cle = #EVT_SUP.cle and
				EXP..FMC_ACC.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_VEH a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_VEH, #EVT_SUP
		where	EXP..FMC_VEH.numero = #EVT_SUP.numero and
			EXP..FMC_VEH.cle = #EVT_SUP.cle and
			EXP..FMC_VEH.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_VEH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_VEH, #EVT_SUP
				where   EXP..FMC_VEH.numero = #EVT_SUP.numero and
					EXP..FMC_VEH.cle = #EVT_SUP.cle and
					EXP..FMC_VEH.sit = @vl_Site
			delete	EXP..FMC_VEH
			from EXP..FMC_VEH, #EVT_SUP
			where	EXP..FMC_VEH.numero = #EVT_SUP.numero and
				EXP..FMC_VEH.cle = #EVT_SUP.cle and
				EXP..FMC_VEH.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_VLN a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_VLN, #EVT_SUP
		where	EXP..FMC_VLN.numero = #EVT_SUP.numero and
			EXP..FMC_VLN.cle = #EVT_SUP.cle and
			EXP..FMC_VLN.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_VLN : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_VLN, #EVT_SUP
				where   EXP..FMC_VLN.numero = #EVT_SUP.numero and
					EXP..FMC_VLN.cle = #EVT_SUP.cle and
					EXP..FMC_VLN.sit = @vl_Site
			delete	EXP..FMC_VLN
			from EXP..FMC_VLN, #EVT_SUP
			where	EXP..FMC_VLN.numero = #EVT_SUP.numero and
				EXP..FMC_VLN.cle = #EVT_SUP.cle and
				EXP..FMC_VLN.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_MET a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_MET, #EVT_SUP
		where	EXP..FMC_MET.numero = #EVT_SUP.numero and
			EXP..FMC_MET.cle = #EVT_SUP.cle and
			EXP..FMC_MET.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_MET : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_MET, #EVT_SUP
				where   EXP..FMC_MET.numero = #EVT_SUP.numero and
					EXP..FMC_MET.cle = #EVT_SUP.cle and
					EXP..FMC_MET.sit = @vl_Site
			delete	EXP..FMC_MET
			from EXP..FMC_MET, #EVT_SUP
			where	EXP..FMC_MET.numero = #EVT_SUP.numero and
				EXP..FMC_MET.cle = #EVT_SUP.cle and
				EXP..FMC_MET.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_MNF a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_MNF, #EVT_SUP
		where	EXP..FMC_MNF.numero = #EVT_SUP.numero and
			EXP..FMC_MNF.cle = #EVT_SUP.cle and
			EXP..FMC_MNF.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_MNF : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_MNF, #EVT_SUP
				where   EXP..FMC_MNF.numero = #EVT_SUP.numero and
					EXP..FMC_MNF.cle = #EVT_SUP.cle and
					EXP..FMC_MNF.sit = @vl_Site
			delete	EXP..FMC_MNF
			from EXP..FMC_MNF, #EVT_SUP
			where	EXP..FMC_MNF.numero = #EVT_SUP.numero and
				EXP..FMC_MNF.cle = #EVT_SUP.cle and
				EXP..FMC_MNF.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_BOU a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_BOU, #EVT_SUP
		where	EXP..FMC_BOU.numero = #EVT_SUP.numero and
			EXP..FMC_BOU.cle = #EVT_SUP.cle and
			EXP..FMC_BOU.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_BOU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_BOU, #EVT_SUP
				where   EXP..FMC_BOU.numero = #EVT_SUP.numero and
					EXP..FMC_BOU.cle = #EVT_SUP.cle and
					EXP..FMC_BOU.sit = @vl_Site
			delete	EXP..FMC_BOU
			from EXP..FMC_BOU, #EVT_SUP
			where	EXP..FMC_BOU.numero = #EVT_SUP.numero and
				EXP..FMC_BOU.cle = #EVT_SUP.cle and
				EXP..FMC_BOU.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_ECH a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_ECH, #EVT_SUP
		where	EXP..FMC_ECH.numero = #EVT_SUP.numero and
			EXP..FMC_ECH.cle = #EVT_SUP.cle and
			EXP..FMC_ECH.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_ECH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_ECH, #EVT_SUP
				where   EXP..FMC_ECH.numero = #EVT_SUP.numero and
					EXP..FMC_ECH.cle = #EVT_SUP.cle and
					EXP..FMC_ECH.sit = @vl_Site
			delete	EXP..FMC_ECH
			from EXP..FMC_ECH, #EVT_SUP
			where	EXP..FMC_ECH.numero = #EVT_SUP.numero and
				EXP..FMC_ECH.cle = #EVT_SUP.cle and
				EXP..FMC_ECH.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_CNC a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_CNC, #EVT_SUP
		where	EXP..FMC_CNC.numero = #EVT_SUP.numero and
			EXP..FMC_CNC.cle = #EVT_SUP.cle and
			EXP..FMC_CNC.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_CNC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_CNC, #EVT_SUP
				where   EXP..FMC_CNC.numero = #EVT_SUP.numero and
					EXP..FMC_CNC.cle = #EVT_SUP.cle and
					EXP..FMC_CNC.sit = @vl_Site
			delete	EXP..FMC_CNC
			from EXP..FMC_CNC, #EVT_SUP
			where	EXP..FMC_CNC.numero = #EVT_SUP.numero and
				EXP..FMC_CNC.cle = #EVT_SUP.cle and
				EXP..FMC_CNC.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_TRF a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_TRF, #EVT_SUP
		where	EXP..FMC_TRF.numero = #EVT_SUP.numero and
			EXP..FMC_TRF.cle = #EVT_SUP.cle and
			EXP..FMC_TRF.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_TRF : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_TRF, #EVT_SUP
				where   EXP..FMC_TRF.numero = #EVT_SUP.numero and
					EXP..FMC_TRF.cle = #EVT_SUP.cle and
					EXP..FMC_TRF.sit = @vl_Site
			delete	EXP..FMC_TRF
			from EXP..FMC_TRF, #EVT_SUP
			where	EXP..FMC_TRF.numero = #EVT_SUP.numero and
				EXP..FMC_TRF.cle = #EVT_SUP.cle and
				EXP..FMC_TRF.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_TRH a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_TRH, #EVT_SUP
		where	EXP..FMC_TRH.numero = #EVT_SUP.numero and
			EXP..FMC_TRH.cle = #EVT_SUP.cle and
			EXP..FMC_TRH.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_TRH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_TRH, #EVT_SUP
				where   EXP..FMC_TRH.numero = #EVT_SUP.numero and
					EXP..FMC_TRH.cle = #EVT_SUP.cle and
					EXP..FMC_TRH.sit = @vl_Site
			delete	EXP..FMC_TRH
			from EXP..FMC_TRH, #EVT_SUP
			where	EXP..FMC_TRH.numero = #EVT_SUP.numero and
				EXP..FMC_TRH.cle = #EVT_SUP.cle and
				EXP..FMC_TRH.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_BAS a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_BAS, #EVT_SUP
		where	EXP..FMC_BAS.numero = #EVT_SUP.numero and
			EXP..FMC_BAS.cle = #EVT_SUP.cle and
			EXP..FMC_BAS.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_BAS : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_BAS, #EVT_SUP
				where   EXP..FMC_BAS.numero = #EVT_SUP.numero and
					EXP..FMC_BAS.cle = #EVT_SUP.cle and
					EXP..FMC_BAS.sit = @vl_Site
			delete	EXP..FMC_BAS
			from EXP..FMC_BAS, #EVT_SUP
			where	EXP..FMC_BAS.numero = #EVT_SUP.numero and
				EXP..FMC_BAS.cle = #EVT_SUP.cle and
				EXP..FMC_BAS.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_OPR a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_OPR, #EVT_SUP
		where	EXP..FMC_OPR.numero = #EVT_SUP.numero and
			EXP..FMC_OPR.cle = #EVT_SUP.cle and
			EXP..FMC_OPR.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_OPR : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_OPR, #EVT_SUP
				where   EXP..FMC_OPR.numero = #EVT_SUP.numero and
					EXP..FMC_OPR.cle = #EVT_SUP.cle and
					EXP..FMC_OPR.sit = @vl_Site
			delete	EXP..FMC_OPR
			from EXP..FMC_OPR, #EVT_SUP
			where	EXP..FMC_OPR.numero = #EVT_SUP.numero and
				EXP..FMC_OPR.cle = #EVT_SUP.cle and
				EXP..FMC_OPR.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_DEL a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_DEL, #EVT_SUP
		where	EXP..FMC_DEL.numero = #EVT_SUP.numero and
			EXP..FMC_DEL.cle = #EVT_SUP.cle and
			EXP..FMC_DEL.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_DEL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_DEL, #EVT_SUP
				where   EXP..FMC_DEL.numero = #EVT_SUP.numero and
					EXP..FMC_DEL.cle = #EVT_SUP.cle and
					EXP..FMC_DEL.sit = @vl_Site
			delete	EXP..FMC_DEL
			from EXP..FMC_DEL, #EVT_SUP
			where	EXP..FMC_DEL.numero = #EVT_SUP.numero and
				EXP..FMC_DEL.cle = #EVT_SUP.cle and
				EXP..FMC_DEL.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_HIS a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_HIS, #EVT_SUP
		where	EXP..FMC_HIS.numero = #EVT_SUP.numero and
			EXP..FMC_HIS.cle = #EVT_SUP.cle and
			EXP..FMC_HIS.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_HIS : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_HIS, #EVT_SUP
				where   EXP..FMC_HIS.numero = #EVT_SUP.numero and
					EXP..FMC_HIS.cle = #EVT_SUP.cle and
					EXP..FMC_HIS.sit = @vl_Site
			delete	EXP..FMC_HIS
			from EXP..FMC_HIS, #EVT_SUP
			where	EXP..FMC_HIS.numero = #EVT_SUP.numero and
				EXP..FMC_HIS.cle = #EVT_SUP.cle and
				EXP..FMC_HIS.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_TFM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_TFM, #EVT_SUP
		where	EXP..FMC_TFM.numero = #EVT_SUP.numero and
			EXP..FMC_TFM.cle = #EVT_SUP.cle
		if @vl_Nombre != 0
		begin
			print "FMC_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_TFM, #EVT_SUP
				where   EXP..FMC_TFM.numero = #EVT_SUP.numero and
					EXP..FMC_TFM.cle = #EVT_SUP.cle
			delete	EXP..FMC_TFM
			from EXP..FMC_TFM, #EVT_SUP
			where	EXP..FMC_TFM.numero = #EVT_SUP.numero and
				EXP..FMC_TFM.cle = #EVT_SUP.cle
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_COM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_COM, #EVT_SUP
		where	EXP..FMC_COM.evenement = #EVT_SUP.numero and
			EXP..FMC_COM.cle = #EVT_SUP.cle and
			EXP..FMC_COM.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_COM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_COM, #EVT_SUP
				where   EXP..FMC_COM.evenement = #EVT_SUP.numero and
					EXP..FMC_COM.cle = #EVT_SUP.cle and
					EXP..FMC_COM.sit = @vl_Site
			delete	EXP..FMC_COM
			from EXP..FMC_COM, #EVT_SUP
			where	EXP..FMC_COM.evenement = #EVT_SUP.numero and
				EXP..FMC_COM.cle = #EVT_SUP.cle and
				EXP..FMC_COM.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de FMC_TRT a supprimer */
		select 	@vl_Nombre = count(*) from EXP..FMC_TRT, #EVT_SUP
		where	EXP..FMC_TRT.evenement = #EVT_SUP.numero and
			EXP..FMC_TRT.cle = #EVT_SUP.cle and
			EXP..FMC_TRT.sit = @vl_Site
		if @vl_Nombre != 0
		begin
			print "FMC_TRT : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..FMC_TRT, #EVT_SUP
				where   EXP..FMC_TRT.evenement = #EVT_SUP.numero and
					EXP..FMC_TRT.cle = #EVT_SUP.cle and
					EXP..FMC_TRT.sit = @vl_Site
			delete	EXP..FMC_TRT
			from EXP..FMC_TRT, #EVT_SUP
			where	EXP..FMC_TRT.evenement = #EVT_SUP.numero and
				EXP..FMC_TRT.cle = #EVT_SUP.cle and
				EXP..FMC_TRT.sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de EQT_FIL a supprimer */
		select 	@vl_Nombre = count(*) from EXP..EQT_FIL, #EVT_SUP
		where	EXP..EQT_FIL.evenement = #EVT_SUP.numero and
			EXP..EQT_FIL.cle = #EVT_SUP.cle
		if @vl_Nombre != 0
		begin
			print "EQT_FIL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..EQT_FIL, #EVT_SUP
				where   EXP..EQT_FIL.evenement = #EVT_SUP.numero and
					EXP..EQT_FIL.cle = #EVT_SUP.cle
			delete	EXP..EQT_FIL
			from EXP..EQT_FIL, #EVT_SUP
			where	EXP..EQT_FIL.evenement = #EVT_SUP.numero and
				EXP..EQT_FIL.cle = #EVT_SUP.cle
			print ' '
		end

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	/*A Rechercher la liste de tous les SAD datant d'au moins */
	/*A la date donnee                                        */
	select * into #PLN_SUP
	from EXP..SAD_PLN 
	where	horodate <= @va_Horodate_in

	select @vl_Nombre = @@rowcount

	/*A Effacer la partie generique des plans d'actions et */
	/*A le complement associe lies a cet evenement         */
	if @vl_Nombre != 0
	begin
		/*B trace et suppression des enregistrements de SAD_PLN a supprimer */
		print "SAD_PLN : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from #PLN_SUP
		delete	EXP..SAD_PLN
		where	numero in (select numero from #PLN_SUP) 
		print ' '

		/*B trace et suppression des enregistrements de SAD_APL a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_APL
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_APL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_APL
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_APL
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_DST_FAX a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_DST_FAX
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_DST_FAX : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_DST_FAX
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_DST_FAX
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_ECH a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_ECH
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_ECH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_ECH
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_ECH
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_FAX a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_FAX
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_FAX : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_FAX
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_FAX
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_MSG_TFM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_MSG_TFM
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_MSG_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_MSG_TFM
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_MSG_TFM
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_NAV a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_NAV
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_NAV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_NAV
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_NAV
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_PMV a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_PMV
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_PMV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_PMV
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_PMV
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_CFE a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_CFE
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_CFE : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_CFE
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_CFE
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_IMU a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_IMU
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_IMU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_IMU
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_IMU
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de PAU SONO a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_SONO
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_SONO : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_SONO
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_SONO
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_TFM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_TFM
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_TFM
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_TFM
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_TUB a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_TUB
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_TUB : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_TUB
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_TUB
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_TXT_FAX a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_TXT_FAX
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_TXT_FAX : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_TXT_FAX
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_TXT_FAX
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*B trace et suppression des enregistrements de SAD_TXT_TFM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..SAD_TXT_TFM
		where	numero in (select numero from #PLN_SUP) 
		if @vl_Nombre != 0
		begin
			print "SAD_TXT_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..SAD_TXT_TFM
				where   numero in (select numero from #PLN_SUP)
			delete	EXP..SAD_TXT_TFM
			where	numero in (select numero from #PLN_SUP) 
			print ' '
		end

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	/*A Rechercher la liste de toutes les actions finies datant d'au */
	/*A moins la date donnee sur le site local                       */
	select * into #ACT_SUP
	from EXP..ACT_GEN 
	where	heure_fin <= @va_Horodate_in and sit = @vl_Site and
		numero not in ( select max(numero) from EXP..ACT_GEN
				where sit = @vl_Site)

	select @vl_Nombre = @@rowcount

	/*A Effacer la partie generique des actions et */
	/*A le complement associe lies a cette action  */
	if @vl_Nombre != 0
	begin
		/*B trace et suppression des enregistrements de ACT_GEN a supprimer */
		print "ACT_GEN : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from #ACT_SUP
		delete	EXP..ACT_GEN
		where	numero in (select numero from #ACT_SUP) and sit = @vl_Site 
		print ' '

		/*B trace et suppression des enregistrements de ACT_PMV a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_PMV
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_PMV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_PMV
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_PMV
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_CFE a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_CFE
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_CFE : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_CFE
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_CFE
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_IMU a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_IMU
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_IMU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_IMU
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_IMU
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_PAU a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_PAU
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_PAU : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_PAU
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_PAU								
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_PAL a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_PAL
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_PAL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_PAL
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_PAL
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_NAV a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_NAV
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_NAV : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_NAV
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_NAV
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TUB a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_TUB
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_TUB : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_TUB
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_TUB
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_ECH a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_ECH
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_ECH : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_ECH
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_ECH
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end
		
		/* AAZ 29/08/2007 */

		/*B trace et suppression des enregistrements de ACT_GTC_SIG a supprimer */
		select 	@vl_Nombre = count(*)
		from    EXP..ACT_GTC_SIG
		where	actio in (select numero
		                  from   #ACT_SUP) and
		        sit   =   @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_SIG : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select  * 
				from    EXP..ACT_GTC_SIG
				where   actio in (select numero 
				                  from   #ACT_SUP) and 
					    sit   =  @vl_Site
			delete	EXP..ACT_GTC_SIG
			where	actio in (select numero 
			                  from   #ACT_SUP)  and
				    sit   =  @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_EXP a supprimer */
		select 	@vl_Nombre = count(*)
		from    EXP..ACT_GTC_EXP
		where	actio in (select numero
		                  from   #ACT_SUP) and
		        sit   =   @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_EXP : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select  * 
				from    EXP..ACT_GTC_EXP
				where   actio in (select numero 
				                  from   #ACT_SUP) and 
					    sit   =  @vl_Site
			delete	EXP..ACT_GTC_EXP
			where	actio in (select numero 
			                  from   #ACT_SUP)  and
				    sit   =  @vl_Site
			print ' '
		end		
		/*B trace et suppression des enregistrements de ACT_GTC_ECL a supprimer */
		select 	@vl_Nombre = count(*)
		from    EXP..ACT_GTC_ECL
		where	actio in (select numero from #ACT_SUP) and
		        sit   =  @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_ECL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select  *
				from    EXP..ACT_GTC_ECL
				where   actio in (select numero
				                  from   #ACT_SUP) and 
					    sit   =  @vl_Site
			delete	EXP..ACT_GTC_ECL
			where	actio in (select numero
			                  from   #ACT_SUP)  and
				    sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_VEN a supprimer */
		select 	@vl_Nombre = count(*)
		from    EXP..ACT_GTC_VEN
		where	actio in (select numero 
		                  from   #ACT_SUP) and
		        sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_VEN : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   EXP..ACT_GTC_VEN
				where   actio in (select numero
				                  from   #ACT_SUP) and 
					    sit   =  @vl_Site
			delete	EXP..ACT_GTC_VEN
			where	actio in (select numero
			                  from   #ACT_SUP)  and
				    sit   =  @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_ENR a supprimer */
		select 	@vl_Nombre = count(*)
		from    EXP..ACT_GTC_ENR
		where	actio in (select numero
		                  from   #ACT_SUP) and
		        sit   =  @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_ENR : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   EXP..ACT_GTC_ENR
				where  actio in (select numero
				                 from   #ACT_SUP) and 
					   sit   =  @vl_Site
			delete	EXP..ACT_GTC_ENR
			where	actio in (select numero
			                  from   #ACT_SUP)  and
				    sit   =  @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GTC_INC a supprimer */
		select 	@vl_Nombre = count(*)
		from    EXP..ACT_GTC_INC
		where	actio in (select numero
		                  from   #ACT_SUP) and
		        sit   =  @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GTC_INC : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * 
				from   EXP..ACT_GTC_INC
				where  actio in (select numero
				                 from   #ACT_SUP) and 
					   sit   =  @vl_Site
			delete	EXP..ACT_GTC_INC
			where	actio in (select numero
			                  from   #ACT_SUP)  and
				    sit   =  @vl_Site
			print ' '
		end

		/* AAZ */

		/*B trace et suppression des enregistrements de ACT_FRG a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_FRG
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_FRG : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_FRG
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_FRG
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_RMQ a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_RMQ
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_RMQ : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_RMQ
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_RMQ
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_INT a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_INT
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_INT : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_INT
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_INT
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_SIS a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_SIS
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_SIS : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_SIS
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_SIS
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_DEP a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_DEP
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_DEP : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_DEP
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_DEP
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_GAR a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_GAR
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_GAR : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_GAR
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_GAR
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TFM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_TFM
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_TFM
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_TFM
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_FAX a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_FAX
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_FAX : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_FAX
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_FAX
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_TXT_TFM a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_TXT_TFM
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_TXT_TFM : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_TXT_TFM
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_TXT_TFM
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*B trace et suppression des enregistrements de ACT_APL a supprimer */
		select 	@vl_Nombre = count(*) from EXP..ACT_APL
		where	actio in (select numero from #ACT_SUP) and sit = @vl_Site 
		if @vl_Nombre != 0
		begin
			print "ACT_APL : %1!", @vl_Nombre
			if @va_Mode_in = XZAGC_IMP_ENR
				select * from EXP..ACT_APL
				where   actio in (select numero from #ACT_SUP) and 
					sit = @vl_Site
			delete	EXP..ACT_APL
			where	actio in (select numero from #ACT_SUP)  and
				sit = @vl_Site
			print ' '
		end

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end


	/*A Effacer toutes les alertes datant de plus que la    */
	/*A date donnee sur le site local en generant une trace */
	select @vl_Nombre = count(*) from EXP..ALT_EXP
	where horodate <= @va_Horodate_in and sit = @vl_Site
	if @vl_Nombre != 0
	begin
		print "ALT_EXP : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..ALT_EXP
			where horodate <= @va_Horodate_in and sit = @vl_Site
		delete	EXP..ALT_EXP
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	/*A Effacer tous les etats de disponibilite des equipements   */
	/*A anterieurs a la date donnee et ne representant le dernier */
	/*A etat d'un equipement sur le site local                    */
	/*! en generant une trace                                     */
	select @vl_Nombre = count(*) from EXP..EQT_DSP
	where horodate <= @va_Horodate_in and sit = @vl_Site and
		dernier = XDC_NON
	if @vl_Nombre != 0
	begin
		print "EQT_DSP : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..EQT_DSP
			where horodate <= @va_Horodate_in and sit = @vl_Site and
				dernier = XDC_NON
		delete	EXP..EQT_DSP
		where horodate <= @va_Horodate_in and sit = @vl_Site and
			dernier = XDC_NON
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

#ifdef TOTO
	/*A Effacer toutes les mesures de trafic anterieures a la date */
	/*A donnee sur le site local en generant une trace             */
	select @vl_Nombre = count(*) from EXP..MES_TRA
	where horodate <= @va_Horodate_in and sit = @vl_Site 
	if @vl_Nombre != 0
	begin
		print "MES_TRA : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..MES_TRA
			where horodate <= @va_Horodate_in and sit = @vl_Site 
		delete	EXP..MES_TRA
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from EXP..MES_POI
	where horodate <= @va_Horodate_in and sit = @vl_Site 
	if @vl_Nombre != 0
	begin
		print "MES_POI : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..MES_POI
			where horodate <= @va_Horodate_in and sit = @vl_Site 
		delete	EXP..MES_POI
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from EXP..MES_NIV
	where horodate <= @va_Horodate_in and sit = @vl_Site 
	if @vl_Nombre != 0
	begin
		print "MES_NIV : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..MES_NIV
			where horodate <= @va_Horodate_in and sit = @vl_Site 
		delete	EXP..MES_NIV
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
#ifndef CI
	select @vl_Nombre = count(*) from EXP..MES_KCS
	where horodate <= @va_Horodate_in and sit = @vl_Site 
	if @vl_Nombre != 0
	begin
		print "MES_NIV : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..MES_KCS
			where horodate <= @va_Horodate_in and sit = @vl_Site 
		delete	EXP..MES_KCS
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from EXP..MES_NEC
	where horodate <= @va_Horodate_in and sit = @vl_Site 
	if @vl_Nombre != 0
	begin
		print "MES_NIV : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..MES_NEC
			where horodate <= @va_Horodate_in and sit = @vl_Site 
		delete	EXP..MES_NEC
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from EXP..MES_QPC
	where horodate <= @va_Horodate_in and sit = @vl_Site 
	if @vl_Nombre != 0
	begin
		print "MES_NIV : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..MES_QPC
			where horodate <= @va_Horodate_in and sit = @vl_Site 
		delete	EXP..MES_QPC
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end
	select @vl_Nombre = count(*) from EXP..ZDP_6MN
	where horodate <= @va_Horodate_in and sit = @vl_Site
	if @vl_Nombre != 0
	begin
		print "ZDP_6MN  : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..ZDP_6MN
			where horodate <= @va_Horodate_in and sit = @vl_Site
		delete  EXP..ZDP_6MN
		where horodate <= @va_Horodate_in and sit = @vl_Site
		print ' '

		/*! positionnement a enregistrement supprimer */
		 select @va_Resultat_out = XDC_OUI
	end

#endif
#endif

	/*A Effacer tous les appels par fax anterieures a la date */
	/*A donnee en generant une trace                          */
	select @vl_Nombre = count(*) from EXP..APL_FAX
	where horodate <= @va_Horodate_in
	if @vl_Nombre != 0
	begin
		print "APL_FAX : %1!", @vl_Nombre
		if @va_Mode_in = XZAGC_IMP_ENR
			select * from EXP..APL_FAX
			where horodate <= @va_Horodate_in
		delete	EXP..APL_FAX
		where horodate <= @va_Horodate_in
		print ' '

		/*! positionnement a enregistrement supprimer */
		select @va_Resultat_out = XDC_OUI
	end

	return XDC_OK
go
