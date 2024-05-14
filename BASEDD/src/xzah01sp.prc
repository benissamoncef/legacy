/*E*/
/*  Fichier : $Id: xzah01sp.prc,v 1.3 1996/09/06 11:21:44 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/09/06 11:21:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	13/10/94	: Creation
* B.G.	26/12/94	: modif type typeraf (1.2)
* niepceron	27/08/94	: Ajout du traitement par BLOC et arg d'entree devient TINYINT (1.3)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAH01_Envoyer_Bouchon_Chaussee_Portion
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Mot       va_IdPortion_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Sens      va_Sens_out
* XDY_Entier    va_CoefTranslation_out
* XDY_Entier    va_CoefExtension_out
* XDY_Octet     va_niveau_out
*
* Service rendu
* Envoie » l'IHM les modifications des variables d'animation synoptique
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
---------------------------------------------------*/

use PRC
go

create procedure  XZAH;01
	@va_Init_in		tinyint=XDC_FAUX
as
	begin tran
/*A 1er cas : la variable de demande d'initialisation est positionn≈e */

	if @va_Init_in=XDC_OUI
	begin
	
/*B		on retourne toutes les anciennes lignes en "SUPPRESSION"*/

		(select	evt=evenement,
			cle,
			portion,
			typeraf=convert(tinyint,XDC_SUPPR),
			sens,
			translation,
			extension,
			niveau
		from EXP..SYN_BOU_POR holdlock
		where ordre<>XDC_AJOUT
		union
		
/*B		on retourne toutes les anciennes lignes non marqu≈es
		"SUPPRESSION" en les marquant "AJOUT" */
		
		select	evenement,
			cle,
			portion,
			typeraf=convert(tinyint,XDC_AJOUT),
			sens,
			translation,
			extension,
			niveau
		from EXP..SYN_BOU_POR holdlock
		where ordre<>XDC_SUPPR)
		order by evt,typeraf desc
	end

/*A 2e cas : la variable de demande d'initialisation est positionn≈e au traitement par bloc */

	else
	if @va_Init_in=XDC_TRAIT_BLOC
	begin
/*B		on retourne toutes les anciennes lignes non marqu≈es
		"SUPPRESSION" en les marquant "AJOUT" */
		
		select	evenement,
			cle,
			portion,
			typeraf=convert(tinyint,XDC_AJOUT),
			sens,
			translation,
			extension,
			niveau
		from EXP..SYN_BOU_POR holdlock
		where ordre<>XDC_SUPPR
		order by evenement,typeraf desc
	
	end
	else
/*A 3e cas : la variable de demande d'initialisation n'est pas positionn≈e */
	begin

/*B		on retourne uniquement les lignes qui n'ont pas ≈t≈ lues */
	
		select	evt=evenement,
			cle,
			portion,
			typeraf=ordre,
			sens,
			translation,
			extension,
			niveau
		from EXP..SYN_BOU_POR holdlock
		where ordre<>XDC_LU
		order by evt,typeraf desc
	end
	

/*A	on efface les lignes marqu≈es "SUPPRESSION" et on marque "LU" les lignes envoy≈es si le traitement n'est pas par bloc */
/*A dans tous les cas : on efface les lignes marqu≈es "SUPPRESSION" */

	if @va_Init_in!=XDC_TRAIT_BLOC
	begin
		delete EXP..SYN_BOU_POR
		where ordre=XDC_SUPPR	
		
		update EXP..SYN_BOU_POR
		set ordre=XDC_LU
	end
	commit tran
	return XDC_OK
go
