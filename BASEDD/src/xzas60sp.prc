/*E*/
/*  Fichier : $Id: xzas60sp.prc,v 1.1 1999/02/26 11:22:26 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:22:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas57sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ecriture de la config des PMV a exporter vers Strada
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron  23/11/98        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAS60_Sauve_Config_PMV
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

if exists (select * from sysobjects where name = 'XZAS60' and type = 'P')

    drop procedure XZAS60

go

create procedure XZAS60
	@va_numero_in	int	= null
as
	declare @vl_nom char(15)

	/* controle les parametres obligatoires */
	if @va_numero_in = null 
		return XDC_ARG_INV


	insert into CFG..ADA_PMV (numero)
	values (@va_numero_in)

	return XDC_OK

go

