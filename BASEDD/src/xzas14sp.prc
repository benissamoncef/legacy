/*E*/
/*  Fichier : $Id: xzas14sp.prc,v 1.4 2021/01/14 12:44:15 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/01/14 12:44:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	03/02/97	: Creation	(V 1.1)
* CGR	11/12/20	: Creation pour fiche metier TDP FCD DEM-SAE152 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Fournit le seuil max, bas et haut ainsi que les pr de 
* de fin, l'autoroute et le sens d'une Zdp
* 
* Sequence d'appel
* SP	XZAS14_Lire_Caract_Zdp
* 
* Arguments en entree
* XDY_Mot	va_zdp_in		: numero de zone de temps de parcours
* 
* Arguments en sortie
* 
* XDY_Mot	va_seuil_max_out	: seuil max de la Zdp
* XDY_Mot	va_seuil_bas_out	: seuil bas de la Zdp
* XDY_Mot	va_seuil_haut_out	: seuil haut de la Zdp
* XDY_Nom	va_NomAutoroute_out	: PR du debut
* XDY_Entier	va_PRdebut_out		: PR du debut
* XDY_Entier	va_PRfin_out		: PR du debut
* XDY_Octet	va_Sens_out		: PR du debut
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  zdp inexistante
* XZASC_RADT_PAS_MES	: pas de mesures
* 
* Conditions d'utilisation
*
*
* Fonction
* selectionne dans la table CFG..ZDP_TDP les caract�ristiques
* d'une sone de tdp
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS14' and type = 'P')

    drop procedure XZAS14

go

create procedure XZAS14
	@va_Zdp_in		smallint	= null,
	@va_seuil_max_out	smallint	output,
	@va_seuil_bas_out	smallint	output,
	@va_seuil_haut_out	smallint	output,
	@va_NomAutoroute_out	char(4)		output,
	@va_PRdebut_out		int		output,
	@va_PRfin_out		int		output,
	@va_Sens_out		tinyint		output
	
as

	/*A controle parametres en entree */
	if (@va_Zdp_in = null  )
		return XDC_ARG_INV

	
	select
		@va_seuil_max_out=TDP_max,
		@va_seuil_bas_out=convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_basse))/100),
		@va_seuil_haut_out=convert(smallint,convert(int,convert(int,TDP_habituel)*convert(int,tolerance_haute))/100),
		@va_NomAutoroute_out=rtrim(AUT.nom),
		@va_PRdebut_out=EQT.PR,
		@va_PRfin_out=ZDP.PR_fin,
		@va_Sens_out=EQT.sens
	from CFG..EQT_GEN EQT, CFG..EQT_ZDP ZDP, CFG..RES_AUT AUT
	where EQT.numero=@va_Zdp_in and
	      EQT.type = 39 and /* XDC_EQT_ZDP */
	      EQT.numero=ZDP.numero and
	      EQT.autoroute=AUT.numero
	
		
	return XDC_OK
go
