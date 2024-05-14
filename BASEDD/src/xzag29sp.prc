/*E*/
/*  Fichier : $Id: xzag29sp.prc,v 1.4 1996/05/07 18:23:28 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/05/07 18:23:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag29sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	11/12/95	: Modif recherche stations	(V 1.2)
* C.T.	09/01/96	: Supprimer le site dans la clause 
*                         de suppression (V 1.3)
* C.T.	03/05/96	: Supprimer tout (pas de données dans HIS) (V1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures niveau de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG29_Purge_Mesures_Niveau_Base_Historique 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus de 5 ans
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;29
	@va_Horodate_in		datetime = null
as

	return XDC_OK
go
