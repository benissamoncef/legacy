/*E*/
/*  Fichier : @(#)xzas33sp.prc  1.1      Release : 1.1        Date : 11/29/94
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas33sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  09/12/94        : Creation      (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les informations necessaires pour le calcul des indices qualites
* des entites logicielles.
*
* Sequence d'appel
* SP   XZAS33_Info_Indice_Qualite 
*
* Arguments en entree :
* Un evenement est compose d'un numero et d'une cle
* XDY_Entier    @va_Numero_in		: Numero de l'evement
* XDY_Mot	@va_Cle_in		: Valeur de la cle
* XDY_Date	@vl_TE0			: Date de debut d'evenement
* XDY_PR	@vl_PR_EVNT		: PR de l'evenement
*
* Arguments en entree :
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;33
	@va_Numero_in		int		= null,
	@va_Cle_in		smallint	= null,
	@va_TE0_in              datetime	= null,
	@va_PR_EVNT_in          T_PR		= null
as
	declare	@vl_Numero		int,	 /* Numero d'action */
		@vl_Station_Troncon	int,	 /* Num station echangeur */
		@vl_Station_Entree	int,     /* Num station entre echgr */
		@vl_Vitesse		int,	 /* Vitesse */
		@vl_Debit		int,	 /* Debit */
		@vl_PR			T_PR,	 /* PR echangeur */
		@vl_Autoroute		smallint,/* Autoroute */
		@vl_Sens             	smallint,/* Sens */
		@vl_Sens_inv		smallint,/* Sens inverse */
		@vl_Sens_Entree         smallint,/* Sens entrant */
		@vl_District		smallint,/* District */
		@vl_Position_Amont	smallint,/* Position de l'echangeur */
		@vl_Code_Retour		tinyint  /* Code retour fonction */

	/* Test Argument APPLIX  */
	if @va_Numero_in = 0    select @va_Numero_in	= null
	if @va_Cle_in = 0	select @va_Cle_in	= null
	if @va_PR_EVNT_in = 0	select @va_PR_EVNT_in	= null	

	/*A 
	** Controle des parametres en entree :
	*/
	if (@va_Numero_in       = null) or
	   (@va_Cle_in		= null) or
	   (@va_PR_EVNT_in	= null)
		return XDC_ARG_INV

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
