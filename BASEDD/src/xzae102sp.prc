/*X  Fichier : $Id: xzae102sp.prc,v 1.2 2007/06/08 14:37:47 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/06/08 14:37:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae102sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/09/96 : Creation
* JPL	08/06/07 : Ajout cas des FMCs Basculement pour travaux (DEM 647) 1.2
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transferer les fmc en prevision debutant dans moins de 24 h
* au CI pour que le CI les passe dans la liste a traiter
*
* Sequence d'appel
* SP    XZAE102_Travaux_A_Mettre_En_Cours
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* a executer au PC simplifie uniquement
*
* Fonction
---------------------------------------------------*/

use PRC
go

#ifdef PC_SIMPL
create procedure XZAE;102
as
	declare @vl_DateButee datetime, @vl_SiteMaitre T_SITE

	select @vl_DateButee = dateadd(hour, 10, getdate())

	/* initialisation variable locale */
	select @vl_SiteMaitre = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

	begin tran

	/*A rechercher les complements des fmc en prevision debutant dans moins de 10h */
	/*A pour les transferer au CI */
	update EXP..FMC_TRF set  sit=@vl_SiteMaitre, zz = zz+1
	where numero in
		(select numero from EXP..FMC_GEN
		 where (type = XZAEC_FMC_Travaux  or  type = XZAEC_FMC_BasculTravaux) and 
		       debut is null and debut_prevu <= @vl_DateButee and
		       sit != @vl_SiteMaitre)

	/*A rechercher les fmc en prevision debutant dans moins de 10h */
	/*A pour les transferer au CI */
	update EXP..FMC_GEN set  sit=@vl_SiteMaitre, poste_enrichisseur = null, zz = zz+1
	where (type = XZAEC_FMC_Travaux  or  type = XZAEC_FMC_BasculTravaux) and
	      debut is null and debut_prevu <= @vl_DateButee and
	      sit != @vl_SiteMaitre

	commit tran

	return XDC_OK
go
#endif
