/*E*/
/*  Fichier : $Id: xzag17sp.prc,v 1.4 2016/09/28 11:42:00 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2016/09/28 11:42:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	07/01/98	: modif condition de purge au CI uniquement :
*                         toutes les alertes fausse ou vrai sont
*                         supprimees sans condition de date (1532) (V 1.2)
* JMG	22/09/11	: purge ALT_LOC DEM 980 1.3
* JMG	27/09/16	: purge alarmes inactives FACTIS 1.4
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des alertes de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG17_Purge_Alertes_Base_Exploitation
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: site local inconnu
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;17
	@va_Horodate_in		datetime = null
as
	declare @vl_Site T_SITE, @vl_SiteMaitre T_SITE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Recherche du numero du site local */
	select @vl_Site = numero
	from CFG..RES_DIS
	where serveur = @@servername

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	if @vl_Site is null
		return XDC_NOK

	/*A Effacer toutes les alertes non liees a un evenement datant */
	/*A de plus que la date donnee sur le site local               */
	/*A Au CI : pas de condition de date sur ces alertes vraies ou fausses ou exploitation */
	delete EXP..ALT_LOC where sit = @vl_Site and 
	numero in (select numero from EXP..ALT_EXP
			where evenement is null and sit = @vl_Site and
				((@vl_Site = @vl_SiteMaitre and 
				qualification is not null and 
				qualification != XDC_ALT_INCONNUE) OR
				horodate <= @va_Horodate_in) and
				qualification != XDC_ALT_VRAIE_EVT and
				qualification != XDC_ALT_CONF_EVT)

	delete EXP..ALT_EXP where evenement is null and sit = @vl_Site and
				((@vl_Site = @vl_SiteMaitre and 
				qualification is not null and 
				qualification != XDC_ALT_INCONNUE) OR
				horodate <= @va_Horodate_in) and
				qualification != XDC_ALT_VRAIE_EVT and
				qualification != XDC_ALT_CONF_EVT

	delete EXP..ALM_EQT where date_dernier_changement <= @va_Horodate_in
				and etat = XDC_FAUX

	return XDC_OK
go
