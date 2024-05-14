/*E*/
/*  Fichier : $Id: xzag18sp.prc,v 1.5 2018/09/07 09:38:25 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/09/07 09:38:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	06/09/96	: Ajout test pour serveur DC et DC (V 1.2)
* C.P.	07/01/98	: Correction gestion multi-sites dans les procs (V 1.3) (dem / 1536)
* JPL	16/03/17	: Ajout de la purge des etats de mise au neutre des PMV / PMVA (DEM 1196)  1.4
* JPL	05/09/18	: Ajout de la purge des tracages de consulattion des FMC (DEM 1300)  1.5
* GGY	28/02/23	: ajout purge MGA_MSG_PRG => Annule : table pas dans HIS
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des etats des eqts de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG18_Purge_Base_Exploitation
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

create procedure XZAG;18
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

	/*A Effacer tous les etats de disponibilite des equipements   */
	/*A anterieurs a la date donnee et ne representant le dernier */
	/*A etat d'un equipement sur le site local                    */
	delete EXP..EQT_DSP where	sit = @vl_Site and 
					horodate <= @va_Horodate_in and
				  	dernier = XDC_NON


	/*A Effacer tous les etats de mise au neutre des PMV / PMVA anterieurs a la date donnee */
	/*! Les etats enregistres sur un site hors CI sont ceux des equipements du site local */
	delete EXP..PMV_NTR where debut <= @va_Horodate_in


	/*A Effacer toutes les traces des consultations de FMC */
	/*! Les etats enregistres sur un site sont celles des consultations sur le site local */
	delete EXP..FMC_TRA where heure_ouverture <= @va_Horodate_in

	return XDC_OK
go
