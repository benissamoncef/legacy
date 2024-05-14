/*E*/
/*  Fichier : @(#)xzae230sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae230sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture desbelles de plans de feux 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMg	01/012/18	creation AVA 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE230
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_ARG_INV   : parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Ajout d'un enregistrement dans la table LIB_PRE (base CFG)
-------------------------------------------------------*/

use PRC
go

drop procedure XZAE230
go

create procedure XZAE230
	@va_identifiant_in	char(50)	,
	@va_numero_out		int	output 	,
	@va_cle_out		tinyint output
as

	/* controle les parametres obligatoires */
	if @va_identifiant_in = null 
		return XDC_ARG_INV

	/*A indiquer la fin de la commande */
	 select @va_numero_out = numero, @va_cle_out = cle
	 from EXP..FMC_AVA where identifiant  = @va_identifiant_in

	if @@rowcount = 0
		select @va_numero_out = -1, @va_cle_out = 0 

	return XDC_OK

go

