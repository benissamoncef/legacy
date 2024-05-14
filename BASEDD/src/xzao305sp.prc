/*  Fichier : $Id: xzao305sp.prc
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao305sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture des batiments du district
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	21/02/24	:	Creation
*
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
*	Retourne la liste des Batiments
* 
* Sequence d'appel
* 
* Arguments en entree
* 
* Arguments en sortie 	-- arguments retournes dans une liste
* 
*	XDY_Octet		va_Numero
*	XDY_Nom			va_NomBatiment
*	XDY_Abrev		va_Abreviation
*	XDY_NomAuto		va_Autoroute
*	XDY_PR			va_PR
*	XDY_Sens		va_Sens
*	XDY_District	va_District_out
*	XDY_Booleen		va_Supprime_out
* 
* Code retour
*	XDC_OK
*	<0 erreur sybase 
* 
* Conditions d'utilisation
*	Toutes les informations sont retournees dans une liste
* 
* Fonction
*	Select from RES_BAT
---------------------------------------------------*/
if exists (select * from sysobjects where name = 'XZAO305' and type = 'P')
	drop procedure XZAO305
go

use PRC
go

create procedure XZAO305
as
	/*A rechercher toutes les aires */
	select
		CFG..RES_BAT.numero,
		CFG..RES_BAT.nom,
		CFG..RES_BAT.abreviation,
		CFG..RES_POR.autoroute,
		CFG..RES_BAT.PR,
		CFG..RES_BAT.sens,
		CFG..RES_POR.district,
		CFG..RES_BAT.supprime
			from CFG..RES_BAT, CFG..RES_POR
				where CFG..RES_POR.numero = CFG..RES_BAT.portion and CFG..RES_BAT.supprime = XDC_FAUX
					order by upper(CFG..RES_BAT.nom), CFG..RES_POR.autoroute, CFG..RES_BAT.PR

	return XDC_OK
go
