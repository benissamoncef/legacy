/*E*/
/*  Fichier : $Id: xzad11sp.prc,v 1.2 2018/05/03 13:34:53 devgfi Exp $     Release : $Revision: 1.2 $        Date :  $Date: 2018/05/03 13:34:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fournit le dernier tdp 6mn pour l'horodate donn≈e.
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	24/02/97	: Creation	(dem/tdp) (V 1.1)
* JPL	03/05/18	: Suppression de la procedure avant compilation  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* fournit le dernier tdp 6mn pour l'horodate donnEe.
* 
* Sequence d'appel
* SP	11
* XZAD11_Dernier_TDP
* 
* Arguments en entree
* XDY_Mot		va_Numero_in	: numero 
* XDY_Octet		va_Site_in	: site de recherche 
* XDY_Datetime		va_Horodate_in
* 
* Arguments en sortie
* XDY_Mot		va_Temps_out : dernier tdp trouve
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* 
* Fonction
* Consultation dans la table TDP_6MN
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAD11' and type = 'P')
	drop procedure XZAD11
go


create procedure XZAD11
	@va_Numero_in		smallint	= null,
	@va_Horodate_in		datetime	= null,
	@va_Temps_out		smallint	output,
	@va_Horodate_out	datetime 	output,
	@va_Validite_out	smallint	output
	
as
	declare  @vl_Retour int,
		@vl_h datetime


	select @vl_Retour = XDC_OK

	/*A controle parametres en entree */
	if @va_Numero_in = null 
		return XDC_ARG_INV

        if @va_Horodate_in = null select @vl_h = getdate()
        if @va_Horodate_in = XDC_DATE_NULLE select @vl_h = getdate()

	/*A verifier l'existance de la numero */
	if not exists ( select * from CFG..EQT_ZDP 
			where	numero = @va_Numero_in )
		return XDC_ARG_INV


	/*A recherche si la mesure existe en base */
	select 
		@va_Temps_out	= temps,
		@va_Horodate_out = horodate,
		@va_Validite_out = validite
	from 	EXP..ZDP_6MN
	where	numero = @va_Numero_in and
		horodate =  ( select max(horodate)
                              from EXP..ZDP_6MN
                               where numero = @va_Numero_in and
                                     horodate <= @vl_h ) 

	if @@rowcount = 0
		select 
			@va_Temps_out	= TDP_habituel,
			@va_Horodate_out = getdate(),
			@va_Validite_out = XDC_FAUX
		from CFG..EQT_ZDP
		where numero = @va_Numero_in

	return @vl_Retour
go
