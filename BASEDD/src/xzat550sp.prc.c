#include "/users/gesconf/migrazur/XDMICG/inc/xdc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaec.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzaac.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzacc.h"
#include "/users/gesconf/migrazur/XDMICG/inc/xzahc.h"
																																/*E*/
/* Fichier : $Id: xzat550sp.prc,v 1.3 2021/06/10 11:28:42 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2021/06/10 11:28:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat550sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 07/04/21	: Creation PAU SONO DEM-SAE244 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Met le compte rendu d'une action sur SONO en base
*
* Sequence d'appel
* SP	XZAT550_CompteRendu_SONO																				
* 
* Arguments en entree
* XDY_Mot	va_NumAction_in
* XDY_Horodate	va_Horodate_in
* XDY_Booleen	va_ResultatAction_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: action non trouvee ou 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code d'erreur de XZAR03 si pb a son appel
* 
* Conditions d'utilisation
* 
* Fonction
* mise � jour de ACTION et appeler XZAR03 si l'action a �chou�e
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT550' and type = 'P')
	drop procedure XZAT550
go

create procedure XZAT550
	@va_NumAction_in	int = null,
	@va_Horodate_in		datetime = null,
	@va_ResultatAction_in	bit,
	@va_Nb_PAUErreur_in	tinyint = null,
	 @va_Nb_PAUPilotes_in	tinyint= null
as
	declare @vl_Status int, @vl_Site T_SITE, @vl_NomEqt T_NOM_MACHINE,
		@vl_Operateur T_OPERATEUR, @vl_NumEvt int, @vl_CleEvt tinyint,
		@vl_TexteReveil char(100), @vl_FinAction bit, @vl_NumEqt T_EQUIPEMENT

	/*! transfo en param�tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Horodate_in = null
		return XDC_ARG_INV

	/*A initialisation variable locale */
	select @vl_FinAction = XDC_NON

	/*A recherche le site */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        /* vl_Site est le site sur lequel s'execute la proc */
        /* En cas de PRA, on doit trouver le site de l'equipement */
        select @vl_Site = sit
        from EXP..ACT_GEN
        where numero=@va_NumAction_in and
              type=XDC_ACT_PAU and
              heure_fin is null and
              heure_succes is null and
              heure_echec is null

        if @vl_Site is null
           	return XDC_NOK
	
	/*A test quel compte rendu a t-on ? celui du lancement de */
	/*A la commande ou de la fin de la commande               */
	if @va_NumAction_in < 0
		select @vl_FinAction = XDC_OUI, @va_NumAction_in = - @va_NumAction_in

	/*A recherche numero equipement et type */
	select @vl_NumEqt = equipement from EXP..ACT_GEN
	where numero = @va_NumAction_in and sit = @vl_Site and type = XDC_ACT_PAU

	if @@rowcount != 1
		return XDC_NOK

	/* Mettre à jour les PAU en erreur et piloté dans la table ACT_PAU */
	update EXP..ACT_PAU set PAUErreur = @va_Nb_PAUErreur_in, PAUPilotes = @va_Nb_PAUPilotes_in
	where actio = @va_NumAction_in and sit = @vl_Site

	/*A si action a reussi */
	if @va_ResultatAction_in = XDC_OK
	begin
		/*A action en cours */
		if @vl_FinAction = XDC_NON
		begin
			/*A mettre a jour la date de succes */
			update EXP..ACT_GEN set heure_succes = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site and
				heure_succes is null

			if @@rowcount != 1
				return XDC_NOK
		end
		else
		begin
			/*A mettre a jour la date de fin */
			update EXP..ACT_GEN set heure_fin  = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site and
				heure_fin is null
		end

		/*A positionner heure fin des actions precedentes */
		update EXP..ACT_GEN set heure_fin  = @va_Horodate_in
		where equipement = @vl_NumEqt and type = XDC_EQT_PAU and sit = @vl_Site and
			heure_fin is null and numero < @va_NumAction_in
	end

	/*A si l'action a echouee : mettre a jour la date d'echec et prevenir */
	if @va_ResultatAction_in = XDC_NOK								
	begin
		/*A action en cours */
		if @vl_FinAction = XDC_NON 
		begin
			/*B mettre a jour la date d'echec */
			/*B et de lancement fin           */
			update EXP..ACT_GEN set 
				heure_echec = @va_Horodate_in,
				heure_lancement_fin = @va_Horodate_in,
				heure_fin = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site 
		end
		else
			/*B mettre a jour la date de fin */
			update EXP..ACT_GEN set heure_fin = @va_Horodate_in
			where numero = @va_NumAction_in and sit = @vl_Site 
		
		if @@rowcount != 1
			return XDC_NOK

		/*A positionner l'heure fin des actions precdentes */
		update EXP..ACT_GEN set heure_fin = @va_Horodate_in
		where 	numero < @va_NumAction_in and sit = @vl_Site  and
			type = XDC_ACT_PAU and equipement = @vl_NumEqt and				
			heure_fin is null

#ifndef PC_SIMPL
		/*B rechercher les informations sur l'action et l'equipement */
		select
			@vl_NumEvt = EXP..ACT_GEN.evenement,
			@vl_CleEvt = EXP..ACT_GEN.cle,
			@vl_Operateur = EXP..ACT_GEN.operateur,
			@vl_NomEqt = CFG..EQT_GEN.nom
		from EXP..ACT_GEN, CFG..EQT_GEN
		where 	EXP..ACT_GEN.numero = @va_NumAction_in and 
			EXP..ACT_GEN.sit = @vl_Site and
			CFG..EQT_GEN.numero = EXP..ACT_GEN.equipement and
			CFG..EQT_GEN.type = XDC_EQT_PAU

		if @@rowcount != 1
			return XDC_NOK

		/*B prevenir */
		select @vl_TexteReveil = XDC_REV_CMD_EQT + @vl_NomEqt
		exec @vl_Status = XZAR;03 @vl_NumEvt, @vl_CleEvt, 
					@vl_Operateur, @vl_TexteReveil
		if @vl_Status != XDC_OK
			return @vl_Status
#endif
	end

	return XDC_OK

go
