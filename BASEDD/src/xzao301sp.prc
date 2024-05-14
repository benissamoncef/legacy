/*E*/
/*  Fichier : $Id: xzao301sp.prc,v 1.5 2013/10/09 15:10:08 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2013/10/09 15:10:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao301sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* C.T.	24/01/95	: modif retourne voies	(V 1.2)
* C.T.	29/09/95	: ordonner suivant autoroute, PR (V 1.3)
* Hilmarcher 25/02/05   : ordonner par ordre alphabetique v1.4
* JPL	27/09/13	: Degroupee; retour numero de sortie et indicateur bifurcation (DEM 1083)  1.5
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des echangeurs
* 
* Sequence d'appel
* SP	XZAO301_Liste_Echangeurs_Ax
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Octet		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* char(15)		va_Abrev_out		-- dans une liste
* XDY_Octet		va_Autoroute_out
* XDY_PR		va_PR_out
* XDY_PR		va_PREntreeSens1_out
* XDY_PR		va_PRSortieSens1_out
* XDY_PR		va_PREntreeSens2_out
* XDY_PR		va_PRSortieSens2_out
* XDY_District		va_District_out
* XDY_Booleen		va_Supprime_out
* XDY_Octet             voie entree (sortie) sens 1 (2)
* ...
* char(20)		va_Sortie_out
* XDY_Booleen		va_Bifurcation_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_ECH
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO301' and type = 'P')
	drop procedure XZAO301
go


create procedure XZAO301
as
	/*A rechercher tous les echangeurs */
	select
		CFG..RES_ECH.numero,
		CFG..RES_ECH.nom,
		CFG..RES_ECH.abreviation,
		CFG..RES_POR.autoroute,
		CFG..RES_ECH.PR,
		CFG..RES_ECH.PR_entree_sens1,
		CFG..RES_ECH.PR_sortie_sens1,
		CFG..RES_ECH.PR_entree_sens2,
		CFG..RES_ECH.PR_sortie_sens2,
		CFG..RES_POR.district,
		CFG..RES_ECH.supprime,
		CFG..RES_ECH.voies_entree_sens1,
		CFG..RES_ECH.voies_sortie_sens1,
		CFG..RES_ECH.voies_entree_sens2,
		CFG..RES_ECH.voies_sortie_sens2,
		CFG..RES_ECH.sortie,
		CFG..RES_ECH.bifurcation
	from CFG..RES_ECH, CFG..RES_POR
	where CFG..RES_POR.numero = CFG..RES_ECH.portion and CFG..RES_ECH.supprime=XDC_FAUX
	order by upper(CFG..RES_ECH.nom),CFG..RES_POR.autoroute, CFG..RES_ECH.PR

	return XDC_OK
go
