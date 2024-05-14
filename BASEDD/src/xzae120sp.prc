/*E*/
/*  Fichier : $Id: xzae120sp.prc,v 1.2 1997/11/06 09:31:14 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/11/06 09:31:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae120sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ecriture des types des evts STRADA
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  20/06/96        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE120_Creation_Evt_ADA
*
* Arguments en entree
* char		va_refevt_in
* int		va_version_in
* datetime      va_horo_in
* char(250)	va_ligne_in
* char		va_type_in
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
* Ajout d'un enregistrement dans la table ANA_SYN
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE120' and type = 'P')
	drop procedure XZAE120
go

create procedure XZAE120
	@va_refevt_in		char(50)	= null,
	@va_version_in		int		= null,
	@va_horo_in		datetime	= null,
	@va_ligne_in		char(250)	= null,
	@va_type_in		char(50)	= null
as
	/* controle les parametres obligatoires */
	if @va_refevt_in = null or @va_version_in = null or @va_ligne_in = null 
			or @va_type_in = null 
		return XDC_ARG_INV

	/*A indiquer la fin de la commande */
	begin tran
		/*B Insertion d'un libelle predefini */
		insert EXP..ADA_SYN (reference, version, horodate, ligne, type)
		values (@va_refevt_in, @va_version_in, @va_horo_in, @va_ligne_in,  
			@va_type_in)

	commit tran

	return XDC_OK

go

