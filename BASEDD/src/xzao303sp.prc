/*E*/
/*  Fichier : $Id: xzao303sp.prc,v 1.5 2005/03/07 17:47:31 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2005/03/07 17:47:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao303sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* C.T.	25/03/95	: Retourne une liste ordonnee suivant
*                         autoroute, PR (V 1.2)
* C.T.	24/01/96	: Ajout numero de sous concessionnaire
*                         en sortie (1.4)
* P.NI	07/03/05	: Tri par nom 1.5 DEM438
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des aires
* 
* Sequence d'appel
* SP	XZAO303_Liste_Aires_Ax
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Octet		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* char(15)		va_Abrev_out		-- dans une liste
* XDY_Octet		va_Autoroute_out
* XDY_PR		va_PREntree_out
* XDY_PR		va_PRSortie_out
* XDY_Sens		va_Sens_out
* XDY_Mot		va_CapaciteVL_out
* XDY_Mot		va_CapacitePL_out
* XDY_District		va_District_out
* XDY_Booleen		va_Supprime_out
* XDY_Mot		va_NumConcessionnaire_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_AIR
---------------------------------------------------*/

use PRC
go


create procedure XZAO;303
as
	/*A rechercher toutes les aires */
	select
		CFG..RES_AIR.numero,
		CFG..RES_AIR.nom,
		CFG..RES_AIR.abreviation,
		CFG..RES_POR.autoroute,
		CFG..RES_AIR.PRentree,
		CFG..RES_AIR.PRsortie,
		CFG..RES_AIR.sens,
		CFG..RES_AIR.capacite_VL,
		CFG..RES_AIR.capacite_PL,
		CFG..RES_POR.district,
		CFG..RES_AIR.supprime,
		CFG..RES_AIR.sous_concessionnaire
	from CFG..RES_AIR, CFG..RES_POR
	where CFG..RES_POR.numero = CFG..RES_AIR.portion
	order by upper(CFG..RES_AIR.nom)
	
	return XDC_OK
go
