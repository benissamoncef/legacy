/*E*/
/*  Fichier : $Id: xzag22sp.prc,v 1.4 1998/01/08 16:18:39 penot Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/01/08 16:18:39 $
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
* C.T.	10/08/95	: Modif numero de proc	(V 1.2)
* C.T.	06/09/96	: Ajout test pour serveur DC et DC (V 1.3)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.4) (dem / 1536)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures poids de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG22_Purge_Mesures_Poids_Base_Exploitation
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

create procedure XZAG;22
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_Site is null
           	return XDC_NOK

	/*A Effacer toutes les mesures de trafic anterieures a la date */
	/*A donnee sur le site local                                   */
	delete EXP..MES_POI where sit = @vl_Site and horodate <= @va_Horodate_in

	return XDC_OK
go
