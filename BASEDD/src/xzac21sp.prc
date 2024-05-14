/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	24/11/94	: Modif recherche de l'etat de l'action (V 1.5)
* C.T.	23/06/94	: Ajout etats echec/fin et succes/fin 
*                         pour etat des actions fax et appels (V 1.6)
* C.T.	23/11/95	: Actions retournees dans l'ordre chronologique inverse (1.7)
* C.T.	16/02/96	: Actions retournees dans l'etat ou elles sont a la date precisee (1.8)
* JMG	18/11/96	: retourner le sous-type astreinte pour un appel tel (DEM/1306) 1.9
* JMG	02/03/98	: retourner le sit_origine (dem/1605) 1.10
* C.P.	08/06/98	: Modif separation EXP et HIS (V 1.11)
* ESCOTA 26/03/99       : Modification de la description des intervenants
* JMG	01/09/99	: tri de la liste des actions par type
* F.L   08/03/04        : Prise en compte des action DATEX (SAGA) 1.12
* JPL	19/12/08	: Sur serveur HIS, les actions DATEX sont dans la base HIS (DEM 857) 1.13
* JPL	03/12/08	: Modification de l'ordre de tri selon l'etat (DEM 839) 1.14
* JMG           02/10/09        : secto DEM887 1/15
* JPL	16/10/09	: Si la date fournie est nulle, prendre la date courante (DEM 916) 1.16
* LCL	19/07/11	: Gestion sous-type astreint client pour le rappel client 1.17 DEM 1000
* LCL	18/06/12	: Correction bug compilation sous HIS 1.18
* JMG	190/06/12	: ACT_PRV en echec fin 1.19
* JPL	08/11/12	: Prise en compte des actions d'envoi de conditions de conduite (DEM 1032)  1.20
* JMG 	09/10/18	: Ajout SMS LOT25 1.21
* ABE	18/12/20	: Ajout IMU DEM-SAE155 1.22
* ABE	07/04/21	: Ajout PAU DEM-SAE244 1.23
* ABE	07/04/21	: Fix PAU DEM-SAE244 1.24
* PNI	23/11/21	: Ajout de HIS..ACT_APL pour compilation dans HIS 1.22
* JPL	26/10/23	: Securisation dans une transaction de la lecture des actions de l'evenement (SAE-543)  1.26
* GGY	13/11/23	: Ajout PIC (DEM-483)
* PNI	15/03/24	: retourne XDC_ACT_ETAT_ECHEC_FIN horscas specifique sir heure_echec et heure_fin != null SAE-597
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des actions realisees pour un evenement (numero+libelle)
* 
* Sequence d'appel
* SP	XZAC21_Actions_Evt
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Numero_out		- Informations retournees dans une liste
* XDY_Octet	va_Site_out		-
* XDY_Octet	va_SiteOrigine_out	-
* XDY_Octet	va_Type_out		-
* XDY_Horodate	va_Heure_out		-
* XDY_Octet	va_Etat_out		-
* XDY_Nom	va_SousType_out		-
*
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* les informations sur chaque actio sont retournees dans une liste
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans la table ACTION where evenement, cle = NumEvt
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC21' and type = 'P')
	drop procedure XZAC21
go


#ifdef HIST
	#define BDD_XPL HIS
#else
	#define BDD_XPL EXP
#endif


create procedure XZAC21
	@va_Horodate_in		datetime	= null,
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null
as
	declare @vl_Num int, @vl_Lancement datetime, @vl_LancementFin datetime,
		@vl_HeureFin datetime, @vl_HeureSucces datetime, 
		@vl_Site tinyint, @vl_Type tinyint, @vl_HeureEchec datetime, 
		@vl_HeureModif datetime, @vl_Etat tinyint, @vl_delta int,
		@vl_SousType char(25) , @vl_Comp smallint , 
		@vl_SiteOrigine tinyint , @vl_Status int ,
		@vl_TypeIntervenant tinyint, @vl_type_astreinte tinyint

	/*! Transformation des parametres d'entree nulls */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV


	/*A une date nulle signifie la date courante */
	if @va_Horodate_in = XDC_DATE_NULLE
		select @va_Horodate_in = getdate ()


#ifndef HIST
	/*! cas ou la procedure est creee au CI  OU  sur le serveur d'un district */

	/* si la date donnee est anterieure d'un mois au moins alors */
 	/* rechercher l'evenement sur le serveur d'historique */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
		begin
			exec @vl_Status = SQL_HIS.PRC..XZAC21 
				@va_Horodate_in,@va_NumEvt_in,@va_CleEvt_in
			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
                end
#endif


	/* Dans les autres cas chercher l'evenement dans la base */
	/* des donnees d'exploitation sur le serveur local */

	create table #ACT_GEN (
		numero			int,
		sit			tinyint,
		sit_origine		tinyint		null,
		type			tinyint,
		heure_lancement		datetime,
		heure_lancement_fin	datetime	null,
		heure_fin		datetime	null,
		heure_succes		datetime	null,
		heure_echec		datetime	null,
		competence		smallint	null
	)

	begin transaction

	insert #ACT_GEN ( numero, sit, sit_origine, type, heure_lancement, heure_lancement_fin, heure_fin, heure_succes, heure_echec, competence )
	select numero, sit, sit_origine, type, heure_lancement, heure_lancement_fin, heure_fin, heure_succes, heure_echec, competence
	from BDD_XPL..ACT_GEN
	where   evenement = @va_NumEvt_in and cle = @va_CleEvt_in

	commit transaction


	/*A creer deux tables temporaires pour stockage des actions selectionnees */
	create table #ACT_REA
	(
		numero		int,
		sit		tinyint,
		sit_origine	tinyint		null,
		type		tinyint,
		heure_modif	datetime	null,
		etat		tinyint		null,
		sous_type	char(250)       null     /* SAGA : char(25) -> char(250) */
	)

	select * into #ACT_REA2 from #ACT_REA


	/*A creation d'un curseur pour parcourir les liste d'actions */
	declare Pointeur_liste cursor 
	for select
		numero,
		sit,
		sit_origine,
		type,
		heure_lancement,
		heure_lancement_fin,
		heure_fin,
		heure_succes,
		heure_echec,
		competence
	from #ACT_GEN
	where	heure_lancement <= @va_Horodate_in


	/*A Positionnement des valeurs de l'etat et de date de modif */
	/*A a la date donnee                                         */
	open Pointeur_liste
	fetch Pointeur_liste into @vl_Num, @vl_Site, @vl_SiteOrigine, @vl_Type, 
				@vl_Lancement, @vl_LancementFin,
				@vl_HeureFin, @vl_HeureSucces, @vl_HeureEchec,
				@vl_Comp
	while (@@sqlstatus = 0)
	begin

		/*A positionnement des dates en fonction de la date de recherche */
		if  @vl_HeureFin is not null
		begin
			select @vl_delta = datediff(second, @vl_HeureFin, @va_Horodate_in)
			if @vl_delta < 0
				select @vl_HeureFin = null
		end
		if  @vl_LancementFin is not null
		begin
			select @vl_delta = datediff(second, @vl_LancementFin, @va_Horodate_in)
			if @vl_delta < 0
				select @vl_LancementFin = null
		end
		if  @vl_HeureSucces is not null
		begin
			select @vl_delta = datediff(second, @vl_HeureSucces, @va_Horodate_in)
			if @vl_delta < 0
				select @vl_HeureSucces = null
		end
		if  @vl_HeureEchec is not null
		begin
			select @vl_delta = datediff(second, @vl_HeureEchec, @va_Horodate_in)
			if @vl_delta < 0
				select @vl_HeureEchec = null
		end

		/*A positionner la date de modification au max des dates */
		if @vl_HeureFin is not null
			select @vl_HeureModif = @vl_HeureFin
		else if @vl_LancementFin is not null
			select @vl_HeureModif = @vl_LancementFin
		else if @vl_HeureEchec is not null
			select @vl_HeureModif = @vl_HeureEchec
		else if @vl_HeureSucces is not null
		begin
			select @vl_HeureModif = @vl_HeureSucces
		end
		else 
			select @vl_HeureModif = @vl_Lancement


		/*A positionnement de l'etat pour chaque actio                 */
		/*! attention : si 2 date sont identiques alors                 */
		/*! positionner l'etat sur la colonne creee le plus tardivement */
		if (@vl_HeureModif = @vl_HeureFin)
		begin
			select @vl_Etat = XDC_ACT_ETAT_FIN

			if @vl_Type in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP, XDC_ACT_CCH)
			begin
			   if @vl_HeureSucces is not null
				select @vl_Etat = XDC_ACT_ETAT_SUCCES_FIN
			   else if @vl_HeureEchec is not null	
				select @vl_Etat = XDC_ACT_ETAT_ECHEC_FIN
			end
			else if @vl_Type = XDC_ACT_PRV or @vl_Type= XDC_ACT_CFE or @vl_Type= XDC_ACT_IMU or @vl_Type= XDC_ACT_PAU or @vl_Type= XDC_ACT_PIC
			begin				
			if @vl_HeureEchec is not null
				select @vl_Etat = XDC_ACT_ETAT_ECHEC_FIN
			end
			else
            begin
                if @vl_HeureEchec is not null
                    select @vl_Etat = XDC_ACT_ETAT_ECHEC_FIN
            end

		end
		else if (@vl_HeureModif = @vl_LancementFin)
			select @vl_Etat = XDC_ACT_ETAT_LANC_FIN
		else if (@vl_HeureModif = @vl_HeureEchec)
			select @vl_Etat = XDC_ACT_ETAT_ECHEC 
		else if (@vl_HeureModif = @vl_HeureSucces)
			select @vl_Etat = XDC_ACT_ETAT_SUCCES
		else if (@vl_HeureModif = @vl_Lancement)
			select @vl_Etat = XDC_ACT_ETAT_LANC
			


		/*A positionnement du sous-type d'astreinte selon le type d'action */
		select @vl_SousType=null

		if @vl_Type=XDC_ACT_APPEL
		begin
#ifdef HIST
			select @vl_type_astreinte=type_d_astreinte from HIS..ACT_APL where actio =  @vl_Num and sit= @vl_Site
#else
			select @vl_type_astreinte=type_d_astreinte from EXP..ACT_APL where actio =  @vl_Num and sit= @vl_Site
#endif
			if @vl_type_astreinte = XDC_AST_CLIENT
			begin
				select @vl_SousType = XDC_LIB_AST_CLIENT				
			end
			else
			begin
				select @vl_SousType=sous_type_d_astreinte
				from CFG..ANN_AST_CMP
				where numero=@vl_Comp
			end
		end

#ifdef HIST
		else if @vl_Type=XDC_ACT_INTERVENTION
		begin
			select @vl_TypeIntervenant=type
			from HIS..ACT_INT
			where actio = @vl_Num and
			      sit = @vl_Site

			if ( @vl_TypeIntervenant = XDC_ACT_INT_POM ) select @vl_SousType = "Int. Pompiers"
			else if ( @vl_TypeIntervenant = XDC_ACT_INT_GND ) select @vl_SousType = "Int. CRS/GN"
			else select @vl_SousType = "Int. Autre"
		end
#else
#ifndef PC_SIMPL
		else if @vl_Type=XDC_ACT_INTERVENTION
		begin
			select @vl_TypeIntervenant=type
			from EXP..ACT_INT
			where actio = @vl_Num and
			      sit = @vl_Site

			if ( @vl_TypeIntervenant = XDC_ACT_INT_POM ) select @vl_SousType = "Int. Pompiers"
			else if ( @vl_TypeIntervenant = XDC_ACT_INT_GND ) select @vl_SousType = "Int. CRS/GN"
			else select @vl_SousType = "Int. Autre"
		end
#endif
#endif
#ifdef HIST
		else if @vl_Type=XDC_ACT_FAX
		begin
			select @vl_TypeIntervenant = 0
			select @vl_TypeIntervenant = count(*)
			from HIS..ACT_APL
			where actio = @vl_Num and sit = @vl_Site and sms =1

			if @vl_TypeIntervenant >=1
				select @vl_SousType = "SMS"
		end
#else
#ifndef PC_SIMPL
		else if @vl_Type=XDC_ACT_FAX
		begin												
			select @vl_TypeIntervenant = 0
			select @vl_TypeIntervenant = count(*)
			from EXP..ACT_APL
			where actio = @vl_Num and sit = @vl_Site and sms =1

			if @vl_TypeIntervenant >=1
				select @vl_SousType = "SMS"
		end
#endif
#endif
#ifdef HIST
		/* Traitement des action DATEX (SAGA) */
		else if @vl_Type=XDC_ACT_DATEX
		begin
			select @vl_SousType=libelle
			from HIS..ACT_ADA
			where numero = @vl_Num and sit = @vl_Site
		end
#else
#ifndef PC_SIMPL
		/* Traitement des action DATEX (SAGA) */
		else if @vl_Type=XDC_ACT_DATEX
		begin
			select @vl_SousType=libelle
			from EXP..ACT_ADA
			where numero = @vl_Num and sit = @vl_Site
		end
#endif
#endif


		/* resultat sauvegarde */
		insert #ACT_REA (numero, sit, sit_origine, type, heure_modif, etat,sous_type)
		values( @vl_Num, @vl_Site, @vl_SiteOrigine, @vl_Type, @vl_HeureModif, @vl_Etat,@vl_SousType)

		/*A passage a l'enregistrement suivant */
		fetch Pointeur_liste into @vl_Num, @vl_Site, @vl_SiteOrigine, @vl_Type, 
					@vl_Lancement, @vl_LancementFin,
					@vl_HeureFin, @vl_HeureSucces, @vl_HeureEchec,@vl_Comp
	end

	close Pointeur_liste


	/* Trier les actions HORS APPELS selon leur etat et heure de modification */
	insert #ACT_REA2 select * from #ACT_REA
	where etat = XDC_ACT_ETAT_LANC
	  and type not in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP)
	order by heure_modif desc, numero desc

	insert #ACT_REA2 select * from #ACT_REA
	where etat = XDC_ACT_ETAT_ECHEC
	  and type not in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP)
	order by heure_modif desc, numero desc

	insert #ACT_REA2 select * from #ACT_REA
	where etat = XDC_ACT_ETAT_SUCCES
	  and type not in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP)
	order by heure_modif desc, numero desc

	insert #ACT_REA2 select * from #ACT_REA
	where etat = XDC_ACT_ETAT_LANC_FIN
	  and type not in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP)
	order by heure_modif desc, numero desc

	insert #ACT_REA2 select * from #ACT_REA
	where etat in (XDC_ACT_ETAT_FIN, XDC_ACT_ETAT_SUCCES_FIN, XDC_ACT_ETAT_ECHEC_FIN)
	  and type not in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP)
	order by heure_modif desc, numero desc


	insert #ACT_REA2 select * from #ACT_REA
	where etat  = XDC_ACT_ETAT_ECHEC_FIN
	and type = XDC_ACT_PRV
	order by heure_modif desc, numero desc

	insert #ACT_REA2 select * from #ACT_REA
	where etat  = XDC_ACT_ETAT_ECHEC_FIN
	and type = XDC_ACT_PIC
	order by heure_modif desc, numero desc

	insert #ACT_REA2 select * from #ACT_REA
	where etat  = XDC_ACT_ETAT_ECHEC_FIN
	and type = XDC_ACT_PIC
	order by heure_modif desc, numero desc

	/* Ajouter les appels tries selon leur heure de modification */
	insert #ACT_REA2 select * from #ACT_REA
	where type in (XDC_ACT_FAX, XDC_ACT_TEL, XDC_ACT_BIP )
	order by heure_modif desc, numero desc


	select * from #ACT_REA2

	return XDC_OK

go
