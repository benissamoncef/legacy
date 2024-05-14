/*E*/
/*  Fichier : $Id: xzac60sp.prc,v 1.7 2021/03/08 14:27:15 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2021/03/08 14:27:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac60sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	28/03/95	: Creation
* C.T.	16/01/98	: Ajout nommage des panneaux d'alerte, 
*                         fermeture d'acces et d'autoroute V1.2
* VR	12/12/11 	:	Ajout PRV v1.3 (DEM/1016)
* LCL	02/03/18	: Ajout controleurs de feux CTRL_FEUX 1.4 DEM1284
* LCL	16/05/18	: Ajout GABEB 1.5 DEM1283
* CGR	18/01/21	: Ajout IMU DEM-SAE155 1.7
* GGY	13/11/23	: Ajout PIC (DEM483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Renommage d'un equipement pour l'affichage ihm
* 
* Sequence d'appel
* SP	XZAC60_Nommage_Eqt
* 
* Arguments en entree
* XDY_Octet	va_Type_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* 
* Arguments en sortie
* char(50)	va_Libelle_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: type ou autoroute inconnu 
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* pour les PMV, NAV, TUB, ECH, FAC, FAU, PAL
* 
* Fonction
* format du libelle : "NomType NomAutoroute-sens-PR"
*                     PR : en km avec 3 decimales
------------------------------------------------------*/

use PRC
go

create procedure XZAC;60
	@va_Type_in		tinyint = null,
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PR_in		T_PR = null,
	@va_Sens_in		T_SENS = null,
	@va_Libelle_out		varchar(50) = null output
as
	declare	@vl_NomAutoroute char(4)

	/*A controle des arguments d'entree */
	if @va_Type_in is null or @va_Autoroute_in is null or 
	   @va_PR_in is null or @va_Sens_in is null
		return XDC_ARG_INV

	select @vl_NomAutoroute = nom from CFG..RES_AUT
	where numero = @va_Autoroute_in

	if @@rowcount = 0
		return XDC_NOK

	/*A construction du libelle de l'equipement */
	if @va_Type_in = XDC_EQT_ECHANGEUR
		select @va_Libelle_out = XDC_LIB_ECHANGEUR + ' ' + 
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_TUB
		select @va_Libelle_out = XDC_LIB_TUBE + ' ' + 
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_NAV
		select @va_Libelle_out = XDC_LIB_NAV + ' ' + 
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_PMV
		select @va_Libelle_out = XDC_LIB_PMV + ' ' + 
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)
				
	else if @va_Type_in = XDC_EQT_PRV
		select @va_Libelle_out = XDC_LIB_PRV + ' ' +
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_PIC
		select @va_Libelle_out = XDC_LIB_PIC + ' ' +
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_FAC
		select @va_Libelle_out = XDC_LIB_FAC + ' ' + 
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_FAU
		select @va_Libelle_out = XDC_LIB_PMV + ' ' + 
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)

	else if @va_Type_in = XDC_EQT_PAL
		select @va_Libelle_out = XDC_LIB_PAL + ' ' +
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)
				
	else if @va_Type_in = XDC_EQT_CFE
		select @va_Libelle_out = XDC_LIB_CFE + ' ' +
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)
				
	else if @va_Type_in = XDC_EQT_CAP
		select @va_Libelle_out = XDC_LIB_CAP + ' ' +
				convert(varchar(4), @vl_NomAutoroute) +
				'-' + convert(char(1), @va_Sens_in) +
				'-' + convert(char(7), @va_PR_in/ 1000.0)
	
	else if @va_Type_in = XDC_EQT_IMU
                select @va_Libelle_out = XDC_LIB_IMU + ' ' +
                                convert(varchar(4), @vl_NomAutoroute) +
                                '-' + convert(char(1), @va_Sens_in) +
                                '-' + convert(char(7), @va_PR_in/ 1000.0)


	else return XDC_NOK


	return XDC_OK
go
