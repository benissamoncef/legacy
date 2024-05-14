/*E*/
/*  Fichier : $Id: xzad05sp.prc,v 1.3 2020/12/15 23:21:06 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/12/15 23:21:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fournit le dernier tdp 6mn pour l'horodate donn≈e.
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	24/02/97	: Creation	(dem/tdp) (V 1.1)
* JMG	25/01/13 : correction TDP nul 1.2
* JPL	15/12/20 : Retour du TDP habituel en DEV, et 0 sinon  1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* fournit le dernier tdp 6mn pour l'horodate donnEe.
* 
* Sequence d'appel
* SP	05
* XZAD05_Dernier_TDP
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

drop procedure XZAD05
go

create procedure XZAD05
	@va_Numero_in		smallint	= null,
	@va_Horodate_in		datetime	= null,
	@va_Temps_out		smallint	output
	
as
	declare  @vl_Retour int


	select @vl_Retour = XDC_OK

	/*A controle parametres en entree */
	if @va_Numero_in = null or @va_Horodate_in = null or
	   @va_Horodate_in = XDC_DATE_NULLE 
		return XDC_ARG_INV



	/*A verifier l'existance de la numero */
	if not exists ( select * from CFG..EQT_ZDP 
			where	numero = @va_Numero_in )
		return XDC_ARG_INV


	/*A recherche si la mesure existe en base */
	select 
		@va_Temps_out	= temps
	from 	EXP..ZDP_6MN
	where	numero = @va_Numero_in and
		validite = XDC_VRAI and
		horodate =  ( select max(horodate)
                              from EXP..ZDP_6MN
                               where numero = @va_Numero_in and
                                     horodate <= @va_Horodate_in and
				     horodate >= dateadd(minute, -6, @va_Horodate_in) ) 

	if @@rowcount = 0
		select 
#if defined (DEV)
			@va_Temps_out	= TDP_habituel
#else
			@va_Temps_out	= 0
#endif
		from CFG..EQT_ZDP
		where numero = @va_Numero_in

	return @vl_Retour
go
