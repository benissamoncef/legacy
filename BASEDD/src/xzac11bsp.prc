/*E*/
/*  Fichier : @(#)xzac11bsp.prc	1.8      Release : 1.8        Date : 03/05/98
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac11bsp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	05/09/94	: Creation
* C.T.	13/09/94	: Modif. ajout type astreinte en entree
*                         et l'insertion de l'enregistement
* C.T.	02/11/94	: Modif. les valeurs possibles de TypeAppel
*                         et modif de la valeur inseree pour
*                         le type d'action dans ACT_GEN
*			  (pas reteste) V 1.3
* C.T.	10/11/94	: Modif bugs applix (V 1.4)
* JMG	21/11/96	: ajout numero de competence (DEM/1306) 1.5
  	* JMG	24/03/97	: le numero d'evenement est un int (DEM/1413) 1.6
* C.P.		07/01/98	: Correction gestion multi-sites dans 
*				  les procs (1.7)
* JMG	03/02/98	: ajout sit_origine (dem/1605) 1.8
* JMG   21/11/07        : ajout envoi de mail DEM/718 1.9
* JPL	21/11/07	: Creation de l'action au sein d'une transaction  1.10
* PNI	09/09/16	: DeaadLock : insert EXP..ACT_APL mis en dehors du commit tran 1.11 DEM1179
* JMG	02/10/18 	: SMS LOT25 DEM1301 1.12
* JMG	15/03/19	: remise du ';' dans le nom 1.13
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
* JPC	15/03/23	: Conversion en degroupee
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer une action d'appel telephonique ou bip.
* 
* Sequence d'appel
* SP	XZAC11_Ajouter_Appel_Tel_Bip
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Mot	va_Interlocuteur_in
* XDY_Octet	va_TypeAstreinte_in
* XDY_Mot	va_NumeroCompetence_in
* XDY_Tel	va_NumeroTelFax_in
* XDY_Octet	va_TypeAppel_in		: tel ou bip ou fax
* XDY_Horodate	va_HoroLancement_in
* XDY_Operateur	va_Operateur_in
* XDY_Horodate	va_HoroEchec_in
* XDY_Horodate	va_HoroSucces_in
* 
* Arguments en sortie
* XDY_Entier		NumeroAction
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Appele une seule fois une fois que l'operateur a declare le succes ou l'echec 
* de l'appel sauf pour un fax
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer dans les tables ACTION et APPEL_TELEPHONE_BIP_OU_FAX avec le 
* site defini en interne
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAC11
go

create procedure XZAC11
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Interlocuteur_in	smallint	= null,
	@va_TypeAstreinte_in	tinyint		= null,
	@va_NumeroCompetence_in	smallint	= null,
	@va_NumeroTelFax_in	char(100)	= null,
	@va_TypeAppel_in	tinyint		= null,
	@va_HoroLancement_in	datetime	= null,
	@va_Operateur_in	smallint	= null,
	@va_HoroEchec_in	datetime	= null,
	@va_HoroSucces_in	datetime	= null,
	@va_NumAction_out	int		= null output,
	@va_sms_in		tinyint		= null,
	@va_NomSiteLocal_in	char(2)		= null
as
	declare @vl_Site tinyint, @vl_Fin datetime

	/*! transfo en param�tres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Interlocuteur_in = 0 select @va_Interlocuteur_in = null
	if @va_TypeAstreinte_in = 0 select @va_TypeAstreinte_in = null
	if @va_NumeroCompetence_in = 0 select @va_NumeroCompetence_in = null
	if @va_TypeAppel_in = 0 select @va_TypeAppel_in = null
	if @va_Operateur_in = 0 select @va_Operateur_in = null
	if @va_HoroLancement_in = XDC_DATE_NULLE select @va_HoroLancement_in = null
	if @va_HoroEchec_in = XDC_DATE_NULLE select @va_HoroEchec_in = null
	if @va_HoroSucces_in = XDC_DATE_NULLE select @va_HoroSucces_in = null

	/*A controle des parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null or 
	   @va_Interlocuteur_in = null or @va_NumeroTelFax_in = null or
	   @va_TypeAppel_in = null or @va_HoroLancement_in = null or
	   @va_Operateur_in = null
		return XDC_ARG_INV

	if @va_TypeAppel_in != XDC_ACT_FAX and
	   @va_TypeAppel_in != XDC_ACT_BIP and
	   @va_TypeAppel_in != XDC_ACT_TEL and
	   @va_HoroEchec_in = null and @va_HoroSucces_in = null
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement */
	if not exists (select * from EXP..FMC_GEN
		where numero = @va_NumEvt_in and cle = @va_CleEvt_in)
		return XDC_NOK

	/*A rechercher le site */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
		select @vl_Site = numero 
        	from CFG..RES_DIS
        	where serveur = @@servername
	else
		select @vl_Site = numero 
        	from CFG..RES_DIS
		where code = @va_NomSiteLocal_in

        if @vl_Site is null
                return XDC_NOK

	/*A valeur de la date de fin egale a la plus grande date de succes et echec */
	if @va_HoroSucces_in != null and 
	   @va_HoroSucces_in >= isnull(@va_HoroEchec_in, @va_HoroSucces_in)
		select @vl_Fin = @va_HoroSucces_in
	else select @vl_Fin = @va_HoroEchec_in


	begin tran
	/*A recherche du numero d'actio suivant */
	select @va_NumAction_out = isnull(max( numero), 0) + 1 from EXP..ACT_GEN
	where sit = @vl_Site

	/*A creer l'actio APL */
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
				heure_succes, heure_echec, prioritaire, heure_lancement_fin,
				heure_fin, operateur, sit, competence, sit_origine)
		values( @va_NumAction_out, @va_NumEvt_in, @va_CleEvt_in, 
			@va_TypeAppel_in, @va_HoroLancement_in,
			@va_HoroSucces_in, @va_HoroEchec_in, XDC_NON, null,
			@vl_Fin, @va_Operateur_in, @vl_Site, @va_NumeroCompetence_in, @vl_Site)

#if defined (DEV)
	update EXP..ACT_GEN
	set heure_succes=@va_HoroLancement_in, heure_fin=@va_HoroLancement_in
	where numero=@va_NumAction_out and sit=@vl_Site
#endif

	commit tran
		/*A creation de l'appel de fax ou de telephone */
		insert EXP..ACT_APL (actio, sit, interlocuteur, 
				numero_telephonique, telephone_ou_bip_ou_fax,
				action_fax, type_d_astreinte,sms)
		values (@va_NumAction_out, @vl_Site, @va_Interlocuteur_in,
			@va_NumeroTelFax_in, @va_TypeAppel_in, null, @va_TypeAstreinte_in,@va_sms_in)

	return XDC_OK
go
