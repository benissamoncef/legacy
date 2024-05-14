/*E*/
/*  Fichier : $Id: xzae140sp.prc,v 1.3 2015/07/07 13:46:44 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2015/07/07 13:46:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae127sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : infos de la fmc vehicule
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	07 oct 1997 : creation
* PNI 	6/07/2015	:: Suppression de la transmission des commentaires v1.3 DEM1119
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE139_Infos_Vehicule_Pour_Strada
*
* Arguments en entree
* - numero de la FMC
* - cle de la FMC
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos obligatoires pour generer
* un fichier STRADA
-------------------------------------------------------*/

use PRC
go
drop proc XZAE140
go
create procedure XZAE140
	@va_numero_in		int	= null,
	@va_cle_in		tinyint = null,
	@va_type_out 		tinyint output,
	@va_volume_out		smallint output,
	@va_matieres_out	char(6) output,
	@va_bebes_out		smallint output,
	@va_agees_out		smallint output,
	@va_animaux_out		bit output,
	@va_escota_out		bit output,
	@va_observations_out	char(150) output
as
	if @va_numero_in is null
		return XDC_ARG_INV
		
	if @va_cle_in is null
		return XDC_ARG_INV
	
	select 	@va_type_out = type_de_vehicule,
		@va_volume_out = volume_de_chargement,
		@va_matieres_out = rtrim(matieres_dangereuses),
		@va_bebes_out = nb_bebes,
		@va_agees_out = nb_agees,
		@va_animaux_out = animaux_vivants,
		@va_escota_out = vehicule_ESCOTA,
		@va_observations_out = "" /*rtrim(commentaire) suppression commentaire */
	from EXP..FMC_VEH
	where 
		numero = @va_numero_in and
		cle = @va_cle_in 
	
	return XDC_OK
go

