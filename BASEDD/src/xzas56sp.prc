/*E*/
/*  Fichier : $Id: xzas56sp.prc,v 1.2 1997/12/03 11:09:55 penot Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/03 11:09:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas56sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* efface l'ancienne config des stations radt a exporter
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
* SP   XZAS56_Efface_Config_RADT_Strada
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

create procedure XZAS56
as
	delete from CFG..ADA_STA

	return XDC_OK

go

