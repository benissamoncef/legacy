/*E*/
/*  Fichier : $Id: xzae34sp.prc,v 1.1 2016/12/08 16:18:17 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2016/12/08 16:18:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae34sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture du numero d'autoroute en fonction du nom en param
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	01/02/21	: Creation DEM-SAE367
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des viaducs
* 
* Sequence d'appel
* SP	XZAE34_Lire_Numero_Autoroute
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_NomAuto		va_Nom_Autoroute_in
* XDY_Autoroute     va_Num_Autoroute_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_AUT
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE34' and type = 'P')
	drop procedure XZAE34
go


create procedure XZAE34
    @va_Nom_Autoroute_in char(5),
    @va_Num_Autoroute_out tinyint output 
as

	/*! transfo en parametres nulls bug applix */
	if @va_Nom_Autoroute_in = XDC_CHAINE_VIDE select @va_Nom_Autoroute_in = null

	/* test la presence des parametres obligatoires */
	if @va_Nom_Autoroute_in = null
		return XDC_ARG_INV

	/*A rechercher le numero a partir du nom */
	select
		@va_Num_Autoroute_out = CFG..RES_AUT.numero 
    from CFG..RES_AUT
	where CFG..RES_AUT.nom = @va_Nom_Autoroute_in
	
	return XDC_OK
go
