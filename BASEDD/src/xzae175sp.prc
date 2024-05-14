/*E*/
/* Fichier : $Id: xzae175sp.prc,v 1.7 2018/01/18 18:31:47 devgfi Exp $        Release : $Revision: 1.7 $        Date : $Date: 2018/01/18 18:31:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae175sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module MI2 : Retourne la liste des Q, V, T_O ... pour MI2
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     24/06/02  : Creation
* Cluchague     18/07/03  : Modifications pour etre conforme au doc EIT
* Cluchague     07/08/03  : Ajout stations 13.G1 et 13.G2 v1.3
* Cluchague     21/01/04  : on n'insere en base que s'il y a une valeur pour ne pas reprendre
			    le calcul de la station precedente + ajout 3 stations MEB13.A
			    MEB13.B MEB13.D (V1.4)
* ferrero	10/06/10  : rajout de 2 stations qui manquaient et modif de certaines dont le sens
			    etait inverse car 1 = SUD et 2 = NORD
* JPL		03/01/18  : Restriction de la periode de recherche a une seule tranche de 6mn  1.6
* JPL		03/01/18  : Description des points de mesure MI2 et stations associees en configuration (DEM 1266)  1.7
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
*
* Sequence d"appel
* SP    XZAE175
*
* Arguments en entree
*
* Arguments en sortie
*
* aucun :
*
* Service rendu
*    Retourne les dernières mesures pour chaque point de mesure MI2 configuré.
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d"entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d"utilisation
*
* Fonction
*
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE175' and type = 'P')
	drop procedure XZAE175
go


create procedure XZAE175
as

declare
@vl_maintenant datetime,
@vl_date_debut datetime,
@vl_date_fin   datetime


/* calcul de la tranche de date a traiter */
select @vl_maintenant = getdate()

select @vl_date_debut = dateadd (second, 1, dateadd (minute, -8, @vl_maintenant))
select @vl_date_fin =   dateadd (minute, -2, @vl_maintenant)


/* Recherche des dernieres valeurs des mesures 6mn base par chaussee */
/* pour toutes les stations RADT representant un point de mesure MI2 */

select	PT_MESURE = rtrim (nom_pt_mesure), QT = MES.Q / 10, TT = MES.T_O, VT = MES.V

from	CFG..RES_DIT, EXP..MES_TRA MES

where	MES.station = numero_radt
  and	MES.sens = sens_radt
  and	MES.voie = 0
  and	MES.horodate between @vl_date_debut and @vl_date_fin

order by PT_MESURE

return (XDC_OK)

go
