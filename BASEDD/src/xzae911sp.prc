/*E*/
/*  Fichier : @(#)xzae911sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae911sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des types de libelles et des libelles predefinis
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL  16/05/18        : Creation GABEB 1.1 DEM1283
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE911_Lect_Liste_CAP
*
* Arguments en entree
*
* Arguments en sortie
* smallint 	va_Numero_out
* char(25)	va_Nom_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture de tous les elements de la table EQT_GEN (base CFG)
-------------------------------------------------------*/

use PRC
go

create procedure XZAE911
	as
	
	/*A Recherche des CAP */
	select numero, nom
	from CFG..EQT_GEN
	where type=67
	order by nom

	return XDC_OK

go

