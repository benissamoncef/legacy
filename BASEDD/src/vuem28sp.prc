/*E*/
/*  Fichier : @(#)vuem28sp.prc	1.1    Release : 1.1        Date : 05/27/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem28sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	16/05/97	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM28_TDP
* 
* Arguments en entree
* datetime	horodate de debut
* smallint	Nieme plus grande valeur
* smallint	Nombre mini d'appariements
* 
* Arguments en sortie
*
* liste retournee
* char		libelle zone TDP
* smallint	tps de parcours
* datetime	horodate
* 
* Service rendu
* retourne pour chaque zone, les n plus grands temps de
* parcours par tranche horaire, sur le mois.
* Tri par ordre decroissant des tdp.
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEM28
	@va_mois		datetime,
	@va_numero		smallint,
	@va_nieme_tdp		smallint,
	@va_nb_appariement	smallint
as

declare @vl_fin		datetime


if @va_mois=null return XDC_ARG_INV
if @va_nieme_tdp=null return XDC_ARG_INV
if @va_nb_appariement=null return XDC_ARG_INV


/* on calcule la date limite */

select @vl_fin=dateadd(month,1,@va_mois)

/* on recupere les mesures dans une table tempo avant de les traiter (performance) */

select	*
into	#tempo
from	HIS..ZDP_HEU
where	validite=XDC_RADT_CORRECT and
	appariement>=@va_nb_appariement and
	numero=@va_numero and
	horodate>@va_mois and
	horodate<=@vl_fin


select	libelle,
	temps,
	convert(char(10),horodate,103)+' '+convert(char(8),horodate,8)
from	#tempo, CFG..EQT_ZDP
where	CFG..EQT_ZDP.numero=#tempo.numero
group by #tempo.numero
having 
	CFG..EQT_ZDP.numero=#tempo.numero and
	count(*)>=@va_nieme_tdp
order by #tempo.numero,temps desc



go

