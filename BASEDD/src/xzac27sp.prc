/*E*/
/*  Fichier : $Id: xzac27sp.prc,v 1.6 2021/03/08 14:36:22 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2021/03/08 14:36:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac27sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	15/09/94	: Creation
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* Guilhou 04/01/94	: compteur a 0 si pas d'actions (V 1.4)
* Niepceron 07/07/98	: separation EXP/HIS (dem/1696) v1.5
* CGR	06/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne le numero de la version actuelle de la liste des actions realisees
* 
* Sequence d'appel
* SP	XZAC27_Controler_Modif
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_Compteur_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Appele pour savoir s'il est utile de relire les actions avec XZAC21
* 
* Fonction
* Select compteur from EVENEMENT where numero, cle = NumEvt
---------------------------------------------*/

use PRC
go

create procedure XZAC;27
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Compteur_out	int		= null	output
as
declare		@vl_Status 	int,
		@vl_serveur_pra	char(6)

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A controle les parametres obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	/*A recherche du numero de version actuelle */
#ifdef HIST
	
	/* Init serveur PRA */
	select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero=XDC_CI

	if @vl_serveur_pra = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAC;27
			@va_NumEvt_in           ,
			@va_CleEvt_in           ,
			@va_Compteur_out output
	else
		exec @vl_Status = SQL_CI.PRC..XZAC;27
			@va_NumEvt_in		,
			@va_CleEvt_in		,
			@va_Compteur_out output
        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
#else
	select	@va_Compteur_out = valeur from EXP..FMC_ACT
	where numero = @va_NumEvt_in and cle = @va_CleEvt_in
#endif

	select	@va_Compteur_out = isnull(@va_Compteur_out,-1) 
	return XDC_OK
go
