/*E*/
/*  Fichier : $Id: xzag26sp.prc,v 1.4 2011/09/23 14:07:40 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2011/09/23 14:07:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag26sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/08/95	: Creation	(V 1.1)
* C.T.	09/01/96	: Supprimer le site dans la clause 
*                         de suppression (V 1.2)
* P.C.	06/02/02	: Correction pour HIS V1.3
* JMG	22/09/11	: purge ALT_LOC DEM 980 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des alertes de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG26_Purge_Alertes_Base_Historique
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

create procedure XZAG;26
	@va_Horodate_in		datetime = null
as
#ifdef HIST
	declare @vl_Site T_SITE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Effacer toutes les alertes non liees a un evenement datant */
	/*A de plus que la date donnee                                 */
	delete HIS..ALT_EXP where evenement is null and 
				  horodate <= @va_Horodate_in and
				  qualification != XDC_ALT_VRAIE_EVT and
				  qualification != XDC_ALT_CONF_EVT

	delete HIS..ALT_LOC where numero not in (select numero from HIS..ALT_EXP)

#endif
	return XDC_OK
go
