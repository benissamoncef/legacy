/*E*/
/*  Fichier : $Id: xzac40sp.prc,v 1.10 2019/01/07 14:50:53 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2019/01/07 14:50:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac40sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	14/11/94	: Modif recherche infos pour CI (V 1.4)
* C.T.	09/12/94	: Modif clause de lecture d'action (V 1.5)
* P.V.  14/08/96	: Ajout lecture ligne direction (V1.5) (ANA30)
*                         non teste
* C.P.	10/06/98	: Separation EXP et HIS (V 1.7)
* JPL	21/07/10	: Au CI ou sur secteur, si l'action est introuvable, executer sur HIS (DEM 941) 1.8
* JPL	01/09/10	: ... et retour compte-rendu OK si l'evenement est trouve 1.9
* JMG	08/08/18	: ligne 18 car suppression SAGA DEM1306 1.10
* GGY	28/04/23 : Augmentation taille va_TextePicto_in a char(XDC_LGR_TXT_PICTO) et va_TypePicto_out (XDC_LGR_NOM_PICTO) (DEM-473)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC40_Lire_Commande_PMV
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_LignePMV	va_Ligne1_out
* XDY_LignePMV	va_Ligne2_out
* XDY_LignePMV	va_Ligne3_out
* XDY_LignePMV	va_Alternat1_out
* XDY_LignePMV	va_Alternat2_out
* XDY_LignePMV	va_Alternat3_out
* XDY_Booleen	va_Clignotement1_out
* XDY_Booleen	va_Clignotement2_out
* XDY_Booleen	va_Clignotement3_out
* char[XDC_LGR_NOM_PICTO]	va_TypePicto_out
* char[XDC_LGR_TXT_PICTO]	va_TextePicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* XDY_LignePMV	va_LigneDir_out
* XDY_LignePMV	va_AlternatDir_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si les informations n'existent pas dans
* EXP tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche d'infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* 
* Fonction
* Select dans les tables ACTION et E_PMV
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAC;40
	@va_NumAction_in		int		= null,
	@va_Site_in			T_SITE		= null,
	@va_NumEqt_out			T_EQUIPEMENT	= null		output,
	@va_Ligne1_out			char(18)	= null		output,
	@va_Ligne2_out			char(18)	= null		output,
	@va_Ligne3_out			char(18)	= null		output,
	@va_Alternat1_out		char(18)	= null		output,
	@va_Alternat2_out		char(18)	= null		output,
	@va_Alternat3_out		char(18)	= null		output,
	@va_Clignotement1_out		bit		= XDC_NON	output,
	@va_Clignotement2_out		bit		= XDC_NON	output,
	@va_Clignotement3_out		bit		= XDC_NON	output,
	@va_TypePicto_out		char(XDC_LGR_NOM_PICTO)		= null		output,
	@va_TextePicto_out		char(XDC_LGR_TXT_PICTO)		= null		output,
	@va_ClignotementPicto_out	bit		= XDC_NON	output,
	@va_HeureLancement_out		datetime	= null		output,
	@va_HeureLancementFin_out	datetime	= null		output,
	@va_HeureSucces_out		datetime	= null		output,
	@va_HeureEchec_out		datetime	= null		output,
	@va_HeureFin_out		datetime	= null		output,
	@va_LigneDir_out		char(18)	= null		output,
	@va_AlternatDir_out		char(18)	= null		output
as
	declare	@vl_NbEnrg	int,
		@vl_Status	int

	/*! transfo en param�tres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A Controle des parametres d'entree */
	if @va_NumAction_in = null or @va_Site_in = null
		return XDC_ARG_INV

	/*A lecture de l'action */

#ifdef HIST

	select 
		@va_NumEqt_out		= HIS..ACT_GEN.equipement,
		@va_Ligne1_out		= HIS..ACT_PMV.premiere_ligne,
		@va_Ligne2_out		= HIS..ACT_PMV.deuxieme_ligne,
		@va_Ligne3_out		= HIS..ACT_PMV.troisieme_ligne,
		@va_Alternat1_out	= HIS..ACT_PMV.premiere_ligne_alternat,
		@va_Alternat2_out	= HIS..ACT_PMV.deuxieme_ligne_alternat,
		@va_Alternat3_out	= HIS..ACT_PMV.troisieme_ligne_alternat,
		@va_Clignotement1_out	= HIS..ACT_PMV.clignotement1,
		@va_Clignotement2_out	= HIS..ACT_PMV.clignotement2,
		@va_Clignotement3_out	= HIS..ACT_PMV.clignotement3,
		@va_TypePicto_out	= HIS..ACT_PMV.pictogramme,
		@va_TextePicto_out	= HIS..ACT_PMV.bandeau,
		@va_ClignotementPicto_out= HIS..ACT_PMV.clignotement_picto,
		@va_HeureLancement_out	= HIS..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out= HIS..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out	= HIS..ACT_GEN.heure_succes,
		@va_HeureEchec_out	= HIS..ACT_GEN.heure_echec,
		@va_HeureFin_out	= HIS..ACT_GEN.heure_fin,
		@va_LigneDir_out	= HIS..ACT_PMV.direction_ligne,
		@va_AlternatDir_out	= HIS..ACT_PMV.direction_ligne_alternat
	from HIS..ACT_PMV, HIS..ACT_GEN
	where	HIS..ACT_GEN.numero = @va_NumAction_in and 
		HIS..ACT_GEN.type = XDC_ACT_PMV and
		HIS..ACT_GEN.sit = @va_Site_in and
		HIS..ACT_PMV.actio = @va_NumAction_in and 
		HIS..ACT_PMV.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

#else

	select 
		@va_NumEqt_out			= EXP..ACT_GEN.equipement,
		@va_Ligne1_out			= EXP..ACT_PMV.premiere_ligne,
		@va_Ligne2_out			= EXP..ACT_PMV.deuxieme_ligne,
		@va_Ligne3_out			= EXP..ACT_PMV.troisieme_ligne,
		@va_Alternat1_out		= EXP..ACT_PMV.premiere_ligne_alternat,
		@va_Alternat2_out		= EXP..ACT_PMV.deuxieme_ligne_alternat,
		@va_Alternat3_out		= EXP..ACT_PMV.troisieme_ligne_alternat,
		@va_Clignotement1_out		= EXP..ACT_PMV.clignotement1,
		@va_Clignotement2_out		= EXP..ACT_PMV.clignotement2,
		@va_Clignotement3_out		= EXP..ACT_PMV.clignotement3,
		@va_TypePicto_out		= EXP..ACT_PMV.pictogramme,
		@va_TextePicto_out		= EXP..ACT_PMV.bandeau,
		@va_ClignotementPicto_out	= EXP..ACT_PMV.clignotement_picto,
		@va_HeureLancement_out		= EXP..ACT_GEN.heure_lancement,
		@va_HeureLancementFin_out	= EXP..ACT_GEN.heure_lancement_fin,
		@va_HeureSucces_out		= EXP..ACT_GEN.heure_succes,
		@va_HeureEchec_out		= EXP..ACT_GEN.heure_echec,
		@va_HeureFin_out		= EXP..ACT_GEN.heure_fin,
		@va_LigneDir_out		= EXP..ACT_PMV.direction_ligne,
		@va_AlternatDir_out		= EXP..ACT_PMV.direction_ligne_alternat
	from EXP..ACT_PMV, EXP..ACT_GEN
	where	EXP..ACT_GEN.numero = @va_NumAction_in and 
		EXP..ACT_GEN.type = XDC_ACT_PMV and
		EXP..ACT_GEN.sit = @va_Site_in and
		EXP..ACT_PMV.actio = @va_NumAction_in and 
		EXP..ACT_PMV.sit = @va_Site_in

	select @vl_NbEnrg = @@rowcount

	/*A si les infos ne sont pas trouvees     */
	/*A alors les rechercher dans la base HIS */
	if @vl_NbEnrg = 0
	begin
		exec @vl_Status = SQL_HIS.PRC..XZAC;40
					@va_NumAction_in,
					@va_Site_in,
					@va_NumEqt_out			output,
					@va_Ligne1_out			output,
					@va_Ligne2_out			output,
					@va_Ligne3_out			output,
					@va_Alternat1_out		output,
					@va_Alternat2_out		output,
					@va_Alternat3_out		output,
					@va_Clignotement1_out		output,
					@va_Clignotement2_out		output,
					@va_Clignotement3_out		output,
					@va_TypePicto_out		output,
					@va_TextePicto_out		output,
					@va_ClignotementPicto_out	output,
					@va_HeureLancement_out		output,
					@va_HeureLancementFin_out	output,
					@va_HeureSucces_out		output,
					@va_HeureEchec_out		output,
					@va_HeureFin_out		output,
					@va_LigneDir_out		output,
					@va_AlternatDir_out		output
		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end

#endif

	if @vl_NbEnrg != 1
		return XDC_NOK

	return XDC_OK
go
