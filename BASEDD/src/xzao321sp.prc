/*E*/
/*  Fichier : $Id: xzao321sp.prc,v 1.1 1995/12/13 10:48:06 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/12/13 10:48:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao321sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture et ecriture des descriptions des equipements 
* et du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/12/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAO321_Ecrire_Vitesse_Libre
* 
* Arguments en entree
* XDY_Mot	va_NumEqt_in
* XDY_Sens	va_Sens_in
* XDY_Vitesse	va_Vitesse_in
* 
* Arguments en sortie
* 
* Service rendu
* modifier  la vitesse libre d'une station ou l'initialiser
* 
* Code retour
* XDC_OK
* XDC_NOK	: si la station n'existe pas
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure XZAO;321
	@va_NumEqt_in	smallint = null,
	@va_Sens_in	T_SENS = null,
	@va_Vitesse_in	int = null
as

	if not exists (select * from CFG..RES_CHA where station = @va_NumEqt_in and sens = @va_Sens_in)
		return XDC_NOK

	/*A recherche les vitesses libres */
	if not exists (select * from CFG..EQT_RAD_VTL where station = @va_NumEqt_in and sens = @va_Sens_in)
		insert CFG..EQT_RAD_VTL (station,  sens, VTL)
		values(@va_NumEqt_in, @va_Sens_in, @va_Vitesse_in)
	else
		update  CFG..EQT_RAD_VTL set VTL = @va_Vitesse_in
		where station = @va_NumEqt_in and sens = @va_Sens_in

	return XDC_OK
go
