/*E*/
/*  Fichier : $Id: xzas57sp.prc,v 1.2 1997/12/03 11:10:15 penot Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/03 11:10:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas57sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la config des stations RADT a exporter vers Strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou  08/05/97        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAS57_Sauve_Config_RADT_Strada
*
* Arguments en entree
* int	numero station
* int	sens station
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

create procedure XZAS57
	@va_numero_in	int	= null,
	@va_sens_in	tinyint = null
as
	declare @vl_nom char(15)

	/* controle les parametres obligatoires */
	if @va_numero_in = null or @va_sens_in = null
		return XDC_ARG_INV

	/*recupere le nom de la station*/
	select @vl_nom = nom from CFG..EQT_GEN 
		   where type = XDC_EQT_RAD and
			 numero = @va_numero_in and
			 sens = @va_sens_in

	insert into CFG..ADA_STA (numero, nom, sens)
	values (@va_numero_in, @vl_nom, @va_sens_in)

	return XDC_OK

go

