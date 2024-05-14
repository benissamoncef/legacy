/*X  Fichier : $Id: xzae10sp.prc,v 1.6 2020/11/03 16:12:47 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/11/03 16:12:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae10.sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth�se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/07/95	: Creation
* C.T.	27/09/96	: Annuler une fmc prevue sauf bouchon (FMC6) (V1.2)
* C.T.	10/10/96	: Positionner la date de cloture XDC_DATE_CLOTURE_PURGE 
*                         (DEM 1233:FMC15) (V1.3)
* P.N.  24/12/96	: Mise en conformite de l'interface xzae11 (1.4)
* JPL	06/04/07	: Changement de nom de la procedure xzae11 appelee (1.5)
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/


/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP    XZAE10_Annuler_FMC_En_Prevision
*
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Machine	va_PosteOperateur_in		: numero de la machine de l'operateur
* XDY_Octet	va_Prevision_Traiter_Clos_in	: etat de l'evenement
* XDY_Mot	va_Type_in
* XDY_Horodate	va_DebutPrevu_in
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_FinPrevu_in
* XDY_Horodate	va_Fin_in
* XDY_Horodate	va_Validation_in
* XDY_Entier	va_NumCause_in
* XDY_Octet	va_CleCause_in
* XDY_Alerte	va_NumAlerte_in
* XDY_Nom	va_Origine_in
* XDY_Eqt	va_PosteEnrich_in		: numero de la machine de l'enrichisseur
* XDY_Booleen	va_ConfirmeSignale_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Octet	va_PtCaracteristique_in		: numero du point caracteristique
* XDY_Octet	va_TypePtCaracteristique_in	: type du point caracteristique
* XDY_Octet	va_Position_in			: position sur aire ou echangeur ou lit
* XDY_Voie	va_VR_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_BAU_I_in
* Arguments en sortie
*
* Service rendu
* Met en base la fiche correspondante
*
* Code retour
* XDC_OK
* XDC_NOK               : alerte inconnue, cause de l'evenement inconnue, insertion
*                         impossible, declencher d'un reveil impossible
* XDC_ARG_INV           : argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC           : procedure stockee n'existe pas en base
* code d'erreur de XZAE11 ET XZA14
* < 0                   : erreur sybase
*
* Conditions d'utilisation
*
* Fonction
-------------------------------------------------------*/

use PRC
go

create procedure XZAE;10
	@va_NumEvenement_in             int     = null,
	@va_CleEvenement_in             tinyint = null,
	@va_PosteOperateur_in           smallint = null,
	@va_Prevision_Traiter_Clos_in	tinyint	= null,
	@va_Type_in			smallint= null,
	@va_DebutPrevu_in		datetime = null,
	@va_Debut_in			datetime = null,
	@va_FinPrevu_in			datetime = null,
	@va_Fin_in			datetime = null,
	@va_Validation_in		datetime = null,
	@va_NumCause_in			int	= null,
	@va_CleCause_in			tinyint	= null,
	@va_NumAlerte_in		int	= null,
	@va_Origine_in			T_NOM	= null,
	@va_PosteEnrich_in		smallint = null,
	@va_ConfirmeSignale_in		bit,
	@va_Autoroute_in		T_AUTOROUTE = null,
	@va_PR_in			T_PR	= null,
	@va_Sens_in			T_SENS	= null,
	@va_PtCaracteristique_in	tinyint	= null,
	@va_TypePtCaracteristique_in	tinyint = null,
	@va_Position_in			tinyint = null,
	@va_VR_in			tinyint	= null,
	@va_VM2_in			tinyint	= null,
	@va_VM1_in			tinyint	= null,
	@va_VL_in			tinyint	= null,
	@va_BAU_in			tinyint	= null,
	@va_VR_I_in			tinyint	= null,
	@va_VM2_I_in			tinyint	= null,
	@va_VM1_I_in			tinyint	= null,
	@va_VL_I_in			tinyint	= null,
	@va_BAU_I_in			tinyint	= null,
	@va_Degats_in			tinyint	= null,
	@va_NomSiteLocal_in             char(2) = null
as
	declare @vl_DateCourante datetime, @vl_resultat int, 
		@vl_Status int

	/*A rechercher la date courante */
	if @va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_PREV or
	   @va_Type_in = XZAEC_FMC_TeteBouchon or
	   @va_Type_in = XZAEC_FMC_QueueBouchon
		return XDC_ARG_INV

	/*A rechercher la date courante */
	select @vl_DateCourante =getdate()

	/*A positionner la date de fin */
	exec @vl_Status = XZAE11 @va_NumEvenement_in, @va_CleEvenement_in, 
				@va_PosteOperateur_in,@va_Prevision_Traiter_Clos_in,
				@va_Type_in, @va_DebutPrevu_in, NULL,
				@va_FinPrevu_in, @vl_DateCourante,
				@va_Validation_in, @va_NumCause_in,
				@va_CleCause_in, @va_NumAlerte_in, 
				@va_Origine_in, @va_PosteEnrich_in,
				@va_ConfirmeSignale_in, @va_Autoroute_in,
				@va_PR_in, @va_Sens_in, @va_PtCaracteristique_in,
				@va_TypePtCaracteristique_in, @va_Position_in,
				@va_VR_in, @va_VM2_in, @va_VM1_in,
				@va_VL_in, @va_BAU_in, @va_VR_I_in,
				@va_VM2_I_in, @va_VM1_I_in, @va_VL_I_in,
				@va_BAU_I_in, null,null,@va_Degats_in, @va_Type_in,XDC_FAUX,
				NULL, NULL, NULL, NULL, NULL, @va_NomSiteLocal_in

	if @vl_Status != XDC_OK
		return @vl_Status

	/*A achever la fmc en prevision */
	/*exec @vl_Status = XZAE;14 @va_NumEvenement_in, @va_CleEvenement_in, 
				@va_PosteOperateur_in,@vl_resultat output

	if @vl_Status != XDC_OK
		return @vl_Status
	if @vl_resultat != XDC_OK
		return @vl_resultat
*/
	/*A modifier la date de cloture a XDC_DATE_CLOTURE_PURGE */
	update EXP..FMC_GEN set cloture = XDC_DATE_CLOTURE_PURGE, zz = zz+1
	where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and cloture is not null

	return XDC_OK
go
