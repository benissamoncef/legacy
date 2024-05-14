/*E*/
/*  Fichier : $Id: xzah08sp.prc,v 1.4 1997/05/21 11:09:58 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/05/21 11:09:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	14/10/94	: Creation
* B.G.	26/12/94	: modif type typeraf (1.2)
* niepceron	27/08/94	: Ajout du traitement par BLOC et arg d'entree devient TINYINT (1.3)
* niepceron     09/01/97	: Ajout du num ordre (dem/1372) 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP XZAH08_Envoyer_Objet_Mobile_Aff_Voies
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Booleen   va_DebFin
* XDY_Mot       va_IdPortion_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Entier    va_CoefTranslation_out
* XDY_Octet     va_NVoie1_out
* XDY_Octet     va_AVoie1_out
* XDY_Octet     va_NVoie2_out
* XDY_Octet     va_AVoie2_out
* XDY_Octet     va_NVoie3_out
* XDY_Octet     va_AVoie3_out
* XDY_Octet     va_NVoie4_out
* XDY_Octet     va_AVoie4_out
* XDY_Octet     va_NVoie5_out
* XDY_Octet     va_AVoie5_out
* XDY_Octet     va_NVoie6_out
* XDY_Octet     va_AVoie6_out
* XDY_Octet     va_NVoie7_out
* XDY_Octet     va_AVoie7_out
* XDY_Octet     va_NVoie8_out
* XDY_Octet     va_AVoie8_out
* XDY_Octet     va_NVoie9_out
* XDY_Octet     va_AVoie9_out
* XDY_Octet     va_NVoie10_out
* XDY_Octet     va_AVoie10_out
* XDY_Octet	va_numordre
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

create procedure  XZAH;08
	@va_Init_in		tinyint=XDC_FAUX
as
	begin tran
/*A 1er cas : la variable de demande d'initialisation est positionn≈e */

	if @va_Init_in=XDC_OUI
	begin
	
/*B		on retourne toutes les anciennes lignes en "SUPPRESSION"*/

		(select	evt=evenement,
			cle,
			debut_fin,
			portion,
			typeraf=convert(tinyint,XDC_SUPPR),
			translation,
			nvoie1,
			vvoie1,
			nvoie2,
			vvoie2,
			nvoie3,
			vvoie3,
			nvoie4,
			vvoie4,
			nvoie5,
			vvoie5,
			nvoie6,
			vvoie6,
			nvoie7,
			vvoie7,
			nvoie8,
			vvoie8,
			nvoie9,
			vvoie9,
			nvoie10,
			vvoie10,
			numordre
		from EXP..SYN_RAB_SEG holdlock
		where ordre<>XDC_AJOUT
		union
		
/*B		on retourne toutes les anciennes lignes non marqu≈es
		"SUPPRESSION" en les marquant "AJOUT" */
		
		select	evenement,
			cle,
			debut_fin,
			portion,
			typeraf=convert(tinyint,XDC_AJOUT),
			translation,
			nvoie1,
			vvoie1,
			nvoie2,
			vvoie2,
			nvoie3,
			vvoie3,
			nvoie4,
			vvoie4,
			nvoie5,
			vvoie5,
			nvoie6,
			vvoie6,
			nvoie7,
			vvoie7,
			nvoie8,
			vvoie8,
			nvoie9,
			vvoie9,
			nvoie10,
			vvoie10,
			numordre
		from EXP..SYN_RAB_SEG holdlock
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
			debut_fin,
			portion,
			typeraf=convert(tinyint,XDC_AJOUT),
			translation,
			nvoie1,
			vvoie1,
			nvoie2,
			vvoie2,
			nvoie3,
			vvoie3,
			nvoie4,
			vvoie4,
			nvoie5,
			vvoie5,
			nvoie6,
			vvoie6,
			nvoie7,
			vvoie7,
			nvoie8,
			vvoie8,
			nvoie9,
			vvoie9,
			nvoie10,
			vvoie10,
			numordre
		from EXP..SYN_RAB_SEG holdlock
		where ordre<>XDC_SUPPR
		order by evenement,typeraf desc
	end

/*A 2e cas : la variable de demande d'initialisation n'est pas positionn≈e */

	else
	begin

/*B		on retourne uniquement les lignes qui n'ont pas ≈t≈ lues */
	
		select	evt=evenement,
			cle,
			debut_fin,
			portion,
			typeraf=ordre,
			translation,
			nvoie1,
			vvoie1,
			nvoie2,
			vvoie2,
			nvoie3,
			vvoie3,
			nvoie4,
			vvoie4,
			nvoie5,
			vvoie5,
			nvoie6,
			vvoie6,
			nvoie7,
			vvoie7,
			nvoie8,
			vvoie8,
			nvoie9,
			vvoie9,
			nvoie10,
			vvoie10,
			numordre
		from EXP..SYN_RAB_SEG holdlock
		where ordre<>XDC_LU
		order by evt,typeraf desc
	end
	

/*A	on efface les lignes marqu≈es "SUPPRESSION" et on marque "LU" les lignes envoy≈es si le traitement n'est pas par bloc */

	if @va_Init_in!=XDC_TRAIT_BLOC
	begin
		delete EXP..SYN_RAB_SEG
		where ordre=XDC_SUPPR	

		update EXP..SYN_RAB_SEG
		set ordre=XDC_LU
	end

	commit tran
	return XDC_OK
go
