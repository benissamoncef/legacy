/*X  Fichier : $Id: vuem50sp.prc,v 1.3 1999/02/26 11:50:15 gaborit Exp $     Release : $Revision: 1.3 $        Date : %G
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem50sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/06/96	: Creation
* C.T.	24/02/98	: Modif suite pb default suite a passage en SQL 11.0.3.2 (V1.2)
* PN	25/02/99	: Modif intergest. dem/1724 v1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Nombre de PMV en service par mois
*
* Sequence d'appel
* SP    VUEM50_Nombre_PMV_En_Service_Par_Mois
*
* Arguments en entree
* XDY_Site	va_District_in
* XDY_Datetime	va_DateDebut_in
* XDY_Datetime	va_DateFin_in
*
* Arguments en sortie
*
* Liste retournee :
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

create procedure VUEM50
	@va_District_in		T_SITE = null,
	@va_DateDebut_in	datetime = null,
	@va_DateFin_in		datetime = null
as
	declare  @vl_HS bit, @vl_Inhibe bit, @vl_Desactive bit, @vl_Date datetime,
		@vl_DateSuivante datetime, @vl_Numero smallint, @vl_District T_SITE

	create table  #NB_PMV_DISTRICT (mois datetime, nombre int default 0)

	/*A Liste des PMV d'un district */
	if @va_District_in is not null and  @va_District_in <> 99
	begin
		/*B initialisation de la table des nombres de pmv */
		select @vl_Date = @va_DateDebut_in
		while @vl_Date <= @va_DateFin_in
		begin
			insert #NB_PMV_DISTRICT( mois, nombre)
			values(@vl_Date, 0)

			select @vl_DateSuivante = @vl_Date
			select @vl_Date = dateadd(month, 1, @vl_DateSuivante)
		end

		declare pointeur cursor
		for select distinct
			CFG..EQT_GEN.numero
		from CFG..EQT_GEN, CFG..RES_POR
		where type=XDC_EQT_PMV and
			CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
			CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
			CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR and
			CFG..RES_POR.district=@va_District_in
	end
	/*A Liste des PMV de tous les districts */
	else
	begin
		/*B initialisation de la table des nombres de pmv */
		select @vl_Date = @va_DateDebut_in
		while @vl_Date <= @va_DateFin_in
		begin
			insert #NB_PMV_DISTRICT(mois, nombre)
			values (@vl_Date, 0)

			select @vl_DateSuivante = @vl_Date
			select @vl_Date = dateadd(month, 1, @vl_DateSuivante)
		end


		declare pointeur cursor
		for select distinct
			CFG..EQT_GEN.numero
		from CFG..EQT_GEN, CFG..RES_POR
		where type=XDC_EQT_PMV and
			CFG..RES_POR.autoroute=CFG..EQT_GEN.autoroute and
			CFG..RES_POR.PR_debut<=CFG..EQT_GEN.PR and
			CFG..RES_POR.PR_fin>CFG..EQT_GEN.PR
		order by CFG..RES_POR.district
	end

	/*A selectionner uniquement les pmv en service */
	open pointeur 
	fetch pointeur into @vl_Numero
	while @@sqlstatus = 0
	begin
		select @vl_DateSuivante = dateadd(month, 1, @va_DateDebut_in), @vl_Date = @va_DateDebut_in

		/* tester si le pmv etait connecte au reseau durant le mois */
		while (@vl_Date <= @va_DateFin_in)
		begin
			if exists (select * from HIS..EQT_DSP 
				where equipement = @vl_Numero and type = XDC_EQT_PMV and horodate < @vl_DateSuivante)
			begin
				/* recuperer le dernier avant le debut du mois */
				select @vl_HS = HS, @vl_Inhibe = inhibe, @vl_Desactive = desactive from HIS..EQT_DSP 
				where equipement = @vl_Numero and type = XDC_EQT_PMV and horodate <= @vl_Date
				having horodate=max(horodate) and equipement = @vl_Numero and type = XDC_EQT_PMV 
	
				if  @vl_HS = 1 or @vl_Inhibe = 1 or @vl_Desactive = 1
				begin
					if exists (select * from HIS..EQT_DSP 
						where equipement = @vl_Numero and type = XDC_EQT_PMV and
						(HS = 0 and inhibe = 0 and desactive = 0) and 
						horodate > @vl_Date and horodate < @vl_DateSuivante)
	
						update #NB_PMV_DISTRICT set nombre = nombre+1
						where mois = @vl_Date
				end
				else
				begin
					update #NB_PMV_DISTRICT set nombre = nombre+1
					where mois = @vl_Date
				end
			end

			select @vl_Date = @vl_DateSuivante
			select @vl_DateSuivante = dateadd(month, 1, @vl_Date)
		end

		fetch pointeur into @vl_Numero
	end
	close pointeur

	/*A retour des resultats */
	select mois, nombre from #NB_PMV_DISTRICT
	order by mois

	return XDC_OK
go
