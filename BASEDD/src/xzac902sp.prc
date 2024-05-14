/*E*/
/* Fichier : $Id: xzac902sp.prc,v 1.2 2020/10/13 17:34:43 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/10/13 17:34:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac900.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Programmation des reveils sur les FMC avec action de pilotage en succes sur les CFE
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	17/04/18 	creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* LCL	20/11/19	MOVIS DEM-SAE93 1.2
* LCL 	03/10/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage CFE.
* 
* Sequence d'appel
* SP	XZAC900_Commande_CFE
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Site	va_Site_in
* XDY_Octet	va_PlanDeFeux_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* XDY_Site	va_site_origine_in
*
* Arguments en sortie
* XDY_Entier	va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* XDC_REV_PB_DCL: probleme pour declencher le reveil
* XDC_PRC_INC	: ARRP03 inexistante
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'action a ete proposee par le plan d'actions, positionner 
* Priorite et DistanceEvt, sinon, mettre DistanceEvt a XDC_DISTANCE_INCONNUE
* (Priorite sera mis automatiquement a null)
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence d'un equipement non indisponible 
* dans le district ou l'actio est executee./A
* 
* Inserer dans la table ACTION et la table E_CFE.
* 
* Chercher l'action precedente sur l'equipement a commander :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a Horodate pour l'action precedente
* 	Si l'action precedente trouvee est liee a un autre 
* 	evenement alors l'autre evenement doit 	etre reveille 
* 	pour l'avertir du vol (appeler XZAR03).
* 
* ACRP900 :	Appeler XZEP900 pour piloter l'equipement CFE
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC902' and type = 'P')
	drop procedure XZAC902
go

create procedure XZAC902
	@va_NomSiteLocal_in	char(2)	= null
as
	declare 
		@vl_Site tinyint,
		@vl_Status int,
		@vl_Intitule varchar(50),
		@vl_Texte char(100),
		@vl_NomMachine char(10),
		@vl_NomSite char(2),
		@vl_num int,
		@vl_cle tinyint,
		@vl_eqt T_EQUIPEMENT,
		@vl_sit tinyint,
		@vl_SiteCI tinyint

 
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

        if @vl_Site is null
                return XDC_NOK

	/* Recherche du site principal */
	select @vl_SiteCI=numero from CFG..RES_DIS where type=XDC_TYPEM_SD
	if @@rowcount != 1
		return XDC_NOK


	declare cfe cursor
	for
	select equipement, sit, evenement, cle, nom
	from EXP..ACT_GEN , CFG..EQT_CFE, CFG..EQT_GEN
	where EXP..ACT_GEN.type=XDC_EQT_CFE
	and  heure_fin is null
	and EXP..ACT_GEN.equipement = CFG..EQT_CFE.numero
	and CFG..EQT_GEN.numero = CFG..EQT_CFE.numero
	and CFG..EQT_GEN.type = XDC_EQT_CFE
	and heure_succes is not null
	and (sit_origine = @vl_Site or sit = @vl_Site or @vl_Site=@vl_SiteCI)
		
	open cfe

	fetch cfe into @vl_eqt, @vl_sit, @vl_num, @vl_cle,  @vl_Intitule

	while (@@sqlstatus = 0)
	begin

		select @vl_Texte = @vl_Intitule + " : plan de feux en cours"

			/* recherche du nom du site */
			select  @vl_NomSite = code
			from CFG..RES_DIS
			where numero = @vl_Site

			if @@rowcount != 1
				return XDC_NOK

			/* declencher le reveil pour le site local */
			if  @vl_Site = XDC_CI
				exec @vl_Status = TASRV_CI...ARRP03 
						@vl_num, 
						@vl_cle,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_VC
				exec @vl_Status = TASRV_VC...ARRP03 
						@vl_num, 
						@vl_cle,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_DP
				exec @vl_Status = TASRV_DP...ARRP03
						@vl_num,
						@vl_cle,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else if @vl_Site = XDC_CA
				exec @vl_Status = TASRV_CA...ARRP03
						@vl_num,
						@vl_cle,
						@vl_NomMachine,
						@vl_NomSite, @vl_Texte
			else return XDC_NOK


fetch cfe into @vl_eqt, @vl_sit, @vl_num, @vl_cle, @vl_Intitule
end
close cfe

	return XDC_OK
go
