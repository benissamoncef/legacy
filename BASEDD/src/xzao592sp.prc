/*E*/
/*  Fichier : $Id: xzao592sp.prc,v 1.1 2012/07/04 11:35:30 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:35:30 $
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
* jmg.  20/06/12        : Creation (DEM/1016)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAO592_Sauve_Config_FMC_Strada
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


if exists (select * from sysobjects where name = 'XZAO592' and type = 'P')
	drop procedure XZAO592
go

create procedure XZAO592
	@va_Basedd_in                     char(3),
	@va_type_in		int		= null,
	@va_qualif_in		bit		
as
	/* controle les parametres obligatoires */
	if @va_type_in = null 
		return XDC_ARG_INV

	if @va_Basedd_in = XDC_BASE_CFG
	begin
		/*A indiquer la fin de la commande */
		begin tran
			/*B Insertion */
			insert CFG..REG_TYP (type, qualification)
			values (@va_type_in, @va_qualif_in)

		commit tran
	end
	else
	begin
		/*A indiquer la fin de la commande */
		begin tran
			/*B Insertion */
			insert CFT..REG_TYP (type, qualification)
			values (@va_type_in, @va_qualif_in)

		commit tran
	end
	return XDC_OK

go

