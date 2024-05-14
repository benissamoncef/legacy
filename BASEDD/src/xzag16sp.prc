/*E*/
/*  Fichier : $Id: xzag16sp.prc,v 1.8 2018/04/19 14:04:43 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2018/04/19 14:04:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/06/95	: Creation	(V 1.1)
* P.N.	27/01/99	: Ajouter SAD_EXP (V1.19) dem/1724
* JMG	27/04/04	: ajout purge SAD_PAL
* PNI	12/07/06	: ajout PMA, BAF,BAD dem576 v1.5
* VR	14/12/11 	: Ajout PRV (DEM/1016)
* JMG	09/06/17 	: ajout EQT_SAGA
* LCL	02/03/18	: ajout controleurs de feux CTRL_FEUX 1.8 DEM1284
* GGY	28/02/23	: ajout purge SAD_IMU et SAD_SONO (SAE-DEM462)
* GGY	11/12/23	: Ajout PIC (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Purge des plans d'actions de la base d'exploitation datant
* d'au moins la date precisee
* 
* Sequence d'appel
* SP	XZAG16_Purge_Plans_Actions_Base_Exploitation
* 
* Arguments en entree
* XDY_Datetime	va_Horodate_in	: date de fin de purge
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametre d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Suppression des lignes vielles de plus d'un mois
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;16
	@va_Horodate_in		datetime = null
as
	/*A controle du parametre en entree */
	if @va_Horodate_in = null
		return XDC_ARG_INV

	/*A Rechercher la liste de tous les SAD datant d'au moins */
	/*A la date donnee                                        */
	select numero
	into #PLN_SUP
	from EXP..SAD_PLN 
	where	horodate <= @va_Horodate_in

	/*A Effacer la partie generique des plans d'actions et */
	/*A le complement associe lies a cet evenement         */
	begin tran
	delete	EXP..SAD_APL
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_DST_FAX
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_ECH 
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_FAX
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_MSG_TFM
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_NAV
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	delete  EXP..SAD_PAL
        where   numero in (select numero from #PLN_SUP)
        commit tran
	begin tran
	delete	EXP..SAD_PMV
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_PMA
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_BAF
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_BAD
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_PRV
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_PIC
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_CFE
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TFM 
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TUB
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TXT_FAX
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete	EXP..SAD_TXT_TFM
	where	numero in (select numero from #PLN_SUP) 
	commit tran
	begin tran
	delete EXP..SAD_SAGA
	where   numero in (select numero from #PLN_SUP)
	commit tran
	begin tran
	delete EXP..SAD_IMU
	where   numero in (select numero from #PLN_SUP)
	commit tran
	begin tran
	delete EXP..SAD_SONO
	where   numero in (select numero from #PLN_SUP)
	commit tran
#ifdef CI
	begin tran
	delete	EXP..SAD_EXP
	where	numero in (select numero from #PLN_SUP) 
	commit tran
#endif
	begin tran
	delete	EXP..SAD_PLN
	where	numero in (select numero from #PLN_SUP) 
	commit tran


	return XDC_OK
go
