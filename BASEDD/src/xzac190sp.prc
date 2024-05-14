/*E*/
/*  Fichier : $Id: xzac190sp.prc,v 1.9 2021/06/29 07:56:37 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2021/06/29 07:56:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac190sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions Trafic FM 
* dans le cas du bouton "envoi Fax ASF"
*
------------------------------------------------------
* HISTORIQUE :
*
* Hilmarcher 29/10/03: Creation
* PNI	02/02/15 : Creation de l'action au sein d'une transaction  1.2
* JPL	28/06/18 : Ajout champ de proprietes du message (modifie ...) (DEM 1293)  1.3
* JMG	22/01/19 : calcul CI 1.4
* PNI	16/07/2019	: commit tran elargie � ACT_TFM ppour eviter deadlock DEM1343 1.5
* LCL   25/03/20   : MOVIS Ajout site local DEM-SAE93
* LPE	13/10/20   : MOVIS DEM-SAE93 1.6
* ABE	06/01/21   : MOVIS ajout PRA DEM-SAE93 1.7
* PNI	18/06/21	: v�rification niveau de diffusion SAE288 1.9
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer un message TFM (au CI) en base
* 
* Sequence d'appel
* SP	XZAC190_Action_TFM
* 
* Arguments en entree
* XDY_Entier		va_NumeroEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Octet		va_Niveau_in
* XDY_Horodate		va_HeureLancement_in
* XDY_Horodate		va_HeureLancementFin_in
* XDY_Operateur		va_Operateur_in
* XDY_Octet		va_Proprietes_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evnement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio est non value en entree alors 
* l'actio est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement
* Insert ou update (pour arreter la diffusion) dans la 
* table ACTION et dans la table MESSAGE_TFM
* Si un message etait actif pour le meme evenement, 
* positionne son horodate de fin.
*--------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAC190' and type = 'P')
	drop procedure XZAC190
go


create procedure XZAC190
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_NumeroAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_Niveau_in			tinyint		= null,
	@va_HeureLancement_in		datetime	= null,
	@va_HeureLancementFin_in	datetime	= null,
	@va_Operateur_in		T_OPERATEUR	= null,
	@va_NumeroAction_out		int		= null	output,
	@va_NomSiteLocal_in		char(2)		= null,
	@va_distant_in 			int 		= XDC_NON
as
	declare @vl_NumeroAction int,
		@va_Proprietes_in		tinyint		,
		@vl_Status int,
		@vl_Site tinyint,
		@vl_serveur_pra char(6)

	select @va_Proprietes_in=0

	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername
	else
		if @va_distant_in = XDC_NON
                begin
		select @vl_Site = numero
		from CFG..RES_DIS
		where code = @va_NomSiteLocal_in
		end
                else
                begin
                        select @vl_Site = @va_Site_in
                end

	/* si niveau incorrect alors forcage � 0 */
	if @va_Niveau_in > 3
		select @va_Niveau_in=0

	if (@vl_Site != XDC_CI) and (@vl_Site != XDC_CA) and @va_distant_in = XDC_NON	
	/* Traitement au CI pour Escota, en local sinon */
	begin

		/* Init serveur PRA */
		select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=XDC_CI
		if @vl_serveur_pra=XDC_SQLCI
			exec @vl_Status = SQL_CI.PRC..XZAC190 @va_NumEvt_in,
			                         @va_CleEvt_in,
						 @va_NumeroAction_in,
						 XDC_CI,
						 @va_Niveau_in,
						 @va_HeureLancement_in,
						  @va_HeureLancementFin_in,
						  @va_Operateur_in,
						  @va_NumeroAction_out output,
						  XDC_NOM_SITE_CI,
						  XDC_OUI

		if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAC190 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumeroAction_in,
						XDC_CI,
						@va_Niveau_in,
						@va_HeureLancement_in,
						@va_HeureLancementFin_in,
						@va_Operateur_in,
						@va_NumeroAction_out output,
						XDC_NOM_SITE_CI,
						XDC_OUI

		if @vl_serveur_pra=XDC_SQLDP 
			exec @vl_Status = SQL_DS.PRC..XZAC190 @va_NumEvt_in,
						@va_CleEvt_in,
						@va_NumeroAction_in,
						XDC_CI,
						@va_Niveau_in,
						@va_HeureLancement_in,
						@va_HeureLancementFin_in,
						@va_Operateur_in,
						@va_NumeroAction_out output,
						XDC_NOM_SITE_CI,
						XDC_OUI

		return @vl_Status
	end
	

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_HeureLancement_in = XDC_DATE_NULLE select @va_HeureLancement_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_Niveau_in = 0 select @va_Niveau_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Operateur_in = null or
	   (@va_HeureLancement_in = null and @va_HeureLancementFin_in = null) or
	   (@va_HeureLancement_in != null and @va_HeureLancementFin_in != null) or
	   (@va_NumeroAction_in = null and @va_HeureLancement_in = null) or
	   (@va_NumeroAction_in != null and @va_HeureLancement_in != null)
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	/*
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK
	*/

	if @va_NumeroAction_in = null
	begin
		/*A recherche le site */
		/* MOVIS */
		if @va_NomSiteLocal_in = null
			select @va_Site_in = numero 
        		from CFG..RES_DIS
        		where serveur = @@servername
		else
			select @va_Site_in = numero 
        		from CFG..RES_DIS
			where code = @va_NomSiteLocal_in	

        	if @va_Site_in is null
                	return XDC_NOK

begin tran
		/*A recherche du numero d'actio suivant */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 
		from EXP..ACT_GEN
		where sit = @va_Site_in

		/*A inserer une actio de type trafic FM*/
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		
		values( @va_NumeroAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_TFM, @va_HeureLancement_in,
			null, null, XDC_NON,null,
			null, @va_Operateur_in, @va_Site_in, null, @va_Site_in)
			insert EXP..ACT_TFM (actio, nombre_diffusions, sit , niveau, proprietes)
			values (@va_NumeroAction_out, 0, @va_Site_in, @va_Niveau_in, @va_Proprietes_in)
commit tran
		
		/* mise � fin des messages precedents */
		update EXP..ACT_GEN
		set heure_lancement_fin=@va_HeureLancement_in
		where evenement=@va_NumEvt_in
		  and cle=@va_CleEvt_in
		  and heure_lancement_fin=null
		  and numero<>@va_NumeroAction_out
		  and type=XDC_ACT_TFM
		  
		
		/*A insert l'action dans TFM_ADA s'il existe une action XDC_ACT_EXP */
		if exists ( select * from EXP..ACT_GEN where evenement=@va_NumEvt_in and cle= @va_CleEvt_in and type=XDC_ACT_EXP and heure_fin=null)
		begin
			/* A s'il n'existe pas d'action XDC_ACT_EXPTFM alors la creer */
			if not exists ( select * from EXP..ACT_GEN where evenement=@va_NumEvt_in and cle=@va_CleEvt_in  and type=XDC_ACT_EXPTFM and heure_fin=null)
			begin
		
				/*A recherche du numero d'actio suivant */
				select @vl_NumeroAction = isnull(max(numero), 0) + 1 
				from EXP..ACT_GEN
				where sit = @va_Site_in
		
				insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
					heure_succes, heure_echec, prioritaire, heure_lancement_fin,
					heure_fin, operateur, sit, distance_evt, sit_origine)
				values( @vl_NumeroAction, @va_NumEvt_in, @va_CleEvt_in, 
					XDC_ACT_EXPTFM, @va_HeureLancement_in,
					null, null, XDC_NON, null,
					null, @va_Operateur_in,  @va_Site_in, null, @va_Site_in)
				insert EXP..ACT_TFM_EXP (actio, sit, actio_tfm)
				values( @vl_NumeroAction, @va_Site_in, @va_NumeroAction_out)
			end
			insert EXP..TFM_ADA (actio,sit) 
			values (@va_NumeroAction_out,@va_Site_in)
		end

	end
	else    return XDC_NOK

	return XDC_OK
go
