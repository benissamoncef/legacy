/*E*/
/*  Fichier : $Id: xzah05sp.prc,v 1.3 1996/09/06 11:21:49 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/09/06 11:21:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah05sp.prc
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
* SP XZAH05_Envoyer_Basculement_Bouchon
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmcBas_out
* XDY_Octet     va_CleFmcBas_out
* XDY_Entier    va_NumFmcBou_out
* XDY_Octet     va_CleFmcBou_out
* XDY_Mot       va_IdPortion_out
* XDY_Entier    va_PR_out
* XDY_Sens      va_Sens_out
* XDY_Voie      va_Voie_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Entier    va_CoefTranslation_out
* XDY_Entier    va_CoefExtension_out
* XDY_Octet     va_Niveau_out
*
* Service rendu
* Envoie � l'IHM les modifications des variables d'animation synoptique
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

create procedure  XZAH;05
	@va_Init_in		tinyint=XDC_FAUX
as
	begin tran
/*A 1er cas : la variable de demande d'initialisation est positionn�e */

	if @va_Init_in=XDC_OUI
	begin
	
/*B		on retourne toutes les anciennes lignes en "SUPPRESSION"*/

		(select	evt=evenement_basc,
			cle_basc,
			evenement_bou,
			cle_bou,
			portion,
			PR,
			sens,
			voie,
			typeraf=convert(tinyint,XDC_SUPPR),
			translation,
			extension,
			niveau
		from EXP..SYN_BAS_SEG holdlock
		where ordre<>XDC_AJOUT
		union
		
/*B		on retourne toutes les anciennes lignes non marqu�es
		"SUPPRESSION" en les marquant "AJOUT" */
		
		select	evenement_basc,
			cle_basc,
			evenement_bou,
			cle_bou,
			portion,
			PR,
			sens,
			voie,
			typeraf=convert(tinyint,XDC_AJOUT),
			translation,
			extension,
			niveau
		from EXP..SYN_BAS_SEG holdlock
		where ordre<>XDC_SUPPR)
		order by evt,typeraf desc
	end

/*A 2e cas : la variable de demande d'initialisation est positionn�e au traitement par bloc */

	else
	if @va_Init_in=XDC_TRAIT_BLOC
	begin
/*B		on retourne toutes les anciennes lignes non marqu�es
		"SUPPRESSION" en les marquant "AJOUT" */
		
		
		select	evenement_basc,
			cle_basc,
			evenement_bou,
			cle_bou,
			portion,
			PR,
			sens,
			voie,
			typeraf=convert(tinyint,XDC_AJOUT),
			translation,
			extension,
			niveau
		from EXP..SYN_BAS_SEG holdlock
		where ordre<>XDC_SUPPR
		order by evenement_basc,typeraf desc
	end

/*A 2e cas : la variable de demande d'initialisation n'est pas positionn�e */

	else
	begin

/*B		on retourne uniquement les lignes qui n'ont pas �t� lues */
	
		select	evt=evenement_basc,
			cle_basc,
			evenement_bou,
			cle_bou,
			portion,
			PR,
			sens,
			voie,
			typeraf=ordre,
			translation,
			extension,
			niveau
		from EXP..SYN_BAS_SEG holdlock
		where ordre<>XDC_LU
		order by evt,typeraf desc
	end
	

/*A	on efface les lignes marqu�es "SUPPRESSION" et on marque "LU" les lignes envoy�es si le traitement n'est pas par bloc */

	if @va_Init_in!=XDC_TRAIT_BLOC
	begin
		delete EXP..SYN_BAS_SEG
		where ordre=XDC_SUPPR	
		update EXP..SYN_BAS_SEG
		set ordre=XDC_LU
	end

	commit tran
	return XDC_OK
go
