/*E*/
/*Fichier : $Id: xzae109sp.prc,v 1.1 1996/10/04 10:49:33 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/04 10:49:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae109sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* R≈cuperation du nom de la machine de l'operateur correspondant
* a la fmc op≈rateur donneer
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.  26/09/96        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE109_Machine_Operateur
*
* Arguments en entree
* int 		va_Numero_in
* smallint 	va_Cle_in
*
* Arguments en sortie
* T_NOM va_NomMachine_out

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

create procedure XZAE;109
	@va_Numero_in		int		= null,
	@va_Cle_in		smallint	= null,
	@va_NomMachine_out	T_NOM		= null output

as

	select  @va_NomMachine_out=nom from CFG..EQT_GEN,EXP..FMC_GEN 
	where 	EXP..FMC_GEN.numero=@va_Numero_in and 
		EXP..FMC_GEN.cle=@va_Cle_in and 
		CFG..EQT_GEN.numero=EXP..FMC_GEN.poste_enrichisseur and 
		CFG..EQT_GEN.type=XDC_EQT_MAC

	return XDC_OK

go

