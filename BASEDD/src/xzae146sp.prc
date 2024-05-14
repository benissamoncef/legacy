/*E*/
/*  Fichier : $Id: xzae146sp.prc,v 1.12 2015/09/01 08:53:10 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2015/09/01 08:53:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae146sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture des evenemets strada en base
------------------------------------------------------
* HISTORIQUE :
*
*  Penot	02 Dec 1997       : Creation
* Guilhou	02/12/97	: on travaille dans PRC !!! 1.3
* Niepceron	07/01/99	: LOC_RDS devient RDS dem/1724 1.4
* Niepceron     07/031/99        : correction v1.5
* CLuchague     22/06/2004       : suppression des caracteres de controle dans le nom 
				   par utilisation des substring dans les select v1.7
* Niepceron     23/06/08	: suppression des substring dans les select 
				  ajout de l'autoroute dans la clause having v1.8 1.9 DEM701
* JMG		15/02/11	: ajout nom autoroute en sortie datex V2 1.10
* JMG		22/04/14	: linux chaine sortie 1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP XZAE146_Recherche_Loc_Rds
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go
drop procedure XZAE146
go
create procedure XZAE146
	@va_pr    	T_PR,
	@va_autoroute	tinyint,
	@va_num_table	char(4) output,
	@va_pr_inf	T_PR 	output,
	@va_pr_sup 	T_PR 	output,
	@va_nom_inf 	char(50)	output,
	@va_nom_sup 	char(50)	output,
	@va_num_inf 	int output,
	@va_num_sup 	int	output,
	@va_nom_autoroute char(10) output
	
as

	select @va_nom_inf = XDC_CHAINE_VIDE,
		@va_nom_sup  = XDC_CHAINE_VIDE,
		@va_nom_autoroute = XDC_CHAINE_VIDE

	if (@va_pr = null  or @va_autoroute = null)
		return XDC_ARG_INV
		
	select @va_pr_inf=pr,@va_nom_inf=nom,@va_num_inf=numero from CFG..RDS 
		where pr <= @va_pr and autoroute = @va_autoroute having pr = max(pr) and autoroute = @va_autoroute 
			
	select @va_pr_sup = pr , @va_nom_sup =nom,@va_num_sup = numero  from  CFG..RDS
			where pr >= @va_pr and autoroute = @va_autoroute having pr = min(pr) and autoroute = @va_autoroute 
        /*if (@va_pr_sup = NULL) */
        if (@va_num_sup=0) OR (@va_num_sup=NULL)
	begin  
		select @va_pr_sup =@va_pr_inf,@va_nom_sup =@va_nom_inf,@va_num_sup =@va_num_inf
	end

	select @va_num_table=numero from CFG..NUM_RDS

	select @va_nom_autoroute=nom from CFG..RES_AUT where numero = @va_autoroute


	select @va_nom_inf,@va_nom_sup,@va_nom_autoroute,@va_num_table
	return XDC_OK
go

