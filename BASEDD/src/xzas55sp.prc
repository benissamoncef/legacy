/*E*/
/*  Fichier : $Id: xzas55sp.prc,v 1.2 1997/12/03 11:01:57 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/03 11:01:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas55sp.prc
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
* SP   XZAS55_Lect_RADT_A_Exporter
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
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

create procedure XZAS55
as
	select numero, sens , nom
	from CFG..ADA_STA

	return XDC_OK

go

