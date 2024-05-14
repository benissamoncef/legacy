/*E*/
/*  Fichier : $Id: xzae125sp.prc,v 1.2 1997/12/01 15:36:21 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/01 15:36:21 $
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
* SP   XZAE125_Sauve_Config_FMC_Strada
*
* Arguments en entree
* int		va_type_in
* int      	va_autoroute_in
* int		va_debut_in
* int		va_fin_in
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
* Ajout d'un enregistrement dans la table ADA_EVT
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE125' and type = 'P')
	drop procedure XZAE125
go

create procedure XZAE125
	@va_type_in		int		= null,
	@va_autoroute_in	int		= null,
	@va_debut_in		int		= null,
	@va_fin_in		int		= null
as
	/* controle les parametres obligatoires */
	if @va_type_in = null or @va_autoroute_in = null or @va_debut_in = null 
			or @va_fin_in = null 
		return XDC_ARG_INV

	/*A indiquer la fin de la commande */
	begin tran
		/*B Insertion */
		insert CFG..ADA_EVT (type, autoroute, debut, fin)
		values (@va_type_in, @va_autoroute_in, @va_debut_in, @va_fin_in)

	commit tran

	return XDC_OK

go

