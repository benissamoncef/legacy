/*E*/
/*  Fichier : $Id: xzas88sp.prc,v 1.2 2010/09/01 19:38:35 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/09/01 19:38:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas88sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de s derniers niveaux de trafic
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/836
* JPL   01/09/10        Ajout instruction "drop procedure" 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
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

drop procedure XZAS88
go

create procedure XZAS88
as
	select station, sens
	from EXP..MES_NIV_DER

	return XDC_OK

go

