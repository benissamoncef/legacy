/*E*/
/*  Fichier : $Id: xzao10sp.prc,v 1.3 2010/10/29 10:36:22 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2010/10/29 10:36:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO10_Couverture_Camera
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		NumCamera
*
* PARAMETRES EN SORTIE :
*
* liste des positions (254=couverture totale) : no position, autoroute,pr1,pr2,sens
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION : BASE CFG
*
* FONCTION :
*
*  Retourne la liste des positions d'une camera
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	29 jan 1995	: Creation (1.1)
* C.T.		21 Mar 1995     : Correction erreur de compile (1.2)
* JPL		29/10/2010 : Degroupee; controle validite args; retourne la pertinence (DEM 949)  1.3
------------------------------------------------------*/

 
use PRC
go

if exists (select * from sysobjects where name = 'XZAO10' and type = 'P')
	drop procedure XZAO10
go


create procedure XZAO10
	@va_NumCamera_in	smallint
as

	/*A Transformation des parametres d'entree nulls (bug applix) */
	if @va_NumCamera_in = 0
		select @va_NumCamera_in = null


	/*A
	** Verifier la validite des parametres d'entree obligatoires
	*/

	if @va_NumCamera_in is null
		return XDC_ARG_INV

	/* verifier l'existence du numero dans EQT_CAM ... retour liste vide */


	/*A
	** Rechercher les positions de la camera indiquee
	*/

	select	numero,
		autoroute,
		PR_debut,
		PR_fin,
		sens,
		pertinence

	from CFG..EQT_CAM_POS

	where camera=@va_NumCamera_in

	order by numero


	return XDC_OK
go            
