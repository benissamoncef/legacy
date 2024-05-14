/*E*/
/*Fichier : $Id: xzao811sp.prc,v 1.1 2007/07/26 16:32:06 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/07/26 16:32:06 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao811sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture et ecriture de la configuration
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	26/07/07 : Creation 1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des analyseurs d'un équipement DAI
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO811' and type = 'P')
	drop procedure XZAO811
go


create procedure XZAO811
	@va_Basedd_in		char(3),
	@va_NumDAI_in		smallint
as 

/*
*
* SP	XZAO811_Liste_Analyseurs_De_La_DAI
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Eqt		va_NumDAI_in
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
*  Retourne la liste des analyseurs de l'equipement DAI de numero indique.
*
* CODE RETOUR :
*
* XDC_OK
* XDC_ARG_INV : parametre d'entree obligatoire Null
* <0 erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
-----------------------------------------------------------------------------*/

declare
	@vl_NumEqt		smallint,
	@vl_NumAna		tinyint,
	@vl_NbVoies		tinyint


	/*A Bug applix null */
	if rtrim ( @va_Basedd_in ) = null	select @va_Basedd_in = null
	if @va_NumDAI_in = 0			select @va_NumDAI_in = null


	/*A Controle des parametres d'entree obligatoires */
	if @va_Basedd_in = null
	or @va_NumDAI_in = null
		return XDC_ARG_INV


	/*A
	**  Travail dans la base CFG
	*/

	if @va_Basedd_in = XDC_BASE_CFG
	begin
		/*A
		**  Recherche dans la table EQT_GEN des équipements
		**  dont l'équipement maitre est la DAI indiquée
		*/

		select @va_NumDAI_in, GEN.numero, nom, autoroute, PR, ANA.PR_fin, sens, ANA.numero_analyseur, ANA.nombre_de_voies
		  from CFG..EQT_GEN GEN, CFG..EQT_ANA ANA
		 where GEN.type = XDC_EQT_ANA
		   and GEN.type_maitre = XDC_MAI_ANA
		   and GEN.maitre = @va_NumDAI_in
		   and ANA.numero = GEN.numero
		 order by ANA.numero_analyseur
	end

#ifdef CI

	/*A
	**  Travail dans la base CFT
	*/

	else if @va_Basedd_in = XDC_BASE_CFT
	begin
		/*A
		**  Recherche dans la table EQT_GEN des équipements
		**  dont l'équipement maitre est la DAI indiquée
		*/

		select @va_NumDAI_in, GEN.numero, nom, autoroute, PR, ANA.PR_fin, sens, ANA.numero_analyseur, ANA.nombre_de_voies
		  from CFT..EQT_GEN GEN, CFT..EQT_ANA ANA
		 where GEN.type = XDC_EQT_ANA
		   and GEN.type_maitre = XDC_MAI_ANA
		   and GEN.maitre = @va_NumDAI_in
		   and ANA.numero = GEN.numero
		 order by ANA.numero_analyseur
	end

#endif

	else
		/*B nom base errone */
		return XDC_NOK

	return XDC_OK
go
