/* Fichier : $Id: xzao306sp.prc,v 1.6 2018/10/23 15:12:45 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/10/23 15:12:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao306sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/03/95	: Creation
* C.T.	24/08/95	: Modif ordre de retour (autoroute,PR,sens) (V 1.2)
* D.M.  05/12/96        : Modif pour filtre station virtuelle (DEM/1252) (V 1.3)
* D.M.  12/12/96        : Retour a la version 1.2 (DEM/1339) (V 1.4)
* JMG	23/05/12	: regul dem 1016
* JPL	15/10/18	: Filtrage des equipements marques comme supprimes SAUF EN HISTORIQUE (DEM 1305)  1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des RADT sur un troncon ou une portion
*
* Sequence d'appel
* SP    XZAO306_Liste_RADT
*
* Arguments en entree
* char(3)	va_Basedd_in
* XDY_Nom	va_NomTroncon_in
* XDY_Mot	va_NumPortion_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Eqt		numeroEqt
* XDY_Autoroute		autoroute
* XDY_Sens		sens
* XDY_PR		PR
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
* Si NomTroncon et NumPortion sont non values alors la liste des
* RADT de l'ensemble des autoroutes est retournee
* Si NomTroncon est value alors la liste des RADT du troncon est 
* retournee
* Si NumPortion est value alors la liste des RADT de la portion est 
* retournee
*
* Fonction
---------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO306' and type = 'P')
	drop procedure XZAO306
go


create procedure XZAO306
	@va_Basedd_in		char(3)	= null,
	@va_NomTroncon_in	T_NOM	= null,
	@va_NumPortion_in	smallint = null
as
	declare @vl_Autoroute T_AUTOROUTE, @vl_PRdebut T_PR, @vl_PRfin T_PR

	if @va_NumPortion_in = 0 select @va_NumPortion_in = null
	if @va_NomTroncon_in = XDC_CHAINE_VIDE select @va_NomTroncon_in = null

	/*A Controle la validite des parametres d'entree */
	if (@va_Basedd_in != XDC_BASE_CFG and @va_Basedd_in != XDC_BASE_CFT) or
	   (@va_NomTroncon_in is not null and @va_NumPortion_in is not null)
		return XDC_ARG_INV

	if @va_Basedd_in = XDC_BASE_CFG
	begin
		/*A Recherche des radt sur une portion */
		if @va_NumPortion_in is not null
		begin
			select
				@vl_Autoroute = autoroute,
				@vl_PRdebut = PR_debut,
				@vl_PRfin = PR_fin
			from  CFG..RES_POR
			where numero = @va_NumPortion_in
	
			select
				CFG..EQT_GEN.numero,
				CFG..EQT_GEN.autoroute,
				CFG..RES_CHA.sens,
				CFG..RES_CHA.PR,
				CFG..EQT_GEN.nom
			from CFG..EQT_GEN, CFG..RES_CHA
			where	type = XDC_EQT_RAD and 
#ifndef HIST
				eqt_supprime = XDC_FAUX and
#endif
				autoroute = @vl_Autoroute and
				CFG..EQT_GEN.PR > @vl_PRdebut and CFG..EQT_GEN.PR <= @vl_PRfin and
				station = CFG..EQT_GEN.numero
			order by CFG..EQT_GEN.autoroute,CFG..RES_CHA.PR,CFG..RES_CHA.sens
		end
	
		/*A Recherche des radt sur un troncon */
		else if @va_NomTroncon_in is not null
		begin
			select
				@vl_Autoroute = autoroute,
				@vl_PRdebut = PR_debut,
				@vl_PRfin = PR_fin
			from  CFG..RES_TRC
			where nom = @va_NomTroncon_in
	
			select
				CFG..EQT_GEN.numero,
				CFG..EQT_GEN.autoroute,
				CFG..RES_CHA.sens,
				CFG..RES_CHA.PR,
				CFG..EQT_GEN.nom
			from CFG..EQT_GEN, CFG..RES_CHA
			where	type = XDC_EQT_RAD and 
#ifndef HIST
				eqt_supprime = XDC_FAUX and
#endif
				autoroute = @vl_Autoroute and
				CFG..EQT_GEN.PR > @vl_PRdebut and CFG..EQT_GEN.PR <= @vl_PRfin and
				station = CFG..EQT_GEN.numero
			order by CFG..EQT_GEN.autoroute,CFG..RES_CHA.PR,CFG..RES_CHA.sens
		end
		/*A Recherche des radt sur une autoroute */
		else 
			select
				CFG..EQT_GEN.numero,
				CFG..EQT_GEN.autoroute,
				CFG..RES_CHA.sens,
				CFG..RES_CHA.PR,
				CFG..EQT_GEN.nom
			from CFG..EQT_GEN, CFG..RES_CHA
			where	type = XDC_EQT_RAD and
#ifndef HIST
				eqt_supprime = XDC_FAUX and
#endif
				station = CFG..EQT_GEN.numero
			order by CFG..EQT_GEN.autoroute,CFG..RES_CHA.PR,CFG..RES_CHA.sens
	end
#ifdef CI
	else
	begin
		/*A Recherche des radt sur une portion */
		if @va_NumPortion_in is not null
		begin
			select
				@vl_Autoroute = autoroute,
				@vl_PRdebut = PR_debut,
				@vl_PRfin = PR_fin
			from  CFT..RES_POR
			where numero = @va_NumPortion_in
	
			select
				CFT..EQT_GEN.numero,
				CFT..EQT_GEN.autoroute,
				CFT..RES_CHA.sens,
				CFT..RES_CHA.PR,
				CFT..EQT_GEN.nom
			from CFT..EQT_GEN, CFT..RES_CHA
			where	type = XDC_EQT_RAD and 
#ifndef HIST
				eqt_supprime = XDC_FAUX and
#endif
				autoroute = @vl_Autoroute and
				CFT..EQT_GEN.PR > @vl_PRdebut and CFT..EQT_GEN.PR <= @vl_PRfin and
				station = CFT..EQT_GEN.numero
			order by CFT..EQT_GEN.autoroute,CFT..RES_CHA.PR,CFT..RES_CHA.sens
		end
	
		/*A Recherche des radt sur un troncon */
		else if @va_NomTroncon_in is not null
		begin
			select
				@vl_Autoroute = autoroute,
				@vl_PRdebut = PR_debut,
				@vl_PRfin = PR_fin
			from  CFT..RES_TRC
			where nom = @va_NomTroncon_in
	
			select
				CFT..EQT_GEN.numero,
				CFT..EQT_GEN.autoroute,
				CFT..RES_CHA.sens,
				CFT..RES_CHA.PR,
				CFT..EQT_GEN.nom
			from CFT..EQT_GEN, CFT..RES_CHA
			where	type = XDC_EQT_RAD and 
#ifndef HIST
				eqt_supprime = XDC_FAUX and
#endif
				autoroute = @vl_Autoroute and
				CFT..EQT_GEN.PR > @vl_PRdebut and CFT..EQT_GEN.PR <= @vl_PRfin and
				station = CFT..EQT_GEN.numero
			order by CFT..EQT_GEN.autoroute,CFT..RES_CHA.PR,CFT..RES_CHA.sens
		end
		/*A Recherche des radt sur une autoroute */
		else 
			select
				CFT..EQT_GEN.numero,
				CFT..EQT_GEN.autoroute,
				CFT..RES_CHA.sens,
				CFT..RES_CHA.PR,
				CFT..EQT_GEN.nom
			from CFT..EQT_GEN, CFT..RES_CHA
			where	type = XDC_EQT_RAD and
#ifndef HIST
				eqt_supprime = XDC_FAUX and
#endif
				station = CFT..EQT_GEN.numero
			order by CFT..EQT_GEN.autoroute,CFT..RES_CHA.PR,CFT..RES_CHA.sens
	end
#endif

	return XDC_OK
go
