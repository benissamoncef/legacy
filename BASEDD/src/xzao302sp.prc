/*E*/
/*  Fichier : $Id: xzao302sp.prc,v 1.3 2005/03/07 17:47:31 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2005/03/07 17:47:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao302sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* C.T.	12/10/95	: Ajout des echangeurs avec peages 
*                         si parametre d'entree positionne a vrai	(V 1.2)
* P.NI	07/03/05	: Ajout #PEAGES pour tri par Nom V1.3 DEM438
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* retourne la liste des gares de peages
* 
* Sequence d'appel
* SP	XZAO302_Liste_Gares_Peage_Ax
* 
* Arguments en entree
* XDY_Octet		va_AvecPeageEch_in 
* 
* Arguments en sortie
* 
* XDY_Mot		va_Numero_out		-- arguments
* XDY_Nom		va_Nom_out		-- retournes
* char(15)		va_Abrev_out		-- dans une liste
* XDY_Octet		va_Autoroute_out
* XDY_PR		va_PRSens1_out
* XDY_PR		va_PRSens2_out
* XDY_District		va_District_out
* XDY_Booleen		va_Supprime_out
* 
* Code retour
* XDC_OK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* Toutes les informations sont retournees dans une liste
* 
* Fonction
* Select from RES_PEA
---------------------------------------------------*/

use PRC
go


create procedure XZAO;302
	@va_AvecPeageEch_in	tinyint	= null
as
	/*A rechercher tous les peages */
	if @va_AvecPeageEch_in is null
		select
			CFG..RES_PEA.numero,
			CFG..RES_PEA.nom,
			CFG..RES_PEA.abreviation,
			CFG..RES_POR.autoroute,
			CFG..RES_PEA.PRsens1,
			CFG..RES_PEA.PRsens2,
			CFG..RES_POR.district,
			CFG..RES_PEA.supprime
		from CFG..RES_PEA, CFG..RES_POR
		where CFG..RES_POR.numero = CFG..RES_PEA.portion
		order by CFG..RES_PEA.nom
	else
	begin
        	/*A Creation du tableau de stockage des peage PV et echangeurs */
        	create table #PEAGES
                (
    		numero                          tinyint               not null,
    		nom                             char(25)                 not null,
    		abreviation                     char(15)              not null,
    		autoroute                       tinyint               not null,
    		PRsens1                         int                  null,
    	    	PRsens2                         int                  null,
    		district                        tinyint               not null,
    		supprime                        bit
                )
                insert #PEAGES
		select
			CFG..RES_PEA.numero,
			CFG..RES_PEA.nom,
			CFG..RES_PEA.abreviation,
			CFG..RES_POR.autoroute,
			CFG..RES_PEA.PRsens1,
			CFG..RES_PEA.PRsens2,
			CFG..RES_POR.district,
			CFG..RES_PEA.supprime
		from CFG..RES_PEA, CFG..RES_POR
		where CFG..RES_POR.numero = CFG..RES_PEA.portion

		union
		(select
			CFG..RES_ECH.numero,
			CFG..RES_ECH.nom,
			CFG..RES_ECH.abreviation,
			CFG..RES_POR.autoroute,
			isnull(CFG..RES_ECH.PR_entree_sens1, PR_sortie_sens1),
			isnull(CFG..RES_ECH.PR_entree_sens2, PR_sortie_sens2),
			CFG..RES_POR.district,
			CFG..RES_ECH.supprime
		from CFG..RES_ECH, CFG..RES_POR
		where CFG..RES_POR.numero = CFG..RES_ECH.portion and CFG..RES_ECH.supprime=XDC_FAUX and
			(peage_entree_sens1 = XDC_VRAI or peage_sortie_sens1 = XDC_VRAI or 
			peage_entree_sens2 = XDC_VRAI or peage_sortie_sens2 = XDC_VRAI))
		
		select * from #PEAGES order by upper(nom)
	end
	return XDC_OK
go
