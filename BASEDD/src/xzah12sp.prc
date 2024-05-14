/*E*/
/*  Fichier : $Id: xzah12sp.prc,v 1.7 1995/04/07 20:58:06 gaborit Exp $      Release : $Revision: 1.7 $        Date : $Date: 1995/04/07 20:58:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation bouchon
* B.G.  16/10/94        : Creation (v1.2)
* B.G.  16/10/94        : Modif (v1.4)
* C.T.  30/03/95        : Modif objets accident (v1.5)
* C.T.  02/04/95        : Correction objets accident (v1.6)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAH12_Animer_Objet_Mobile
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Octet		va_TypeObjet_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Voie		va_Voie_in
* XDY_Octet		va_TypPtCar_in
* XDY_Octet		va_PtCar_in
* XDY_Octet		va_PosPtCar_in
* XDY_Booleen		va_Suppr_in
* 
* Arguments en sortie
* 
* Service rendu
* Prepare l'animation IHM
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
* appele sur trigger (fmc_his et act_gen)
---------------------------------------------------*/

use PRC
go

create procedure  XZAH;12
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_NumAction_in	int		= null,
	@va_Site_in		T_SITE		= null,
	@va_TypeObjet_in	tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Voie_in		tinyint		= XZAHC_VoieInconnue,
	@va_TypPtCar_in		tinyint		= null,
	@va_PtCar_in		tinyint		= null,
	@va_PosPtCar_in		tinyint		= null,
	@va_Suppr_in		bit
as

/*A D≈claration des variables locales */

	declare @vl_Portion smallint,
		@vl_Translation int,
		@vl_Nombre int
		
/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK
	
/*A S'il s'agit d'une suppression, marque la ligne en XDC_SUPPR */

	if @va_Suppr_in=XDC_VRAI
	begin
		update EXP..SYN_OBJ_POR
		
		set ordre=XDC_SUPPR
		
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			actio=@va_NumAction_in and
			cle_action=@va_Site_in
	return XDC_OK
	end

/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_TypeObjet_in=null or
		@va_Autoroute_in=null or
		@va_PR_in=null or
		@va_Sens_in=null
	return XDC_NOK

/*A Si l'objet n'est pas sur echangeur, calcule portion, et translation */

	if	@va_TypPtCar_in<>XDC_POINT_CARACT_ECHANGEUR or
		@va_Sens_in=XDC_SENS_INCONNU or
		@va_Sens_in=XDC_SENS_NORD or
		@va_Sens_in=XDC_SENS_SUD

	begin

		select	@vl_Portion=numero,
			@vl_Translation=10000*(@va_PR_in-PR_debut)/(PR_fin-PR_debut)
		
		from CFG..RES_POR
	
		where	autoroute=@va_Autoroute_in and
			PR_debut<@va_PR_in and
			PR_fin>=@va_PR_in
		
		if @@rowcount<>1
		return XDC_NOK

/*B	si le sens est connu, le force sur section courante */

		if	@va_Sens_in=XDC_SENS_SORTIE_NORD or
			@va_Sens_in=XDC_SENS_ENTREE_NORD
			
		select @va_Sens_in=XDC_SENS_NORD
		
		else if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
			@va_Sens_in=XDC_SENS_ENTREE_SUD
			
		select @va_Sens_in=XDC_SENS_SUD

	end
	
/*A Si l'objet est sur echangeur, portion=num ech, et translation=pos sur ech */
	
	else
		select	@vl_Portion=@va_PtCar_in,
			@vl_Translation=isnull(@va_PosPtCar_in,0)

/*A Verifie s'il s'agit d'une cr≈ation ou d'une modification */

	select @vl_Nombre=count(*)
	
	from EXP..SYN_OBJ_POR holdlock
	
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in and
		actio=@va_NumAction_in and
		cle_action=@va_Site_in and
		portion=@vl_Portion and
		type_objet=@va_TypeObjet_in and
		sens=@va_Sens_in and
		voie=@va_Voie_in

/*B	il s'agit d'une modification */

	if @vl_Nombre=1
	begin
		update EXP..SYN_OBJ_POR
		set	translation=@vl_Translation
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			actio=@va_NumAction_in and
			cle_action=@va_Site_in and
			portion=@vl_Portion and
			type_objet=@va_TypeObjet_in and
			sens=@va_Sens_in and
			voie=@va_Voie_in
			
		update EXP..SYN_OBJ_POR
		set	ordre=XDC_MODIF
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			actio=@va_NumAction_in and
			cle_action=@va_Site_in and
			portion=@vl_Portion and
			type_objet=@va_TypeObjet_in and
			sens=@va_Sens_in and
			voie=@va_Voie_in and
			ordre<>XDC_AJOUT
	end

/*B	il s'agit d'une modification importante (suppression puis ajout)
	ou d'un ajout */
	
	else
	begin
		update EXP..SYN_OBJ_POR
		set	ordre=XDC_SUPPR
		where	evenement=@va_NumEvt_in and
			cle=@va_CleEvt_in and
			(type_objet=@va_TypeObjet_in or
			(( @va_TypeObjet_in >= XZAHC_Meteo  and  
			   @va_TypeObjet_in <= XZAHC_Meteo + XZAEC_MET_Autre) and
			 ( type_objet >= XZAHC_Meteo  and
			   type_objet <= XZAHC_Meteo + XZAEC_MET_Autre)) or
			(( @va_TypeObjet_in >= XZAHC_Accident  and  
			  @va_TypeObjet_in <= XZAHC_Accident + XZAHC_Accident_VL +
					XZAHC_Accident_PL + XZAHC_Accident_Car +
					XZAHC_Accident_Remorque + XZAHC_Accident_Moto) and
			( type_objet >= XZAHC_Accident  and  
			  type_objet <= XZAHC_Accident + XZAHC_Accident_VL +
					XZAHC_Accident_PL + XZAHC_Accident_Car +
					XZAHC_Accident_Remorque + XZAHC_Accident_Moto))) and
			actio=@va_NumAction_in and
			cle_action=@va_Site_in
			
		insert EXP..SYN_OBJ_POR (
			evenement,
			cle,
			portion,
			ordre,
			actio,
			cle_action,
			type_objet,
			sens,
			voie,
			translation
			)
		values	(
			@va_NumEvt_in,
			@va_CleEvt_in,
			@vl_Portion,
			XDC_AJOUT,
			@va_NumAction_in,
			@va_Site_in,
			@va_TypeObjet_in,
			@va_Sens_in,
			@va_Voie_in,
			@vl_Translation
			)
	end
	return XDC_OK
go
