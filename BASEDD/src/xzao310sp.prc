/*X  Fichier : $Id: xzao310sp.prc,v 1.2 2017/06/20 16:38:44 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2017/06/20 16:38:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao310sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/06/95	: Creation
* PNI	06/06/17	: Ajout des cas RADT SAGA 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne les informations sur une station RADT
*
* Sequence d'appel
* SP    XZAO310_Lire_PointDeMesure
*
* Arguments en entree
* XDY_Eqt	va_NumStation_in
*
* Arguments en sortie
*
* Liste retournee :
* XDY_Sens		sens
* XDY_Voie		voie
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
drop proc  XZAO310
go
create procedure XZAO310
	@va_NumStation_in	T_EQUIPEMENT	= null
as
	declare @vl_Chaussee T_VOIE

	if @va_NumStation_in = 0 select @va_NumStation_in = null

	/*A Controle la validite des parametres d'entree */
	if @va_NumStation_in is null
		return XDC_ARG_INV

	/* initialisation de la variable de chaussee */
	select @vl_Chaussee = XDC_RADT_CHAUSSEE

	/* creation d'une table de stockage des informations */
	create table #LISTE_POINTS (sens tinyint, voie tinyint)

	/*A Rechercher des points de mesure de la station */
	insert #LISTE_POINTS (sens, voie)
	select
		sens,
		voie
	from CFG..EQT_PNT_MES
	where station = @va_NumStation_in
	order by sens,voie

	/*A rajout du point de mesure sur chaussee pour tous les sens trouves */
	insert #LISTE_POINTS (sens, voie)
	select distinct 
		sens,
		@vl_Chaussee
	from #LISTE_POINTS
	

/* pour les station SAGA on force  en chaussée */
 	if @@rowcount=0
	begin
	insert #LISTE_POINTS (sens, voie) 
	select distinct 
		sens,
		@vl_Chaussee
	from CFG..RES_CHA where station=@va_NumStation_in 
	order by sens
	end
	
	/*A retourner les informations */
	select
		sens,
		voie
	from #LISTE_POINTS
	order by sens,voie

	return XDC_OK
go
