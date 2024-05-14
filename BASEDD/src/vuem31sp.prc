/*E*/
/*  Fichier : $Id: vuem31sp.prc,v 1.1 2000/07/21 17:14:42 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2000/07/21 17:14:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem31_depassement_seuil.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fiche metier dont l'objet est de connaitre la liste
* des depassements de seuil de temps de parcours
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	05/07/99	: Creation		V1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM31
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* int		va_numero_in
*
* Arguments en sortie
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree incorrects
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEM31
	@va_Debut_in		datetime	= null,
	@va_Fin_in		datetime	= null,
	@va_numero_in		int		= null,
	@va_seuil_haut_in	int		= null,
	@va_seuil_bas_in	int		= null,
	@va_heure_debut_in	smallint	= null,
	@va_heure_fin_in	smallint	= null,
	@va_seuil_in		int		= null
as
	declare @vl_horodate datetime,
		@vl_horodate_fin datetime

/*A controle des parametres d'entree */
if @va_Debut_in is null or @va_Fin_in is null or @va_numero_in is null
   or @va_seuil_haut_in is null or @va_seuil_bas_in is null or
   @va_heure_debut_in is null or @va_heure_fin_in is null or
   @va_seuil_in is null
	return XDC_ARG_INV
		
/* on recupere les mesures dans une table tempo avant de les traiter (performance) */
/*horodate,temps*/
create table #LISTE_FRN_TDP
	(horodate datetime null,
	 temps	  int null,
	 depassement smallint null)

select @vl_horodate=@va_Debut_in
select @vl_horodate=dateadd(hour,@va_heure_debut_in,@vl_horodate)
select @vl_horodate_fin=@va_Fin_in
select @vl_horodate_fin=dateadd(hour,@va_heure_fin_in,@vl_horodate_fin)

while datediff(minute,@vl_horodate,@vl_horodate_fin)>=0
begin
	insert #LISTE_FRN_TDP (horodate, temps)
	values (@vl_horodate,0)

	select @vl_horodate=dateadd(minute,6,@vl_horodate)
end

update #LISTE_FRN_TDP 
set  	#LISTE_FRN_TDP.temps =  HIS..ZDP_6MN.temps
from    HIS..ZDP_6MN
where   validite=XDC_RADT_CORRECT and
	numero=@va_numero_in and
	HIS..ZDP_6MN.horodate>=@va_Debut_in and
	HIS..ZDP_6MN.horodate<=@va_Fin_in and
	datepart(hour,HIS..ZDP_6MN.horodate)>=@va_heure_debut_in and
	datepart(hour,HIS..ZDP_6MN.horodate)<=@va_heure_fin_in and
	#LISTE_FRN_TDP.horodate = HIS..ZDP_6MN.horodate

update #LISTE_FRN_TDP
	set depassement = 1 
	where temps >= @va_seuil_bas_in

select * from #LISTE_FRN_TDP
order by horodate
go
