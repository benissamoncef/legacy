/*E*/
/*  Fichier : $Id: xzah07sp.prc,v 1.9 2007/03/26 15:28:14 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2007/03/26 15:28:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah07sp.prc
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
* C.T.	25/03/95	: recherche le nom de l'echangeur
*                         dans le cas d'evenement sur echangeur (1.3)
* C.T.	30/03/95	: Retourner le nom de l'echangeur
*                         en tronquant les blancs en fin (1.5)
* niepceron	27/08/94	: Ajout du traitement par BLOC et arg d'entree devient TINYINT (1.6)
* Hilmarcher 24/02/05   : modif ecriture TAB + # car pb a la compil DEM423 v1.7
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP XZAH07_Envoyer_Objet_Mobile_Portion
* 
* Arguments en entree
* XDY_Booleen	va_Init_in
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Mot       va_IdPortion_out
* XDY_octet     va_TypeRaf_out XDC_AJOUT,MODIF,SUPPR
* XDY_Entier    va_NumAction_out
* XDY_Octet     va_CleAction_out
* XDY_Octet     va_TypeObjet_out
* XDY_Sens      va_Sens_out
* XDY_Voie      va_Voie_out
* XDY_Entier    va_CoefTranslation_out
* XDY_Nom    	va_NomEchangeur_out
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
* Dans le cas d'evenement sur echangeur, le coefficient
* de translation contient la position sur echangeur, la portion contient
* le numero d'echangeur et le nom de l'echangeur est value a non
* chaine vide (XDC_CHAINE_VIDE)
* 
---------------------------------------------------*/

use PRC
go

create procedure  XZAH;07
	@va_Init_in		tinyint=XDC_FAUX
as
	declare @vl_Evt int, @vl_Cle tinyint, @vl_Echangeur varchar(25),
	@vl_NumEchangeur smallint

	create table #OBJ_MOB (	evenement	int,
				cle		tinyint,
				portion		smallint,
				typeraf		tinyint,
				actio		int	null,
				cle_action	tinyint	null,
				type_objet	tinyint,
				sens		tinyint,
				voie		tinyint,
				translation	int,
				echangeur	varchar(25) )
	begin tran
/*A 1er cas : la variable de demande d'initialisation est positionn≈e */

	if @va_Init_in=XDC_OUI
	begin
	
/*B		on retourne toutes les anciennes lignes en "SUPPRESSION"*/

		insert #OBJ_MOB(evenement, cle, portion, typeraf, actio, 
				cle_action, type_objet, sens, voie, translation,echangeur)
		(select	evt=evenement,
			cle,
			portion,
			typeraf=convert(tinyint,XDC_SUPPR),
			actio,
			cle_action,
			type_objet,
			sens,
			voie,
			translation,
			echangeur=convert(varchar,XDC_CHAINE_VIDE)
		from EXP..SYN_OBJ_POR holdlock
		where ordre<>XDC_AJOUT
		union	

/*B		on retourne toutes les anciennes lignes non marqu≈es
		"SUPPRESSION" en les marquant "AJOUT" */
		
		select	evenement,
			cle,
			portion,
			typeraf=convert(tinyint,XDC_AJOUT),
			actio,
			cle_action,
			type_objet,
			sens,
			voie,
			translation,
			echangeur=convert(varchar,XDC_CHAINE_VIDE)
		from EXP..SYN_OBJ_POR holdlock
		where ordre<>XDC_SUPPR)
		order by evt,typeraf desc
	end

/*A 2e cas : la variable de demande d'initialisation est positionn≈e au traitement par bloc */

	else
	if @va_Init_in=XDC_TRAIT_BLOC
	begin
/*B		on retourne toutes les anciennes lignes non marqu≈es
		"SUPPRESSION" en les marquant "AJOUT" */

	        insert #OBJ_MOB(evenement, cle, portion, typeraf, actio,
					      cle_action, type_objet, sens, voie, translation,echangeur)
		select	evenement,
			cle,
			portion,
			typeraf=convert(tinyint,XDC_AJOUT),
			actio,
			cle_action,
			type_objet,
			sens,
			voie,
			translation,
			echangeur=convert(varchar,XDC_CHAINE_VIDE)
		from EXP..SYN_OBJ_POR holdlock
		where ordre<>XDC_SUPPR
		order by evenement,typeraf desc
	end

/*A 2e cas : la variable de demande d'initialisation n'est pas positionn≈e */

	else
	begin

/*B		on retourne uniquement les lignes qui n'ont pas ≈t≈ lues */
	
		insert #OBJ_MOB(evenement, cle, portion, typeraf, actio, 
				cle_action, type_objet, sens, voie, translation,echangeur)
		select	evt=evenement,
			cle,
			portion,
			typeraf=ordre,
			actio,
			cle_action,
			type_objet,
			sens,
			voie,
			translation,
			echangeur=convert(varchar,XDC_CHAINE_VIDE)
		from EXP..SYN_OBJ_POR holdlock
		where ordre<>XDC_LU
		order by evt,typeraf desc
	end
	

/*A	on efface les lignes marqu≈es "SUPPRESSION" et on marque "LU" les lignes envoy≈es si le traitement n'est pas par bloc */

	if @va_Init_in!=XDC_TRAIT_BLOC
	begin
		delete EXP..SYN_OBJ_POR
		where ordre=XDC_SUPPR	
		update EXP..SYN_OBJ_POR
		set ordre=XDC_LU
	end

	commit tran

/*A pour tous les evenements nouveaux ou en modification sur echangeur */
/*A rechercher le nom de l'echangeur                                   */
/*! dans la portion se trouve le numero de l'echangeur                 */

	update #OBJ_MOB set echangeur = isnull (nom,XDC_CHAINE_VIDE)
	from CFG..RES_ECH
	where numero = #OBJ_MOB.portion and ( #OBJ_MOB.sens = XDC_SENS_SORTIE_NORD or #OBJ_MOB.sens = XDC_SENS_ENTREE_NORD or #OBJ_MOB.sens = XDC_SENS_SORTIE_SUD or #OBJ_MOB.sens = XDC_SENS_ENTREE_SUD )

/*A retourner la liste des objets mobiles */
	select 
		evenement,
		cle,
		portion,
		typeraf,
		actio,
		cle_action,
		type_objet,
		sens,
		voie,
		translation,
		echangeur
	from #OBJ_MOB
	order by evenement,typeraf desc


	return XDC_OK
go
