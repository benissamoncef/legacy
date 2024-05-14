/*E*/
/*  Fichier : $Id: xzao304sp.prc,v 1.2 1995/01/27 16:56:47 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/27 16:56:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao304sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* B.G.	27/01/91	: suppression group by	(V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des lits d'arret
* 
* Sequence d'appel
* SP	XZAO304_Liste_Lits_Arret_Ax
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* XDY_Octet		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* XDY_Octet		va_Autoroute_out	-- dans une liste
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* XDY_District		va_District_out
* XDY_Booleen		va_Supprime_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* L'identicateur de suppression est toujours positionne a faux
* 
* Fonction
* Select from EQT_GEN
---------------------------------------------------*/

use PRC
go


create procedure XZAO;304
as
	declare @vl_Supprime bit

	select @vl_Supprime = XDC_NON

	/*A rechercher tous les lits d'arret */
	select distinct
		CFG..EQT_GEN.numero,
		CFG..EQT_GEN.nom,
		CFG..EQT_GEN.autoroute,
		CFG..EQT_GEN.PR,
		CFG..EQT_GEN.sens,
		EXP..EQT_DSP.sit,
		@vl_Supprime
	from CFG..EQT_GEN, EXP..EQT_DSP
	where CFG..EQT_GEN.type = XDC_CAPT_LIT_ARRET
	  and CFG..EQT_GEN.numero=EXP..EQT_DSP.equipement
	  and CFG..EQT_GEN.type=EXP..EQT_DSP.type

	order by CFG..EQT_GEN.autoroute
	return XDC_OK
go
