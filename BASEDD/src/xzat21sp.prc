/*E*/
/*  Fichier : $Id: xzat21sp.prc,v 1.14 2021/03/08 13:40:43 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2021/03/08 13:40:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	23/09/94	: Creation
* C.T.	29/11/94	: procedure differente au CI V 1.4
*                         non teste
* C.T.	05/12/94	: Correction erreur nom maitre V 1.5
*                         non teste
* B.G.	02/11/95	: envoi de desactive a la mise en service (1.6)
* C.T.	13/09/96	: Ajout cas DS et DC (RADT V1.7)
* C.P.	08/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.8) (dem / 1536)
* PN	22/09/98	: Ajout cas sur serveurs DY DP (dem/1700) 1.9
* LCL   09/07/07        : Suppression DY
* JMG	02/10/09	: secto DEM887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.21
* LCL	22/01/21	: MOVIS PRA DEM-SAE93 1.22
* GGY	26/09/23	: Ajout district CA (DEM483)
* ABK	29/09/23	: Suppression du cas de l'ancien district "VV"
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met un equipement en service
* 
* Sequence d'appel
* SP	XZAT21_Mise_ES
* 
* Arguments en entree
* XDY_Eqt	va_NumEqt_in
* XDY_Octet	va_TypeEqt_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: equipement non trouve 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
* 
* Conditions d'utilisation
* 
* Fonction
* mise a jour de INDISPONIBILITE
-------------------------------------------------------*/

use PRC
go

create procedure XZAT;21
	@va_NumEqt_in	T_EQUIPEMENT	= null,
	@va_TypeEqt_in	tinyint	= null
as
	declare @vl_NomMaitre T_NOM_MACHINE, @vl_EtatEqt smallint,
		@vl_NumEqt T_EQUIPEMENT, @vl_TypeEqt tinyint, @vl_Status int,
		@vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
		@vl_MachineTrouvee bit, @vl_Site T_SITE, @vl_Horodate datetime,
		@vl_Critique bit, @vl_Desactive bit, @vl_Majeure bit, @vl_Mineure bit,
		@vl_HS bit, @vl_Inhibe bit, @vl_Changt smallint, @vl_Erreur bit

	/*! transfo en parametres nulls bug applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_TypeEqt_in = 0 select @va_TypeEqt_in = null

	/*A controle parametre obligatoire */
	if @va_NumEqt_in = null or @va_TypeEqt_in = null
		return XDC_ARG_INV

	/*A recherche le site */
	select  @vl_Site = sit,
		@vl_Critique = critique,
		@vl_Desactive = desactive,
		@vl_Majeure = majeure,
		@vl_Mineure = mineure,
		@vl_Inhibe = inhibe 
	from EXP..EQT_DSP
	where equipement = @va_NumEqt_in and type = @va_TypeEqt_in and
		dernier = XDC_OUI

	if @@rowcount != 1
		return XDC_NOK

#ifdef CI
	/*! procedure creee au CI */

	/*A si le site n'est pas local alors demander au site */
	/*A concerne de faire l'action                        */
	if @vl_Site != XDC_CI and @vl_Site != XDC_CA
	begin
		if @vl_Site = XDC_VC
			exec @vl_Status = SQL_VC.PRC..XZAT;21 
				@va_NumEqt_in, @va_TypeEqt_in
		else if @vl_Site = XDC_DP
			exec @vl_Status = SQL_DP.PRC..XZAT;21 
				@va_NumEqt_in, @va_TypeEqt_in
		else
			return XDC_NOK

		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end
#else
	/*! procedure creee au district */

	/*A test si action est possible */
	if not exists ( select serveur from CFG..RES_DIS 
/*		where numero = @vl_Site and serveur = @@servername )
*/		where numero = @vl_Site)
	    return ( XDC_NOK )	
#endif

	begin tran
		/*A positionner les indicateurs de l'equipement */
		select @vl_Horodate = getdate()
		insert EXP..EQT_DSP ( equipement, type,  sit, critique, desactive, majeure,
		mineure, HS, inhibe, horodate)
		values(@va_NumEqt_in, @va_TypeEqt_in, @vl_Site, @vl_Critique, XDC_OUI,
		@vl_Majeure, @vl_Mineure,  XDC_NON, @vl_Inhibe, @vl_Horodate)
	
		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
		else	
			/*A positionner l'indicateur de dernier enregistrement a 0 pour */
			/*A les enregistrements precedents pour cet equipement          */
			update EXP..EQT_DSP set dernier = XDC_NON
			where	equipement = @va_NumEqt_in and type = @va_TypeEqt_in and
				horodate != @vl_Horodate and dernier = XDC_OUI

		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
	commit

	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	/*A initialisation des variables pour la recherche de la */
	/*A machine pilotant l'equipement                        */
	select	@vl_NumEqt = @va_NumEqt_in, 
		@vl_TypeEqt = @va_TypeEqt_in,
		@vl_MachineTrouvee = XDC_NON

	/*A recherche de la machine pilotant l'equipement */
	while @vl_MachineTrouvee = XDC_NON
	begin
		/*B recherche la machine pilotant un equipement */
		select 
			@vl_NumMaitre = maitre,
			@vl_TypeMaitre = type_maitre,
			@vl_NomMaitre = nom
		from CFG..EQT_GEN
		where numero = @vl_NumEqt and type = @vl_TypeEqt

		if @@rowcount != 1
			return XDC_NOK

		/*B test si l'equipement n'a plus de maitre */
		if @vl_NumMaitre = null 
			select @vl_MachineTrouvee = XDC_OUI
		else if @vl_TypeMaitre = XDC_EQT_MAC
		begin
			select @vl_NomMaitre = nom
			from CFG..EQT_GEN
			where numero = @vl_NumMaitre and type = @vl_TypeMaitre
			select @vl_MachineTrouvee = XDC_OUI
		end
		else	select @vl_NumEqt = @vl_NumMaitre, @vl_TypeEqt = @vl_TypeMaitre
	end
		
	/*A initialiser l'etat de l'equipement a positionner */
	select	@vl_EtatEqt = XDC_EQT_INHIBE * @vl_Inhibe + XDC_EQT_DESACTIVE +
				XDC_EQT_MINEUR * @vl_Mineure +
				XDC_EQT_CRITIQUE * @vl_Critique + XDC_EQT_MAJEUR * @vl_Majeure,
		@vl_Changt = XDC_EQT_HS + XDC_EQT_DESACTIVE

	/*A desactivation de l'equipement */
	if @vl_NumMaitre != null 
	begin
		if @vl_Site = XDC_CI
			exec @vl_Status = TASRV_CI...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_NumEqt_in,
					@vl_Changt, @vl_EtatEqt
		else if @vl_Site = XDC_DP
			exec @vl_Status = TASRV_DP...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_NumEqt_in,
					@vl_Changt, @vl_EtatEqt
		else if @vl_Site = XDC_CA
			exec @vl_Status = TASRV_CA...ATRP18 @vl_NomMaitre, @va_TypeEqt_in, @va_NumEqt_in,
					@vl_Changt, @vl_EtatEqt
		else
			return XDC_NOK

		if @vl_Status != XDC_OK
			return @vl_Status
	end

	return XDC_OK

go
