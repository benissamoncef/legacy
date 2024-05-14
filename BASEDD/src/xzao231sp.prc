/*E*/
/*  Fichier : $Id: xzao231sp.prc,v 1.2 2011/01/03 18:50:34 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2011/01/03 18:50:34 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao231sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	03/01/10 : Creation (DEM 970)  1.1
* JPL	03/01/10 : Ajout contrainte type equipement sur table des sequences (DEM 970)  1.2
-----------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des Sequences autorisees d'un equipement GTC
*  d'un type donne dans le domaine Signalisation.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO231_Seq_Auto_GTC_SIG
*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in	: nom de la base de configuration ;
* XDY_TypeEqt		va_TypeEqt_in	: type d'equipement ;
* XDY_Eqt		va_NumEqt_in	: numero d'equipement.
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
* Retourne la liste des Sequences autorisees d'un equipement donne :
*
* XDY_Octet		va_NumSequence_out	: numero de la sequence ;
* XDY_Nom		va_NomSequence_out	: nom de la sequence ;
* XDY_Octet		va_Supprime_out		: indicateur de suppression.
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : base ou equipement inconnu ou de type incorrect
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* < 0               : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Recherche dans la base indiquee (CFG, CFT) et liste les sequences
*  autorisees pour l'equipement de type et de numero specifies.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO231' and type = 'P')
	drop procedure XZAO231
go


create procedure XZAO231
	@va_Basedd_in		char(3),
	@va_TypeEqt_in		tinyint,
	@va_NumEqt_in		smallint

as 

	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_TypeEqt_in = 0		select @va_TypeEqt_in = null
	if @va_NumEqt_in = 0		select @va_NumEqt_in = null



	/*A
	** Verifier la validite des parametres d'entree obligatoires
	*/

	if @va_Basedd_in is null or
	   @va_TypeEqt_in is null or
	   @va_NumEqt_in is null
		return XDC_ARG_INV


	/*A
	** Travail dans la base CFG
	*/

	if @va_Basedd_in = XDC_BASE_CFG
	begin

		/*A
		** Verifier l'existence de l'equipement
		*/

		if not exists (select nom from CFG..EQT_GEN
				where type = @va_TypeEqt_in and numero = @va_NumEqt_in)
			return XDC_NOK


		/*A
		** Rechercher la liste des Sequences autorisees pour l'equipement
		*/

		select distinct SQAT.sequence,
				SQT.nom,
				SQT.supprime,
				SQT.nature_seq

		from CFG..EQT_SQA_GTC_SIG SQAT, CFG..EQT_SEQ_GTC_SIG SQT

		where SQAT.type		= @va_TypeEqt_in
		  and SQAT.equipement	= @va_NumEqt_in
		  and SQT.numero	= SQAT.sequence
		  and SQT.type		= @va_TypeEqt_in

		order by SQAT.sequence

	end

#ifdef CI

	else
	if @va_Basedd_in = XDC_BASE_CFT

	/*A
	** Travail dans la base CFT
	*/

	begin

		/*A
		** Verifier l'existence de l'equipement
		*/

		if not exists (select nom from CFT..EQT_GEN
				where type = @va_TypeEqt_in and numero = @va_NumEqt_in)
			return XDC_NOK


		/*A
		** Rechercher la liste des Sequences autorisees pour l'equipement
		*/

		select distinct SQAT.sequence,
				SQT.nom,
				SQT.supprime,
				SQT.nature_seq

		from CFT..EQT_SQA_GTC_SIG SQAT, CFT..EQT_SEQ_GTC_SIG SQT

		where SQAT.type		= @va_TypeEqt_in
		  and SQAT.equipement	= @va_NumEqt_in
		  and SQT.numero	= SQAT.sequence
		  and SQT.type		= @va_TypeEqt_in

		order by SQAT.sequence

	end

#endif

	else
		/*B nom base errone */
		return XDC_NOK

	return XDC_OK

go
