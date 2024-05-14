/*E*/
/*  Fichier : $Id: xzat10sp.prc,v 1.11 2021/03/08 13:41:09 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2021/03/08 13:41:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	22/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix V 1.2
* C.T.	29/11/94	: procedure differente au CI V 1.3
* B.G.	10/01/95	: test si equipement est deja desactive V 1.4
* C.T.	13/09/96	: Ajout cas sur serveurs DS et DC (RADT 1.5)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*			  les procs (V 1.6) (dem / 1536)
* PN	22/09/98	: Ajout cas sur serveurs DY DP (dem/1700) 1.7
* LCL   09/07/07        : Suppression DY 1.8
* JMG   02/10/09        : secto DEM887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.10
* LCL	22/01/21	: MOVIS PRA DEM-SAE93 1.11
* GGY	26/09/23	: Ajout district CA (DEM483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAT10_Eqt_A_Desactiver
* 
* Arguments en entree
* XDY_Eqt	va_NumEqt_in
* XDY_Octet	va_TypeEqt_in
* 
* Arguments en sortie
* 
* Service rendu
* Met l'equipement a desactiver
* 
* Code retour
* XDC_OK
* XDC_NOK	: equipement non trouve, ou site inconnu
* XDC_ARG_INV	: parametres d'entree null
* XDC_PRC_INC   : procedure pas creee chez un district
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Update dans la table ACTIVATION
--------------------------------------------------------*/

use PRC
go

create procedure XZAT;10
	@va_NumEqt_in	T_EQUIPEMENT	= null,
	@va_TypeEqt_in	tinyint	= null
as
	declare @vl_Site T_SITE, @vl_Status int

	/*! transfo en parametres nulls bug applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_TypeEqt_in = 0 select @va_TypeEqt_in = null

	/*A controle parametre obligatoire */
	if @va_NumEqt_in = null or @va_TypeEqt_in = null
		return XDC_ARG_INV

	/*A recherche le site */
	select  @vl_Site = sit from EXP..EQT_ACV
	where equipement = @va_NumEqt_in and type = @va_TypeEqt_in

	if @@rowcount != 1
		return XDC_NOK

#ifdef CI
	/*! procedure creee au CI */

	/*A si le site n'est pas local alors demander au site */
	/*A concerne de faire l'action                        */
	if @vl_Site != XDC_CI and @vl_Site != XDC_CA
	begin
		if @vl_Site = XDC_VC
			exec @vl_Status = SQL_VC.PRC..XZAT;10 
				@va_NumEqt_in, @va_TypeEqt_in
		else if @vl_Site = XDC_DP
			exec @vl_Status = SQL_DP.PRC..XZAT;10 
				@va_NumEqt_in, @va_TypeEqt_in
		else
			return XDC_NOK

		if @vl_Status = null
			return XDC_PRC_INC
		else return @vl_Status
	end
#else
	/*! procedure creee au district */

	/*A test si action est possible */
	if not exists (select serveur from CFG..RES_DIS 
/*		where numero = @vl_Site and serveur = @@servername )
*/		where numero = @vl_Site)
	    return ( XDC_NOK )
#endif

	if exists (select * from EXP..EQT_DSP
			where equipement = @va_NumEqt_in
			  and type = @va_TypeEqt_in
			  and dernier = XDC_OUI
			  and desactive = XDC_NON )

	begin
		update EXP..EQT_ACV set a_desactiver = XDC_OUI
		where equipement = @va_NumEqt_in and type = @va_TypeEqt_in

		if @@rowcount != 1
			return XDC_NOK
	end
	
	return XDC_OK

go
