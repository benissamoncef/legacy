/*E*/
/*  Fichier : $Id: xzag27sp.prc,v 1.7 2018/09/07 09:38:39 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/09/07 09:38:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag27sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	10/08/95	: Creation	(V 1.1)
* C.T.	09/01/96	: Supprimer le site dans la clause de suppression (V 1.2)
* P.C.	06/03/02	: Correction pour HIS V1.3         
* JPL	16/03/17	: Ajout de la purge des etats de mise au neutre des PMV / PMVA (DEM 1196)  1.4
* JPL	20/04/17	: Ajout de la purge des tracages des basculements RAU (DEM 1225)  1.5
* JPL	21/07/17	: Ajout de la purge des tracages de la messagerie SAE (DEM 1229)  1.6
* JPL	05/09/18	: Ajout de la purge des tracages de consulattion des FMC (DEM 1300)  1.7
* GGY	28/02/23	: ajout purge MGA_MSG_PRG => Annule : table pas dans HIS
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des etats des eqts de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG27_Purge_Base_Historique
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
* Suppression des lignes vieilles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;27
	@va_Horodate_in		datetime = null
as
#ifdef HIST
	declare @vl_Site T_SITE

	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Effacer tous les etats de disponibilite des equipements   */
	/*A anterieurs a la date donnee et ne representant le dernier */
	/*A etat d'un equipement                                      */
	delete HIS..EQT_DSP where	horodate <= @va_Horodate_in and
				  	dernier = XDC_NON


	/*A Effacer tous les etats de mise au neutre des PMV / PMVA anterieurs a la date donnee */
	delete HIS..PMV_NTR where debut <= @va_Horodate_in


	/*A Effacer les tracages des basculements RAU anterieurs a la date donnee */
	delete HIS..BAS_RAU_TRA where debut <= @va_Horodate_in


	/*A Effacer les tracages des messages echanges entre operateurs du SAE anterieurs a la date donnee */
	delete HIS..MSG_TRA where horodate <= @va_Horodate_in


	/*A Effacer les tracages des consultations de FMC */
	delete HIS..FMC_TRA where heure_ouverture <= @va_Horodate_in

#endif

	return XDC_OK
go
