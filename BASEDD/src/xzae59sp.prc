/*E*/
/*  Fichier : $Id: xzae59sp.prc,v 1.4 2011/09/23 09:27:13 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2011/09/23 09:27:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae59sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche de eqt dans la table EQT_TDP_INH
*
* Cette proc ne se compile que sur CI / VC / DP
*									--> Pas sur HIS
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	10/01/23	Creation AVA DEM-SAE431
* GGY	05/04/23	Ajout d'un ifdef pour ne pas compiler cette proc sur HIS (DEM-SAE431)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche de eqt dans la table EQT_TDP_INH
------------------------------------------------------- 
* Sequence d'appel
* SP	xzae59_Rechercher_EQT_TDP_INHIB
* 
* Arguments en entree
* XDY_Mot		va_NumEqt_in Numero d equipement recherche
* XDY_Octet 	va_TypeEqt_in Type d equipement recherche
* 
* Arguments en sortie
* XDY_Octet		va_EQT_TDP_INHIB_out
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Recherche de eqt dans la table EQT_TDP_INH
* 
* Fonction
* Select dans la table EQT_TDP_INH
-------------------------------------------------------*/

use PRC
go


if exists (select * from sysobjects where name = 'XZAE59' and type = 'P')
	drop procedure XZAE59
go


create procedure XZAE59
	@va_NumEqt_in	        smallint = null,
	@va_TypeEqt_in	       tinyint = null,
	@va_EQT_TDP_INHIB_out	   tinyint output
as	

#ifndef HIS

	/*! transfo en parametres nulls bug applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_TypeEqt_in = 0 select @va_TypeEqt_in = null

	/* test la presence des parametres obligatoires */
	if @va_NumEqt_in = null or @va_TypeEqt_in = null
		return XDC_ARG_INV

    /* Recherche dans la table si l'equipement est dans la table */
    select * 
    from EXP..EQT_TDP_INH T
    where T.numero = @va_NumEqt_in
    and T.type = @va_TypeEqt_in
	and T.horodate_fin is null

    if @@rowcount!=0
        select @va_EQT_TDP_INHIB_out = XDC_VRAI
    else
        select @va_EQT_TDP_INHIB_out = XDC_FAUX

#else

	select @va_EQT_TDP_INHIB_out = XDC_FAUX

#endif

return XDC_OK
go
