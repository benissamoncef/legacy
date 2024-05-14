/*E*/
/*  Fichier : $Id: xzae100sp.prc,v 1.2 1995/12/13 10:52:40 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/12/13 10:52:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae100sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des fmc
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	28/07/95	: Creation
* C.T.	11/12/95	: Modif clause de recherche (V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE100_ListeBouchons
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* 
* Arguments en sortie
* 
* liste retournee :
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in

* Service rendu
* Recherche les bouchons sur tout le reseau durant la periode demandee
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Tout bouchon doit etre fini dans la periode demandee
* en sortie, retourne la liste des queues de bouchon
* utilisable uniquement au CI
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAE100
	@va_Debut_in	datetime	= null,
	@va_Fin_in	datetime	= null
as
	/*A controle des parametres d'entree */
	if @va_Debut_in is null or @va_Fin_in is null
		return XDC_ARG_INV

	/*A recherche les queues de bouchon */ 
	select
		numero,
		cle
	from HIS..FMC_GEN
	where type = XZAEC_FMC_QueueBouchon and
		debut >= @va_Debut_in and debut < @va_Fin_in

	return XDC_OK
go
