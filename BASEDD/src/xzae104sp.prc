/*X  Fichier : $Id: xzae104sp.prc,v 1.3 2020/11/03 16:11:45 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 16:11:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae104sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/09/96	: Creation
* JMG	07/01/98	: test cle = site local(dem/1535) 1.2
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Rechercher les fmc operateur en cours
*
* Sequence d'appel
* SP    XZAE104_FMC_Operateur_En_Cours
*
* Arguments en entree
*
* Arguments en sortie
*
* XDY_Entier	numero
* XDY_Octet	cle
* XDY_Mot	type
* XDY_Horodate	debut
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

create procedure XZAE;104
        @va_NomSiteLocal_in             char(2)         = null
as
declare @vl_Site tinyint

	/* recherche le site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in


	/*A rechercher les fmc operateur en cours */
	select
		numero,
		cle,
		type,
		debut,
		poste_enrichisseur
	from EXP..FMC_GEN
	where type = XZAEC_FMC_PosteOperateur and cle = @vl_Site and fin is null and cloture is null

	return XDC_OK
go
