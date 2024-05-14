/*E*/
/*  Fichier : $Id: xzae040sp.prc,v 1.36 2019/05/13 12:27:10 devgfi Exp $        $Revision: 1.36 $        $Date: 2019/05/13 12:27:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae040sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	01/06/23	: Creation => Correction Calcul Plan d'Action Consignation Bouchons (DEM-475)
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs generiques d'une fiche main 
* courante a l'heure precisee
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAE040_Lire_Fiche_MC
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in : par defaut heure courante
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Numero_out
* XDY_Octet	va_Cle_out
* XDY_Entier	va_Site_out
* XDY_Mot	va_Type_out
* XDY_Horodate	va_DebutPrevu_out
* XDY_Horodate	va_Debut_out
* XDY_Horodate	va_FinPrevu_out
* XDY_Horodate	va_Fin_out
* XDY_Horodate	va_Validation_out
* XDY_Entier	va_NumCause_out
* XDY_Octet	va_CleCause_out
* XDY_Octet	va_TypeCause_out
* XDY_Alerte	va_NumAlerte_out
* XDY_Horodate	va_HoroAlerte_out
* XDY_Nom	va_TypeAlerte_out
* XDY_Nom       va_Origine_out
* XDY_Booleen	va_Confirme_out
* XDY_Autoroute	va_Autoroute_out
* XDY_PR	va_PR_out
* XDY_Sens	va_Sens_out
* XDY_Octet	va_PointCar_out
* XDY_Nom	va_NomPtCar_out
* XDY_Voie	va_VR_out
* XDY_Voie	va_VM2_out
* XDY_Voie	va_VM1_out
* XDY_Voie	va_VL_out
* XDY_Voie	va_BAU_out
* XDY_Voie	va_VR_I_out
* XDY_Voie	va_VM2_I_out
* XDY_Voie	va_VM1_I_out
* XDY_Voie	va_VL_I_out
* XDY_Voie	va_BAU_I_out
* XDY_Autoroute	va_AutorouteCause_out
* XDY_PR	va_PRCause_out
* XDY_Sens	va_SensCause_out
* XDY_Horodate	va_HoroCause_out
* XDY_Octet	va_Position_out
* XDY_Octet	va_Degats_out
* XDY_Mot	va_TypeAnterieur_out
* XDY_Octet	va_FausseAlerte_out
* XDY_Octet	va_BAU_etroite_out
* XDY_Octet	va_BAU_I_etroite_out
* XDY_Entier	va_Longueur_out
* char(15)	va_AbrPtCar_out
* XDY_Octet	va_NumPtCar_out
* XDY_Octet	va_Degrade_out
* XDY_Entier	va_NumEvtInit_out
* XDY_Octet	va_CleEvtInit_out
* XDY_Octet	va_SiteAlerte_out
* XDY_Nom	va_ComEvtInit_out
* XDY_Octet	va_TypeEvtInit_out
* XDY_Autoroute	va_AutorouteEvtInit_out
* XDY_PR	va_PREvtInit_out
* XDY_Sens	va_SensEvtInit_out
* XDY_Horodate	va_HoroEvtInit_out
* XDY_Octet	va_Datex_out
* XDY_Octet	va_Trafic_out		
* XDY_Octet	va_Gravite_out		
* XDY_Booleen	va_Bloquant_out	
* XDY_Octet	va_Duree_out
* XDY_Octet	va_Majeur_out
* XDY_Octet	va_Ecoulement_out
*
*
* XDY_Octet	va_NumCommentaire_out
* XDY_Horodate	va_HorodateCommentaire_out
* XDY_Horodate	va_HoroPerimeCommentaire_out
* XDY_Commentaire	va_TexteCommentaire_out
* XDY_Site	va_Site_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Penser a lire le complement fonction du type d'evenement
* En formation, preciser l'horodate
* Les informations concernant les commentaires sont retournes
* dans une liste
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
* Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, 
* ALERTE, COMMENTAIRES where numero evenement and horodate
----------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAE040' and type = 'P')
	drop procedure XZAE040
go


create procedure XZAE040
	@va_Horodate_in		datetime , -- par defaut heure courante
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null,
	@va_Numero_out		int	 	= null	output,
	@va_Cle_out		tinyint	 	= null	output,
	@va_Site_out		T_SITE	 	= null	output,
	@va_Type_out		tinyint	 	= null	output,	
	@va_DebutPrevu_out	datetime 	= null	output,
	@va_Debut_out		datetime 	= null	output,
	@va_FinPrevu_out	datetime 	= null	output,
	@va_Fin_out		datetime 	= null	output,
	@va_Validation_out	datetime 	= null	output,
	@va_NumCause_out	int	 	= null	output,
	@va_CleCause_out	tinyint	 	= null	output,
	@va_TypeCause_out	smallint 	= null	output,
	@va_NumAlerte_out	int	 	= null	output,
	@va_HoroAlerte_out	datetime 	= null	output,
	@va_TypeAlerte_out	T_NOM	 	= null	output,
	@va_Origine_out		T_NOM	 	= null	output,
	@va_Confirme_out	bit	 	= XDC_NON	output,
	@va_Autoroute_out	tinyint	 	= null	output,
	@va_PR_out		T_PR	 	= null	output,
	@va_Sens_out		tinyint	 	= null	output,
	@va_PointCar_out	tinyint	 	= null	output,
	@va_NomPtCar_out	T_NOM	 	= null	output,
	@va_VR_out		tinyint	 	= null	output,
	@va_VM2_out		tinyint	 	= null	output,
	@va_VM1_out		tinyint	 	= null	output,
	@va_VL_out		tinyint	 	= null	output,
	@va_BAU_out		tinyint	 	= null	output,
	@va_VR_I_out		tinyint	 	= null	output,
	@va_VM2_I_out		tinyint	 	= null	output,
	@va_VM1_I_out		tinyint	 	= null	output,
	@va_VL_I_out		tinyint	 	= null	output,
	@va_BAU_I_out		tinyint	 	= null	output,
	@va_AutorouteCause_out	tinyint	 	= null	output,
	@va_PRCause_out		T_PR	 	= null	output,
	@va_SensCause_out	tinyint	 	= null	output,
	@va_HoroCause_out	datetime 	= null	output,
	@va_Position_out	tinyint	 	= null	output,
	@va_Degats_out		tinyint	 	= null	output,
	@va_TypeAnterieur_out	smallint	= null	output,
	@va_FausseAlerte_out	tinyint	 	= null	output,
	@va_BAU_Etroite_out	tinyint		= null	output,
	@va_BAU_I_Etroite_out	tinyint		= null	output,
	@va_Longueur_out		int		= null	output,
	@va_AbrPtCar_out	char(15) 	= null	output,
	@va_NumPtCar_out	tinyint	 	= null	output,
	@va_Degrade_out		tinyint	 	= null	output,
	@va_NumEvtInit_out	int 		= null	output,
	@va_CleEvtInit_out	tinyint 	= null	output,
	@va_ComEvtInit_out	T_NOM	 	= null	output,
	@va_TypeEvtInit_out	smallint 	= null	output,
	@va_AutorouteEvtInit_out	tinyint	 	= null	output,
	@va_PREvtInit_out	T_PR	 	= null	output,
	@va_SensEvtInit_out	tinyint	 	= null	output,
	@va_HoroEvtInit_out	datetime 	= null	output,
	@va_SiteAlerte_out	tinyint	 	= null	output,
	@va_Datex_out		tinyint	 	= null	output,
	@va_Trafic_out		tinyint	 	= null	output,
	@va_Gravite_out		tinyint	 	= null	output,
	@va_Bloquant_out	bit	 	= XDC_NON	output,
	@va_Duree_out		tinyint	 	= null	output,
	@va_Majeur_out		tinyint         = null  output,
	@va_Ecoulement_out	tinyint		= null	output
    
as
	declare @vl_sqlserver char(6), 
			@vl_Status int

	/*! transfo en parametres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A controle des arguments obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
    select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_CleEvt_in

	if @vl_sqlserver is null
		return XDC_NOK

    if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE40    @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Numero_out output,
                                                @va_Cle_out output,
                                                @va_Site_out output,
                                                @va_Type_out output,	
                                                @va_DebutPrevu_out output,
                                                @va_Debut_out output,
                                                @va_FinPrevu_out output,
                                                @va_Fin_out output,
                                                @va_Validation_out output,
                                                @va_NumCause_out output,
                                                @va_CleCause_out output,
                                                @va_TypeCause_out output,
                                                @va_NumAlerte_out output,
                                                @va_HoroAlerte_out output,
                                                @va_TypeAlerte_out output,
                                                @va_Origine_out output,
                                                @va_Confirme_out output,
                                                @va_Autoroute_out output,
                                                @va_PR_out output,
                                                @va_Sens_out output,
                                                @va_PointCar_out output,
                                                @va_NomPtCar_out output,
                                                @va_VR_out output,
                                                @va_VM2_out output,
                                                @va_VM1_out output,
                                                @va_VL_out output,
                                                @va_BAU_out output,
                                                @va_VR_I_out output,
                                                @va_VM2_I_out output,
                                                @va_VM1_I_out output,
                                                @va_VL_I_out output,
                                                @va_BAU_I_out output,
                                                @va_AutorouteCause_out output,
                                                @va_PRCause_out output,
                                                @va_SensCause_out output,
                                                @va_HoroCause_out output,
                                                @va_Position_out output,
                                                @va_Degats_out output,
                                                @va_TypeAnterieur_out output,
                                                @va_FausseAlerte_out output,
                                                @va_BAU_Etroite_out output,
                                                @va_BAU_I_Etroite_out output,
                                                @va_Longueur_out output,
                                                @va_AbrPtCar_out output,
                                                @va_NumPtCar_out output,
                                                @va_Degrade_out output,
                                                @va_NumEvtInit_out output,
                                                @va_CleEvtInit_out output,
                                                @va_ComEvtInit_out output,
                                                @va_TypeEvtInit_out output,
                                                @va_AutorouteEvtInit_out output,
                                                @va_PREvtInit_out output,
                                                @va_SensEvtInit_out output,
                                                @va_HoroEvtInit_out output,
                                                @va_SiteAlerte_out output,
                                                @va_Datex_out output,
                                                @va_Trafic_out output,
                                                @va_Gravite_out output,
                                                @va_Bloquant_out output,
                                                @va_Duree_out output,
                                                @va_Majeur_out output,
                                                @va_Ecoulement_out output

	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE40    @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Numero_out output,
                                                @va_Cle_out output,
                                                @va_Site_out output,
                                                @va_Type_out output,	
                                                @va_DebutPrevu_out output,
                                                @va_Debut_out output,
                                                @va_FinPrevu_out output,
                                                @va_Fin_out output,
                                                @va_Validation_out output,
                                                @va_NumCause_out output,
                                                @va_CleCause_out output,
                                                @va_TypeCause_out output,
                                                @va_NumAlerte_out output,
                                                @va_HoroAlerte_out output,
                                                @va_TypeAlerte_out output,
                                                @va_Origine_out output,
                                                @va_Confirme_out output,
                                                @va_Autoroute_out output,
                                                @va_PR_out output,
                                                @va_Sens_out output,
                                                @va_PointCar_out output,
                                                @va_NomPtCar_out output,
                                                @va_VR_out output,
                                                @va_VM2_out output,
                                                @va_VM1_out output,
                                                @va_VL_out output,
                                                @va_BAU_out output,
                                                @va_VR_I_out output,
                                                @va_VM2_I_out output,
                                                @va_VM1_I_out output,
                                                @va_VL_I_out output,
                                                @va_BAU_I_out output,
                                                @va_AutorouteCause_out output,
                                                @va_PRCause_out output,
                                                @va_SensCause_out output,
                                                @va_HoroCause_out output,
                                                @va_Position_out output,
                                                @va_Degats_out output,
                                                @va_TypeAnterieur_out output,
                                                @va_FausseAlerte_out output,
                                                @va_BAU_Etroite_out output,
                                                @va_BAU_I_Etroite_out output,
                                                @va_Longueur_out output,
                                                @va_AbrPtCar_out output,
                                                @va_NumPtCar_out output,
                                                @va_Degrade_out output,
                                                @va_NumEvtInit_out output,
                                                @va_CleEvtInit_out output,
                                                @va_ComEvtInit_out output,
                                                @va_TypeEvtInit_out output,
                                                @va_AutorouteEvtInit_out output,
                                                @va_PREvtInit_out output,
                                                @va_SensEvtInit_out output,
                                                @va_HoroEvtInit_out output,
                                                @va_SiteAlerte_out output,
                                                @va_Datex_out output,
                                                @va_Trafic_out output,
                                                @va_Gravite_out output,
                                                @va_Bloquant_out output,
                                                @va_Duree_out output,
                                                @va_Majeur_out output,
                                                @va_Ecoulement_out output

	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE40    @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Numero_out output,
                                                @va_Cle_out output,
                                                @va_Site_out output,
                                                @va_Type_out output,	
                                                @va_DebutPrevu_out output,
                                                @va_Debut_out output,
                                                @va_FinPrevu_out output,
                                                @va_Fin_out output,
                                                @va_Validation_out output,
                                                @va_NumCause_out output,
                                                @va_CleCause_out output,
                                                @va_TypeCause_out output,
                                                @va_NumAlerte_out output,
                                                @va_HoroAlerte_out output,
                                                @va_TypeAlerte_out output,
                                                @va_Origine_out output,
                                                @va_Confirme_out output,
                                                @va_Autoroute_out output,
                                                @va_PR_out output,
                                                @va_Sens_out output,
                                                @va_PointCar_out output,
                                                @va_NomPtCar_out output,
                                                @va_VR_out output,
                                                @va_VM2_out output,
                                                @va_VM1_out output,
                                                @va_VL_out output,
                                                @va_BAU_out output,
                                                @va_VR_I_out output,
                                                @va_VM2_I_out output,
                                                @va_VM1_I_out output,
                                                @va_VL_I_out output,
                                                @va_BAU_I_out output,
                                                @va_AutorouteCause_out output,
                                                @va_PRCause_out output,
                                                @va_SensCause_out output,
                                                @va_HoroCause_out output,
                                                @va_Position_out output,
                                                @va_Degats_out output,
                                                @va_TypeAnterieur_out output,
                                                @va_FausseAlerte_out output,
                                                @va_BAU_Etroite_out output,
                                                @va_BAU_I_Etroite_out output,
                                                @va_Longueur_out output,
                                                @va_AbrPtCar_out output,
                                                @va_NumPtCar_out output,
                                                @va_Degrade_out output,
                                                @va_NumEvtInit_out output,
                                                @va_CleEvtInit_out output,
                                                @va_ComEvtInit_out output,
                                                @va_TypeEvtInit_out output,
                                                @va_AutorouteEvtInit_out output,
                                                @va_PREvtInit_out output,
                                                @va_SensEvtInit_out output,
                                                @va_HoroEvtInit_out output,
                                                @va_SiteAlerte_out output,
                                                @va_Datex_out output,
                                                @va_Trafic_out output,
                                                @va_Gravite_out output,
                                                @va_Bloquant_out output,
                                                @va_Duree_out output,
                                                @va_Majeur_out output,
                                                @va_Ecoulement_out output

  else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE40    @va_Horodate_in,
                                                @va_NumEvt_in,
                                                @va_CleEvt_in,
                                                @va_Numero_out output,
                                                @va_Cle_out output,
                                                @va_Site_out output,
                                                @va_Type_out output,	
                                                @va_DebutPrevu_out output,
                                                @va_Debut_out output,
                                                @va_FinPrevu_out output,
                                                @va_Fin_out output,
                                                @va_Validation_out output,
                                                @va_NumCause_out output,
                                                @va_CleCause_out output,
                                                @va_TypeCause_out output,
                                                @va_NumAlerte_out output,
                                                @va_HoroAlerte_out output,
                                                @va_TypeAlerte_out output,
                                                @va_Origine_out output,
                                                @va_Confirme_out output,
                                                @va_Autoroute_out output,
                                                @va_PR_out output,
                                                @va_Sens_out output,
                                                @va_PointCar_out output,
                                                @va_NomPtCar_out output,
                                                @va_VR_out output,
                                                @va_VM2_out output,
                                                @va_VM1_out output,
                                                @va_VL_out output,
                                                @va_BAU_out output,
                                                @va_VR_I_out output,
                                                @va_VM2_I_out output,
                                                @va_VM1_I_out output,
                                                @va_VL_I_out output,
                                                @va_BAU_I_out output,
                                                @va_AutorouteCause_out output,
                                                @va_PRCause_out output,
                                                @va_SensCause_out output,
                                                @va_HoroCause_out output,
                                                @va_Position_out output,
                                                @va_Degats_out output,
                                                @va_TypeAnterieur_out output,
                                                @va_FausseAlerte_out output,
                                                @va_BAU_Etroite_out output,
                                                @va_BAU_I_Etroite_out output,
                                                @va_Longueur_out output,
                                                @va_AbrPtCar_out output,
                                                @va_NumPtCar_out output,
                                                @va_Degrade_out output,
                                                @va_NumEvtInit_out output,
                                                @va_CleEvtInit_out output,
                                                @va_ComEvtInit_out output,
                                                @va_TypeEvtInit_out output,
                                                @va_AutorouteEvtInit_out output,
                                                @va_PREvtInit_out output,
                                                @va_SensEvtInit_out output,
                                                @va_HoroEvtInit_out output,
                                                @va_SiteAlerte_out output,
                                                @va_Datex_out output,
                                                @va_Trafic_out output,
                                                @va_Gravite_out output,
                                                @va_Bloquant_out output,
                                                @va_Duree_out output,
                                                @va_Majeur_out output,
                                                @va_Ecoulement_out output

	else 
                return XDC_NOK

	return @vl_Status
go
