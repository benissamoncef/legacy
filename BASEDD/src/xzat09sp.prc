/*E*/
/*  Fichier : $Id: xzat09sp.prc,v 1.3 1998/09/22 13:29:31 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/09/22 13:29:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	26/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix V 1.2
* P.N.	03/07/1998	: separation EXP/HIS v 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la file d'attente d'un ≈quipement
* 
* S≈quence d'appel
* SP	XZAT09_File_Attente
* 
* Arguments en entr≈e
* XDY_Eqt	va_NumEqt_in
* XDY_Octet	va_TypeEqt_in
* 
* Arguments en sortie (Liste)
* XDY_Octet	va_Priorite_out
* XDY_Entier	va_NumEvt_out
* XDY_Octet	va_CleEvt_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* 
* Conditions d'utilisation
* 
* Fonction
* select dans la table FILE_D_ATTENTE
-------------------------------------------------------*/

use PRC
go

create procedure XZAT;09
	@va_NumEqt_in	T_EQUIPEMENT = null,
	@va_TypeEqt_in	tinyint = null
as

#ifndef HIST
	/*! transfo en param…tres nulls bug applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_TypeEqt_in = 0 select @va_TypeEqt_in = null

	/*A Controle les parametres d'entree */
	if @va_NumEqt_in = null or @va_TypeEqt_in = null
		return XDC_ARG_INV

	/*A liste des actions en attente */
	select 
		priorite,
		evenement,
		cle
	from EXP..EQT_FIL
	where equipement = @va_NumEqt_in and type_equipement = @va_TypeEqt_in
#endif
	return XDC_OK
go
