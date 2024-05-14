/*E*/
/*  Fichier : $Id: xzae641sp.prc,v 1.2 2020/11/24 08:46:46 gesconf Exp $      Release : $Revision: 1.2 $       Date :  $Date: 2020/11/24 08:46:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae64sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	07/10/09	: creation DEM/835
* PNI   04/10/21	: diffusion_datex à 2 dans tous les cas SAE-337
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Memorise qu'une FMC a ete completement diffusee par les passerelles (generiques)
* 
* Sequence d'appel
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Octet		va_Diffusion_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table POSTE_OPERATEUR
* S'il s'agit de la creation de la FMC, reconstruire la liste 
* des evenements a traiter pour les 24 prochaines heures (XZAE18)
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE641' and type = 'P')
	drop procedure XZAE641
go

create procedure XZAE641
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Diffusion_in	tinyint	= null
as
	declare @vl_Erreur tinyint, @vl_diff int

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Diffusion_in = 0 select @va_Diffusion_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_Diffusion_in = null
		return XDC_ARG_INV

	/*A verifier l'existance de l'evenement fini! et la concordance */
	/*A du poste enrichisseur                                 */
	if not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in and fin != null)
		return XDC_NOK

	begin tran
			/*A modification de la fiche generique */
			select @vl_diff = diffusion_datex
			from EXP..FMC_HIS_DER
			where   numero = @va_NumEvenement_in and
			cle = @va_CleEvenement_in

			if @vl_diff=0
				begin
				update EXP..FMC_HIS_DER set 
					diffusion_datex=2
				where	numero = @va_NumEvenement_in and 
					cle = @va_CleEvenement_in
			end
			else
				begin
				update EXP..FMC_HIS_DER set
					diffusion_datex=2
				where   numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in
			end
		if @@rowcount != 1
			select @vl_Erreur = XDC_OUI
	commit tran
	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	return XDC_OK

go
