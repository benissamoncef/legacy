/*E*/
/*  Fichier : $Id: xzad06sp.prc,v 1.2 2018/05/03 13:42:19 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/05/03 13:42:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Mise » jour de FRN_TDP pour la fiche operateur
* donn≈e 
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
* Mise » jour de FRN_TDP pour la fiche operateur
* donn≈e 
* 
* Sequence d'appel
* SP	XZAD06_Ajout_FMC_Op_Frn
* 
* Arguments en entree
* XDY_Entier		va_NoFMC_in
* XDY_Octet		va_CleFMC_in
* 
* Arguments en sortie
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
* Consultation de la table TDP_FRN
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAD06' and type = 'P')
	drop procedure XZAD06
go


create procedure XZAD06
	@va_NoFMC_in		int		= null,
	@va_CleFMC_in		tinyint		= null
	
as
	declare  @vl_Retour int,
		@vl_zdp smallint,
		@vl_debut datetime,
		@vl_max_num int

	select @vl_Retour = XDC_OK

	/*A controle parametres en entree */
	if @va_NoFMC_in = null or 
	   @va_CleFMC_in = null 
		return XDC_ARG_INV



	select @vl_max_num=count(*) from EXP..FRN_TDP
	
	if @vl_max_num <> 0
	begin
		select 
			@vl_max_num = max(numero) 
		from 	EXP..FRN_TDP
	end

	declare frn cursor for
	select distinct
	       zdp,
	       debut
	 from 	EXP..FRN_TDP
	 where 	fin=null and
	 	site_zdp=@va_CleFMC_in
	 	

	open frn
	
	fetch frn into	@vl_zdp,
			@vl_debut
        
	/*A Pour chaque fmc trouv≈e : */

	while (@@sqlstatus = 0)
	begin
		/*B incremente me dernier numero de franchissement */
		select  @vl_max_num = @vl_max_num + 1
		insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
		values (@vl_max_num,@vl_zdp,@va_CleFMC_in,@va_NoFMC_in,@va_CleFMC_in,@vl_debut,null)
		
		fetch frn into	@vl_zdp,
				@vl_debut

	end

	return @vl_Retour
go






