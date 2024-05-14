/*E*/
/*  Fichier : @(#)vuem11sp.prc	1.5   Release : 1.5        Date : 12/01/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	28/07/95	: Creation
* B.G.  02/10/95	: Modif pour avoir heure de 06 a 00 + choix entre 12 ou 100 + fortes valeurs (1.2)
* JMG	01/12/97	: pas de donnees extrapolees 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM11_DebitMax
* 
* Arguments en entree
* datetime	premier jour du mois a traiter
* smallint	station
* tinyint	sens
* tinyint	voie
* 
* Arguments en sortie
*
* liste retournee
* char horodate
* int		debit horaire
* 
* Service rendu
* retourne les 12 plus forts debits horaires sur un mois
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

create procedure VUEM11
	@va_mois	datetime,
	@va_station	smallint,
	@va_sens	tinyint,
	@va_voie	tinyint,
	@va_100_val	tinyint=null
as

declare @vl_fin	datetime

if @va_mois=null return XDC_ARG_INV

/* on calcule la date limite */

select @vl_fin=dateadd(month,1,@va_mois)

/* on recupere les mesure dans une table tempo avant de les traiter (performance) */

select	Q,HORODATE=dateadd(minute,-6,horodate)
into	#tempo
from	HIS..MES_TRA
where	station=@va_station and
	sens=@va_sens and
	voie=@va_voie and
	horodate>@va_mois and
	horodate<=@vl_fin and
	validite_Q != XDC_RADT_RECONST

if @va_100_val=XDC_VRAI
	set rowcount 100
else
	set rowcount 12

select	distinct
	jour=convert(char(8),HORODATE,3)+" "+convert(char(2),HORODATE,8)+":00",
	debit=avg(Q)
from	#tempo
group by datepart(day,HORODATE)*24+datepart(hour,HORODATE)
order by debit desc

set rowcount 0

go

