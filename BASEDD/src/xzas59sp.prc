/*E*/
/*  Fichier : $Id: xzas59sp.prc,v 1.1 1999/02/26 11:22:25 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:22:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas56sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* efface l'ancienne config des PMV a exporter
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
* SP   XZAS59_Efface_Config_PMV
*
* Arguments en entree
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
* 
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS59' and type = 'P')

    drop procedure XZAS59

go

create procedure XZAS59
as
	delete from CFG..ADA_PMV

	return XDC_OK

go

