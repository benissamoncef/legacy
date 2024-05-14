/*E*/
/*  Fichier : $Id: xzae181sp.prc,v 1.2 2020/11/03 16:15:09 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 16:15:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	06/11/07	creation DEM/716
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des postes operateurs
* sur le site local pour la transmission du droit d enrichir
-------------------------------------------------------
* Sequence d'appel
* SP	XZAE181_Liste_Fiches_Operateur_Site_Local
* 
* Arguments en entree
* XDY_Machine	va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XDC_ACT_REFUSEE	: action impossible pas de voisin connecte
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* ne fonctionne pas sur pc simplifie 
* 
* Fonction
*
----------------------------------------------------*/

use PRC
go
drop procedure XZAE181
go

create procedure XZAE181
	@va_PosteOperateur_in		smallint= null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE
	
	/*! transfo en paramÉtres nulls bug applix */
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	
	/*A controle des arguments obligatoires */
	if @va_PosteOperateur_in = null 
		return XDC_ARG_INV

	/*A recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	select numero, nom from CFG..EQT_GEN
	where type = 13
	and numero in (select poste_enrichisseur
			from EXP..FMC_GEN, EXP..FMC_TRT
			where EXP..FMC_GEN.numero=EXP..FMC_TRT.evenement
			and EXP..FMC_GEN.cle = EXP..FMC_TRT.cle
			and type = 4
			and EXP..FMC_GEN.sit = @vl_Site
			and fin is null
			and poste_enrichisseur <> @va_PosteOperateur_in)

	return XDC_OK
go
