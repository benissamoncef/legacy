/*E*/
/*  Fichier : $Id: xzag21sp.prc,v 1.7 2000/07/21 16:58:50 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2000/07/21 16:58:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag22sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	10/08/95	: Modif facon de supprimer	(V 1.2)
* C.T.	11/12/95	: Modif clause de recherche de station	(V 1.3)
* C.T.	06/09/96	: Ajout test pour serveur DC et DC (V 1.4)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.5) (dem / 1536)
* PN	27/01/1999	: ajout MES_ADA dem/1724 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures niveau de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG21_Purge_Mesures_Niveau_Base_Exploitation
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;21
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE, @vl_Station smallint,
		@vl_Sens T_SENS

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
           	return XDC_NOK

	/*A Effacer toutes les mesures de niveau anterieures a la date */
	/*A donnee sur le site local                                   */
	/*! (utiliser l'index de la table des mesures) */
	declare Pointeur_station cursor
	for select distinct
		station,
		sens
	from CFG..RES_CHA

	open Pointeur_station
	fetch Pointeur_station into @vl_Station, @vl_Sens
	while @@sqlstatus = 0
	begin
		delete EXP..MES_NIV 
		where station = @vl_Station and
			sens = @vl_Sens and 
			horodate <= @va_Horodate_in and sit = @vl_Site

		fetch Pointeur_station into @vl_Station, @vl_Sens
	end
	close Pointeur_station

#ifdef CI
	delete EXP..MES_ADA 
	where horodate <= @va_Horodate_in 
#endif
	return XDC_OK
go
