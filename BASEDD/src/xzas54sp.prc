/*E*/
/*  Fichier : $Id: xzas54sp.prc,v 1.6 2000/07/21 16:55:21 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2000/07/21 16:55:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas54sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module STRADA : Extraction des donnees radt a exporter
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	02/05/95	: Creation
* Guilhou	02/12/97 : modif clause where 1.3
* Guilhou	02/12/97 : renvoie donnees reelles et extrapolees 1.4
* Niepceron	06/01/98 : envoie les donnees correct de station non hs,desactivee ou inhibee et dernier 6mn v1.5 dem/1724
* Niepceron     31/03/99 : suppression du having  1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAS;54
* 
* Arguments en entree
* smallint	station
* tinyint	sens
* 
* Arguments en sortie
*
* int		Q
* int		V
* int		TO
* 
* Service rendu
* retourne la liste ci-dessus pour l'horodate courante-6mn
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

if exists (select * from sysobjects where name = 'XZAS54' and type = 'P')

    drop procedure XZAS54

go

create procedure XZAS54
	@va_station	smallint = null,
	@va_sens	tinyint = null,
	@va_Q_out	int=null output,
	@va_V_out	int=null output,
	@va_TO_out	int=null output
as

declare @vl_date_courante	datetime,
	@vl_date_min	datetime,
	@vl_validite_Q		tinyint,
	@vl_validite_TO		tinyint,
	@vl_validite_V		tinyint

if @va_station is null or @va_sens is null
	return XDC_ARG_INV


/* Verification que l'eqt ne sont pas HS desctive ou inhibe */
if exists ( select * from EXP..EQT_DSP
		where type=XDC_EQT_RAD and equipement=@va_station and dernier=1 and
			HS=0 and desactive=0 and inhibe=0)
begin
/* on calcule la date courante */
select @vl_date_courante = getdate()
select @vl_date_min = dateadd(minute,-6,getdate())

select	@va_Q_out = Q,
	@vl_validite_Q=validite_Q,
	@va_V_out = V,
	@vl_validite_V=validite_V,
	@va_TO_out = T_O,
	@vl_validite_TO=validite_TO
from	EXP..MES_TRA
where	station=@va_station and
	sens=@va_sens and
	voie=XDC_RADT_CHAUSSEE and
	horodate<=@vl_date_courante and
	horodate>@vl_date_min

if @vl_validite_Q<>XDC_RADT_CORRECT 	select @va_Q_out=-1
if @vl_validite_V<>XDC_RADT_CORRECT 	select @va_V_out=-1
if @vl_validite_TO<>XDC_RADT_CORRECT 	select @va_TO_out=-1
if @va_Q_out=null  	select @va_Q_out=-1
if @va_V_out=null  	select @va_V_out=-1
if @va_TO_out=null  	select @va_TO_out=-1

end
else
begin
	select @va_Q_out=-1
	select @va_V_out=-1
	select @va_TO_out=-1
end
go

