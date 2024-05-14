/*E*/
/*  Fichier : @(#)xzae551.prc	1.0      Release : 1.0        Date : 01/04/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae551.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  01/04/2004 :  Creation
* F.Lizot  27/06/2005 :  Correction SAGA DEM473 1.2
* PNI	06/07/2016 : Droit d'enrichir force a OUI pour saga 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Determine le droit de propriete d'une FMC 
* 
* Sequence d'appel
* SP	XZAE551_Droit_Propriete
* 
* Arguments en entree
* XDY_Entier    va_evenement_in
* XDY_Octet     va_cle_in
*
* Arguments en sortie
* XDC_Booleen	va_propriete_fmc_out
* XDC_Booleen   va_propriete_queue_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Determine le droit de propriete d'une FMC DATEX
* Pour une FMC creee par ESCOTA,
* ESCOTA possede le droit de propriete si la localisation n'est pas sur le reseau de la DDE83
* Pour une FMC DATEX dont l'emetteur est la DDE83,
* ESCOTA possede le droit de propriete si la localisation n'est pas sur le reseau de la DDE83
* Pour une FMC DATEX dont l'emetteur n'est pas la DDE83,
* ESCOTA ne possede pas le droit de propriete 
* 
-----------------------------------------------------*/

use PRC
go
drop proc XZAE551
go

create procedure XZAE551
	@va_evenement_in        int        	= null,
	@va_cle_in              tinyint 	= null,
	@va_propriete_fmc_out   bit             = XDC_NON output,
	@va_propriete_queue_out bit             = XDC_NON output

as
	declare @vl_autoroute tinyint,
		@vl_PR        int,
		@va_emetteur  char(10),
		@vl_type      smallint,
		@vl_cause     int,
		@vl_cleCause  tinyint

	select @va_propriete_fmc_out   = XDC_OUI
	select @va_propriete_queue_out = XDC_OUI

	return XDC_OK
go
