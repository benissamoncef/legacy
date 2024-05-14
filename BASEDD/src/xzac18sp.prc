/*E*/
/*  Fichier : $Id: xzac18sp.prc,v 1.18 2020/10/13 17:19:39 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2020/10/13 17:19:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation
* C.T.	13/09/94	: Rajout test sur le site
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	01/03/95	: update de date de fin (V 1.4)
* C.T.	30/11/95	: Creation d'une alerte dans le cas 
*                         d'un echec de fax (V 1.5)
* C.T.	25/03/96	: correction erreur de compilation (V1.6-1.7)
* C.T.	22/04/96	: declenchement d'un reveil sur fmc au lieu
*                         d'une alerte sur un echec de fax (V 1.8)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.9)
* P.C.	25/11/03	: on accepte aussi les actions de type TFM pour envoi a ASF(1.10)
* Hilmarcher 07/06/04   : modif pour prevenir operateur en cas d'envoi fax ASF brule v1.11
* JMG	21/11/07	: pas de brulage sur envoi de mail v1.12 DEM/718
* JPL	09/01/12	: Changement "RADIO TRAFIC" en "RADIO VINCI AUTOROUTES" (DEM 1009)  1.13
* PNI	02/02/15	: begin transaction pour eviter deadlock                            1.14
* JMG	02/10/18	: Ajout SMS LOT25 DEM1301                                           1.15
* RGR	23/10/18	: Correction synchro revision                                       1.17
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC18_Ecrire_CR_Fax
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* XDY_Horodate	va_HeureSucces_in
* XDY_Horodate	va_HeureEchec_in
* 
* Arguments en sortie
* 
* Service rendu
* Positionner la date de succes ou d'echec d'une actio de type envoi de fax
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Sur compte rendu d'emission de la carte FAX
* 
* Fonction
* Verifier l'existence de l'actio 
* Update dans ACTION
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC18
go

create procedure XZAC18
	@va_NumAction_in	int		= null,
	@va_Site_in		tinyint		= null,
	@va_HoroSucces_in	datetime	= null,
	@va_HoroEchec_in	datetime	= null,
	@va_NomSiteLocal_in	char(2)		= null
as
	declare @vl_TypeAction tinyint, @vl_Texte char(100), @vl_NumTel varchar(100),
		@vl_NumInterlocuteur smallint, @vl_Interlocuteur varchar(25),
		@vl_NumEvt int, @vl_CleEvt tinyint, @vl_DateCourante datetime, @vl_NumEqt smallint,
		@vl_Mail int, @vl_sms tinyint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null
	if @va_HoroSucces_in = XDC_DATE_NULLE select @va_HoroSucces_in = null
	if @va_HoroEchec_in = XDC_DATE_NULLE select @va_HoroEchec_in = null

	/*A Controle les paraletres obligatoires */
	if @va_NumAction_in = null or @va_Site_in = null or
	   (@va_HoroSucces_in = null and @va_HoroEchec_in = null)
		return XDC_ARG_INV

	/*A tester si le site est correct */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
	begin
		if not exists ( select numero from CFG..RES_DIS
                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_ARG_INV
	end 
	else
	begin
		if not exists ( select numero from CFG..RES_DIS
                                where code = @va_NomSiteLocal_in and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_ARG_INV
	end

	select @vl_Mail=0

	/*A rechercher l'evenement cause de l'actio */
	/*A avec verification de l'existance de l'actio et du fax */
	select 
		@vl_TypeAction = type,
		@vl_NumEvt = numero,
		@vl_CleEvt = cle
	from EXP..ACT_GEN
	where	numero = @va_NumAction_in and sit = @va_Site_in and ( type = XDC_ACT_FAX or type = XDC_ACT_TFM)

	if @@rowcount = 0
	begin

		return XDC_NOK
	end

	select @vl_sms = 0
	select @vl_sms = sms from EXP..ACT_APL
	where   actio = @va_NumAction_in and sit = @va_Site_in

	if @vl_sms = 1
		begin
		update EXP..ACT_GEN set
			heure_succes = isnull(@va_HoroSucces_in, @va_HoroEchec_in),
			heure_fin = isnull(@va_HoroSucces_in, @va_HoroEchec_in)
			where numero = @va_NumAction_in and sit = @va_Site_in
		return XDC_OK
	end
	/*A positionner la date de succes et d'echec */
	update EXP..ACT_GEN set 
		heure_succes = @va_HoroSucces_in, 
		heure_echec = @va_HoroEchec_in,
		heure_fin = isnull(@va_HoroSucces_in, @va_HoroEchec_in)
	where numero = @va_NumAction_in and sit = @va_Site_in


	/*A prevenir l'operateur d'un brulage d'un numero de fax */
	if @va_HoroEchec_in is not null and (@vl_TypeAction = XDC_ACT_FAX or @vl_TypeAction = XDC_ACT_TFM)
	begin
		/*B rechercher le numero de fax appele */
		select @vl_NumTel = numero_telephonique,
			@vl_NumInterlocuteur = interlocuteur
		from EXP..ACT_APL
		where	actio = @va_NumAction_in and sit = @va_Site_in


		/*B rechercher l'interlocuteur du numero de fax appele */
		if @vl_NumInterlocuteur is not null
		begin
			select @vl_Interlocuteur = nom
			from CFG..ANN_FIC
			where	numero = @vl_NumInterlocuteur

			if @@rowcount = 1
			begin
				select @vl_Texte = XZACC_FAX_BRUL_TXT + @vl_NumTel + XZACC_FAX_BRUL_TXT_SUITE1 + 
						@vl_Interlocuteur + XZACC_FAX_BRUL_TXT_SUITE2
			end
		end
		else	
		
			if @vl_NumTel is not null
			begin
				select @vl_Texte = XZACC_FAX_BRUL_TXT + @vl_NumTel + XZACC_FAX_BRUL_TXT_SUITE2
			end
			else
				if @vl_TypeAction = XDC_ACT_TFM
				begin
					select @vl_Texte = XZACC_FAX_BRUL_TXT + "00490328959" + XZACC_FAX_BRUL_TXT_SUITE1 +
						"RADIO VINCI AUTOROUTES" + XZACC_FAX_BRUL_TXT_SUITE2
				end
		/* prevenir dans la fenetre d'avertissement */
		if @vl_Mail=0
			exec XZAR;03 -1, 0, null, @vl_Texte , @va_Site_in

	end


	return XDC_OK
go
