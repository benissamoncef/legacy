/*E*/
/*  Fichier : $Id: xzag30sp.prc,v 1.3 2002/03/07 10:14:12 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2002/03/07 10:14:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* C.T.	09/01/96	: Supprimer le site dans la clause 
*                         de suppression (V 1.2)
* P.C.	06/03/02	: Correction pour HIS V1.3         
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des mesures poids de la base de l'historique datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG30_Purge_Mesures_Poids_Base_Historique
* 
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

create procedure XZAG;30
	@va_Horodate_in		datetime = null
as
#ifdef HIST
	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Effacer toutes les mesures de trafic anterieures a la date */
	/*A donnee                                                     */
	delete HIS..MES_POI where horodate <= @va_Horodate_in
#endif
	return XDC_OK
go
