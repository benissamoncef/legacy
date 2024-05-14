/*E*/
/*Fichier : @(#)xzao463sp.prc	1.2     Release : 1.2        Date : 14/11/11
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao463sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* VR   14/11/11        : Création (DEM/1014)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao463sp.h" */

/* static char *version = "@(#)xzao463sp.prc	1.2 1.2 14/11/11: xzao463" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des PMVA pour une machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO4630
go

create procedure XZAO4630
   as 

/* 
*
* SP	XZAO463SP
*
* PARAMETRES EN ENTREE :
*
* 			
*
* PARAMETRES EN SORTIE :
*
*
* 
* VALEUR RENDUE :
*
* Retourne la liste des PMV de la Machine avec leur destinations proche/lointaine/noeud
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Retourne les parametres numero de PMVA, nom du ST, numero de liaison, adresse rgs
* numero de type de PMVA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	14/11/11	: Création (DEM/1014)
------------------------------------------------------*/
declare @vl_numero int,
	@vl_numero_dest int,
	@vl_nom_dest char(20)

	create table #PMV_DEST
	(
		numero			smallint,
		num_dest_proche		smallint null,
		nom_dest_proche		char(15) null,
		num_dest_lointaine	smallint null,
		nom_dest_lointaine	char(15) null,
		num_dest_noeud		smallint null,
		nom_dest_noeud		char(15) null	
	)

	/*A
	**  Récupérer toutes les association pmv destination par type de destination
	*/

	insert into #PMV_DEST (numero, num_dest_proche, nom_dest_proche, num_dest_lointaine, nom_dest_lointaine, num_dest_noeud, nom_dest_noeud)
	(
		select distinct PMV.numero_PMV, PMV.numero_dest, RES.libelle_PMV, 0, "", 0, ""     
		from CFG..DTP_PMV PMV, CFG..RES_DTP RES
		where PMV.type_dest = XDC_DESTINATION_PROCHE and
		      RES.inhibition = 0 and
		      PMV.numero_dest = RES.numero
	)


	declare curs_dest_loin cursor for  
	select distinct PMV.numero_PMV, PMV.numero_dest, RES.libelle_PMV     
	from CFG..DTP_PMV PMV, CFG..RES_DTP RES
	where PMV.type_dest = XDC_DESTINATION_LOINTAINE and
	      RES.inhibition = 0 and
	      PMV.numero_dest = RES.numero
	open curs_dest_loin
	fetch curs_dest_loin into @vl_numero, @vl_numero_dest, @vl_nom_dest
	while (@@sqlstatus = 0)
	begin
		update #PMV_DEST set num_dest_lointaine=@vl_numero_dest, nom_dest_lointaine=@vl_nom_dest
		where numero=@vl_numero

		if ( @@rowcount = 0 )
			insert into #PMV_DEST (numero, num_dest_proche, nom_dest_proche, num_dest_lointaine, nom_dest_lointaine, num_dest_noeud, nom_dest_noeud)
			values ( @vl_numero, 0, "", @vl_numero_dest, @vl_nom_dest, 0, "")

		fetch curs_dest_loin into @vl_numero, @vl_numero_dest, @vl_nom_dest

	end

	declare curs_dest_noeud cursor for
	select distinct PMV.numero_PMV, PMV.numero_dest, RES.libelle_PMV     
	from CFG..DTP_PMV PMV, CFG..RES_DTP RES
	where PMV.type_dest = XDC_DESTINATION_NOEUD and
	      RES.inhibition = 0 and
	      PMV.numero_dest = RES.numero
	open curs_dest_noeud
	fetch curs_dest_noeud into @vl_numero, @vl_numero_dest, @vl_nom_dest
	while (@@sqlstatus = 0)
	begin

		update #PMV_DEST set num_dest_noeud =@vl_numero_dest, nom_dest_noeud=@vl_nom_dest                
		where numero=@vl_numero

		if ( @@rowcount = 0 )
			insert into #PMV_DEST (numero, num_dest_proche, nom_dest_proche, num_dest_lointaine, nom_dest_lointaine, num_dest_noeud, nom_dest_noeud)
			values ( @vl_numero, 0, "", 0, "", @vl_numero_dest, @vl_nom_dest)

		fetch curs_dest_noeud into @vl_numero, @vl_numero_dest, @vl_nom_dest


	end

	select distinct numero, num_dest_proche, nom_dest_proche, num_dest_lointaine, nom_dest_lointaine, num_dest_noeud, nom_dest_noeud
	from #PMV_DEST
	order by numero
	
	return XDC_OK	
	
go  
