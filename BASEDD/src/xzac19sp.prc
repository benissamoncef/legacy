/*E*/
/*  Fichier : $Id: xzac19sp.prc,v 1.24 2021/06/29 07:56:36 gesconf Exp $      Release : $Revision: 1.24 $        Date : $Date: 2021/06/29 07:56:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	09/12/94	: Modif clause existance action 
*                         et enlever la mise en commentaire a
*                         ACRP19 via TASRV (V 1.4)
*                         non teste
* B.G.	29/12/94	: mise a fin de l'action precedente (1.5)
* P.N.	18/10/96	: Ajout de la colonne niveau (dem/1248)  (1.6)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.7)
* JMG	02/02/98	: ajout sit_origine (dem/1605) 1.8
* P.N	27/11/98	: intergestionnaire (dem/1724) 1.9
* P.N   02/03/99	: correction intergest 1.10
* JMG           02/10/09        : secto DEM887 1.12
* JPL	02/11/09	: Degroupee; preciser au poste TFM la fin de l'action (DEM 917) 1.13
* PNI	02/02/15	: Creation de l'action au sein d'une transaction  1.14
*  JMG           03/03/17 : regio DEM1220
* LCL	30/06/17	: Modif PRA
* JPL	28/06/18	: Ajout champ de proprietes du message (modifie ...) (DEM 1293)  1.17
* JMG	22/01/19 : calcul au CI 1.18
* JMG	19/03/19 : correction filtre sur CI en entree 1.19
* PNI	16/07/2019	: commit tran elargie � ACT_TFM ppour eviter deadlock DEM1343 1.20
* LCL	19/11/19	: MOVIS DEM-SAE93 1.21
* LPE	13/10/20 	: MOVIS DEM-SAE93 1.21
* CGR	06/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.22
* PNI	18/06/21	: v�rification niveau de diffusion SAE288 1.24
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer ou modifier un message TFM (au CI)
* 
* Sequence d'appel
* SP	XZAC19_Ecrire_Action_TFM
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
* Appelle XZIT06 pour prevenir l'animateur radio d'un 
* changement dans sa liste de messages.
*--------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAC19' and type = 'P')
	drop procedure XZAC19
go


create procedure XZAC19
	@va_NumEvt_in			int		= null,
	@va_CleEvt_in			tinyint		= null,
	@va_NumeroAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_Niveau_in			tinyint		= null,
	@va_HeureLancement_in		datetime	= null,
	@va_HeureLancementFin_in	datetime	= null,
	@va_Operateur_in		T_OPERATEUR	= null,
	@va_NumeroAction_out		int		= null	output,
	@va_Proprietes_in		tinyint		= null,
	@va_NomSiteLocal_in		char(2)		= null,
	@va_distant_in 			int 		= XDC_NON
as
	declare @vl_NumeroAction	int,
		@vl_Cmd_TFM		tinyint,
		@vl_Status		int,
		@vl_site 		T_SITE,
		@vl_serveur_pra		char(6)

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_HeureLancement_in = XDC_DATE_NULLE select @va_HeureLancement_in = null
	if @va_HeureLancementFin_in = XDC_DATE_NULLE select @va_HeureLancementFin_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_NumeroAction_in = 0 select @va_NumeroAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

		/*A recherche le site */
		/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_site = numero 
    		from   CFG..RES_DIS
    		where  serveur = @@servername
	else
		if @va_distant_in = XDC_NON
		begin
			select @vl_site = numero
	        	from CFG..RES_DIS
			where code = @va_NomSiteLocal_in
		end
		else
		begin
			select @vl_site = @va_Site_in
		end

	/* si niveau incorrect alors forcage � 0 */
	if @va_Niveau_in > 3
		select @va_Niveau_in=0

	if @va_Site_in = XDC_CI and @vl_site != XDC_CI and @va_Site_in != XDC_CA and @vl_site != XDC_CA and @va_distant_in = XDC_NON
	/* Traitement au CI pour Escota, en local sinon */
	begin
		
		/* Init serveur PRA */
		select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=XDC_CI
		if @vl_serveur_pra = XDC_SQLCI
				exec @vl_Status = SQL_DS.PRC..XZAC19
				                @va_NumEvt_in, @va_CleEvt_in, @va_NumeroAction_in, XDC_CI,
						@va_Niveau_in, @va_HeureLancement_in, @va_HeureLancementFin_in,
						@va_Operateur_in, @va_NumeroAction_out output , @va_Proprietes_in, XDC_NOM_SITE_CI, XDC_OUI
		if @vl_serveur_pra = XDC_SQLDP
			exec @vl_Status = SQL_DS.PRC..XZAC19
					@va_NumEvt_in, @va_CleEvt_in, @va_NumeroAction_in, XDC_CI,
					@va_Niveau_in, @va_HeureLancement_in, @va_HeureLancementFin_in,
					@va_Operateur_in, @va_NumeroAction_out output , @va_Proprietes_in, XDC_NOM_SITE_CI, XDC_OUI
		else
			exec @vl_Status = SQL_CI.PRC..XZAC19
					@va_NumEvt_in, @va_CleEvt_in, @va_NumeroAction_in, XDC_CI,
					@va_Niveau_in, @va_HeureLancement_in, @va_HeureLancementFin_in, 
					@va_Operateur_in, @va_NumeroAction_out output , @va_Proprietes_in, XDC_NOM_SITE_CI, XDC_OUI
		return @vl_Status
	end

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or @va_Operateur_in = null or
	   (@va_HeureLancement_in = null and @va_HeureLancementFin_in = null) or
	   (@va_HeureLancement_in != null and @va_HeureLancementFin_in != null) or
	   (@va_NumeroAction_in = null and @va_HeureLancement_in = null) or
	   (@va_NumeroAction_in != null and @va_HeureLancement_in != null)
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	if not exists ( select * from EXP..FMC_GEN 
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK

	if @va_NumeroAction_in = null
	begin
		/*A recherche le site */
		/* MOVIS @va_Site_in est passe en parametre
		select @va_Site_in = numero 
        	from CFG..RES_DIS
        	where serveur = @@servername
		*/

        	if @va_Site_in is null
                	return XDC_NOK

begin tran
		/*A recherche du numero d'actio suivant */
		select @va_NumeroAction_out = isnull(max(numero), 0) + 1 
		from EXP..ACT_GEN
		where sit = @va_Site_in

		/*A inserer une actio de type Trafic FM */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		values( @va_NumeroAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			XDC_ACT_TFM, @va_HeureLancement_in,
			null, null, XDC_NON, null,
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
	else
	begin
		/*A tester si le site est correct */
		if not exists ( select numero from CFG..RES_DIS
                                where numero = @va_Site_in and type <> XDC_TYPE_PCNIVEAU3 )
/*                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
*/
			return XDC_NOK

		/*A tester si l'actio existe */
		if not exists (select * from EXP..ACT_GEN, EXP..ACT_TFM
				where	EXP..ACT_GEN.numero = @va_NumeroAction_in and 
					EXP..ACT_GEN.type = XDC_ACT_TFM and
					EXP..ACT_GEN.sit = @va_Site_in and
					EXP..ACT_GEN.evenement = @va_NumEvt_in and
					EXP..ACT_GEN.cle = @va_CleEvt_in and
					EXP..ACT_TFM.actio = @va_NumeroAction_in and 
					EXP..ACT_TFM.sit = @va_Site_in)
			return XDC_NOK

		/*A modification d'une actio */
		update EXP..ACT_GEN 
			set heure_lancement_fin = @va_HeureLancementFin_in
		where numero = @va_NumeroAction_in and sit = @va_Site_in

		/*A numero d'actio */
		select @va_NumeroAction_out = @va_NumeroAction_in


		/*A prevenir l'animateur radio de la modification de l'action */
		select @vl_Cmd_TFM = XDC_TERMINE
		if @va_Site_in = XDC_CI
			exec @vl_Status = TASRV_CI...ACRP19 @vl_Cmd_TFM, @va_NumeroAction_in, @va_Site_in
		else if @va_Site_in = XDC_DP
			exec @vl_Status = TASRV_DP...ACRP19 @vl_Cmd_TFM, @va_NumeroAction_in, @va_Site_in
		else if @va_Site_in = XDC_VC
			exec @vl_Status = TASRV_VC...ACRP19 @vl_Cmd_TFM, @va_NumeroAction_in, @va_Site_in
		else if @va_Site_in = XDC_CA
			exec @vl_Status = TASRV_CA...ACRP19 @vl_Cmd_TFM, @va_NumeroAction_in, @va_Site_in

		if @vl_Status = null
			return XDC_PRC_INC
		else if @vl_Status != XDC_OK
			return XDC_NOK
	end

	return XDC_OK
go
