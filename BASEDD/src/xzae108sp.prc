/*X  Fichier : $Id: xzae108sp.prc,v 1.8 2021/03/08 14:12:48 pc2dpdy Exp $     Release : $Revision: 1.8 $      Date : $Date: 2021/03/08 14:12:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae108sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* test de la connexion avec le CI 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	24/09/96	: Creation
* PN	04/06/1998	: Ajout site HIS 
* LCL   09/07/2007      : Suppression DY 
* JMG           02/10/09        : secto DEM887
* JMG		27/02/17: regio
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.6
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* CGR	06/01/21	: MOVIS ajout serveur PRA DEM-SAE93 1.8
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
* Sequence d'appel
* SP    XZAE108_Teste_Connexion_Site
*
* Arguments en entree
* @va_Site_A_Tester : site a tester
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

create procedure XZAE;108 
  @va_Site_A_Tester	T_SITE,
  @va_NomSiteLocal_in	char(2)		= null
as
	declare @vl_Site T_SITE,
		@vl_retcode  int,
		@vl_serveur_pra char(6)

	/*le site en entree ne peut pas etre nul*/
	if @va_Site_A_Tester = null
	  return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	/*si le site a tester est mon site local*/
	if @vl_Site = @va_Site_A_Tester
	  return XDC_OK
	else
	begin
		/* Init serveur PRA cable pour CI/DP/VC */
		select @vl_serveur_pra = serveur_pra from CFG..RES_PRA where numero = @va_Site_A_Tester
		
		/*execution distante*/
		if @va_Site_A_Tester = XDC_CI
			if @vl_serveur_pra = XDC_SQLDP
				exec @vl_retcode = SQL_DS.PRC..XZAE;108 @va_Site_A_Tester
			else
				exec @vl_retcode = SQL_CI.PRC..XZAE;108 @va_Site_A_Tester

		else if @va_Site_A_Tester = XDC_VC
			if @vl_serveur_pra = XDC_SQLCI
				exec @vl_retcode = SQL_CI.PRC..XZAE;108 @va_Site_A_Tester
			else
				exec @vl_retcode = SQL_DM.PRC..XZAE;108 @va_Site_A_Tester
		
		else if @va_Site_A_Tester = XDC_DP
			if @vl_serveur_pra = XDC_SQLVC
				exec @vl_retcode = SQL_DM.PRC..XZAE;108 @va_Site_A_Tester
			else
				exec @vl_retcode = SQL_DS.PRC..XZAE;108 @va_Site_A_Tester
		
		else if @va_Site_A_Tester = XDC_CA
			exec @vl_retcode = SQL_CA.PRC..XZAE;108 @va_Site_A_Tester

		else if @va_Site_A_Tester = XDC_HIS
			exec @vl_retcode = SQL_HIS.PRC..XZAE;108 @va_Site_A_Tester
		else
			return XDC_NOK

		return @vl_retcode
	end

	return XDC_OK
go
