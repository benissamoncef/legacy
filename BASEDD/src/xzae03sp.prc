/*E*/
/*  Fichier : $Id: xzae03sp.prc,v 1.4 2007/04/16 17:43:54 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2007/04/16 17:43:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/09/94	: Creation	(V 1.1)
* C.T.	14/02/95	: Modif ordre de retour (V 1.3)
* JPL	16/04/07	: Changement de nom (V 1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des types d'evenement connus
* 
* Sequence d'appel
* SP	XZAE03_Liste_Types_Evt
* 
* Arguments en entree
* 
* Arguments en sortie
* XDY_Octet	va_Nom_out
* XDY_Octet	va_Classe_out
* XDY_Nom	va_Nom_out
* char(10)	va_Abreviation_out
* XDY_Booleen	va_Operation_out
* XDY_Booleen	va_Supprime_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* select * from FMC_TYP
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE03
go


create procedure XZAE03
as
	select
		numero,
		classe,
		nom,
		abreviation,
		operation,
		supprime
	from CFG..TYP_FMC
	order by operation, nom

	return XDC_OK
go
