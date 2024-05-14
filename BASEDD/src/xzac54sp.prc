/*E*/
/*  Fichier : $Id: xzac54sp.prc,v 1.5 2020/10/13 17:22:15 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/10/13 17:22:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac54sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation
* C.T.	13/09/94	: Rajout test sur le site
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (1.4)
* LCL   25/03/20   	: MOVIS Ajout site local DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer  un morceau de texte de fax a une actio deja existante
* 
* Sequence d'appel
* SP	XZAC54_Ajouter_Texte_Fax
* 
* Arguments en entree
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Texte		va_Texte_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Il faut que l'actio existe deja : utiliser avant XZAC09
* 
* Fonction
* Inserer dans la table TEXTE_FAX
-------------------------------------------------------*/

use PRC
go

create procedure XZAC;54
	@va_NumAction_in	int	= null,
	@va_Site_in		tinyint	= null,
	@va_Texte_in		T_TEXTE	= null,
	@va_NomSiteLocal_in	char(2)	= null
as
	declare @vl_Morceau smallint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumAction_in = 0 select @va_NumAction_in = null
	if @va_Site_in = 0 select @va_Site_in = null

	/*A controle des parametres obligatoires */
	if @va_NumAction_in = null or @va_Site_in = null or 
	   @va_Texte_in = null
		return XDC_ARG_INV

	/*A tester si le site est correct */
	/* MOVIS */
	if @va_NomSiteLocal_in = null
	begin
		if not exists ( select numero from CFG..RES_DIS
                                where serveur = @@servername and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK
	end
	else
	begin
		if not exists ( select numero from CFG..RES_DIS
                                where code = @va_NomSiteLocal_in and type <> XDC_TYPE_PCNIVEAU3 )
		return XDC_NOK
	end

	if not exists ( select * from EXP..ACT_FAX
		where actio = @va_NumAction_in and sit = @va_Site_in)
		return XDC_NOK

	/*A recherche du numero de morceau suivant */
	select @vl_Morceau = max(morceau) + 1 
	from EXP..ACT_FAX
	where actio = @va_NumAction_in and sit = @va_Site_in

	if @vl_Morceau = null
		return XDC_NOK

	/*B inserer le texte du fax */
	insert EXP..ACT_FAX (actio, sit, morceau, texte)
	values (@va_NumAction_in, @va_Site_in, @vl_Morceau, @va_Texte_in)

	return XDC_OK

go
